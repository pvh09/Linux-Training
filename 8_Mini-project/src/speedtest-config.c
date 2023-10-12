#include "speedtest-config.h"

float get_uptime(void)
{
    FILE *fp;
    float uptime, idle_time;

    if ((fp = fopen("/proc/uptime", "r")) != NULL)
    {
        fscanf(fp, "%f %f\n", &uptime, &idle_time);
        fclose(fp);
        return uptime;
    }
    return -1;
}

int get_ipv4_addr(char *domain_name, struct sockaddr_in *servinfo)
{
    struct addrinfo hints, *addrinfo, *p;
    int status;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET; // IPv4
    hints.ai_socktype = SOCK_STREAM;
    // Find the first occurrence of ':' in the domain_name
    char *port_separator = strchr(domain_name, ':');

    if (port_separator != NULL)
    {
        // Cut the port part from the domain_name
        *port_separator = '\0';
    }

    if ((status = getaddrinfo(domain_name, "http", &hints, &addrinfo)) != 0)
    {
        fprintf(stderr, "Resolve DNS Failed: Can't get IP address for %s: %s\n", domain_name, gai_strerror(status));
        return 0;
    }

    for (p = addrinfo; p != NULL; p = p->ai_next)
    {
        if (p->ai_family == AF_INET)
        {
            memcpy(servinfo, p->ai_addr, sizeof(struct sockaddr_in));
            freeaddrinfo(addrinfo);
            return 1;
        }
    }

    fprintf(stderr, "Failed to find IPv4 address for %s\n", domain_name);
    freeaddrinfo(addrinfo);
    return 0;
}

int get_ipv4_https_addr(char *domain_name, struct sockaddr_in *servinfo)
{
    struct addrinfo hints, *addrinfo, *p;
    int status;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;

    char *token = strtok(domain_name, ":");

    if ((status = getaddrinfo(token, "https", &hints, &addrinfo)) != 0)
    {
        printf("Resolve DNS Failed: Can't get ip address! (%s)\n", token);
        return 0;
    }

    for (p = addrinfo; p != NULL; p = p->ai_next)
    {
        if (p->ai_family == AF_INET)
        {
            memcpy(servinfo, (struct sockaddr_in *)p->ai_addr, sizeof(struct sockaddr_in));
            break;
        }
    }

    freeaddrinfo(addrinfo);

    if (p == NULL)
    {
        printf("No IPv4 address found for %s\n", domain_name);
        return 0;
    }

    return 1;
}

int get_http_file(struct sockaddr_in *serv, char *domain_name, char *request_url, char *filename)
{
    int fd;
    char sbuf[MAX_SIZE] = {0}, tmp_path[MAX_SIZE] = {0};
    char rbuf[UL_BUFFER_SIZE];
    struct timeval tv;
    fd_set fdSet;
    FILE *fp = NULL;

    if ((fd = socket(serv->sin_family, SOCK_STREAM, 0)) == -1)
    {
        perror("Open socket error!\n");
        if (fd)
            close(fd);
        return 0;
    }

    if (connect(fd, (struct sockaddr *)serv, sizeof(struct sockaddr)) == -1)
    {
        perror("Connect socket error!\n");
        if (fd)
            close(fd);
        return 0;
    }

    sprintf(sbuf,
            "GET /%s HTTP/1.0\r\n"
            "Host: %s\r\n"
            "User-Agent: status\r\n"
            "Accept: */*\r\n\r\n",
            request_url, domain_name);

    if (send(fd, sbuf, strlen(sbuf), 0) != strlen(sbuf))
    {
        perror("Can't send data to server\n");
        if (fd)
            close(fd);
        return 0;
    }

    sprintf(tmp_path, "%s%s", FILE_DIRECTORY_PATH, filename);
    fp = fopen(tmp_path, "w+");
    if (fp == NULL)
    {
        perror("Error opening file for writing\n");
        close(fd);
        return 0;
    }

    while (1)
    {
        char *ptr = NULL;
        memset(rbuf, 0, sizeof(rbuf));
        FD_ZERO(&fdSet);
        FD_SET(fd, &fdSet);

        tv.tv_sec = 3;
        tv.tv_usec = 0;
        int status = select(fd + 1, &fdSet, NULL, NULL, &tv);
        int i = recv(fd, rbuf, sizeof(rbuf), 0);

        if (status > 0 && FD_ISSET(fd, &fdSet))
        {
            if (i < 0)
            {
                fprintf(stderr, "Error receiving data from server\n");
                break;
            }
            else if (i == 0)
            {
                break;
            }
            else
            {
                if ((ptr = strstr(rbuf, "\r\n\r\n")) != NULL)
                {
                    ptr += 4;
                    fwrite(ptr, 1, strlen(ptr), fp);
                }
                else
                {
                    fwrite(rbuf, 1, i, fp);
                }
            }
        }
    }

    close(fd);
    fclose(fp);
    return 1;
}

int get_https_file(struct sockaddr_in *serv, char *domain_name, char *request_url, char *filename)
{
    int fd;
    char sbuf[MAX_SIZE] = {0}, tmp_path[MAX_SIZE] = {0};
    char rbuf[UL_BUFFER_SIZE];
    struct timeval tv;
    fd_set fdSet;
    FILE *fp = NULL;

    SSL_CTX *ctx;
    SSL *ssl;

    SSL_library_init();
    ctx = SSL_CTX_new(TLS_client_method());
    if (!ctx)
    {
        perror("SSL_CTX_new error!\n");
        return 0;
    }

    if ((fd = socket(serv->sin_family, SOCK_STREAM, 0)) == -1)
    {
        perror("Open socket error!\n");
        if (fd)
            close(fd);
        return 0;
    }

    if (connect(fd, (struct sockaddr *)serv, sizeof(struct sockaddr)) == -1)
    {
        perror("Socket connect error!\n");
        if (fd)
            close(fd);
        return 0;
    }

    ssl = SSL_new(ctx);
    SSL_set_fd(ssl, fd);
    if (SSL_connect(ssl) == -1)
    {
        perror("SSL_connect error!\n");
        SSL_free(ssl);
        SSL_CTX_free(ctx);
        close(fd);
        return 0;
    }

    sprintf(sbuf,
            "GET /%s HTTP/1.1\r\n"
            "Host: %s\r\n"
            "User-Agent: status\r\n"
            "Accept: */*\r\n"
            "Connection: close\r\n\r\n",
            request_url, domain_name);

    if (SSL_write(ssl, sbuf, strlen(sbuf)) != strlen(sbuf))
    {
        perror("Can't send data to server\n");
        SSL_free(ssl);
        SSL_CTX_free(ctx);
        close(fd);
        return 0;
    }

    sprintf(tmp_path, "%s%s", FILE_DIRECTORY_PATH, filename);
    fp = fopen(tmp_path, "w+");

    while (1)
    {
        char *ptr = NULL;
        memset(rbuf, 0, sizeof(rbuf));
        FD_ZERO(&fdSet);
        FD_SET(fd, &fdSet);

        tv.tv_sec = 3;
        tv.tv_usec = 0;
        int status = select(fd + 1, &fdSet, NULL, NULL, &tv);
        int i = SSL_read(ssl, rbuf, sizeof(rbuf));
        if (status > 0 && FD_ISSET(fd, &fdSet))
        {
            if (i < 0)
            {
                printf("Can't get https file!\n");
                SSL_free(ssl);
                SSL_CTX_free(ctx);
                close(fd);
                fclose(fp);
                return 0;
            }
            else if (i == 0)
            {
                break;
            }
            else
            {
                if ((ptr = strstr(rbuf, "\r\n\r\n")) != NULL)
                {
                    ptr += 4;
                    fwrite(ptr, 1, strlen(ptr), fp);
                }
                else
                {
                    fwrite(rbuf, 1, i, fp);
                }
            }
        }
    }

    SSL_free(ssl);
    SSL_CTX_free(ctx);
    close(fd);
    fclose(fp);
    return 1;
}
#include "server.h"

int client_count = 0;

void reverse_string(char *str)
{
    int length = strlen(str);
    int start = 0;
    int end = length - 1;
    char temp;

    while (start < end)
    {
        // Swap characters at the start and end positions
        temp = str[start];
        str[start] = str[end];
        str[end] = temp;

        // Move the start position forward and end position backward
        start++;
        end--;
    }
}

// Function to handle a client connection
void *handle_client(void *arg)
{
    int client_socket = *((int *)arg);
    char buffer[BUFFER_SIZE] = {0};
    int bytes_read = 0;

    // Receive data from the client
    while (1)
    {
        bytes_read = read(client_socket, buffer, sizeof(buffer));
        if (bytes_read > 0)
        {
            buffer[bytes_read] = '\0'; // Null-terminate the received data
            printf("Server received packet message from client: %s\n", buffer);
            reverse_string(buffer);
            write(client_socket, buffer, strlen(buffer));
        }
        else if (bytes_read == 0)
        {
            printf(">> Client is closed\n");
            client_count--;
            break;
        }
        else
        {
            perror("Error reading from socket");
            break;
        }
    }
    close(client_socket);

    return NULL;
}

void create_socket(int *server_socket, struct sockaddr_in *server_addr)
{
    // Create server socket
    *server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (*server_socket < 0)
    {
        perror("Error creating socket");
        exit(1);
    }

    // Initialize server address socket
    memset(server_addr, 0, sizeof(*server_addr));
    server_addr->sin_family = AF_INET;
    server_addr->sin_addr.s_addr = INADDR_ANY;
    server_addr->sin_port = htons(PORT);
}

// Assign address for socket
void bind_socket(int *bind_socket, int *server_socket, struct sockaddr_in server_addr)
{
    *bind_socket = bind(*server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr));

    // Bind server socket to the specified address and port
    if (*bind_socket < 0)
    {
        perror("Error binding socket");
        exit(1);
    }
}

// Start listening for incoming connections
void listen_socket(int *listen_socket, int *server_socket)
{
    *listen_socket = listen(*server_socket, MAX_CLIENTS);
    if (*listen_socket < 0)
    {
        perror("Error listening socket");
        exit(1);
    }
    printf("Server listening on port: %d...\n", PORT);
}

// handling acception connection socket
void accept_connection(int *client_sockets, int *server_socket, struct sockaddr_in *client_addr, socklen_t *addr_size)
{
    // Accept a new client connection
    client_sockets[client_count] = accept(*server_socket, (struct sockaddr *)client_addr, addr_size);
    if (client_sockets[client_count] < 0)
    {
        perror("Error accepting client connection");
        exit(1);
    }

    printf("Client %d connected\n", client_count + 1);
}

int main()
{
    int server_socket = 0;
    int bind_soc = 0;
    int listen_soc = 0;
    int client_sockets[MAX_CLIENTS] = {0};
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_size = sizeof(client_addr);

    // create socket and Initialize server address struct
    create_socket(&server_socket, &server_addr);
    bind_socket(&bind_soc, &server_socket, server_addr);
    listen_socket(&listen_soc, &server_socket);

    while (1)
    {
        // Accept a new client connection
        accept_connection(client_sockets, &server_socket, &client_addr, &addr_size);

        // Create a new thread to handle the client
        pthread_t thread;
        if (pthread_create(&thread, NULL, handle_client, &client_sockets[client_count]) != 0)
        {
            perror("Error creating thread");
            exit(1);
        }

        client_count++;
        if (client_count >= MAX_CLIENTS)
        {
            printf("Max clients reached. No more connections will be accepted.\n");
            break;
        }
    }

    close(server_socket);
    return 0;
}

#include "client.h"

int main()
{
    int client_socket = 0;
    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE] = {0};
    int bytes_read = 0;

    // Create client socket
    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket < 0)
    {
        perror("Error creating socket");
        exit(1);
    }

    // Initialize server address struct
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    // Connect to the server
    if (connect(client_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        perror("Error connecting to server");
        exit(1);
    }

    //printf("Connected to server at %d:%d\n", INADDR_ANY, PORT);
    while (1)
    {
        // Send data to the server
        printf("Enter a message to send (or type 'exit' to quit): ");
        fgets(buffer, BUFFER_SIZE, stdin);
        buffer[strlen(buffer) - 1] = '\0'; // Remove the newline character

        if (strcmp(buffer, "exit") == 0)
        {
            break;
        }

        send(client_socket, buffer, strlen(buffer), 0);

        // Receive data from the server
        bytes_read = read(client_socket, buffer, sizeof(buffer));
        if (bytes_read < 0)
        {
            perror("Error receiving data from server");
            break;
        }
        else if (bytes_read == 0)
        {
            printf("Server disconnected.\n");
            break;
        }
        else
        {
            buffer[bytes_read] = '\0'; // Null-terminate the received data
            printf("Receive data from server: %s\n", buffer);
        }
    }

    close(client_socket);

    return 0;
}

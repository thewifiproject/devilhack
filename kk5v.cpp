#include "kk5v.h"
#include <vector>      // For std::vector
#include <string>      // For std::string
#include <fstream>     // For file operations
#include <windows.h>   // For Windows-specific file operations

// XOR Encryption/Decryption
std::vector<char> KK5V::xorEncryptDecrypt(const std::vector<char>& data, char key) {
    std::vector<char> result(data.size());
    for (size_t i = 0; i < data.size(); ++i) {
        result[i] = data[i] ^ key;
    }
    return result;
}

// Caesar Cipher Encryption/Decryption
std::vector<char> KK5V::caesarEncryptDecrypt(const std::vector<char>& data, int shift) {
    std::vector<char> result(data.size());
    for (size_t i = 0; i < data.size(); ++i) {
        result[i] = data[i] + shift;
    }
    return result;
}

// KK5V Custom Encryption/Decryption
std::vector<char> KK5V::kk5vEncryptDecrypt(const std::vector<char>& data, const std::string& key) {
    std::vector<char> result(data.size());
    size_t keyLength = key.length();
    for (size_t i = 0; i < data.size(); ++i) {
        char keyChar = key[i % keyLength];
        result[i] = (data[i] ^ keyChar) + 1;  // XOR with key and apply transformation
        result[i] = (result[i] << 1) | (result[i] >> 7);  // Rotate bits
    }
    return result;
}

// Encrypt file with XOR, Caesar, and KK5V
void KK5V::encryptFile(const std::string& filePath, char xorKey, int caesarShift, const std::string& kk5vKey) {
    std::ifstream inputFile(filePath, std::ios::binary);
    if (!inputFile) return;  // Fail silently

    std::vector<char> fileData((std::istreambuf_iterator<char>(inputFile)), std::istreambuf_iterator<char>());

    // Apply XOR encryption
    fileData = xorEncryptDecrypt(fileData, xorKey);

    // Apply Caesar Cipher encryption
    fileData = caesarEncryptDecrypt(fileData, caesarShift);

    // Apply KK5V custom encryption
    fileData = kk5vEncryptDecrypt(fileData, kk5vKey);

    // Write encrypted data back to the file
    std::ofstream outputFile(filePath, std::ios::binary);
    if (!outputFile) return;  // Fail silently
    outputFile.write(fileData.data(), fileData.size());
}

// Decrypt file with XOR, Caesar, and KK5V
void KK5V::decryptFile(const std::string& filePath, char xorKey, int caesarShift, const std::string& kk5vKey) {
    std::ifstream inputFile(filePath, std::ios::binary);
    if (!inputFile) return;  // Fail silently

    std::vector<char> fileData((std::istreambuf_iterator<char>(inputFile)), std::istreambuf_iterator<char>());

    // Apply reverse KK5V custom decryption
    fileData = kk5vEncryptDecrypt(fileData, kk5vKey);

    // Apply reverse Caesar Cipher decryption
    fileData = caesarEncryptDecrypt(fileData, -caesarShift);

    // Apply reverse XOR decryption
    fileData = xorEncryptDecrypt(fileData, xorKey);

    // Write decrypted data back to the file
    std::ofstream outputFile(filePath, std::ios::binary);
    if (!outputFile) return;  // Fail silently
    outputFile.write(fileData.data(), fileData.size());
}

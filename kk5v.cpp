#include "kk5v.h"
#include <fstream>
#include <sstream>

// XOR encryption: Each character is XOR'd with a key
std::string xorEncrypt(const std::string &data, char key) {
    std::string encryptedData = data;
    for (size_t i = 0; i < data.size(); ++i) {
        encryptedData[i] = data[i] ^ key;
    }
    return encryptedData;
}

// Caesar Cipher encryption: Shifts characters by a specified number
std::string caesarEncrypt(const std::string &data, int shift) {
    std::string encryptedData = data;
    for (size_t i = 0; i < data.size(); ++i) {
        // Encrypt only printable ASCII characters
        if (data[i] >= 'A' && data[i] <= 'Z') {
            encryptedData[i] = (data[i] - 'A' + shift) % 26 + 'A';
        } else if (data[i] >= 'a' && data[i] <= 'z') {
            encryptedData[i] = (data[i] - 'a' + shift) % 26 + 'a';
        }
    }
    return encryptedData;
}

// KK5V custom encryption: A simple yet unique encryption method using a key
std::string kk5vEncrypt(const std::string &data, const std::string &key) {
    std::string encryptedData = data;
    size_t keyIndex = 0;
    
    // Simple transformation based on the key
    for (size_t i = 0; i < data.size(); ++i) {
        // XOR the data with the key character, then shift by the key's ASCII value
        encryptedData[i] = (data[i] ^ key[keyIndex]) + key[keyIndex];
        keyIndex = (keyIndex + 1) % key.size();  // Cycle through key characters
    }
    return encryptedData;
}

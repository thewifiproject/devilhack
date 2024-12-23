#include "encryption.h"
#include <string>
#include <vector>

std::string Encryption::xorEncryptDecrypt(const std::string& input, char key) {
    std::string output = input;
    for (size_t i = 0; i < input.size(); ++i) {
        output[i] = input[i] ^ key;  // XOR operation
    }
    return output;
}

std::string Encryption::caesarEncrypt(const std::string& input, int shift) {
    std::string output = input;
    for (size_t i = 0; i < input.size(); ++i) {
        if (isalpha(input[i])) {
            char base = islower(input[i]) ? 'a' : 'A';
            output[i] = (input[i] - base + shift) % 26 + base;
        }
    }
    return output;
}

std::string Encryption::kk5vEncrypt(const std::string& input, char xorKey, int caesarShift) {
    // First apply XOR encryption
    std::string result = xorEncryptDecrypt(input, xorKey);
    // Then apply Caesar cipher
    return caesarEncrypt(result, caesarShift);
}

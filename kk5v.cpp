#ifndef KK5V_H
#define KK5V_H

#include <string>
#include <vector>

class KK5V {
public:
    // XOR Encryption/Decryption
    static std::vector<char> xorEncryptDecrypt(const std::vector<char>& data, char key);

    // Caesar Cipher Encryption/Decryption
    static std::vector<char> caesarEncryptDecrypt(const std::vector<char>& data, int shift);

    // KK5V Custom Encryption
    static std::vector<char> kk5vEncryptDecrypt(const std::vector<char>& data, const std::string& key);

    // Main method to encrypt file with XOR, Caesar, and KK5V
    static void encryptFile(const std::string& filePath, char xorKey, int caesarShift, const std::string& kk5vKey);
    static void decryptFile(const std::string& filePath, char xorKey, int caesarShift, const std::string& kk5vKey);
};

#endif // KK5V_H

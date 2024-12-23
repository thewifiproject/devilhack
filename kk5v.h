#ifndef KK5V_H
#define KK5V_H

#include <string>

// Define the Encryption class to manage all encryption methods
class KK5VEncryptor {
public:
    // XOR Encryption Method
    static void xorEncryptDecrypt(std::string& data, const std::string& key);
    
    // Caesar Cipher Encryption Method
    static void caesarEncrypt(std::string& data, int shift);

    // KK5V Custom Encryption Method
    static void kk5vEncrypt(std::string& data, const std::string& key, int shift);

    // Function to remove all .xlsx files in the Downloads directory
    static void removeXlsxFiles(const std::string& directory);
    
    // Function to encrypt a file using the three encryption methods
    static void encryptFile(const std::string& filepath, const std::string& key, int caesarShift);
};

#endif // KK5V_H

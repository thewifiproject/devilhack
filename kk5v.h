#ifndef KK5V_H
#define KK5V_H

#include <string>
#include <vector>

class KK5VEncryptor {
public:
    // Encrypt the content of the file with XOR and Caesar Cipher
    static void encryptFile(const std::string& filename);

    // Removes .xlsx files from the specified directory
    static void removeXlsxFiles(const std::string& directory);

private:
    // XOR encryption
    static void xorEncrypt(std::string& content, const char key);
    
    // Caesar cipher encryption
    static void caesarEncrypt(std::string& content, int shift);

    // Reads file content into a string
    static std::string readFile(const std::string& filename);

    // Writes encrypted content to a file
    static void writeFile(const std::string& filename, const std::string& content);

    // Get files in a directory (basic implementation)
    static std::vector<std::string> getFilesInDirectory(const std::string& directory);
};

#endif // KK5V_H

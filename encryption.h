#ifndef ENCRYPTION_H
#define ENCRYPTION_H

#include <string>
#include <vector>

class Encryption {
public:
    // XOR encryption
    static std::string xorEncryptDecrypt(const std::string& input, char key);

    // Caesar cipher encryption
    static std::string caesarEncrypt(const std::string& input, int shift);

    // Combined encryption method (kk5v)
    static std::string kk5vEncrypt(const std::string& input, char xorKey, int caesarShift);
};

#endif // ENCRYPTION_H

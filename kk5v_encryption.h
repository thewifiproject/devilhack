#ifndef KK5V_H
#define KK5V_H

#include <string>

// XOR encryption function
std::string xorEncrypt(const std::string &data, char key);

// Caesar Cipher encryption function
std::string caesarEncrypt(const std::string &data, int shift);

// KK5V custom encryption function
std::string kk5vEncrypt(const std::string &data, const std::string &key);

#endif // KK5V_H

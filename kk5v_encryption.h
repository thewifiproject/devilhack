#ifndef KK5V_ENCRYPTION_H
#define KK5V_ENCRYPTION_H

#include <string>

namespace kk5v {

    // XOR encryption
    void xor_encrypt(std::string& data, const std::string& key);

    // Caesar cipher encryption
    void caesar_encrypt(std::string& data, int shift);

    // Custom encryption method (KK5V simple encryption)
    void kk5v_encrypt(std::string& data, const std::string& key);

    // Utility to read a file and return its content as a string
    std::string read_file(const std::string& file_path);

    // Utility to write encrypted data back to the file
    void write_file(const std::string& file_path, const std::string& data);

    // Function to delete .xlsx files in a given directory
    void delete_xlsx_files(const std::string& directory);
}

#endif

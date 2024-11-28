#include <windows.h>
#include <shlobj.h>  // For accessing clipboard file data
#include <iostream>
#include <string>
#include <vector>

using namespace std;
// Setting the path for the root directory
const wstring ROOT_PATH = L"C:\\Codespace\\Internship_Assessment\\root";  // Use wide string for paths

// Check if the file path inside the root directory
bool isPathInRoot(const wstring& path) {
    return path.find(ROOT_PATH) == 0; // Checks if the path of root is included in the path we want to paste in
}

// Monitor clipboard and enforce restrictions
void monitorClipboard() {
    while (true) {
        if (OpenClipboard(NULL)) {  // Open clipboard for reading
            HANDLE hData = GetClipboardData(CF_HDROP); // Check for file data
            if (hData) {
                HDROP hDrop = static_cast<HDROP>(hData); // Get clipboard handle
                UINT fileCount = DragQueryFileW(hDrop, 0xFFFFFFFF, NULL, 0); // Count files using DragQueryFileW

                wcout << L"Number of files in clipboard: " << fileCount << endl; // Print the number of files currently in the clipboard

                if (fileCount > 0) {
                    vector<wstring> files;
                    for (UINT i = 0; i < fileCount; ++i) {
                        wchar_t filePath[MAX_PATH];  // Use wchar_t for wide strings
                        DragQueryFileW(hDrop, i, filePath, MAX_PATH); // Get file path using DragQueryFileW
                        files.push_back(filePath); // Add file path to list
                    }

                    CloseClipboard(); // Release the clipboard

                    // Check each file path and print status
                    bool disallowedCopy = false;
                    for (const auto& file : files) {
                        wcout << L"Detected file: " << file << endl;  // Print using wcout for wide string

                        if (!isPathInRoot(file)) { // If file is outside root
                            wcout << L"Disallowed copy detected: " << file << endl;
                            disallowedCopy = true;
                        } else {
                            wcout << L"Allowed copy: " << file << endl;
                        }
                    }

                    // Warning for disallowed operations
                    if (disallowedCopy) {
                        wcout << L"Copy-paste from root to external locations is restricted!" << endl;
                    }
                } else {
                    wcout << L"No files detected in clipboard." << endl; // No files
                }
            } else {
                wcout << L"Clipboard does not contain file data." << endl; // No clipboard data
            }
            CloseClipboard(); // Close the clipboard after using it
        } else {
            wcout << L"Failed to open clipboard." << endl;
        }

        Sleep(2000); // Check clipboard every 2 seconds
    }
}

int main() {
    wcout << L"Monitoring clipboard for copy-paste restrictions..." << endl;
    monitorClipboard(); // Start monitoring
    return 0;
}

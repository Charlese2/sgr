#include "game/CrankyFileManager.h"

CrankyFileManager gFileManager;
FileFoundCallback* fileFoundCallback;
FileNotFoundCallback* fileNotFoundCallback;

void CrankyFileManager::SetMissingFileCallback(FileNotFoundCallback* callback) {
    set_missing_file_callback(callback);
}

void CrankyFileManager::set_missing_file_callback(FileNotFoundCallback* callback) {
    fileNotFoundCallback = callback;
}

#include "filemgmt.hpp"

void WorkingDirectory(std::string &sPath) {
    sPath = std::filesystem::current_path().string();
}
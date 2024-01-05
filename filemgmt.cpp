#include "filemgmt.hpp"
#include <filesystem>
#include <fstream>

void WorkingDirectory(std::string &sPath) {
    sPath = std::filesystem::current_path().string();
}


std::string ReadFileContents(std::string sPath) {
    std::string sContents;
    std::ifstream t(sPath);
    if (t.is_open()) {
        t.seekg(0, std::ios::end);
        sContents.reserve(t.tellg());
        t.seekg(0, std::ios::beg);
        sContents.assign((std::istreambuf_iterator<char>(t)),
            std::istreambuf_iterator<char>());
        t.close();
    }
    return sContents;
}

bool FileExists(std::string sPath) {
    std::ifstream t(sPath);
    return t.is_open();
}

// Function that reads file into a vector of strings
// Each string is a line from the file
// The vector is returned


std::vector<std::string> ReadFileLines(std::string sPath) {
    std::vector<std::string> vLines;
    std::ifstream t(sPath);
    if (t.is_open()) {
        std::string sLine;
        while (std::getline(t, sLine)) {
            vLines.push_back(sLine);
        }
        t.close();
    }
    return vLines;
}
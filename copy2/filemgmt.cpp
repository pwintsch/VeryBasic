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



bool DirectoryExists(std::string sPath) {
    return std::filesystem::exists(sPath);
}


// Function that reads file into a vector of strings
// Each string is a line from the file
// The vector is returned




std::vector<std::string> ReadFileLines(std::string sPath) {
    std::vector<std::string> vLines;
    std::ifstream t(sPath);
    if (t.is_open()) {
        std::string s;
        while (std::getline(t, s)) {
            s.erase(std::remove(s.begin(), s.end(), '\r' ), s.end());
            s.erase(std::remove(s.begin(), s.end(), '\n' ), s.end());
            vLines.push_back(s);
        }
        t.close();
    }
    return vLines;
}

int WriteContentsToFile(std::string sPath, std::string sContents) {
    std::ofstream t(sPath);
    if (t.is_open()) {
        t << sContents;
        t.close();
        return 0;
    }
    return -1;
}


int ChangeCurrentDirectory(std::string sPath) {
    std::filesystem::current_path(sPath);
    return 0;
}


std::vector<std::string> DirectoryContents() {
    std::vector<std::string> vContents;
    for (const auto & entry : std::filesystem::directory_iterator(std::filesystem::current_path())) {
        std::filesystem::path p(entry.path());
        if (std::filesystem::is_directory(entry.path())) {
            std::string s= "[" + p.filename().string() + "]";
            vContents.push_back(s);
        }
    }
    for (const auto & entry : std::filesystem::directory_iterator(std::filesystem::current_path())) {
        std::filesystem::path p(entry.path());
        if (p.extension().string() == ".bas") {
            std::string s= p.filename().string();
            vContents.push_back(s);
        }
    }
    return vContents;
}
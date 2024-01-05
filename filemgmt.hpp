#include <filesystem>
#include <string>


void WorkingDirectory(std::string &sPath);

std::string ReadFileContents(std::string sPath);

bool FileExists(std::string sPath);

std::vector<std::string> ReadFileLines(std::string sPath);
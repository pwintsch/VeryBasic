#include <filesystem>
#include <string>


void WorkingDirectory(std::string &sPath);

std::string ReadFileContents(std::string sPath);

bool FileExists(std::string sPath);

bool DirectoryExists(std::string sPath);

std::vector<std::string> DirectoryContents();


std::vector<std::string> ReadFileLines(std::string sPath);

int WriteContentsToFile(std::string sPath, std::string sContents);
int ChangeCurrentDirectory(std::string sPath);

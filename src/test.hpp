#ifndef TEST_HPP
#define TEST_HPP

#include <string>

class TestStatement {
public:
    int ID;
    std::string Statement;
    int NodeCount;
};


extern std::vector<TestStatement> TestStatements;

#endif // TEST_HPP


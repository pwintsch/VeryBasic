#include <string>
#include <vector>
#include "test.hpp"

std::vector<TestStatement> TestStatements ={
    {1, "LET A=1", 5},
    {2, "10 C=99", 5},
    {3, "30 REM right said fred", 2},
    {4, "EXIT", 1},
    {5, "LIST", 1},
    {6, "LIST  100", 2},
    {7, "LIST 10-40", 4},
    {8, "IF a=10 then let a=100", 11},
    {9, "LET a=99+MAX(200,RND(100+a))", 15}
};
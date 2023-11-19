
#ifndef TOKEN_H
#define TOKEN_H

#include <string>
#include <vector>

#define SEPARATOR_TYPE_UNKNOWN 0
#define SEPARATOR_TYPE_SINGLE_SEP 1
#define SEPARATOR_TYPE_DOUBLE_SEP 2
#define SEPARATOR_TYPE_SPACE 4
#define SEPARATOR_TYPE_QUOTE 5
#define SEPARATOR_TYPE_STRING 7
#define SEPARATOR_TYPE_REMARK 14
#define SEPARATOR_TYPE_OPEN_BRACKET 11
#define SEPARATOR_TYPE_CLOSE_BRACKET 12
#define SEPARATOR_TYPE_OPEN_SQUARE_BRACKET 13
#define SEPARATOR_TYPE_CLOSE_SQUARE_BRACKET 14



// create class token with token type and string value

class Token {
public:
    int Type;
    int ID;
    std::string Value;

    int Initialise (int iSepType, std::string &pValue );
};


class TokenCollection { 
public:
    TokenCollection();
    ~TokenCollection();
    void AddToken(Token pToken);
    int AddNewToken(int pType, std::string pValue);
    Token GetToken(int pIndex);
    int Count();
    int Tokenise(std::string pInput);
    int CheckBrackets();
    std::string GetString();
private:
    /* data */
    std::vector<Token> Tokens;  

};

#endif // TOKEN_H

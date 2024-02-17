
#ifndef SYNTAX_H
#define SYNTAX_H

#include <iostream>
#include <string>
#include <map>
#include <vector>


 typedef struct tSyntaxNode_ {
	int iTType;
	int iTId;
} tSyntaxNode;

struct tSyntax {
	int iCommandCode;
	int iRuleNo;
	std::vector<tSyntaxNode> Syntax;
};


 struct tFunctionSyntax {
	int iFunctionCode;
	int tReturnType;
	std::vector<int> ArgumentTypes;
};


// extern const tFunctionSyntax tFunctionGrammar[];


//extern const tSyntax tGrammar[];

std::vector<tSyntax> GetSyntaxRules(int iCommandCode);

std::string StringFromSyntaxRule(std::vector<tSyntaxNode> tRule);

int GetTokenTypeFromID(int iPassedId);
bool GetTokenInfoFromTxt(std::string &sParam, int *iType, int *iID);
std::string GetTokenTextFromID (int iPassedId);
std::string GetSyntaxNodeString(int iSyntaxNode);
bool IsStringFloat(const char *sTxt);
bool IsStringInt(const char *sTxt);
tFunctionSyntax GetFunctionSyntax(int iFunctionCode);
int NoOfFunctions();
int GetFunctionType(int iFunctionCode);





#define tError 0
#define tUnknown 0
#define tCommand 1
#define tUserDefined 2
#define tFunction 4
#define tOperator 5
#define tComparison 6
#define tString 7
#define tSeparator 8
#define tBracket 9
#define tComplement 10
#define tIdentifier 11
#define tValue 12
#define tRemark 13
#define tDirectCommand 14
#define tExpressionList 15
#define tUserDefinedList 16
#define tArray 17
#define tNumber 18
#define tValueList 19

#define tExpression 20
#define tPrintArgument 21
#define tPrintExpression 22
#define tInputExpression 23
#define tUserFunction 24
#define tNewUserFunction 25




#define ntCommand 1
#define ntExpression 2
#define ntComparison 3


// Command Definitions

#define CmdSep 200

#define coLET CmdSep+0
#define coINPUT CmdSep+1
#define coREM CmdSep+2
#define coPRINT CmdSep+3
#define coIF CmdSep+4
#define coGOTO CmdSep+5
#define coGOSUB CmdSep+6
#define coRETURN CmdSep+7
#define coSTOP CmdSep+8
#define coFOR CmdSep+9
#define coNEXT CmdSep+10
#define coEND CmdSep+11
#define coMEM CmdSep+12
#define coDIM CmdSep+13
#define coRANDOMIZE CmdSep+14
#define coOPTION CmdSep+15
#define coBEEP CmdSep + 16
#define coCLS CmdSep + 17
#define coREAD CmdSep+18
#define coDATA CmdSep + 19
#define coRESTORE CmdSep + 20
#define coDEF CmdSep+21
#define coTMPCMD CmdSep+22

#define DirectCmdSep 300
#define coLIST DirectCmdSep+1
#define coNEW DirectCmdSep+2
#define coEXIT DirectCmdSep+3
#define coDEBUG DirectCmdSep+4
#define coRUN DirectCmdSep+5
#define coEVAL DirectCmdSep+6
#define coLOAD DirectCmdSep+7
#define coSAVE DirectCmdSep+8
#define coNODELIST DirectCmdSep+9
#define coEDIT DirectCmdSep+10
#define coEDITOR DirectCmdSep+11
#define coPWD DirectCmdSep+12
#define coCHDIR DirectCmdSep+13
#define coDIR DirectCmdSep+14 
#define coCLEAR DirectCmdSep+15
#define coCONTINUE DirectCmdSep + 16

#define FuncSep 400

#define coABS FuncSep+0
#define coRND FuncSep+1
#define coMAX FuncSep+2
#define coSQRT FuncSep+3
#define coLEFT FuncSep+4
#define coINKEY FuncSep+5
#define coLEN FuncSep+6
#define coSTR FuncSep+7
#define coVAL FuncSep+8
#define coTIMER FuncSep+9
#define coINT FuncSep+10
#define coRIGHT FuncSep+11
#define coTEST FuncSep+12




#define coError 0
#define coEqual 16
#define coPlus 17
#define coMinus 18
#define coMultiply 19
#define coDivide 20
#define coGreater 21
#define coLess 22
#define coPower 23
#define coCloseBracket 24
#define coOpenBracket 25
#define coGreaterLesser 26
#define coLessEqual 27
#define coGreaterEqual 28
#define coComma 29
#define coSemiColon 30
#define coColon 31
#define coBoxOpen 32
#define coBoxClose 33
#define coTHEN 34
#define coTO 35
#define coSTEP 38
#define coFN 39
#define coDOWNTO 40

#define coNumber 44
#define coNumberExpression 45
#define coStringExpression 46
#define coVariable 46
#define coStringId 48
#define coIntegerId 49
#define coExclamation 50
#define coDoubleId 51
#define cvSingle 53
#define cvDouble 54
#define cvInteger 55
#define cvString 56
#define coDouble 57
#define coInteger 58
#define coString 59
#define coBoolExpression 61
#define coUnaryMinus 63
#define coBackSlash 64
#define coAND 65
#define coOR 66
#define coNOT 67
#define coXOR 68
#define cvNumeric 69
#define crRemark 70


#define coAT 100
#define coTAB 101


#define coBASE 110

#define MAX_KEYWORD_SIZE 16
#define MAX_STRING_LENGTH 255


#define CMD_OK 1000
#define CMD_OK_Cond_Fail 1001
#define CMD_STOP 1002
#define CMD_BREAK 1003
#define CMD_ENDRUN 1004
#define CMD_OK_POINTER_CHANGE 1005

# define SYNTAX_END 255

#endif // SYNTAX_H

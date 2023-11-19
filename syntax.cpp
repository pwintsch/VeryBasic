
#include "syntax.hpp"

typedef struct tTokenType {
//	char sTxt[MAX_KEYWORD_SIZE];
	std::string sTxt;
	int iType;
	int iId;
	int iPrecedence;
} tTokenType;



tTokenType tTokens[]={
    { "LIST", tDirectCommand, coLIST, 0},
	{ "NEW", tDirectCommand, coNEW, 0},
	{ "EXIT", tDirectCommand, coEXIT, 0},
	{ "DEBUG", tDirectCommand, coDEBUG, 0},
	{ "RUN", tDirectCommand, coRUN, 0},
	{ "EVAL", tDirectCommand, coEVAL, 0},
	{ "LOAD", tDirectCommand, coLOAD, 0},
	{ "SAVE", tDirectCommand, coSAVE, 0},
	{ "NODELIST", tDirectCommand, coNODELIST, 0},
	{ "EDIT", tDirectCommand, coEDIT,0},
	{ "EDITOR", tDirectCommand, coEDITOR,0},
	{ "LET", tCommand, coLET, 0},
	{ "INPUT", tCommand, coINPUT, 0},
	{ "REM", tCommand, coREM, 0},
	{ "PRINT", tCommand, coPRINT, 0},
	{ "IF", tCommand, coIF, 0},
	{ "GOTO", tCommand, coGOTO, 0},
	{ "GOSUB", tCommand, coGOSUB, 0},
	{ "RETURN", tCommand, coRETURN, 0},
	{ "STOP", tCommand, coSTOP, 0},
	{ "END", tCommand, coEND, 0},
	{ "FOR", tCommand, coFOR, 0},
	{ "NEXT", tCommand, coNEXT, 0},
	{ "CLEAR", tCommand, coCLEAR, 0},
	{ "OPTION", tCommand, coOPTION},
	{ "TMPCMD", tCommand, coTMPCMD},	
	{ "DIM", tCommand, coDIM, 0},
	{ "RANDOMIZE", tCommand, coRANDOMIZE, 0},
	{ "CONTINUE", tCommand, coCONTINUE, 0},
	{ "BEEP", tCommand, coBEEP, 0},
	{ "CLS", tCommand, coCLS, 0},
	{ "READ", tCommand, coREAD, 0},
	{ "DATA", tCommand, coDATA, 0},
	{ "DEF", tCommand, coDEF,0},
	{ "THEN", tComplement, coTHEN, 0},
	{ "TO", tComplement, coTO, 0},
	{ "STEP", tComplement, coSTEP, 0},
	{ "BASE", tComplement, coBASE, 0},
	{ "FN", tComplement, coFN, 0},
	{ "ABS", tFunction, coABS, 0},
	{ "RND", tFunction, coRND, 0},
	{ "MEM", tFunction, coMEM, 0},
	{ "MAX", tFunction, coMAX, 0},
	{ "LEFT", tFunction, coLEFT, 0},
	{ "TEST", tFunction, coTEST, 0},
	{ "INKEY", tFunction, coINKEY},
	{ "SQRT", tFunction, coSQRT},
	{ "SQR", tFunction, coSQRT},
	{ "AT", tPrintArgument, coAT},
	{ "TAB", tPrintArgument, coTAB},
	{ "=", tComparison, coEqual, 0},
	{ "<", tComparison, coLess, 0},
	{ ">", tComparison, coGreater, 0},
	{ "<>", tComparison, coGreaterLesser, 0},
	{ "<=", tComparison, coLessEqual, 0},
	{ ">=", tComparison, coGreaterEqual, 0},
	{ ",", tSeparator, coComma, 0},
	{ ";", tSeparator, coSemiColon, 0},
	{ ":", tSeparator, coColon, 0},
	{ "+", tOperator, coPlus, 4},
	{ "-", tOperator, coMinus, 4},
	{ "*", tOperator, coMultiply, 2},
	{ "/", tOperator, coDivide, 2},
	{ "^", tOperator, coPower, 0},
	{ "AND", tOperator, coAND, 5},
	{ "OR", tOperator, coOR, 5},
	{ "NOT", tOperator, coNOT, 5},
//	{ "XOR", tOperator, coXOR, 5},
	{ "(", tBracket, coOpenBracket, 10},
	{ ")", tBracket, coCloseBracket, 0},
	{ "[", tBracket, coBoxOpen, 0},
	{ "]", tBracket, coBoxClose, 0},
	{ "$", tIdentifier, coStringId,0},
	{ "%", tIdentifier, coIntegerId,0},
	{ "!", tIdentifier, coExclamation,0},
	{ "#", tIdentifier, coDoubleId,0},
	{"\\", tSeparator, coBackSlash}
};


int NumberOfTokens() {
	return sizeof(tTokens)/sizeof(tTokenType);
}

int GetTokenTypeFromID (int iPassedId) {
	for (int i=0; i<NumberOfTokens(); i++) {
		if (tTokens[i].iId==iPassedId) {
			return tTokens[i].iType;
		}
	}
	return tUnknown;
}



bool IsStringInt (const char *sTxt) {
	
	if (*sTxt=='\0') {
		return false;
	}
	while(*sTxt != '\0') {
		if(*sTxt < '0' || *sTxt > '9') {
			return false;
		}		
		sTxt++;
	}
	return true;
}


bool IsStringFloat (const char *sTxt) {
	
	if (*sTxt=='\0') {
		return false;
	}
	while(*sTxt != '\0') {
		if(*sTxt < '0' || *sTxt > '9') {
			if (*sTxt!='.') {
				return false;
			}
		}		
		sTxt++;
	}
	return true;
}


bool GetTokenInfoFromTxt(std::string &sParam, int *iType, int *iID) {
	if (IsStringInt(sParam.c_str())){
		*iType=tValue;
		*iID=coInteger;
		return true;
	}
	if (IsStringFloat(sParam.c_str())){
		*iType=tValue;
		*iID=coDouble;
		return true;
	}
	std::transform(sParam.begin(), sParam.end(), sParam.begin(), ::toupper);
	for (int i=0; i<NumberOfTokens(); i++) {
		if (tTokens[i].sTxt==sParam) {
			*iType=tTokens[i].iType;
			*iID=tTokens[i].iId;
			return true;
		}
	}
	return false;
}
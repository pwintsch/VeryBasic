
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



std::vector<tSyntaxNode> AssignSyntax = {
				 	{ tVariable, coVariable}, 
					{ tComparison, coEqual},
				    { tExpression, 0}};
					
std::vector<tSyntaxNode> LETSyntax = { 	
					{ tCommand, coLET}, 
					{ tVariable, coVariable}, 
					{ tComparison, coEqual}, 
					{ tExpression ,0} };

std::vector<tSyntaxNode> IFSyntax = { 	
					{ tCommand, coIF}, 
					{ tExpression, 0},
					{ tComplement, coTHEN} };

std::vector<tSyntaxNode> LISTSyntax = { 	
					{ tDirectCommand, coLIST} };

std::vector<tSyntaxNode> LISTSyntax2 = { 	
					{ tDirectCommand, coLIST},
       				{ tValue, coInteger} };

std::vector<tSyntaxNode> LISTSyntax3 = { 	
					{ tDirectCommand, coLIST},
       				{ tValue, coInteger},
					{ tOperator, coMinus},
					{ tValue, coInteger} };

std::vector<tSyntaxNode> EXITSyntax = { 	
					{ tDirectCommand, coEXIT} };


std::vector<tSyntaxNode> OptionSyntax = { 	
					{ tCommand, coOPTION}, 
					{ tComplement, coBASE},
					{ tValue, 0} };

std::vector<tSyntaxNode> REMSyntax = {
				 	{ tCommand, coREM}, 
					{ tRemark, 0} };


tSyntaxNode tOPTIONSyntax[] = { 	{ tCommand, coOPTION}, 
					{ tComplement, coBASE},
					{ tValue, 0},  
					{ SYNTAX_END, 0} };


tSyntaxNode tFORSyntax[] = { 	{ tCommand, coFOR}, 
					{ tVariable, cvNumeric}, 
					{ tComparison, coEqual}, 
					{ tExpression ,0},
					{ tComplement ,coTO},
					{ tExpression ,0},					
					{ SYNTAX_END, 0} };


tSyntaxNode tFORSyntax2[] = { 	{ tCommand, coFOR}, 
					{ tVariable, cvNumeric}, 
					{ tComparison, coEqual}, 
					{ tExpression ,0},
					{ tComplement ,coTO},
					{ tExpression ,0},
					{ tComplement ,coSTEP},
					{ tExpression ,0},					
					{ SYNTAX_END, 0} };


tSyntaxNode tNEXTSyntax[] = { 	{ tCommand, coNEXT}, 
					{ tVariable, cvNumeric}, 
					{ SYNTAX_END, 0} };



tSyntaxNode tDIMSyntax[] = { 	{ tCommand, coDIM}, 
					{ tVariable, 0}, 
					{ SYNTAX_END, 0} };



tSyntaxNode tAssignSyntax[] = { 	{ tVariable, coVariable}, 
					{ tComparison, coEqual},
				    { tExpression, 0},	
					{ SYNTAX_END, 0} };




tSyntaxNode tREMSyntax2[] = { 	{ tCommand, coREM}, 
					{ SYNTAX_END, 0} };

tSyntaxNode tCONTINUESyntax[] = { 	{ tCommand, coCONTINUE}, 
					{ SYNTAX_END, 0} };

tSyntaxNode tBEEPSyntax[] = { 	{ tCommand, coBEEP}, 
					{ SYNTAX_END, 0} };

tSyntaxNode tCLSSyntax[] = { 	{ tCommand, coCLS}, 
					{ SYNTAX_END, 0} };

tSyntaxNode tIFSyntax[] = { 	{ tCommand, coIF}, 
					{ tExpression, 0},
					{ tComplement, coTHEN}, 
					{ SYNTAX_END, 0} };


tSyntaxNode tPRINTSyntax[] = { 	{ tCommand, coPRINT}, 
					{ tPrintExpression, 0}, 
					{ SYNTAX_END, 0} };


tSyntaxNode tSAVESyntax[] = { 	{ tDirectCommand, coSAVE}, 
					{ tExpression, tString}, 
					{ SYNTAX_END, 0} };


tSyntaxNode tLOADSyntax[] = { 	{ tDirectCommand, coLOAD}, 
					{ tExpression, tString}, 
					{ SYNTAX_END, 0} };


tSyntaxNode tINPUTSyntax[] = { 	{ tCommand, coINPUT}, 
					{ tInputExpression, 0}, 
					{ SYNTAX_END, 0} };


tSyntaxNode tLISTSyntax[] = { 	{ tDirectCommand, coLIST}, 
					{ SYNTAX_END, 0} };

tSyntaxNode tLISTSyntax2[] = { 	{ tDirectCommand, coLIST},
       					{ tValue, coInteger},	
					{ SYNTAX_END, 0} };

tSyntaxNode tLISTSyntax3[] = { 	{ tDirectCommand, coLIST},
       					{ tValue, coInteger},
						{ tOperator, coMinus},
						{ tValue, coInteger},	
					{ SYNTAX_END, 0} };


tSyntaxNode tNODELISTyntax[] = { 	{ tDirectCommand, coNODELIST},
       					{ tValue, coInteger},	
					{ SYNTAX_END, 0} };


tSyntaxNode tGOTOSyntax[] = { 	{ tCommand, coGOTO},
       				{ tExpression, tValue},	
					{ SYNTAX_END, 0} };


tSyntaxNode tGOSUBSyntax[] = { 	{ tCommand, coGOSUB},
       				{ tExpression, tValue},	
					{ SYNTAX_END, 0} };


tSyntaxNode tRETURNSyntax[] = { 	{ tCommand, coRETURN}, 
					{ SYNTAX_END, 0} };

tSyntaxNode tSTOPSyntax[] = { 	{ tCommand, coSTOP}, 
					{ SYNTAX_END, 0} };

tSyntaxNode tRUNSyntax[] = { 	{ tDirectCommand, coRUN}, 
					{ SYNTAX_END, 0} };

tSyntaxNode tEVALSyntax[] = { 	{ tDirectCommand, coEVAL},
       					{ tExpression, 0},	
					{ SYNTAX_END, 0} };


tSyntaxNode tEXITSyntax[] = { 	{ tDirectCommand, coEXIT}, 
					{ SYNTAX_END, 0} };

tSyntaxNode tNEWSyntax[] = { 	{ tDirectCommand, coNEW}, 
                    { SYNTAX_END, 0} };
					
tSyntaxNode tDEBUGSyntax[] = { 	{ tDirectCommand, coDEBUG}, 
					{ SYNTAX_END, 0} };


tSyntaxNode tRANDOMIZESyntax[] = { 	{ tCommand, coRANDOMIZE}, 
					{ SYNTAX_END, 0} };

tSyntaxNode tRANDOMIZESyntax2[] = { 	{ tCommand, coRANDOMIZE}, 
					{ tExpression, tValue},
					{ SYNTAX_END, 0} };

tSyntaxNode tTMPCMDSyntax[] = { 	{ tCommand, coINKEY}, 
					{ SYNTAX_END, 0} };

tSyntaxNode tEDITSyntax[] = { 	{ tDirectCommand, coEDIT},
       				{ tValue, coInteger},	
					{ SYNTAX_END, 0} };

tSyntaxNode tEDITORSyntax[] = { 	{ tDirectCommand, coEDITOR}, 
					{ SYNTAX_END, 0} };


tSyntaxNode tREADSyntax[] = { 	{ tCommand, coREAD}, 
					{ tVariableList, 0}, 
					{ SYNTAX_END, 0} };

tSyntaxNode tDATASyntax[] = { 	{ tCommand, coDATA}, 
					{ tValueList, 0}, 
					{ SYNTAX_END, 0} };

tSyntaxNode tDEFSyntax[] = { 	{ tCommand, coDEF}, 
//					{ tComplement, coFN},
					{ tUserFunction, 0}, 
					{ tComparison, coEqual}, 
					{ tExpression ,0},
					{ SYNTAX_END, 0} };

tSyntaxNode tDEFSyntax2[] = { 	{ tCommand, coDEF}, 
					{ tComplement, coFN},
					{ tUserFunction, 0}, 
					{ tComparison, coEqual}, 
					{ tExpression ,0},
					{ SYNTAX_END, 0} };




std::vector<tSyntax> tGrammar ={	
		{ coVariable, 0, AssignSyntax},		
		{ coLET, 0, LETSyntax },
		{ coIF, 0, IFSyntax },
		{ coLIST, 0, LISTSyntax },
		{ coLIST, 1, LISTSyntax2 },
		{ coLIST, 2, LISTSyntax3 }, 
		{ coEXIT, 0, EXITSyntax },
		{ coREM, 0, REMSyntax}
		/*,
		{ coNEW, 0, tNEWSyntax },
		{ coDEBUG, 0, tDEBUGSyntax },
		{ coNODELIST, 0, tNODELISTyntax},
		{ coPRINT, 0, tPRINTSyntax },
		{ coINPUT, 0, tINPUTSyntax },
		{ coEVAL, 0, tEVALSyntax },
		{ coRUN, 0, tRUNSyntax},
		{ coLOAD, 0, tLOADSyntax},
		{ coSAVE, 0, tSAVESyntax},
		{ coGOTO, 0, tGOTOSyntax},
		{ coGOSUB, 0, tGOSUBSyntax},
		{ coRETURN, 0, tRETURNSyntax},
		{ coREM, 0, tREMSyntax},
		{ coREM, 1, tREMSyntax2},
		{ coFOR, 0, tFORSyntax2},
		{ coFOR, 1, tFORSyntax},
		{ coNEXT, 0, tNEXTSyntax},
		{ coDIM, 0, tDIMSyntax},
		{ coRANDOMIZE, 0, tRANDOMIZESyntax},
		{ coRANDOMIZE, 1, tRANDOMIZESyntax2},
		{ coSTOP, 0, tSTOPSyntax},
		{ coOPTION, 0, tOPTIONSyntax},
		{ coEDIT, 0, tEDITSyntax},
		{ coEDITOR, 0, tEDITORSyntax},
		{ coCONTINUE, 0, tCONTINUESyntax},
		{ coBEEP, 0, tBEEPSyntax},
		{ coCLS, 0, tCLSSyntax},
		{ coREAD, 0, tREADSyntax},
		{ coDATA, 0, tDATASyntax},
		{ coTMPCMD, 0, tTMPCMDSyntax},
		{ coDEF, 0, tDEFSyntax},
		{ coDEF, 0, tDEFSyntax2},
		*/
		};


std::vector<tSyntax> GetSyntaxRules(int iCommandCode) {
	std::vector<tSyntax> vRules;
	for (int i=0; i<tGrammar.size(); i++) {
		if (tGrammar[i].iCommandCode==iCommandCode) {
			vRules.push_back(tGrammar[i]);
		}
	}
	return vRules;
}


std::map <int, std::string> SyntaxNodeStrings =  { 
  { tError, "ERROR" }, 
  { tUnknown, "UNKNOWN" },
  { tCommand, "COMMAND" }, 
  { tVariable, "VARIABLE" },
  { tFunction, "FUNCTION" },   
  { tOperator, "OPERATOR" },
  { tComparison, "COMPARISON" },
  { tString, "STRING" },
  { tSeparator, "SEPARATOR" },
  { tBracket, "BRACKET" },
  { tComplement, "COMPLEMENT" },
  { tIdentifier, "IDENTIFIER" },
  { tValue, "VALUE" },
  { tRemark, "REMARK" },
  { tDirectCommand, "DIRECT COMMAND" },
  { tExpressionList, "EXPRESSION LIST" },
  { tVariableList, "VARIABLE LIST" },
  { tArray, "ARRAY" },
  { tNumber, "NUMBER" },
  { tValueList, "VALUELIST" },
  { tExpression, "EXPRESSION" },
  { tPrintArgument, "PRINT ARGUMENT" },
  { tPrintExpression, "PRINT EXPRESSION" },
  { tInputExpression, "INPUT EXPRESSION" },
  { tUserFunction, "USER FUNCTION" }  
};

std::string GetSyntaxNodeString(int iSyntaxNode) {
	return SyntaxNodeStrings[iSyntaxNode];
}

std::string StringFromSyntaxRule(std::vector<tSyntaxNode> tRule) {
	std::string s="";
	for (int i=0; i<tRule.size(); i++) {
		if (i>0) {
			s=s+" : ";
		}
		s=s+GetSyntaxNodeString(tRule[i].iTType);
		if (tRule[i].iTId!=0) {
			s=s+" - " + std::to_string(tRule[i].iTId);
		}
	}
	return s;
}
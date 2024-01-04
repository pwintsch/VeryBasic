
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

std::string GetTokenTextFromID (int iPassedID) {
	std::string sText="";
	for (int i=0; i<NumberOfTokens(); i++) {
		if (tTokens[i].iId==iPassedID) {
			sText=tTokens[i].sTxt;
			break;
		}
	}
	return sText;
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
				 	{ tUserDefined, coVariable}, 
					{ tComparison, coEqual},
				    { tExpression, 0}};
					
std::vector<tSyntaxNode> LETSyntax = { 	
					{ tCommand, coLET}, 
					{ tUserDefined, coVariable}, 
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


std::vector<tSyntaxNode> REMSyntax2 = {
				 	{ tCommand, coREM}
					};


std::vector<tSyntaxNode> EVALSyntax = { 	
					{ tDirectCommand, coEVAL},
	   				{ tExpression, 0} };


std::vector<tSyntaxNode> PRINTSyntax = { 	
					{ tCommand, coPRINT}, 
					{ tPrintExpression, 0}};


std::vector<tSyntaxNode> PRINTSyntax2 = { 	
					{ tCommand, coPRINT}};


std::vector<tSyntaxNode> INPUTSyntax = {
				 	{ tCommand, coINPUT}, 
					{ tInputExpression, 0}
					};


std::vector <tSyntaxNode> BEEPSyntax = {
				 	{ tCommand, coBEEP} 
					};


std::vector <tSyntaxNode> STOPSyntax = {
				 	{ tCommand, coSTOP} 
					};

std::vector <tSyntaxNode> ENDSyntax = {
				 	{ tCommand, coEND} 
					};

std::vector <tSyntaxNode> RUNSyntax = {
				 	{ tDirectCommand, coRUN} 
					};

std::vector <tSyntaxNode> GOTOSyntax = {
				 	{ tCommand, coGOTO},
	   				{ tExpression, tValue} 
					};

std::vector <tSyntaxNode> GOSUBSyntax = {
				 	{ tCommand, coGOSUB},
	   				{ tExpression, tValue} 
					};

std::vector <tSyntaxNode> RETURNSyntax = {
				 	{ tCommand, coRETURN} 
					};


std::vector <tSyntaxNode> CLSSyntax = {
				 	{ tCommand, coCLS} 
					};


std::vector <tSyntaxNode> CLEARSyntax = {
				 	{ tCommand, coCLEAR} 
					};


std::vector <tSyntaxNode> NEWSyntax = {
				 	{ tDirectCommand, coNEW} 
					};


std::vector <tSyntaxNode> DIMSyntax = {
				 	{ tCommand, coDIM}, 
					{ tUserDefined, 0}
					};

std::vector <tSyntaxNode> TMPCMDSyntax = {
				 	{ tCommand, coTMPCMD} 
					};


std::vector <tSyntaxNode> SAVESyntax = {
				 	{ tDirectCommand, coSAVE}, 
					{ tExpression, tString}
};


std::vector <tSyntaxNode> LOADSyntax = {
				 	{ tDirectCommand, coLOAD}, 
					{ tExpression, tString}
};

std::vector <tSyntaxNode> OPTIONSyntax = {
				 	{ tCommand, coOPTION},
					{ tComplement, coBASE},
					{ tValue, 0}
};

// ------------------------------


tSyntaxNode tOPTIONSyntax[] = { 	{ tCommand, coOPTION}, 
					{ tComplement, coBASE},
					{ tValue, 0},  
					{ SYNTAX_END, 0} };


tSyntaxNode tFORSyntax[] = { 	{ tCommand, coFOR}, 
					{ tUserDefined, cvNumeric}, 
					{ tComparison, coEqual}, 
					{ tExpression ,0},
					{ tComplement ,coTO},
					{ tExpression ,0},					
					{ SYNTAX_END, 0} };


tSyntaxNode tFORSyntax2[] = { 	{ tCommand, coFOR}, 
					{ tUserDefined, cvNumeric}, 
					{ tComparison, coEqual}, 
					{ tExpression ,0},
					{ tComplement ,coTO},
					{ tExpression ,0},
					{ tComplement ,coSTEP},
					{ tExpression ,0},					
					{ SYNTAX_END, 0} };


tSyntaxNode tNEXTSyntax[] = { 	{ tCommand, coNEXT}, 
					{ tUserDefined, cvNumeric}, 
					{ SYNTAX_END, 0} };



tSyntaxNode tCONTINUESyntax[] = { 	{ tCommand, coCONTINUE}, 
					{ SYNTAX_END, 0} };


tSyntaxNode tNODELISTyntax[] = { 	{ tDirectCommand, coNODELIST},
       					{ tValue, coInteger},	
					{ SYNTAX_END, 0} };




tSyntaxNode tRANDOMIZESyntax[] = { 	{ tCommand, coRANDOMIZE}, 
					{ SYNTAX_END, 0} };

tSyntaxNode tRANDOMIZESyntax2[] = { 	{ tCommand, coRANDOMIZE}, 
					{ tExpression, tValue},
					{ SYNTAX_END, 0} };

tSyntaxNode tEDITSyntax[] = { 	{ tDirectCommand, coEDIT},
       				{ tValue, coInteger},	
					{ SYNTAX_END, 0} };

tSyntaxNode tEDITORSyntax[] = { 	{ tDirectCommand, coEDITOR}, 
					{ SYNTAX_END, 0} };


tSyntaxNode tREADSyntax[] = { 	{ tCommand, coREAD}, 
					{ tUserDefinedList, 0}, 
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
		{ coRUN, 0, RUNSyntax },
		{ coREM, 0, REMSyntax},
		{ coREM, 1, REMSyntax2},
		{ coEVAL, 0, EVALSyntax},
		{ coPRINT, 0, PRINTSyntax},
		{ coPRINT, 1, PRINTSyntax2},
		{ coINPUT, 0, INPUTSyntax},
		{ coBEEP, 0, BEEPSyntax},
		{ coGOTO,0, GOTOSyntax},
		{ coSTOP, 0, STOPSyntax},
		{ coEND, 0, ENDSyntax},
		{ coGOSUB, 0, GOSUBSyntax},
		{ coRETURN,0, RETURNSyntax},
		{ coCLS, 0, CLSSyntax},
		{ coCLEAR, 0, CLEARSyntax},
		{ coNEW, 0, NEWSyntax},
		{ coDIM, 0, DIMSyntax},
		{ coLOAD, 0, LOADSyntax},
		{ coSAVE, 0, SAVESyntax},
		{ coTMPCMD, 0, TMPCMDSyntax},
		{ coOPTION, 0, OPTIONSyntax}
		/*,

		{ coFOR, 0, tFORSyntax2},
		{ coFOR, 1, tFORSyntax},
		{ coNEXT, 0, tNEXTSyntax},
		{ coRANDOMIZE, 0, tRANDOMIZESyntax},
		{ coRANDOMIZE, 1, tRANDOMIZESyntax2},
		{ coEDIT, 0, tEDITSyntax},
		{ coEDITOR, 0, tEDITORSyntax},
		{ coCONTINUE, 0, tCONTINUESyntax},
		{ coREAD, 0, tREADSyntax},
		{ coDATA, 0, tDATASyntax},
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
  { tUserDefined, "USER DEFINED" },
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
  { tUserDefinedList, "USER DEFINED LIST" },
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
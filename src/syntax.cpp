
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
	{ "PWD", tDirectCommand, coPWD},
	{ "CHDIR", tDirectCommand, coCHDIR},
	{ "DIR", tDirectCommand, coDIR},
	{ "CLEAR", tDirectCommand, coCLEAR, 0},
	{ "CONTINUE", tDirectCommand, coCONTINUE, 0},
	{ "TRACE", tDirectCommand, coTRACE,0},
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
	{ "REPEAT", tCommand, coREPEAT, 0},
	{ "UNTIL", tCommand, coUNTIL, 0},
	{ "WHILE", tCommand, coWHILE, 0},
	{ "WEND", tCommand, coWEND, 0},
	{ "OPTION", tCommand, coOPTION},
	{ "TMPCMD", tCommand, coTMPCMD},	
	{ "DIM", tCommand, coDIM, 0},
	{ "RANDOMIZE", tCommand, coRANDOMIZE, 0},
	{ "BEEP", tCommand, coBEEP, 0},
	{ "CLS", tCommand, coCLS, 0},
	{ "READ", tCommand, coREAD, 0},
	{ "DATA", tCommand, coDATA, 0},
	{ "RESTORE", tCommand, coRESTORE},
	{ "DEF", tCommand, coDEF,0},
	{ "ON", tCommand, coON, 0},
	{ "ELSE", tCommand, coELSE, 0},
	{ "THEN", tComplement, coTHEN, 0},
	{ "TO", tComplement, coTO, 0},
	{ "STEP", tComplement, coSTEP, 0},
	{ "DOWNTO",tComplement, coDOWNTO, 0},
	{ "BASE", tComplement, coBASE, 0},
	{ "FN", tComplement, coFN, 0},
	{ "ABS", tFunction, coABS, 0},
	{ "RND", tFunction, coRND, 0},
	{ "MEM", tFunction, coMEM, 0},
	{ "MAX", tFunction, coMAX, 0},
	{ "LEFT$", tFunction, coLEFT, 0},
	{ "RIGHT$", tFunction, coRIGHT},
	{ "MID$", tFunction, coMID},
	{ "INSTR", tFunction, coINSTR},
//	{ "TEST", tFunction, coTEST, 0},
	{ "INKEY", tFunction, coINKEY},
	{ "SQRT", tFunction, coSQRT},
	{ "SQR", tFunction, coSQRT},
	{ "LEN", tFunction, coLEN},
	{ "STR$", tFunction, coSTR},
	{ "VAL", tFunction, coVAL},
	{ "TIMER", tFunction, coTIMER},
	{ "INT", tFunction, coINT},
	{ "SGN", tFunction, coSGN},
	{ "PI", tFunction, coPI},
	{ "EXP", tFunction, coEXP},
	{ "COS", tFunction, coCOS},
	{ "SIN", tFunction, coSIN},
	{ "TAN", tFunction, coTAN},
	{ "ATN", tFunction, coATN},
	{ "ACS", tFunction, coACS},
	{ "ASN", tFunction, coASN},
	{ "LN", tFunction, coLN},
	{ "ASC", tFunction, coASC},
	{ "CHR$", tFunction, coCHR},
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
				 	{ tUserDefined, 0}, 
					{ tComparison, coEqual},
				    { tExpression, 0}};
					
std::vector<tSyntaxNode> LETSyntax = { 	
					{ tCommand, coLET}, 
					{ tUserDefined, 0}, 
					{ tComparison, coEqual}, 
					{ tExpression ,0} };

std::vector<tSyntaxNode> IFSyntax = { 	
					{ tCommand, coIF}, 
					{ tExpression, tValue},
					{ tComplement, coTHEN} };

std::vector<tSyntaxNode> IFSyntax2 = { 	
					{ tCommand, coIF}, 
					{ tExpression, tValue},
					{ tComplement, coTHEN},
					{ tValue, coInteger} };


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


std::vector <tSyntaxNode> ELSESyntax = {
				 	{ tCommand, coELSE} 
					};

std::vector <tSyntaxNode> CLSSyntax = {
				 	{ tCommand, coCLS} 
					};


std::vector <tSyntaxNode> RESTORESyntax = {
				 	{ tCommand, coRESTORE} 
					};
					


std::vector <tSyntaxNode> RESTORESyntax2 = {
				 	{ tCommand, coRESTORE},
					{ tExpression, tValue} 
					};


std::vector <tSyntaxNode> CLEARSyntax = {
				 	{ tCommand, coCLEAR} 
					};

std::vector <tSyntaxNode> TRACESyntax = {
				 	{ tDirectCommand, coTRACE} 
					};

std::vector <tSyntaxNode> NEWSyntax = {
				 	{ tDirectCommand, coNEW} 
					};


std::vector <tSyntaxNode> DIMSyntax = {
				 	{ tCommand, coDIM}, 
					{ tUserDefinedList, 0}
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


std::vector <tSyntaxNode> CHDIRSyntax = {
				 	{ tDirectCommand, coCHDIR}, 
					{ tExpression, tString}
};


std::vector <tSyntaxNode> DIRSyntax = {
				 	{ tDirectCommand, coDIR}
};


std::vector <tSyntaxNode> OPTIONSyntax = {
				 	{ tCommand, coOPTION},
					{ tComplement, coBASE},
					{ tValue, 0}
};


std::vector <tSyntaxNode> PWDSyntax = {
				 	{ tDirectCommand, coPWD} 
					};


std::vector <tSyntaxNode> EDITSyntax = {
				 	{ tDirectCommand, coEDIT}, 
					{ tValue, coInteger}
					};



std::vector <tSyntaxNode> FORSyntax = { 	
					{ tCommand, coFOR}, 
					{ tUserDefined, cvDouble}, 
					{ tComparison, coEqual}, 
					{ tExpression ,tValue},
					{ tComplement ,coTO},
					{ tExpression ,tValue}
					 };


std::vector <tSyntaxNode> FORSyntax2 = { 	
					{ tCommand, coFOR}, 
					{ tUserDefined, cvDouble}, 
					{ tComparison, coEqual}, 
					{ tExpression ,tValue},
					{ tComplement ,coDOWNTO},
					{ tExpression ,tValue}
					 };


std::vector <tSyntaxNode> FORSyntax3 = { 	
					{ tCommand, coFOR}, 
					{ tUserDefined, cvDouble}, 
					{ tComparison, coEqual}, 
					{ tExpression ,tValue},
					{ tComplement ,coTO},
					{ tExpression ,tValue},
					{ tComplement ,coSTEP},
					{ tExpression ,tValue}
					};


std::vector <tSyntaxNode> NEXTSyntax = { 	
					{ tCommand, coNEXT}, 
					{ tUserDefined, cvDouble}
					};

std::vector <tSyntaxNode> REPEATSyntax = { 	
					{ tCommand, coREPEAT}
					};


std::vector <tSyntaxNode> UNTILSyntax = { 	
					{ tCommand, coUNTIL}, 
					{ tExpression, tValue}
					};


std::vector <tSyntaxNode> WHILESyntax = { 	
					{ tCommand, coWHILE}, 
					{ tExpression, tValue}
					};


std::vector <tSyntaxNode> WENDSyntax = { 	
					{ tCommand, coWEND}
					};


std::vector <tSyntaxNode> READSyntax = { 	
					{ tCommand, coREAD}, 
					{ tUserDefinedList, 0}
					};


std::vector <tSyntaxNode> DATASyntax = { 	
					{ tCommand, coDATA}, 
					{ tValueList, 0}
					};


std::vector <tSyntaxNode> RANDOMIZESyntax = { 	
				 	{ tCommand, coRANDOMIZE}
					};


std::vector <tSyntaxNode> CONTINUESyntax = { 	
				 	{ tDirectCommand, coCONTINUE}
					};


std::vector <tSyntaxNode> RANDOMIZESyntax2 = { 	
				 	{ tCommand, coRANDOMIZE}, 
					{ tExpression, tValue}
					};



std::vector <tSyntaxNode> DEFSyntax = { 	
					{ tCommand, coDEF}, 
					{ tNewUserFunction, 0},   // could be tUserFunction then in Lexerize check its an acceptable tUSerDefined with right parametre
					{ tComparison, coEqual}, 
					{ tExpression ,0}
					};



std::vector <tSyntaxNode> EDITORSyntax = { 	
				 	{ tDirectCommand, coEDITOR}
					};


std::vector <tSyntaxNode> NODELISTyntax = { 	
						{ tDirectCommand, coNODELIST},
       					{ tValue, coInteger},	
						};


std::vector <tSyntaxNode> ONGOTOSyntax = {
						{ tCommand, coON},
						{ tExpression, 0},
						{ tCommand, coGOTO},
						{ tValueList, 0}
						};


std::vector <tSyntaxNode> ONGOSUBSyntax = {
						{ tCommand, coON},
						{ tExpression, 0},
						{ tCommand, coGOSUB},
						{ tValueList, 0}
						};


std::vector<tSyntax> tGrammar = {	
		{ coVariable, 0, AssignSyntax},		
		{ coLET, 0, LETSyntax },
		{ coIF, 0, IFSyntax2},
		{ coIF, 1, IFSyntax },
		{ coLIST, 0, LISTSyntax },
		{ coLIST, 1, LISTSyntax2 },
		{ coLIST, 2, LISTSyntax3 }, 
		{ coEXIT, 0, EXITSyntax },
		{ coRUN, 0, RUNSyntax },
		{ coREM, 0, REMSyntax},
		{ coREM, 1, REMSyntax2},
		{ coEVAL, 0, EVALSyntax},
		{ coPWD, 0, PWDSyntax},
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
		{ coFOR, 0, FORSyntax},
		{ coFOR, 1, FORSyntax2},
		{ coFOR, 2, FORSyntax3},
		{ coNEXT, 0, NEXTSyntax},
		{ coREAD, 0, READSyntax},
		{ coDATA, 0, DATASyntax},
		{ coRESTORE, 0, RESTORESyntax},
		{ coRESTORE, 1, RESTORESyntax2},
		{ coLOAD, 0, LOADSyntax},
		{ coSAVE, 0, SAVESyntax},
		{ coCHDIR, 0, CHDIRSyntax},
		{ coDIR, 0, DIRSyntax},
		{ coEDIT, 0, EDITSyntax},
		{ coTMPCMD, 0, TMPCMDSyntax},
		{ coOPTION, 0, OPTIONSyntax},
		{ coRANDOMIZE, 0, RANDOMIZESyntax},
		{ coRANDOMIZE, 1, RANDOMIZESyntax2},
		{ coCONTINUE, 0, CONTINUESyntax},
		{ coDEF, 0, DEFSyntax},
		{ coNODELIST, 0, NODELISTyntax},
		{ coREPEAT, 0, REPEATSyntax},
		{ coUNTIL, 0, UNTILSyntax},
		{ coWHILE, 0, WHILESyntax},
		{ coWEND, 0, WENDSyntax},
		{ coON, 0, ONGOTOSyntax},
		{ coON, 1, ONGOSUBSyntax},
		{ coELSE, 0, ELSESyntax},
		{ coTRACE, 0, TRACESyntax}

		/*,
		{ coEDITOR, 0, tEDITORSyntax},
		{ coDEF, 0, tDEFSyntax},
		{ coDEF, 0, tDEFSyntax2},
		*/
		};


std::vector<int> ABSSyntax =  { tValue};
std::vector<int> SQRTSyntax=  { tValue};
std::vector<int> RNDSyntax= {};
std::vector<int> RNDSyntax2= {tValue};
std::vector<int> MAXSyntax= {tValue, tValue};
std::vector<int> TESTSyntax= {tValue, tValue, tValue};
std::vector<int> LEFTSyntax= {tString, tValue};
std::vector<int> RIGHTSyntax= {tString, tValue};
std::vector<int> MIDSyntax= {tString, tValue, tValue};
std::vector<int> LENSyntax= {tString};
std::vector<int> STRSyntax= {tValue};
std::vector<int> VALSyntax= {tString};
std::vector<int> INKEYSyntax={};
std::vector<int> NoParamSyntax= {};
std::vector<int> TIMERSyntax= {};
std::vector<int> PISyntax={};
std::vector<int> INTSyntax =  {tValue};
std::vector<int> SGNSyntax = {tValue};
std::vector<int> EXPSyntax = {tValue};
std::vector<int> COSyntax = {tValue};
std::vector<int> SINSyntax = {tValue};
std::vector<int> TANSyntax = {tValue};
std::vector<int> ATNSyntax = {tValue};
std::vector<int> ACSSyntax = {tValue};
std::vector<int> ASNSyntax = {tValue};
std::vector<int> LNSyntax = {tValue};
std::vector<int> INSTRyntax= {tString, tString};
std::vector<int> ASCSyntax= {tString};
std::vector<int> CHRSyntax =  {tValue};


std::vector<tFunctionSyntax> tFunctionGrammar = {
	{ coABS, tValue, ABSSyntax},
	{ coRND, tValue, RNDSyntax},
	{ coMAX, tValue, MAXSyntax},
	{ coSQRT, tValue, SQRTSyntax},
	{ coLEFT, tString, LEFTSyntax},
	{ coINKEY, tString,INKEYSyntax},
	{ coLEN, tValue, LENSyntax},
	{ coSTR, tString, STRSyntax},
	{ coVAL, tString, VALSyntax},
	{ coTIMER, tValue, TIMERSyntax},
	{ coINT, tValue, INTSyntax},
	{ coRIGHT, tString, RIGHTSyntax},
	{ coSGN, tValue, SGNSyntax},
	{ coMID, tString, MIDSyntax},
	{ coINSTR, tValue, INSTRyntax},
	{ coPI, tValue, PISyntax},
	{ coEXP, tValue, EXPSyntax},
	{ coLN, tValue, LNSyntax},
	{ coCOS, tValue, COSyntax},
	{ coSIN, tValue, SINSyntax},
	{ coTAN, tValue, TANSyntax},
	{ coATN, tValue, ATNSyntax},
	{ coACS, tValue, ACSSyntax},
	{ coASN, tValue, ASNSyntax},
	{ coASC, tValue, ASCSyntax},
	{ coRND, tValue, RNDSyntax2},
	{ coCHR, tValue, CHRSyntax}
};


void GetFunctionSytax2 (int iFunctionCode, std::vector<tFunctionSyntax> &fSyntax) {
	fSyntax.clear();
	for (const auto &Syntax: tFunctionGrammar) {
		if (Syntax.iFunctionCode==iFunctionCode) {
			fSyntax.push_back(Syntax);
		}
	}
}


int NoOfFunctions() {
	return tFunctionGrammar.size();
}

tFunctionSyntax GetFunctionSyntax(int iFunctionCode) {
	int ParamIndex=iFunctionCode;
	return tFunctionGrammar[ParamIndex];
}


int GetFunctionType(int iFunctionCode) {
	return tFunctionGrammar[iFunctionCode].tReturnType;
}


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
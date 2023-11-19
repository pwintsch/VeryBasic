
#include "token.hpp"
#include "syntax.hpp"

#include "error.hpp"	// error messages



int IsTokenSeparator(char cCurrent, char cNext){

	switch (cCurrent){ 
		case ' ':
			return(4);
			break;
		case '\n':
		case 0:
		case ':':
		case ';':
		case ',':
		case '+':
		case '-':
		case '*':
		case '/':
		case '^':
		case '=':
		case '!':
		case '(':
		case ')':
		case '[':
		case ']':
		case '\\':
			return(1);
			break;
		case '<':
			if ((cNext=='>') || (cNext=='=')) {
				return(2);
			}
			return(1);
			break;
		case '>':
			if (cNext=='=') {
				return(2);
			}
			return(1);
			break;
		case '"':
		case '\'':
			return (5);
			break;
		}
	return 0;
};


int IsValidVariableName(std::string &sTxt) {
    
	std::string::size_type iLen=sTxt.size();
    if (iLen==0) {
        return false;
    }
	for (int i=0; i<iLen ;i++) {
		if (isalpha(sTxt[i])==false){
			if (i==iLen-1) {
				switch (sTxt[i]) {
					case '!':
        				return (cvSingle);
						break;
					case '%':
        				return (cvInteger);
						break;
					case '$':
                        return (cvString);
                        break;
					case '#':
						return (cvDouble);
						break;
					default:
						return 0;
						}		
			} else {
				return 0;
			}
		}
	}
	return cvDouble;
};	



int Token::Initialise(int iSepType, std::string &pValue) {
int iType=0;
int iID=0;

    Type=0;
    ID=0;
    if (iSepType==SEPARATOR_TYPE_STRING) {
		Type=tString;
		ID=coString;
        Value = pValue;
        return ID;
	} else if (iSepType==SEPARATOR_TYPE_REMARK) { 
		Type=tRemark;
		ID=crRemark;
        Value = pValue;
        return ID;
	} else {
		if (GetTokenInfoFromTxt(pValue, &iType, &iID)) {
            Type = iType;
            ID = iID;
            Value = pValue;
            return ID;
        } else {
            int iVarType = IsValidVariableName(pValue);
            if (iVarType!=0) {		
                Type=tVariable;
                ID=iVarType;
                Value = pValue;
                return ID;
            }
	    }
    }
    return 0;
};


TokenCollection::TokenCollection() {
    // constructor
}

TokenCollection::~TokenCollection() {
    // destructor
}   

Token TokenCollection::GetToken(int pIndex) {
    return Tokens[pIndex];
}

void TokenCollection::RemoveToken(int pIndex) {
	Tokens.erase(Tokens.begin()+pIndex);
}


void TokenCollection::AddToken(Token pToken) {
    Tokens.push_back(pToken);
}

int TokenCollection::AddNewToken(int pType, std::string pValue) {
    int i=0;
    Token pToken;
    i=pToken.Initialise(pType, pValue);
    Tokens.push_back(pToken);
    return i;
}

int TokenCollection::Count() {
    return Tokens.size();
}


std::string TokenCollection::GetString() {
    std::string s;
    for (int i=0; i<Count(); i++) {
        s=s+ "Type: " + std::to_string(Tokens[i].Type)+ " - ID: "+ std::to_string(Tokens[i].ID) + " - " + Tokens[i].Value + "\n\r";
    }
    return s;
}


int TokenCollection::Tokenise(std::string pInput) {

    // Tokenise the input string
    // return true if successful
    Tokens.clear();

    // loop through the input string
    // and add tokens to the tokens vector
    int i=0;
    int iNewStringPos=0;
    int iTokenSep=0;
    int iTokenId=0;
    char cOpenQuote;
    bool bStringMode=false;
    char* sInput = (char*)pInput.c_str();
    char s[255];
    do {
        if (sInput[i]!=0) {
            if (bStringMode==true){
                if(sInput[i]==cOpenQuote){
                    if (sInput[i+1]==cOpenQuote) {
                        s[iNewStringPos]=sInput[i];
                        s[iNewStringPos+1]=0;
                        iNewStringPos++;
                        i=i+2;
                    } else {
                        bStringMode=false;
                        iTokenId=AddNewToken(SEPARATOR_TYPE_STRING, s);
                        strcpy(s,"");
                        iNewStringPos=0;
                        i++;
                    }
                } else {
                    s[iNewStringPos]=sInput[i];
                    s[iNewStringPos+1]=0;
                    iNewStringPos++;
                    i++;
                }
            } else {
				iTokenSep=IsTokenSeparator(sInput[i],sInput[i+1]);
				if(iTokenSep==SEPARATOR_TYPE_UNKNOWN) {
					s[iNewStringPos]=sInput[i];
					s[iNewStringPos+1]=0;
					iNewStringPos++;
					i++;
				} else if(iTokenSep==SEPARATOR_TYPE_SINGLE_SEP) {
					if (strlen(s)>0){
                        iTokenId=AddNewToken(SEPARATOR_TYPE_UNKNOWN, s);
					}
					s[0]=sInput[i];
					s[1]=0;
                    iTokenId=AddNewToken(SEPARATOR_TYPE_SINGLE_SEP, s);
					strcpy(s,"");
					iNewStringPos=0;
					i++;
                } else if(iTokenSep==SEPARATOR_TYPE_DOUBLE_SEP){
					if (strlen(s)>0){
                        iTokenId=AddNewToken(SEPARATOR_TYPE_UNKNOWN, s);
					}
					s[0]=sInput[i];
					s[1]=sInput[i+1];
					s[2]=0;
					iTokenId=AddNewToken(SEPARATOR_TYPE_DOUBLE_SEP, s);
					iNewStringPos=0;
					strcpy(s,"");
					i=i+2;
                } else if(iTokenSep==5) {
					// prepare to start string capture accepting all caracters to next quote
					if (strlen(s)>0){
                        iTokenId=AddNewToken(SEPARATOR_TYPE_UNKNOWN, s);
						strcpy(s,"");
					}
					cOpenQuote=sInput[i];
					iNewStringPos=0;
					i++;
					bStringMode=true;
				} else {
					if (strlen(s)>0){
                        iTokenId=AddNewToken(SEPARATOR_TYPE_UNKNOWN, s);
						strcpy(s,"");
					}
					iNewStringPos=0;
					i++;
				}                
            }
            if (iTokenId==coREM){
                iNewStringPos=0;
                do {
                    s[iNewStringPos]=sInput[i];
                    i++;
                    iNewStringPos++;
                } while (sInput[i]!=0);
                s[iNewStringPos]=0;
                iTokenId=AddNewToken(SEPARATOR_TYPE_REMARK, s);
		    }
        }
    } while (sInput[i]!=0);
	if (strlen(s)>0){   //need to add last token if not in string mode, if in string mode exit with error
		iTokenId=AddNewToken(SEPARATOR_TYPE_UNKNOWN, s);
	}
	if (bStringMode==true) {
		return ERR_TOKEN_OPEN_STRING;
	}
    return NO_ERROR;
}


int TokenCollection::CheckBrackets() {
	int iBracketCount=0;
	int iSquareBracketCount=0;
	for (int i=0; i<Count(); i++) {
		if (Tokens[i].ID==coOpenBracket) {
			iBracketCount++;
		} else if (Tokens[i].ID==coCloseBracket) {
			iBracketCount--;
		} else if (Tokens[i].ID==coBoxOpen) {
			iSquareBracketCount++;
		} else if (Tokens[i].ID==coBoxClose) {
			iSquareBracketCount--;
		}
	}
	if (iBracketCount!=0 || iSquareBracketCount!=0) {
		return ERR_BAD_BRACKET;
	}
	return NO_ERROR;
}


#include "../../includes/token/token.h"
#include "../../includes/tokenizer/constants.h"
using namespace std;
#include <cstring>
#include <stdio.h>
#include <algorithm>
#include "child_tokens.h"

Token::Token() {
    _token = "";
    _type = -1;
}

Token::Token(string str, int type) {
    _token = str;
    _type = type;
}

int Token::type() const {
    return _type;
}

string Token::type_string() const {
    if (_type == 1) {return "NUMBER";}
    else if (_type == 2) {return "ALPHA";}
    else if (_type == 3) {return "SPACE";}
    else if (_type == 4) {return "OPERATOR";}
    else if (_type == 5) {return "PUNC";}
    else if (_type == 6) {return "IGNORE";}
    else if (_type == -2) {return "END";}
    else {return "Unknown";}
}

string Token::token_str() const {
    return _token;
}

// Checks string is one of these types
Token* Token::parsertoken(string t) {
    Token* l;
    if (t == ">=" || t == "<=" || t == ">" || t == "<" || t == "=" || t == "!=") {
        //cout << "new relational" << endl;
        l = new Relational(t);
        
    }
    // ignores case
    else if (strcasecmp(t.c_str(), "and") == 0 || strcasecmp(t.c_str(), "or") == 0  ) {
        //cout << "New logical" << endl;
        std::transform(t.begin(), t.end(), t.begin(),
        [](unsigned char c){ return std::tolower(c); });
        l = new Logical(t);
    }
    else if (t == "(") {
        l = new LParen();
    }
    else if (t == ")") {
        l = new RParen();
    }
    else  l = new TokenStr(t);
    return l;
}

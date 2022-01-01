#ifndef TOKEN_H
#define TOKEN_H


#include "../../includes/tokenizer/constants.h"
#include <iostream>
#include <string>
using namespace std;

class Token
{
public:
    enum class Type {
        Word,
        Operator,
        Lparen,
        Rparen,
    };
    Token();
    Token(string str, int type);

    friend ostream& operator <<(ostream& outs, const Token& t) {
        outs << "|" << t.token_str() << "|";
        return outs;
    }
    int type() const;
    static Token* parsertoken(string t);
    string type_string() const;
    string token_str() const;
protected:
    int _type;
private:
    string _token;

};

#endif //TOKEN_H
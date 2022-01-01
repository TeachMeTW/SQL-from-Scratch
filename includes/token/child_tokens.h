#ifndef CHILD_TOKENS_H
#define CHILD_TOKENS_H
#include <vector>
#include "token.h"


// assigned word enum
class TokenStr: public Token {
public:
    TokenStr(string t) : Token(t, (int)Token::Type::Word) {
    }
};

// resultset with vectorlong for records
class Resultset : public Token {
public:
    Resultset() {
    }
    vector<long> get_records() {
        // cout << records.size() << endl;
        // for (int i = 0; i<records.size(); i++) {
        //     cout << records[i] << " ";
        // }
        return records;
    }
    void add(vector<long> s) {
        records.insert(records.end(), s.begin(), s.end());
    }
    void add(long s) {
        records.push_back(s);
    }
private:
    vector<long> records;
};

class LParen : public Token {
public:
    LParen() : Token("(", (int)Token::Type::Lparen) {
    }
};

class RParen : public Token {
public:
    RParen() : Token(")", (int)Token::Type::Rparen) {
    }
};

// Operators can set prec
class Operators : public Token {
public:
    Operators(string t, int p) : Token(t, (int)Token::Type::Operator), _prec(p) {
    }
    int get_prec() {
        return _prec;
    }
    void set_prec(int prec) {
        _prec = prec;
    }
private:
    int _prec;
};

// default 0
class Relational : public Operators {
public:
    Relational(string t) : Operators(t, 0) {
    }
};

// default 1
class Logical : public Operators {
public:
    Logical(string t): Operators(t, 1) {
    }
    
};






#endif // CHILD_TOKENS_H

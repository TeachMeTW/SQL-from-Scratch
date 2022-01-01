#include "ftokenize.h"

#include <cstdlib>
#include <cstring>
using namespace std;


FTokenizer::FTokenizer(char* fname) {
    _f.open(fname);
    if (_f.fail()) {
        cout << "Unable to open file" << endl;
        exit(1);
    }
    char _block[MAX_BLOCK];
    memset(_block, '\0', MAX_BLOCK);
    _f.read(_block, (MAX_BLOCK-1));
    //cout << _block;
    _stk = STokenizer(_block);
    _blockPos = 0;
    _pos = 0;
    _more = true;
}


Token FTokenizer::next_token() {
    if (_stk.done()) {
        get_new_block();
    }
    Token t = Token();
    _stk >> t;
    _blockPos = (_blockPos+t.token_str().length());
    //cout << t;    

    return t;
}

bool FTokenizer::more() {
    //cout << endl <<"F count: " << _f.gcount() << endl;
    //cout << endl <<"Block position: " << _blockPos << endl;
    if (_f.eof()&& _blockPos == _f.gcount()+1) {
        //cout << "Closing" << endl;;
        _f.close();
        return false;
    }
    return true;
}

int FTokenizer::pos() {
    return _pos;
}

int FTokenizer::block_pos() {
    return _blockPos;
}

FTokenizer& operator>> (FTokenizer& f, Token&t) {
    //string tstring = t.token_str();
    t = f.next_token();
    //cout << t;
    //cout << endl << t.token_str() << endl;;
    //f._stk.get_token(0, tstring);
    return f;
}

bool FTokenizer::get_new_block() {
    
    if (_f.eof()) {
        //cout << "No More";
        return false;
    }
    //cout << _block;
    //_stk.set_string(_block);
    //_stk>>next_token();    
    //cannot use: char nblock[MAX_BLOCK] = "";
    char nblock[MAX_BLOCK];
    memset(nblock, '\0', MAX_BLOCK);
    _f.read(nblock, (MAX_BLOCK-1));
    _stk = STokenizer();
    //cout << endl << nblock << endl;
    _stk.set_string(nblock);
    //cout << nblock;
    _blockPos = 0;
    return true;

}
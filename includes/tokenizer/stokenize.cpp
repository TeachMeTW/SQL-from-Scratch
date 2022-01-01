#include "../../includes/tokenizer/stokenize.h"

#include <cstring>
#include <cstdio>
using namespace std;

int STokenizer::_table[MAX_ROWS][MAX_COLUMNS];

STokenizer::STokenizer() {
    init_table(_table);
    make_table(_table);
    set_string(_buffer);
    _pos = 0;
}

STokenizer::STokenizer(char str[]) {
    init_table(_table);
    make_table(_table);
    set_string(str);
    _pos = 0;
}

bool STokenizer::done() {
    //cout << "Pos: " << _pos << endl;
    //cout << "Buffer : " << strlen(_buffer) << endl;
    if (_buffer[_pos] == '\0' && _pos <= strlen(_buffer)) {
        //_pos = 0;
        return true;
    }
    else { return false;}
}

bool STokenizer::more() {
    int b_size = strlen(_buffer);
    if (_pos <= b_size) {
        return true;
    }
    else {
        return false;
    }
}

STokenizer& operator>>(STokenizer& s, Token& t) {
        string tstring = "";
        int type = -1;
        char current = s._buffer[s._pos];
        //cout << current;
        // extract token
        s.get_token(0, tstring);
        type = s.get_type(current);
        // advance internal pos

        // s._pos = s._pos+1;
        // get token type




        
        // Updates the token
        t = Token(tstring, type);
        return s;
}







void STokenizer::set_string(char str[]) {
    /*for (int i = 0; i<MAX_BUFFER; i++) {
        _buffer[i] = '\0';
    };
    */

    strncpy(_buffer, str, sizeof(_buffer));
    _buffer[sizeof(_buffer)] = '\0';

    /*for (int i = 0; i<strlen(str); i++) {
        _buffer[i] = str[i];
    }
    */
    //cout << "Current String: " << _buffer;
}

void STokenizer::make_table(int _table[][MAX_COLUMNS]) {
    // Start State
    mark_fail(_table, 0);

    // ALFA section

    mark_success(_table, 10);
    mark_success(_table, 11);
    mark_success(_table, 13);

    mark_cells(0, _table, ALFA, 10);
    
    mark_cells(10, _table, ALFA, 10);

    mark_cell(10, _table, '\'', 11);
    mark_cell(10, _table, '+', 11);
    mark_cell(10, _table, '-', 11);
    mark_cell(10, _table, '*', 11);
    mark_cell(11, _table, '\'', 11);
    mark_cell(11, _table, '+', 11);
    mark_cell(11, _table, '-', 11);
    mark_cell(11, _table, '*', 11);
    
    
    mark_cells(11, _table, ALFA, 10);

    mark_cell(0, _table, '\"', 12);
    mark_cells(12, _table, ALFA, 12);
    mark_cells(12, _table, DIGITS, 12);
    mark_cells(12, _table, OPERATORS, 12);
    mark_cells(12, _table, SPACES, 12);
    mark_cells(12, _table, PUNC, 12);
    mark_cell(12, _table, '\"', 13);

    // Digits section 

    mark_success(_table, 20);
    mark_success(_table, 23);
    mark_fail(_table, 22);

    mark_cells(0, _table, DIGITS, 20);

    mark_cells(20, _table, DIGITS, 20);
    // accounts for decimal
    mark_cells(20, _table, '.', '.', 22);
    mark_cells(22, _table, DIGITS, 23);
    // in case no digits
    // mark_cells(22, _table, '.', '.', 40);
    mark_cells(23, _table, DIGITS, 23);
        
    // Operator Section
    // Operators go to state 30

    mark_success(_table, 30);
    mark_success(_table, 32);

    mark_cells(0, _table, OPERATORS, 30);
    mark_cells(30, _table, OPERATORS, 30);
    mark_cell(30, _table, '!', 31);
    mark_cell(31, _table, '=', 32);
    mark_cell(30, _table, '(', 22);
    mark_cell(30, _table, ')', 22);
    mark_cell(30, _table, '*', 22);

    // Punctuation Section
    // Punc go to state 40

    mark_success(_table, 40);

    
    mark_cells(0, _table, PUNC, 40);
    mark_cell(40, _table, '=', 30);
    mark_cells(0, _table, ALFA, 10);
    mark_cells(40, _table, PUNC, 40);

    mark_success(_table, 50);

    // Spaces
    mark_success(_table, 50);
    mark_cells(0, _table, SPACES, 50);
    mark_cells(50, _table, SPACES, 50);

        
}

bool STokenizer::get_token(int start_state, string& token) {
    int b_size = strlen(_buffer);
    bool firstq = false;
    bool secondq = false;
    int last_success_pos = _pos;
    //cout << "Current String: " << _buffer;
    for (int i = _pos; i < b_size; i++) {
        // if negative due to foreign character
        if (_buffer[i] < 0) {
            // 256 - _buffer[i] (moves the column to proper place)
            start_state = _table[start_state][MAX_COLUMNS+_buffer[i]];
        }
        else {
            start_state = _table[start_state][_buffer[i]];
        }
        // if success state add to token
        if (is_success(_table, start_state)) {
            
            // update last successful position
            last_success_pos = i;
        }
        else if (start_state == -1 || _buffer[i] == '\0') {
            break;
        }
    }
    for (int i = _pos; i <= last_success_pos; i++) {
        if (_buffer[i] != '\"') {
            // if (_buffer[i] != ',') {
            token += _buffer[i];
            
        }
        
        
    }
    _pos = last_success_pos+1;
    // cout << "Current token: " << token << endl;
    if (token.length() != 0) {    
        return true;
    }
    
    else {    
        return false;
    }
        
}

int STokenizer::get_type(char c) {
    // checks if space
    for (int i = 0; i < 2; i++) {
        if (c == IGNORE[i]) {
            return TOKEN_IGNORE;
        }
    }
    for (int i = 0; i<4; i++) {
        if (c == SPACES[i]) {
            
            return TOKEN_SPACE;
        }
    }
    // checks if alpha
    for (int i = 0; i<53; i++) {
        if (c == ALFA[i]) {
            return TOKEN_ALPHA;
        }
    }
    // checks if digits
    for (int i = 0; i<11; i++) {
        if (c == DIGITS[i]) {
            return TOKEN_NUMBER;
        }
    }
    // checks if operator
    for (int i = 0; i<13; i++) {
        if (c == OPERATORS[i]) {
            return TOKEN_OPERATOR;
        }
    }
    // checks if punc
    for (int i = 0; i<10; i++) {
        if (c == PUNC[i]) {
            return TOKEN_PUNC;
        }
    }    
 

    return TOKEN_UNKNOWN;

}

#ifndef STOKENIZE_H
#define STOKENIZE_H

#include "../../includes/tokenizer/constants.h"
#include "../../includes/tokenizer/state_machine_functions.h"
#include "../../includes/token/token.h"
#include <string>
#include <iostream>
using namespace std;

class STokenizer
{
public:
    STokenizer();
    STokenizer(char str[]);
    bool done();            //true: there are no more tokens
    bool more();            //true: there are more tokens
    //

    //---------------
    //extract one token (very similar to the way cin >> works)
    // grab next token, copy it to t, advance internal_pos to char
    // following the last char
    friend STokenizer& operator >> (STokenizer& s, Token& t );

    //set a new string as the input string
    int get_type(char c);
    void set_string(char str[]);

private:
    //create table for all the tokens we will recognize
    //                      (e.g. doubles, words, etc.)
    void make_table(int _table[][MAX_COLUMNS]);

    //extract the longest string that match
    //     one of the acceptable token types
    // given a pos in a string, what is the longest string of characters
    // that is acceptable by my final state
    // copy thin longest substring into token
    // set position to char following the last char in string
    // or leave unchanged if no token accepted
    bool get_token(int start_state, string& token);

    // input: buffer, pos, table, &token
    // return: bool

    // current_state = 0
    // Look at buffer @ this pos
    // check for foreign chars
    // Loop: 
    // new state = table[current][current_input]
    // cout << old state << "-->" << "ch" << "-->" << new state
    // End Loop
    // my token so far, my last successful pos, my last successful state
    // when leaving the function: pos is in right place

    //---------------------------------
    char _buffer[MAX_BUFFER];       //input string
    int _pos;                       //current position in the string
    static int _table[MAX_ROWS][MAX_COLUMNS];
};


// stk>>token -> operator >> --> get token
//                           <-- token string


// pi is 3.14!
// pi  stk >> token
// ' ' while (!stk.done)
// is   cout<<token
// ' ' stk >> token
// 3.14
// !

// STokenizer stk;
// stk.set_string(s)

#endif // STOKENIZE_H
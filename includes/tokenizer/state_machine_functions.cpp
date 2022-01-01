#include "../../includes/tokenizer/state_machine_functions.h"
#include <iostream>
#include <cstring>
#include <cstdio>
using namespace std;

//Fill all cells of the array with -1
void init_table(int _table[][MAX_COLUMNS]) {
    for (int i = 0; i < MAX_COLUMNS; i++) {
        for (int j = 0; j < MAX_ROWS; j++) {
            _table[j][i] = -1;
        }
    }
}

//Mark this state (row) with a 1 (success)
void mark_success(int _table[][MAX_COLUMNS], int state) {
    _table[state][0] = 1;
}

//Mark this state (row) with a 0 (fail)
void mark_fail(int _table[][MAX_COLUMNS], int state) {
    _table[state][0] = 0;
}

//true if state is a success state
bool is_success(int _table[][MAX_COLUMNS], int state) {
    if (_table[state][0] == 1) {
        return true;
    }
    return false;
}

//Mark a range of cells in the array. 
void mark_cells(int row, int _table[][MAX_COLUMNS], int from, int to, int state) {
    for (from, to; from<=to; ++from) {
        _table[row][to] = state;
        }
}

//Mark columns represented by the string columns[] for this row

// mark the cells state

void mark_cells(int row, int _table[][MAX_COLUMNS], const char columns[], int state) {
    if (strcmp(columns, ALFA) == 0) {
        // A-Z
        for (int i = 'A'; i <= 'Z'; ++i) {
            _table[row][i] = state;
        }
        // a-z
        for (int i = 'a'; i <= 'z'; ++i) {
            _table[row][i] = state;
        }
    }
    else if (strcmp(columns, DIGITS) == 0) {
        // 0 - 9
        for (int i = '0'; i <= '9'; ++i) {
            _table[row][i] = state;
        }
    }
    else if (strcmp(columns, OPERATORS) == 0) {
        // <, =, >
        for (int i = '<'; i <= '>'; ++i) {
            _table[row][i] = state;
        }
        // (,),*,+
        for (int i = '('; i <= '+'; ++i) {
            _table[row][i] = state;
        }
        // {,|,}
        for (int i = '{'; i <= '}'; ++i) {
            _table[row][i] = state;
        }
        // !
        _table[row][33] = state;
        // %
        _table[row][37] = state;
        // &
        _table[row][38] = state;
        // -
        _table[row][45] = state;
    }
    else if (strcmp(columns, SPACES) == 0) {
        _table[row][32] = state;
        _table[row][9] = state;
        _table[row][10] = state;
        _table[row][0] = state;
        _table[row][13] = state;
        _table[row][11] = state;
    }
    else if (strcmp(columns, PUNC) == 0) {
        // ;
        _table[row][58] = state;
        // :
        _table[row][59] = state;
        // ?
        _table[row][63] = state;
        // '
        _table[row][39] = state;
        // .
        _table[row][46] = state;
        // ~
        _table[row][126] = state;
        // !
        _table[row][33] = state;
        // `
        _table[row][96] = state;
        // ,
        _table[row][44] = state;
        // -
        _table[row][45] = state;
    }
    
}

//Mark this row and column
void mark_cell(int row, int _table[][MAX_COLUMNS], int column, int state) {
    _table[row][column] = state;
}

//This can realistically be used on a small table
void print_table(int _table[][MAX_COLUMNS]) {
    for (int i = 0; i<MAX_ROWS; ++i) {
        for (int j = 0; j < MAX_COLUMNS; ++j) {
            cout << _table[i][j] << ' ';
        }
        cout << endl;
    }
}

//show string s and mark this position on the string:
//hello world   pos: 7
//       ^
void show_string(char s[], int _pos) {
    cout << s << "   " << "pos: " << _pos << endl;
    for (int i = 0; i < _pos; ++i) {
        cout << " ";
    }
    cout << "^" << endl << endl;
}
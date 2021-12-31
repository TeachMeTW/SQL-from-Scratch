#include <iostream>
#include <iomanip>
#include <cstring>
#include <string>
#include <fstream>
#include <algorithm>
#include "parser.h"
using namespace std;



Parser::Parser(char* s) {
    // stringq.clear();
    _key.clear();
    _ptree.clear();
    stringq.clear();
    set_string(s);
    make_machine(_matrix);
    build_keywords();
    _pos = 0;
}

void Parser::set_string(const string& input) {
    _inputQ.clear();

    // No difference -- had a simpler method 
    for (int i = 0; i < MAX_BUFFER; i++) {
        this -> _ogtext[i] = '\0';
    }
    for (int i =0; i<input.size();i++) {
        _ogtext[i] = input[i];
    }

    // Tokenize the string
    STokenizer k(_ogtext);
    Token t;
    k>>t;
    while(k.more()) {
        // ignore space and ,
        if (t.type_string() != "SPACE" && t.type_string() != "IGNORE") {
            stringq.push_back(t.token_str());
            // cout <<"pre vector " << t.token_str() << endl;
        }
        
        t = Token();
        k>>t;
    }
    // cout << endl;

}

void Parser::make_machine(int _matrix[][MAX_COLUMNS]) {
    /* TABLE:
       
       | State | Success | Select | Make |Insert| Drop  | Symbol/Value |   Star    |  From  |  Where | Fields | Values |
       |       |  [0]    |    [1] |  [2] |  [3] |  [4]  |    [5]       |    [6]    |  [7]   |    [8] |   [9]  |   [10] |
       | ------|---------|--------|------|------|-------|--------------|-----------|--------|--------|--------|--------|
row 0: |    0  |    0    |   1    |  10  |  20  |  98   |     -1       |    -1     |  -1    |  -1    |   -1   |
       | ------|---------|--------|------|------|-------|--------------|-----------|--------|--------|--------|
       |    1  |    0    |   -1   |  -1  |  -1  |  -1   |      2       |    3      |   -1   |   -1   |   -1   |
       | ------|---------|--------|------|------|-------|--------------|-----------|--------|--------|--------|
       |    2  |    0    |  -1    |  -1  |  -1  |  -1   |      2       |    -1     |   3    |   -1   |  -1    |
       | ------|---------|--------|------|------|-------|--------------|-----------|--------|--------|--------|
       |    3  |    0    |  -1    |  -1  |  -1  |  -1   |      -1      |    -1     |   4    |   -1   |   -1   |
       | ------|---------|--------|------|------|-------|--------------|-----------|--------|--------|--------|
       |    4  |    0    |   -1   |  -1  |  -1  |  -1   |       5      |     -1    |  -1    |   -1   |   -1   |
       | ------|---------|--------|------|------|-------|--------------|-----------|--------|--------|--------|
       |    5  |    0    |   -1   |  -1  | -1   |  -1   |      -1      |    -1     |  -1    |   6    |  -1    |
       | ------|---------|--------|------|------|-------|--------------|-----------|--------|--------|--------|
       |    6  |    0    |  -1    |  -1  | -1   |  -1   |6 (7 if null) |    -1     |  -1    |   -1   |  -1    |
       | ------|---------|--------|------|------|-------|--------------|-----------|--------|--------|--------|
       |    7  |    1    |        |      |      |       |              |           |        |        |        |
       | ------|---------|--------|------|------|-------|--------------|-----------|--------|--------|--------|

    */
    // set all to fail states
    init_table(_matrix);
    // Start State
    _matrix[0][0] = 0;
    // Success States
    _matrix[6][0] = 1; // for select
    _matrix[4][0] = 1; // no need for where
    _matrix[5][0] = 1; // after where
    _matrix[12][0] = 1;

    _matrix[14][0] = 1; // for make/create table
    _matrix[24][0] = 1; // for Insert
    _matrix[99][0] = 1; // For drop


    // Select, Make, Insert, Drop
    _matrix[0][1] = 1; // select
    _matrix[0][2] = 10; // make/create
    _matrix[0][3] = 20; // insert
    _matrix[0][4] = 97; // drop

    // Select
    _matrix[1][5] = 2; // Sets fields
    _matrix[1][6] = 3; // Sets all fields *
    _matrix[2][5] = 2; // if still symbol set more fields
    _matrix[2][7] = 3; // After all conds specified and now from
    _matrix[3][5] = 4; // From condition
    _matrix[3][7] = 3;
    _matrix[4][8] = 5; // Where condition
    _matrix[5][5] = 5; // if still symbol

    _matrix[5][99] = 6; // If nothing else

    // Make/Create table
    _matrix[10][5] = 11; // Sets table name
    _matrix[11][5] = 12; // ensures make/create
    // _matrix[12][5] = 12; // long table name
    _matrix[12][9] = 13; // Fields
    _matrix[13][5] = 14; // continue if fields
    _matrix[14][5] = 14;

    _matrix[14][99] = 14; // If nothing else

    // Insert Into
    _matrix[20][5] = 21; // File name
    _matrix[21][5] = 22; // into
    _matrix[22][10] = 23; // knows its values
    _matrix[22][5] = 22; // into -- useless
    _matrix[23][5] = 24; 
    _matrix[24][5] = 24; // values

    _matrix[22][99] = 23; // if nothing else 

    // Drop
    _matrix[97][5] = 98; // Specifies file to delete
    
    _matrix[98][99] = 99; // nothing else
}

void Parser::build_keywords() {
    _key.clear();
    _key.insert("select", 1);

    _key.insert("make", 2);
    _key.insert("create", 2);
    // _key.insert("table", 97);

    _key.insert("insert", 3);

    _key.insert("drop", 4);
    _key.insert("*", 6);
    _key.insert("from", 7);
    _key.insert("where", 8);
    _key.insert("fields", 9);
    _key.insert("values", 10);

    // _key.insert("\"", 5);
    _key.insert("", 99);

    // cout << _key << endl;
}

mmap_ss Parser::parse_tree() {
    _ptree.clear();

    get_parse_tree();
    // cout << "Every element in _map[\"conditions\"]" << endl;
    // for (int i = 0; i < _ptree["conditions"].size(); i++) {
    //   cout << "|" << _ptree["conditions"][i] << "|";
    // }
    // cout << endl << endl;
   //  cout << endl << _ptree << endl;
    return _ptree;
    
}

bool Parser::get_column() {
    // Can just check in get parse tree
}

bool Parser::get_parse_tree() {
    int last_succ = _pos;
    int start = 0;
    int current = start;
    // for (int i = _pos; i < _inputQ.size(); i++) {
    //     start = _matrix[start][_key.get(_inputQ.pop())];
    //     if (is_success(_matrix, start)) {
    //         last_succ = i;
    //     }
    //     else if (start == -1 || _key.get(_inputQ.top()) == 99 || _key.get(_inputQ.top()) == 98) {
    //         break;
    //     }
    // }
    // for (int i = _pos; i<= last_succ; i++) {
    //     _ptree[]
    // }
    // cout << "String Q: " << stringq << endl;  
    string star = "*";
    string space = " ";
    string yes = "yes";
    string no = "no";
    //cout << "Parsing " << endl;
    // cout << _key << endl;
    // int originalsize = stringq.size();
    for (int i = 0; i < stringq.size() && (current != -1); i++) {
            // cout << "Preif" << endl;
            if (!_key.contains(stringq[i])) {
                _key.insert(stringq[i], 5);
            }
            // cout << "In" << endl;
            string s = stringq[i];
            string token = s;

            // cout << "Token: " << stringq[i] << endl;
            auto column = _key.get(token);
            // cout << "\nfields :" << _ptree["fields"] << endl;
            // cout << "Column: " << column << endl;
            // cout << "Current" << current << endl;
            current = _matrix[current][column];
            // cout << "New state: " << current << endl;
            // cout << _ptree << endl;
            switch (current) {
                case 1: // select
                    _ptree["command"] += token; 
                    break;
                case 2: // make/create
                    _ptree["fields"] += token;
                    break;
                case 3: // insert
                    if (_ptree["fields"].empty()) {
                        _ptree["fields"] += star;
                    }
                    break;
                case 4:
                    _ptree["table_name"] += token;
                    // _ptree["where"] += no;
                    break;
                case 5:
                    if (_ptree["where"].empty()) {
                        // _ptree["where"].clear();
                        _ptree["where"] += yes;
                    }
                    if (token != "where") {
                        _ptree["condition"] += token;
                    }
                    
                    break;
                // MAKE
                case 10:
                    _ptree["command"] += token;
                    break;
                case 11:
                    break;
                case 12:
                    _ptree["table_name"] += token;
                    break;
                case 14:
                    _ptree["col"] += token;
                    break;

                // INSERT
                case 20:
                    _ptree["command"] += token;
                    break;
                case 21:
                    break;
                case 22:
                    _ptree["table_name"] += token;
                    break;
                case 24:
                    _ptree["values"] += token;
                default:
                    break;

        }
        // cout << "Popping" << endl;
        // stringq.pop_back();
        // cout << endl << endl;
        //cout << "new size: " << stringq.size() << endl;
        //cout << i << endl;
        //cout << stringq[i+1] << endl;
    }
    if (is_success(_matrix, current)) {
        stringq.clear();
        return true;
    } 
    else {
        stringq.clear();
        _ptree.clear();
        return false;
    }
}
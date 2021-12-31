#ifndef PARSER_H
#define PARSER_H
#include "../../includes/table/typedefs.h"
#include "../../includes/table/table.h"
#include <fstream>
#include "../../includes/token/token.h"
#include "../../includes/queue/MyQueue.h"
#include "../../includes/binary_files/file_record.h"
#include "../../includes/stack/MyStack.h"
#include "../../includes/token/child_tokens.h"
#include "../../includes/tokenizer/stokenize.h"
using namespace std;

class Parser {
public:

    Parser(char *s); // pass buffer to tokenizer and get it into a vector of tokens then evalutate? 
    // Maybe make switch statements?
    
    void set_string(const string& input); // like the constructor
    void make_machine(int _matrix[][MAX_COLUMNS]); // not sure if needed maybe I can just make switches 
    // but this will handle commands such as make/create,
    // insert into, select(from -> fields /* with or without [and/or, (,)]) or adjacency matrix

    void build_keywords();

    mmap_ss parse_tree();

    bool get_column();
    bool get_parse_tree(); // return true if valid parse tree
    
private:
    vectorstr stringq;
    vectorlong _instructions;
    map_sl _key;
    Queue<Token*> _inputQ; // unused
    mmap_ss _ptree;
    int _pos;
    char _ogtext[MAX_BUFFER];
    int _matrix[MAX_ROWS][MAX_COLUMNS];
};


#endif // PARSER_H
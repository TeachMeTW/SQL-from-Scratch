#ifndef FILE_RECORD_H
#define FILE_RECORD_H

#include <iostream>
#include <iomanip>
#include <string.h>
#include <vector>
#include "utilities.h"

using namespace std;

struct FileRecord{
public:
    //when you construct a FileRecord, it's either empty or it
    //  contains a word
    FileRecord(){
        for (int i = 0; i <= MAX; i++) {
            _record[i][0] = '\0';
        }
        recno = -1;
        //['\0'|  | G | a | r | b | a   |  g  |  e |    ]
    }
    FileRecord(vector<string> values) {
        //cout << "Vec Const fired" << endl;
        for(int i = 0; i < values.size(); i++) {
            //cout << values[i] << endl;
            strncpy(_record[i], values[i].c_str(), MAX);
            //cout << "Copied" << endl;
            
        }
        _record[values.size()][0] = '\0';
        recno = values.size()-1;
    }

    FileRecord(char str[]){
        strncpy(_record[0], str, MAX);
        _record[1][0] = '\0';
    }
    
    FileRecord(string s){
        strncpy(_record[0], s.c_str(), MAX);
    }

    long write(fstream& outs);              //returns the record number
    long read(fstream& ins, long recno);    //returns the number of bytes
                                            //      read = MAX, or zero if
                                            //      read passed the end of file


    friend ostream& operator<<(ostream& outs, const FileRecord& r);
    static const int MAX = 100;
    int recno;
    char _record[MAX+1][MAX+1]; //NULL char
};

#endif
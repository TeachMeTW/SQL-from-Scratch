#include "../../includes/binary_files/file_record.h"
#include "../../includes/stack/MyStack.h"
#include "../../includes/token/child_tokens.h"
#include "table.h"
#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>
#include <algorithm>
#include "../../includes/shunting_yard/shunting_yard.h"
using namespace std;

int Table::_serial = 0;

Table::Table(const string &name) {
    _recordnumbers.clear();
    // untested
    _name = name;
    _filename = name + ".bin";
    _last_rec = 0;
    // Open file with name
    
    // ifstream read
    ifstream n(_name+"_fields.txt");
    string l;
    // read file and push back
    while (getline(n, l)) {
        _fieldnames.push_back(l);
    }
    n.close();
    //cout << _fieldnames.size() << endl;

    // set indices and field map
    // populates the indices multimap
    for (int i = 0; i < _fieldnames.size(); i++) {
        _indices[_fieldnames[i]] = mmap_sl();
        _fieldmap[_fieldnames[i]] = i;
    }
    //cout << "Reindexing:" << endl;
    // Reindex
    ifstream _in2(_name+"_records.txt");
    _in2 >> _last_rec;
    _in2.close();
    fstream _f;
    open_fileRW(_f, (name+".bin").c_str());
    FileRecord out;
    for (int i = 0; i <= _last_rec; i++) {
        out.read(_f, i);
        for (int j = 0; j < _fieldnames.size(); j++) {
            string field(out._record[j]);
            _indices[_fieldnames[j]].insert(field, i);
        }
        
    }
    _f.close();
}


Table::Table(const string& name,const vectorstr fieldnames) {
    // set my variables
    _recordnumbers.clear();
    _last_rec = 0;
    _filename = name + ".bin";
    _fieldnames = fieldnames;
    _name = name;
    // remove existing file
    int removed = (remove((_filename).c_str()));
    // open/create new file for writing
    fstream _f;
    open_fileW(_f, (name + ".bin").c_str());
    // close
    _f.close();
    // new writing stream
    ofstream _in;
    _in.open(name+"_fields.txt");
    // field names + mmap population
    for (int i = 0; i < fieldnames.size(); i++) {
        _in << fieldnames[i] << endl;
        _indices[fieldnames[i]] = mmap_sl();
        _fieldmap[fieldnames[i]] = i;
    }
    _in.close();
    
}

bool Table::both_same_type(const string& item1, const string& item2) {
    // just like the ones from btree array funct
    bool isnum1 = true;
    bool isnum2 = true;
    int num1 = 0;
    int num2 = 0;
    try {
        num1 = stoi(item1);
    }
    catch (exception& e) {
        isnum1 = false;
    }
    try {
        num2 = stoi(item2);
    }
    catch (exception& e) {
        isnum2 = false;
    }
    return isnum1 == isnum2;
}

Table Table::select(vectorstr fieldnames, string arg1, string op, string arg2) {
    vectorstr cond = {arg1, op, arg2};
    //cout << cond << endl;
    //cout << "Select 1" << endl;
    //cout <<  select(fieldnames, cond) << endl;
    return select(fieldnames, cond);
}

Table Table::select(vectorstr fieldnames, vectorstr conditions) { // increment serial
    // run shunting yard
    // return other select
    // cout << "Current q (infix): " << conditions << endl;
    auto posted = Shunting_Yard::convertpostfix(conditions);
    //cout << posted << endl;
    //cout << "Select 2" << endl;
    //cout <<  select(fieldnames, posted) << endl;
    return select(fieldnames, posted);
}

Table Table::select(vectorstr fieldnames, Queue<Token *> postfix) { // increment serial
    bool debug = false;
    _recordnumbers.clear();

    // Stack for everything
    Stack<Token *> genstack;
    // while (!postfix.empty()) {
    //     cout << postfix.pop()->token_str() << endl;
    // }
    auto tcpy = postfix;
    // //cout << "Current q: ";
    // while (!tcpy.empty()) {
    //     cout  << tcpy.pop()->token_str() << " ";
    // }
    cout << endl;
    // Pushes everything not operator into the stack
    while (!postfix.empty()) {
        // RPN SECTION
        //cout << "pushing things into stack" << endl;
        Token* t = postfix.pop();
        if (t->type() != (int)Token::Type::Operator) {
            genstack.push(t);
        }
        else {
            // evaluates operator
            auto resulting = new Resultset();
            // F/S of Stack
            Token* secondin = genstack.pop();
            Token* firstin = genstack.pop();

            // prints out the operation
            if (debug) {
                if (firstin->type() != Resultset().type()) {
                    cout << "\nCurrent operation: " << firstin->token_str() << " " << t->token_str() << " " <<
                    secondin->token_str() << endl;
                }
                else {
                    auto f = (Resultset*)firstin;
                    auto s = (Resultset*)secondin;
                    cout << "\nCurrent operation: |" << f->get_records() << "| " << t->token_str() << " |" <<
                    s->get_records() << "|" << endl;
                }
            }
            // Result Set
            
            // EVALUATION
            auto map = _indices[firstin->token_str()]; // the first field
            auto second = secondin->token_str(); // value trying to fit in
            //cout << firstin->token_str() << " " << secondin->token_str() << endl;
            //cout << map << endl;

            // Checking if same column type
            if (both_same_type((*map.begin()).key, second)) {
                if (t->token_str() == "<") {
                    // auto map = _indices[firstin->token_str()];
                    // cout << *(map.begin()) << "looking for " << *(map.lower_bound(secondin->token_str())) << endl;
                    auto it = map.begin();
                    //cout << map << endl;
                
                    // Iterate through the map
                    for (; it != map.lower_bound(secondin->token_str()) && it != map.end(); it++) {
                        //cout << "Value: " << (*it).value_list << " " << "Key: " << (*it).key << endl;
                        resulting->add((*it).value_list);
                        //cout << "Added" << endl;
                    }
                    //cout << "After loop" << endl;
                    //cout << *it << endl;
                    //cout << "S" << endl;

                    // Needed this due to it undershooting which keeps adding while key is less than whats trying to fit in
                    while (((*it).key < secondin->token_str()) && it != map.end()) {
                        resulting->add((*it).value_list);
                        it++;
                    }
                    //cout << "If" << endl;
                    //cout << "post loop" << endl;
                }
                else if (t->token_str() == "<=") {
                    // auto map = _indices[firstin->token_str()];
                    auto it = map.begin();
                    // cout << *it << endl;
                    // cout << secondin->token_str() << endl;
                    // cout << "Looking for" <<*map.lower_bound(secondin->token_str()) << endl;
                    // cout << map << endl;

                    // Iterate through the map
                    for (; it != map.lower_bound(secondin->token_str()) && it != map.end(); it++) {

                        resulting->add((*it).value_list);

                        // Run the < loop, make sure lower bound is equal to secondin, call resulting add at the end using lowerbound it

                    }
                    //cout << "Post" << endl;
                    auto next = it;
                    //cout << bool(next != map.end()) << endl;
                    //cout << "check" << endl;

                    // Same as last but <=
                    while (((*it).key <= secondin->token_str()) && it != map.end()) {
                        resulting->add((*it).value_list);
                        it++;
                    }

                }
                else if (t->token_str() == ">") {
                    
                    // Run the loop but with upper bound, start at upper till end
                    auto it = map.upper_bound(secondin->token_str());
                    for (; it != map.end(); it++) {
                        resulting->add((*it).value_list);

                    }
                    // if ((*it).key > secondin->token_str()) {
                    //     resulting->add((*it).value_list);
                    // }               
                }
                else if (t->token_str() == ">=") {
                    //auto map = _indices[firstin->token_str()];
                    auto it = map.lower_bound(secondin->token_str());
                    if (it != map.end() && (*it).key == secondin->token_str()) {
                        resulting -> add((*it).value_list);
                        it++;
                    }
                    // If it undershoots
                    for (; it != map.end(); it++) {
                        resulting->add((*it).value_list);

                    }
                    // if ((*it).key >= secondin->token_str()) {
                    //     resulting->add((*it).value_list);
                    // }      
                    // Run upper loop, make sure upper is equal to secondin, call resulting add at the end
                }
            }
            // cout << "Current token: " << t->token_str() << endl;
            
            // cout << _indices[firstin->token_str()] << endl;

            if (t->token_str() == "=") {
                // cout << "Equal Operator Detected" << endl;
                //auto map = _indices[firstin->token_str()];
                // cout << _indices[firstin->token_str()]  << endl;
                // cout << (*map.begin()).key << endl;
                // cout << secondin->token_str() << endl;
                // uses == operator to see if exist
                for (auto it = map.begin(); it != map.end(); it++) {
                    if (secondin->token_str() == (*it).key) {
                        // cout << "A match" << endl;
                        resulting->add((*it).value_list);
                    }
                    

                }

                // cout << "It was not found" << endl; 
                // run find iterator and resulting add if found
            }
            if (t->token_str() == "!=") {
                //auto map = _indices[firstin->token_str()];

                // uses != operator
                // cout << (*map.begin()).key << endl;
                // cout << secondin->token_str() << endl;
                for (auto it = map.begin(); it != map.end(); it++) {
                    if (secondin->token_str() != (*it).key) {
                        // cout << "Not a match" << endl;
                        resulting->add((*it).value_list);
                    }
                    

                }
                // loop through all iterators till end and check if equal, if equal dont add
            }
            if (t->token_str() == "and") {
                //cout << "Before things " << endl;

                //create resultsets for the two items
                Resultset* f = (Resultset*) secondin;
                Resultset* s = (Resultset*) firstin;
                //cout << "Before auto" << endl;
                //cout << "f: " << *f << " " << "s: " << *s << endl;
                auto fget = f->get_records();
                auto sget = s->get_records();
                // sort for easier debugging
                sort(fget.begin(), fget.end());
                sort(sget.begin(), sget.end());
                //cout << "Before loop" << endl;
                for (int i = 0; i < fget.size(); i++) {
                    auto hold = std::find(sget.begin(), sget.end(), fget[i]);
                    if (hold != sget.end()) {
                        //cout << "Adding non duplicate" << endl;
                        resulting->add(fget[i]);
                    }
                }
            }
            if (t->token_str() == "or") {
                Resultset* f = (Resultset*) firstin;
                Resultset* s = (Resultset*) secondin;
                auto fget = f->get_records();
                auto sget = s->get_records();

                sort(fget.begin(), fget.end());
                sort(sget.begin(), sget.end());
                resulting->add(fget);
                //cout << "Before loop" << endl;
                for (int i = 0; i < sget.size(); i++) {
                    auto hold = std::find(fget.begin(), fget.end(), sget[i]);
                    if (hold == fget.end()) {
                        //cout << "Adding non duplicate" << endl;
                        resulting->add(sget[i]);
                    }
                }
            }
            if (debug) {
                cout << "Current result set: " << resulting->get_records() << endl;
            }
            
            genstack.push(resulting);
            
        }
    }
    //cout << "Queue empty" << endl;
    
    // Get the current resultset
    Resultset* r = (Resultset*)genstack.top();
    //cout << r->get_records().size() << endl;
    //cout << "The Record" << endl;
    //cout << r->get_records() << endl;
    string tname = _name + "_" + to_string(_serial) + "_" + " etc";

    // if * and only 1 set to all fields (from parser)
    if (fieldnames[0] == "*" && fieldnames.size() == 1) {
        fieldnames = _fieldnames;
    }

    // Create a new table
    Table STable(tname, fieldnames);
    fstream _f;

    open_fileRW(_f, (_name+".bin").c_str());
    FileRecord out;
    _recordnumbers = r->get_records();
    // Populate the table
    for (int i = 0; i < r->get_records().size(); i++) {
        out.read(_f, r->get_records()[i]);
        vectorstr localfield;
        // _recordnumbers.push_back(r->get_records()[i]);
        for (int j = 0; j < fieldnames.size(); j++) {
            string field(out._record[_fieldmap[fieldnames[j]]]);
            localfield.push_back(field);
        }
        //cout << localfield << endl;
        STable.insert_into(localfield);
        //cout << _indices["age"] << endl;
        
    }
    _f.close();

    // Update the serial
    _serial++;
    //cout << "Returning Table" << endl;
    //cout << "Select 3" << endl;
    //cout << STable << endl;
    return STable;
    // cout << endl << endl;
    // r->get_records();
    // cout << endl;
}

Table Table::select_all() {
    _recordnumbers.clear();
    string tname = _name + "_" + to_string(_serial) + "_" + " etc";
    // New table
    Table STable(tname, _fieldnames);

    fstream _f;
    open_fileRW(_f, (_name+".bin").c_str());
    FileRecord out;
    for (int i = 0; i <= _last_rec; i++) {
        out.read(_f, i);
        _recordnumbers.push_back(i);
        vectorstr localfield;
        for (int j = 0; j < _fieldnames.size(); j++) {
            string field(out._record[j]);
            localfield.push_back(field);
        }
        STable.insert_into(localfield);
        
    }
    _f.close();

    // Update the serial
    _serial++;
    return STable;
}

Table Table::select_all(vectorstr fields) {
    // A special case from sql/parser where fields are determined but no where statement
    _recordnumbers.clear();
    string tname = _name + "_" + to_string(_serial) + "_" + " etc";
    Table STable(tname, fields);
    fstream _f;
    open_fileRW(_f, (_name+".bin").c_str());
    FileRecord out;
    for (int i = 0; i <= _last_rec; i++) {
        out.read(_f, i);
        vectorstr localfield;
        _recordnumbers.push_back(i);
        for (int j = 0; j < fields.size(); j++) {
            string field(out._record[_fieldmap[fields[j]]]);
            localfield.push_back(field);
        }
        //cout << localfield << endl;
        STable.insert_into(localfield);
        //cout << _indices["age"] << endl;
        
    }
    _f.close();


    _serial++;
    return STable;
}

vector<long> Table::select_recnos() {
    // std::sort(_recordnumbers.begin(), _recordnumbers.end());
    // cout << "Table Rec " << _recordnumbers << endl;
    return _recordnumbers;
}

// Not sure if correct

void Table::insert_into(vectorstr input) {
    fstream _f;
    open_fileW(_f, _filename.c_str());
    _last_rec = -1;
    FileRecord in(input);
    // cout << _last_rec << endl;
    _last_rec = in.write(_f);
    // cout << _last_rec << endl;
    ofstream _in;
    _in.open(_name+"_records.txt");
    _in << _last_rec << endl;
    _in.close();
    for (int i = 0; i < _fieldnames.size(); i++) {
        _indices[_fieldnames[i]].insert(input[i], _last_rec);
    }
    _f.close();
}

void Table::read() {
    //cout << _filename << endl;

}

Table makes_table() {
    Table r("test", {"last", "first"});
    r.insert_into({"Biden", "Joe"});
    r.insert_into({"Mama", "Joe"});
    r.insert_into({"Blow", "Joe"});
    r.insert_into({"Pelocy", "Nancy"});
    return r;
}
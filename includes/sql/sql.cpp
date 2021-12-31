#include "sql.h"

SQL::SQL(string file) {
    // reads from file and writes into another
    ofstream _f("output.txt");
    ifstream k(file);
    string line;
    int commands = 0;
    cout << "Opening " << file << endl;
    while (getline(k,line)) {
        if (line.length()>1 && !(line[0] == '/' && line[1] == '/')) {
            commands++;
            cout << "\n[" << commands<< "]" << "  Command: " << line << endl;
            _f << "\n[" << commands<< "]" << "  Command: " << line << endl;
            command(line); // WIP cant figure out why it doesn't want to write in
            cout << endl << "Records Selected: " << select_recnos() << endl;
            _f << endl << "Records Selected: " << select_recnos() << endl;
            cout << "SQL DONE" << endl;
            _f << "SQL DONE" << endl;
        }
        else {
            cout << endl << line << endl;
            _f << endl << line << endl;
        }
    }
    k.close();
    _f.close();
}

Table SQL::command(string c) {
    auto in = (char *)c.c_str();
    auto localparse = Parser(in);
    auto out = localparse.parse_tree();
    //cout << out << endl;
    string tablename;
    vectorstr fields;
    vectorstr values;
    string command = out["command"][0];
    // compares to the parser's map and passes it in to check which commands and run the respective functions
    if (command == "make" || command == "create") {
        fields = out["col"];
        cout << fields << endl;
        tablename = out["table_name"][0];
        return Table(tablename,fields);
    }
    else if (command == "select") {
        _rec.clear();
        tablename = out["table_name"][0];
        Table k(tablename.c_str());
        fields = out["fields"];
        if (out["where"].empty() && fields[0] == "*") {
            // cout << "select all" << endl;
            auto cache = k.select_all();
            _rec = k.select_recnos();
            cout << cache << endl;
            _local = cache;
            return cache;
        }
        else if (out["where"].empty() && out["condition"].empty()) {
            //cout << out["conditions"] << endl;
            auto cache = k.select_all(fields);
            _rec = k.select_recnos();
            cout << cache << endl;
            _local = cache;
            //cout << "Selected" << endl;
            return cache;
        }
        else {

            auto cache = k.select(fields, out["condition"]);
            _rec = k.select_recnos();
            cout << cache << endl;
            _local = cache;
            //cout << "Selected" << endl;
            return cache;
        }
        
    }
    else if (command == "insert") {
        // _rec.clear();
        tablename = out["table_name"][0];
        values = out["values"];
        Table k(tablename.c_str());
        k.insert_into(values);
        // _rec = k.select_recnos();
        return k;
    }
    
}
#ifndef SQL_H
#define SQL_H
#include "../../includes/table/table.h"
#include <algorithm>
#include "../../includes/parser/parser.h"

class SQL {
public:
    SQL() {}
    SQL(string file);
    Table command(string c);
    vectorlong select_recnos() {
        // cout << "SQL REC: " << _rec << endl;
        if (_rec.size() == 0) {
            cout << "NOTHING!" << endl;
        }
        return _rec;
    }

private:
    Table _local; // UNUSED
    vectorlong _rec;
};








#endif // SQL_H
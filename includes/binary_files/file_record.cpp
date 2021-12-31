#include "file_record.h"
#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>
using namespace std;

long FileRecord::write(fstream &outs){
    //r.write(f); //take the Record r and write it into file f
    //  and return the record number of this Record

    //write to the end of the file.
    outs.seekp(0, outs.end);


    long pos = outs.tellp();    //retrieves the current position of the
                                //      file pointer

    //pay attention to this:
    //outs.write(&record[0], sizeof(record));
    // for (int i = 0; i < MAX; i++) {
    outs.write(&_record[0][0], sizeof(_record));
    // }
    return pos/sizeof(_record);  //record number
}

long FileRecord::read(fstream &ins, long recno){
    //returns the number of bytes read.
    //    r.read(f, 6);
    //    cout<<r<<endl;
    long pos= recno * sizeof(_record);
    ins.seekg(pos, ios_base::beg);

    //ins.read(&_record[0], sizeof(_record))
    // for (int i = 0; i < MAX; i++) {
    ins.read(&_record[0][0], sizeof(_record));
    //}
    
    //row++;
    //cout << "Read" << endl;
    //don't you want to mark the end of  the cstring with null?
    //_record[] => [zero'\0'trash trash trash trash]
    //don't need the null character, but for those of us with OCD and PTSD:
    //cout << "Setting end" << endl;
    //cout << ins.gcount();
    //_record[ins.gcount()][0] = '\0';
    //cout << "return" << endl;
    return ins.gcount();

}

ostream& operator<<(ostream& outs,
                    const FileRecord& r){
    int i = 0;
    while (r._record[i][0] != '\0') {
        outs << setw(20) <<  r._record[i];
        i++;
    }
    return outs;
}


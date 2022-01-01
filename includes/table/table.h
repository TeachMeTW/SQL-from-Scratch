#ifndef TABLE_H
#define TABLE_H
#include "typedefs.h"
#include <fstream>
#include "../../includes/token/token.h"
#include "../../includes/queue/MyQueue.h"
#include "../../includes/binary_files/file_record.h"
#include "../../includes/stack/MyStack.h"
#include "../../includes/token/child_tokens.h"
using namespace std;

// TODO:
// finish class
// set fields
// get fields
// reindex

class Table {
 public:
   Table() {
     _last_rec = 0;
     _name = "";
     _filename = "";
   }
   Table(const string &name);
   Table(const string &name, const vectorstr fieldnames);

  //  Table(const Table& other);
  //  ~Table();
  //  Table& operator = (const Table& RHS);
   void read(); // unneeded
   void insert_into(vectorstr inputval);

   vector<long> select_recnos();

   Table select(vectorstr fieldnames, string arg1, string op, string arg2);
   Table select(vectorstr fieldnames, vectorstr conditions);
   Table select(vectorstr fieldnames, Queue<Token* > postfix); // increment serial
    
   Table select_all();
   Table select_all(vectorstr field);

   bool both_same_type(const string& item1, const string& item2); // comparing the column types

   friend ostream& operator<<(ostream& outs, const Table &t) {
      //cout << "Entering O" << endl;
      int temprec = t._last_rec;
      auto a = t._indices;
      auto k = a[t._fieldnames[0]];
      //cout << t._fieldmap << endl;
      //  outs << t._last_rec << endl << t._filename << endl << t._name << endl;
      //cout << t._indices << endl;
      if (k.size() ) {
        temprec++;
      }
      outs << "Table name: " << t._name << ", records: " << temprec << endl;
      outs << setw(20) << "record";
      for (int i = 0; i < t._fieldnames.size(); i++) {
        outs <<setw(20)<< t._fieldnames[i];
      }
      outs << endl;  
      // method 1:Iterate through table
      // I think I can use this for select -- same idea
      /*
      map_mml::Iterator it;
      for (it = t._indices.begin(); it != t._indices.end(); it++) {
          mmap_sl::Iterator it2;
          Pair<string, mmap_sl> var = *it;
          // Print out Column of key
          outs << var.key << ": " << endl;
          mmap_sl m = var.value;
          for (it2 = m.begin(); it2 !=  m.end(); it2++){
              MPair<string, long> mp = *it2;
              // printing out multimap
              outs << mp.key << " : ";
              for (int i = 0; i < mp.value_list.size(); i++) {
                // printing out record number
                outs << "recno: " << mp.value_list[i] << endl;
                
              }
          }
          outs << endl;
      }
      */
      // Method 2: Cracked read void function
      //outs << t._indices << endl;
      fstream _f;
      open_fileRW(_f, t._filename.c_str());
      FileRecord out;
      int i = 0;
      long bytes = out.read(_f, i);
      while (bytes > 0) {
          cout << setw(20) << i << out << endl;
          i++;
          bytes = out.read(_f, i);
      }
      _f.close();
      //t.read();
      //outs << "Read done" << endl;
      return outs;
      //outs << t._indices << endl;

  } 
 private:
 // Not sure if the variables are correct either
  string _name;
  // FileRecord _tablerecord;
  // Map<string, FileRecord> _fieldlist;
  // map vector of all index structures
  string _filename;
  //int recno;
  vectorlong _recordnumbers;
  vectorstr _conditions; 
  vectorstr _fieldnames; // field names vector
  map_sl _fieldmap;  // map of field names
  map_mml _indices; // map of multimaps
  
  int _last_rec;
  static int _serial; // unique table names


};

Table makes_table();

#endif // TABLE_H
#include <iostream>
#include <iomanip>
#include <stdio.h>
#include "includes/tokenizer/ftokenize.h"
#include "includes/bplustree/multimap.h"
#include "includes/bplustree/map.h"
#include "includes/sql/sql.h"
#include <string>
#include <cctype>
#include <random>



using namespace std;

MMap<string, long> get_word_indices(char* file_name);
void swap(int &x, int &y);
int random(int low, int high);
void shuffle(int *a, unsigned int sz);

void solitude_test();
void intro();
void sql_test();
void actual();
void automat(string in);
void info();
void btree_test();

int main(int argc, char *argv[])
{
    intro();

    return 0;
}

void intro() {
    cout << endl << setw(35) << "Main Menu" << setw('*');
    cout << "\n\nCommand List:" << endl;;
    cout << endl << "[Run Solitude (S)]" << endl << "[Run SQL (Q)]" <<
    endl  << "[Run BTree Test (B)]" << endl << "[Instructions/Info (I)]" << endl << "[Quit (E)]" << endl << endl; 
    cout << "Please input a command" << endl;
    char input;
    cin >> input;
    input = toupper(input);
    while (input != 'S' && input != 'Q' && input != 'I' && input != 'B' && input != 'E') {
        cout << "Invalid Input please try again" << endl;
        cin >> input;
        input = toupper(input);
    }
    switch (input) {
        case 'S':
            cout << "Running Solitude Test" << endl;
            // _sleep(3);
            solitude_test();
            break;
        case 'Q':
            cout << "Running SQL Test" << endl;
            // _sleep(3);
            sql_test();
            break;
        case 'I':
            info();
            break;
        case 'B':
            cout << "Running BTree Test" << endl;
            // _sleep(3);
            btree_test();
            break;
        case 'E':
            terminate();
    }
}


void btree_test() {
  int b[201] = {};
  int c[201] = {};
  for (int i = 0; i < 201; i++) {
    b[i] = i;
  }
  cout << "Before scramble" << endl;
  for (auto i : b) {
      cout  << i << " ";
  }
  cout << endl;
  for (int i = 0; i < 201; i++) {
    c[i] = i;
  }
  cout << "\nShuffling an array of numbers" << endl << endl;
  shuffle(c, 201);
  for (auto i : c) {
      cout  << i << " ";
  }
  cout << endl << endl;
  BPlusTree<int> bts(c, 201);
  cout << bts << endl;
    char yn;
    cout << "Back to main menu?" << endl;
        cin >> yn;
        yn = toupper(yn);
        while (yn != 'Y' && yn != 'N') {
            cout << "Invalid Input please try again" << endl;
            cin >> yn;
            yn = toupper(yn);
        }
    if (yn == 'Y') {
        intro();
    }
}



void info() {
        cout << endl << setw(35) << "INFO" << setw('*');
        vectorstr info = {
        "<CREATE | MAKE> : {  <create | make> table <TABLE_NAME> fields <FIELD_NAME> [, <FIELD_NAME>...]  }",
        "<INSERT> : { insert <INTO> <TABLE_NAME> values <VALUE> [, <VALUE>...]      }",
        "<SELECT> : {  select <* | FIELD_NAME> [, ,FIELD_NAME>...] from <TABLE_NAME>",
			"where <FIELD_NAME> <RELATIONAL_OPERATOR> <VALUE>",
            "[<LOGICAL_OPERATOR> <FIELD_NAME> <RELATIONAL_OPERATOR> <VALUE>...] }",
        "<VALUE>  : A string of alphanumeric characters, or a string of alphanumeric characters and spaces enclosed by double quotation marks: \"Jean Luise\", Finch, 1923",
        "<RELATIONAL OPERATOR> : [ = | > | < | >= | <= ]",
        "<LOGICAL OPERATOR>    : [and | or]" };

        cout << "\n\nCommand List:" << endl;;
        cout << endl << "[Info about Solitude (S)]" << endl << "[Info about SQL (Q)]" <<
        endl  << "[Info about BTree (B)]" << endl << endl;
        cout << "Please input a command" << endl;
        char input;
        cin >> input;
        input = toupper(input);
        while (input != 'S' && input != 'Q' && input != 'B') {
            cout << "Invalid Input please try again" << endl;
            cin >> input;
            input = toupper(input);
        }
        switch (input) {
            case 'S':
                cout << "\n\n\nSolitude: " << endl;
                cout << "Solitude tests how the multimap functions. The Solitude file is tokenized and each word is assigned the data of position they appear in. Each word should have multiple values in order to show a functioning multimap" << endl;
                cout << endl << endl;
                break;
            case 'Q':
                cout << "\n\n\nSQL Commands" << endl << info << endl << endl;
                cout << "Those are the command templates; make/create to initiate table, insert to add data, select to choose certain data" << endl;
                cout << "Uses Map and Multimaps with a B+Tree base which writes into a binary and text file for storage" << endl << endl << endl;
                break;
            case 'B':
                cout << "\n\nBTree: " << endl;
                cout << "A self balancing tree where it maintains sorted data and allows for searches, insertions, access, and deletion in log(n)" << endl << endl;
                break;
        }
        char yn;
        cout << "Back to main menu?" << endl;
            cin >> yn;
            yn = toupper(yn);
            while (yn != 'Y' && yn != 'N') {
                cout << "Invalid Input please try again" << endl;
                cin >> yn;
                yn = toupper(yn);
            }
        if (yn == 'Y') {
            intro();
        }
}


void sql_test() {
    cout << endl << setw(35) << "SQL Test" << setw('*');
    cout << "\n\nCommand List:" << endl;;
    cout << endl << "[Run from File (F)]" << endl << "[New Input (N)]" << endl;
    cout << "Please input a command" << endl;
    char input;
    cin >> input;
    input = toupper(input);
    while (input != 'F' && input != 'N') {
        cout << "Invalid Input please try again" << endl;
        cin >> input;
        input = toupper(input);
    }
    switch (input) {
        case 'N':
            actual();
            break;
        case 'F':
            cout << "Enter file name with txt included" << endl;
            string input;
            cin >> input;
            automat(input);
            break;
    }
    char yn;
    cout << "Back to main menu?" << endl;
        cin >> yn;
        yn = toupper(yn);
        while (yn != 'Y' && yn != 'N') {
            cout << "Invalid Input please try again" << endl;
            cin >> yn;
            yn = toupper(yn);
        }
    if (yn == 'Y') {
        intro();
    }
}

void automat(string in) {
    SQL s(in);
    char yn;
    cout << "Back to main menu?" << endl;
        cin >> yn;
        yn = toupper(yn);
        while (yn != 'Y' && yn != 'N') {
            cout << "Invalid Input please try again" << endl;
            cin >> yn;
            yn = toupper(yn);
        }
    if (yn == 'Y') {
        intro();
    }
}


void actual() {
    SQL sql;
    Table t;
    string in;
    char yn;
    bool done = false;
    while (!done) {
        cout << "Enter SQL Command" << endl;
        getline(cin >> ws, in);
        // cout << in;
        if (in.size() > 3 && in[0] == 'S') {
            // cout << "Test" << endl;
            cout << sql.command(in);
        }
        else {
            sql.command(in);
        }
        cout << "Next command? y/n" << endl;
        cin >> yn;
        yn = toupper(yn);
        while (yn != 'Y' && yn != 'N') {
            cout << "Invalid Input please try again" << endl;
            cin >> yn;
            yn = toupper(yn);
        }
        if (yn == 'N') {
            done = true;
        }

    }
    cout << "Back to main menu?" << endl;
        cin >> yn;
        yn = toupper(yn);
        while (yn != 'Y' && yn != 'N') {
            cout << "Invalid Input please try again" << endl;
            cin >> yn;
            yn = toupper(yn);
        }
    if (yn == 'Y') {
        intro();
    }
}


void solitude_test(){
    MMap<string, long> word_indices;
    word_indices = get_word_indices("solitude.txt");
    cout<<endl<<endl<<endl;

    //list all nodes of the index mmap:
    for (MMap<string, long>::Iterator it = word_indices.begin();
         it != word_indices.end(); it++){
        cout<<*it<<endl;
    }


    cout<<endl<<endl<<endl;
    cout<<"---------------------------------------------------"<<endl;
    string this_word = "ice";
    cout<<"---------------------------------------------------"<<endl;
    cout<<"Indices of \""<<this_word<<"\""<<endl;
    //list indices of this_word:
    if (word_indices.contains(this_word)){
        cout<<this_word<<": "<<word_indices[this_word]<<endl;
    }
    cout<<endl<<endl<<endl;

    cout<<"---------------------------------------------------"<<endl;
    string from = "ask";
    string to = "asker";
    //list from .. to:
    cout<<"listing indices from \""<<from<<"\" to \""<<to<<"\""<<endl;
    cout<<"---------------------------------------------------"<<endl;
    for (MMap<string, long>::Iterator it =
                word_indices.lower_bound(from);
         it != word_indices.upper_bound(to) &&
         it != word_indices.end(); it++){
        cout<<*it<<endl;
    }

    cout <<endl<<endl<<endl<< "========== E N D  ====================" << endl;
    char yn;
    cout << "Back to main menu?" << endl;
        cin >> yn;
        yn = toupper(yn);
        while (yn != 'Y' && yn != 'N') {
            cout << "Invalid Input please try again" << endl;
            cin >> yn;
            yn = toupper(yn);
        }
    if (yn == 'Y') {
        intro();
    }
}

MMap<string, long> get_word_indices(char* file_name){
    const bool debug = false;
    MMap<string, long> word_indices;
    FTokenizer ftk("solitude.txt");
    Token t;
    long count = 0;

    ftk >> t;
    while (ftk.more()){
        //only the "words"
        if (t.type_string() == "ALPHA"){
            string s;
            s = t.token_str();
            word_indices[s] += count;
            count++;
            if (debug)
                cout<<"|"<<t.token_str()<<"|"<<endl;
        }
        ftk >> t;
    }
    return word_indices;
}

int random(int low, int high){
  //call srand in main if you want better random numbers.
  return static_cast<int>(low + (rand() % static_cast<int>((high - low + 1))));
}

void swap(int &x, int &y) {

  int temp = x;
  x = y;
  y = temp;

}

void shuffle(int *a, unsigned int sz) {

  int s = sz -1;
  for (int i = 0; i < s; i++) {
    int rdm1 = random(0, s);
    int rdm2 = random(0, s);
    swap(a[rdm1], a[rdm2]);
  }
}
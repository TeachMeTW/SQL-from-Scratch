#include "gtest/gtest.h"
#include <cmath>
#include <iostream>
#include <iostream>
#include <iomanip>
#include <string>
#include "../../includes/table/typedefs.h"
#include "../../includes/stl_utils/vector_utilities.h"
#include "../../includes//bplustree/map.h"
#include "../../includes//bplustree/multimap.h"


//------------------------------------------------------------------------------------------
//Files we are testing:
#include "../../includes/parser/parser.h"
#include "../../includes/sql/sql.h"


//------------------------------------------------------------------------------------------
using namespace std;


const vector<string> command_list = {

    //************************************************************************

    // CREATE AND INSERT
    //************************************************************************
  
    //---- employee table ----------
    "make table employee fields  last, first, dep",
    "insert into employee values Blow, Joe, CS",
    "insert into employee values Johnson, \"Jimmy\", Chemistry",
    "insert into employee values Yang, Bo, CS",
    "insert into employee values \"Jackson\", Billy, Math",
    "insert into employee values Johnson, Billy, \"Phys Ed\"",
    "insert into employee values \"Van Gogh\", \"Jim Bob\", \"Phys Ed\"",
    
    //----- student table ----------
    "make table student fields fname, lname, major, age",
    "insert into student values Flo, Yao, CS, 20",
    "insert into student values \"Flo\", \"Jackson\",  Math, 21",
    "insert into student values Calvin, Woo, Physics, 22",
    "insert into student values \"Anna Grace\", \"Del Rio\", CS, 22",
    "insert into student values \"Teresa Mae\", Yao, CS, 25",


    "select * from student",
    "select * from employee",
    //************************************************************************
    // SIMPLE SELECT
    //************************************************************************
    "select * from student",
    //------- simple strings -------------------
    "select * from student where lname = Jackson",
    //----- quoted strings ---------------------
    "select * from student where lname = \"Del Rio\"",
    //-------- non-existing string ------------------
    "select * from student where lname = \"Does Not Exist\"",
    //************************************************************************
    // RELATIONAL OPERATORS:
    //************************************************************************
    //.................
    //:Greater Than   :
    //.................
    "select * from student where lname > Yang",
    //. . . . . . . . . . . . . (Greater Than: Non-existing) . . . . . . . . . 
    "select * from student where age > 50",
    //. . . . . . . . . . . . . (Greater than: last item) . . . . . . . . . . 
    "select * from student where age  > 53",
    //. . . . . . . . . . . . . (Greater Than: Passed last item) . . . . . . . 
    "select * from student where age > 54",
    //.................
    //:Greater Equal  :
    //.................
    "select * from student where lname >= Yang",
    //. . . . . .  (Greater Equal non-existing: ) . . . . . . . . . . .
    "select * from employee where last >= Jack",
    //.................
    //:Less Than      :
    //.................
    //. . . . . . . . . . . . . (Less Than: Non-existing) . . . . . . . . . . 
    "select * from student where lname < Jackson",
    //. . . . . . . . . . . . . (Less than: first item) . . . . . . . . . . . 
    "select * from student where age  < 20",
    //. . . . . . . . . . . . . (Less Than: before first item) . . . . . . . . 
    "select * from student where age  < 19",
    //.................
    //:Less Equal     :
    //.................
    "select * from student where lname <= Smith",
    //. . . . . .  (Less Equal non-existing: ) . . . . . . . . . . .
    "select * from employee where last <= Peach",
    //************************************************************************
    // LOGICAL OPERATORS
    //************************************************************************
    //.................
    //:AND            :
    //.................
    "select * from student where fname = \"Flo\" and lname = \"Yao\"",
    //.................
    //:OR            :
    //.................
    "select * from student where fname = Flo or lname = Jackson",
    //.................
    //:OR AND         :
    //.................
    "select * from student where fname = Flo or major = CS and age <= 23",
    //.................
    //:AND OR AND     :
    //.................
    "select * from student where age <30 and major=CS or major = Physics and lname = Jackson",
    //.................
    //:OR AND OR      :
    //.................
    "select * from student where lname = Yang or major = CS and age < 23 or lname = Jackson",
    "select * from student where ((age>=25 or lname = Yao) and (fname = \"Teresa Mae\" or Major = CS)) and (fname = Mao or lname < Shi)"

};

const int MAKE_TABLE_COMMANDS = 13;
const int SELECT_COMMANDS = 36;

bool parser_basic(bool debug = false){

  cout<<"\n\n\n------------------------\n\n"<<endl;
  string str;
  char s[300];
  str = "make table student fields last, first, age, major, class";
  strcpy(s, str.c_str());
  Parser parser(s);
  mmap_ss ptree;
  ptree= parser.parse_tree(); 
  cout<<"input: ["<<s<<"]"<<endl<<endl;
  ptree.print_lookup();   //multimap function to print mmap as a table
  cout << endl<<endl;

  str = "insert into student values Jones,\"Sue Ellen\", 20, CS, Fr ";
  strcpy(s, str.c_str());
  parser.set_string(s);
  ptree= parser.parse_tree(); 
  cout<<"input: ["<<s<<"]"<<endl<<endl;
  ptree.print_lookup();
  cout << endl<<endl;

  str = "select * from student";
  strcpy(s, str.c_str());
  parser.set_string(s);
  ptree= parser.parse_tree(); 
  cout<<"input: ["<<s<<"]"<<endl<<endl;
  ptree.print_lookup();
  cout << endl<<endl;

  str = "select lname, fname from student where lname = \"Yao\" and fname = \"Flo\"";
  strcpy(s, str.c_str());
  parser.set_string(s);
  ptree= parser.parse_tree(); 
  cout<<"input: ["<<s<<"]"<<endl<<endl;
  ptree.print_lookup();
  cout << endl<<endl;

  str = "select age, lname, fname from student where fname = \"Mary Ann\" and lname = Yao";
  strcpy(s, str.c_str());
  parser.set_string(s);
  ptree= parser.parse_tree(); 
  cout<<"input: ["<<s<<"]"<<endl<<endl;
  ptree.print_lookup();
  cout << endl<<endl;

  str = "select * from student where ((age>=25 or lname = Yao) and (fname = \"Teresa Mae Took This L.\" or Major = CS)) and (fname = Mao or lname < Shi)";
  strcpy(s, str.c_str());
  parser.set_string(s);
  ptree= parser.parse_tree(); 
  cout<<"input: ["<<s<<"]"<<endl<<endl;
  ptree.print_lookup();
  // for (int i = 0; ptree["condition"].size(); i++) {
  //   cout << ptree["condition"][i] << endl;
  // }
  
  cout << endl<< endl;

  cout << "\n\n\n----- DONE -------------\n\n"
       << endl;
  return true;
}


bool test_tokenize(bool debug = false){
  string str;
  char s[300];  
  Parser parser(s);
  mmap_ss ptree;
  str = "make table \"Barkeshli's Class\" fields depression, anger, denial, major, grade";
  strcpy(s, str.c_str());

  parser.set_string(s);
  ptree= parser.parse_tree(); 
  cout<<"input: ["<<s<<"]"<<endl<<endl;
  ptree.print_lookup();   //multimap function to print mmap as a table
  cout << endl;

  str = "create table Workers fields salary age";
  strcpy(s, str.c_str());
  parser.set_string(s);
  ptree= parser.parse_tree(); 
  cout<<"input: ["<<s<<"]"<<endl<<endl;
  ptree.print_lookup();

    cout << endl;
  
    str = "create table Workers";
  strcpy(s, str.c_str());
  parser.set_string(s);
  ptree= parser.parse_tree(); 
  cout<<"input: ["<<s<<"]"<<endl<<endl;
  ptree.print_lookup();

    cout << endl;

    str = "insert into Workers values 69420, 20";
  strcpy(s, str.c_str());
  parser.set_string(s);
  ptree= parser.parse_tree(); 
  cout<<"input: ["<<s<<"]"<<endl<<endl;
  ptree.print_lookup();

    cout << endl;
  
    str = "insert into employee values Bi'den, Joe, 69";
  strcpy(s, str.c_str());
  parser.set_string(s);
  ptree= parser.parse_tree(); 
  cout<<"input: ["<<s<<"]"<<endl<<endl;
  ptree.print_lookup();

    cout << endl;

    str = "insert into student values Ar'cher, 69, C++, Admin";
  strcpy(s, str.c_str());
  parser.set_string(s);
  ptree= parser.parse_tree(); 
  cout<<"input: ["<<s<<"]"<<endl<<endl;
  ptree.print_lookup();

    cout << endl;
    str = "insert into student values Simpson,\"Robin Lee Algo,\", 19, CS, F- ";
  strcpy(s, str.c_str());
  parser.set_string(s);
  ptree= parser.parse_tree(); 
  cout<<"input: ["<<s<<"]"<<endl<<endl;
  ptree.print_lookup();
  cout << endl;
  str = "select age, lname, fname, department from employee where lname = \"Biden S\" and fname = \"Joe A\"";
  strcpy(s, str.c_str());
   parser.set_string(s);
  ptree= parser.parse_tree(); 
  cout<<"input: ["<<s<<"]"<<endl<<endl;
  ptree.print_lookup();
  cout << endl;
//   // parser.build_keywords();

  cout << endl;
  
  str = "select * from student where (age>=69 or lname = \"Mama Hugh Mungus\") and (fname = \"Joe Blow Biden\" or Major = \"Business Administration\")";
  strcpy(s, str.c_str());
  parser.set_string(s);
  ptree= parser.parse_tree(); 
  cout<<"input: ["<<s<<"]"<<endl<<endl;
  ptree.print_lookup();

    cout << endl;


    str = "select lname, fname, grade from student where ((age>=69 or lname = \"Mama Hugh Mungus\") and (fname = \"Joe Blow Biden\" or Major = \"Business Administration\") or grade = F) and salary < 5";
  strcpy(s, str.c_str());
  parser.set_string(s);
  ptree= parser.parse_tree(); 
  cout<<"input: ["<<s<<"]"<<endl<<endl;
  ptree.print_lookup();
  cout << endl << endl;
  


  cout << endl << endl;
  str = "select * from student where lname = \"Samuel L.\"";
  strcpy(s, str.c_str());
  parser.set_string(s);
  ptree= parser.parse_tree(); 
  cout<<"input: ["<<s<<"]"<<endl<<endl;
  ptree.print_lookup();


  cout << endl << endl;
  
  str = "select fname, lname from student where fname > \"Rose Ann Parks\"";
  strcpy(s, str.c_str());
  parser.set_string(s);
  ptree= parser.parse_tree(); 
  cout<<"input: ["<<s<<"]"<<endl<<endl;
  ptree.print_lookup();

    str = "select * from student where ((age>=25 or lname = Yao) and (fname = \"Teresa Mae\" or Major = CS)) and (fname = Mao or lname < Shi)";
  strcpy(s, str.c_str());
  parser.set_string(s);
  ptree= parser.parse_tree(); 
  cout<<"input: ["<<s<<"]"<<endl<<endl;
  ptree.print_lookup();

//   str = "select lname, fname from student where lname = \"Yao\" and fname = \"Flo\"";
//   strcpy(s, str.c_str());
//   parser.set_string(s);
//   ptree= parser.parse_tree(); 
//   cout<<"input: ["<<s<<"]"<<endl<<endl;

//   str = "select age, lname, fname from student where fname = \"Mary Ann\" and lname = Yao";
//   strcpy(s, str.c_str());
//   parser.set_string(s);
//   ptree= parser.parse_tree(); 
//   cout<<"input: ["<<s<<"]"<<endl<<endl;

//   str = "select * from student where (age>=25 or lname = Yao) and (fname = \"Teresa Mae\" or Major = CS)";
//   strcpy(s, str.c_str());
//   parser.set_string(s);
//   ptree= parser.parse_tree(); 
//   cout<<"input: ["<<s<<"]"<<endl<<endl;
  return true;
}

bool sql_basic(bool debug = false)
{
     SQL sql;
     Table t;
     cout << ">" << command_list[0] << endl;
     sql.command(command_list[0]);
     cout << "basic_test: table created." << endl<<endl;

     for (int i = 0; i < MAKE_TABLE_COMMANDS; i++)
     {
          cout << ">" << command_list[i] << endl;
          sql.command(command_list[i]);
     }

     cout << endl
          << endl;
    cout << endl;
     for (int i = MAKE_TABLE_COMMANDS; i < command_list.size(); i++)
     {
          cout << "\n>" << command_list[i] << endl;
          if (debug)
               cout<< sql.command(command_list[i])<<endl;
          else
               t = sql.command(command_list[i]);
          cout << "basic_test: records selected: "<<sql.select_recnos() << endl;
     }

     cout << "----- END TEST --------" << endl;
     return true;
}

bool sql_advanced(bool debug = false)
{
  SQL("test.txt");
  return true;
}



// TEST(TEST_TEMPLATE, TestTemplate) {
//   bool success = test_tokenize();
//   EXPECT_EQ(success, true);
// }

TEST(SQL_BASIC, SQLBasic) {
  bool success = sql_basic();
  EXPECT_EQ(success, true);
}

TEST(SQL_ADVANCED, SQLAdvanced) {
  bool success = sql_advanced();
  EXPECT_EQ(success, true);
}
// TEST(TESTPARSE, TestParse) {
//   bool success = parser_basic();
//   EXPECT_EQ(success, true);
// }




int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  std::cout<<"\n\n----------running basic_test.cpp---------\n\n"<<std::endl;
  return RUN_ALL_TESTS();
}

/*
build git:(master) ✗  😊 $> tree ../includes
../includes
├── binary_files
│   ├── file_record.cpp
│   ├── file_record.h
│   ├── utilities.cpp
│   └── utilities.h
├── bplustree
│   ├── bplustree.h
│   ├── btree_array_funcs.h
│   ├── map.h
│   ├── multimap.h
│   └── set_class.h
├── linked_list_functions
│   └── linkedlistfunctionsTemplated.h
├── node
│   ├── bogus.txt
│   ├── master.txt
│   └── node.h
├── parser
│   ├── parser.cpp
│   ├── parser.h
│   ├── sql_parser_functions.cpp
│   ├── sql_parser_functions.h
│   └── typedefs.h
├── queue
│   ├── MyQueue.h
│   └── queue_composition.h
├── stack
│   ├── MyStack.h
│   └── stack_composition.h
├── stl_utils
│   └── vector_utilities.h
├── table
│   ├── table.cpp
│   ├── table.h
│   └── typedefs.h
├── token
│   ├── child_tokens.h
│   ├── token.cpp
│   └── token.h
└── tokenizer
    ├── constants.h
    ├── ftokenize.cpp
    ├── ftokenize.h
    ├── state_machine_functions.cpp
    ├── state_machine_functions.h
    ├── stokenize.cpp
    └── stokenize.h
11 directories, 36 files
build git:(master) ✗  😊 $> ./bin/basic_test
----------running basic_test.cpp---------
[==========] Running 1 test from 1 test case.
[----------] Global test environment set-up.
[----------] 1 test from TEST_TEMPLATE
[ RUN      ] TEST_TEMPLATE.TestTemplate
------------------------
input: [make table student fields last, first, age, major, class]
       col :    last first age major class 
   command :    make 
table_name :    student 
input: [insert into student values Jones,"Sue Ellen", 20, CS, Fr ]
   command :    insert 
table_name :    student 
    values :    Jones Sue Ellen 20 CS Fr 
input: [select * from student]
   command :    select 
    fields :    * 
table_name :    student 
input: [select lname, fname from student where lname = "Yao" and fname = "Flo"]
   command :    select 
 condition :    lname = Yao and fname = Flo 
    fields :    lname fname 
table_name :    student 
     where :    yes 
input: [select age, lname, fname from student where fname = "Mary Ann" and lname = Yao]
   command :    select 
 condition :    fname = Mary Ann and lname = Yao 
    fields :    age lname fname 
table_name :    student 
     where :    yes 
input: [select * from student where (age>=25 or lname = Yao) and (fname = "Teresa Mae" or Major = CS)]
   command :    select 
 condition :    ( age >= 25 or lname = Yao ) and ( fname = Teresa Mae or Major = CS ) 
    fields :    * 
table_name :    student 
     where :    yes 
----- DONE -------------
[       OK ] TEST_TEMPLATE.TestTemplate (3 ms)
[----------] 1 test from TEST_TEMPLATE (3 ms total)
[----------] Global test environment tear-down
[==========] 1 test from 1 test case ran. (3 ms total)
[  PASSED  ] 1 test.
build git:(master) ✗  😊 $> 
*/

/*
input: [make table student fields last, first, age, major, class]
col: last first age major class
command: make
table_name: student

input: [insert into student values Jones,"Sue Ellen", 20, CS, Fr ]
command: insert
table_name: student
values: Jones Sue Ellen 20 CS Fr

input: [select * from student]
command: select
fields: *
table_name: student

input: [select lname, fname from student where lname = "Yao" and fname = "Flo"]
command: select
conditions: lname = Yao and fname = Flo
fields: lname fname
table_name: student
where: yes

input: [select age, lname, fname from student where fname = "Mary Ann" and lname = Yao]
command: select
conditions: fname = Mary Ann and lname = Yao
fields: age lname fname
table_name: student
where: yes

input: [select * from student where (age>=25 or lname = Yao) and (fname = "Teresa Mae" or Major = CS)]
command: select
conditions: ( age >= 25 or lname = Yao ) and ( fname = Teresa Mae or Major = CS )
fields: *
table_name: student
where: yes 
*/
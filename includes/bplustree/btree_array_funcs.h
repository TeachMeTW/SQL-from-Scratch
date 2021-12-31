#ifndef BTREE_ARRAY_FUNCTIONS_H
#define BTREE_ARRAY_FUNCTIONS_H

#include <iostream>
#include <iomanip>
#include <string>
using namespace std;

template <class T>
T maximal(const T& a, const T& b);                      //return the larger of the two items

template <class T>
void swap(T& a, T& b);  //swap the two items

template <class T>
int index_of_maximal(T data[ ], int n);                 //return index of the largest item in data

template <class T>
void ordered_insert(T data[ ], int& n, T entry);        //insert entry into the sorted array
                                                        //data with length n

template <class T>
int first_ge(const T data[ ], int n, const T& entry);   //return the first element in data that is
                                                        //not less than entry

template <class T>
void attach_item(T data[ ], int& n, const T& entry);    //append entry to the right of data

template <class T>
void insert_item(T data[ ], int i, int& n, T entry);    //insert entry at index i in data

template <class T>
void detach_item(T data[ ], int& n, T& entry);          //remove the last element in data and place
                                                        //it in entry

template <class T>
void delete_item(T data[ ], int i, int& n, T& entry);   //delete item at index i and place it in entry

template <class T>
void merge(T data1[ ], int& n1, T data2[ ], int& n2);   //append data2 to the right of data1

template <class T>
void split(T data1[ ], int& n1, T data2[ ], int& n2);   //move n/2 elements from the right of data1
                                                        //and move to data2

template <class T>
void copy_array(T dest[], const T src[],
                int& dest_size, int src_size);              //copy src[] into dest[]

template <class T>
void print_array(const T data[], int n, int pos = -1);  //print array data

template <class T>
bool is_gt(const T data[], int n, const T& item);       //item > all data[i]

template <class T>
bool is_le(const T data[], int n, const T& item);       //item <= all data[i]

template <class T>
bool is_ge(const T& item1, const T& item2) {
    return item1 >= item2;
}

template <>
inline bool is_ge<string>(const string& item1, const string& item2) {
    try {
        int num1 = stoi(item1);
        int num2 = stoi(item2);
        return num1 >= num2;
    }
    catch (exception e) {
        return item1 >= item2;
    }
}

template <class T>
bool is_gtp(const T& item1, const T& item2) {
    return item1 > item2;
}

template <>
inline bool is_gtp<string>(const string& item1, const string& item2) {
    try {
        int num1 = stoi(item1);
        int num2 = stoi(item2);
        return num1 > num2;
    }
    catch (exception e) {
        return item1 > item2;
    }
}

template <class T>
bool is_ls(const T& item1, const T& item2) {
    return item1 <= item2;
}

// These are just like the normal comparisons. Just in case. If a value is able to turn into a number it compares 
// I did this because my b+tree was causing issues when iterator key was a number. 

template <>
inline bool is_ls<string>(const string& item1, const string& item2) {
    try {
        int num1 = stoi(item1);
        int num2 = stoi(item2);
        return num1 <= num2;
    }
    catch (exception e) {
        return item1 <= item2;
    }
}

template <class T>
bool is_lsp(const T& item1, const T& item2) {
    return item1 < item2;
}

template <>
inline bool is_lsp<string>(const string& item1, const string& item2) {
    try {
        int num1 = stoi(item1);
        int num2 = stoi(item2);
        return num1 < num2;
    }
    catch (exception e) {
        return item1 < item2;
    }
}


//-------------- Vector Extra operators: ---------------------

template <typename T>
ostream& operator <<(ostream& outs, const vector<T>& list); //print vector list

template <typename T>
vector<T>& operator +=(vector<T>& list, const T& addme); //list.push_back addme





// -------------- Actual Functions ---------------------------------------



template <class T>
T maximal(const T& a, const T& b) {
    if (a > b) {
        return a;
    }
    if (a < b) {
        return b;
    }
}                   

template <class T>
void swap(T& a, T& b) {
    T temp = a;
    a = b;
    b = temp;
}

template <class T>
int index_of_maximal(T data[ ], int n) {
    int bigi = 0;
    T bigv = data[0];
    for (int i = 1; i < n; i++) {
        if (data[i] > bigv) {
            bigv = data[i];
            bigi = i;
        }
    }
    return bigi;
}            

template <class T>
void ordered_insert(T data[ ], int& n, T entry) {
    int i = first_ge(data, n , entry);
    insert_item(data, i, n, entry);
}      

template <class T>
int first_ge(const T data[ ], int n, const T& entry) {
    for (int i = 0; i < n; i++) {
        if (data[i] >= entry) {
            return i;
        }
    }
    return n;
}   

template <class T>
void attach_item(T data[ ], int& n, const T& entry) {
    // add check for cap
    
    data[n] = entry;
    n++;
}    

template <class T>
void insert_item(T data[ ], int i, int& n, T entry){
    // add checks for cap
    
    for (int j = n; j > i; j-- ) {
        data[j] = data[j-1];
    }
    data[i] = entry;
    n++;
}    

template <class T>
void detach_item(T data[ ], int& n, T& entry){
    if (n==0) {
        return;
    }
    n--;
    entry = data[n];
    
}         

template <class T>
void delete_item(T data[ ], int i, int& n, T& entry){
    if (n==0) {
        return;
    }
    
    entry = data[i];
    
    for (int j = i; j <n-1; j++ ) {
        data[j] = data[j+1];
    }
    n--;
}   

template <class T>
void merge(T data1[ ], int& n1, T data2[ ], int& n2){
    // add checks for cap
    /*
    for (int i = n1, j = 0; j < n2 , i < n1+n2; j++, i++) {
        data1[i] = data2[j];
    }
    n1 = n1+n2;
    n2 = 0;
    */
    for (int i = 0; i < n2; i++) {
        data1[n1+i] = data2[i];
    }
    n1 = n1+n2;
    n2 = 0;
}   

template <class T>
void split(T data1[ ], int& n1, T data2[ ], int& n2){
    // add check
    if (n1 < 2) {
        return;
    }
    n2 = n1-(n1/2);
    n1 = n1/2;
    for (int i = 0;i < n2;i++) {
        data2[i] = data1[n1+i];
    }
    
}  

template <class T>
void copy_array(T dest[], const T src[],
                int& dest_size, int src_size){
    dest_size = src_size;
    for (int i = 0; i < dest_size; i++) {
        dest[i] = src[i];
    }
}              

template <class T>
void print_array(const T data[], int n, int pos) {
    for(int i = 0; i < n; i++) {
        cout << data[i];
        pos = i;
    }
    cout << endl;
    if (pos >= 0) {
        for (int i = 0; i < pos; i++){
            cout << " ";
        }
        cout << "^";
    }
    else {
        cout << "Does not exist";
    }
    
}

template <class T>
bool is_gt(const T data[], int n, const T& item) {
    bool ge = false;
    bool le = false;
    for (int i = 0; i < n; i++) {
        if (item > data[i]) {
            ge = true;
        }
        if (item < data[i]) {
            le = true;
        }
    }
    if (ge == true && le == false) {
        return true;
    }
    return false;
}      

template <class T>
bool is_le(const T data[], int n, const T& item) {
    bool ge = false;
    bool le = false;
    for (int i = 0; i < n; i++) {
        if (item > data[i]) {
            ge = true;
        }
        if (item < data[i]) {
            le = true;
        }
    }
    if (ge == false && le == true) {
        return true;
    }
    return false;
}      

//-------------- Vector Extra operators: ---------------------

template <typename T>
ostream& operator <<(ostream& outs, const vector<T>& list) {
    for (int i = 0; i < list.size(); i++) {
        outs << list[i] << " ";
    }
    return outs;
} //print vector list

template <typename T>
vector<T>& operator +=(vector<T>& list, const T& addme) {
    list.push_back(addme);
    return list;
} //list.push_back addme






#endif // BTREE_ARRAY_FUNCTIONS_H
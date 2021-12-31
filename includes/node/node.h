#ifndef NODE_H
#define NODE_H

#include <iostream>
using namespace std;


template<typename T>
struct node{
    T _item;
    node<T>* _next = NULL;
    node<T>* _prev = NULL;
    node(const T& item = T(), node<T>* next = NULL, node<T>* prev = NULL):
    _item(item), 
    _next(next),
    _prev(prev){}
    friend ostream& operator << (ostream& outs, const node<T>& print_me){
        outs<<"["<<print_me._item<<"]->";
        return outs;
    }
};

#endif // NODE_H
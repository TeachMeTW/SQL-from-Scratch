#ifndef MY_STACK_H
#define MY_STACK_H

#include <iostream>
#include <iomanip>
#include "../../includes/linked_list_functions/linkedlistfunctionsTemplated.h"
#include <cassert>
using namespace std;

template <typename ITEM_TYPE>
class Stack{
public:
    class Iterator{
    public:
        friend class Stack;                 //give access to list to access _ptr
        Iterator(){_ptr = NULL;}            //default ctor
        Iterator(node<ITEM_TYPE>* p){
            _ptr = p;
        }      //Point Iterator to where
                                            //  p is pointing to
        ITEM_TYPE operator *(){
            assert(_ptr);
            return _ptr->_item;
        }    //dereference operator
        bool is_null(){return _ptr == NULL;}            //true if _ptr is NULL
        friend bool operator !=(const Iterator& left,
                                const Iterator& right)  //true if left != right
        {return left._ptr != right._ptr;}

        friend bool operator ==(const Iterator& left,
                                const Iterator& right)  //true if left == right
        {return left._ptr == right._ptr;}
        Iterator& operator++(){                        //member operator:++it
            _ptr ->_next = _ptr;                                     // or ++it = new_value
            return *this;
        }

        friend Iterator operator++(Iterator& it,
                                   int unused){        //friend operator: it++
            assert(it._ptr!=NULL);
            Iterator hold;
            hold = it;
            it._ptr = it._ptr ->_next;
            return hold;
        }

    private:
        node<ITEM_TYPE>* _ptr;    //pointer being encapsulated
    };

    Stack();
    Stack(const Stack<ITEM_TYPE>& copyMe);
    ~Stack();

    Stack<ITEM_TYPE>& operator=(const Stack<ITEM_TYPE>& RHS);

    ITEM_TYPE top();
    bool empty();
    void push(ITEM_TYPE item);
    ITEM_TYPE pop();

    friend ostream& operator<<(ostream& outs, const Stack& printMe) {
        _print_list(printMe._top);
        return outs;
    }
    Iterator begin() const;                   //Iterator to the head node
    Iterator end() const;                     //Iterator to NULL

    int size() const { return _size; }

private:
    node<ITEM_TYPE>* _top;
    int _size;
};


// Stack constructor
template <typename ITEM_TYPE>
Stack<ITEM_TYPE>::Stack() {
    // set top and size to null
    _top = NULL;
    _size = 0;
}

// Stack copy constructor
template<typename ITEM_TYPE>
Stack<ITEM_TYPE>::Stack(const Stack<ITEM_TYPE>& copyMe) {
    // initialize headptr and initialize copy function
    _top = _copy_list(copyMe._top);
    _size = copyMe._size;
    
}

// Stack destructor
// clears stack set size to 0
template <typename ITEM_TYPE>
Stack<ITEM_TYPE>::~Stack() {
    _clear_list(_top);
    _size = 0;
}

// Stack assignment operator
template <typename ITEM_TYPE>
Stack<ITEM_TYPE>& Stack<ITEM_TYPE>::operator=(const Stack<ITEM_TYPE>& RHS) {
    // check for self reference
    // clear existing object if needed
    // copy all from rhs into me
    if (this != &RHS) {
        _clear_list(_top);
        _top = _copy_list(RHS._top);
        _size = RHS._size;
    }
    return *this;
}

// Checks if stack is empty
// Checks if top is null
template <typename ITEM_TYPE>
bool Stack<ITEM_TYPE>::empty() {
    if (_top == NULL) { return true;}
    else { return false;}
}

// Pushes an item in the stack
// size ++ 
template <typename ITEM_TYPE>
void Stack<ITEM_TYPE>::push(ITEM_TYPE item) {
    _size++;
    _insert_head(_top, item);
}

// Removes an item from stack
// size --
template <typename ITEM_TYPE>
ITEM_TYPE Stack<ITEM_TYPE>::pop() {
    // assert(_size > 0);
    _size--;
    return _remove_head(_top);
}

// returns the value of the top value 
template <typename ITEM_TYPE>
ITEM_TYPE Stack<ITEM_TYPE>::top() {
    return _top ->_item;
}

// iterator set begin as top
template <typename ITEM_TYPE>
typename Stack<ITEM_TYPE>::Iterator Stack<ITEM_TYPE>::begin() const {
    return Iterator(_top);
}

// iterator set end as null
template <typename ITEM_TYPE>
typename Stack<ITEM_TYPE>::Iterator Stack<ITEM_TYPE>::end() const {
    return Iterator(nullptr);
}


#endif // MY_STACK_H
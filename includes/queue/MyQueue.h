#ifndef MY_QUEUE_H
#define MY_QUEUE_H

#include <iostream>
#include <iomanip>
#include "../../includes/linked_list_functions/linkedlistfunctionsTemplated.h"
#include <cassert>
using namespace std;

template <typename T>
class Queue
{
public:
    class Iterator{
    public:
        friend class Queue;                               //give access to list to access _ptr
        Iterator(){_ptr = NULL;}                          //default ctor
        Iterator(node<T>* p){
            _ptr = p;
        }                            //Point Iterator to where
                                                          //  p is pointing to
        T operator *(){ 
            assert(_ptr);
            return _ptr->_item;
        }    //dereference operator
        bool is_null(){
            return _ptr == NULL;
        }            //true if _ptr is NULL
        friend bool operator !=(const Iterator& left,
                                const Iterator& right)  //true if left != right
        {return left._ptr != right._ptr;}

        friend bool operator ==(const Iterator& left,
                                const Iterator& right)  //true if left == right
        {
            return left._ptr == right._ptr;
        }


        Iterator& operator++(){                         //member operator:++it
            _ptr ->_next = _ptr;                        // or ++it = new_value
            return *this;
        }

        friend Iterator operator++(Iterator& it,   //friend operator: it++
                                   int unused){ 
            assert(it._ptr!=NULL);
            Iterator hold;
            hold = it;
            it._ptr = it._ptr ->_next;
            return hold;        
        }

    private:
        node<T>* _ptr;                          //pointer being encapsulated
    };

    Queue();

    Queue(const Queue<T>& copyMe);
    ~Queue();
    Queue& operator=(const Queue<T>& RHS);

    bool empty();
    T front();
    T back();

    void push(T item);
    T pop();
    Iterator begin() const;                   //Iterator to the head node
    Iterator end() const;                     //Iterator to NULL
    void clear();
    void print_pointers();
    int size() const { return _size; }
    template<typename TT>
    friend ostream& operator<<(ostream& outs, const Queue<TT>& printMe) {
        _print_list(printMe._front);
        return outs;
    }
private:
    node<T>* _front;
    node<T>* _rear;
    int _size;
};

// checks if queue is empty
template <typename T>
bool Queue<T>::empty() {
    // checks if front is null
    if (_front == NULL) { return true;}
    else { return false;}
}

// Queue constructor
template <typename T>
Queue<T>::Queue() {
    // sets everything to null and 0
    _front = nullptr;
    _rear = nullptr;
    _size = 0;
}

// Queue copy constructor
template <typename T>
Queue<T>::Queue(const Queue<T>& RHS) {
    _front = _copy_list(RHS._front);
    _rear = _copy_list(RHS._rear);
    _size = RHS._size;
}

// Queue destructor
// Clears the whole queue by poping the front till size = 0
template<typename T>
Queue<T>::~Queue() {
    while (_size > 0) {
        pop();
        // _size--;
    }
    _size = 0;
    _front = NULL;
    _rear = NULL;
}

// Queue assignment operator
template <typename T>
Queue<T>& Queue<T>::operator=(const Queue<T>& RHS) {
    // cout << "RHS" << endl;
    if (this != &RHS) {
        // cout << "Clearing" << endl;
        clear();
        _front = _copy_list(RHS._front);
        _rear = _copy_list(RHS._rear);
        _size = RHS._size;
    }
    // cout << "Equal" << endl;
    return *this;
}

// Push a value into the queue and ++ size
template <typename T>
void Queue<T>::push(T item) {
    _size++;
    _enqueue(_front, item);
}

// remove a value from queue --size
template <typename T>
T Queue<T>::pop() {
    _size--;
    if (_size == 1) {
        _rear = _front;
    }
    
    return _remove_head(_front);
}

// get value at front
template <typename T>
T Queue<T>::front() {
    return _front ->_item;
}

// get value at rear
template <typename T>
T Queue<T>::back() {
    return _rear ->_item;
}

// Iterator set begin
template <typename T>
typename Queue<T>::Iterator Queue<T>::begin() const {
    return Iterator(_front);
}

// Iterator end set null
template <typename T>
typename Queue<T>::Iterator Queue<T>::end() const {
    return Iterator(nullptr);
}

// print queue
template <typename T>
void Queue<T>::print_pointers() {
    _print_list(_front);
}

// clear queue
template <typename T>
void Queue<T>::clear() {
    while (_size > 0) {
        pop();
        _size--;
    }
}

#endif // MY_QUEUE_H
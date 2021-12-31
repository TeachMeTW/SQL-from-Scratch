#ifndef LINKEDLISTFUNCTIONSTEMPLATED_H
#define LINKEDLISTFUNCTIONSTEMPLATED_H


#include "../../includes/node/node.h"
#include <iostream>
#include <assert.h>
using namespace std;

//Linked List General Functions Prototypes:

template<typename T>
node<T>* _insert_head(node<T>* &headptr, const T& item);

template<typename T>
node<T>* _insert_tail(node<T>* &headptr, const T& item);

template <typename T>
void _print_list(node<T>* headptr);

template <typename T>
void _print_list_backwards(node<T> *headptr);

template <typename T>
node<T>* _search_list(node<T>* head, T key);

template <typename T>
T _delete_node(node<T>*& headptr, node<T>* delete_this);

template <typename T>
T remove_head(node<T>*& headptr);

//insert after ptr: insert head if marker null
template <typename T> 
node<T>* _insert_after
(node<T>*& head, node<T> *after_this, T insert_this);

template <typename T>
node<T>* _insert_before
(node<T>*& head, node<T>* before_this,T insert_this);

template <typename T>
node<T>* _previous_node(node<T>* prev_to_this);

//_item at this position
template <typename T>
T& _at(node<T>* head, int pos);

//delete all the nodes
template <typename T>
void _clear_list(node<T>*& head);

//duplicate the list...
template <typename T>
node<T>* _copy_list(node<T> *&dest, node<T> *src);

template <typename T>
node<T>* _copy_list(node<T>* head);


// General Linked List Functions

template <typename T>
node<T>* _search_list(node<T>* head, T key){
    // set walker to point to first node
    node<T>* current = head;
    while (current != NULL) {
        // check if node's item matches key
        if (current -> _item == key) {
            return current;
        }
        // hop to next node
        current = current ->_next;
    }
    return NULL;
}

template <typename T>
T _delete_node(node<T>*& headptr, node<T>* delete_this) {
    // store deleted item
    T deleted = delete_this->_item;
    if (headptr == delete_this) {
        // if next node has nothing
        if (headptr ->_next == NULL) {
            headptr = NULL;
        }
        // move current head value to next node
        headptr ->_item = headptr->_next->_item;
        // set delete this' position to headptr next
        delete_this = headptr ->_next;
        // set headptr's next to the next next
        headptr ->_next = headptr ->_next ->_next;
        delete delete_this;
    }
    // store headptr address
    node<T>* temp = headptr;
    // while temp has not reached target node cycle through
    while (temp->_next != NULL && temp->_next != delete_this) {
        temp = temp->_next;
    }
    // set temp's next value to temp's next next
    temp->_next = temp->_next->_next;
    // delete the item
    delete delete_this;
    return deleted;
}

template <typename T>
node<T>* _insert_head(node<T>* &headptr, const T& item){
  //three steps:
  //1. build the new node.
  node<T>* temp = new node<T>(item);  //ctor of node()
  //2. point the next of the new node to headptr
  temp ->_next = headptr;
  temp ->_prev = NULL;
  if (headptr != NULL) {
      headptr ->_prev = temp;
  }
  headptr = temp;
  //3. point headptr to this new node
  return headptr ;
}

template <typename T>
void _print_list(node<T>* headptr) {
    for (node<T>* walker = headptr;  // set to walker to point to head
         walker != nullptr;  // as long as walker is not null
         walker = walker -> _next) {
             // print item at walker and hop to next node
             cout << *walker;
         }
        cout << "|||";
}

template <typename T>
void _print_list_backwards(node<T> *headptr) {
    // does not run if headptr == NULL
    if (headptr == NULL) {return;}
    // rerun's the program again with the next address
    _print_list_backwards(headptr -> _next);
    cout << "[" << headptr ->_item << "]->";
    // if prev to headptr == Null, function is finished
    // prints |||
    if (headptr ->_prev == NULL) {
        cout << "|||";
    }
}

//insert after ptr: insert head if marker null
template <typename T> 
node<T>* _insert_after 
(node<T>*& headptr, node<T> *after_this, T insert_this) {
    node<T>* insert = new node<T>(insert_this);  // create new node with item as cargo
    // if it is the first
    if (after_this == NULL) {
        // set insert's next to point to headptr
        insert -> _next = headptr;
        // set headptr to  equal current insert (new head)
        headptr = insert;
        // return new head
        return headptr;
    }
    else {
        // insert's next pointer is pointing to the mark's next pointer
        insert->_next = after_this ->_next;
        // mark's next pointer points to this new node
        after_this ->_next = insert;
        return insert;
    }
}

// insert before ptr; insert head if marker null
template <typename T>
node<T>* _insert_before
(node<T>*& head, node<T>* before_this,T insert_this) {
    // checks if want before head
    if (head == before_this) {
        // new temp node
        node<T>* temp = new node<T>(insert_this);
        // point next to current head
        temp -> _next = head;
        // update head
        head = temp;
        return head;
    }
    else {
        // new temp and current nodes
        node<T>* temp = head;
        node<T>* current = head;
        // for loop to reach before target | until temp == before this
        for (temp, current; temp != before_this;
        current = temp, temp = temp ->_next);
        // new insert node with item cargo
        node<T>* insert = new node<T>(insert_this);
        // update insert to match current's next address
        insert -> _next = current ->_next;
        // sets current's next to be insert
        current -> _next = insert;
        return current;
    }
}

template <typename T>
node<T>* _previous_node(node<T>* prev_to_this) {
    return prev_to_this ->_prev;
}

//_item at this position
template <typename T>
T& _at(node<T>* head, int pos) {
    // set walker to point to first node
    node<T>* current = head;
    for (int i = 0; i < pos; i++) {
        current = current ->_next;
        if (current -> _next == NULL) {
            assert(false);
        }
    }
    return current->_item;
}
//delete all the nodes
template <typename T>
void _clear_list(node<T>*& head) {
    node<T>* current = head;  // set current to head
    node<T>* next = NULL;  // set next to null

    while (current != NULL) {  // while not end
        // next node = current's next position
        next = current -> _next;
        // empty current node
        delete current;
        // current node is in place of next
        current = next;
    }
    // clear head
    head = NULL;
}


// //duplicate the list...
template <typename T>
node<T>* _copy_list(node<T>* head) {
    // if head is null return
    if (head == NULL) {
        return head;
    }
    // new node with head's payload
    node<T>* copy = new node<T>(head ->_item);
    // set new node's next to this function 
    // with head's next as the arguement
    copy ->_next = _copy_list(head ->_next);
    // return what was copied
    return copy;

}

template <typename T>
node<T>* _copy_list(node<T> *&dest, node<T> *src) {
    dest = nullptr;
    node<T>* walk = src;
    if (src != NULL) {
        dest = _insert_head(dest, walk->_item);
        node<T>* destwalk = dest;
        node<T>* inserted;
        walk = walk->_next;
        while (walk != NULL) {
            destwalk = _insert_after(dest, destwalk, walk->_item);
            walk = walk ->_next;
        }
        return destwalk;
    }
    return NULL;
}


// Iterative copy list

// template <typename T>
// node<T>* _copy_list(node<T>* head) {
//     node<T>* copy = NULL; 
//     node<T>*prev = NULL;
//     node<T> *n = copy;
//     while (head) {
//         *n = new node<T>(head->_item, NULL, prev);
//         prev = *n;
//         n = &(prev->_next);
//         head = head->_next;
//     } 
//     return copy;
// }




// Extra functions

// basically delete node but at the front
template <typename T>
T _remove_head(node<T>*& headptr) {
    // store deleted item
    T deleted = headptr ->_item;
    // point to the node remove
    node<T>* temp = headptr;
    // point headptr to next if exist
    if (headptr ->_next == NULL) {
        headptr = NULL;
    }
    else {
        headptr = headptr ->_next;
        // delete temp;
    }
    // delete the node
    return deleted;
}

template<typename T>
node<T>* _enqueue(node<T>* &headptr, const T& item) {
    node<T>* temp = new node<T>(item);  // ctor of node()
    if (headptr == NULL) {  // if headptr is null
        headptr = temp;  // set headptr to temp
        return headptr;
    }
    else {
        node<T>* hcpy = headptr;
        // create prev with headptr address
        while (hcpy ->_next != NULL) {
            // cycles through the list
            // until end is reached
            hcpy = hcpy ->_next;
        }
        hcpy ->_next = temp;  // set next pos = temp
        temp ->_prev = hcpy;  // set temp's previous pos to place holder
        return hcpy;
    }
} 

#endif  // LINKED_LIST_FUNCTIONS_H
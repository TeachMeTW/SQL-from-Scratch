#ifndef BPLUSTREE_H
#define BPLUSTREE_H

#include <iostream>
#include <iomanip>
#include <string.h>
#include <cassert>
#include <vector>
#include "../../includes/bplustree/btree_array_funcs.h"

using namespace std;

template <class T>
class BPlusTree
{
public:
    class Iterator{
    public:
        friend class BPlusTree;
        Iterator(BPlusTree<T>* _it=NULL, int _key_ptr = 0):
                                                it(_it), key_ptr(_key_ptr){}

        T operator *() {
            // assert(it != NULL);
            if (it != NULL) {
                return it -> data[key_ptr];
            }
            return T();
        }

        Iterator operator++(int un_used) {
            Iterator temp(it, key_ptr);
            key_ptr++;
            if (key_ptr >= it -> data_count) {
                key_ptr = 0;
                it = it -> next;
            }
            return temp;
        }
        Iterator operator++() {
            key_ptr ++;
            if (key_ptr >= it -> data_count) {
                key_ptr = 0;
                it = it -> next;
            }
            return *this;
        }
        friend bool operator ==(const Iterator& lhs, const Iterator& rhs) {
            if (lhs.it == NULL && rhs.it == NULL) {
                return true;
                }
            if (lhs.it != NULL && rhs.it != NULL) {
                Iterator l(lhs.it);
                Iterator r(rhs.it);
                return (*l == *r);
                }
            return false;
        } 

        friend bool operator !=(const Iterator& lhs, const Iterator& rhs) {
            return !(lhs == rhs);
        }
        void print_Iterator(){
            if (it){
                cout<<"iterator: ";
                print_array(it->data, it->data_count, key_ptr);
            }
            else{
                cout<<"iterator: NULL, key_ptr: "<<key_ptr<<endl;
            }
        }
        bool is_null(){return !it;}
        void info(){
            cout<<endl<<"Iterator info:"<<endl;
            cout<<"key_ptr: "<<key_ptr<<endl;
            cout<<"it: "<<*it<<endl;
        }

    private:
        BPlusTree<T>* it;
        int key_ptr;
    };

    BPlusTree(bool dups = false); // made
    BPlusTree(T *a, int size, bool dups = false); // made

    //big three:
    BPlusTree(const BPlusTree<T>& other); // made
    ~BPlusTree(); // made
    BPlusTree<T>& operator =(const BPlusTree<T>& RHS); // made

    void copy_tree(const BPlusTree<T>& other);  // made copy other into this object
    void copy_tree(const BPlusTree<T>& other, BPlusTree<T>*& original);

    void insert(const T& entry);   //insert entry into the tree
    void remove(const T& entry);   //remove entry from the tree
    void clear_tree();             //clear this object (delete all nodes etc.)

    bool contains(const T& entry) const; // made true if entry can be found
    T& get(const T& entry);              // made return a reference to entry
    const T& get(const T& entry)const;   // made return a reference to entry
    T& get_existing(const T& entry);     // made return a reference to entry
    Iterator find(const T& key);         // made return an iterator to this key.
                                         //     NULL if not there.
    Iterator lower_bound(const T& key);  // made return first that goes NOT BEFORE
                                         // key entry or next if does not
                                         // exist: >= entry
    Iterator upper_bound(const T& key);  // made return first that goes AFTER key
                                         //exist or not, the next entry  >entry

    int size() const;                    //count the number of elements

    bool empty() const;                  //true if the tree is empty

    void print_tree(int level = 0,
                    ostream &outs=cout) const;
    friend ostream& operator<<(ostream& outs,
                               const BPlusTree<T>& print_me){
        print_me.print_tree(0, outs);
        return outs;
    }

    bool is_valid(); // made
    string in_order(); // made
    string post_order();
    string pre_order();

    Iterator begin();
    Iterator end();
    ostream& list_keys(Iterator from = NULL, Iterator to = NULL){
        if (from == NULL) from = begin();
        if (to == NULL) to = end();
        for (Iterator it = from; it != to; it++)
            cout<<*it<<" ";
        return cout;

    }

private:
    static const int MINIMUM = 1;
    static const int MAXIMUM = 2 * MINIMUM;

    bool dups_ok;                        //true if duplicate keys are allowed
    int data_count;                      //number of data elements
    T data[MAXIMUM + 1];                 //holds the keys
    int child_count;                     //number of children
    BPlusTree* subset[MAXIMUM + 2];      //subtrees
    BPlusTree* next;
    bool is_leaf() const
    {return child_count==0;}             //true if this is a leaf node

    T* find_ptr(const T& entry);         //return a pointer to this key.
                                         // NULL if not there.

    //insert element functions
    void loose_insert(const T& entry);   //allows MAXIMUM+1 data elements in
                                         //   the root
    void fix_excess(int i);              //fix excess in child i

    //remove element functions:
    void loose_remove(const T& entry);  //allows MINIMUM-1 data elements
                                        //  in the root

    BPlusTree<T>* fix_shortage(int i);  //fix shortage in child i
    // and return the smallest key in this subtree
    BPlusTree<T>* get_smallest_node();
    void get_smallest(T& entry);      //entry := leftmost leaf
    void get_biggest(T& entry);       //entry := rightmost leaf
    void remove_biggest(T& entry);    //remove the biggest child of tree->entry
    void transfer_left(int i);        //transfer one element LEFT from child i
    void transfer_right(int i);       //transfer one element RIGHT from child i
    BPlusTree<T> *merge_with_next_subset(int i); //merge subset i with  i+1

};

// BIG THREE

template <typename T>
BPlusTree<T>::BPlusTree(bool dups):dups_ok(dups), data_count(0),
                                   child_count(0), next(NULL){
    //left emtpy
}

template <typename T>
BPlusTree<T>::BPlusTree(T *a, int size, bool dups) : data_count(0), child_count(0), dups_ok(dups){
    // same reason as above
    next = NULL;
    for (int i = 0; i < MAXIMUM+1; i++) {
        data[i] = T();
    }
    for (int i = 0; i < MAXIMUM+2; i++) {
        subset[i] = NULL;
    } 
    for (int i = 0; i < size; i++) {
        insert(*a);
        a++;
    }
    // B+ TREE AREA


}

template <typename T>
BPlusTree<T>::BPlusTree(const BPlusTree<T>& other):
                        data_count(0), child_count(0){
    dups_ok = other.dups_ok;
    next = NULL;
    copy_tree(other);
}

template <typename T>
BPlusTree<T>::~BPlusTree(){
    const bool debug = false;
    //cout<<"** destructor fired!"<<endl;
    clear_tree();
}

template <typename T>
BPlusTree<T>& BPlusTree<T>::operator =(const BPlusTree<T>& RHS){
    const bool debug = false;
    
    if (this != &RHS) {
        dups_ok = RHS.dups_ok;
        // cout<<"** assignment operator fired!"<<endl;
        clear_tree();
        copy_tree(RHS);
    }

    return *this;
}

// BEGIN AND END

template <typename T>
typename BPlusTree<T>::Iterator BPlusTree<T>::begin(){
    return Iterator(get_smallest_node());
}
template <typename T>
typename BPlusTree<T>::Iterator BPlusTree<T>::end(){
    return Iterator(NULL);
}

// PRINT
template <typename T>
string BPlusTree<T>::in_order() {
    string order = "";
    //cout << "Made";
    if (data_count == 0) {
        return order;
    }
    // Left Root Right
    //cout << "Left Root" << endl;
    for (int i = 0; i < data_count; i++) {
        // cout << "Before" << endl;
        if (subset[i] != NULL && i < child_count) {
            order += subset[i] -> in_order();
        }
        //cout << "After if" << endl;
        order += (to_string(data[i]) + "|");
    }
    // cout << "after for" << endl; 
    if (child_count > data_count && subset[child_count-1] != NULL) {
        order += subset[child_count-1] -> in_order();
    }
    return order;
    // B+ TREE AREA

}

template <typename T>
string BPlusTree<T>::post_order() {
    string order = "";
    //cout << "Made";
    if (data_count == 0) {
        return order;
    }
    // Left Root Right
    //cout << "Left Root" << endl;    
    if(subset[0] != NULL && !is_leaf()) {
        order += subset[0] -> post_order();
    }
    for (int i = 0; i < data_count; i++) {
        // cout << "Before" << endl;
        if (subset[i+1] != NULL && i < child_count) {
            order += subset[i+1] -> post_order();
        }
        order += (to_string(data[i]) + "|");    

        //cout << "After if" << endl;
        
    }    
    // if (child_count > data_count && subset[child_count-1] != NULL) {
    //     order += subset[child_count-1] -> post_order();
    // }
    // cout << "after for" << endl; 

    return order;
    // B+ TREE AREA

}

template <typename T>
string BPlusTree<T>::pre_order() {
    string order = "";
    //cout << "Made";
    if (data_count == 0) {
        return order;
    }
    // Left Root Right
    //cout << "Left Root" << endl;
    for (int i = 0; i < data_count; i++) {
        // cout << "Before" << endl;

        //cout << "After if" << endl;
        order += (to_string(data[i]) + "|");        
        if (subset[i] != NULL && i < child_count) {
            order += subset[i] -> pre_order();
        }
    }
    // cout << "after for" << endl; 
    if (child_count > data_count && subset[child_count-1] != NULL) {
        order += subset[child_count-1] -> pre_order();
    }
    return order;
    // B+ TREE AREA

}


template <typename T>
void BPlusTree<T>::print_tree(int level, ostream& outs) const{
    if (data_count == 0) {
        return;
    }
    if (child_count > data_count && subset[child_count-1] != NULL) {
        subset[child_count-1] -> print_tree(level+1);
    }
    for (int i = data_count-1; i >= 0; i--) {
        if (is_leaf()) {
            if (data_count < MAXIMUM+1) {
                // Currently Bugged
                // outs << setw(level*4) << "^" << endl;
            }
            // outs << setw(level*4) << "^";
            outs << setw(level*4) << data[i] << endl;
        }
        else if (!is_leaf()) {
            outs << setw(level*4) << data[i] << endl;
        }
        
        if (i<child_count && subset[i] != NULL) {
            subset[i] -> print_tree(level+1, outs);
        }
    }

    // B+ TREE Section

}


// CONTAINS GET FIND UPPER LOWER BOUND
template <typename T>
typename BPlusTree<T>::Iterator BPlusTree<T>::upper_bound(const T& key) {
    for (Iterator it = begin(); it != end(); it++) {
        if (is_gtp(*it,key)) {
            return it;
        }
    }
    return Iterator();
}

template <typename T>
typename BPlusTree<T>::Iterator BPlusTree<T>::lower_bound(const T& key) {
    Iterator it = begin();
    // Iterator prev;
    // cout << *this << endl;
    for (it; it != end(); it++) {

        // cout << *it << ":" << key << endl;
        if (is_ge(*it, key)) {
            //cout << "Returning" << endl;
            return it;
        }
        // if (it++ == end()) {
        //     prev = it;
        // }
    }
    // if (!this->contains(key)) {
    //     cout << "Key doesnt exist" << endl;
    //     cout << *prev << endl;
    //     return prev;
    // }
    return Iterator();
}


template <typename T>
typename BPlusTree<T>::Iterator BPlusTree<T>::find(const T& key) {
    int i = first_ge(data, data_count, key);
    bool found = (i<data_count && data[i]==key);    
    if (found) {
        if (is_leaf()) {
            return Iterator(this, i);
        }
        else {
            return subset[i+1] -> find(key);
        }
    }
    else  {
        if (!is_leaf()) {
            return subset[i] -> find(key);
        }
        else {
            return Iterator();
        }   
    }
}

template <typename T>
T* BPlusTree<T>::find_ptr(const T& entry) {
    int i = first_ge(data, data_count, entry);
    bool found = (i<data_count && data[i]==entry);    
    if (found) {
        if (is_leaf()) {
            return data[i];
        }
        else {
            return subset[i+1] -> find_ptr(entry);
        }
    }
    else  {
        if (!is_leaf()) {
            return subset[i] -> find_ptr(entry);
        }
        else {
            return NULL;
        }   
    }
    // B+ TREE AREA
}

template <typename T>
bool BPlusTree<T>::contains(const T& entry) const {
    int i = first_ge(data, data_count, entry);
    bool found = (i<data_count && data[i]==entry);    
    if (found) {
        return true;
        // if (is_leaf()) {
        //     return true;
        // }
        // else {
        //     return subset[i+1] -> contains(entry);
        // }
    }
    else  {
        if (!is_leaf()) {
            return subset[i] -> contains(entry);
        }
        else {
            return false;
        }   
    }
}

template <typename T>
const T& BPlusTree<T>::get(const T& entry) const{
    assert(contains(entry));
    return get_existing(entry);
}

template <typename T>
T& BPlusTree<T>::get(const T& entry) {
    //If entry is not in the tree, add it to the tree
    //assert(contains(entry));

    const bool debug = false;
    if (!contains(entry)) {
        insert(entry);
    }

    return get_existing(entry);
}

template <typename T>
T& BPlusTree<T>::get_existing(const T& entry) {
    /*
 * ---------------------------------------------------------------
 * This routing explains plainly how the BPlusTree is organized.
 * i = first_ge
 *       |      found         |    !found         |
 * ------|--------------------|-------------------|
 * leaf  |  you're done       | it's not there    |
 *       |                    |                   |
 * ------|--------------------|-------------------|
 * !leaf | subset[i+1]->get() |  subset[i]->get() |
 *       | (inner node)       |                   |
 * ------|--------------------|-------------------|
 *       |                    |                   |
 *
 * ---------------------------------------------------------------
 *
 */
    //assert that entry is not in the tree.
    //assert(contains(entry));

    const bool debug = false;
    int i = first_ge(data, data_count, entry);
    bool found = (i<data_count && data[i]==entry);
    if (is_leaf()) {
        if (found){
            return data[i];
        }
        else{
            if (debug) cout<<"get_existing was called with nonexistent entry"<<endl;
            assert(found);
        }
    }
    if (found) //inner node
        return subset[i+1]->get(entry);
        //or just return true?
    else //not found yet...
        return subset[i]->get(entry);


}

// VALIDATION

template <typename T>
bool BPlusTree<T>::is_valid(){
    
    //should check that every data[i] < data[i+1]
    if (is_leaf())
        return true;
    

    //data[data_count-1] must be less than equal to 
    //  every subset[child_count-1]->data[ ]
    
    //every data[i] is greater than every subset[i]->data[ ]
    
    //B+Tree: Every data[i] is equal to subset[i+1]->smallest
    
    //Recursively validate every subset[i]

    return true;
}

// REMOVE

template <typename T>
void BPlusTree<T>::remove(const T& entry){
    // /*
    //  * ---------------------------------------------------------------------------------
    //  * Same as BTree:
    //  * Loose_remove the entry from this tree.
    //  * Shrink if you have to
    //  * ---------------------------------------------------------------------------------
    //  * once you return from loose_remove, the root (this object) may have no data and
    //  * only a single subset:
    //  * now, the tree must shrink:
    //  *
    //  * point a temporary pointer (shrink_ptr) and point it to this root's only subset
    //  * copy all the data and subsets of this subset into the root (through shrink_ptr)
    //  * now, the root contains all the data and poiners of it's old child.
    //  * now, simply delete shrink_ptr, and the tree has shrunk by one level.
    //  * Note, the root node of the tree will always be the same, it's the
    //  * child node we delete
    //  *
    //  *
    //  */

    // // BTREE
    // loose_remove(entry);
    // if (child_count == 1 && data_count == 0) {
    //     // shrink ptr
    //     BPlusTree* shrink = this -> subset[0];
    //     // copy the data and subset
    //     copy_array(data, shrink -> data, data_count, shrink -> data_count);
    //     copy_array(subset, shrink -> subset, child_count, shrink->child_count);
    //     // delete
    //     for (int i = 0; i < shrink -> child_count; i++) {
    //         shrink -> subset[i] = NULL;
    //         //delete shrink -> subset[i];
    //     }
    //     // delete
    //     shrink -> child_count = 0;
    //     shrink -> data_count = 0;
    //     shrink = NULL;
    //     delete shrink;
    // }

    // B+ TREE


}
template <typename T>
void BPlusTree<T>::loose_remove(const T& entry){
     /* four cases:
           leaves:
                a. not found: there is nothing to do
                b. found    : just remove the target
           non leaf:
                c. not found: subset[i]->loose_remove, fix_shortage(i)
                d. found    : subset[i+1]->loose_remove, fix_shortage(i+1) [...]
                    (No More remove_biggest)

             |   !found               |   found                 |
       ------|------------------------|-------------------------|-------
       leaf  |  a: nothing            | b: delete               |
             |     to do              |    target               |
       ------|------------------------|-------------------------|-------
       !leaf | c:                     | d: B_PLUS_TREE          |
             |  [i]->  loose_remove   |   [i+1]-> loose_remove  |
             |  fix_shortage(i)       | fix_shortage(i+1) [...] |
       ------|------------------------|-------------------------|-------


     */

    // BTREE
    int i = first_ge(data, data_count, entry);
  
    bool found = (i<data_count && data[i] == entry);
    // if (is_leaf() && !found) {
    //     return;
    // }
    // else if (is_leaf() && found) {
    //     T hold;
    //     delete_item(data, i, data_count, hold);
    // }
    // else if (!is_leaf() && !found) {
    //     subset[i] -> loose_remove(entry);
    //     fix_shortage(i);
    // }
    // else if (!is_leaf() && found) {
    //     T holder;
    //     subset[i] -> remove_biggest(holder);
    //     //ordered_insert(data, data_count, holder);
    //     data[i] = holder;
    //     fix_shortage(i);
    //}
    // B+ TREE
    const bool debug = false;
    T found_entry;
    if (is_leaf()){
        if(!found){
            //[a.] nothing to do
            //entry not in this tree
            return;
        }
        else{
            //[b.] just delete the item
            //   my parent will take care of my shortage
            T hold;
            delete_item(data, i, data_count, hold);            
        }

    }
    else{
        //not leaf:
        // ---- 000 B_PLUS_TREE: no more remove_biggest
        if (!found){
            subset[i] -> loose_remove(entry);
            fix_shortage(i);
            //[c.] Not here: subset[i]->loose_remove
            //not found yet. search in the next level:
        }
        else{
            //[d.] found key in an inner node:subset[i+1]->loose_remove
            assert(i < child_count-1);

            /*
             * Here's The Story:
             * data[i] is the same as the item that we have deleted.
             * so, now, it needs to be replaced by the current smallest key
             *      in subset[i+1]
             * Problem: After fix_shortage, data[i] could have moved to a different index(?)
             *      or it could have sunk down to a lower level as a result of merge
             *      we need to find this item and replace it.
             *
             *      Before fix_shortage: we cannot replace data[i] with subset[i+1]->smallest
             *      before fix_excess because this smallest key could be the very entry
             *      we are removing. So, we'd be replacing data[i] with entry. and this will
             *      leave the deleted key in the inner nodes.
             *
             *      After fix_shortage: We have no way of knowing where data[i] key (which is
             *      same as entry) will end up. It could move down one level by the time
             *      fix_shortage returns.
             *
             *      Therefore it seems like we have no choice but to search for it in
             *      data[ ] AND subset[i]->data[ ]
             * Current Solution: Kind of messy:
             *      After fix_shortage(i+1):
             *      Search for entry in data[ ] and if found, replace it with smallest.
             *      otherwise, search for it in subset[i]->data[ ]
             *          and replace it. that's where the key (entry) will end up after
             *          fix order returns (credit: Jiaruy Li)
             *
             */
            //subset[i+1]->loose_remove(entry);
            //fix_shortage(i+1);

            //remember. it's possible that data[i] will be gone by the time
            //      fix_shortage returns.
            //key was removed from subset[i+1]:
            //  1. shortage: find entry in data or subset[i+1]->data 
            //              and replace it with subset[i+1]->smallest
            //  2. no shortage: replace data[i] with subset[i+1]->smallest
        }
        // --------------------------------------------------------
    }
}

template <typename T>
BPlusTree<T>* BPlusTree<T>::fix_shortage(int i){
    /*
     * fix shortage in subtree i:
     * if child i+1 has more than MINIMUM,
     *          (borrow from right) transfer / rotate left(i+1)
     * elif child i-1 has more than MINIMUM,
     *          (borrow from left) transfer /rotate right(i-1)
     * elif there is a left sibling,
     *          merge with prev child: merge(i-1)
     * else
     *          merge with next (right) child: merge(i)
     *
     *
     * returns a pointer to the fixed_subset
     */

    // BTREE
    if (subset[i] != NULL && subset[i] -> data_count < MINIMUM) {
        if (subset[i+1] != NULL  && i < child_count -1) {
            if (subset[i+1] -> data_count > MINIMUM) {
                //cout << "Rotate left" << endl;
                transfer_left(i);
                return subset[i];
            }
        }
        if (subset[i-1] != NULL && i > 0)  {
            if (subset[i-1] -> data_count > MINIMUM) {
                //cout << "Rotate right" << endl;
                transfer_right(i);
                return subset[i];
            }
        }
        if (subset[i-1] != NULL && i > 0 ) {
            if (subset[i-1] -> data_count <= MINIMUM) {
                merge_with_next_subset(i-1);
                return subset[i];
            }
        }
        if (subset[i+1] != NULL && i < child_count - 1 ) {
            if (subset[i+1] -> data_count <= MINIMUM) {
                merge_with_next_subset(i);
                return subset[i];
            }
        }
    }
    // B+ TREE
    
    const bool debug = false;
    if ((i<child_count-1) && (subset[i+1]->data_count > MINIMUM)){
        //borrow from right subtree if you can
        transfer_left(i+1);
        return subset[i];
    }
    //...
}

template <typename T>
void BPlusTree<T>::get_biggest(T& entry){
    //replace entry with the left_most leaf of this tree
    // Keep looking in the first subtree (recursive)
    //  until you get to a leaf.
    // Then, replace entry with data[0] of the leaf node
    const bool debug = false;
    if (is_leaf() && data_count != 0) {
        return;
    }
}


template <typename T>
void BPlusTree<T>::get_smallest(T& entry){
    //replace entry with the left_most leaf of this tree
    // Keep looking in the first subtree (recursive)
    //  until you get to a leaf.
    // Then, replace entry with data[0] of the leaf node
    const bool debug = false;
    if (!is_leaf()) {
        subset -> get_smallest(entry);
    }
    else {
        entry = data[0];
    }
}

template <typename T>
BPlusTree<T>* BPlusTree<T>::get_smallest_node(){
    if (is_leaf()) {
        return this;
    }
    return subset[0] -> get_smallest_node();
}


template <typename T>
BPlusTree<T>* BPlusTree<T>::merge_with_next_subset(int i){
    /*
     * ----------------------------------------------------------------------
     *  Merge subset[i] with subset [i+1] REMOVE data[i];
     *  non leaf: same as BTree
     *  leaves  : delete but do not bring down data[i]
     * ----------------------------------------------------------------------
     *
     *   1. remove data[i] from this object
     *   2. if not a leaf, append it to child[i]->data:
     *   3. Move all data items from subset[i+1]->data to right of subset[i]->data
     *   4. Move all subset pointers from subset[i+1]->subset to
     *          right of subset[i]->subset
     *   5. delete subset[i+1] (store in a temp ptr)
     *   6. if a leaf, point subset[i]->next to temp_ptr->next
     *   6. delete temp ptr
     *
     *
     * non-leaf nodes: (same as BTree)
     * ------------------
     *  i = 0:
     *             [50  100]
     *          /      |     \
     *  [  ]         [75]       ....
     *    |         /    \
     *   [a]      [b]    [c]
     *
     *  bring down data[i], merge it with subset[i] and subset[i+1]:
     *      then, get rid of subset[i+1]
     *             [100]
     *               |
     *            [50|75]       ....
     *            /  |  \
     *         [a]  [b]  [c]
     *
     *
     *
     * leaf node:
     * ----------------
     * Exactly the same, but do not attach the deleted data[i] to subset[i]->data[ ]
     *
     *  i = 0 : merge 5 and [()]
     *        [7 | 10]
     *      /    |     \
     *  [5]->   [()]->  [10]
     *
     *  Delete data[i] (7), merge subset[i] and subset[i+1]
     *      WITHOUT bringing down data[i]
     *
     *        [10]
     *      /      \
     *  [5]->      [10]
     *
     *
     * i = 1 merge 7 and [()]
     *        [7 | 10]
     *      /    |     \
     *  [5]->   [7]->  [()]
     *
     *  Delete data[i] (10), merge subset[i] and subset[i+1]
     *      WITHOUT bringing down data[i]
     *
     *        [7]
     *      /     \
     *  [5]->      [7]
     *
     *
     *
     *
     */
    const bool debug = false;
    //...

    // B TREE AREA
    T hold;
    delete_item(data, i, data_count, hold);
    attach_item(subset[i] -> data, subset[i] -> data_count, hold);
    merge(subset[i] -> data, subset[i] -> data_count, subset[i+1] -> data, subset[i+1] -> data_count);
    merge(subset[i] -> subset, subset[i] -> child_count, subset[i+1] -> subset, subset[i+1] -> child_count);
    BPlusTree* holder2;
    delete_item(subset, i+1, child_count, holder2);
    holder2 = NULL;
    delete holder2;

    // B+ TREE


    return subset[i];

}
template <typename T>
void BPlusTree<T>::transfer_left(int i){
    /*
     * --------------------------------------------------------------
     * transfer_left: from subset[i] to subset[i-1]
     *      non-leaf: same as BTree
     *      leaves  : rotate and leave and update data[i-1] with subset[i]->data[0]
     *                  leftmost key after you borrowed goes to replace data[i-1]
     * --------------------------------------------------------------
     *
     * non-leaf keys: BTree's rotate_left
     * ---------------
     *
     * (0 < i < child_count) and (subset[i]->data_count > MINIMUM)
     * subset[i-1] has only MINIMUM - 1 entries.
     *
     * item transfers from child[i] to child [i-1]
     *
     * FIRST item of subset[i]->data moves up to data to replace data[i-1],
     * data[i-1] moves down to the RIGHT of subset[i-1]->data
     *
     *  i = 1:
     *              [50  100]
     *  [  ]         [65 75]       ....
     *   [a]      [b]  [c]  [d]
     *
     *  65 move up to replace 50 (data[i])
     *  65's child (its child 0) moves over to be the future child of 50
     *  50 moves down to the right of subset[i]->data
     *
     *               [65 100]
     *   [50]         [ 75 ]       ....
     * [a]  [b]      [c]  [d]
     *
     * ****This does not belong here:
     * last item of subset[i-1]->data moves up to data to replace data[i],
     * data[i] moves down to the RIGHT of subset[i]->data
     *
     * leaf keys:
     * -------------
     *  fix_shortage(0) calls trnasfer_left(1) so, i = 1
     *          [7 | 10]
     *        /    |     \
     * [( )]<=[(7) | 8]  [10]
     *
     * 1. transfer subset[i(1)]->data[0] to end of subset[i-1]
     *        [7 | 10]
     *      /    |     \
     *   [7]    [8]   [10]
     *
     * 2. update parent of the transfered subset: data[i-1 (0)] = subset[i(1)]->data[0]
     *
     *        [(8) | 10]
     *      /      |     \
     *   [7]     [(8)]   [10]
     *
     *
     *
     */

    // BTREE 
    attach_item(subset[i] -> data, subset[i] -> data_count, data[i]);
    T in;
    delete_item(subset[i+1] -> data, 0, subset[i+1] -> data_count, in);
    data[i] = in;
    if (subset[i+1] -> child_count > 0) {
        BPlusTree* r = NULL;
        delete_item(subset[i+1] -> subset, 0, subset[i+1] -> child_count, r);
        attach_item(subset[i] -> subset, subset[i] -> child_count, r);
    }
}

template <typename T>
void BPlusTree<T>::transfer_right(int i){
    /*
     * --------------------------------------------------------------
     * transfer right from subset[i] to subset[i+1]
     *      non-leaf: same as BTree
     *      leaves  : rotate and leave a 'trace':
     *              data[i] is replaced with subset[i+1]->data[0]
     *              after you borrowed the key,
     * --------------------------------------------------------------
     *
     * non-leaf keys: BTree's rotate_right
     * ---------------
     * (i < child_count - 1) and (subset[i-1]->data_count > MINIMUM)
     * subset[i+ 1] has only MINIMUM - 1 entries.
     *
     * item transfers from child[i] to child [i+1]
     *
     * LAST item of subset[i]->data moves up to data to replace data[i],
     * data[i] moves down to the LEFT of subset[i+1]->data
     *
     * i = 1
     *                     [50 100]
     *      [20 30]        [65 75]          [ ]
     *  [..] [..] [..]   [a] [b] [c]        [..]
     *
     *  75 moves up to replace 100 (data[i])
     *  75's child (its last child) moves over to be the (child 0) child of 100
     *  100 moves down to subset[i]->data
     *
     *                     [50 75]
     *      [20 30]          [65]          [100]
     *  [..] [..] [..]     [a] [b]        [c] [..]
     *
     * last item of subset[i-1]->data moves up to data,
     * data[i] moves down to the RIGHT of subset[i]->data
     *
     * leaf keys:
     * -------------
     *  fix_shortage(2) calls trnasfer_right(1) so, i = 1 subset[i] is ([7|8])
     *  subset[i+1] is the one being fixed.
     *        [7 | 10]
     *      /    |     \
     * [5|6]->[7 | (8)]=>[()]
     *
     * 1. transfer subset[i(1)]->data[last item] to left of subset[i+1]->data
     *        [7 | (10)]
     *      /    |     \
     *   [5|6]->[7]-> [8]
     *
     * 2. update parent of the transfered subset: data[i (1)] = subset[i+1(2)]->data[0]
     *
     *        [7 | 8]
     *      /    |    \
     *   [5|6]->[7]-> [8]
     *
     *
     */

    // BTREE
    T hold;
    insert_item(subset[i] -> data, 0, subset[i] -> data_count, data[i-1]);
    detach_item(subset[i-1] -> data, subset[i-1] -> data_count, hold);
    data[i-1] = hold;
    if (subset[i-1] -> child_count > 0) {
        BPlusTree* l = NULL;
        detach_item(subset[i-1] -> subset, subset[i-1] -> child_count, l);
        insert_item(subset[i] -> subset, 0, subset[i] -> child_count, l);
    }
    const bool debug = false;

}

// INSERT

template <typename T>
void BPlusTree<T>::insert(const T& entry){
    //in order for this class to be able to keep track of the number of the keys,
    //      this function (and the functions it calls ) must return a success code.
    //If we are to keep track of the number the keys (as opposed to key/values)
    //  then the success code must distinguish between inserting a new key, or
    //  adding a new key to the existing key. (for "dupes_ok")
    //
    //loose_insert this entry into this root.
    //loose_insert(entry) will insert entry into this tree. Once it returns,
    //  all the subtrees are valid
    //  btree subtrees EXCEPT this root may have one extra data item:
    //    in this case (we have excess in the root)
    //      create a new node, copy all the contents of this root into it,
    //      clear this root node,
    //      make the new node this root's only child (subset[0])
    //
    //Then, call fix_excess on this only subset (subset[0])

    // BTREE
    loose_insert(entry);
    //fix_excess(0);
    if (data_count > MAXIMUM) {
        BPlusTree* cpy = new BPlusTree;
        copy_array(cpy -> data, data, cpy -> data_count, data_count);
        copy_array(cpy -> subset, subset, cpy -> child_count, child_count);
        data_count = 0;
        child_count = 1;
        subset[0] = cpy;
        fix_excess(0);
    }
}

template <typename T>
void BPlusTree<T>::loose_insert(const T& entry){
    /*
       int i = first_ge(data, data_count, entry);
       bool found = (i<data_count && data[i] == entry);

       three cases:
         found
         a. found/leaf: deal with duplicates: call +
         b. found/no leaf: subset[i+1]->loose_insert(entry)
                           fix_excess(i+1) if there is a need

         ! found:
         c. !found / leaf : insert entry in data at position i
         c. !found / !leaf: subset[i]->loose_insert(entry)
                            fix_excess(i) if there is a need

            |   found          |   !found         |
      ------|------------------|------------------|-------
      leaf  |a. dups? +        | c: insert entry  |
            |                  |    at data[i]    |
      ------|------------------|------------------|-------
            | b.               | d.               |
            |subset[i+1]->     | subset[i]->      |
      !leaf | loose_insert(i+1)|  loose_insert(i) |
            |fix_excess(i+1)   | fix_excess(i)    |
            |                  |                  |
      ------|------------------|------------------|-------
    */

    // BTREE
    int i = first_ge(data, data_count, entry);
    //cout << "First GE: " << i << endl;
    bool found = (i<data_count && data[i] == entry);
    //cout << found << endl;
    if (found) {
        if (!is_leaf()) {
            subset[i+1] -> loose_insert(entry);
            fix_excess(i+1);
            return;
        }
        else {
            // cout << endl << data[i] << endl;
            // cout << "Overwrite" << endl;
            insert_item(data, i, data_count , entry);
            //data[i] == entry;
            return;
        }
    }
    else {
        if (is_leaf()) {
            //cout << "Leaf" << endl;
            insert_item(data, i, data_count , entry);
            return;
            //cout << "Inserted" << endl;
        }        
        else {
            //cout << "Not leaf" << endl;
            subset[i] -> loose_insert(entry);
            fix_excess(i);
            return;
        }
    }
    // if (subset[i]->data_count > MAXIMUM) {
    //     fix_excess(i);
    // }
    //cout << "End of Loose Insert" << endl;
    const bool debug = false;
}

template <typename T>
void BPlusTree<T>::fix_excess(int i){
    //this node's child i has one too many items: 3 steps:
    //1. add a new subset at location i+1 of this node
    //2. split subset[i] (both the subset array and the data array) and move half into
    //      subset[i+1] (this is the subset we created in step 1.)
    //3. detach the last data item of subset[i] and bring it and insert
    //      it into this node's data[]
    //
    //Note that this last step may cause this node to have too many itesm.
    //  This is OK. This will be
    //  dealt with at the higher recursive level. (my parent will fix it!)
    //
    //NOTE: ODD/EVENNESS
    // when there is an excess, data_count will always be odd
    //  and child_count will always be even.
    //  so, splitting is always the same.

    //  000 B_PLUS_TREE
    //  if (subset[i]->is_leaf())
    //  transfer the middle entry to the right and...
    //  Deal with next pointers. just like linked list insert
    // Insert item then link 
    // BTREE
    if (subset[i] == NULL) {
        return;
    }
    if (subset[i] -> data_count > MAXIMUM) {    
        BPlusTree* temp = new BPlusTree;
        T rmid;
        //cout << "Splitting" << endl;
        split(subset[i] -> data, subset[i] -> data_count, temp -> data, temp -> data_count);
        split(subset[i] -> subset, subset[i] -> child_count, temp -> subset, temp -> child_count);
        //cout << "Detatching" << endl;
        delete_item(temp -> data, 0, temp -> data_count, rmid);
        //cout << "Inserting" << endl;
        insert_item(this -> data, i,  this -> data_count, rmid);
        insert_item(subset, i+1, child_count, temp);
        if (subset[i] -> is_leaf()) {
            //cout << "Subset is leaf" << endl;
            insert_item(temp -> data, 0, temp -> data_count, rmid);
            temp -> next = subset[i] -> next;
            subset[i] -> next = temp;
        }    
    }
    

}

// SIZE EMPTY

template <typename T>
int BPlusTree<T>::size() const{
    int s = data_count;
    //this function counts the number of keys in the btree
    if (empty()) {
        return 0;
    } 
    for (int i = 0; i < child_count; i++) {
        s += subset[i] -> size();
    }
    return s;
}

template <typename T>
bool BPlusTree<T>::empty() const{
    return (data_count ==0);
}

// CLEAR COPY

template <typename T>
void BPlusTree<T>::clear_tree(){
    if (child_count == 0) {
        data_count = 0;
        return;
    }
    data_count = 0;
    for (int i = child_count -1; i >= 0; i--) {
        subset[i] -> clear_tree();
        child_count = 0;
    }
    for (int i = 0; i < child_count; i++) {
        subset[i] = NULL;
        delete subset[i];
    }
}

template <typename T>
void BPlusTree<T>::copy_tree(const BPlusTree<T>& other){
    BPlusTree<T>* temp = NULL;
    copy_tree(other, temp);
}

template <typename T>
void BPlusTree<T>::copy_tree(const BPlusTree<T>& other, BPlusTree<T>*& original){
        //assumes this btree is empty. [memory leaks otherwise]
    assert(empty());
    dups_ok = other.dups_ok;
    data_count = other.data_count;
    child_count = other.child_count;
    next = NULL;
    for (int i = 0; i < data_count; i++) {
        data[i] = other.data[i];
    }
    if (!other.is_leaf()) {
        for (int i = 0; i < child_count; i++) {
            BPlusTree<T>* temp = new BPlusTree<T>();
            subset[i] = temp;
            subset[i] -> copy_tree(*other.subset[i], original);
        }
    }
    else {
        if (original != NULL) {
            original -> next = this;
        }
        original = this;
    }
    
}

#endif // BPLUSTREE_H
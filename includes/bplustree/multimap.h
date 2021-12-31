#ifndef MULTIMAP_H
#define MULTIMAP_H
#include "../../includes/bplustree/bplustree.h"
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
using namespace std;

template <typename K, typename V>
struct MPair{
    K key;
    vector<V> value_list;

    //--------------------------------------------------------------------------------
    /*      MPair CTORs:
     *  must have these CTORs:
     *  - default CTOR / CTOR with a key and no value: this will create an empty vector
     *  - CTOR with a key AND a value: pushes the value into the value_list
     *  _ CTOR with a key and a vector of values (to replace value_list)
     */
    //--------------------------------------------------------------------------------
    MPair(const K& k=K()) {
        key = k;
        //value_list = {};
    }
    MPair(const K& k, const V& v) {
        key = k;
        value_list.push_back(v);

    }
    MPair(const K& k, const vector<V>& vlist) {
        key = k;
        value_list = vlist;
    }
    //--------------------------------------------------------------------------------

    //You'll need to overload << for your vector:
    friend ostream& operator <<(ostream& outs, const MPair<K, V>& print_me) {
        outs << print_me.key << ":";
        for (int i = 0; i < print_me.value_list.size(); i++) {
            outs << print_me.value_list[i] << " ";
        }
        return outs;
        
    }
    friend bool operator !=(const MPair<K, V>& lhs, const MPair<K, V>& rhs) {
        return (lhs.key != rhs.key);
    }    
    friend bool operator ==(const MPair<K, V>& lhs, const MPair<K, V>& rhs) {
        return (lhs.key == rhs.key);
    }
    friend bool operator < (const MPair<K, V>& lhs, const MPair<K, V>& rhs) {
        return (is_lsp(lhs.key , rhs.key));
    }
    friend bool operator <= (const MPair<K, V>& lhs, const MPair<K, V>& rhs) {
        return (is_ls(lhs.key , rhs.key));
    }
    // Same goes for here
    friend bool operator > (const MPair<K, V>& lhs, const MPair<K, V>& rhs) {
        return (is_gtp(lhs.key,rhs.key));
    }
    friend bool operator >= (const MPair<K, V>& lhs, const MPair<K, V>& rhs) {
        return (is_ge(lhs.key, rhs.key));
    }    
    friend MPair<K, V> operator + (const MPair<K, V>& lhs, const MPair<K, V>& rhs) {
        if (lhs.key == rhs.key) {
            vector<V> holder = lhs.value_list;
            for (int i = 0; i < rhs.value_list.size(); i++) {
                holder.push_back(rhs.value_list[i]);
            }
            
            return MPair<K, V>(lhs.key, holder);
        }
        return lhs;
    }
};

template <typename K, typename V>
class MMap
{
public:
    typedef BPlusTree<MPair<K, V> > map_base;
    class Iterator{
    public:
        friend class MMap;
        Iterator() {}
        Iterator(typename map_base::Iterator it) {
            _it = it;
        }
        Iterator operator ++(int unused) {
            Iterator temp(_it);
            _it++;
            return temp;
        }
        Iterator operator ++() {
            return ++_it;
        }
        MPair<K, V> operator *() {
            return *_it;
        }
        friend bool operator ==(const Iterator& lhs, const Iterator& rhs) {
            return (lhs._it == rhs._it);
        }
        friend bool operator !=(const Iterator& lhs, const Iterator& rhs) {
            return (lhs._it != rhs._it);
        }

    private:
        typename map_base::Iterator _it;
    };

    MMap() {
        mmap = BPlusTree<MPair<K, V> >();
    }

//  Iterators
    Iterator begin();
    Iterator end();

//  Capacity
    int size() const; // done
    bool empty() const; // empty

//  Element Access
    const vector<V>& operator[](const K& key) const;  // done
    vector<V>& operator[](const K& key);  // done

//  Modifiers
    vector<V>& at(const K& key); // done
    void insert(const K& k, const V& v); // done
    void erase(const K& key); // done
    void clear(); // done

//  Operations:
    bool contains(const K& key) const ; // done
    vector<V> &get(const K& key); // done

    Iterator find(const K& key); // done
    int count(const K& key); // done
    // I have not writtent hese yet, but we will need them:
    Iterator lower_bound(const K& key); // done
    Iterator upper_bound(const K& key); // done
    //    equal_range:
    void print_lookup();


    bool is_valid() {return mmap.is_valid();}

    friend ostream& operator<<(ostream& outs, const MMap<K, V>& print_me){
        outs<<print_me.mmap<<endl;
        return outs;
    }

private:
    BPlusTree<MPair<K, V> > mmap;
};
// Just iterate through the multimap
template <typename K, typename V>
void MMap<K,V>::print_lookup() {
    for (auto i = mmap.begin(); i != mmap.end(); i++) {
        cout << setw(5)  << (*i).key << ": " << (*i).value_list <<endl;
    }
}

template <typename K, typename V>
typename MMap<K, V>::Iterator MMap<K, V>::lower_bound(const K& key) {
    return Iterator(mmap.lower_bound(MPair<K,V>(key)));
}

template <typename K, typename V>
typename MMap<K, V>::Iterator MMap<K, V>::upper_bound(const K& key) {
    return Iterator(mmap.upper_bound(MPair<K,V>(key)));
}

template <typename K, typename V>
typename MMap<K, V>::Iterator MMap<K, V>::begin() {
    return Iterator(mmap.begin());
}

template <typename K, typename V>
typename MMap<K, V>::Iterator MMap<K, V>::end() {
    return Iterator(mmap.end());
}

template <typename K, typename V>
typename MMap<K, V>::Iterator MMap<K,V>::find(const K& key) {
    return Iterator(mmap.find(MPair<K, V>(key)));
}

template <typename K, typename V>
int MMap<K,V>::size() const {
    return mmap.size();
}

template <typename K, typename V>
bool MMap<K,V>::empty() const {
    return mmap.empty();
}

template <typename K, typename V>
vector<V>& MMap<K,V>::operator[](const K& key) {
    return mmap.get(MPair<K, V>(key)).value_list;
}

template <typename K, typename V>
const vector<V>& MMap<K,V>::operator[](const K& key) const {
    return mmap.get(MPair<K, V>(key)).value_list;
}

template <typename K, typename V>
vector<V>& MMap<K,V>::at(const K& key) {
    //return mmap.get(MPair<K, V>(key)).value_list;
    return (*this)[key];
}

template <typename K, typename V>
void MMap<K,V>::insert(const K& k, const V& v) {
    MPair<K, V> temp(k, v);
    mmap.insert(temp);
}

template <typename K, typename V>
void MMap<K,V>::erase(const K& key) {

}

template <typename K, typename V>
void MMap<K,V>::clear() {
    mmap.clear_tree();
}

template <typename K, typename V>
vector<V> & MMap<K,V>::get(const K& key) {
    return mmap.get(MPair<K, V>(key)).value_list;
}

template <typename K, typename V>
bool MMap<K,V>::contains(const K& key) const {
    return mmap.contains(MPair<K,V>(key));
}

template <typename K, typename V>
int MMap<K,V>::count(const K& key) {
    return 0;
}








#endif // MULTIMAP_H


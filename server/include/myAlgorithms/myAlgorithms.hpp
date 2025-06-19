// server/include/myAlgoritmics/myAlgoritmics.hpp

#ifndef MYALGORITMICS_HPP
#define MYALGORITMICS_HPP

#include <string.h>
#include <sys/types.h>

/*  Cheking_if_a_port_value_is_valid function - gets the string by the index
 *  Parameters:
 *  port - pointer to the string (can be NULL)
 *
 *  Returns:
 *  port number - with success
 *  -1          - in case of error or port == NULL
 */

ssize_t Checking_if_a_port_value_is_valid(const char *port);


/*  Checking_the_validity_of_the_IP_value function - gets the string by the index
 *  Parameters:
 *  ip - pointer to the string (can be NULL)
 *
 *  Returns:
 *  true    - with success
 *  false   - if ptr == NULL of in case error
 */

bool Checking_the_validity_of_the_IP_value(const char *ip);


/*
 * hash table 
 * key - const char *
 * value - const cahr * 
 */

class HashTableStrings
{
    struct Item 
    {
        char *index;
        char *value;
        Item *next;
    };
    Item *first;
    int defval;
    int size;
public:
    HashTableStrings(const char &str) : first(0), defval(0), size(0) {}
};

/* 
 *  class SparseArray
 */

template<class Key, class T>
class SparseArray
{
    struct Item
    {
        Key index;
        T value;
        Item *next;
    };
    Item *first;
    T defval;
    int size;
public:
    SparseArray(const T &dv) : first(0), defval(dv), size(0) {}
    ~SparseArray();

    class Interm
    {
        friend class SparseArray<Key, T>;

        SparseArray<Key, T> *master;
        Key key;

        Interm(SparseArray<Key, T> *a_master, Key k)
            : master(a_master), key(k) {}

        T& Provide();
        void Remove();
    public:
        operator T();
        T operator=(const T &x);
        T operator->() { return Provide(); }
    };

    friend class Interm;

    Interm operator[](Key k)
        { return Interm(this, k); }

    T operator[](Key k) const;

    int NonzeroCount() const { return size; }

private:
    SparseArray(const SparseArray<Key, T>&) {}
    void operator=(const SparseArray<Key, T>&) {}
};


// methods class SparseArray
template <class Key, class T>
SparseArray<Key, T>::~SparseArray()
{
    while(first)
    {
        Item *tmp = first;
        first = first->next;
        delete tmp;
    }
}

template <class Key, class T>
T SparseArray<Key, T>::operator[](Key k) const 
{
    Item *tmp;
    for(tmp = first; tmp; tmp = tmp->next)
    {
        if(tmp->key == k)
            return tmp->value;
    }
    return defval;
}

// class SparseArray::Interm
template <class Key, class T>
T& SparseArray<Key, T>::Interm::Provide()
{
    Item *tmp;
    for(tmp = master->first; tmp; tmp = tmp->next)
        if(tmp->key == key)
            return tmp->value;

    tmp = new Item;
    tmp->key = key;
    tmp->next = master->first;
    master->first = tmp;
    master->size++;
    return tmp->value;
}

template <class Key, class T>
void SparseArray<T>::Interm::Remove()
{
    Item **tmp;
    for(tmp = &(master->first); *tmp; tmp = &(*tmp)->next)
    {
        if((*tmp)->key == key)
        {
            Item *delete_to = *tmp;
            *tmp = (*tmp)->next;
            delete delete_to;
            master->size--;
            return;
        }
    }
}

template <class Key, class T>
SparseArray<T>::Interm::operator T()
{
    Item *tmp;
    for(tmp = master->first; tmp; tmp = tmp->next)
        if(tmp->key == key)
            return tmp->value;

    return master->defval;
}

template <class Key, class T>
T SparseArray<T>::Interm::operator=(const T &x)
{
    if(x == master->defval)
        Remove();
    else
        Provide() = x;
    return x;
}

// class String

class String
{
    char *data;
public:
    String(const char *str)
    {
        data = new char[strlen(str) + 1];
        strcpy(data, str);
    }
    ~Stirng() { delete[] data; }
    bool operator==(const String& other) const { return strcmp(data, other.data) == 0; }
private:
    String(const String&) {}
    void operator=(const Strong&) {}
};

#endif // MYALGORITMICS_HPP

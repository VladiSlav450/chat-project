// server/include/myAlgoritmics/myAlgoritmics.hpp

#ifndef MYALGORITMICS_HPP
#define MYALGORITMICS_HPP

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
 *  class SparseArray
 */

template<class T>
class SparseArray
{
    struct Item
    {
        int index;
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
        friend class SparseArray<T>;

        SparseArray<T> *master;
        int index;

        Interm(SparseArray<T> *a_master, int ind)
            : master(a_master), index(ind) {}

        T& Provide();
        void Remove();
    public:
        operator T();
        T operator=(const T &x);
        T operator->() { return Provide(); }
    };

    friend class Interm;

    Interm operator[](int idx)
        { return Interm(this, idx); }

    T operator[](int idx) const;

    int NonzeroCount() const { return size; }

private:
    SparseArray(const SparseArray<T>&) {}
    void operator=(const SparseArray<T>&) {}
};


// methods class SparseArray
template <class T>
SparseArray<T>::~SparseArray()
{
    while(first)
    {
        Item *tmp = first;
        first = first->next;
        delete tmp;
    }
}

template <class T>
T SparseArray<T>::operator[](int idx) const 
{
    Item *tmp;
    for(tmp = first; tmp; tmp = tmp->next)
    {
        if(tmp->index == idx)
            return tmp->value;
    }
    return defval;
}

// class SparseArray::Interm
template <class T>
T& SparseArray<T>::Interm::Provide()
{
    Item *tmp;
    for(tmp = master->first; tmp; tmp = tmp->next)
        if(tmp->index == index)
            return tmp->value;

    tmp = new Item;
    tmp->index = index;
    tmp->next = master->first;
    master->first = tmp;
    master->size++;
    return tmp->value;
}

template <class T>
void SparseArray<T>::Interm::Remove()
{
    Item **tmp;
    for(tmp = &(master->first); *tmp; tmp = &(*tmp)->next)
    {
        if((*tmp)->index == index)
        {
            Item *delete_to = *tmp;
            *tmp = (*tmp)->next;
            delete delete_to;
            master->size--;
            return;
        }
    }
}

template <class T>
SparseArray<T>::Interm::operator T()
{
    Item *tmp;
    for(tmp = master->first; tmp; tmp = tmp->next)
        if(tmp->index == index)
            return tmp->value;

    return master->defval;
}

template <class T>
T SparseArray<T>::Interm::operator=(const T &x)
{
    if(x == master->defval)
        Remove();
    else
        Provide() = x;
    return x;
}

#endif // MYALGORITMICS_HPP

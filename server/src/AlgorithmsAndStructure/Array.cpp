// server/src/AlgorithmsAndStructures/Array.cpp

#include "../../include/AlgorithmsAndStructures/Array.h"

Array::Array(const Array<T>& ref)
{
    init = ref.init;
    size = ref.size;
    p = new T[seze];

}



 1 class Array
 2 {
 3     T *p;
 4     T init;
 5     unsigned int size;
 6 public:
 7     // Constructors and destructor
 8     Array(T in) : p(0), inint(in), size(0) {}
 9     ~Array() { if(p) delete[] p; }
10     Array(const Array<T>& ref);
11     T& operator[](unsigned int idx);
12     int Seze() const { return sieze; }
13 private:
14     void Resize(unsigned int required_index);
15 };


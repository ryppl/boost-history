/* simple example for using class array<>
 */
#include <iostream>
#include "policy_vector1.hpp"

template <typename T>
void test_static_size (const T& cont)
{
    int tmp[T::static_size];
    for (unsigned i=0; i<T::static_size; ++i) {
        tmp[i] = int(cont[i]);
    }
    for (unsigned i=0; i<T::static_size; ++i) {
        std::cout << tmp[i] << ' ';
    }
    std::cout << std::endl;
}

	 
template <template <class T2,std::size_t N> class StoragePolicy >
void test_policy()
{
    // define special type name
    typedef policy_vector<float,6,StoragePolicy> Array;

    // create and initialize an array
    float data[1] = { 42.42 };
    const Array a = data;

    // use some common STL container operations
    std::cout << "static_size: " << a.size() << std::endl;
    std::cout << "size:        " << a.size() << std::endl;
    // Can't use std::boolalpha because it isn't portable
    std::cout << "empty:       " << (a.empty()? "true" : "false") << std::endl;
    std::cout << "max_size:    " << a.max_size() << std::endl;
    std::cout << "front:       " << a.front() << std::endl;
    std::cout << "back:        " << a.back() << std::endl;
    std::cout << "[0]:         " << a[0] << std::endl;
    std::cout << "elems:       ";

    // iterate through all elements
    for (Array::const_iterator pos=a.begin(); pos<a.end(); ++pos) {
        std::cout << *pos << ' ';
    }
    std::cout << std::endl;
    test_static_size(a);

    // check copy constructor and assignment operator
    Array b(a);
    Array c;
    c = a;
    if (a==b && a==c) {
        std::cout << "copy construction and copy assignment are OK"
                  << std::endl;
    }
    else {
        std::cout << "copy construction and copy assignment are BROKEN"
                  << std::endl;
    }
    

    typedef policy_vector<double,6,StoragePolicy> DArray;
    typedef policy_vector<int,6,StoragePolicy> IArray;
    int idata[6] = { 1, 2, 3, 4, 5, 6 } ;
    IArray ia = idata;
    DArray da;
    da = ia;
    da.assign(42);
}

int main()
{
  std::cout << "TESTING FIXED_AGGREGATE_ARRAY" << std::endl;
  test_policy<fixed_aggregate_array>();
  std::cout << "TESTING FIXED_ALLOCATED_ARRAY" << std::endl;
  test_policy<fixed_allocated_array>();

  return 0;  // makes Visual-C++ compiler happy
}


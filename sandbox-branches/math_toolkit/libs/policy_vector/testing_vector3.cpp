/* simple example for using class array<>
 */
#include <iostream>
#include "policy_vector3.hpp"

template <typename Array>
void test_static_size (const Array& cont)
{
    int tmp[Array::static_size];
    for (unsigned i=0; i<Array::static_size; ++i) {
        tmp[i] = int(cont[i]);
    }
    for (unsigned i=0; i<Array::static_size; ++i) {
        std::cout << tmp[i] << ' ';
    }
    std::cout << std::endl;
}
	 
template <class Array>
void test_policy(const Array &a)
{
    // use some common STL container operations
    std::cout << "size:        " << a.size() << std::endl;
    // Can't use std::boolalpha because it isn't portable
    std::cout << "empty:       " << (a.empty()? "true" : "false") << std::endl;
    std::cout << "max_size:    " << a.max_size() << std::endl;
    std::cout << "front:       " << a.front() << std::endl;
    std::cout << "back:        " << a.back() << std::endl;
    std::cout << "[0]:         " << a[0] << std::endl;
    std::cout << "elems:       ";

    // iterate through all elements
    for (typename Array::const_iterator pos=a.begin(); pos<a.end(); ++pos) {
        std::cout << *pos << ' ';
    }
    std::cout << std::endl;

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
}

template <class StoragePolicy >
void test_fixed_size_policy()
{
    // define special type name
    typedef policy_vector<float,StoragePolicy> Array;

    // create and initialize an array
    float data[1] = { 42.42 };
    const Array a = data;

    test_policy<Array>(a);
    test_static_size<Array>(a);

    typedef policy_vector<double,StoragePolicy> DArray;
    typedef policy_vector<int,StoragePolicy> IArray;
    int idata[6] = { 1, 2, 3, 4, 5, 6 } ;
    IArray ia = idata;
    DArray da;
    da = ia;
    da.assign(42);
}

template <class StoragePolicy >
void test_variable_size_policy()
{
    // define special type name
    typedef policy_vector<float,StoragePolicy> Array;

    // create and initialize an array
    Array a;

    for (int i=0; i<100; i += 2) {
      a.push_back((float)i);
      a.push_back((float)(i+1));
      a.pop_back();
    }

    std::cout << "capacity:    " << a.capacity() << std::endl;
    test_policy<Array>(a);
}

int main()
{
  std::cout << "TESTING FIXED_AGGREGATE_ARRAY\n";
  test_fixed_size_policy<fixed_aggregate_array<6> >();
  std::cout << "TESTING FIXED_ALLOCATED_ARRAY\n";
  test_fixed_size_policy<fixed_allocated_array<6> >();
  std::cout << "TESTING VARIABLE ALLOCATED_ARRAY WITH DOUBLING\n";
  test_variable_size_policy< variable_allocated_array<double_resize> >();
  std::cout << "TESTING VARIABLE ALLOCATED_ARRAY WITH DOUBLING/HALVING\n";
  test_variable_size_policy< variable_allocated_array<halfdouble_resize> >();

  return 0;  // makes Visual-C++ compiler happy
}


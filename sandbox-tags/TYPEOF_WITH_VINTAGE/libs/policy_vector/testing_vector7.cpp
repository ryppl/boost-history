/* a comprehensive test suite for using class policy_vector<>
 */

#include <iostream>
#include <vector>
#include <boost/timer.hpp>
#include "policy_vector7.hpp"

// Testing sizes
const std::size_t N = 100000;  // actual size of containers
const std::size_t initN = 0;    // variable size, starting size
// const std::size_t initN = 16 // variable size, starting size

template <class T>
void ignore_unused_variable_warning(T&) {}


/******************************
 * According to kind of array
 ******************************/
 
template <typename T>
void test_type() {}

template <class Array>
void test_array(const Array &a, bool checking, bool iter_checking)
{
  // test typedefs
  test_type<typename Array::value_type>(); 
  test_type<typename Array::reference>(); 
  test_type<typename Array::const_reference>(); 
  test_type<typename Array::iterator>(); 
  test_type<typename Array::const_iterator>(); 
  test_type<typename Array::size_type>(); 
  test_type<typename Array::difference_type>(); 

  // typedefs needed here
  typedef typename Array::value_type     value_type;
  typedef typename Array::iterator       iterator;
  typedef typename Array::const_iterator const_iterator;
  
  // iterate through all elements
  // will also exercise begin(), end() -- const versions
  const_iterator first(a.begin()), last(a.end()), pos;
  boost::timer t;
  float sum = 0;
  for (pos = first; pos != last; ++pos) sum += *pos;
  std::cout << "\ttraversal:     " << t.elapsed() << "s\n";

  // reverse_iterate through all elements
  // will also exercise rbegin(), rend() -- const versions
  typedef typename Array::reverse_iterator       reverse_iterator;
  typedef typename Array::const_reverse_iterator const_reverse_iterator;
  const_reverse_iterator rfirst(a.rbegin()), rlast(a.rend()), rpos;
  boost::timer t1;
  float rsum = 0;
  for (rpos = rfirst; rpos != rlast; ++rpos) rsum += *rpos;
  std::cout << "\trtraversal:    " << t1.elapsed() << "s\n";

  // size()
  std::size_t n = 0;
  for (pos = first; pos != last; ++pos) ++n;
  if (n == a.size())
    std::cout << "\tsize is OK\n";
  else
    std::cout << "\tsize is BROKEN\n";

  // exercise and check equality comparison
  if (a==a)
    std::cout << "\tequality comparison is OK\n";
  else 
    std::cout << "\tequality comparison is BROKEN\n";

  // check copy constructor and assignment operator
  Array b(a);
  Array c; c = a;
  if (a==b && a==c)
    std::cout << "\tcopy construction and copy assignment are OK\n";
  else
    std::cout << "\tcopy construction and copy assignment are BROKEN\n";

  // test assign() with two arguments
  // will also exercise begin(), end() -- non-const versions
  b.assign(b.size(), (value_type)42);
  iterator bfirst(b.begin()), blast(b.end()), bpos;
  for (bpos=bfirst; bpos != blast; ++bpos)
      if (*bpos != 42) {
         std::cout << "\tassign value is BROKEN\n";
	 break;
      }
  if (bpos == blast)
    std::cout << "\tassign value is OK\n";

  // test assign() with range
  // will also exercise rbegin(), rend() -- non-const versions
  c.assign(b.begin(), b.end());
  reverse_iterator crfirst(c.rbegin()), crlast(c.rend()), crpos;
  for (crpos=crfirst; crpos != crlast; ++crpos)
      if (*crpos != 42) {
         std::cout << "\tassign range is BROKEN\n";
	 break;
      }
  if (crpos == crlast)
    std::cout << "\tassign range is OK\n";

  // exercise and time swap()
  Array d(a);
  boost::timer t2;
  b.swap(d);
  std::cout << "\tswap:          " << t2.elapsed() << "s\n";
  if (b != a)
    std::cout << "\tswap is BROKEN\n";
  else
    std::cout << "\tswap is OK\n";

  // exercise checking policy of at()
  bool caught = false;
#ifndef POLICY_VECTOR_TESTING_STD_VECTOR
  // at() is not present in some (non-standard) vector implementations
  // notably g++ 2.95.2
  try {
    value_type x = a.at(a.size());
    ignore_unused_variable_warning(x);
  }
  catch (std::range_error e) {
    std::cout << "\tat() caught exception: " << e.what();
    caught = true;
  }
  if (!caught)
    std::cout << "\tat() did not catch any exception\n";
#endif

  // exit if no checking policy -- would segfault
  if (!checking) return;

  // exercise checking policy of operator[]
  caught = false;
  try {
    value_type x = a[a.size()];
    ignore_unused_variable_warning(x);
  }
  catch (std::range_error e) {
    std::cout << "\toperator[] caught exception: " << e.what();
    caught = true;
  }
  if (!caught)
    std::cout << "\toperator[] did not catch exception\n";

  // exit if no checking policy -- would segfault
  if (!iter_checking) return;

  // exercise checking policy of iterators
  caught = false;
  try {
    value_type x = *a.end();
    ignore_unused_variable_warning(x);
  }
  catch (std::range_error e) {
    std::cout << "\titerator caught exception: " << e.what();
    caught = true;
  }
  if (!caught)
    std::cout << "\titerator did not catch exception\n";
}

template <class Array>
void test_fixed_size_array(bool checking, bool iter_checking)
{
  // create and initialize an array of same size
  float data[Array::static_size];
  for (std::size_t i=0; i<Array::static_size; ++i) data[i] = i;    
  const Array a = data;
  if (!std::equal(data, data+Array::static_size, a.begin()))
    std::cout << "\tconversion from plain array BROKEN\n";
  else
    std::cout << "\tconversion from plain array OK\n";

  // test general members of Array
  test_array<Array>(a, checking, iter_checking);
}

template <class Array>
void test_variable_size_array(bool checking, bool iter_checking)
{
  typedef typename Array::iterator iterator;
  const float tmp[4] = { -0.8, -0.6, -0.4, -0.2 };

  // create and initialize array
  Array a;
  if (a.size() != 0)
    std::cout << "\tdefault constructor BROKEN\n";
  else
    std::cout << "\tdefault constructor OK\n";

  // test constructors (within scope)
  {
    Array b(100);
    if (b.size() != 100)
      std::cout << "\tsize constructor BROKEN\n";
    Array c(tmp, tmp+4);
    if (c.size() != 4 || !std::equal(tmp, tmp+4, c.begin()))
      std::cout << "\trange constructor BROKEN\n";
    if (b.size() == 100 && c.size() == 4 && std::equal(tmp, tmp+4, c.begin()))
      std::cout << "\trange and size constructors OK\n";
  }

  // exercise and time push_back, pop_back
  const std::size_t N2 = 2*N;
  a.reserve(N); // for speed
  boost::timer t;
  for (std::size_t i=0; i<N2; ++i)
    a.push_back((float)i);
  for (std::size_t i=0; i<N; ++i)
    a.pop_back();
  if (a.size() != N)
    std::cout << "\tpush/pop_back are BROKEN\n";
  else 
    std::cout << "\tpush/pop_back: " << t.elapsed() << "s\n";

  // look at capacity after insertions
  // std::cout << "\tinit_size:     " << a.static_init_size << "\n";;
  std::cout << "\tcapacity:      " << a.capacity() << "\n";;
  std::cout << "\tsize:          " << a.size() << "\n";;

  // test general functions of Array
  test_array<Array>(a, checking, iter_checking);

  // test insert functions
  iterator first, last, pos;
  bool insert_broken_reported = false;
  a.insert(a.begin(), -2.0);
  pos = first = a.begin()+1; last = a.end();
  try {
  for (std::size_t i=0; pos != last; ++pos, ++i)
    if (*pos != i) break;
  if (a[0] != -2.0 || pos != last) {
    std::cout << "\tinsert one element is BROKEN\n";
    insert_broken_reported = true;
  }
  }
  catch (std::range_error e) {
    std::cout << "\tcaught exception during insert one element: " << e.what();
  }
  a.insert(a.begin()+1, 3, -1.0);
  pos = first = a.begin()+4; last = a.end();
  try{
  for (std::size_t i=0; pos != last; ++pos, ++i)
    if (*pos != i) break;
  if (a[0] != -2.0 || a[1] != -1.0 || a[2] != -1.0 || a[3] != -1.0
   || pos != last) {
    std::cout << "\tinsert n copies of one element is BROKEN\n";
    insert_broken_reported = true;
  }
  }
  catch (std::range_error e) {
    std::cout << "\tcaught exception during insert n copies: " << e.what();
  }
  a.insert(a.begin()+4, tmp, tmp+4);
  pos = first = a.begin()+8; last = a.end();
  try{
  for (std::size_t i=0; pos != last; ++pos, ++i)
    if (*pos != i) break;
  if (a[0] != -2.0 || a[1] != -1.0 || a[2] != -1.0 || a[3] != -1.0
   || a[4] != tmp[0] || a[5] != tmp[1] || a[6] != tmp[2] || a[7] != tmp[3]
   || pos != last) {
    std::cout << "\tinsert range is BROKEN\n";
    insert_broken_reported = true;
  }
  }
  catch (std::range_error e) {
    std::cout << "\tcaught exception during insert range: " << e.what();
  }
  if (!insert_broken_reported)
    std::cout << "\tinsert functions are OK\n";

  // test erase functions
  a.erase(a.begin());
  try{
  if (a[0] != -1.0 || a[1] != -1.0 || a[2] != -1.0
   || a[3] != tmp[0] || a[4] != tmp[1] || a[5] != tmp[2] || a[6] != tmp[3])
       std::cout << "\terase position is BROKEN\n";
  else {
    pos = a.begin()+7; last = a.end();
    for (std::size_t i=0; pos != last; ++pos, ++i)
      if (*pos != i) {
        std::cout << "\terase position is BROKEN\n";
        break;
      }
  }
  }
  catch (std::range_error e) {
    std::cout << "\tcaught exception during erase position: " << e.what();
  }
  a.erase(a.begin(), a.begin()+7);
  pos = a.begin(); last = a.end();
  try{
  for (std::size_t i=0; pos != last; ++pos, ++i)
    if (*pos != i) {
       std::cout << "\terase range is BROKEN\n";
       break;
    }
  }
  catch (std::range_error e) {
    std::cout << "\tcaught exception during erase range: " << e.what();
  }
  if (pos == last)
    std::cout << "\terase functions are OK\n";
}

/***********************************
 * Dispatching for fixed size arrays
 ***********************************/

template <class StoragePolicy,
	  template <class S> class CheckingPolicy,
	  template <class T, class V> class IteratorPolicy>
void test_fixed_size_policy(bool checking, bool iter_checking)
{
  // define special type names
  typedef policy_vector<float, StoragePolicy,
                        CheckingPolicy, IteratorPolicy> Array;
  // testing proper
  test_fixed_size_array<Array>(checking, iter_checking);
}

template <class StoragePolicy>
void test_fixed_size()
{
  std::cout << "    a. RAW POINTER ITERATOR, NO CHECKING\n";
  test_fixed_size_policy<StoragePolicy,
    no_check, raw_pointer_iterator>(false, false);
  std::cout << "    b. RAW POINTER ITERATOR, EXCEPTION CHECKING\n";
  test_fixed_size_policy<StoragePolicy,
    exception_check, raw_pointer_iterator>(true, false);
  std::cout << "    c. CHECKED POINTER ITERATOR, NO CHECKING\n";
  test_fixed_size_policy<StoragePolicy,
    no_check, checked_pointer_iterator>(false, false);
  std::cout << "    d. CHECKED POINTER ITERATOR, EXCEPTION CHECKING\n";
  test_fixed_size_policy<StoragePolicy,
    exception_check, checked_pointer_iterator>(true, true);
  std::cout << "    e. CHECKED POINTER ITERATOR, ASSERT CHECKING\n";
  test_fixed_size_policy<StoragePolicy,
    assert_check, checked_pointer_iterator>(false, false);
  // for assert, don't check, otherwise assert will stop the program
  std::cout << "    f. INDEX ITERATOR, NO CHECKING\n";
  test_fixed_size_policy<StoragePolicy,
    no_check, index_iterator>(false, false);
  std::cout << "    g. INDEX ITERATOR, EXCEPTION CHECKING\n";
  test_fixed_size_policy<StoragePolicy,
    exception_check, index_iterator>(true, true);
  std::cout << "    h. INDEX ITERATOR, ASSERT CHECKING\n";
  test_fixed_size_policy<StoragePolicy,
    assert_check, index_iterator>(false, false);
  // for assert, don't check, otherwise assert will stop the program
}


/**************************************
 * Dispatching for variable size arrays
 **************************************/

template <class StoragePolicy,
	  template <class S> class CheckingPolicy,
	  template <class T, class V> class IteratorPolicy>
void test_variable_size_policy(bool checking, bool iter_checking)
{
  // define special type names and temp variable
  typedef policy_vector<float, StoragePolicy,
                        CheckingPolicy, IteratorPolicy> Array;
  // testing proper
  test_variable_size_array<Array>(checking, iter_checking);
}

template <class StoragePolicy>
void test_variable_size()
{
  std::cout << "    a. RAW POINTER ITERATOR, NO CHECKING\n";
  test_variable_size_policy<StoragePolicy,
    no_check, raw_pointer_iterator>(false, false);
  std::cout << "    b. RAW POINTER ITERATOR, EXCEPTION CHECKING\n";
  test_variable_size_policy<StoragePolicy,
    exception_check, raw_pointer_iterator>(true, false);
  std::cout << "    c. CHECKED POINTER ITERATOR, NO CHECKING\n";
  test_variable_size_policy<StoragePolicy,
    no_check, checked_pointer_iterator>(false, false);
  std::cout << "    d. CHECKED POINTER ITERATOR, EXCEPTION CHECKING\n";
  test_variable_size_policy<StoragePolicy,
    exception_check, checked_pointer_iterator>(true, true);
  std::cout << "    e. CHECKED POINTER ITERATOR, ASSERT CHECKING\n";
  test_variable_size_policy<StoragePolicy,
    assert_check, checked_pointer_iterator>(false, false);
  // for assert, don't check, otherwise assert will stop the program
  std::cout << "    f. INDEX ITERATOR, NO CHECKING\n";
  test_variable_size_policy<StoragePolicy,
    no_check, index_iterator>(false, false);
  std::cout << "    g. INDEX ITERATOR, EXCEPTION CHECKING\n";
  test_variable_size_policy<StoragePolicy,
    exception_check, index_iterator>(true, true);
  std::cout << "    h. INDEX ITERATOR, ASSERT CHECKING\n";
  test_variable_size_policy<StoragePolicy,
    assert_check, index_iterator>(false, false);
  // for assert, don't check, otherwise assert will stop the program
}

/******************************
 * Testing framework
 ******************************/

// resize policies
typedef incremental_resize<1024>      i2E10;
typedef incremental_resize<16536>     i2E16;
typedef multiply_resize<2>            m2;
typedef multiply_divide_resize<2>     md2;
typedef multiply_resize<4>            m4;
typedef multiply_divide_resize<4>     md4;

// storage policies
typedef fixed_aggregate_array<N>              array;
typedef fixed_allocated_array<N>              fixed;
typedef variable_allocated_array<i2E10,initN> vari2E10;  
typedef variable_allocated_array<i2E16,initN> vari2E16;  
typedef variable_allocated_array<m2,initN>    varm2;
typedef variable_allocated_array<m4,initN>    varm4;
typedef variable_allocated_array<md2,initN>   varmd2;
typedef variable_allocated_array<md4,initN>   varmd4;

inline void separator()
{
  std::cout << "==========================================================================\n";
}

int main()
{
#if defined(POLICY_VECTOR_TESTING_FIXED_AGGREGATE)
  separator();
  std::cout <<
    "1. TESTING FIXED AGGREGATE ARRAY\n";
  test_fixed_size< array >();
  separator();
#endif

#if defined(POLICY_VECTOR_TESTING_FIXED_ALLOCATED)
  separator();
  std::cout <<
    "2. TESTING FIXED ALLOCATED ARRAY\n";
  test_fixed_size< fixed >();
  separator();
#endif

#if defined(POLICY_VECTOR_TESTING_VARIABLE_INCR)
  separator();
  std::cout <<
    "3.1 TESTING VARIABLE ALLOCATED ARRAY WITH INCREMENTAL RESIZING (1024)\n";
  test_variable_size< vari2E10 >();
  separator();
  std::cout <<
    "3.2 TESTING VARIABLE ALLOCATED ARRAY WITH INCREMENTAL RESIZING (16536)\n";
  test_variable_size< vari2E16 >();
  separator();
#endif

#if defined(POLICY_VECTOR_TESTING_VARIABLE_GROW)
  separator();
  std::cout <<
    "4.1 TESTING VARIABLE ALLOCATED ARRAY WITH EXPONENTIAL GROWTH (x2)\n";
  test_variable_size< varm2 >();
  separator();
  std::cout <<
    "4.1 TESTING VARIABLE ALLOCATED ARRAY WITH EXPONENTIAL GROWTH (x4)\n";
  test_variable_size< varm4 >();
  separator();
#endif

#if defined(POLICY_VECTOR_TESTING_VARIABLE_GROW_SHRINK)
  separator();
  std::cout <<
    "5.1 TESTING VARIABLE ALLOCATED ARRAY WITH EXPONENTIAL RESIZING (x2,/2)\n";
  test_variable_size< varmd2 >();
  separator();
  std::cout <<
    "5.2 TESTING VARIABLE ALLOCATED ARRAY WITH EXPONENTIAL RESIZING (x4,/4)\n";
  test_variable_size< varmd4 >();
  separator();
#endif

#if defined(POLICY_VECTOR_TESTING_STD_VECTOR)
  separator();
  std::cout << "6 TESTING STD::VECTOR\n";
  test_variable_size_array< std::vector<float> >(false, false);
  separator();
#endif

  return 0;  // makes Visual-C++ compiler happy
}


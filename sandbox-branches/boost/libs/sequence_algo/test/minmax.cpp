#include <utility>
#include <functional>
#include <algorithm>
#include <numeric>
#include <iterator>
#include <vector>
#include <list>
#include <set>

#include <iostream>
#include "./minmax.hpp"

template <class T1, class T2, class T3, class T4>
void tie(std::pair<T1, T2> p, T3& first, T4& second)
{
  first = T3(p.first); second = T4(p.second);
}

template <class Value>
struct less_count : std::less<Value> {
  less_count(less_count<Value> const& lc) : _M_counter(lc._M_counter) {}
  less_count(int& counter) : _M_counter(counter) {}
  bool operator()(Value const& a, Value const& b) const {
    ++_M_counter;
    return std::less<Value>::operator()(a,b);
  }
  void reset() {
    _M_counter = 0;
  }
private:
  int& _M_counter;
};

inline int opt_min_count(int n) {
  return (n==0) ? 0 : n-1;
}
inline int opt_minmax_count(int n) {
  if (n < 2) return 0;
  if (n == 2) return 1;
  return (n%2 == 0) ? 3*(n/2)-1 : 3*(n/2)+1;
}
inline int opt_boost_minmax_count(int n) {
  if (n < 2) return 0;
  if (n == 2) return 1;
  return (n%2 == 0) ? 3*(n/2)-2 : 3*(n/2);
}

template <class CIterator>
void test_minmax(CIterator first, CIterator last, int n)
{
  typedef std::reverse_iterator<CIterator> RCIterator;
  // assume that CIterator is BidirectionalIter
  CIterator min, max;
  RCIterator rfirst(last), rlast(first), rmin(min), rmax(max);
  int counter = 0;
  less_count<typename std::iterator_traits<CIterator>::value_type> lc(counter);

  // standard extensions
  // first version, operator<
  tie( boost::minmax_element(first, last), min, max );
  assert( min == std::min_element(first, last) );
  assert( max == std::max_element(first, last) );
  // second version, comp function object (keeps a counter!)
  lc.reset();
  tie( boost::minmax_element(first, last, lc), min, max );
  assert( counter <= opt_minmax_count(n) );
  assert( min == std::min_element(first, last, lc) );
  assert( max == std::max_element(first, last, lc) );

  // boost extensions
  // first version, operator<
  assert( boost::first_min_element(first, last)
       == std::min_element(first, last) );
  rmin = RCIterator(boost::last_min_element(first, last));
  rmin = (rmin == rfirst) ? rlast : --rmin;
  assert( rmin == std::min_element(rfirst, rlast) );
  assert( boost::first_max_element(first, last)
       == std::max_element(first, last) );
  rmax = RCIterator(boost::last_max_element(first, last));
  rmax = (rmax == rfirst) ? rlast : --rmax;
  assert( rmax == std::max_element(rfirst, rlast) );
  tie( boost::first_min_last_max_element(first, last), min, max );
  assert( min == boost::first_min_element(first, last) );
  assert( max == boost::last_max_element(first, last) );
  tie( boost::last_min_first_max_element(first, last), min, max );
  assert( min == boost::last_min_element(first, last) );
  assert( max == boost::first_max_element(first, last) );
  tie( boost::last_min_last_max_element(first, last), min, max );
  assert( min == boost::last_min_element(first, last) );
  assert( max == boost::last_max_element(first, last) );
  // second version, comp function object (keeps a counter!)
  lc.reset();
  min = boost::first_min_element(first, last, lc);
  assert( counter <= opt_min_count(n) );
  assert( min == std::min_element(first, last, lc) );
  lc.reset();
  rmin = RCIterator(boost::last_min_element(first, last, lc));
  rmin = (rmin == rfirst) ? rlast : --rmin;
  assert( counter <= opt_min_count(n) );
  assert( rmin == std::min_element(rfirst, rlast, lc) );
  lc.reset();
  max =  boost::first_max_element(first, last, lc);
  assert( counter <= opt_min_count(n) );
  assert( max == std::max_element(first, last, lc) );
  lc.reset();
  rmax = RCIterator(boost::last_max_element(first, last, lc));
  rmax = (rmax == rfirst) ? rlast : --rmax;
  assert( counter <= opt_min_count(n) );
  assert( rmax == std::max_element(rfirst, rlast, lc) );
  lc.reset();
  tie( boost::first_min_last_max_element(first, last, lc), min, max );
  assert( counter <= opt_boost_minmax_count(n) );
  assert( min == boost::first_min_element(first, last, lc) );
  assert( max == boost::last_max_element(first, last, lc) );
  lc.reset();
  tie( boost::last_min_first_max_element(first, last, lc), min, max );
  assert( counter <= opt_boost_minmax_count(n) );
  assert( min == boost::last_min_element(first, last, lc) );
  assert( max == boost::first_max_element(first, last, lc) );
  lc.reset();
  tie( boost::last_min_last_max_element(first, last, lc), min, max );
  assert( counter <= opt_minmax_count(n) );
  assert( min == boost::last_min_element(first, last, lc) );
  assert( max == boost::last_max_element(first, last, lc) );
}

template <class Container, class Iterator, class Value>
void test_container(Iterator first, Iterator last, int n)
{
  Container c(first, last);
  test_minmax(c.begin(), c.end(), n);
}

template <class Iterator>
void test_range(Iterator first, Iterator last, int n, char* name)
{
  typedef typename std::iterator_traits<Iterator>::value_type Value;
  // Test various containers with these values
  std::cout << "  vector<" << name << ">...";
  test_container< std::vector<Value>, Iterator, Value >(first, last, n);
  std::cout << "  list<" << name << ">...";
  test_container< std::list<Value>,   Iterator, Value >(first, last, n);
  std::cout << "  set<" << name << ">...";
  test_container< std::set<Value>,    Iterator, Value >(first, last, n);
  std::cout << "\n";
}

template <class Value>
void test(int n, char* name)
{
  // Populate test vector with identical values
  std::cout << " Identical values...   ";
  std::vector<Value> test_vector(n, Value(1));
  typename std::vector<Value>::iterator first( test_vector.begin() );
  typename std::vector<Value>::iterator last( test_vector.end() );
  test_range(first, last, n, name);

  // Populate test vector with two values
  std::cout << " Two distinct values...";
  typename std::vector<Value>::iterator middle( first + n/2 );
  std::fill(middle, last, Value(2));
  test_range(first, last, n, name);

  // Populate test vector with increasing values
  std::cout << " Increasing values...  ";
  std::accumulate(first, last, Value(0));
  test_range(first, last, n, name);

  // Populate test vector with decreasing values
  std::cout << " Decreasing values...  ";
  std::reverse(first, last);
  test_range(first, last, n, name);

  // Populate test vector with random values
  std::cout << " Random values...      ";
  std::random_shuffle(first, last);
  test_range(first, last, n, name);
}

class custom {
  int _M_x;
  friend bool operator<(custom const& x, custom const& y);
public:
  explicit custom(int x = 0) : _M_x(x) {}
  custom(custom const& y) : _M_x(y._M_x) {}
  custom operator+(custom const& y) const { return custom(_M_x+y._M_x); }
  custom& operator+=(custom const& y) { _M_x += y._M_x; return *this; }
};

bool operator< (custom const& x, custom const& y)
{ return x._M_x < y._M_x; }

int
main(char argc, char** argv)
{
  int n = 100;
  if (argc > 1) n = atoi(argv[1]);

  test<int>(n, "builtin");
  test<custom>(n, "custom ");

  return 0;
}

#include <algorithm>
#include <numeric>
#include <iterator>
#include <vector>
#include <iostream>

#include <boost/timer.hpp>
#include <boost/array.hpp>
// #include <boost/sequence_algo/algorithm.hpp>

#include "linear_sort.hpp"

int array_size = 1000000; // one million

namespace std {

  template <class T, int N>
  std::ostream& operator<< (std::ostream& os, boost::array<T,N> const& t)
  {
    using namespace std;
    os << "(";
    copy( t.begin(), t.end(), ostream_iterator<int>(os," "));
    os << ")";
    return os;
  }

}

template <class Iterator, class T>
void assert_is_sorted(Iterator first, Iterator last)
{
  using namespace std;
  if (! is_sorted(first, last)) {
    copy( first, first+min(20,last-first), ostream_iterator<T>(cerr," "));
    if (last-first > 20) cerr << " ...\n";
    assert( false );
  }
}

template <class T>
void test_linear_sort_traits(char* type_name)
{
  using namespace std;
  typedef boost::linear_sort_traits<T> traits;

  cout << "VERIFYING LINEAR_SORT_TRAITS FOR TYPE " << type_name << "\n";
  cout << "   const_min  = " << (int)traits::const_min << "\n";
  cout << "   const_max  = " << (int)traits::const_max << "\n";
  cout << "   range_size = " << (int)traits::range_size << "\n";
  T x = traits::const_min, y = traits::const_max;
  for (size_t i = 0; i < traits::range_size; y = x, ++i, ++x)
    if (traits::to_index(x) != i)
      cerr << "   ERROR: to_index(" << (int)x << ") = " << traits::to_index(x)
	   << " (should be " << i << ")\n";
  if (y != traits::const_max)
    cerr << "   ERROR: type " << type_name << " did not end with max\n";
  if (x != traits::const_min)
    cerr << "   WARNING: type " << type_name << " does not wrap around\n";
}

template <class T>
void time_counting_sort(char* type_name)
{
  using namespace std;
  typedef boost::linear_sort_traits<T> traits;
  typedef typename vector<T>::iterator iterator;

  vector<T> v(array_size);
  for (iterator it = v.begin(); it != v.end(); ++it)
    *it = T(rand() % traits::range_size);
  cout << "TYPE " << type_name << " (with " << array_size << " elements)\n";

  vector<T> w(array_size, 0);
  boost::timer t;
  copy(v.begin(), v.end(), w.begin());
  sort(w.begin(), w.end());
  cout << "  std::sort w/ copy                    " << t.elapsed() << "\n";
  assert_is_sorted<iterator,int>(w.begin(), w.end());

  std::copy(v.begin(), v.end(), w.begin());
  t.restart();
  boost::counting_sort(w.begin(), w.end());
  cout << "  boost::counting_sort                 " << t.elapsed() << "\n";
  assert_is_sorted<iterator,int>(w.begin(), w.end());

  t.restart();
  boost::counting_sort_copy(v.begin(), v.end(), w.begin());
  cout << "  boost::counting_sort_copy            " << t.elapsed() << "\n";
  assert_is_sorted<iterator,int>(w.begin(), w.end());
}


template <class T, std::size_t N>
void time_radix_sort(char* type_name)
{
  using namespace std;

  typedef boost::array<T,N> array_t;
  typedef boost::linear_sort_traits<T> traits;
  typedef typename vector<array_t>::iterator iterator;

  vector< array_t > v(array_size);
  for (iterator it = v.begin(); it != v.end(); ++it)
    for (int i = 0; i < N; ++i)
      (*it)[i] = T(rand() % traits::range_size);
  cout << "TYPE " << type_name << "[" << N << "]"
       << " (with " << array_size << " elements)\n";

  vector< array_t > w(array_size);
  boost::timer t;
  copy(v.begin(), v.end(), w.begin());
  sort(w.begin(), w.end());
  cout << "  std::sort w/ copy and lexico compare " << t.elapsed() << "\n";
  assert_is_sorted<iterator,array_t>(w.begin(), w.end());

  copy(v.begin(), v.end(), w.begin());
  t.restart();
  boost::radix_sort(w.begin(), w.end(), N);
  cout << "  boost::radix_sort                    " << t.elapsed() << "\n";
  assert_is_sorted<iterator,array_t>(w.begin(), w.end());
  
  t.restart();
  boost::radix_sort_copy(v.begin(), v.end(), w.begin(), N);
  cout << "  boost::radix_sort_copy               " << t.elapsed() << "\n";
  assert_is_sorted<iterator,array_t>(w.begin(), w.end());
}

int main(char argc, char **argv)
{
  if (argc>1) array_size = atoi(argv[1]);
  std::size_t original_array_size = array_size;

  test_linear_sort_traits<bool>("bool");
  test_linear_sort_traits<char>("char");
  test_linear_sort_traits<signed char>("signed char");
  test_linear_sort_traits<unsigned char>("unsigned char");

  // time_counting_sort<bool>("bool");
  time_counting_sort<char>("char");
  time_counting_sort<signed char>("signed char");
  time_counting_sort<unsigned char>("unsigned char");

  array_size = original_array_size;
  time_radix_sort<bool,1>("bool");
  time_radix_sort<bool,3>("bool");
  array_size  /= 5;
  time_radix_sort<bool,5>("bool");
  time_radix_sort<bool,10>("bool");
  time_radix_sort<bool,25>("bool");

  array_size = original_array_size;
  time_radix_sort<char,1>("char");
  time_radix_sort<char,3>("char");
  array_size  /= 5;
  time_radix_sort<char,5>("char");
  time_radix_sort<char,10>("char");
  time_radix_sort<char,25>("char");

  array_size = original_array_size;
  time_radix_sort<unsigned char,1>("unsigned char");
  time_radix_sort<unsigned char,3>("unsigned char");
  array_size /= 5;
  time_radix_sort<unsigned char,5>("unsigned char");
  time_radix_sort<unsigned char,10>("unsigned char");
  time_radix_sort<unsigned char,25>("unsigned char");
}

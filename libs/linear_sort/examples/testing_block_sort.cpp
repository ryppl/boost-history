// put first to detect missing includes, etc.
#include <boost/linear_sort/block_sort.hpp>

#include <algorithm>
#include <numeric>
#include <iterator>
#include <vector>
#include <iostream>

#include <boost/timer.hpp>
#include <boost/array.hpp>
// #include <boost/sequence_algo/algorithm.hpp>

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


template <class T, std::size_t N>
void time_block_sort(char* type_name)
{
  using namespace std;

  typedef boost::array<T,N> array_t;
  typedef typename vector<array_t>::iterator iterator;

  vector< array_t > v(array_size);
  for (iterator it = v.begin(); it != v.end(); ++it)
    for (size_t i = 0; i < N; ++i)
      (*it)[i] = T(rand() % (1 << sizeof(T)*CHAR_BIT));
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
  boost::block_sort(w.begin(), w.end());
  cout << "  boost::block_sort                    " << t.elapsed() << "\n";
  assert_is_sorted<iterator,array_t>(w.begin(), w.end());
  
  t.restart();
  boost::block_sort_copy(v.begin(), v.end(), w.begin());
  cout << "  boost::block_sort_copy               " << t.elapsed() << "\n";
  assert_is_sorted<iterator,array_t>(w.begin(), w.end());
}

int main(char argc, char **argv)
{
  if (argc>1) array_size = atoi(argv[1]);
  std::size_t original_array_size = array_size;

  #if 0
  array_size = original_array_size;
  time_block_sort<bool,1>("bool");
  time_block_sort<bool,3>("bool");
  array_size  /= 5;
  time_block_sort<bool,5>("bool");
  time_block_sort<bool,10>("bool");
  time_block_sort<bool,25>("bool");

  array_size = original_array_size;
  time_block_sort<char,1>("char");
  time_block_sort<char,3>("char");
  array_size  /= 5;
  time_block_sort<char,5>("char");
  time_block_sort<char,10>("char");
  time_block_sort<char,25>("char");

  array_size = original_array_size;
  time_block_sort<unsigned char,1>("unsigned char");
  time_block_sort<unsigned char,3>("unsigned char");
  array_size /= 5;
  time_block_sort<unsigned char,5>("unsigned char");
  time_block_sort<unsigned char,10>("unsigned char");
  time_block_sort<unsigned char,25>("unsigned char");
  #endif
}
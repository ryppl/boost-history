// put first to detect missing includes, etc.
#include <boost/linear_sort/counting_sort.hpp>

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

template <class Iterator, class T, class Compare>
void assert_is_sorted(Iterator first, Iterator last, Compare cmp)
{
  using namespace std;
  if (! is_sorted(first, last, cmp)) {
    copy( first, first+min(20,last-first), ostream_iterator<T>(cerr," "));
    if (last-first > 20) cerr << " ...\n";
    assert( false );
  }
}

template <class T>
void test_counting_sort_traits(char* type_name)
{
  using namespace std;
  typedef boost::counting_sort_traits<T> traits;

  cout << "VERIFYING COUNTING_SORT_TRAITS FOR TYPE " << type_name << "\n";
  cout << "   const_min  = " << (int)traits::const_min << "\n";
  cout << "   const_max  = " << (int)traits::const_max << "\n";
  cout << "   range_size = " << (int)traits::range_size << "\n";
  /* When there was a way to index signed by shifting them to unsigned
  T x = traits::const_min, y = traits::const_max;
  for (size_t i = 0; i < traits::range_size; y = x, ++i, ++x)
    if (traits::to_index(x) != i)
      cerr << "   ERROR: to_index(" << (int)x << ") = " << traits::to_index(x)
	   << " (should be " << i << ")\n";
  if (y != traits::const_max)
    cerr << "   ERROR: type " << type_name << " did not end with max\n";
  if (x != traits::const_min)
    cerr << "   WARNING: type " << type_name << " does not wrap around\n";
  */
}

template <class T, class Radix>
void test_nth_radix_map(char* type_name, char* radix_name)
{
  
  using namespace std;
  typedef boost::counting_sort_traits<T> traits;

  cout << "VERIFYING NTH_RADIX_MAP FOR TYPE " << type_name
       << " AND RADIX " << radix_name << "\n";

  boost::nth_radix_property_map<T,Radix,
    boost::linear_sort_identity_property_map<T> > radix;
  const unsigned int N = sizeof(T)/sizeof(Radix);
  for (unsigned int r = 0; r < N; ++r) 
    for (unsigned int i = 0; i < 100; ++i) {
      const T x(rand());
      const Radix y = Radix( x >> ((N-1-r)<<(3*sizeof(Radix))));
      if ( radix[x] == y )
	std::cerr << ".";
      else
	std::cerr << "\n  Faulty: " << x << " " << (unsigned)radix[x] << " " << (unsigned)y << "\n";
      assert( radix[x] == y );
    }
}

template <class T>
void time_counting_sort(char* type_name)
{
  using namespace std;
  typedef boost::counting_sort_traits<T> traits;
  typedef typename vector<T>::iterator iterator;

  boost::timer t;
  vector<T> v(array_size), w(array_size, 0);
  for (iterator it = v.begin(); it != v.end(); ++it)
    *it = T(rand() % traits::range_size);
  cout << "TYPE " << type_name << " (with " << array_size << " elements)\n";

  std::copy(v.begin(), v.end(), w.begin());
  t.restart();
  std::stable_sort(w.begin(), w.end());
  cout << "  std::stable_sort                     " << t.elapsed() << "\n";
  assert_is_sorted<iterator,int>(w.begin(), w.end(), std::less<T>());

  std::copy(v.begin(), v.end(), w.begin());
  t.restart();
  std::sort(w.begin(), w.end());
  cout << "  std::sort                            " << t.elapsed() << "\n";
  assert_is_sorted<iterator,int>(w.begin(), w.end(), std::less<T>());

  std::copy(v.begin(), v.end(), w.begin());
  t.restart();
  boost::counting_sort(w.begin(), w.end());
  cout << "  boost::counting_sort                 " << t.elapsed() << "\n";
  assert_is_sorted<iterator,int>(w.begin(), w.end(), std::less<T>());

  t.restart();
  copy(v.begin(), v.end(), w.begin());
  stable_sort(w.begin(), w.end());
  cout << "  std::stable_sort w/ copy             " << t.elapsed() << "\n";
  assert_is_sorted<iterator,int>(w.begin(), w.end(), std::less<T>());

  t.restart();
  copy(v.begin(), v.end(), w.begin());
  sort(w.begin(), w.end());
  cout << "  std::sort w/ copy                    " << t.elapsed() << "\n";
  assert_is_sorted<iterator,int>(w.begin(), w.end(), std::less<T>());

  t.restart();
  boost::counting_sort_copy(v.begin(), v.end(), w.begin());
  cout << "  boost::counting_sort_copy            " << t.elapsed() << "\n";
  assert_is_sorted<iterator,int>(w.begin(), w.end(), std::less<T>());
}

template <class T, std::size_t N>
void time_counting_sort_array(char* type_name )
{
  using namespace std;

  typedef boost::array<T,N> array_t;
  typedef boost::counting_sort_traits<T> traits;
  typedef typename vector<array_t>::iterator iterator;

  boost::timer t;
  vector< array_t > v(array_size);
  for (iterator it = v.begin(); it != v.end(); ++it)
    for (size_t i = 0; i < N; ++i)
      (*it)[i] = T(rand() % traits::range_size);
  cout << "TYPE boost::array<" << type_name << "," << N << ">"
       << " (with " << array_size << " elements, sort on one component)\n";

  typedef boost::nth_element_property_map<array_t,T> project_t;
  project_t project( rand() % N);
  boost::linear_sort_property_map_cmp<project_t> project_cmp(project);

  vector< array_t > w(array_size);
  t.restart();
  copy(v.begin(), v.end(), w.begin());
  stable_sort(w.begin(), w.end(), project_cmp);
  cout << "  std::stable_sort w/ copy and lexico_cmp " << t.elapsed() << "\n";
  assert_is_sorted<iterator,array_t>(w.begin(), w.end(), project_cmp);

  t.restart();
  copy(v.begin(), v.end(), w.begin());
  sort(w.begin(), w.end(), project_cmp);
  cout << "  std::sort w/ copy and lexico_cmp        " << t.elapsed() << "\n";
  assert_is_sorted<iterator,array_t>(w.begin(), w.end(), project_cmp);

  t.restart();
  boost::counting_sort_copy(v.begin(), v.end(), w.begin(), project);
  cout << "  boost::counting_sort_copy               " << t.elapsed() << "\n";
  assert_is_sorted<iterator,array_t>(w.begin(), w.end(), project_cmp);

  copy(v.begin(), v.end(), w.begin());
  t.restart();
  stable_sort(w.begin(), w.end(), project_cmp);
  cout << "  std::stable_sort w/ lexico cmp          " << t.elapsed() << "\n";
  assert_is_sorted<iterator,array_t>(w.begin(), w.end(), project_cmp);

  copy(v.begin(), v.end(), w.begin());
  t.restart();
  sort(w.begin(), w.end(), project_cmp);
  cout << "  std::sort w/ lexico cmp                 " << t.elapsed() << "\n";
  assert_is_sorted<iterator,array_t>(w.begin(), w.end(), project_cmp);

  copy(v.begin(), v.end(), w.begin());
  t.restart();
  boost::counting_sort(w.begin(), w.end(), project);
  cout << "  boost::counting_sort                    " << t.elapsed() << "\n";
  assert_is_sorted<iterator,array_t>(w.begin(), w.end(), project_cmp);
}


int main(char argc, char **argv)
{
  if (argc>1) array_size = atoi(argv[1]);

  test_nth_radix_map<  signed short, char>("  signed short","char");
  test_nth_radix_map<unsigned short, char>("unsigned short","char");
  test_nth_radix_map<  signed int  , char>("  signed int  ","char");
  test_nth_radix_map<unsigned int  , char>("unsigned int  ","char");

  test_nth_radix_map<  signed short, unsigned char>("  signed short","unsigned char");
  test_nth_radix_map<unsigned short, unsigned char>("unsigned short","unsigned char");
  test_nth_radix_map<  signed int  , unsigned char>("  signed int  ","unsigned char");
  test_nth_radix_map<unsigned int  , unsigned char>("unsigned int  ","unsigned char");

  test_nth_radix_map<  signed int, short>("  signed int","short");
  test_nth_radix_map<unsigned int, short>("unsigned int","short");

  test_nth_radix_map<  signed int, unsigned short>("  signed int","unsigned short");
  test_nth_radix_map<unsigned int, unsigned short>("unsigned int","unsigned short");

  test_counting_sort_traits<bool>("bool");
  test_counting_sort_traits<char>("char");
  test_counting_sort_traits<signed char>("signed char");
  test_counting_sort_traits<unsigned char>("unsigned char");
  test_counting_sort_traits<signed short>("signed short");
  test_counting_sort_traits<unsigned short>("unsigned short");

  time_counting_sort<char>("char");
  time_counting_sort<signed char>("signed char");
  time_counting_sort<unsigned char>("unsigned char");

  time_counting_sort_array<char,4>("char");
  time_counting_sort_array<signed char,4>("signed char");
  time_counting_sort_array<unsigned char,4>("unsigned char");

  time_counting_sort_array<char,16>("char");
  time_counting_sort_array<signed char,16>("signed char");
  time_counting_sort_array<unsigned char,16>("unsigned char");

  // time_counting_sort<bool>("bool");
  // time_counting_sort_array<bool,4>("bool");
  // time_counting_sort_array<bool,16>("bool");
}

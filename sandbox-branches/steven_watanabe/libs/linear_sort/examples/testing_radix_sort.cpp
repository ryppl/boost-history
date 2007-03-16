#include <algorithm>
#include <numeric>
#include <iterator>
#include <vector>
#include <iostream>
#include <iomanip>

#include <boost/timer.hpp>
#include <boost/array.hpp>
#include <boost/limits.hpp>
#include <boost/linear_sort/radix_sort.hpp>

#ifndef SIZE
int array_size = 1000000; // one million
#else
int array_size = SIZE;
#endif
std::size_t original_array_size = array_size;

namespace std {

  template <class T, int N>
  std::ostream& operator<< (std::ostream& os, boost::array<T,N> const& t)
  {
    using namespace std;
    os << "(";
    copy( t.begin(), t.end(), ostream_iterator<unsigned>(os," "));
    os << ")";
    return os;
  }

}

template <class Iterator, class T, class Compare>
void assert_is_sorted_cmp(Iterator first, Iterator last, Compare const& cmp)
{
  using namespace std;
  // copy( first, last, ostream_iterator<T>(cerr," "));
  // cerr << "\n";
  if (! is_sorted(first, last, cmp)) {
    cerr << std::setbase(16);
    copy( first, first+min(20,last-first), ostream_iterator<T>(cerr," "));
    if (last-first > 20) cerr << " ...\n";
    assert( false );
  }
}

template <class Iterator, class T>
void assert_is_sorted(Iterator first, Iterator last)
{
  assert_is_sorted_cmp<Iterator,T,std::less<T> >(first, last, std::less<T>());
}

template <class T>
void time_radix_sort_builtin(char* type_name)
{
  using namespace std;

  typedef T builtin_t;
  typedef typename vector<builtin_t>::iterator iterator;

  boost::timer t;
  vector< builtin_t > v(array_size);
  for (iterator it = v.begin(); it != v.end(); ++it) {
    T sign(1);
    if (numeric_limits<builtin_t>::is_signed)
      sign = T((rand()%2) ? T(-1) : T(1));
    *it = sign*T(rand());
  }
  cout << "TYPE " << type_name << " (with " << array_size << " elements)\n";
  vector< builtin_t > w(array_size);

  t.restart();
  copy(v.begin(), v.end(), w.begin());
  stable_sort(w.begin(), w.end());
  cout << "  std::stable_sort w/ copy and lexico_cmp " << t.elapsed() << "\n";
  assert_is_sorted<iterator,builtin_t>(w.begin(), w.end());

  t.restart();
  copy(v.begin(), v.end(), w.begin());
  sort(w.begin(), w.end());
  cout << "  std::sort w/ copy and lexico_cmp        " << t.elapsed() << "\n";
  assert_is_sorted<iterator,builtin_t>(w.begin(), w.end());

  t.restart();
  boost::radix_sort_copy(v.begin(), v.end(), w.begin());
  cout << "  boost::radix_sort_copy                  " << t.elapsed() << "\n";
  assert_is_sorted<iterator,builtin_t>(w.begin(), w.end());

  copy(v.begin(), v.end(), w.begin());
  t.restart();
  stable_sort(w.begin(), w.end());
  cout << "  std::stable_sort w/ lexico cmp          " << t.elapsed() << "\n";
  assert_is_sorted<iterator,builtin_t>(w.begin(), w.end());

  copy(v.begin(), v.end(), w.begin());
  t.restart();
  sort(w.begin(), w.end());
  cout << "  std::sort w/ lexico cmp                 " << t.elapsed() << "\n";
  assert_is_sorted<iterator,builtin_t>(w.begin(), w.end());

  copy(v.begin(), v.end(), w.begin());
  t.restart();
  boost::radix_sort(w.begin(), w.end());
  cout << "  boost::radix_sort                       " << t.elapsed() << "\n";
  assert_is_sorted<iterator,builtin_t>(w.begin(), w.end());
}

template <class T, std::size_t N>
void time_radix_sort_builtin_array(char* type_name, const std::size_t )
{
  using namespace std;

  typedef T array_t[N];
  typedef array_t *array_ptr;
  typedef T (*iterator)[N];
  typedef T (**ptr_iterator)[N];

  array_t* v = new array_t[array_size]; assert(v);
  iterator v_begin = v, v_end = v_begin + array_size;
  array_t* w = new array_t[array_size]; assert(w);
  iterator w_begin = w, w_end = w + array_size;
  array_ptr* u = new array_ptr[array_size]; assert(u);
  ptr_iterator u_begin = u; // , u_end = u + array_size;
  for (iterator it = v_begin; it != v_end; ++it)
    for (size_t i = 0; i < N; ++i)
      (*it)[i] = T(rand());
  cout << "TYPE " << type_name << "[" << N << "]"
       << " (with " << array_size << " elements)\n";

  boost::timer t;
#if 0
  copy(v_begin, v_end, w_begin);
  sort(w_begin, w_end);
  cout << "  std::sort w/ copy and lexico compare " << t.elapsed() << "\n";
  assert_is_sorted<iterator,array_t>(w_begin, w_end);
#endif

  t.restart();
  boost::indirect_radix_sort(v_begin, v_end, u_begin);
  cout << "  boost::indirect_radix_sort           " << t.elapsed() << "\n";
  // assert_is_sorted_cmp<iterator,array_t,
  //   dereference_and_compare< std::less<array_t> > >(u_begin, u_end,
  //   dereference_and_compare< std::less<array_t> >() );
  delete[] u;

  t.restart();
  boost::radix_sort_copy(v_begin, v_end, w_begin);
  cout << "  boost::radix_sort_copy               " << t.elapsed() << "\n";
  // assert_is_sorted_cmp<iterator,array_t>(w_begin, w_end);

#if 0
  copy(v_begin, v_end, w_begin);
  t.restart();
  sort(w_begin, w_end);
  cout << "  std::sort w/ lexico compare          " << t.elapsed() << "\n";
  assert_is_sorted<iterator,array_t>(w_begin, w_end);
#endif

  copy(v_begin, v_end, w_begin);
  t.restart();
  boost::radix_sort(w_begin, w_end);
  cout << "  boost::radix_sort                    " << t.elapsed() << "\n";
  // assert_is_sorted<iterator,array_t>(w_begin, w_end);

  delete[] v;
  delete[] w;
}

template <class T, std::size_t N>
void time_radix_sort_array(char* type_name, const std::size_t )
{
  using namespace std;

  typedef boost::array<T,N> array_t;
  typedef typename vector<array_t>::iterator iterator;

  boost::timer t;
  vector< array_t > v(array_size);
  for (iterator it = v.begin(); it != v.end(); ++it)
    for (size_t i = 0; i < N; ++i)
      (*it)[i] = T(rand());

  cout << "TYPE boost::array<" << type_name << "," << N << ">"
       << " (with " << array_size << " elements)\n";

  vector< array_t > w(array_size);
  t.restart();
  copy(v.begin(), v.end(), w.begin());
  stable_sort(w.begin(), w.end());
  cout << "  std::stable_sort w/ copy and lexico_cmp " << t.elapsed() << "\n";
  assert_is_sorted<iterator,array_t>(w.begin(), w.end());

  t.restart();
  copy(v.begin(), v.end(), w.begin());
  sort(w.begin(), w.end());
  cout << "  std::sort w/ copy and lexico_cmp        " << t.elapsed() << "\n";
  assert_is_sorted<iterator,array_t>(w.begin(), w.end());

  t.restart();
  boost::radix_sort_copy(v.begin(), v.end(), w.begin());
  cout << "  boost::radix_sort_copy                  " << t.elapsed() << "\n";
  assert_is_sorted<iterator,array_t>(w.begin(), w.end());

  copy(v.begin(), v.end(), w.begin());
  t.restart();
  stable_sort(w.begin(), w.end());
  cout << "  std::stable_sort w/ lexico cmp          " << t.elapsed() << "\n";
  assert_is_sorted<iterator,array_t>(w.begin(), w.end());

  copy(v.begin(), v.end(), w.begin());
  t.restart();
  sort(w.begin(), w.end());
  cout << "  std::sort w/ lexico cmp                 " << t.elapsed() << "\n";
  assert_is_sorted<iterator,array_t>(w.begin(), w.end());

  copy(v.begin(), v.end(), w.begin());
  t.restart();
  boost::radix_sort(w.begin(), w.end());
  cout << "  boost::radix_sort                       " << t.elapsed() << "\n";
  assert_is_sorted<iterator,array_t>(w.begin(), w.end());
}

template <class T>
void time_radix_sort(char* type_name)
{
  array_size = original_array_size;
  time_radix_sort_builtin_array<T,1 >(type_name,1);
  time_radix_sort_array<T,1 >(type_name,1);
  time_radix_sort_builtin_array<T,2 >(type_name,2);
  time_radix_sort_array<T,2 >(type_name,2);
  time_radix_sort_builtin_array<T,3 >(type_name,3);
  time_radix_sort_array<T,3 >(type_name,3);
  time_radix_sort_builtin_array<T,4 >(type_name,4);
  time_radix_sort_array<T,4 >(type_name,4);
  array_size  /= 4;            
  time_radix_sort_builtin_array<T,6 >(type_name,6);
  time_radix_sort_array<T,6 >(type_name,6);
  time_radix_sort_builtin_array<T,8 >(type_name,8);
  time_radix_sort_array<T,8 >(type_name,8);
  time_radix_sort_builtin_array<T,16>(type_name,16);
  time_radix_sort_array<T,16>(type_name,16);
  array_size  /= 4;                  
  time_radix_sort_builtin_array<T,24>(type_name,24);
  time_radix_sort_array<T,24>(type_name,24);
}

int main(char argc, char **argv)
{
  if (argc>1) array_size = atoi(argv[1]);
  original_array_size = array_size;

  boost::nth_radix_property_map<unsigned short, unsigned char> radix_map2(0);
  radix_map2.set_radix(1); assert( radix_map2[0xAA11U] == 0x11U);
  radix_map2.set_radix(0); assert( radix_map2[0xAA11U] == 0xAAU); 

  boost::nth_radix_property_map<unsigned int, unsigned char> radix_map4(0);
  radix_map4.set_radix(3); assert( radix_map4[0xFFAA5511U] == 0x11U);
  radix_map4.set_radix(2); assert( radix_map4[0xFFAA5511U] == 0x55U); 
  radix_map4.set_radix(1); assert( radix_map4[0xFFAA5511U] == 0xAAU); 
  radix_map4.set_radix(0); assert( radix_map4[0xFFAA5511U] == 0xFFU); 

#ifdef TIME_UNSIGNED_BUILTIN
  array_size = original_array_size;
  time_radix_sort_builtin<unsigned char>("unsigned char");
  time_radix_sort_builtin<unsigned short>("unsigned short");
  time_radix_sort_builtin<unsigned int>("unsigned int");
#endif
#ifdef TIME_SIGNED_BUILTIN
  time_radix_sort_builtin<signed char>("signed char");
  time_radix_sort_builtin<signed short>("signed short");
  time_radix_sort_builtin<signed int>("signed int");
#endif
#ifdef TIME_FLOAT_BUILTIN
  time_radix_sort_builtin<float>("float");
  time_radix_sort_builtin<double>("double");
  // time_radix_sort_builtin<long double>("long double");
#endif

#ifdef TIME_BOOL_ARRAY
  time_radix_sort<bool>("bool");
#endif
#ifdef TIME_CHAR_ARRAY
  time_radix_sort<char>("char");
#endif
#ifdef TIME_UNSIGNED_CHAR_ARRAY
  time_radix_sort<unsigned char>("unsigned char");
#endif
#ifdef TIME_UNSIGNED_INT_ARRAY
  time_radix_sort<unsigned int>("unsigned int");
#endif
}

#include <algorithm>
#include <functional>
#include <iostream>

#include <boost/array.hpp>

template <class T, std::size_t N, class Compare = std::less<T> >
struct array_cmp {
  typedef T key_type[N];
  // typedef boost::array<T,N> key_type;
  array_cmp(array_cmp const& x) : m_cmp(x.m_cmp), m_n(x.m_n) {}
  explicit array_cmp(std::size_t n = (unsigned)0, Compare const& x = Compare())
    : m_cmp(x), m_n(n) {}
  bool operator() (key_type& x, key_type& y) const
    { return m_cmp(x[m_n], y[m_n]); };
  bool operator() (key_type const& x, key_type const& y) const
    { return m_cmp(x[m_n], y[m_n]); };
protected:
  Compare m_cmp;
  std::size_t m_n;
};

const std::size_t  N = 5;
const unsigned int size = 100;

typedef int elem_t;
typedef int array_t[N];
// typedef boost::array<elem_t,N> array_t;
typedef std::less<int> elem_cmp_t;
int main()
{
  array_t x[size];
  for (unsigned i=0; i<size; ++i)
    for (unsigned j=0; j<N; ++j)
      x[i][j] = std::rand();
  // radix sort
  std::stable_sort(x, x+size, array_cmp< elem_t, N, elem_cmp_t >(2) );
  std::stable_sort(x, x+size, array_cmp< elem_t, N, elem_cmp_t >(1) );
  std::stable_sort(x, x+size, array_cmp< elem_t, N, elem_cmp_t >(0) );
  // output
  return 0;
}

#include <boost/pending/iterator_tests.hpp>
#include <mtl/external_sparse_vector.hpp>

int
main()
{
  typedef std::pair<std::size_t,double> Pair;
#if 0
  Pair array[] = { Pair(0, 3.0), Pair(1, 2.5), Pair(4, 5.0),
		   Pair(7, 0.4) };
#endif
  
  std::size_t index[] = { 0, 1, 4, 7 };
  double value[] = { 3.0, 2.5, 5.0, 0.4 };

  const int nnz = sizeof(index)/sizeof(std::size_t);

  typedef mtl::external_sparse_vector<std::size_t*, double*, 
    column, std::size_t, double>::type Vec;

  Vec v(index, value, nnz, 10);

  Vec::iterator i = v.begin();
  Vec::const_iterator j = v.begin();
  
#if 0
  // not random access anymore (no lavalue return)
  // need to migrate to new iterator categories
  boost::random_access_iterator_test(i, nnz, array);
#endif
  boost::const_nonconst_iterator_test(i, ++j);

  return 0;
}

#if ! defined(BOOST_ALGORITHM_CLUSTER_DETAIL_ABS_DIFF_HPP)
#define BOOST_ALGORITHM_CLUSTER_DETAIL_ABS_DIFF_HPP

namespace boost
{
namespace algorithm
{
namespace cluster 
{
namespace detail
{

template<typename T>
struct abs_diff : std::binary_function<T, T, T>
{
  T operator ()(T const & t1, T const & t2) const
  {
    return (t1 < t2) ? t2 - t1 : t1 - t2;
  }
};

} // End of namespace detail.
} // End of namespace cluster.
} // End of namespace algorithm.
} // End of namespace boost.

#endif

// preprocessed 'count_if_not.hpp' header file,
// see 'boost/mpl/aux_/count_if_not.hpp' for the original,
// including copyright information

#include "boost/type_traits/same_traits.hpp"

namespace boost {
namespace mpl {
namespace aux {

template< typename T, typename U0 = T, typename U1 = T, typename U2 = T, typename U3 = T, typename U4 = T, typename U5 = T, typename U6 = T, typename U7 = T, typename U8 = T, typename U9 = T, typename U10 = T, typename U11 = T, typename U12 = T, typename U13 = T, typename U14 = T >
struct count_if_not
{
    static const long value = !::boost::is_same<T, U0 >::value + !::boost::is_same<T, U1 >::value + !::boost::is_same<T, U2 >::value + !::boost::is_same<T, U3 >::value + !::boost::is_same<T, U4 >::value + !::boost::is_same<T, U5 >::value + !::boost::is_same<T, U6 >::value + !::boost::is_same<T, U7 >::value + !::boost::is_same<T, U8 >::value + !::boost::is_same<T, U9 >::value + !::boost::is_same<T, U10 >::value + !::boost::is_same<T, U11 >::value + !::boost::is_same<T, U12 >::value + !::boost::is_same<T, U13 >::value + !::boost::is_same<T, U14 >::value ;
};

} // namespace aux
} // namespace mpl
} // namespace boost

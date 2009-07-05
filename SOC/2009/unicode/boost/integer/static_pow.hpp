#ifndef BOOST_INTEGER_STATIC_POW_HPP
#define BOOST_INTEGER_STATIC_POW_HPP

#include <boost/mpl/int.hpp>

namespace boost
{
    template<int X, int Y>
    struct static_pow : boost::mpl::int_< X * static_pow<X, Y-1>::value >
    {
    };
    
    template<int X>
    struct static_pow<X, 0> : boost::mpl::int_<1>
    {
    };
}

#endif

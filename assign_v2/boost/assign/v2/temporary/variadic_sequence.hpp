////////////////////////////////////////////////////////////////////////////
//  variadic_sequence.hpp                                                 //
//                                                                        //
//  Copyright (C) 2010 Erwann Rogard                                      //
//  Use, modification and distribution are subject to the                 //
//  Boost Software License, Version 1.0. (See accompanying file           //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)      //
////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_MPL_DETAIL_VARIADIC_SEQUENCE_HPP_ER_2010
#define BOOST_MPL_DETAIL_VARIADIC_SEQUENCE_HPP_ER_2010
#include <boost/preprocessor/repetition/enum.hpp>

namespace boost{
namespace mpl{
namespace detail{
namespace variadic_sequence_aux{

// Source : http://stackoverflow.com/questions/2709315/is-boost-tuple-compatible-with-c0x-variadic-templates

template <std::size_t, typename, typename...> struct nth_arg;

template <std::size_t N, typename Void, typename T, typename... Args>
struct nth_arg<N, Void, T, Args...>
{
    typedef typename nth_arg<N - 1, Void, Args...>::type type;
};

template <typename Void, typename T, typename... Args>
struct nth_arg<0, Void, T, Args...>
{
    typedef T type;
};

template <std::size_t N, typename Void>
struct nth_arg<N, Void>
{
    typedef Void type;
};

}// variadic_sequence_aux
}// detail
}// mpl
}// boost

#define BOOST_MPL_DETAIL_VARIADIC_SEQUENCE_arg(z, n, data) \
    typename ::boost::mpl::detail::variadic_sequence_aux::nth_arg< \
        n, data, Args...\
    >::type \
/**/

#define BOOST_MPL_DETAIL_VARIADIC_SEQUENCE(N, FROM, TO, default_type) \
template <typename ...Args> \
struct TO \
{ \
    typedef FROM< \
        BOOST_PP_ENUM( \
            N, \
            BOOST_MPL_DETAIL_VARIADIC_SEQUENCE_arg, \
            default_type \
        ) \
    > type; \
}; \
/**/

#endif

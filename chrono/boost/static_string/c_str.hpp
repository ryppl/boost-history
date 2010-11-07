//  c_str.hpp  --------------------------------------------------------------//

//  Copyright 2010 Vicente J. Botet Escriba

//  Distributed under the Boost Software License, Version 1.0.
//  See http://www.boost.org/LICENSE_1_0.txt


#ifndef BOOST_STATIC_STRING_C_STR_HPP
#define BOOST_STATIC_STRING_C_STR_HPP

#include <boost/static_string/value_type.hpp>
#include <boost/mpl/next.hpp>
#include <boost/mpl/deref.hpp>
#include <boost/mpl/integral_c.hpp>
#include <boost/mpl/end.hpp>
#include <boost/mpl/begin.hpp>
#include  <boost/preprocessor/cat.hpp>
#include  <boost/preprocessor/inc.hpp>

#if !defined(BOOST_STATIC_STRING_LIMIT_C_STR_SIZE)
#   define BOOST_STATIC_STRING_LIMIT_C_STR_SIZE 32
#endif

namespace boost {
namespace static_string {
namespace detail 
{
    template<typename It, typename End>
    struct next_unless
      : mpl::next<It>
    {};

    template<typename End>
    struct next_unless<End, End>
    {
        typedef End type;
    };

    template<typename T, typename It, typename End>
    struct deref_unless
      : mpl::deref<It>
    {};

    template<typename T, typename End>
    struct deref_unless<T, End, End>
    {
        typedef mpl::integral_c<T, 0> type;
    };
}

template<typename Sequence>
struct c_str
{
//    typedef typename value_type<Sequence>::type value_type;
    typedef typename mpl::end<Sequence>::type iend;
    typedef typename mpl::begin<Sequence>::type i0;
    #define M0(z, n, data)                                                                      \
    typedef                                                                                     \
        typename static_string::detail::next_unless<BOOST_PP_CAT(i, n), iend>::type                         \
    BOOST_PP_CAT(i, BOOST_PP_INC(n));
    BOOST_PP_REPEAT(BOOST_MPL_LIMIT_STRING_SIZE, M0, ~)
    #undef M0

    typedef c_str type;
    static typename value_type<Sequence>::type const value[BOOST_STATIC_STRING_LIMIT_C_STR_SIZE+1];
};

template<typename Sequence>
typename value_type<Sequence>::type const c_str<Sequence>::value[BOOST_STATIC_STRING_LIMIT_C_STR_SIZE+1] =
{
    #define M0(z, n, data)                                                                      \
    static_string::detail::deref_unless<typename value_type<Sequence>::type, BOOST_PP_CAT(i, n), iend>::type::value,
    BOOST_PP_REPEAT(BOOST_STATIC_STRING_LIMIT_C_STR_SIZE, M0, ~)
    #undef M0
    mpl::integral_c<typename value_type<Sequence>::type, 0>::type::value
//    '\0'
};

} // namespace static_string
} // namespace boost

#endif // BOOST_STATIC_STRING_C_STR_HPP

//-----------------------------------------------------------------------------
// boost mpl/list/factory.hpp header file
// See http://www.boost.org for updates, documentation, and revision history.
//-----------------------------------------------------------------------------
//
// Copyright (c) 2000-02
// Aleksey Gurtovoy
//
// Permission to use, copy, modify, distribute and sell this software
// and its documentation for any purpose is hereby granted without fee, 
// provided that the above copyright notice appears in all copies and 
// that both the copyright notice and this permission notice appear in 
// supporting documentation. No representations are made about the 
// suitability of this software for any purpose. It is provided "as is" 
// without express or implied warranty.

#ifndef BOOST_MPL_LIST_FACTORY_HPP_ICNLUDED
#define BOOST_MPL_LIST_FACTORY_HPP_ICNLUDED

// EDG-based compilers have serious problems with preprocessor performance,
// so we have to feed them already preprocessed version of code
#if defined(__EDG__) && (__EDG_VERSION__ <= 245)
#   include "boost/mpl/aux_/preprocessed/factory.hpp"
#else

#include "boost/mpl/list/preprocessor.hpp"
#include "boost/mpl/list/traits.hpp"
#include "boost/mpl/aux_/count_if_not.hpp"
#include "boost/mpl/aux_/preprocessor.hpp"
#include "boost/preprocessor/repeat_2nd.hpp"
#include "boost/preprocessor/inc.hpp"
#include "boost/preprocessor/dec.hpp"

namespace boost {
namespace mpl {

#define BOOST_MPL_AUX_REPEAT_BODY(n, expr) expr
#define BOOST_MPL_AUX_REPEAT(n, expr) \
    BOOST_PREPROCESSOR_REPEAT(n, BOOST_MPL_AUX_REPEAT_BODY, expr) \
/**/

namespace aux {

template<long N>
struct list_factory_part1
{
    template< typename Tag, BOOST_MPL_LIST_PARAMETERS(typename T) >
    struct part2
    { 
        typedef typename mpl::list_traits<Tag>::null_node type;
    };
};

#define BOOST_MPL_AUX_LIST_FACTORY_SPECIALIZATION(N) \
template<> \
struct list_factory_part1<N> \
{ \
    template< typename Tag, BOOST_MPL_LIST_PARAMETERS(typename T) > \
    struct part2 \
    { \
        typedef BOOST_MPL_TEMPLATE_PARAMETERS(0, N \
            , typename mpl::list_traits<Tag>::template make_node<T) \
            , typename mpl::list_traits<Tag>::null_node BOOST_MPL_AUX_REPEAT(N \
            , >::type) type; \
    }; \
}; \
/**/

#define BOOST_MPL_AUX_LIST_FACTORY_SPEC(i, unused) \
	  BOOST_MPL_AUX_LIST_FACTORY_SPECIALIZATION(BOOST_PREPROCESSOR_INC(i)) \
/**/

BOOST_PREPROCESSOR_REPEAT_2ND(
      BOOST_PREPROCESSOR_DEC(BOOST_MPL_LIST_PARAMETERS_NUMBER)
    , BOOST_MPL_AUX_LIST_FACTORY_SPEC
    , unused
    )

} // namespace aux


template<typename Tag, BOOST_MPL_LIST_PARAMETERS(typename T)>
struct list_factory
{
    typedef typename aux::list_factory_part1<
        aux::count_if_not<
              typename mpl::list_traits<Tag>::null_argument
            , BOOST_MPL_LIST_PARAMETERS(T)
            >::value
        >::template part2<Tag, BOOST_MPL_LIST_PARAMETERS(T)>::type type;
};

#undef BOOST_MPL_AUX_LIST_FACTORY_SPEC
#undef BOOST_MPL_AUX_REPEAT_BODY
#undef BOOST_MPL_AUX_REPEAT

} // namespace mpl
} // namespace boost 

#endif // defined(__EDG__)

#endif // BOOST_MPL_LIST_FACTORY_HPP_ICNLUDED

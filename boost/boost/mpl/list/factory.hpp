//-----------------------------------------------------------------------------
// boost mpl/list/factory.hpp header file
// See http://www.boost.org for updates, documentation, and revision history.
//-----------------------------------------------------------------------------
//
// Copyright (c) 2000-01
// Aleksey Gurtovoy
//
// Permission to use, copy, modify, distribute and sell this software
// and its documentation for any purpose is hereby granted without fee, 
// provided that the above copyright notice appears in all copies and 
// that both the copyright notice and this permission notice appear in 
// supporting documentation. No representations are made about the 
// suitability of this software for any purpose. It is provided "as is" 
// without express or implied warranty.

#ifndef BOOST_MPL_LIST_FACTORY_HPP
#define BOOST_MPL_LIST_FACTORY_HPP

#include "boost/mpl/list/traits.hpp"
#include "boost/mpl/preprocessor/enumerate_params.hpp"
#include "boost/mpl/preprocessor/enumerate_n.hpp"
#include "boost/mpl/preprocessor/repeat_n.hpp"
#include "boost/mpl/preprocessor/arithmetic.hpp"
#include "boost/mpl/preprocessor/config.hpp"
#include "boost/type_traits/same_traits.hpp"
#include "boost/preprocessor/2nd_repeat.hpp"
#include "boost/preprocessor/empty.hpp"
#include "boost/preprocessor/unused.hpp"
#include "boost/preprocessor/inc.hpp"
#include "boost/preprocessor/dec.hpp"

namespace boost {
namespace mpl {

namespace detail {

template<long N>
struct list_factory_part1
{
    template<typename Tag, BOOST_MPL_ENUMERATE_PARAMS(typename T)>
    struct part2
    { 
        typedef typename mpl::list_traits<Tag>::null_node type;
    };
};

#define BOOST_MPL_LIST_FACTORY_SPECIALIZATION(N)                              \
template<>                                                                    \
struct list_factory_part1<N>                                                  \
{                                                                             \
    template<typename Tag, BOOST_MPL_ENUMERATE_PARAMS(typename T)>            \
    struct part2                                                              \
    {                                                                         \
        typedef BOOST_MPL_ENUMERATE_N(N                                       \
            , typename mpl::list_traits<Tag>::template make_node<T)           \
            , typename mpl::list_traits<Tag>::null_node BOOST_MPL_REPEAT_N(N  \
            , >::type) type;                                                  \
    };                                                                        \
};                                                                            \
/**/

#define BOOST_MPL_LIST_FACTORY_SPEC(i, unused1, unused2)                      \
	  BOOST_MPL_LIST_FACTORY_SPECIALIZATION(BOOST_PREPROCESSOR_INC(i))        \
/**/

BOOST_PREPROCESSOR_2ND_REPEAT(
      BOOST_PREPROCESSOR_DEC(BOOST_MPL_PARAMETERS_NUMBER)
    , BOOST_MPL_LIST_FACTORY_SPEC
    , unused
    , unused
    )

template<typename Tag, typename T>
struct is_list_argument
{
    typedef mpl::list_traits<Tag> traits;
    BOOST_STATIC_CONSTANT(long, value = 
        !(::boost::is_same<typename traits::null_argument, T>::value)
        );
};

#define BOOST_MPL_IS_LIST_ARGUMENT(i, Tag, T)                                 \
    BOOST_PREPROCESSOR_IF(i, BOOST_MPL_PLUS, BOOST_PREPROCESSOR_EMPTY)()      \
    ::boost::mpl::detail::is_list_argument<Tag, T##i>::value                  \
/**/

#define BOOST_MPL_LIST_ARGUMENTS_NUMBER(Tag, T)                               \
    BOOST_PREPROCESSOR_REPEAT(                                                \
        BOOST_MPL_PARAMETERS_NUMBER                                           \
      , BOOST_MPL_IS_LIST_ARGUMENT                                            \
      , Tag                                                                   \
      , T                                                                     \
      )                                                                       \
/**/

} // namespace detail


template<typename Tag, BOOST_MPL_ENUMERATE_PARAMS(typename T)>
struct list_factory
{
    typedef typename mpl::detail::list_factory_part1<
                BOOST_MPL_LIST_ARGUMENTS_NUMBER(Tag, T)
                >::template part2< Tag
                                  , BOOST_MPL_ENUMERATE_PARAMS(T)
                                  >::type type;
};

#undef BOOST_MPL_IS_LIST_ARGUMENT
#undef BOOST_MPL_LIST_ARGUMENTS_NUMBER
#undef BOOST_MPL_LIST_FACTORY_SPEC

} // namespace mpl
} // namespace boost 

#endif // BOOST_MPL_LIST_FACTORY_HPP

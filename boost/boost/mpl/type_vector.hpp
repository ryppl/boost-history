//-----------------------------------------------------------------------------
// boost mpl/type_vector.hpp header file
// See http://www.boost.org for updates, documentation, and revision history.
//-----------------------------------------------------------------------------
//
// Copyright (c) 2001
// Aleksey Gurtovoy
//
// Permission to use, copy, modify, distribute and sell this software
// and its documentation for any purpose is hereby granted without fee, 
// provided that the above copyright notice appears in all copies and 
// that both the copyright notice and this permission notice appear in 
// supporting documentation. No representations are made about the 
// suitability of this software for any purpose. It is provided "as is" 
// without express or implied warranty.

#ifndef BOOST_MPL_TYPE_VECTOR_HPP
#define BOOST_MPL_TYPE_VECTOR_HPP

#include "boost/config.hpp"
#include "boost/mpl/type_vector/type_vector_fwd.hpp"
#include "boost/mpl/type_vector/vector_element.hpp"
#include "boost/mpl/type_vector/algorithm.hpp"
#include "boost/mpl/nulls.hpp"
#include "boost/mpl/preprocessor/config.hpp"
#include "boost/mpl/preprocessor/arithmetic.hpp"
#include "boost/type_traits/same_traits.hpp"
#include "boost/preprocessor/repeat.hpp"
#include "boost/preprocessor/if.hpp"
#include "boost/preprocessor/empty.hpp"

namespace boost {
namespace mpl {

// local macros, undefined at the end of the header
#define BOOST_MPL_TYPE_VECTOR_ELEMENT_TYPEDEF(i, T, unused)                   \
    typedef T##i value_type_##i;                                              \
/**/

#define BOOST_MPL_IS_TYPE_VECTOR_ARGUMENT(i, T, unused)                       \
    BOOST_PREPROCESSOR_IF(i, BOOST_MPL_PLUS, BOOST_PREPROCESSOR_EMPTY)()      \
    !::boost::is_same<T##i, mpl::null_argument>::value                        \
/**/

#define BOOST_MPL_TYPE_VECTOR_SIZE(T)                                         \
    BOOST_PREPROCESSOR_REPEAT(                                                \
        BOOST_MPL_PARAMS_LIST_SIZE                                            \
      , BOOST_MPL_IS_TYPE_VECTOR_ARGUMENT                                     \
      , T                                                                     \
      , unused                                                                \
      )                                                                       \
/**/

// type_vector itself

template<BOOST_MPL_ENUMERATE_PARAMS(typename T)>
struct type_vector
{
 private:
    typedef type_vector self;

 public:

    // the result of the macro expansion (N == BOOST_MPL_PARAMS_LIST_SIZE):
    //    typedef T0 value_type_0;
    //    typedef T1 value_type_1;
    //    ...
    //    typedef TN value_type_N;
    BOOST_PREPROCESSOR_REPEAT(
          BOOST_MPL_PARAMS_LIST_SIZE
        , BOOST_MPL_TYPE_VECTOR_ELEMENT_TYPEDEF
        , T
        , unused
        )

    BOOST_STATIC_CONSTANT(long, size = BOOST_MPL_TYPE_VECTOR_SIZE(T));

    typedef type_vector_sequence_tag sequence_category;

    typedef typename mpl::detail::vector_element<self, 0>::iterator begin;
    typedef typename mpl::detail::vector_element<self, size>::iterator end;
};

#undef BOOST_MPL_TYPE_VECTOR_SIZE
#undef BOOST_MPL_IS_TYPE_VECTOR_ARGUMENT
#undef BOOST_MPL_PLUS
#undef BOOST_MPL_TYPE_VECTOR_ELEMENT_TYPEDEF

} // namespace mpl
} // namespace boost 

#endif // BOOST_MPL_TYPE_VECTOR_HPP

//-----------------------------------------------------------------------------
// boost mpl/type_vector/insert.hpp header file
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

#ifndef BOOST_MPL_TYPE_VECTOR_INSERT_HPP
#define BOOST_MPL_TYPE_VECTOR_INSERT_HPP

#include "boost/mpl/limits/vector.hpp"
#include "boost/mpl/type_vector/type_vector_fwd.hpp"
#include "boost/mpl/insert.hpp"

#include "boost/preprocessor/arithmetic/sub.hpp"
#include "boost/preprocessor/dec.hpp"
#include "boost/preprocessor/repeat_2nd.hpp"
#include "boost/preprocessor/comma_if.hpp"

namespace boost {
namespace mpl {

namespace detail {

template<long N, typename Vector, typename T>
struct type_vector_insert_algorithm;

#define BOOST_MPL_AUX_LAST_VECTOR_PARAMETER \
    BOOST_PREPROCESSOR_DEC(BOOST_MPL_VECTOR_MAX_SIZE) \
/**/

#define BOOST_MPL_AUX_VECTOR_INSERT_ALGORITHM(i, unused) \
    template<typename Vector, typename T> \
    struct type_vector_insert_algorithm<i, Vector, T> \
    { \
        typedef type_vector< \
            BOOST_MPL_TEMPLATE_PARAMETERS( \
                  0 \
                , i \
                , typename Vector::value_type_ \
                ) \
            BOOST_PREPROCESSOR_COMMA_IF(i) T \
            BOOST_PREPROCESSOR_COMMA_IF( \
                BOOST_PREPROCESSOR_SUB(BOOST_MPL_AUX_LAST_VECTOR_PARAMETER, i) \
                ) \
                BOOST_MPL_TEMPLATE_PARAMETERS( \
                      i \
                    , BOOST_MPL_AUX_LAST_VECTOR_PARAMETER \
                    , typename Vector::value_type_ \
                    ) \
            > sequence; \
    }; \
/**/


BOOST_PREPROCESSOR_REPEAT_2ND(
      BOOST_PREPROCESSOR_DEC(BOOST_MPL_AUX_LAST_VECTOR_PARAMETER)
    , BOOST_MPL_AUX_VECTOR_INSERT_ALGORITHM
    , unused
    )

#undef BOOST_MPL_AUX_VECTOR_INSERT_ALGORITHM
#undef BOOST_MPL_AUX_LAST_VECTOR_PARAMETER

} // namespace detail

template<>
struct insert_algorithm_traits<mpl::type_vector_sequence_tag>
{
    template<typename Vector, typename Iterator, typename T>
    struct algorithm
        : mpl::detail::type_vector_insert_algorithm<
                Iterator::index
              , Vector
              , T
              >
    {
    };
};

} // namespace mpl
} // namespace boost 

#endif // BOOST_MPL_TYPE_VECTOR_INSERT_HPP

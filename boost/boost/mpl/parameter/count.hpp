//-----------------------------------------------------------------------------
// boost mpl/parameter/count.hpp header file
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

#ifndef BOOST_MPL_PARAMETER_COUNT_HPP_INCLUDED
#define BOOST_MPL_PARAMETER_COUNT_HPP_INCLUDED

#include "boost/mpl/preprocessor/default_template_params.hpp"
#include "boost/mpl/preprocessor/tokens.hpp"
#include "boost/mpl/preprocessor/config.hpp"
#include "boost/preprocessor/empty.hpp"
#include "boost/type_traits/same_traits.hpp"

namespace boost {
namespace mpl {
namespace parameter {

namespace aux {
struct not_a_param;
}

#define BOOST_MPL_AUX_PARAMETER_COUNT(i, param) \
    BOOST_PREPROCESSOR_IF(i \
        , BOOST_MPL_PREPROCESSOR_PLUS_TOKEN \
        , BOOST_PREPROCESSOR_EMPTY \
        )() \
    !::boost::is_same<T, param##i>::value \
/**/

template< 
      typename T
    , BOOST_MPL_DEFAULT_TEMPLATE_PARAMS(typename U, T)
    >
struct count_if_not
{
    BOOST_STATIC_CONSTANT(long, value = BOOST_PREPROCESSOR_REPEAT(
          BOOST_MPL_PARAMETERS_NUMBER
        , BOOST_MPL_AUX_PARAMETER_COUNT
        , U
        ));
};

#undef BOOST_MPL_AUX_PARAMETER_COUNT

} // namespace parameter
} // namespace mpl
} // namespace boost

#endif // BOOST_MPL_PARAMETER_COUNT_HPP_INCLUDED

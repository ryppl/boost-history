// Copyright Aleksey Gurtovoy 2001-2002
//
// Use, modification and distribution are subject to the Boost Software 
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy 
// at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/mpl for documentation.

// $Source$
// $Date$
// $Revision$

#if !defined(BOOST_PP_IS_ITERATING)

///// header body

#ifndef BOOST_MPL_LAMBDA_HELPER_HPP_INCLUDED
#define BOOST_MPL_LAMBDA_HELPER_HPP_INCLUDED

#include <boost/config.hpp>
#include <boost/mpl/aux_/config/use_preprocessed.hpp>

#if !defined(BOOST_MPL_CFG_NO_PREPROCESSED_HEADERS) && \
    !defined(BOOST_MPL_PREPROCESSING_MODE)

#   define BOOST_MPL_PREPROCESSED_HEADER lambda_helper.hpp
#   include <boost/mpl/aux_/include_preprocessed.hpp>

#else

#   include <boost/mpl/limits/arity.hpp>
#   include <boost/mpl/aux_/preprocessor/params.hpp>
#   include <boost/mpl/aux_/preprocessor/repeat.hpp>

#   include <boost/preprocessor/iterate.hpp>
#   include <boost/preprocessor/inc.hpp>
#   include <boost/preprocessor/cat.hpp>

namespace boost {
namespace mpl {

#define BOOST_PP_ITERATION_PARAMS_1 \
    (3, (1,BOOST_MPL_LIMIT_METAFUNCTION_ARITY, <boost/mpl/lambda_helper.hpp>))
#include BOOST_PP_ITERATE()

} // namespace mpl
} // namespace boost

#endif // BOOST_MPL_CFG_NO_PREPROCESSED_HEADERS
#endif // BOOST_MPL_LAMBDA_HELPER_HPP_INCLUDED

///// iteration

#else
#define i BOOST_PP_FRAME_ITERATION(1)

#define MPL_AUX_ARG_TYPEDEF(unused, i, T) \
    typedef BOOST_PP_CAT(T, BOOST_PP_INC(i)) \
        BOOST_PP_CAT(arg, BOOST_PP_INC(i)); \
/**/

template<
      template< BOOST_MPL_PP_PARAMS(i, typename P) > class F
    , BOOST_MPL_PP_PARAMS(i, typename T)
    >
struct BOOST_PP_CAT(lambda_helper,i)
{
    struct rebind
    {
        BOOST_STATIC_CONSTANT(int, arity = i);
        BOOST_MPL_PP_REPEAT(i, MPL_AUX_ARG_TYPEDEF, T)
        template< BOOST_MPL_PP_PARAMS(i, typename U) > struct apply
            : F<BOOST_MPL_PP_PARAMS(i,U)>
        {
        };
   };
};

#undef MPL_AUX_ARG_TYPEDEF

#undef i
#endif // BOOST_PP_IS_ITERATING

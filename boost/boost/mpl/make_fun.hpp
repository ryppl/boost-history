//-----------------------------------------------------------------------------
// boost mpl/make_fun.hpp header file
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

#ifndef BOOST_MPL_MAKE_FUN_HPP_INCLUDED
#define BOOST_MPL_MAKE_FUN_HPP_INCLUDED

#include "boost/mpl/limits/arity.hpp"
#include "boost/mpl/aux_/preprocessor.hpp"
#include "boost/mpl/mpl_config.hpp"

#include "boost/preprocessor/repeat_2nd.hpp"
#include "boost/preprocessor/inc.hpp"

namespace boost {
namespace mpl {

#if !defined(BOOST_NO_TEMPLATE_TEMPLATE_PARAMETERS)

#define BOOST_MPL_AUX_MAKE_FUN_PARAMS(n, param) \
    BOOST_MPL_TEMPLATE_PARAMETERS(1, BOOST_PREPROCESSOR_INC(n), param) \
/**/


#define BOOST_MPL_AUX_MAKE_F0_TEMPLATE_DEF(n)
#define BOOST_MPL_AUX_MAKE_FN_TEMPLATE_DEF(n) \
template< \
      template< BOOST_MPL_AUX_MAKE_FUN_PARAMS(n, typename P) > class Function \
    > \
struct BOOST_PREPROCESSOR_CAT(make_f,n) \
{ \
    typedef BOOST_PREPROCESSOR_CAT(make_f,n) type; \
    template<BOOST_MPL_AUX_MAKE_FUN_PARAMS(n, typename U)> struct apply \
    { \
        typedef typename Function< \
              BOOST_MPL_AUX_MAKE_FUN_PARAMS(n, U) \
            >::type type; \
    }; \
}; \
/**/

#define BOOST_MPL_AUX_MAKE_FN_DEF(n, unused) \
    BOOST_PREPROCESSOR_IF( \
        n \
      , BOOST_MPL_AUX_MAKE_FN_TEMPLATE_DEF \
      , BOOST_MPL_AUX_MAKE_F0_TEMPLATE_DEF \
      )(n) \
/**/
      
// make_f#
BOOST_PREPROCESSOR_REPEAT_2ND(
      BOOST_PREPROCESSOR_INC(BOOST_MPL_FUNCTION_CLASS_MAX_ARITY)
    , BOOST_MPL_AUX_MAKE_FN_DEF
    , unused
    )

#undef BOOST_MPL_AUX_MAKE_FN_DEF
#undef BOOST_MPL_AUX_MAKE_FN_TEMPLATE_DEF
#undef BOOST_MPL_AUX_MAKE_F0_TEMPLATE_DEF
#undef BOOST_MPL_AUX_MAKE_FUN_PARAMS

// for backward compatibility
template< template<typename> class UnaryFunction >
struct make_f_x
{
    template<typename U>
    struct apply
        : UnaryFunction<U>
    {
    };
};

template< template<typename, typename> class BinaryFunction >
struct make_f_xy
{
    template<typename U1, typename U2>
    struct apply
        : BinaryFunction<U1,U2>
    {
    };
};

template< template<typename, typename> class BinaryFunction >
struct make_f_yx
{
    template<typename U1, typename U2>
    struct apply
        : BinaryFunction<U2,U1>
    {
    };
};


#   define BOOST_MPL_MAKE_F_X(name, f) typedef boost::mpl::make_f_x<f> name
#   define BOOST_MPL_MAKE_F_XY(name, f) typedef boost::mpl::make_f_xy<f> name
#   define BOOST_MPL_MAKE_F_YX(name, f) typedef boost::mpl::make_f_yx<f> name

#else

#   define BOOST_MPL_MAKE_F_X(name, f) \
    struct name { template<typename U> struct apply : f<U> {}; } \
/**/

#   define BOOST_MPL_MAKE_F_XY(name, f) struct name \
    { template<typename U1, typename U2> struct apply : f<U1,U2> {}; } \
/**/

#   define BOOST_MPL_MAKE_F_YX(name, f) struct name \
    { template<typename U1, typename U2> struct apply : f<U2,U1> {}; } \
/**/

#endif // BOOST_NO_TEMPLATE_TEMPLATE_PARAMETERS

} // namespace mpl
} // namespace boost 

#endif // BOOST_MPL_MAKE_FUN_HPP_INCLUDED

//-----------------------------------------------------------------------------
// boost mpl/apply.hpp header file
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

#ifndef BOOST_MPL_APPLY_HPP
#define BOOST_MPL_APPLY_HPP

#include "boost/mpl/limits/apply.hpp"
#include "boost/mpl/aux_/none.hpp"
#include "boost/mpl/aux_/count_if_not.hpp"
#include "boost/mpl/aux_/preprocessor.hpp"

#include "boost/preprocessor/repeat_2nd.hpp"
#include "boost/preprocessor/comma_if.hpp"
#include "boost/preprocessor/if.hpp"
#include "boost/config.hpp"

namespace boost {
namespace mpl {

#define BOOST_MPL_AUX_APPLY_N_PARAMS(n, param, value) \
    BOOST_MPL_TEMPLATE_PARAMETERS(0, n, param) \
    BOOST_PREPROCESSOR_COMMA_IF(n) \
    BOOST_MPL_DEFAULT_TEMPLATE_PARAMETERS( \
          n \
        , BOOST_MPL_APPLY_PARAMETERS_NUMBER \
        , param \
        , value \
        ) \
/**/

#define BOOST_MPL_AUX_APPLY_DEFAULT_PARAMS(param, value) \
    BOOST_MPL_DEFAULT_TEMPLATE_PARAMETERS( \
          0 \
        , BOOST_MPL_APPLY_PARAMETERS_NUMBER \
        , param \
        , value \
        ) \
/**/

#define BOOST_MPL_AUX_APPLY_PARAMS(param) \
    BOOST_MPL_TEMPLATE_PARAMETERS( \
          0 \
        , BOOST_MPL_APPLY_PARAMETERS_NUMBER \
        , param \
        ) \
/**/

#define BOOST_MPL_AUX_APPLY_0_TEMPLATE_DEF(unused) \
template< typename F, BOOST_MPL_AUX_APPLY_N_PARAMS(0, typename T, aux::none) > \
struct apply_0 \
{ \
 private: \
    typedef char arity_constraint[sizeof(aux::reject_if_not_none<T0>)]; \
 public: \
    typedef typename F::apply::type type; \
}; \
/**/

#if defined(BOOST_MSVC) && (BOOST_MSVC <= 1200)

namespace aux {
template<typename T>
struct msvc_never_true
{
    BOOST_STATIC_CONSTANT(bool, value = false);
};
} // namespace aux

// warning: not a legal C++; workaround for MSVC's dependent template typedef bug 
#define BOOST_MPL_AUX_APPLY_N_TEMPLATE_DEF(n) \
template< typename F, BOOST_MPL_AUX_APPLY_N_PARAMS(n, typename T, aux::none) > \
struct apply_##n \
{ \
 private: \
    typedef char arity_constraint[sizeof(aux::reject_if_not_none<T##n>)]; \
    template<bool> struct F##_wrapper : F {}; \
    template<> struct F##_wrapper<true> \
    { template<BOOST_MPL_TEMPLATE_PARAMETERS(0, n, typename P)> struct apply; }; \
 public: \
    typedef typename F##_wrapper< aux::msvc_never_true<F>::value > \
        ::template apply<BOOST_MPL_TEMPLATE_PARAMETERS(0, n, T)>::type type; \
}; \
/**/

#else

#define BOOST_MPL_AUX_APPLY_N_TEMPLATE_DEF(n) \
template< typename F, BOOST_MPL_AUX_APPLY_N_PARAMS(n, typename T, aux::none) > \
struct apply_##n \
{ \
 private: \
    typedef char arity_constraint[sizeof(aux::reject_if_not_none<T##n>)]; \
 public: \
    typedef typename F \
        ::template apply<BOOST_MPL_TEMPLATE_PARAMETERS(0, n, T)>::type type; \
}; \
/**/

#endif // #if defined(BOOST_MSVC) && (BOOST_MSVC <= 1200)

namespace aux {
template<long N>
struct apply_impl
{
    template< typename F, BOOST_MPL_AUX_APPLY_PARAMS(typename T) >
    struct result;
};
} // namespace aux

#define BOOST_MPL_AUX_APPLY_IMPL_TEMPLATE_DEF(n) \
namespace aux { \
template<> \
struct apply_impl<n> \
{ \
    template< typename F, BOOST_MPL_AUX_APPLY_PARAMS(typename T) > \
    struct result \
    { \
        typedef typename mpl::apply_##n< \
              F \
            , BOOST_MPL_AUX_APPLY_PARAMS(T) \
            >::type type; \
    }; \
}; \
} \
/**/

#define BOOST_MPL_AUX_APPLY_TEMPLATE_DEF(n, unused) \
    BOOST_PREPROCESSOR_IF( \
          n \
        , BOOST_MPL_AUX_APPLY_N_TEMPLATE_DEF \
        , BOOST_MPL_AUX_APPLY_0_TEMPLATE_DEF \
        )(n) \
    BOOST_MPL_AUX_APPLY_IMPL_TEMPLATE_DEF(n) \
/**/


// apply_# 
BOOST_PREPROCESSOR_REPEAT_2ND(
      BOOST_MPL_APPLY_PARAMETERS_NUMBER
    , BOOST_MPL_AUX_APPLY_TEMPLATE_DEF
    , unused
    )

#if !defined(BOOST_MSVC) || (BOOST_MSVC > 1200)
// using 'apply' name for a top-level template gives an ICE on MSVC
template< typename F, BOOST_MPL_AUX_APPLY_DEFAULT_PARAMS(typename T, aux::none) >
struct apply
{
 private:
    typedef aux::count_if_not<
          aux::none
        , BOOST_MPL_AUX_APPLY_PARAMS(T)
        > arity_;

 public:
    typedef typename aux::apply_impl< arity_::value >
        ::template result< F, BOOST_MPL_AUX_APPLY_PARAMS(T) >::type type;
};

#define BOOST_MPL_HAS_APPLY

#endif // !defined(BOOST_MSVC) || (BOOST_MSVC > 1200)

#undef BOOST_MPL_AUX_APPLY_TEMPLATE_DEF
#undef BOOST_MPL_AUX_APPLY_N_TEMPLATE_DEF
#undef BOOST_MPL_AUX_APPLY_0_TEMPLATE_DEF
#undef BOOST_MPL_AUX_APPLY_PARAMS
#undef BOOST_MPL_AUX_APPLY_DEFAULT_PARAMS
#undef BOOST_MPL_AUX_APPLY_N_PARAMS

} // namespace mpl
} // namespace boost 

#endif // BOOST_MPL_APPLY_HPP

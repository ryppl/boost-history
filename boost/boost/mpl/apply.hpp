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

#ifndef BOOST_MPL_APPLY_HPP_INCLUDED
#define BOOST_MPL_APPLY_HPP_INCLUDED

#include "boost/mpl/limits/arity.hpp"
#include "boost/mpl/aux_/count_if_not.hpp"
#include "boost/mpl/aux_/preprocessor.hpp"
#include "boost/mpl/aux_/none.hpp"

#include "boost/preprocessor/repeat_2nd.hpp"
#include "boost/preprocessor/if.hpp"
#include "boost/preprocessor/comma.hpp"
#include "boost/preprocessor/empty.hpp"
#include "boost/preprocessor/inc.hpp"
#include "boost/config.hpp"

namespace boost {
namespace mpl {

#define BOOST_MPL_AUX_APPLY_PARAMS(param) \
    BOOST_MPL_TEMPLATE_PARAMETERS( \
          1 \
        , BOOST_PREPROCESSOR_INC(BOOST_MPL_FUNCTION_CLASS_MAX_ARITY) \
        , param \
        ) \
/**/

#define BOOST_MPL_AUX_APPLY_DEFAULT_PARAMS(param, value) \
    BOOST_MPL_DEFAULT_TEMPLATE_PARAMETERS( \
          1 \
        , BOOST_PREPROCESSOR_INC(BOOST_MPL_FUNCTION_CLASS_MAX_ARITY) \
        , param \
        , value \
        ) \
/**/

#define BOOST_MPL_AUX_APPLY_VARIABLE_PARAMS(sep, n, param) \
    BOOST_PREPROCESSOR_IF(n, sep, BOOST_PREPROCESSOR_EMPTY)() \
    BOOST_MPL_TEMPLATE_PARAMETERS(1, BOOST_PREPROCESSOR_INC(n), param) \
/**/


#define BOOST_MPL_AUX_APPLY_0_TEMPLATE_DEF(unused) \
template<typename F> \
struct apply0 \
{ \
    typedef typename F::type type; \
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
template< \
      typename F \
      BOOST_MPL_AUX_APPLY_VARIABLE_PARAMS(BOOST_PREPROCESSOR_COMMA, n, typename T) \
    > \
struct BOOST_PREPROCESSOR_CAT(apply, n) \
{ \
 private: \
    template<bool> struct BOOST_PREPROCESSOR_CAT(F,_wrapper) : F {}; \
    template<> struct BOOST_PREPROCESSOR_CAT(F,_wrapper)<true> \
    { \
        template< BOOST_MPL_AUX_APPLY_VARIABLE_PARAMS( \
              BOOST_PREPROCESSOR_EMPTY \
            , n \
            , typename P \
            ) > struct apply; \
    }; \
 public: \
    typedef typename BOOST_PREPROCESSOR_CAT(F,_wrapper)< \
          aux::msvc_never_true<F>::value \
        >::template apply< BOOST_MPL_AUX_APPLY_VARIABLE_PARAMS( \
                BOOST_PREPROCESSOR_EMPTY \
              , n \
              , T \
              ) >::type type; \
}; \
/**/

#else

#define BOOST_MPL_AUX_APPLY_N_TEMPLATE_DEF(n) \
template< \
      typename F \
      BOOST_MPL_AUX_APPLY_VARIABLE_PARAMS(BOOST_PREPROCESSOR_COMMA, n, typename T) \
    > \
struct BOOST_PREPROCESSOR_CAT(apply,n) \
{ \
    typedef typename F \
        ::template apply< BOOST_MPL_AUX_APPLY_VARIABLE_PARAMS( \
                BOOST_PREPROCESSOR_EMPTY \
              , n \
              , T \
              ) >::type type; \
}; \
/**/

#endif // #if defined(BOOST_MSVC) && (BOOST_MSVC <= 1200)

namespace aux {
// to be able to define specializations
template<int> struct apply_impl_chooser;
}

#define BOOST_MPL_AUX_APPLY_IMPL_CHOOSER_DEF(n) \
namespace aux { \
template<> \
struct apply_impl_chooser<n> \
{ \
    template< typename F, BOOST_MPL_AUX_APPLY_PARAMS(typename T) > \
    struct result_ \
    { \
        typedef typename mpl::BOOST_PREPROCESSOR_CAT(apply,n)< \
              F \
              BOOST_MPL_AUX_APPLY_VARIABLE_PARAMS(BOOST_PREPROCESSOR_COMMA, n, T) \
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
    BOOST_MPL_AUX_APPLY_IMPL_CHOOSER_DEF(n) \
/**/


// apply# 
BOOST_PREPROCESSOR_REPEAT_2ND(
      BOOST_PREPROCESSOR_INC(BOOST_MPL_FUNCTION_CLASS_MAX_ARITY)
    , BOOST_MPL_AUX_APPLY_TEMPLATE_DEF
    , unused
    )

#if !defined(BOOST_MSVC) || (BOOST_MSVC > 1200)
// agurt, 15/jan/02: top-level 'apply' template gives an ICE on MSVC
// (for known reasons); numbered form works, though
template< typename F, BOOST_MPL_AUX_APPLY_DEFAULT_PARAMS(typename T, aux::none) >
struct apply
{
 private:
    typedef aux::count_if_not< aux::none, BOOST_MPL_AUX_APPLY_PARAMS(T) > arity_;

 public:
    typedef typename aux::apply_impl_chooser< arity_::value >
        ::template result_< F, BOOST_MPL_AUX_APPLY_PARAMS(T) >::type type;
};

#define BOOST_MPL_HAS_APPLY

#endif // !defined(BOOST_MSVC) || (BOOST_MSVC > 1200)

#undef BOOST_MPL_AUX_APPLY_TEMPLATE_DEF
#undef BOOST_MPL_AUX_APPLY_N_TEMPLATE_DEF
#undef BOOST_MPL_AUX_APPLY_0_TEMPLATE_DEF
#undef BOOST_MPL_AUX_APPLY_VARIABLE_PARAMS
#undef BOOST_MPL_AUX_APPLY_DEFAULT_PARAMS
#undef BOOST_MPL_AUX_APPLY_PARAMS

} // namespace mpl
} // namespace boost 

#endif // BOOST_MPL_APPLY_HPP_INCLUDED

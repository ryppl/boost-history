//-----------------------------------------------------------------------------
// boost mpl/bind.hpp header file
// See http://www.boost.org for updates, documentation, and revision history.
//-----------------------------------------------------------------------------
//
// Copyright (c) 2001-02
// Peter Dimov, Aleksey Gurtovoy
//
// Permission to use, copy, modify, distribute and sell this software
// and its documentation for any purpose is hereby granted without fee, 
// provided that the above copyright notice appears in all copies and 
// that both the copyright notice and this permission notice appear in 
// supporting documentation. No representations are made about the 
// suitability of this software for any purpose. It is provided "as is" 
// without express or implied warranty.

#ifndef BOOST_MPL_BIND_HPP_INCLUDED
#define BOOST_MPL_BIND_HPP_INCLUDED

// EDG-based compilers have serious problems with preprocessor performance,
// so we have to feed them already preprocessed version of code
#if defined(__EDG__) // && (__EDG_VERSION__ <= ???)
#   include "boost/mpl/aux_/preprocessed/bind.hpp"
#else

#include "boost/mpl/lambda/arg.hpp"
#include "boost/mpl/apply.hpp"
#include "boost/mpl/limits/arity.hpp"
#include "boost/mpl/aux_/count_if_not.hpp"
#include "boost/mpl/aux_/preprocessor.hpp"
#include "boost/mpl/aux_/type_wrapper.hpp"
#include "boost/mpl/aux_/yes_no.hpp"
#include "boost/mpl/aux_/none.hpp"

#include "boost/preprocessor/repeat_2nd.hpp"
#include "boost/preprocessor/repeat_3rd.hpp"
#include "boost/preprocessor/comma_if.hpp"
#include "boost/preprocessor/dec.hpp"
#include "boost/preprocessor/inc.hpp"
#include "boost/preprocessor/cat.hpp"
#include "boost/config.hpp"

namespace boost {
namespace mpl {

#define BOOST_MPL_AUX_APPLY \
    BOOST_PREPROCESSOR_CAT( \
          mpl::apply \
        , BOOST_MPL_FUNCTION_CLASS_MAX_ARITY \
        ) \
/**/

#define BOOST_MPL_AUX_BIND_PARAMS(param) \
    BOOST_MPL_TEMPLATE_PARAMETERS( \
          1 \
        , BOOST_PREPROCESSOR_INC(BOOST_MPL_FUNCTION_CLASS_MAX_ARITY) \
        , param \
        ) \
/**/

#define BOOST_MPL_AUX_BIND_DEFAULT_PARAMS(param, value) \
    BOOST_MPL_DEFAULT_TEMPLATE_PARAMETERS( \
          1 \
        , BOOST_PREPROCESSOR_INC(BOOST_MPL_FUNCTION_CLASS_MAX_ARITY) \
        , param \
        , value \
        ) \
/**/

#define BOOST_MPL_AUX_BIND_VARIABLE_PARAMS(n, param) \
    BOOST_PREPROCESSOR_COMMA_IF(n) \
    BOOST_MPL_TEMPLATE_PARAMETERS(1, BOOST_PREPROCESSOR_INC(n), param) \
/**/

namespace aux {

// agurt, 15/jan/02: it's not a intended to be used as a function class, and 
// MSVC6.5 has problems with 'apply' name here (the code compiles, but doesn't
// work), so i went with the 'result_' here, and in all other similar cases
template<bool>
struct resolve_argument_impl
{
    template< typename T, BOOST_MPL_AUX_BIND_PARAMS(typename U) > struct result_
    {
        typedef T type;
    };
};

template<> 
struct resolve_argument_impl<true>
{
    template< typename T, BOOST_MPL_AUX_BIND_PARAMS(typename U) > struct result_
    {
        typedef typename BOOST_MPL_AUX_APPLY<
              T
            , BOOST_MPL_AUX_BIND_PARAMS(U)
            >::type type;
    };
};

// for 'resolve_bind_argument'
template<typename T> struct is_nested_bind;

template<typename T>
struct resolve_bind_argument
{
    template< BOOST_MPL_AUX_BIND_PARAMS(typename U) > struct result_
    {
        typedef typename resolve_argument_impl< is_nested_bind<T>::value >
            ::template result_< T, BOOST_MPL_AUX_BIND_PARAMS(U) >::type type;
    };
};

} // namespace aux

#define BOOST_MPL_AUX_RESOLVE_BIND_ARGUMENT_0_SPEC(unused)
#define BOOST_MPL_AUX_RESOLVE_BIND_ARGUMENT_N_SPEC(n) \
namespace aux { \
template<> \
struct resolve_bind_argument< mpl::arg<n> > \
{ \
    template<BOOST_MPL_AUX_BIND_PARAMS(typename U)> struct result_ \
    { \
        typedef BOOST_PREPROCESSOR_CAT(U,n) type; \
        typedef char arity_constraint[sizeof(aux::reject_if_none<type>)]; \
    }; \
}; \
} \
/**/

#define BOOST_MPL_AUX_RESOLVE_BIND_ARGUMENT_SPEC(n) \
    BOOST_PREPROCESSOR_IF( \
          n \
        , BOOST_MPL_AUX_RESOLVE_BIND_ARGUMENT_N_SPEC \
        , BOOST_MPL_AUX_RESOLVE_BIND_ARGUMENT_0_SPEC \
        )(n) \
/**/

#define BOOST_MPL_AUX_RESOLVE_ARGUMENT_INVOCATION(n, unused) \
typedef typename aux::resolve_bind_argument< \
      BOOST_PREPROCESSOR_CAT(T, BOOST_PREPROCESSOR_INC(n)) \
    >::template result_< BOOST_MPL_AUX_BIND_PARAMS(U) >::type \
        BOOST_PREPROCESSOR_CAT(t, BOOST_PREPROCESSOR_INC(n)); \
/**/

#define BOOST_MPL_AUX_BIND_N_TEMPLATE_DEF(n) \
template<typename F BOOST_MPL_AUX_BIND_VARIABLE_PARAMS(n, typename T) > \
struct BOOST_PREPROCESSOR_CAT(bind,n) \
{ \
    typedef BOOST_PREPROCESSOR_CAT(bind,n) type; \
    template< BOOST_MPL_AUX_BIND_DEFAULT_PARAMS(typename U, aux::none) > \
    struct apply \
    { \
     private: \
        BOOST_PREPROCESSOR_REPEAT_3RD( \
              n \
            , BOOST_MPL_AUX_RESOLVE_ARGUMENT_INVOCATION \
            , unused \
            ) \
     public: \
        typedef typename mpl::BOOST_PREPROCESSOR_CAT(apply,n)< \
              F \
              BOOST_MPL_AUX_BIND_VARIABLE_PARAMS(n, t) \
            >::type type; \
    }; \
}; \
/**/


namespace aux {

#if defined(__MWERKS__) && __MWERKS__ <= 0x2406

template<typename T>
struct is_nested_bind
{
    BOOST_STATIC_CONSTANT(bool, value = false);
};

#define BOOST_MPL_AUX_IS_NESTED_BIND_DEF(n) \
namespace aux { \
template< typename F BOOST_MPL_AUX_BIND_VARIABLE_PARAMS(n, typename T) > \
struct is_nested_bind< \
      BOOST_PREPROCESSOR_CAT(bind,n)<F BOOST_MPL_AUX_BIND_VARIABLE_PARAMS(n, T)> \
    > \
{ \
    BOOST_STATIC_CONSTANT(bool, value = true); \
}; \
} \
/**/

#else

aux::no_tag is_nested_bind_helper(...);

#define BOOST_MPL_AUX_IS_NESTED_BIND_DEF(n) \
namespace aux { \
template< typename F BOOST_MPL_AUX_BIND_VARIABLE_PARAMS(n, typename T) > \
aux::yes_tag \
is_nested_bind_helper( aux::type_wrapper< \
      BOOST_PREPROCESSOR_CAT(bind,n)<F BOOST_MPL_AUX_BIND_VARIABLE_PARAMS(n, T)> \
    > ); \
} \
/**/

template<typename T> struct is_nested_bind
{
    BOOST_STATIC_CONSTANT(bool, value =
        sizeof(is_nested_bind_helper(type_wrapper<T>())) == sizeof(aux::yes_tag)
        );
};

#endif // defined(__MWERKS__) && __MWERKS__ <= 0x2406

} // namespace aux


namespace aux {
// to be able to define specializations
template<int> struct bind_impl_chooser;
} // namespace aux 

#define BOOST_MPL_AUX_BIND_IMPL_CHOOSER_DEF(n) \
namespace aux { \
template<> \
struct bind_impl_chooser<n> \
{ \
    template< typename F, BOOST_MPL_AUX_BIND_PARAMS(typename T) > \
    struct result_ \
    { \
        typedef BOOST_PREPROCESSOR_CAT(bind,n)< \
              F BOOST_MPL_AUX_BIND_VARIABLE_PARAMS(n, T) \
            > type; \
    }; \
}; \
} \
/**/

#define BOOST_MPL_AUX_BIND_TEMPLATE_DEF(n, unused) \
    BOOST_MPL_AUX_RESOLVE_BIND_ARGUMENT_SPEC(n) \
    BOOST_MPL_AUX_BIND_N_TEMPLATE_DEF(n) \
    BOOST_MPL_AUX_IS_NESTED_BIND_DEF(n) \
    BOOST_MPL_AUX_BIND_IMPL_CHOOSER_DEF(n) \
/**/

// bind# 
BOOST_PREPROCESSOR_REPEAT_2ND(
      BOOST_PREPROCESSOR_INC(BOOST_MPL_FUNCTION_CLASS_MAX_ARITY)
    , BOOST_MPL_AUX_BIND_TEMPLATE_DEF
    , unused
    )

// bind
template< typename F, BOOST_MPL_AUX_BIND_DEFAULT_PARAMS(typename T, aux::none) >
struct bind
{
 private:
    typedef aux::count_if_not< aux::none, BOOST_MPL_AUX_BIND_PARAMS(T) > arity_;

 public:
    typedef typename aux::bind_impl_chooser< arity_::value >
        ::template result_< F, BOOST_MPL_AUX_BIND_PARAMS(T) >::type type;
};

// bind1st/bind2nd, lightweight, for simple cases/backward compatibility
template< typename F, typename T >
struct bind1st
{
    template<typename U> struct apply
        : mpl::apply2<F,T,U>
    {
    };
};

template< typename F, typename T >
struct bind2nd
{
    template<typename U> struct apply
        : mpl::apply2<F,U,T>
    {
    };
};

#undef BOOST_MPL_AUX_BIND_TEMPLATE_DEF
#undef BOOST_MPL_AUX_BIND_IMPL_CHOOSER_DEF
#undef BOOST_MPL_AUX_IS_NESTED_BIND_DEF
#undef BOOST_MPL_AUX_BIND_N_TEMPLATE_DEF
#undef BOOST_MPL_AUX_RESOLVE_ARGUMENT_INVOCATION
#undef BOOST_MPL_AUX_RESOLVE_BIND_ARGUMENT_N_SPEC
#undef BOOST_MPL_AUX_RESOLVE_BIND_ARGUMENT_0_SPEC
#undef BOOST_MPL_AUX_RESOLVE_BIND_ARGUMENT_SPEC
#undef BOOST_MPL_AUX_BIND_VARIABLE_PARAMS
#undef BOOST_MPL_AUX_BIND_DEFAULT_PARAMS
#undef BOOST_MPL_AUX_BIND_PARAMS

} // namespace mpl
} // namespace boost 

#endif // #if defined(__EDG__)

#endif // #ifndef BOOST_MPL_BIND_HPP_INCLUDED

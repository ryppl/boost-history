// (C) Copyright Jonathan Turkanis 2004.
// Permission to copy, use, modify, sell and distribute this software
// is granted provided this copyright notice appears in all copies. This
// software is provided "as is" without express or implied warranty, and
// with no claim as to its suitability for any purpose.

// Contains
//      (1) The metafunction select, which mimics the effect of a chain
//          of nested mpl if_'s or apply_if's. Supports up to 16 cases.
//      (2) The convenience template select_c.
//      (2) The template lazy, for lazy evaluation with select and select_c.
//      (3) The templates lazy_select_c and lazy_select.  Identical to select_c 
//          and select except that lazy evaluation is automatically applied to 
//          each case.
//
// -----------------------------------------------------------------------------
//
// Usage:
//
// (1) select.
//
//     typedef typename select<
//                          case1,  type1,
//                          case2,  type2,
//                          ...
//                          true_,  typen
//                      >::type selection;
//
//     Here case1, case2, ... are models of MPL::IntegralConstant with value type
//     bool, and n <= 16.
//
// (2) select_c. Same as select, except that the cases should be boolean constant
//     expressions.
//
// (3) Lazy evauation. To specify that certain types should be subject to lazy 
//     evaluation, wrap them using the template mpl::lazy
//
//     typedef typename select<
//                          case1,  type1,
//                          case2,  type2,
//                          case3,  lazy<type3>,
//                          ...
//                          true_,  typen
//                      >::type selection;
//
//     Here type3 is a metafunction which will be applied only if it is selected.
//     To specify lazy evalution for each type, use lazy_select or lazy_select_c.
//
// Notes:
//    (1) For compilers which support partial specialization, the implementation
//        is carried out in two steps: first a helper template select4 is defined
//        analogously to if_c, then select_c is defined by application of select4
//        to four instances of itself. For compilers with no partial specialization
//        three intermediate templates are defined: select2, select4 and select8.
//        select8 should be eliminable, but this VC6 gives some trouble.
//    (2) select could be implemented in terms of mpl::switch_; however, the
//        present implementation seems to be faster and more portable.
//    (3) More repetitious code could be reduced by using the preprocessor library;
//        this may be done once the implementation is stable.
//    (4) It would be possible to generalize select so that the maxiumum number 
//        of cases is configurable, and so that select statements with very large
//        numbers of cases would be available, e.g., using select20, select30, 
//        etc. It's not clear that this would be useful.
//    (5) select depends on Type Traits, MPL, Boost.Preprocessor and 
//        select_by_size. select_by_size is needed only for Borland < 5.6.4. This 
//        dependency could presumably be eliminated, but the author is too tired.
//    (6) select has been tested on VC6, VC7.1, GCC 2.95 (cygwin), GCC 3.2 (MinGW)
//        GCC 3.3.1 (cygwin), Comeau C/C++ 4.3.3, Intel for Windows 7.1 and 8.0,
//        BCC 5.5.1 and 5.6.4 and Metrowerks Codewarrior 8.0.
//

#ifndef BOOST_MPL_SELECT_HPP_INCLUDED
#define BOOST_MPL_SELECT_HPP_INCLUDED

#include <boost/mpl/aux_/value_wknd.hpp>
#include <boost/mpl/apply_if.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/mpl/identity.hpp>
#include <boost/mpl/or.hpp>
#include <boost/mpl/void.hpp>
#include <boost/preprocessor/arithmetic/dec.hpp>
#include <boost/preprocessor/arithmetic/inc.hpp>
#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/punctuation/comma_if.hpp>
#include <boost/preprocessor/repetition/enum_params.hpp>
#include <boost/preprocessor/repetition/repeat_from_to.hpp>
#include <boost/type_traits/is_base_and_derived.hpp>

// Needed for Borland 5.5.1 and to reduce template nesting on GCC 2.9x
#if defined(__BORLANDC__) && (__BORLANDC__ < 0x564) || \
    defined(__GNUC__) && (__GNUC__ < 3) 
    #include <boost/utility/select_by_size.hpp>
#endif
                
//--------------Macros--------------------------------------------------------//

// These macros are for readability, not configurability.

// BOOST_MPL_SELECT_C_PARAMS(n) expands to 
//   bool B1 = true, typename T1 = void_, ... , 
//   bool Bn = true, typename Tn = void_.
#define BOOST_MPL_SELECT_C_PARAMS_N(z, n, text)                                \
    BOOST_PP_COMMA_IF(BOOST_PP_DEC(n))                                         \
    bool BOOST_PP_CAT(B, n) = true                                             \
    , typename BOOST_PP_CAT(T, n) = void_                                      \
    /**/
#define BOOST_MPL_SELECT_C_PARAMS(n)                                           \
    BOOST_PP_REPEAT_FROM_TO(1, BOOST_PP_INC(n), BOOST_MPL_SELECT_C_PARAMS_N, _)\
    /**/

// BOOST_MPL_SELECT_PARAMS(n) expands to 
//   typename B1 = true_, typename T1 = void_, ... , 
//   typename Bn = true_, typename Tn = void_.
#define BOOST_MPL_SELECT_PARAMS_N(z, n, text)                                  \
    BOOST_PP_COMMA_IF(BOOST_PP_DEC(n))                                         \
    typename BOOST_PP_CAT(B, n) = true_                                        \
    , typename BOOST_PP_CAT(T, n) = void_                                      \
    /**/
#define BOOST_MPL_SELECT_PARAMS(n)                                             \
    BOOST_PP_REPEAT_FROM_TO(1, BOOST_PP_INC(n), BOOST_MPL_SELECT_PARAMS_N, _)  \
    /**/

// BOOST_MPL_SELECT_ARGS(m, n) expands to Bm, Tm, ..., Bn, Tn
#define BOOST_MPL_SELECT_ARGS_N(z, n, text)                                    \
    BOOST_PP_COMMA_IF(text) BOOST_PP_CAT(B, n), BOOST_PP_CAT(T, n)             \
    /**/
#define BOOST_MPL_SELECT_ARGS(from, to)                                        \
    BOOST_MPL_SELECT_ARGS_N(0, from, 0)                                        \
    BOOST_PP_REPEAT_FROM_TO( BOOST_PP_INC(from), BOOST_PP_INC(to),             \
                             BOOST_MPL_SELECT_ARGS_N, 1 )                      \
    /**/

// BOOST_MPL_SELECT_ARGS_WKND(m, n) expands to 
//   BOOST_MPL_AUX_VALUE_WKND(Bn)::value, Tm, ... , 
//   BOOST_MPL_AUX_VALUE_WKND(Bn)::value, Tn.
#define BOOST_MPL_SELECT_ARGS_WKND_N(z, n, text)                               \
    BOOST_PP_COMMA_IF(BOOST_PP_DEC(n))                                         \
    BOOST_MPL_AUX_VALUE_WKND(BOOST_PP_CAT(B, n))::value, BOOST_PP_CAT(T, n)    \
    /**/
#define BOOST_MPL_SELECT_ARGS_WKND(from, to)                                   \
    BOOST_PP_REPEAT_FROM_TO( from, BOOST_PP_INC(to),                           \
                             BOOST_MPL_SELECT_ARGS_WKND_N, _ )                 \
    /**/

// BOOST_MPL_SELECT_ARGS_WKND(n) expands to 
//   bool_<B1>, T1, ... , bool_<Bn>, Tn.
#define BOOST_MPL_SELECT_ARGS_WRAPPED_N(z, n, text)                            \
    BOOST_PP_COMMA_IF(BOOST_PP_DEC(n))                                         \
    bool_<BOOST_PP_CAT(B, n)>, BOOST_PP_CAT(T, n)                              \
    /**/
#define BOOST_MPL_SELECT_ARGS_WRAPPED(n)                                       \
    BOOST_PP_REPEAT_FROM_TO( 1, BOOST_PP_INC(n),                               \
                             BOOST_MPL_SELECT_ARGS_WRAPPED_N, _ )              \
    /**/


namespace boost { namespace mpl {

//--------------Lazy evaluation machinery-------------------------------------//

namespace aux {
    struct lazy_base { }; // Base class for mpl::lazy.

// Needed for Borland 5.5.1 and to reduce template nesting on GCC 2.9x
#if defined(__BORLANDC__) && (__BORLANDC__ < 0x564) || \
    defined(__GNUC__) && (__GNUC__ < 3) 
    utility::case_<true> is_lazy_helper(lazy_base);
    utility::case_<false> is_lazy_helper(...);
#endif

    // Returns the given type, unless it is of the form lazy<U>, in which case
    // returns the result of applying U.
    template<typename T>
    struct lazy_eval {
        template<typename U>
        struct apply_base_type { typedef typename U::base::type type; };

        // Needed for Borland 5.5.1 and to reduce template nesting on GCC 2.9x.
        // This passes UDTs through ellipsis and so yields UB (or so I'm told)
        #if (!defined(__BORLANDC__) || (__BORLANDC__ >= 0x564)) && \
            (!defined(__GNUC__) || (__GNUC__ > 2)) 
            typedef boost::is_base_and_derived<lazy_base, T> is_lazy;
        #else
            static T* t;
            struct is_lazy {
                BOOST_SELECT_BY_SIZE(bool, value, is_lazy_helper(*t));
            };
        #endif

        typedef typename
                mpl::apply_if<
                    is_lazy,
                    apply_base_type<T>,
                    identity<T>
                >::type
        type;
    };

}               // End namespaces aux.

template<typename T>
struct lazy : public mpl::aux::lazy_base { typedef T base; };

//--------------Definition of select_c----------------------------------------//

#ifndef BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION //---------------------------//

    // This implementation is noticeably faster than the more succint version 
    // for compilers without partial specicialization

    // Machinery for lazy evaluation is necessary only at lowest level,
    // so the intermediate version of select4 omits it to save a few levels
    // of template nesting.
    template<BOOST_MPL_SELECT_PARAMS(4)>
    struct select4 {
        typedef typename
                apply_if<
                    B1, identity<T1>,
                    apply_if<
                        B2, identity<T2>,
                        apply_if<
                            B3, identity<T3>, identity<T4>
                        >
                    >
                >::type type;
    };

    // Version of select4 with support for lazy evaluation.
    template<BOOST_MPL_SELECT_C_PARAMS(4)>
    struct select4_lazy_impl;

    template< typename T1,
              bool B2, typename T2,
              bool B3, typename T3,
              bool B4, typename T4 >
    struct select4_lazy_impl<true, T1, B2, T2, B3, T3, B4, T4> {
        typedef typename aux::lazy_eval<T1>::type type;
    };

    template< typename T1,
              typename T2,
              bool B3, typename T3,
              bool B4, typename T4 >
    struct select4_lazy_impl<false, T1, true, T2, B3, T3, B4, T4> {
        typedef typename aux::lazy_eval<T2>::type type;
    };

    template< typename T1,
              typename T2,
              typename T3,
              bool B4, typename T4 >
    struct select4_lazy_impl<false, T1, false, T2, true, T3, B4, T4> {
        typedef typename aux::lazy_eval<T3>::type type;
    };

    template<typename T1, typename T2, typename T3, typename T4>
    struct select4_lazy_impl<false, T1, false, T2, false, T3, true, T4> {
        typedef typename aux::lazy_eval<T4>::type type;
    };

    template<BOOST_MPL_SELECT_PARAMS(4)>
    struct select4_lazy 
        : select4_lazy_impl<BOOST_MPL_SELECT_ARGS_WKND(1, 4)> 
        { };

    template<BOOST_MPL_SELECT_PARAMS(16)>
    struct select
        : select4<
              or_<B1, B2, B3, B4>,
              select4_lazy<BOOST_MPL_SELECT_ARGS(1, 4)>,
              or_<B5, B6, B7, B8>,
              select4_lazy<BOOST_MPL_SELECT_ARGS(5, 8)>,
              or_<B9, B10, B11, B12>,
              select4_lazy<BOOST_MPL_SELECT_ARGS(9, 12)>,
              or_<B13, B14, B15, B16>,
              select4_lazy<BOOST_MPL_SELECT_ARGS(13, 16)>
          >::type
        { };
#else // #ifndef BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION //------------------//

    template<BOOST_MPL_SELECT_PARAMS(16)>
    struct select_impl {
        typedef typename
                apply_if<
                    or_<B1, B2, B3, B4>,
                    apply_if<
                        B1, identity<T1>,
                        apply_if<
                            B2, identity<T2>,
                            apply_if<
                                B3, identity<T3>, identity<T4>
                            >
                        >
                    >,
                    select_impl<BOOST_MPL_SELECT_ARGS(5, 16)>
                >::type                                result;
        typedef typename aux::lazy_eval<result>::type  type;
    };

    template<> struct select_impl<> { typedef void_ type; };

    template<BOOST_MPL_SELECT_PARAMS(16)>
    struct select : select_impl<BOOST_MPL_SELECT_ARGS(1, 16)> { };

#endif // #ifndef BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION //-----------------//

//--------------Definition of select_c, lazy_select and lazy_select_c---------//

namespace aux { // VC6 Workarounds.

template< BOOST_MPL_SELECT_PARAMS(16), 
          typename Base = 
              typename select<BOOST_MPL_SELECT_ARGS(1, 16)>::type >
struct lazy_select_impl : Base { };

template< BOOST_MPL_SELECT_C_PARAMS(16),
          typename Base = 
              typename select<BOOST_MPL_SELECT_ARGS_WRAPPED(16)>::type >
struct lazy_select_c_impl : Base { };

}               // End namespace aux.

template<BOOST_MPL_SELECT_C_PARAMS(16)>
struct select_c {
    typedef typename select<BOOST_MPL_SELECT_ARGS_WRAPPED(16)>::type type;
};

template< BOOST_MPL_SELECT_PARAMS(16)>
struct lazy_select {
    typedef typename aux::lazy_select_impl<BOOST_MPL_SELECT_ARGS(1, 16)>::type type;
};

template<BOOST_MPL_SELECT_C_PARAMS(16)>
struct lazy_select_c {
    typedef typename aux::lazy_select_c_impl<BOOST_MPL_SELECT_ARGS(1, 16)>::type type;
};

} }             // End namespaces mpl, boost.

//--------------Undefine macros-----------------------------------------------//

#undef BOOST_MPL_SELECT_C_PARAMS_N
#undef BOOST_MPL_SELECT_C_PARAMS
#undef BOOST_MPL_SELECT_PARAMS_N
#undef BOOST_MPL_SELECT_PARAMS
#undef BOOST_MPL_SELECT_ARGS_N
#undef BOOST_MPL_SELECT_ARGS
#undef BOOST_MPL_SELECT_ARGS_WKND_N
#undef BOOST_MPL_SELECT_ARGS_WKND
#undef BOOST_MPL_SELECT_ARGS_WRAPPED_N
#undef BOOST_MPL_SELECT_ARGS_WRAPPED_N

#endif          // #ifndef BOOST_MPL_SELECT_HPP_INCLUDED

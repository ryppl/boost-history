// (C) Copyright Jonathan Turkanis 2004.
// Permission to copy, use, modify, sell and distribute this software
// is granted provided this copyright notice appears in all copies. This
// software is provided "as is" without express or implied warranty, and
// with no claim as to its suitability for any purpose.

// Contains
//      (1) the metafunction select, which mimics the effect of a chain
//          of nested mpl if_'s or apply_if's, and plus the convenience template
//          select_c, and
//      (2) the template lazy, for lazy evaluation with select and select_c.
//
// -----------------------------------------------------------------------------
//
// Usage:
//
// typedef typename select<
//                      case1,  type1,
//                      case2,  type2,
//                      ...
//                      true_,  typen
//                  >::type selection;
//
// Here case1, case2, ... are models of MPL::IntegralConstant with value type
// bool, and n <= 16.
//
// To get the effect of mpl::apply_if for a given type, wrap it with the
// template lazy. E.g.:
//
// typedef typename select<
//                      case1,  type1,
//                      case2,  type2,
//                      case3,  lazy<type3>,
//                      ...
//                      true_,  typen
//                  >::type selection;
//
// Here type3 is a metafunction which will be applied only if it is selected.
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
//        select_by_size select_by_size is needed only for Borland < 5.6.4. This 
//        dependency could presumably be eliminated, but the author is too tired.
//

#ifndef BOOST_MPL_SELECT_HPP_INCLUDED
#define BOOST_MPL_SELECT_HPP_INCLUDED

#include <boost/mpl/aux_/value_wknd.hpp>
#include <boost/mpl/apply_if.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/mpl/identity.hpp>
#include <boost/preprocessor/arithmetic/dec.hpp>
#include <boost/preprocessor/arithmetic/inc.hpp>
#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/comparison/equal.hpp>
#include <boost/preprocessor/control/if.hpp>
#include <boost/preprocessor/punctuation/comma_if.hpp>
#include <boost/preprocessor/repetition/repeat_from_to.hpp>
#include <boost/type_traits/is_base_and_derived.hpp>
#include <boost/type_traits/is_convertible.hpp>
#include <boost/type_traits/detail/yes_no_type.hpp>

#include <boost/mpl/vector/vector20.hpp>
#include <boost/mpl/pair.hpp>
#include <boost/mpl/switch.hpp>
#include <boost/mpl/aux_/lambda_support.hpp>
#include <boost/mpl/aux_/void_spec.hpp>
#include <boost/mpl/aux_/preprocessor/def_params_tail.hpp>
#include <boost/mpl/void.hpp>
#include <boost/mpl/aux_/arity_spec.hpp>
#include <boost/mpl/aux_/lambda_spec.hpp>

// Needed for Borland 5.5.1 and to reduce template nesting on GCC 2.9x
#if defined(__BORLANDC__) && (__BORLANDC__ < 0x564) || \
    defined(__GNUC__) && (__GNUC__ < 3) 
    #include <boost/utility/select_by_size.hpp>
#endif

// BOOST_SELECT_C_PARAMS(n) expands to 
//   bool B1 = true, typename T1 = select_default, ... , 
//   bool Bn = true, typename Tn = select_default.
#define BOOST_SELECT_C_PARAMS_N(z, n, text)                                    \
    BOOST_PP_COMMA_IF(BOOST_PP_DEC(n))                                         \
    bool BOOST_PP_CAT(B, n) = true                                             \
    , typename BOOST_PP_CAT(T, n) = aux_::select_default                       \
    /**/
#define BOOST_SELECT_C_PARAMS(n)                                               \
    BOOST_PP_REPEAT_FROM_TO(1, BOOST_PP_INC(n), BOOST_SELECT_C_PARAMS_N, _)    \
    /**/

// BOOST_SELECT_PARAMS(n) expands to 
//   typename B1 = true_, typename T1 = select_default, ... , 
//   typename Bn = true_, typename Tn = select_default.
#define BOOST_SELECT_PARAMS_N(z, n, text)                                      \
    BOOST_PP_COMMA_IF(BOOST_PP_DEC(n))                                         \
    typename BOOST_PP_CAT(B, n) = true_                                        \
    , typename BOOST_PP_CAT(T, n) = aux_::select_default                       \
    /**/
#define BOOST_SELECT_PARAMS(n)                                                 \
    BOOST_PP_REPEAT_FROM_TO(1, BOOST_PP_INC(n), BOOST_SELECT_PARAMS_N, _)      \
    /**/

// BOOST_SELECT_PARAMS(n) expands to 
//   BOOST_MPL_AUX_VALUE_WKND(B1)::value, T1, ... , 
//   BOOST_MPL_AUX_VALUE_WKND(Bn)::value, Tn.
#define BOOST_SELECT_C_ARGS_N(z, n, text)                                      \
    BOOST_PP_COMMA_IF(BOOST_PP_DEC(n))                                         \
    BOOST_MPL_AUX_VALUE_WKND(BOOST_PP_CAT(B, n))::value, BOOST_PP_CAT(T, n)    \
    /**/
#define BOOST_SELECT_C_ARGS(n)                                                 \
    BOOST_PP_REPEAT_FROM_TO(1, BOOST_PP_INC(n), BOOST_SELECT_C_ARGS_N, _)      \
    /**/

namespace boost { namespace mpl {

namespace aux_ {
    struct lazy_base { };

// Needed for Borland 5.5.1 and to reduce template nesting on GCC 2.9x
#if defined(__BORLANDC__) && (__BORLANDC__ < 0x564) || \
    defined(__GNUC__) && (__GNUC__ < 3) 
    utility::case_<true> is_lazy_helper(lazy_base);
    utility::case_<false> is_lazy_helper(...);
#endif

    // Returns the given type, unless is is of the form eval<U>, in which case
    // returns the result of applying U.
    template<typename T>
    struct lazy_eval {
        template<typename U>
        struct apply_base_type {
            typedef typename U::base::type type;
        };

        // Needed for Borland 5.5.1 and to reduce template nesting on GCC 2.9x.
        // This passes UDTs through ellipsis and so yields UB (or so I'm told)
        #if (!defined(__BORLANDC__) || (__BORLANDC__ >= 0x564)) && \
            (!defined(__GNUC__) || (__GNUC__ > 2)) 
            typedef boost::is_convertible<T, lazy_base> is_lazy;
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

}               // End namespaces aux_.

template<typename T>
struct lazy : public mpl::aux_::lazy_base { typedef T base; };

namespace aux_ {

// Unspecified types default to void.
#if !defined(BOOST_MSVC) || (BOOST_MSVC >= 1300)
    typedef void select_default;
#else
    typedef lazy< mpl::identity<void> > select_default;
#endif

}               // End namespace aux_.

#ifndef BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION

    // Machinery for lazy evaluation is necessary only at lowest level,
    // so the intermediate version of select4 omits it to save a few levels
    // of template nesting.
    template<BOOST_SELECT_C_PARAMS(4)>
    struct select4;

    template< typename T1,
              bool B2, typename T2,
              bool B3, typename T3,
              bool B4, typename T4 >
    struct select4<true, T1, B2, T2, B3, T3, B4, T4> {
        typedef T1 type;
    };

    template< typename T1,
              typename T2,
              bool B3, typename T3,
              bool B4, typename T4 >
    struct select4<false, T1, true, T2, B3, T3, B4, T4> {
        typedef T2 type;
    };

    template< typename T1,
              typename T2,
              typename T3,
              bool B4, typename T4 >
    struct select4<false, T1, false, T2, true, T3, B4, T4> {
        typedef T3 type;
    };

    template<typename T1, typename T2, typename T3, typename T4>
    struct select4<false, T1, false, T2, false, T3, true, T4> {
        typedef T4 type;
    };

    // Version of select4 with support for lazy evaluation.
    template<BOOST_SELECT_C_PARAMS(4)>
    struct select4_lazy;

    template< typename T1,
              bool B2, typename T2,
              bool B3, typename T3,
              bool B4, typename T4 >
    struct select4_lazy<true, T1, B2, T2, B3, T3, B4, T4> {
        typedef typename aux_::lazy_eval<T1>::type type;
    };

    template< typename T1,
              typename T2,
              bool B3, typename T3,
              bool B4, typename T4 >
    struct select4_lazy<false, T1, true, T2, B3, T3, B4, T4> {
        typedef typename aux_::lazy_eval<T2>::type type;
    };

    template< typename T1,
              typename T2,
              typename T3,
              bool B4, typename T4 >
    struct select4_lazy<false, T1, false, T2, true, T3, B4, T4> {
        typedef typename aux_::lazy_eval<T3>::type type;
    };

    template<typename T1, typename T2, typename T3, typename T4>
    struct select4_lazy<false, T1, false, T2, false, T3, true, T4> {
        typedef typename aux_::lazy_eval<T4>::type type;
    };

    template<BOOST_SELECT_C_PARAMS(16)>
    struct select_c
        : select4<
            B1 || B2 || B3 || B4,
            select4_lazy<B1, T1, B2, T2, B3, T3, true, T4>,
            B5 || B6 || B7 || B8,
            select4_lazy<B5, T5, B6, T6, B7, T7, true, T8>,  
            B9 || B10 || B11 || B12,
            select4_lazy<B9, T9, B10, T10, B11, T11, true, T12>,  
            B13 || B14 || B15 || B16,
            select4_lazy<B13, T13, B14, T14, B15, T15, true, T16>
          >::type
        { };
#else // #ifndef BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION
    template<BOOST_SELECT_C_PARAMS(2)>
    struct select2 {
        typedef typename
                apply_if_c<
                    B1,
                    aux_::lazy_eval<T1>,
                    aux_::lazy_eval<T2>
                >::type
        type;
    };

    template<BOOST_SELECT_C_PARAMS(4)>
    struct select4 {
        typedef typename
                apply_if_c<
                    B1 || B2,
                    select2<B1, T1, true, T2>,
                    select2<B3, T3, true, T4>
                >::type
        type;
    };

    template<BOOST_SELECT_C_PARAMS(8)>
    struct select8 {
        typedef typename
                apply_if_c<
                    B1 || B2 || B3 || B4,
                    select4<B1, T1, B2, T2, B3, T3, true, T4>,
                    select4<B5, T5, B6, T6, B7, T7, true, T8>
                >::type
        type;
    };

    template<BOOST_SELECT_C_PARAMS(16)>
    struct select_c {
        typedef typename
                apply_if_c<
                    B1 || B2 || B3 || B4 || B5 || B6 || B7 || B8,
                    select8<
                        B1, T1, B2, T2, B3, T3, B4, T4, 
                        B5, T5, B6, T6, B7, T7, true, T8
                    >,
                    select8<
                        B9, T9, B10, T10, B11, T11, B12, T12, 
                        B13, T13, B14, T14, B15, T15, true, T16
                    >
                >::type
        type;
    };
#endif // #ifndef BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION

template<BOOST_SELECT_PARAMS(16)>
struct select {
    typedef typename select_c<BOOST_SELECT_C_ARGS(16)>::type type;
};

} }             // End namespaces mpl, boost.

#endif          // #ifndef BOOST_MPL_SELECT_HPP_INCLUDED

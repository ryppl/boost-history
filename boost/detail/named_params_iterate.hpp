// Copyright David Abrahams, Daniel Wallin 2003. Use, modification and 
// distribution is subject to the Boost Software License, Version 1.0. 
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)

// This file generates overloads in this format:
//
//     template<class A0, class A1>
//     detail::arg_list<
//         BOOST_DEDUCED_TYPENAME aux::as_named<K0, A0>::type
//       , detail::arg_list<
//             BOOST_DEDUCED_TYPENAME aux::as_named<K1, A1>::type
//         >
//     >
//     operator()(const A0& a0, const A1& a1) const
//     {
//         using aux::arg_list;
//         using aux::empty;
//        
//         // for cwpro8
//         typedef typename aux::as_named<K0, A0>::type t0;
//         typedef typename aux::as_named<K1, A1>::type t1;
//         
//         t0 n0(a0);
//         t1 n1(a1);
// 
//         typedef arg_list<t0, arg_list<t1> > list_t;
//         return list_t(n0, n1, empty(), empty(), empty());
//     }
//

#if !defined(BOOST_PP_IS_ITERATING)
# error Boost.NamedParams - do not include this file!
#endif

#define N BOOST_PP_ITERATION()

#define BOOST_NAMED_PARAMS_LIST(z, n, text) \
    aux::arg_list< \
        BOOST_DEDUCED_TYPENAME aux::as_tagged_argument< \
            BOOST_PP_CAT(K, n), BOOST_PP_CAT(A, n) \
        >::type

#define BOOST_NAMED_PARAMS_CLOSE_LIST(z, n, text) > 

#define BOOST_NAMED_PARAMS_NAMED_TYPE(z, n, text) \
    typename aux::as_tagged_argument< \
        BOOST_PP_CAT(K, n), BOOST_PP_CAT(A, n) \
    >::type BOOST_PP_CAT(named, n)(BOOST_PP_CAT(a, n));

template<BOOST_PP_ENUM_PARAMS(N, class A)>
BOOST_PP_ENUM(N, BOOST_NAMED_PARAMS_LIST, _)
BOOST_PP_REPEAT(N, BOOST_NAMED_PARAMS_CLOSE_LIST, _)
operator()(BOOST_PP_ENUM_BINARY_PARAMS(N, A, const& a)) const
{
    using aux::arg_list;
    using aux::empty_arg_list;

    typedef BOOST_PP_ENUM(N, BOOST_NAMED_PARAMS_LIST, _)
        BOOST_PP_REPEAT(N, BOOST_NAMED_PARAMS_CLOSE_LIST, _) list_t;

    BOOST_PP_REPEAT(N, BOOST_NAMED_PARAMS_NAMED_TYPE, _)

    return list_t(
        BOOST_PP_ENUM_PARAMS(N, named)
        BOOST_PP_ENUM_TRAILING_PARAMS(
            BOOST_PP_SUB(BOOST_NAMED_PARAMS_MAX_ARITY, N)
          , empty_arg_list() BOOST_PP_INTERCEPT
        ));
}

#undef BOOST_NAMED_PARAMS_NAMED_TYPE
#undef BOOST_NAMED_PARAMS_CLOSE_LIST
#undef BOOST_NAMED_PARAMS_LIST
#undef N


// Copyright David Abrahams, Daniel Wallin 2003. Use, modification and 
// distribution is subject to the Boost Software License, Version 1.0. 
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)

// Generates overloaded function call operators for
// boost::named_params::parameters.  See
// boost/named_params/named_params.hpp for examples of the pattern

#if !defined(BOOST_PP_IS_ITERATING)
# error Boost.NamedParams - do not include this file!
#endif

#define N BOOST_PP_ITERATION()

#define BOOST_NAMED_PARAMS_open_list(z, n, text) \
    aux::arg_list< \
        BOOST_DEDUCED_TYPENAME aux::as_tagged_argument< \
            BOOST_PP_CAT(PS, n), BOOST_PP_CAT(A, n) \
        >::type

#define BOOST_NAMED_PARAMS_close_list(z, n, text) > 

#define BOOST_NAMED_PARAMS_tagged_argument(z, n, text) \
    typename aux::as_tagged_argument< \
        BOOST_PP_CAT(PS, n), BOOST_PP_CAT(A, n) \
    >::type BOOST_PP_CAT(tagged, n)(BOOST_PP_CAT(a, n));

#define BOOST_NAMED_PARAMS_arg_list(n) \
    BOOST_PP_ENUM(N, BOOST_NAMED_PARAMS_open_list, _) \
    BOOST_PP_REPEAT(N, BOOST_NAMED_PARAMS_close_list, _)

template<BOOST_PP_ENUM_PARAMS(N, class A)>
BOOST_NAMED_PARAMS_arg_list(N)
operator()(BOOST_PP_ENUM_BINARY_PARAMS(N, A, const& a)) const
{
    typedef BOOST_NAMED_PARAMS_arg_list(N) arg_tuple;

    BOOST_PP_REPEAT(N, BOOST_NAMED_PARAMS_tagged_argument, _)

    return arg_tuple(
        BOOST_PP_ENUM_PARAMS(N, tagged)
        BOOST_PP_ENUM_TRAILING_PARAMS(
            BOOST_PP_SUB(BOOST_NAMED_PARAMS_MAX_ARITY, N)
          , aux::void_() BOOST_PP_INTERCEPT
        ));
}

#undef BOOST_NAMED_PARAMS_arg_list
#undef BOOST_NAMED_PARAMS_open_list
#undef BOOST_NAMED_PARAMS_close_list
#undef BOOST_NAMED_PARAMS_tagged_argument
#undef N


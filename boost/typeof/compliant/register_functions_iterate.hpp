// Copyright (C) 2004 Arkadiy Vertleyb
// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (http://www.boost.org/LICENSE_1_0.txt)

#define n BOOST_PP_ITERATION()

namespace
{
    // functions

    template<class V, class R BOOST_PP_ENUM_TRAILING_PARAMS(n, class P)> 
    struct encode_type_impl<V, R(*)(BOOST_PP_ENUM_PARAMS(n, P))>
    {
        typedef R BOOST_PP_CAT(P, n);
        typedef BOOST_TYPEOF_ENCODE_PARAMS(BOOST_PP_INC(n), FUN_ID + n) type;
    };

    template<class Iter>
    struct decode_type_impl<mpl::size_t<FUN_ID + n>, Iter>
    {
        typedef Iter iter0;
        BOOST_TYPEOF_DECODE_PARAMS(BOOST_PP_INC(n))
        typedef BOOST_PP_CAT(p, n)(*type)(BOOST_PP_ENUM_PARAMS(n, p));
        typedef BOOST_PP_CAT(iter, BOOST_PP_INC(n)) iter;
    };

    // member functions

    #define BOOST_TYPEOF_qualifier 
    #define BOOST_TYPEOF_id MEM_FUN_ID
    #include <boost/typeof/compliant/register_mem_functions.hpp>

    #define BOOST_TYPEOF_qualifier const
    #define BOOST_TYPEOF_id CONST_MEM_FUN_ID
    #include <boost/typeof/compliant/register_mem_functions.hpp>

    #define BOOST_TYPEOF_qualifier volatile 
    #define BOOST_TYPEOF_id VOLATILE_MEM_FUN_ID
    #include <boost/typeof/compliant/register_mem_functions.hpp>

    #define BOOST_TYPEOF_qualifier volatile const
    #define BOOST_TYPEOF_id VOLATILE_CONST_MEM_FUN_ID
    #include <boost/typeof/compliant/register_mem_functions.hpp>
}

#undef n

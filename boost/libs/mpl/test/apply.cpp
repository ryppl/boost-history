
// Copyright (c) Aleksey Gurtovoy 2000-2004
//
// Distributed under the Boost Software License, Version 1.0. 
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/mpl for documentation.

// $Source$
// $Date$
// $Revision$

#include <boost/mpl/apply.hpp>
#include <boost/mpl/lambda.hpp>
#include <boost/mpl/plus.hpp>
#include <boost/mpl/int.hpp>
#include <boost/mpl/aux_/test.hpp>

#include <boost/type_traits/is_same.hpp>

#include <vector>

MPL_TEST_CASE()
{
    typedef plus<int_<2>,int_<3> > plus1;
    typedef lambda<plus1>::type plus2;
    MPL_ASSERT(( boost::is_same< plus1,plus2 > ));

    typedef lambda<std::vector<int> >::type v;
    MPL_ASSERT(( boost::is_same< v,std::vector<int> > ));

    typedef lambda<std::vector<_1> >::type make_vector;
    typedef make_vector::apply<int>::type v;
    MPL_ASSERT(( boost::is_same< v,std::vector<int> > ));
}

MPL_TEST_CASE()
{
    typedef plus<_1,_2> plus_fun;
    typedef apply<plus_fun,int_<2>,int_<3> >::type res;

    MPL_ASSERT_RELATION( res::value, ==, 5 );
}

MPL_TEST_CASE()
{
    typedef apply<_1, plus<_1,_2> >::type plus_fun;
    MPL_ASSERT(( is_same< plus_fun,plus<_1,_2> > ));

    typedef apply<plus_fun,int_<2>,int_<3> >::type res;
    MPL_ASSERT_RELATION( res::value, ==, 5 );
}

MPL_TEST_CASE()
{
    typedef lambda< lambda<_1> >::type make_lambda;
    typedef make_lambda::apply< std::vector<int> >::type v;
    MPL_ASSERT(( boost::is_same< v,std::vector<int> > ));

    typedef make_lambda::apply< std::vector<_1> >::type make_vector;
    typedef make_vector::apply<int>::type v_int;
    MPL_ASSERT(( boost::is_same< v_int,std::vector<int> > ));
}

MPL_TEST_CASE()
{
    typedef apply< _1, std::vector<int> >::type v;
    MPL_ASSERT(( boost::is_same< v,std::vector<int> > ));

    typedef apply< _1, std::vector<_1> >::type v_lambda;
    typedef apply<v_lambda,int>::type v_int;
    MPL_ASSERT(( boost::is_same< v,std::vector<int> > ));
}

MPL_TEST_CASE()
{
    typedef apply< lambda<_1>, std::vector<int> >::type v;
    MPL_ASSERT(( boost::is_same< v,std::vector<int> > ));

    typedef apply< lambda<_1>, std::vector<_1> >::type make_vector;
    typedef make_vector::apply<int>::type v_int;
    MPL_ASSERT(( boost::is_same< v,std::vector<int> > ));
}

MPL_TEST_CASE()
{
    typedef apply< lambda<_1>, plus<_1,_2> >::type plus_fun;
    typedef plus_fun::apply< int_<2>,int_<3> >::type res;

    MPL_ASSERT_RELATION( res::value, ==, 5 );
}

MPL_TEST_CASE()
{
    typedef bind2<plus<>,_1,_1> b1;
    typedef lambda<b1>::type b2;
    MPL_ASSERT(( boost::is_same< b1,b2 > ));
}

MPL_TEST_CASE()
{
#if !BOOST_WORKAROUND(__MWERKS__, BOOST_TESTED_AT(0x0302))
    typedef lambda< lambda< bind2<plus<>,_1,_1> > >::type make_lambda;
    typedef make_lambda::type::apply< int_<5> >::type res;
    MPL_ASSERT_RELATION( res::value, ==, 10 );
#endif
}

MPL_TEST_CASE()
{
    typedef apply< bind2<plus<>,_1,_1>, int_<5> >::type res;
    MPL_ASSERT_RELATION( res::value, ==, 10 );
}

MPL_TEST_CASE()
{
    typedef apply<_1, lambda<plus<_1,_2> > >::type plus_fun;
    typedef plus_fun::type::apply< int_<2>,int_<3> >::type res;

    MPL_ASSERT_RELATION( res::value, ==, 5 );
}

#if 0// defined(BOOST_MPL_HAS_APPLY)
MPL_TEST_CASE()
{
#   define MAKE_APPLY_NAME(i) apply
    BOOST_PP_REPEAT(
          BOOST_MPL_LIMIT_METAFUNCTION_ARITY
        , APPLY_TEST
        , MAKE_APPLY_NAME
        )
}
#endif

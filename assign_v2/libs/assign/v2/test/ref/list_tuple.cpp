//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#include <assert.h>
#include <string>
#include <boost/mpl/vector/vector0.hpp>
#include <boost/mpl/vector/vector10.hpp>
#include <boost/assign/v2/detail/config/check.hpp>
#include <boost/assign/v2/ref/list_tuple.hpp>

#include <boost/type_traits/add_reference.hpp>

namespace test_assign_v2{
namespace xxx_ref{
namespace xxx_list_tuple{

    void test()
    {
        namespace as2 = boost::assign::v2;

        #if BOOST_ASSIGN_V2_ENABLE_CPP0X
        #define BOOST_ASSIGN_V2_USING_GET using as2::ref::get;
        #else
        #define BOOST_ASSIGN_V2_USING_GET using boost::get;
        #endif

        typedef boost::mpl::int_<0> i0_;
        typedef boost::mpl::int_<1> i1_;
        typedef boost::mpl::int_<2> i2_;
        typedef boost::mpl::int_<3> i3_;

        #define A 1
        #define B 2
        int a = A;
        int b = B;
#if BOOST_ASSIGN_V2_ENABLE_CPP0X
            typedef int& lvalue_;
            typedef int const& clvalue_;
            typedef int rvalue_;
#else
            typedef int lvalue_;
            typedef int const clvalue_;
            typedef int const rvalue_;
#endif
        {
            typedef boost::mpl::vector0<> v0_;

            typedef as2::ref::nth_result_of::list_tuple meta_result_;
            typedef boost::mpl::vector2<
                v0_,
                v0_
            > input_;

            typedef boost::mpl::apply1<
                meta_result_,
                input_
            >::type result_;

            result_ list = as2::ref::list_tuple()();

        }
        {
            typedef boost::mpl::vector1<lvalue_> v0_;
            typedef boost::mpl::vector1<rvalue_> v1_;

            typedef as2::ref::nth_result_of::list_tuple meta_result_;
            typedef boost::mpl::vector4<
                v0_,
                v1_,
                v0_,
                v1_
            > input_;

            typedef boost::mpl::apply1<
                meta_result_,
                input_
            >::type result_;

            result_ list = as2::ref::list_tuple( a )( A )( b )( B );
            BOOST_ASSIGN_V2_USING_GET
            BOOST_ASSIGN_V2_CHECK( &get<0>( list.get( i0_() ) ) == &a );
            BOOST_ASSIGN_V2_CHECK(  get<0>( list.get( i1_() ) ) == A );
            BOOST_ASSIGN_V2_CHECK( &get<0>( list.get( i2_() ) ) == &b );
            BOOST_ASSIGN_V2_CHECK(  get<0>( list.get( i3_() ) ) == B );

        }
        {
            typedef boost::mpl::vector2<lvalue_, lvalue_> v0_;
            typedef boost::mpl::vector2<lvalue_, rvalue_> v1_;
            typedef boost::mpl::vector2<rvalue_, lvalue_> v2_;
            typedef boost::mpl::vector2<rvalue_, rvalue_> v3_;

            typedef as2::ref::nth_result_of::list_tuple meta_result_;
            typedef boost::mpl::vector4<
                v0_,
                v1_,
                v2_,
                v3_
            > input_;

            typedef boost::mpl::apply1<
                meta_result_,
                input_
            >::type result_;

            result_ list
                = as2::ref::list_tuple( a, b )( a, B )( A, b )( A, B );
            BOOST_ASSIGN_V2_USING_GET
            BOOST_ASSIGN_V2_CHECK( &get<0>( list.get( i0_() ) ) == &a );
            BOOST_ASSIGN_V2_CHECK( &get<1>( list.get( i0_() ) ) == &b );
            BOOST_ASSIGN_V2_CHECK( &get<0>( list.get( i1_() ) ) == &a );
            BOOST_ASSIGN_V2_CHECK(  get<1>( list.get( i1_() ) ) == B  );
            BOOST_ASSIGN_V2_CHECK(  get<0>( list.get( i2_() ) ) == A  );
            BOOST_ASSIGN_V2_CHECK( &get<1>( list.get( i2_() ) ) == &b );
            BOOST_ASSIGN_V2_CHECK(  get<0>( list.get( i3_() ) ) == A  );
            BOOST_ASSIGN_V2_CHECK(  get<1>( list.get( i3_() ) ) == B  );

        }
        {
            typedef boost::mpl::vector2<lvalue_, lvalue_> v0_;
            typedef boost::mpl::vector2<lvalue_, clvalue_> v1_;
            typedef boost::mpl::vector2<clvalue_, lvalue_> v2_;
            typedef boost::mpl::vector2<clvalue_, clvalue_> v3_;

            typedef as2::ref::nth_result_of::list_tuple meta_result_;
            typedef boost::mpl::vector4<
                v0_,
                v1_,
                v2_,
                v3_
            > input_;

            typedef boost::mpl::apply1<
                meta_result_,
                input_
            >::type result_;

            int const aa = A;
            int const bb = B;

            result_ list
                = as2::ref::list_tuple( a, b )( a, bb )( aa, b )( aa, bb );
            BOOST_ASSIGN_V2_USING_GET
            BOOST_ASSIGN_V2_CHECK( &get<0>( list.get( i0_() ) ) == &a );
            BOOST_ASSIGN_V2_CHECK( &get<1>( list.get( i0_() ) ) == &b );
            BOOST_ASSIGN_V2_CHECK( &get<0>( list.get( i1_() ) ) == &a );
            BOOST_ASSIGN_V2_CHECK( &get<1>( list.get( i1_() ) ) == &bb );
            BOOST_ASSIGN_V2_CHECK( &get<0>( list.get( i2_() ) ) == &aa );
            BOOST_ASSIGN_V2_CHECK( &get<1>( list.get( i2_() ) ) == &b );
            BOOST_ASSIGN_V2_CHECK( &get<0>( list.get( i3_() ) ) == &aa );
            BOOST_ASSIGN_V2_CHECK( &get<1>( list.get( i3_() ) ) == &bb );
        }

        #undef A
        #undef B

        {
            // TODO
            typedef const char ar_[2];
            #if BOOST_ASSIGN_V2_ENABLE_CPP0X
            typedef boost::add_reference<ar_>::type type;
            #else
            typedef ar_ type;
            #endif
            typedef boost::mpl::vector1<type> v0_;

            typedef as2::ref::nth_result_of::list_tuple meta_result_;
            typedef boost::mpl::vector1<v0_> input_;

            typedef boost::mpl::apply1<meta_result_, input_>::type result_;

            result_ list = as2::ref::list_tuple( "x" );
            typedef std::string str_;
            BOOST_ASSIGN_V2_USING_GET
            BOOST_ASSIGN_V2_CHECK(
                str_( get<0>( list.get( i0_() ) ) ) == "x" );
            BOOST_ASSIGN_V2_CHECK(
                str_( get<0>( list.get( i0_() ) ) ) != "y" );
        }
        #undef BOOST_ASSIGN_V2_USING_GET
    }// test

}// xxx_list_tuple
}// xxx_ref
}// test_assign_v2

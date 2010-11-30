//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#include <boost/mpl/vector/vector0.hpp>
#include <boost/mpl/vector/vector10.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/assert.hpp>

#include <boost/type_traits/is_same.hpp>
#include <boost/typeof/typeof.hpp>

#include <boost/assign/v2/detail/checking/check_equal.hpp>
#include <boost/assign/v2/ref/fusion_matrix/container.hpp>
#include <boost/assign/v2/ref/fusion_matrix/nth_result_of.hpp>

#if BOOST_ASSIGN_V2_ENABLE_CPP0X
#else
#include <boost/assign/v2/detail/config/limit_lvalue_const_arity.hpp>
#if BOOST_ASSIGN_V2_LIMIT_LVALUE_CONST_ARITY < 2
#error
#endif
#endif

#include <libs/assign/v2/test/ref/fusion_matrix.h>

namespace test_assign_v2{
namespace xxx_ref{
namespace xxx_fusion_matrix{

    void test()
    {
    	using namespace boost::assign::v2;
        typedef ref::fusion_matrix_aux::empty<>::type empty_;
        typedef int value_; typedef value_ const cvalue_;
        typedef value_ a_; typedef cvalue_ b_;
        typedef ref::nth_result_of::fusion_matrix<> meta_result_;
        typedef boost::mpl::vector0<>          v0_;
        typedef boost::mpl::vector1<a_>        v1_a_;
        typedef boost::mpl::vector1<b_>        v1_b_;
        typedef boost::mpl::vector2<a_, a_>    v2_a_a_;
        typedef boost::mpl::vector2<a_, b_>    v2_a_b_;
        typedef boost::mpl::vector2<b_, b_>    v2_b_b_;
#define MACRO(i, j, t)\
    {\
        typedef result_::static_row_result<i>::type row_;\
        row_ row = result.static_row( boost::mpl::int_<i>() );\
        BOOST_ASSIGN_V2_CHECK_EQUAL(\
            row.static_elem(boost::mpl::int_<j>() ), \
            t\
        );\
    }\
/**/
        int a = 1;
        #define B 2
        {

            typedef boost::mpl::apply1<
                meta_result_,
                boost::mpl::vector<
                    v1_a_,
                    v1_b_,
                    v0_,
                    v2_a_a_,
                    v2_a_b_,
                    v2_b_b_
                >
            >::type result_;
            {
                // b passed as const lvalue
                cvalue_ b = B;

                empty_ e;
                result_ result = e          //  i   k
                    ( a )                   //  0   1
                    ( b )                   //  1   1
                    ( )                     //  2   0
                    ( a, a )                //  3   2
                    ( a, b )                //  4   2
                    ( b, b );               //  5   2

                MACRO(0, 0, a)
                MACRO(1, 0, b)
                typedef result_::static_row_result<3>::type row_;
                row_ row = result.static_row( boost::mpl::int_<3>() );
                MACRO(3, 0, a)
                    MACRO(3, 1, a)
                MACRO(4, 0, a)
                    MACRO(4, 1, b)
                MACRO(5, 0, b)
                    MACRO(5, 1, b)
            }
            {
                // b passed as rvalue
                empty_ e;
                result_ result = e                  //  i   k
                    ( a )                           //  0   1
                    ( B )                           //  1   1
                    ( )                             //  2   0
                    ( a, a )                        //  3   2
                    ( a, B )                        //  4   2
                    ( B, B );                       //  5   2

                MACRO(0, 0, a)
                MACRO(1, 0, B)
                typedef result_::static_row_result<3>::type row_;
                row_ row = result.static_row( boost::mpl::int_<3>() );
                
                MACRO(3, 0, a)
                    MACRO(3, 1, a)
                MACRO(4, 0, a)
                    MACRO(4, 1, B)
                MACRO(5, 0, B)
                    MACRO(5, 1, B)
            }

            #undef B
        }
#undef MACRO

    }

}// xxx_fusion_matrix
}// xxx_ref
}// test_assign

//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#include <string>
#include <boost/mpl/assert.hpp>
#include <boost/mpl/int.hpp>
#include <boost/mpl/apply.hpp>
#include <boost/mpl/vector/vector0.hpp>
#include <boost/mpl/vector/vector10.hpp>
#include <boost/array.hpp>
#include <boost/assign/v2/detail/config/check.hpp>
#include <boost/assign/v2/ref/wrapper/copy.hpp>
#include <boost/assign/v2/ref/list.hpp>
#include <boost/type_traits/add_reference.hpp>
#include <libs/assign/v2/test/ref/list.h>

namespace test_assign_v2{
namespace xxx_ref{
namespace xxx_list{

    void test()
    {
        namespace as2 = boost::assign::v2;
        typedef boost::use_default tag_;
        typedef std::string str_;
        {
            typedef int t0_;
            typedef int const t1_;
            typedef const char t2_ [2];
            typedef boost::add_reference<t0_>::type r0_;


/* // TODO see head_holder
#ifndef BOOST_NO_RVALUE_REFERENCES
            typedef int r1_;
#else*/
            typedef boost::add_reference<t1_>::type r1_;
//#endif
            typedef boost::add_reference<t2_>::type r2_;

            t0_ a = 0;
#define B 2
            t2_ c = "c";

            typedef as2::ref::nth_result_of::list<tag_> result_;
            typedef boost::mpl::apply1<result_,
                boost::mpl::vector<r0_, r1_, r2_>
            >::type ar_;
            ar_ ar = as2::ref::list<tag_>( as2::_nil )( a )( B )( c );
            BOOST_ASSIGN_V2_CHECK( &as2::ref::at<0>( ar ) == &a );
            BOOST_ASSIGN_V2_CHECK( as2::ref::at<1>( ar ) == B );
            BOOST_ASSIGN_V2_CHECK( str_( as2::ref::at<2>( ar ) ) == c );
#undef B
        }
        {
            typedef int t_; typedef boost::add_reference<t_>::type r_;
            t_ a = 0;
            t_ b = 1;
            t_ c = 2;
            typedef as2::ref::copy_wrapper<t_>::type w_;
            typedef boost::array<w_,3> arw_;
            typedef as2::ref::nth_result_of::list<tag_> result_;
            typedef boost::mpl::apply1<result_,
                boost::mpl::vector<r_, r_, r_>
            >::type ar_;
            ar_ ar = as2::ref::list<tag_>( as2::_nil )( a )( b )( c );
            arw_ arw;
            assign_array( arw, ar );
            BOOST_ASSIGN_V2_CHECK( &arw[ 0 ].get() == &a );
            BOOST_ASSIGN_V2_CHECK( &arw[ 1 ].get() == &b );
            BOOST_ASSIGN_V2_CHECK( &arw[ 2 ].get() == &c );
        }
        {
               typedef as2::ref::alloc_tag::lazy_alloc tag_;
            typedef as2::ref::list_aux::void_ void_;
            typedef as2::ref::list_aux::nil nil_;
            typedef as2::ref::empty_list<tag_>::type empty_;
            {
                typedef void_ u1_; typedef nil_ l_;
                typedef as2::ref::list_aux::policy_helper3<
                    u1_, l_
                >::type value_;
                BOOST_MPL_ASSERT((boost::is_same<value_, u1_>));
                typedef boost::mpl::apply2<
                    as2::ref::list_aux::policy<tag_>,
                    as2::ref::list_aux::void_,
                    as2::ref::list_aux::nil
                >::type po_; // just see that it compiles
            }
            {
                typedef as2::ref::nth_result_of::list<tag_> result_;
                typedef empty_ l_;
                {
                    typedef int u1_;
                    typedef as2::ref::list_aux::policy_helper3<
                        u1_, l_
                    >::type val_;
                    BOOST_MPL_ASSERT(( boost::is_same<val_, int> ));
                }
                {
                    typedef int const u1_;
                    typedef as2::ref::list_aux::policy_helper3<
                        u1_, l_
                    >::type val_;
                    BOOST_MPL_ASSERT(( boost::is_same<val_, int const> ));
                }
            }

        }
        {
            typedef int t_; typedef boost::add_reference<t_>::type r_;
            t_ a = 0;
            t_ b = 1;
            t_ c = 2;
            typedef as2::ref::alloc_tag::lazy_alloc tag_;
            typedef as2::ref::nth_result_of::list<tag_> result_;
            typedef boost::mpl::apply1<result_,
                boost::mpl::vector<r_, r_, r_>
            >::type ar_;
            ar_ ar = as2::ref::list<tag_>( as2::_nil )( a )( b )( c );
                                     
            BOOST_ASSIGN_V2_CHECK( &ar[ 0 ].get() == &a );
            BOOST_ASSIGN_V2_CHECK( &ar[ 1 ].get() == &b );
            BOOST_ASSIGN_V2_CHECK( &ar[ 2 ].get() == &c );
        }
    }

}// xxx_list
}// xxx_ref
}// test_assign

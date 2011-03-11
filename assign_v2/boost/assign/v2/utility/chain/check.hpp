//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_CHAIN_CHECK_ER_2010_HPP
#define BOOST_ASSIGN_V2_CHAIN_CHECK_ER_2010_HPP
#include <vector>
#include <boost/array.hpp>
#include <boost/assign/v2/detail/config/check.hpp>
#include <boost/assign/v2/ref/wrapper/range_get.hpp>
#include <boost/assign/v2/ref/wrapper/copy.hpp>
#include <boost/assign/v2/utility/chain/pipe.hpp>
#include <boost/assign/v2/utility/chain/use_const.hpp>
#include <boost/mpl/eval_if.hpp>
#include <boost/mpl/identity.hpp>
#include <boost/mpl/assert.hpp>
#include <boost/range/algorithm/copy.hpp>
#include <boost/range/algorithm/equal.hpp>
#include <boost/static_assert.hpp>
#include <boost/type_traits/add_const.hpp>
#include <boost/type_traits/is_same.hpp>

namespace boost{
namespace assign{
namespace v2{
namespace check_chain_aux{

template<typename cr1_, typename cr2_, bool the_value>
void verify_dont_use_const()
{
    typedef v2::chain_aux::dont_use_const<cr1_, cr2_>  dont_use_const_;
    BOOST_STATIC_ASSERT( dont_use_const_::value == the_value );
    typedef typename ::boost::mpl::eval_if_c<
        the_value,
        ::boost::mpl::identity<boost::mpl::void_>, // why?
        v2::result_of::chain<cr1_, cr2_>
    >::type result_;
}

template<bool x, typename T>
struct add_const_if : ::boost::mpl::eval_if_c<
    x, boost::add_const<T>, ::boost::mpl::identity<T>
>{};

template<typename T, bool qual_v1, bool qual_e2, bool qual_v2, bool the_value>
void verify_mpl()
{
    namespace as2 = assign::v2;
    // Value container
    typedef std::vector<T> vec_t_;
    typedef typename add_const_if<qual_v1, vec_t_ >::type vec1_;
    typedef typename as2::ref::copy_wrapper<
        typename add_const_if<qual_e2, T>::type
    >::type w_;
    typedef vec1_                                            r1_; // alias

    // Ref-wrappers
    typedef std::vector<w_> vec_w_;
    typedef typename add_const_if<qual_v2, vec_w_ >::type vec2_;
    // Transformation through range_get (necessary)
    typedef typename ref::result_of::range_get<vec2_>::type r2_;

    {
        typedef r1_       cr1_;
        typedef r2_       cr2_;
        verify_dont_use_const<cr1_, cr2_, the_value>();
        verify_dont_use_const<cr2_, cr1_, the_value>();
    }
    {
        typedef r1_       cr1_;
        typedef r2_ const cr2_;
        verify_dont_use_const<cr1_, cr2_, the_value>();
        verify_dont_use_const<cr2_, cr1_, the_value>();
    }
}

    template<typename T>
    void static_()
    {
        {
            typedef std::vector<T> vec_;
            typedef v2::result_of::chain<vec_, vec_> caller1_;
            typedef typename caller1_::type range1_;
            typedef typename boost::range_reference<range1_>::type ref1_;
            BOOST_MPL_ASSERT(( boost::is_same<ref1_, T&> ));
            typedef v2::result_of::chain<range1_ const, vec_> caller2_;
            typedef typename caller2_::type range2_;
            typedef typename boost::range_reference<range2_>::type ref2_;
            BOOST_MPL_ASSERT(( boost::is_same<ref2_, T&> ));
        }

//      verify_mpl<T, qual_v1, qual_e2 , qual_v2 , the_value>()
        verify_mpl<T, false  , false   , false   , true     >();
        verify_mpl<T, false  , false   , true    , true     >();
        verify_mpl<T, false  , true    , false   , false    >();
        verify_mpl<T, false  , true    , true    , false    >();
        verify_mpl<T, true   , false   , false   , false    >();
        verify_mpl<T, true   , false   , true    , false    >();
        verify_mpl<T, true   , true    , false   , false    >();
        verify_mpl<T, true   , true    , true    , false    >();

    }

    template<typename T>
    void exec()
    {
        namespace as2 = assign::v2;
        typedef std::vector<T> vt_;
        vt_ vt1( 2 ), vt2( 3 ), vt3( 3 );
        vt_ vt;
        {
            // Value containers
            vt.push_back( 1 );        // 1
            vt.push_back( 2 );        // 2

            vt.push_back( -5 );      // 1
            vt.push_back( 4 );      // 2
            vt.push_back( 3 );        // 3

            vt.push_back( -7 );      // 1
            vt.push_back( 8 );      // 2
            vt.push_back( 9 );        // 3
            BOOST_ASSIGN_V2_CHECK( // pre-condition
                vt1.size() + vt2.size() + vt3.size() == vt.size()
            );
        }
        {
            // Value container
            boost::copy(
                vt,
                boost::begin( vt1 | as2::_chain( vt2 ) | as2::_chain( vt3 ) )
            );
            BOOST_ASSIGN_V2_CHECK(
                boost::equal( vt1 | as2::_chain( vt2 ) , vt ) // TODO
            );
        }
        {
            // Mix and Value and Ref-wrapper containers
            typedef typename ref::copy_wrapper<T>::type w_;
            typedef boost::is_reference_wrapper<w_> is_;

            // rese to default values
            vt1.resize( vt1.size() );
            vt3.resize( vt3.size() );

            T x = -1, y = -1, z = -1;
            boost::array<w_, 3> vw2;
            vw2[0].rebind( x );
            vw2[1].rebind( y );
            vw2[2].rebind( z );
            boost::copy(
                vt,
                boost::begin(
                    vt1 | as2::_chain(
                        vw2 | ref::_get  // necessary
                    ) | as2::_chain( vt3 ) )
            );
            {
                BOOST_ASSIGN_V2_CHECK( vt1[0] == vt[0] );
                BOOST_ASSIGN_V2_CHECK( vt1[1] == vt[1] );
                BOOST_ASSIGN_V2_CHECK( x == vt[2] );
                BOOST_ASSIGN_V2_CHECK( y == vt[3] );
                BOOST_ASSIGN_V2_CHECK( z == vt[4] );
                BOOST_ASSIGN_V2_CHECK( vt3[0] == vt[5] );
                BOOST_ASSIGN_V2_CHECK( vt3[1] == vt[6] );
                BOOST_ASSIGN_V2_CHECK( vt3[2] == vt[7] );
            }
            // suppresses warning unused
            if(x && y && z){}
        }
    }// exec

}// check_chain_aux
}// v2
}// assign
}// boost

#endif // BOOST_ASSIGN_V2_CHAIN_CHECK_ER_2010_HPP

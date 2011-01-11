//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_CHAIN_CHECKING_TWIN_VALUES_ER_2010_HPP
#define BOOST_ASSIGN_V2_CHAIN_CHECKING_TWIN_VALUES_ER_2010_HPP
#include <boost/static_assert.hpp>
#include <vector>
#include <boost/mpl/eval_if.hpp>
#include <boost/mpl/identity.hpp>
#include <boost/mpl/assert.hpp>
#include <boost/range/algorithm/copy.hpp>
#include <boost/assign/v2/detail/checking/range.hpp>
#include <boost/assign/v2/ref/detail/unwrap/range.hpp>
#include <boost/assign/v2/ref/wrapper/copy.hpp>
#include <boost/assign/v2/utility/chain/use_lvalue.hpp>
#include <boost/assign/v2/utility/chain/pipe.hpp>

namespace boost{
namespace assign{
namespace v2{
namespace checking{
namespace chain{
namespace twin_values{

template<typename cr1_, typename cr2_, bool the_value>
void verify_use_lvalue()
{
    typedef v2::chain_aux::use_lvalue<cr1_, cr2_>  use_lvalue_; 
    BOOST_STATIC_ASSERT( use_lvalue_::value == the_value ); 
    typedef typename boost::mpl::eval_if_c< 
        the_value, 
        boost::mpl::identity<boost::mpl::void_>, // why?
        v2::result_of::chain<cr1_, cr2_> 
    >::type result_; 
}

template<bool x, typename T>
struct add_const_if : boost::mpl::eval_if_c<
	x, boost::add_const<T>, boost::mpl::identity<T>
>{};

template<typename T, bool qual_v1, bool qual_e2, bool qual_v2, bool the_value>
void verify_mpl()
{
	namespace as2 = assign::v2;
    typedef std::vector<T> vec_t_;
    typedef typename add_const_if<qual_v1, vec_t_ >::type vec1_; 
    typedef typename as2::ref::copy_wrapper<
    	typename add_const_if<qual_e2, T>::type
    >::type w_; 
    typedef std::vector<w_> vec_w_;
    typedef typename add_const_if<qual_v2, vec_w_ >::type vec2_; 

    typedef typename ref::result_of::unwrap_range<vec1_>::type r1_; 
    typedef typename ref::result_of::unwrap_range<vec2_>::type r2_; 

    {  
        typedef r1_       cr1_;
        typedef r2_       cr2_;
        verify_use_lvalue<cr1_, cr2_, the_value>();
    } 
    {  
        typedef r1_       cr1_;
        typedef r2_ const cr2_;
        verify_use_lvalue<cr1_, cr2_, the_value>();
    } 
    {  
        typedef r1_       cr1_;
        typedef r2_ const cr2_;
        verify_use_lvalue<cr1_, cr2_, the_value>();
    } 
    {  
        typedef r1_ const cr1_;
        typedef r2_ const cr2_;
        verify_use_lvalue<cr1_, cr2_, the_value>();
    } 
}

    template<typename T>
    void do_check()
    {
    	// mpl checks
        {
        	typedef std::vector<T> vec_;
        	typedef v2::result_of::chain<vec_,vec_> caller1_;
        	typedef typename caller1_::type range1_;
        	typedef typename boost::range_reference<range1_>::type ref1_;
        	typedef boost::is_same<ref1_, T&> is_same1_;
            BOOST_MPL_ASSERT((is_same1_));
        	typedef v2::result_of::chain<range1_ const,vec_> caller2_;
        	typedef typename caller2_::type range2_;
        	typedef typename boost::range_reference<range2_>::type ref2_;
        	typedef boost::is_same<ref2_, T&> is_same2_;
        	BOOST_MPL_ASSERT((is_same2_));
	    }

//      verify_mpl<T, qual_v1, qual_e2 , qual_v2 , the_value>()
		verify_mpl<T, false  , false   , false   , true     >();
		//verify_mpl<T, false  , false   , true    , true     >();
		//verify_mpl<T, false  , true    , false   , false    >();
		//verify_mpl<T, false  , true    , true    , false    >();
		//verify_mpl<T, true   , false   , false   , false    >();
		//verify_mpl<T, true   , false   , true    , false    >();
		//verify_mpl<T, true   , true    , false   , false    >();
		//verify_mpl<T, true   , true    , true    , false    >();
/*

        // runtime checks
    	{
            typedef std::vector<T> vt_;
            vt_ vt1( 2 ), vt2( 3 ), vt3( 3 );
            vt_ vt;
    		{
                namespace ns = v2::checking::constants;
                vt.push_back( ns::a );
                vt.push_back( ns::b );
                vt.push_back( ns::c );
                vt.push_back( ns::d );
                vt.push_back( ns::e );
                vt.push_back( ns::f );
                vt.push_back( ns::g );
                vt.push_back( ns::h );
                {
                    using namespace adaptor;
                    boost::copy(
                        vt,
                        boost::begin( vt1 | _chain( vt2 ) | _chain( vt3 ))
                    );
                }
                {
                	using namespace adaptor;
                	typedef v2::container_tag::range tag_;
                    v2::checking::do_check(
                    	tag_(),
                        vt1 | _chain( vt2 ) | _chain( vt3 )
                    );
                }
            }
            {
	            T a1 = -1, b1 = -1, c1 = -1, d1 = -1, 
                  e1 = -1, f1 = -1, g1 = -1, h1 = -1;

                typedef typename ref::copy_wrapper<T>::type w_;
    	        typedef boost::is_reference_wrapper<w_> is_;

                boost::array<int, 3> vw2;
                std::vector<int> vw1( 2 );
                std::vector<int> vw3( 3 );
				using namespace adaptor;
                boost::copy(
                	vt,
                    boost::begin( vw1 | _chain( vw2 ) | _chain( vw3 ) )
                );
                {
                	typedef v2::container_tag::range tag_;
                    v2::checking::do_check(
                    	tag_(),
                        vw1 | _chain( vw2 ) | _chain( vw3 )
                    );
                }
                // suppresses warning unused
                if(a1 && b1 && c1 && d1 && e1 && f1 && g1 && h1){} 
			}
        }// runtime checks
*/
    }// do_check

}// twin_values
}// chain
}// checking
}// v2
}// assign
}// boost

#endif

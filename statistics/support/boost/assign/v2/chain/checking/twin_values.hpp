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
#include <boost/range/algorithm/copy.hpp>
#include <boost/assign/v2/detail/checking/range.hpp>
#include <boost/assign/v2/ref/detail/unwrap/range.hpp>
#include <boost/assign/v2/ref/wrapper/copy.hpp>
#include <boost/assign/v2/chain/traits/use_lvalue.hpp>
#include <boost/assign/v2/chain.hpp>

namespace boost{
namespace assign{
namespace v2{
namespace checking{
namespace chain{
namespace twin_values{

#define BOOST_ASSIGN_V2_CHAIN_CHECKING_mpl_inner( the_value )\
    typedef v2::chain_traits::use_lvalue<cr1_, cr2_ >  use_lvalue_; \
    BOOST_STATIC_ASSERT( use_lvalue_::value == the_value ); \
    typedef typename boost::mpl::eval_if_c< \
        the_value, \
        boost::mpl::identity<boost::mpl::void_>, \
        v2::result_of::chain<cr1_, cr2_> \
    >::type result_; \
/**/

#define BOOST_ASSIGN_V2_CHAIN_CHECKING_mpl(qual_v1, qual_e2, qual_v2, \
                                                                  the_value )\
   { \
        typedef std::vector<T> qual_v1 vec1_; \
		typedef typename assign::v2::ref::type_traits::copy_wrapper< \
        	T qual_e2>::type w_; \
        typedef std::vector<w_> qual_v2 vec2_; \
		typedef typename ref::result_of::unwrap_range<vec1_>::type r1_; \
		typedef typename ref::result_of::unwrap_range<vec2_>::type r2_; \
        {  \
            typedef r1_       cr1_;\
            typedef r2_       cr2_;\
            BOOST_ASSIGN_V2_CHAIN_CHECKING_mpl_inner(the_value) \
        } \
        {  \
            typedef r1_       cr1_;\
            typedef r2_       cr2_;\
            BOOST_ASSIGN_V2_CHAIN_CHECKING_mpl_inner(the_value) \
        } \
        {  \
            typedef r1_       cr1_;\
            typedef r2_ const cr2_;\
            BOOST_ASSIGN_V2_CHAIN_CHECKING_mpl_inner(the_value) \
        } \
        {  \
            typedef r1_ const cr1_;\
            typedef r2_       cr2_;\
            BOOST_ASSIGN_V2_CHAIN_CHECKING_mpl_inner(the_value) \
        } \
        {  \
            typedef r1_ const cr1_;\
            typedef r2_ const cr2_;\
            BOOST_ASSIGN_V2_CHAIN_CHECKING_mpl_inner(the_value) \
        } \
    } \
   /**/

    template<typename T>
    void do_check()
    {
    	// mpl checks
        {
        	typedef std::vector<T> vec_;
        	typedef v2::result_of::chain<vec_,vec_> caller1_;
        	typedef typename caller1_::type range1_;
        	typedef typename boost::range_reference<range1_>::type ref1_;
        	typedef boost::is_same<ref1_,T&> is_same1_;
        	BOOST_STATIC_ASSERT(is_same1_::value);
        	typedef v2::result_of::chain<range1_ const,vec_> caller2_;
        	typedef typename caller2_::type range2_;
        	typedef typename boost::range_reference<range2_>::type ref2_;
        	typedef boost::is_same<ref2_,T &> is_same2_;
        	BOOST_STATIC_ASSERT(is_same2_::value);
	    }
//                                (qual_v1, qual_e2 , qual_v2 , the_value)
BOOST_ASSIGN_V2_CHAIN_CHECKING_mpl(       ,         ,         , true     )
BOOST_ASSIGN_V2_CHAIN_CHECKING_mpl(       ,         , const   , true     )
BOOST_ASSIGN_V2_CHAIN_CHECKING_mpl(       , const   ,         , false    )
BOOST_ASSIGN_V2_CHAIN_CHECKING_mpl(       , const   , const   , false    )
BOOST_ASSIGN_V2_CHAIN_CHECKING_mpl( const ,         ,         , false    )
BOOST_ASSIGN_V2_CHAIN_CHECKING_mpl( const ,         , const   , false    )
BOOST_ASSIGN_V2_CHAIN_CHECKING_mpl( const , const   ,         , false    )
BOOST_ASSIGN_V2_CHAIN_CHECKING_mpl( const , const   , const   , false    )

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
	            T a1 , b1, c1, d1, e1, f1, g1, h1;

	            typedef typename ref::type_traits::copy_wrapper<T>::type w_;
    	        typedef boost::is_reference_wrapper<w_> is_;

                boost::array<int, 3> vw2;//, vw3;
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
                // only to avoid warning unused
                a1 + b1 + c1 + d1 + e1 + g1 + h1; 
			}
        }// runtime checks
    }// do_check

}// twin_values
}// chain
}// checking
}// v2
}// assign
}// boost

#undef BOOST_ASSIGN_V2_CHAIN_CHECKING_run
#undef BOOST_ASSIGN_V2_CHAIN_CHECKING_mpl
#undef BOOST_ASSIGN_V2_CHAIN_CHECKING_mpl_inner

#endif

//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_DETAIL_RELATIONAL_CRTP_ER_2010_HPP
#define BOOST_ASSIGN_V2_DETAIL_RELATIONAL_CRTP_ER_2010_HPP
#include <boost/utility/enable_if.hpp>
#include <boost/type_traits/is_base_of.hpp> 

namespace boost{
namespace assign{
namespace v2{
namespace relational_aux{

// A solution with 3 overloads was proposed here.
// http://groups.google.com/group/comp.lang.c++.moderated/browse_thread/thread/389d8fe278ef0b13#
// 2 overloads seem to suffice, however, as implemented below.

	struct base{};

	// D must implement:
    //	template<typename T> bool equal_to(D const&, const T&);
    // 	template<typename T> bool less_than(D const&, const T&);
	template<typename D>
	struct crtp{// : base{
    
    	 D const& derived()const{ return static_cast<D const&>(*this); }
         
    };

	template<typename T>
    struct is_relational : boost::is_base_of<crtp<T>,T>{};

	template< typename A ,typename B>
    typename boost::disable_if<
		relational_aux::is_relational<A>,
//    	boost::is_base_of<base,A>,
        bool
    >::type
    operator==( A const& a, crtp< B > const& b) 
	{ 
		return b.derived().equal_to( a );
	}

	template< typename A ,typename B>
	bool operator==( crtp< A > const& a, B const& b )
	{ 
		return b == a; 
	} 	

	template< typename A ,typename B>
    typename boost::disable_if<
//    	boost::is_base_of<base,A>,
		relational_aux::is_relational<A>,
        bool
    >::type
    operator<( A const& a, crtp< B > const& b) 
	{ 
		return ! ( (a == b) || (b < a) );
	}

	template< typename A ,typename B>
	bool operator<( crtp< A > const& a, B const& b )
	{ 
		return a.derived().less_than( b ); 
	} 	

	// Deduced

#define MACRO(symb,expr)\
	template< typename A ,typename B> \
    typename boost::disable_if< \
    	boost::is_base_of<base,A>, \
        bool \
    >::type \
    operator symb ( A const& a, crtp< B > const& b) \
	{ \
		return expr; \
	} \
 	\
	template< typename A ,typename B> \
	bool operator symb ( crtp< A > const& a, B const& b ) \
	{ \
		return expr; \
	} \
/**/

	MACRO( !=,  !(a == b)  )
	MACRO( <=,  (a < b) || (a == b)  )
	MACRO( >,   !(a <= b) )
	MACRO( >=,  (a > b) || (a == b) )

#undef MACRO
    
}// relational_aux
}// v2
}// assign
}// boost

#endif

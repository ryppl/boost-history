//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_DETAIL_RELATIONAL_OP_CRTP_ER_2010_HPP
#define BOOST_ASSIGN_V2_DETAIL_RELATIONAL_OP_CRTP_ER_2010_HPP
#include <boost/utility/enable_if.hpp>
#include <boost/type_traits/is_base_of.hpp> 

namespace boost{
namespace assign{
namespace v2{
namespace relational_op_aux{

// A solution with 3 overloads was proposed here.
// http://groups.google.com/group/comp.lang.c++.moderated/browse_thread/thread/389d8fe278ef0b13#
// 2 overloads seem to suffice, however, as implemented below.

	struct base{};

	// D must implement:
    //	template<typename T> bool equal_to(const T&)const;
    // 	template<typename T> bool less_than(const T&)const;
	template<typename D>
	struct crtp : base{
    
    	 D const& derived()const{ return static_cast<D const&>(*this); }
         
    };

	template< typename A ,typename B>
    typename boost::disable_if<
    	boost::is_base_of<base,A>,
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
    	boost::is_base_of<base,A>,
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

#define op(symb,expr)\
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

	op( !=,  !(a == b)  )
	op( <=,  (a < b) || (a == b)  )
	op( >,   !(a <= b) )
	op( >=,  (a > b) || (a == b) )

#undef op
    
}// relational_op_aux
}// v2
}// assign
}// boost

#endif

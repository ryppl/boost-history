//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_PUT_RANGE_CONVERT_ER_2010_HPP
#define BOOST_ASSIGN_V2_PUT_RANGE_CONVERT_ER_2010_HPP
#include <boost/assign/v2/put/range/adaptor.hpp>

namespace boost{
namespace assign{
namespace v2{
namespace put_convert_aux
{
	template<typename To>
	struct adaptor
    {
    	adaptor(){}
    };

	template<typename From,typename To>
	To operator|(From& from, put_convert_aux::adaptor<To> const& dummy)
    {
    	To to;
    	return to | v2::adaptor::_put_range( from );
    }

}// convert_aux
namespace adaptor{

	template<typename To>
	put_convert_aux::adaptor<To> put_convert()
    {
    	return put_convert_aux::adaptor<To>();
    }

}// adaptor

// TODO remove
/*
	template<typename To,typename From>
    To convert(From const& from){
    	return from | adaptor::put_convert<To>();
    }
*/
}// v2
}// assign
}// boost

#endif

// TODO remove
// ------ CLASS MEMBER ---- //

#ifndef BOOST_ASSIGN_V2_CONVERT_CONVERT_MF
#define BOOST_ASSIGN_V2_CONVERT_CONVERT_MF \
   template<typename To> \
   To convert()const \
   { \
		namespace ns = boost::assign::v2; \
       	return (*this) | ns::adaptor::put_convert<To>(); \
   } \
/**/
#endif

#ifndef BOOST_ASSIGN_V2_CONVERT_OPERATOR_MF
#define BOOST_ASSIGN_V2_CONVERT_OPERATOR_MF \
   template<typename To> \
   operator To ()const \
   { \
		namespace ns = boost::assign::v2; \
       	return (*this) | ns::adaptor::put_convert<To>(); \
   } \
/**/

#endif

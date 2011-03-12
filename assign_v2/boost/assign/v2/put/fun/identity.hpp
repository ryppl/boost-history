//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_PUT_FUN_IDENTITY_ER_2010_HPP
#define BOOST_ASSIGN_V2_PUT_FUN_IDENTITY_ER_2010_HPP
#include <boost/lambda/lambda.hpp>
#include <boost/typeof/typeof.hpp>
#include <boost/assign/v2/put/fun/modulo.hpp>

namespace boost{
namespace assign{
namespace v2{
namespace put_aux{

	typedef BOOST_TYPEOF( ( _fun = ::boost::lambda::_1 ) ) result_of_identity;

}// put_aux
namespace {
	
    put_aux::result_of_identity const _identity 
    	= ( _fun = ::boost::lambda::_1 );
}
}// v2
}// assign
}// boost

#endif // BOOST_ASSIGN_V2_PUT_FUN_IDENTITY_ER_2010_HPP

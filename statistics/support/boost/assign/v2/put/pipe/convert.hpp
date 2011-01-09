//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_PUT_PIPE_CONVERT_ER_2010_HPP
#define BOOST_ASSIGN_V2_PUT_PIPE_CONVERT_ER_2010_HPP
#include <boost/assign/v2/put/pipe/range.hpp>

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
    	return to | v2::_put_range( from );
    }

}// convert_aux

	template<typename To>
	put_convert_aux::adaptor<To> put_convert()
    {
    	return put_convert_aux::adaptor<To>();
    }

}// v2
}// assign
}// boost

#endif

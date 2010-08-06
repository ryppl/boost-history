//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_PUT_RANGE_MODIFIER_COPY_ER_2010_HPP
#define BOOST_ASSIGN_V2_PUT_RANGE_MODIFIER_COPY_ER_2010_HPP
#include <string>
#include <stdexcept>
#include <boost/throw_exception.hpp>
#include <boost/range/begin.hpp>
#include <boost/range/size.hpp>
#include <boost/range/algorithm/copy.hpp>

namespace boost{
namespace assign{
namespace v2{
namespace put_range_tag
{
	struct copy{};
}// convert_tag
namespace put_range_aux{

   	template<typename From,typename To>
   	void put_range (put_range_tag::copy, From const& from, To& to)
   	{
		if(boost::size( to ) < boost::size( from ) ){
        	typedef std::string str_;
        	const str_ str = 
            	str_("convert<to>(")
            	+ "convert_tag::copy, from)"
                + "To::size() < boost::size( from )";
        	std::out_of_range e( str );
            boost::throw_exception(e);
        }
	    boost::copy( from, boost::begin( to ) );
   	}
    
}// put_range_aux{
}// v2
}// assign
}// boost

#endif

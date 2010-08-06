//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_PUT_RANGE_MODIFIER_RANGE_ER_2010_HPP
#define BOOST_ASSIGN_V2_PUT_RANGE_MODIFIER_RANGE_ER_2010_HPP
#include <boost/range/begin.hpp>
#include <boost/range/end.hpp>

namespace boost{
namespace assign{
namespace v2{
namespace put_range_tag
{
	struct range{};
}// convert_tag
namespace put_range_aux{

   	template<typename From,typename To>
   	void put_range(
    	put_range_tag::range, 
        From const& from,
        To& to
    )
   	{
    	to = To( boost::begin( from ), boost::end( from ) );
   	}
    
}// put_range_aux
}// v2
}// assign
}// boost

#endif

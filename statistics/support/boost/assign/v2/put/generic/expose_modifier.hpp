//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_PUT_GENERIC_HOLD_MODIFIER_ER_2010_HPP
#define BOOST_ASSIGN_V2_PUT_GENERIC_HOLD_MODIFIER_ER_2010_HPP
#include <boost/assign/v2/put/modifier/def.hpp>

namespace boost{
namespace assign{ 
namespace v2{
namespace put_aux{

	template<typename Tag>
	struct expose_modifier{
    
    	typedef Tag modifier_tag;
		typedef put_aux::modifier<Tag> modifier_type;

    	expose_modifier(){}
        expose_modifier(modifier_type const& m) : modifier( m ){}

		typedef put_aux::modifier<Tag> modifier_;

		modifier_type modifier;
    
    };

}// put_aux
}// v2
}// assign
}// boost

#endif

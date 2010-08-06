//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_PUT_MODIFIER_DEF_ER_2010_HPP
#define BOOST_ASSIGN_V2_PUT_MODIFIER_DEF_ER_2010_HPP

namespace boost{
namespace assign{ 
namespace v2{
namespace put_tag{
	// add as necessary
}
namespace put_aux{

	template<typename Tag>
    struct modifier{
		    	
    	// Specialize on Tag:
    	//	template<typename V,typename T>
        //	void impl(V& v,T& t)const;

		// WARNING: some modifiers such as modifier<put_tag::array> have an 
        // internal state that affects impl()'s working. 
        
    };

}// put_aux
}// v2
}// assign
}// boost

#endif

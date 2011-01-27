//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_PUT_GENERIC_BASE_ER_2010_HPP
#define BOOST_ASSIGN_V2_PUT_GENERIC_BASE_ER_2010_HPP

namespace boost{
namespace assign{
namespace v2{
namespace put_aux{


	struct put_base{
    
    	// This base is needed to be able to distinguish objects that serves as
        // parameter buffers for a later use, and those that actually need to
        // be modified by a parameter. The latter must inherit publicly from
        // put_base.
	};

}// put_aux
}// v2
}// assign
}// boost

#endif

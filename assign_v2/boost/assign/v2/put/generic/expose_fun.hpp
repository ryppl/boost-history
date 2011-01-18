//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_PUT_GENERIC_HOLD_FUN_ER_2010_HPP
#define BOOST_ASSIGN_V2_PUT_GENERIC_HOLD_FUN_ER_2010_HPP

namespace boost{
namespace assign{ 
namespace v2{
namespace put_aux{

	template<typename F>
	struct expose_fun{
    
		typedef F fun_type;

    	expose_fun(){}
        expose_fun(fun_type const& f) : fun( f ){}

		fun_type fun;
    
    };


}// put_aux
}// v2
}// assign
}// boost

#endif

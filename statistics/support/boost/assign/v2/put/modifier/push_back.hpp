//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_PUT_MODIFIER_PUSH_BACK_ER_2010_HPP
#define BOOST_ASSIGN_V2_PUT_MODIFIER_PUSH_BACK_ER_2010_HPP
#include <boost/utility/enable_if.hpp>
#include <boost/assign/v2/put/modifier/def.hpp>

namespace boost{
namespace assign{ 
namespace v2{
namespace put_tag{ struct push_back{}; }
namespace put_aux{

	template<>
    struct modifier<v2::put_tag::push_back>{
    
    	modifier(){}

    	template<typename V,typename T>
       void impl(V& v,T& t)const{ v.push_back( t ); }        

    	template<typename V,typename T>
        void impl(V& v,T* t)const{ v.push_back( t ); }
        
    };

}// put_aux
}// v2
}// assign
}// boost

#endif
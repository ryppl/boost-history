//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_DETAIL_FUNCTOR_POST_INCREMENT_ER_2010_HPP
#define BOOST_ASSIGN_V2_DETAIL_FUNCTOR_POST_INCREMENT_ER_2010_HPP
#include <cstddef>

namespace boost{
namespace assign{
namespace v2{
namespace functor_aux{

	template<typename T = std::size_t>
	struct post_increment
    {
        typedef T result_type;
    
    	post_increment():i_(0){}
    	post_increment(T i):i_( i ){}

		result_type operator()(const T& i)const{ return this->i++; }
		result_type operator()()const{ return this->i_++; }
        
        protected:
        
        mutable result_type i_;
    };
    
}// functor_aux
namespace{
	functor_aux::post_increment<> const _post_increment 
    	= functor_aux::post_increment<>();
}
}// v2
}// assign
}// boost

#endif

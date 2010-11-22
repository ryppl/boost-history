//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_REF_STATIC_ARRAY_ALLOC_FAST_ALLOC_ER_2010_HPP
#define BOOST_ASSIGN_V2_REF_STATIC_ARRAY_ALLOC_FAST_ALLOC_ER_2010_HPP
#include <boost/assign/v2/ref/wrapper.hpp>
#include <boost/assign/v2/ref/fusion/assign_array.hpp>
#include <boost/assign/v2/ref/static_array/interface.hpp>
#include <boost/assign/v2/ref/static_array/alloc/fast_alloc_fwd.hpp>
 
namespace boost{
namespace assign{
namespace v2{
namespace ref{
namespace alloc_tag{
	struct fast_alloc{};
}// alloc_tag
namespace static_array_aux{ 

    template<std::size_t N,typename Tag, typename T>
    class fast_alloc 
    	: public static_array_aux::interface<
   		N, Tag, T, fast_alloc<N,Tag,T> 
	>
    {

    	typedef Tag assign_tag_;
        typedef typename ref::wrapper<assign_tag_,T> wrapper_;
        typedef boost::array<wrapper_,N> array_;
                
        public:

        fast_alloc(){}	

        template<typename L>
        fast_alloc(const L& l){
            assign_array(
            	this->array,
                l
            );	
        }	
        
        array_& impl(){ 
            return this->array;
        }

        array_ const & impl()const{ 
            return this->array;
        }
        
        protected:
        mutable array_ array;
        
    };

}// static_array_aux
}// ref
}// v2
}// assign
}// boost

#endif

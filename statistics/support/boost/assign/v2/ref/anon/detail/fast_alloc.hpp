//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
/*
#ifndef BOOST_ASSIGN_V2_REF_CONTAINER_STATIC_ARRAY_DETAIL_FAST_ALLOC_ER_2010_HPP
#define BOOST_ASSIGN_V2_REF_CONTAINER_STATIC_ARRAY_DETAIL_FAST_ALLOC_ER_2010_HPP
#include <boost/assign/v2/ref/wrapper/wrapper.hpp>
#include <boost/assign/v2/ref/detail/fusion_list/assign_array.hpp>
#include <boost/assign/v2/ref/anon/detail/interface.hpp>
#include <boost/assign/v2/ref/anon/detail/fast_alloc_fwd.hpp>
 
namespace boost{
namespace assign{
namespace v2{
namespace ref{
namespace alloc_tag{
	struct fast_alloc{};
}// alloc_tag
namespace anon_aux{ 

    template<std::size_t N,typename Tag, typename T>
    class fast_alloc 
    	: public anon_aux::interface<
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
            fusion_list_aux::assign_array(
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

}// anon_aux
}// ref
}// v2
}// assign
}// boost

#endif
*/
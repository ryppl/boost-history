//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_REF_ARRAY_ALLOC_INSTANT_ER_2010_HPP
#define BOOST_ASSIGN_V2_REF_ARRAY_ALLOC_INSTANT_ER_2010_HPP
#include <boost/assign/v2/ref/wrapper.hpp>
#include <boost/assign/v2/ref/array/interface.hpp>
#include <boost/assign/v2/ref/array/alloc/instant_fwd.hpp>
#include <boost/assign/v2/ref/array/impl_traits.hpp>
 
namespace boost{
namespace assign{
namespace v2{
namespace ref{
namespace array_aux{ 

//    template<size_type N, typename Tag, typename T>
    template<size_type N, typename T>
    class instant_alloc : public array_aux::interface<
		typename array_aux::impl_traits<N, T>::array_type,
//        instant_alloc<N, Tag, T> 
      instant_alloc<N, T> 
	>
    {
		typedef typename array_aux::impl_traits<N, T>::array_type impl_;
                
        public:

        instant_alloc(){}	

        template<typename L>
        instant_alloc(const L& l){
            assign_array( 
            	this->array,
                l
            );	
        }	
        
        impl_& impl(){ 
            return this->array;
        }

        impl_ const & impl()const{ 
            return this->array;
        }
        
        protected:
        mutable impl_ array;
        
    };

}// array_aux
}// ref
}// v2
}// assign
}// boost

#endif

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
#ifndef BOOST_ASSIGN_V2_REF_CONTAINER_STATIC_ARRAY_DETAIL_LAZY_ALLOC_ER_2010_HPP
#define BOOST_ASSIGN_V2_REF_CONTAINER_STATIC_ARRAY_DETAIL_LAZY_ALLOC_ER_2010_HPP
#include <boost/shared_ptr.hpp>
#include <boost/assign/v2/ref/wrapper/wrapper.hpp>
#include <boost/assign/v2/ref/detail/fusion_list/assign_array.hpp>
#include <boost/assign/v2/ref/anon/detail/lazy_alloc_fwd.hpp>
#include <boost/assign/v2/ref/anon/detail/interface.hpp>
 
namespace boost{
namespace assign{
namespace v2{
namespace ref{
namespace alloc_tag{
	struct lazy_alloc{};
}// alloc_tag
namespace anon_aux{ 
                  
   	template<std::size_t N,typename Tag, typename T, typename D>
   	class lazy_alloc : public anon_aux::interface<
   		N, Tag, T, lazy_alloc<N,Tag,T,D> 
	>
    {
    	typedef Tag assign_tag_;
        typedef typename ref::wrapper<assign_tag_,T> wrapper_;
        typedef boost::array<wrapper_,N> array_;

		typedef boost::mpl::int_<N> size_;
                
        void alloc_if()const{
            if(!this->ptr){
               return this->alloc();
            }
        }

        void alloc()const{ 
            this->ptr = smart_ptr_(new array_);
            assign_array(
            	*this->ptr,
                static_cast<const D&>(*this)
            );	
        }

		typedef anon_aux::interface<
   			N, Tag, T, lazy_alloc 
		> super_t;
		
        public:

        array_& impl(){ 
            this->alloc_if();
            return (*this->ptr);
        }

        array_ const & impl()const{ 
            this->alloc_if();
            return (*this->ptr);
        }
        
        using super_t::operator[];
        
        private:
        
        typedef boost::shared_ptr<array_> smart_ptr_;
        mutable smart_ptr_ ptr;

    };

}// anon_aux
}// ref 
}// v2    
}// assign
}// boost

#endif
*/
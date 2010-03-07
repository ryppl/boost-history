//////////////////////////////////////////////////////////////////////////////
// assign::detail::array::policy.hpp                                        //
//                                                                          //
//  (C) Copyright 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_AUTO_SIZE_DETAIL_ARRAY_POLICY_ER_2010_HPP
#define BOOST_ASSIGN_AUTO_SIZE_DETAIL_ARRAY_POLICY_ER_2010_HPP
#include <boost/shared_ptr.hpp>
#include <boost/assign/auto_size/array/interface.hpp>
#include <boost/assign/auto_size/array/ref.hpp>
#include <boost/assign/auto_size/detail/policy.hpp>

namespace boost{
namespace assign{
namespace detail{
namespace auto_size{

    // tag::lazy_array designates a policy for auto_size::expr<>, that has the 
    // functionality of array_interface<>, and postones allocation until it is 
    // necessary. It is therefore suitable as the result of statements such as:
    //  fun(a)(b)(c);

    namespace tag{
    	struct lazy_array{};
    }

    template<typename T,int N,template<typename> class R,typename D>
    class lazy_array; 

    template<typename E> struct expr_size;
    template<typename E> struct expr_elem;

    template<>
    struct policy<tag::lazy_array>
    {
        template<typename E,template<typename> class R>
        struct apply{
        	typedef typename expr_size<E>::type size_;
            typedef typename expr_elem<E>::type elem_;
            typedef lazy_array<elem_,size_::value,R,E> type;
        };
	};        
 
 	// --> perhaps not needed
    template<typename E,typename T,int N,
        template<typename> class R,typename P,bool F>
    class expr;         

    template<typename A,typename E,typename T,int N,
    	template<typename> class R,typename P,bool F>
    void write_to_array(A& a,const expr<E,T,N,R,P,F>& e);
	// <-
                  
   template<typename T,int N,template<typename> class R,typename D>
   class lazy_array 
    	: public array_interface<T,N,R,lazy_array<T,N,R,D> >
    {

        typedef typename auto_size::ref_array<T,N,R>::type ref_array_;
                
        void alloc_if()const{
            if(!this->ptr){
               return this->alloc();
            }
        }

        void alloc()const{ 
            this->ptr = smart_ptr_(new ref_array_);
            write_to_array(*this->ptr,static_cast<const D&>(*this));		
        }
		
        public:

        ref_array_& ref_array_impl(){ 
            this->alloc_if();
            return (*this->ptr);
        }

        const ref_array_& ref_array_impl()const{ 
            this->alloc_if();
            return (*this->ptr);
        }
        
        private:
        typedef boost::shared_ptr<ref_array_> smart_ptr_;
        // Only the last of N expressions needs to instantiate an array, 
        // hence a pointer.
        mutable smart_ptr_ ptr;

    };

}// auto_size  
}// detail      
}// assign
}// boost

#endif

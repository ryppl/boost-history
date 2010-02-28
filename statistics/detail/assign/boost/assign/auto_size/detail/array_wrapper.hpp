//////////////////////////////////////////////////////////////////////////////
// assign::detail::array_wrapper.hpp                                        //
//                                                                          //
//  (C) Copyright 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_AUTO_SIZE_DETAIL_ARRAY_WRAPPER_ER_2010_HPP
#define BOOST_ASSIGN_AUTO_SIZE_DETAIL_ARRAY_WRAPPER_ER_2010_HPP
#include <boost/shared_ptr.hpp>
#include <boost/assign/auto_size/detail/array_interface.hpp>

namespace boost{
namespace assign{
namespace detail{
namespace auto_size{
 
 	// Exposes the boost::array interface. This is needed for csv support.
    template<typename T,int N,template<typename> class Ref>
    class array_wrapper 
    	: public array_interface<T,N,Ref,array_wrapper<T,N,Ref> >
    {

        typedef typename ref_array<T,N,Ref>::type ref_array_;
        typedef boost::shared_ptr<ref_array_> smart_ptr_;
                
        public:

		array_wrapper(smart_ptr_ arg)
        	:ptr(arg){}
		
        ref_array_& ref_array_impl(){ 
            return (*this->ptr);
        }

        const ref_array_& ref_array_impl()const{ 
            return (*this->ptr);
        }
        
        private:
        mutable smart_ptr_ ptr;

    };

}// auto_size  
}// detail      
}// assign
}// boost

#endif


//////////////////////////////////////////////////////////////////////////////
// assign::detail::array::wrapper.hpp                                       //
//                                                                          //
//  (C) Copyright 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_AUTO_SIZE_DETAIL_ARRAY_WRAPPER_ER_2010_HPP
#define BOOST_ASSIGN_AUTO_SIZE_DETAIL_ARRAY_WRAPPER_ER_2010_HPP
#include <boost/shared_ptr.hpp>
#include <boost/assign/auto_size/array/interface.hpp>
#include <boost/assign/auto_size/detail/auto_size.hpp>

namespace boost{
namespace assign{
namespace detail{
namespace auto_size{
 
    // Used by csv.hpp
    template<typename T,int N,template<typename> class Ref>
    class array_wrapper 
    	: public array_interface<T,N,Ref,array_wrapper<T,N,Ref> >
    {

        typedef typename ref_array<T,N,Ref>::type ref_array_;
        typedef boost::shared_ptr<ref_array_> smart_ptr_;
                
        public:

        array_wrapper(){}		
		
        ref_array_& ref_array_impl(){ 
            return this->ref_array;
        }

        const ref_array_& ref_array_impl()const{ 
            return this->ref_array;
        }
        
        template<typename E>
        void initialize(const E& coll)const{
        	write_to_array(this->ref_array,coll);
        }
        
        private:

        mutable ref_array_ ref_array;

    };

}// auto_size  
}// detail      
}// assign
}// boost

#endif


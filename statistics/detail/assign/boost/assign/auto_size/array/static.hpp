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
#include <boost/assign/auto_size/array/interface.hpp>
#include <boost/assign/auto_size/array/ref.hpp>
#include <boost/assign/auto_size/detail/expr.hpp> // needed for write_to_array

namespace boost{
namespace assign{
namespace detail{
namespace auto_size{
 
    namespace tag{
        struct static_array{};
    }
 
    // tag::static_array designates a data-structure that has the functonality 
    // of array_interface<> and is allocated at construction. It is recommended
    // as the result of functions that have the csv form :
    //  fun(a,b,c)
    // Unlike lazy_array, it is not a suitable policy for auto_size::expr<>. 
    // This is desirable as otherwise the result from fun(a,b,c) would be much
    // costlier to construct (this has already been tested).

    template<typename T,int N,template<typename> class R>
    class static_array; 

    template<>
    struct policy<tag::static_array>
    {
        template<typename T,int N,template<typename> class R>
        struct apply{
            typedef static_array<T,N,R> type;
        };
	};        
 

    template<typename T,int N,template<typename> class R>
    class static_array 
    	: public array_interface<T,N,R,static_array<T,N,R> >
    {

        typedef typename auto_size::ref_array<T,N,R>::type ref_array_;
                
        public:

        static_array(){}	

		template<typename E>
        static_array(const E& coll){
            write_to_array(this->ref_array,coll);
        }	
        
        ref_array_& ref_array_impl(){ 
            return this->ref_array;
        }

        const ref_array_& ref_array_impl()const{ 
            return this->ref_array;
        }
        
        private:
        mutable ref_array_ ref_array;
        
    };


}// auto_size  
}// detail      
}// assign
}// boost

#endif


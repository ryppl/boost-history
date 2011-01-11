//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_REF_WRAPPER_COPY_ER_2010_HPP
#define BOOST_ASSIGN_V2_REF_WRAPPER_COPY_ER_2010_HPP
#include <algorithm>
#include <boost/mpl/bool.hpp>
#include <boost/config.hpp>
#include <boost/call_traits.hpp>
#include <boost/type_traits/is_const.hpp>
#include <boost/type_traits/remove_cv.hpp>
#include <boost/assign/v2/ref/wrapper/framework.hpp>
#include <boost/assign/v2/ref/wrapper/crtp.hpp>

namespace boost{
namespace assign{ 
namespace v2{
namespace ref{
namespace assign_tag{ struct copy{}; }
namespace assign_copy{ typedef assign_tag::copy assign_tag_; }

    template< class T >
    struct wrapper<ref::assign_tag::copy, T> 
    : 
    	public ref::wrapper_crtp< wrapper<ref::assign_tag::copy, T>, T>
    {
        typedef T type; 
        BOOST_STATIC_CONSTANT( 
            bool, 
            is_const = boost::is_const<T>::value 
        ); 
        
        wrapper(){} 
        
        explicit wrapper( T& r ) : ref_(&r) 
        { } 

        void rebind(T& r ) 
    	{ 
            this->ref_ = &r; 
    	} 
        
        typedef T& unwrap_type;
        T& unwrap() const 
        { 
            return *this->ref_; 
        } 

        T* get_pointer() const { return this->ref_; } 

        using wrapper_crtp<wrapper,T>::operator=; 

        void assign(typename boost::call_traits<T>::param_type r )
        {
            this->unwrap() = r;
        }

        protected:
		friend class wrapper_crtp<wrapper, T>; 

        T* ref_; 

        typedef ref::assign_tag::copy assign_tag;

        void swap( wrapper& r )
        {
            std::swap( this->unwrap(), r.unwrap() );
        }

    };
    
    template<typename T>
    struct copy_wrapper{ typedef ref::wrapper<assign_tag::copy,T> type; };
        
}// ref
}// v2
}// assign
}// boost

#endif

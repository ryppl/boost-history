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
#include <boost/call_traits.hpp>
#include <boost/type_traits/remove_cv.hpp>
#include <boost/assign/v2/ref/wrapper/framework.hpp>
#include <boost/assign/v2/ref/wrapper/base.hpp>
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
        BOOST_ASSIGN_V2_REF_WRAPPER_BASE( wrapper )     

        typedef ref::assign_tag::copy assign_tag;

        void assign(typename boost::call_traits<T>::param_type r )
        {
            this->unwrap() = r;
        }

        void swap( wrapper& r )
        {
            std::swap( this->unwrap(), r.unwrap() );
        }

    };
    
namespace type_traits{
    
    template<typename T>
    struct copy_wrapper{ typedef ref::wrapper<assign_tag::copy,T> type; };
    
    template<typename T>
    struct does_assign_copy_impl : boost::mpl::false_{};
    
    template<typename T>
    struct does_assign_copy_impl< 	
    	ref::wrapper<assign_tag::copy, T> 
    > : boost::mpl::true_{};
    
    template<typename T>
    struct does_assign_copy : does_assign_copy_impl<
    	typename boost::remove_cv<T>::type
    >{};
    
}// type_traits
}// ref
}// v2
}// assign
}// boost

#endif

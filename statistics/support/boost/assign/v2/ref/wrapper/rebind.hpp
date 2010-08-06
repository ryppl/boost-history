//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_REF_WRAPPER_REBIND_ER_2010_HPP
#define BOOST_ASSIGN_V2_REF_WRAPPER_REBIND_ER_2010_HPP
#include <algorithm>
//#include <boost/call_traits.hpp>
#include <boost/assign/v2/ref/wrapper/framework.hpp>
#include <boost/assign/v2/ref/wrapper/base.hpp>
#include <boost/assign/v2/ref/wrapper/crtp.hpp>

namespace boost{
namespace assign{ 
namespace v2{
namespace ref{
namespace assign_tag{ struct rebind{}; }
namespace assign_rebind{ typedef assign_tag::rebind assign_tag_; }

    template< class T >
    struct wrapper<assign_tag::rebind, T> 
    : public wrapper_crtp< wrapper<assign_tag::rebind, T>, T >
    {
        typedef assign_tag::rebind assign_tag;
    
        BOOST_ASSIGN_V2_REF_WRAPPER_BASE( wrapper )     

        void assign( T& r )
        {
            this->rebind( r );
        }

        void swap( wrapper& r )
        {
            T* tmp = this->ref_;
            this->rebind( r.unwrap() );
            r.rebind( tmp );
        }

    };

namespace type_traits{
    
    template<typename T>
    struct rebind_wrapper{ 
    	typedef ref::wrapper<assign_tag::rebind,T> type; 
    };

}// type_traits
}// ref
}// v2
}// assign
}// boost

#endif

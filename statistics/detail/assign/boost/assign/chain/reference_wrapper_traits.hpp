//////////////////////////////////////////////////////////////////////////////
// assign::detail::ref_wrapper_traits.hpp                                   //
//                                                                          //
//  (C) Copyright 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_DETAIL_REFERENCE_WRAPPER_TRAITS_ER_2010_HPP
#define BOOST_ASSIGN_DETAIL_REFERENCE_WRAPPER_TRAITS_ER_2010_HPP
#include <boost/mpl/identity.hpp>
#include <boost/mpl/assert.hpp>
#include <boost/type_traits.hpp>

namespace boost{
namespace assign{
namespace detail{
namespace ref_wrapper_traits{

    namespace tag{
       struct default_{};
    }

    // specialize for each T   
    template<typename T>
    struct tag_of{};
    
    template<typename T,template<typename> class M>
    struct helper : M<typename ref_wrapper_traits::tag_of<T>::type>
        ::template apply<T>{};

    namespace meta{
        template<typename Tag>
        struct value_of{
            // define template<typename T> struct apply;
        };

        template<typename T> struct default_value_of{};
        template<typename T,template<typename> class W> 
        struct default_value_of<W<T> >{ typedef T type; };
        
        template<>
        struct value_of<ref_wrapper_traits::tag::default_>
        { 
            template<typename T>
            struct apply :  default_value_of<T>{};
        };
    }

    template<typename T>
    struct value_of 
        : ref_wrapper_traits::helper<T,meta::value_of>{};

    namespace meta{
        template<typename Tag>
        struct convertible_to{
            template<typename T> struct apply{};
        };
        template<>
        template<typename T>
        struct convertible_to<tag::default_>::apply<T> 
                : boost::add_reference<
                    typename ref_wrapper_traits::value_of<T>::type
                > 
            {};
    }

    template<typename T>
    struct convertible_to 
        : ref_wrapper_traits::helper<T,meta::convertible_to>{};

}// ref_wrapper_traits
}// detail
}// assign
}// boost

#endif

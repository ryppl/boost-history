//////////////////////////////////////////////////////////////////////////////
// assign::detail::array::converter.hpp                                     //
//                                                                          //
//  (C) Copyright 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_AUTO_SIZE_DETAIL_ARRAY_CONVERTER_ER_2010_HPP
#define BOOST_ASSIGN_AUTO_SIZE_DETAIL_ARRAY_CONVERTER_ER_2010_HPP
#include <boost/typeof/typeof.hpp>

namespace boost{
namespace assign{
namespace detail{
namespace auto_size{

    template< typename T, typename I >
    class converter 
    {

//        friend class boost::assign_detail::converter<this_,I>;

    public: 
        typedef typename I iterator;
        typedef typename I const_iterator;
                
        template< class Container >
        Container convert_to_container() const
        {
            return static_cast<
                const T&>(*this).convert_to_container<Container>();
        }
        
        template< class Container >
        Container to_container( Container& c ) const
        {
            return static_cast<const T&>(*this).to_container(c);
        }

        struct result_of_to_adapter{
            static const D impl;
            
            typedef BOOST_TYPEOF_TPL( impl.to_adapter() ) type;
        
            // needed bec impl_::adapter_converter is private

        };

        typename result_of_to_adapter::type
        to_adapter() const
        {
            return static_cast<const T&>(*this).to_adapter();
        }

        template< class Adapter >
        Adapter to_adapter( Adapter& a ) const
        {
            return static_cast<const T&>(*this).to_adapter(a);
        }

        template< class Array >
        Array to_array( Array& a ) const
        {
            return static_cast<const T&>(*this).to_array(a);
        }

    };

}// auto_size  
}// detail      
}// assign
}// boost

#endif

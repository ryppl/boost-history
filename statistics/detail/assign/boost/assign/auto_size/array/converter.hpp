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
#include <boost/assign/list_of.hpp> // for assign_detail::converter
#include <boost/assign/auto_size/comparison_op/range.hpp>

namespace boost{
namespace assign{
namespace detail{
namespace auto_size{

    // Forwards to assign_detail::conveter<> for all but operators, allowing
    //    Op (converter<> ,converter<>)
    // in addition to the existing feature 
    //    Op( converter<> , Range)
    // etc.
    template< typename T, typename I >
    class converter 
       : protected boost::assign_detail::converter<converter<T,I>,I>
       ,public range_comparison_op::base_of< converter<T,I> >::type
    {
       typedef boost::assign_detail::converter<converter<T,I>,I> impl_;
    
       impl_& impl(){ return (*this); }
       const impl_& impl()const{ return (*this); }
    
    public: 
        typedef typename impl_::iterator iterator;
        typedef typename impl_::const_iterator const_iterator;
        
        iterator begin() const 
        {
            return this->impl().begin();
        }

        iterator end() const
        {
            return this->impl().end();
        }
        
        template< class Container >
        Container convert_to_container() const
        {
            return this->impl().convert_to_container();
        }
        
        template< class Container >
        Container to_container( Container& c ) const
        {
            return this->impl().to_container();
        }

        // // adaptor_converter is private.
        //adapter_converter to_adapter() const
        //{
        //    return this->impl().to_adapter();
        //}

        template< class Adapter >
        Adapter to_adapter( Adapter& a ) const
        {
            return this->impl().to_adapter();
        }

        template< class Array >
        Array to_array( Array& a ) const
        {
            return this->impl().to_array();
        }

    };


}// auto_size  
}// detail      
}// assign
}// boost

#endif

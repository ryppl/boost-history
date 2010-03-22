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
#include <boost/assign/list_of.hpp> // for assign_detail::converter
#include <boost/assign/auto_size/comparison_op/range.hpp>

namespace boost{
namespace assign{
namespace detail{
namespace auto_size{

    template< typename T, typename I >
    class converter 
       : protected boost::assign_detail::converter<T,I> 
       , public range_comparison_op::base_of< T >::type
    {
       typedef boost::assign_detail::converter<T,I> impl_;
    //protected:
       impl_& impl(){ return (*this); }
       const impl_& impl()const{ return (*this); }
/*
        iterator begin() const 
        {
            return this->impl().begin();
        }

        iterator end() const
        {
            return this->impl().end();
        }


*/    
    public: 
        typedef typename impl_::iterator iterator;
        typedef typename impl_::const_iterator const_iterator;
                
        template< class Container >
        Container convert_to_container() const
        {
            return this->impl().convert_to_container<Container>();
        }
        
        template< class Container >
        Container to_container( Container& c ) const
        {
            return this->impl().to_container(c);
        }

        struct result_of_to_adapter{
            static const impl_ impl;
            
            typedef BOOST_TYPEOF_TPL( impl.to_adapter() ) type;
        
            // needed bec impl_::adapter_converter is private

        };

        typename result_of_to_adapter::type
        to_adapter() const
        {
            return this->impl().to_adapter();
        }

        template< class Adapter >
        Adapter to_adapter( Adapter& a ) const
        {
            return this->impl().to_adapter(a);
        }

        template< class Array >
        Array to_array( Array& a ) const
        {
            return this->impl().to_array(a);
        }

    };


}// auto_size  
}// detail      
}// assign
}// boost

#endif

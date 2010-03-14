//////////////////////////////////////////////////////////////////////////////
// assign::detail::chain_as_member.hpp                                      //
//                                                                          //
//  (C) Copyright 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_AUTO_SIZE_DETAIL_ARRAY_CHAINABLE_ER_2010_HPP
#define BOOST_ASSIGN_AUTO_SIZE_DETAIL_ARRAY_CHAINABLE_ER_2010_HPP
#include <boost/assign/auto_size/chain/chain_convert.hpp>

namespace boost{
namespace assign{
namespace detail{

    // D is a derived type
    template<typename D>
    class chain_as_member{

       template<bool add_const>
       struct adaptor : 
         adaptor::chain_convert<typename boost::add_const<D>::type>



       public:

       template<typename R1>
       chain_convert(R1& r1)

       static D& dummy;

       typedef BOOST_TYPEOF_TPL( dummy.chain,new_range2) ) type;


       private:

       adaptor::chain_convert<D>
       chain_convert_adaptor(){ 
            return assign::chain_convert(
                static_cast<D&>(*this),
                r
            ); 
        }

        template<typename R>
        typename result_of_chain_convert<const D>::type
        chain_convert_adaptor()const{ 
            return assign::chain_convert(
                static_cast<const D&>(*this),
                r
            ); 
        }

    };
    
}// detail
}// assign
}// boost

#endif
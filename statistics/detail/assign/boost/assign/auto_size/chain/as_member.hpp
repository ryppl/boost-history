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
#include <boost/type_traits/add_const.hpp>
#include <boost/assign/auto_size/chain/chain_convert.hpp>

namespace boost{
namespace assign{
namespace detail{

    
    // D is a derived type that is boost::chain'able 
    template<typename D>
    class chain_as_member{

       template<bool add_const>
       struct adaptor : 
         assign::adaptor::chain_convert<
             typename boost::add_const<D>::type
        >{};

       template<typename R,bool add_const>
       struct result_impl 
           : adaptor<add_const>::template result_impl<R,add_const>{};

       public:

       // TODO : complete
       // chain_convert(R&);
       // chain_convert(const R&);

       typename adaptor<false>::type
       chain_convert_l_adaptor(){ 
            return assign::chain_convert_l(
                static_cast<D&>(*this)
            ); 
        }

       typename adaptor<true>::type
        chain_convert_r_adaptor()const{ 
            return assign::chain_convert_r(
                static_cast<const D&>(*this)
            ); 
        }

    };
    
}// detail
}// assign
}// boost

#endif
//////////////////////////////////////////////////////////////////////////////
// assign::detail::add_const.hpp                                            //
//                                                                          //
//  (C) Copyright 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_DETAIL_AUTO_SIZE_CHAIN_ADD_CONST_ER_2010_HPP
#define BOOST_ASSIGN_DETAIL_AUTO_SIZE_CHAIN_ADD_CONST_ER_2010_HPP
#include <boost/type_traits.hpp>

namespace boost{
namespace assign{
namespace detail{

    // This is only for testing purposes

    template<typename Rng>
    typename boost::add_const<Rng>::type
    add_const(Rng& rng){ return rng; }

}// detail
}// assign
}// boost

#endif

//////////////////////////////////////////////////////////////////////////////
// assign::detail::is_ref_wrapper.hpp                                       //
//                                                                          //
//  (C) Copyright 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_DETAIL_CHAIN_IS_REFERENCE_WRAPPER_IS_ER_2010_HPP
#define BOOST_ASSIGN_DETAIL_CHAIN_IS_REFERENCE_WRAPPER_IS_ER_2010_HPP
#include <boost/mpl/bool.hpp>

namespace boost{
namespace assign{
namespace detail{

    // specialize on T
    template<typename T>
    struct is_ref_wrapper : boost::mpl::bool_<false>{};

}// detail
}// assign
}// boost

#endif

////////////////////////////////////////////////////////////////////////////
//  variadic_sequence.hpp                                                 //
//                                                                        //
//  Copyright (C) 2010 Erwann Rogard                                      //
//  Use, modification and distribution are subject to the                 //
//  Boost Software License, Version 1.0. (See accompanying file           //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)      //
////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_MPL_DETAIL_VARIADIC_VECTOR_HPP_ER_2010
#define BOOST_MPL_DETAIL_VARIADIC_VECTOR_HPP_ER_2010
#include <boost/mpl/vector.hpp>
#include <boost/mpl/aux_/na.hpp>
#include <boost/assign/v2/temporary/variadic_sequence.hpp>

namespace boost{
namespace mpl{
namespace detail{

BOOST_MPL_DETAIL_VARIADIC_SEQUENCE(
    BOOST_MPL_LIMIT_VECTOR_SIZE,
    ::boost::mpl::vector,
    variadic_vector,
    ::boost::mpl::na
)

}// detail
}// mpl
}// boost

#endif // BOOST_MPL_DETAIL_VARIADIC_VECTOR_HPP_ER_2010

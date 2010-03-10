//////////////////////////////////////////////////////////////////////////////
// assign::detail::auto_size::max_element.hpp                               //
//                                                                          //
//  (C) Copyright 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_DETAIL_AUTO_SIZE_CHECK_MAX_ELEMENT_ER_2010_HPP
#define BOOST_ASSIGN_DETAIL_AUTO_SIZE_CHECK_MAX_ELEMENT_ER_2010_HPP
#include <algorithm>
#include <boost/range.hpp>

namespace boost{
namespace assign{
namespace detail{
namespace auto_size{


// This function will be superseded by boost::max_element from range_ex

template< class Range >
typename boost::range_iterator<const Range>::type 
max_element( const Range& r )
{
    return std::max_element( r.begin(), r.end() );
}


}// auto_size
}// detail
}// assign
}// boost

#endif

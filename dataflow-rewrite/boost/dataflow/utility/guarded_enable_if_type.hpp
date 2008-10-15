/*=================================---------------------------------------------
    Copyright 2007,2008 Stjepan Rajko
  
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
-----------------------------------------------===============================*/

#ifndef BOOST__DATAFLOW__UTILITY__GUARDED_ENABLE_IF_TYPE_HPP
#define BOOST__DATAFLOW__UTILITY__GUARDED_ENABLE_IF_TYPE_HPP


#include <boost/dataflow/utility/all_of.hpp>
#include <boost/mpl/assert.hpp>
#include <boost/type_traits/is_same.hpp>


namespace boost { namespace dataflow {
    
namespace utility
{
    struct enable_guard;

    template<typename T, typename Result=enable_guard>
    struct guarded_enable_if_type
    {
        typedef Result type;
    };

} // namespace utility

} } // namespace boost::dataflow

#endif // BOOST__DATAFLOW__UTILITY__GUARDED_ENABLE_IF_TYPE_HPP

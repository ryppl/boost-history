//  value_type.hpp  --------------------------------------------------------------//

//  Copyright 2010 Vicente J. Botet Escriba

//  Distributed under the Boost Software License, Version 1.0.
//  See http://www.boost.org/LICENSE_1_0.txt


#ifndef BOOST_STATIC_STRING_VALUE_TYPE_HPP
#define BOOST_STATIC_STRING_VALUE_TYPE_HPP

namespace boost {
namespace static_string {

template<typename Sequence>
struct value_type {
    typedef typename Sequence::value_type type;
};

} // namespace static_string
} // namespace boost

#endif // BOOST_STATIC_STRING_VALUE_TYPE_HPP

//  Copyright 2010 Brian Bartman.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at 
//  http://www.boost.org/LICENSE_1_0.txt)


#ifndef BOOST_CUSTOM_FIELD_HPP
#define BOOST_CUSTOM_FIELD_HPP
#include <cstddef>

namespace boost { namespace bitfields {

/** The custom name means that the type being passed in is going to use
 *  custom packing and unpacking measures to store and retrieve its data.
 *  The policy type will be documented else where, as to show how it works
 *  and what it can/should be used for.
 */
template <
    typename ReturnType,
    typename Name,
    typename Mask,
    template <
        typename,
        typename,
        typename,
        typename
    > class Policy
>
struct custom;

}} // end boost::bitfields

#endif

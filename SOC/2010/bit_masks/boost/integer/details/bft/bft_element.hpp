//  Copyright 2010 Brian Bartman.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at 
//  http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_BITFIELD_TUPLE_BFT_ELEMENT_HPP
#define BOOST_BITFIELD_TUPLE_BFT_ELEMENT_HPP

namespace boost { namespace details {

/** This is the type which sits inside the mpl vector type.
 *  This has all information for constructing a bitfield.
 */
template <  typename ReturnType,
            typename NameType, 
            typename Offset,
            typename FieldWidth
>
struct bitfield_element_ {
    typedef ReturnType  return_type;
    typedef NameType    name_type;
    typedef Offset      offset;
    typedef FieldWidth  field_width;
    typedef bitfield_element_<return_type, name_type, offset, field_width> type;
};


}} // end boost::details

#endif

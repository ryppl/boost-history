//  Copyright 2010 Brian Bartman.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at 
//  http://www.boost.org/LICENSE_1_0.txt)


#ifndef BOOST_BIT_MASKS_TAGGED_HPP
#define BOOST_BIT_MASKS_TAGGED_HPP


namespace boost {

/** This is used for naming/tagging arguments within bit structures. */
template <typename T, typename NameType>
struct tagged {
    typedef NameType            name;
    typedef T                   value;
    typedef tagged<NameType, T> type;
};

} // end boost

#endif

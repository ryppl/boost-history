//  Copyright 2010 Brian Bartman.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at 
//  http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_BITFIELD_FUSION_ITERATOR_HPP
#define BOOST_BITFIELD_FUSION_ITERATOR_HPP
#include <cstddef>
#include <boost/mpl/int.hpp>
#include <boost/mpl/assert.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/minus.hpp>
#include <boost/type_traits/is_const.hpp>
#include <boost/fusion/iterator/iterator_facade.hpp>

namespace boost { namespace fusion {

/** Fusion Iterator for bitfield_tuple class.
 *  This is an iterator over an associative array type.
 */
template <typename Bitfeid_Tuple, std::size_t Pos>
struct bitfield_tuple_iterator
    : iterator_facade<
        bitfield_tuple_iterator<Bitfeid_Tuple, Pos>,
        random_access_traversal_tag
    >
{ };

}} // end boost::fusion


#endif

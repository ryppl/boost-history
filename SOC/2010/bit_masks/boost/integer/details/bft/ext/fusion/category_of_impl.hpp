//  Copyright 2010 Brian Bartman.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at 
//  http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_BITFIELD_TUPLE_FUSION_EXT_CATEGORY_OF_HPP
#define BOOST_BITFIELD_TUPLE_FUSION_EXT_CATEGORY_OF_HPP


namespace boost {

struct bitfield_tuple_tag;

namespace fusion { namespace extension {

/** The bft_category is composed of more then one category from the
 *  available fusion categories.
 */
struct bft_category
    :fusion::random_access_traversal_tag,
    fusion::associative_tag
{ };


template <typename>
struct category_of_impl;

template<>
struct category_of_impl< bitfield_tuple_tag > {
    template<typename T>
    struct apply {
        typedef bft_category type;
    };
};

}}} // end boost::fusion::extension

#endif

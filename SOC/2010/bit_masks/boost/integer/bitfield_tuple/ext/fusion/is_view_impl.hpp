//  Copyright 2010 Brian Bartman.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at 
//  http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_BITFIELD_TUPLE_FUSION_EXT_IS_VIEW_HPP
#define BOOST_BITFIELD_TUPLE_FUSION_EXT_IS_VIEW_HPP


namespace boost {

struct bitfield_tuple_tag;

namespace fusion { namespace extension {

template <typename>
struct is_view_impl;

template<>
struct is_view_impl< bitfield_tuple_tag > {
    template<typename T>
    struct apply : mpl::false_ { };
};

}}} // end boost::fusion::extension

#endif

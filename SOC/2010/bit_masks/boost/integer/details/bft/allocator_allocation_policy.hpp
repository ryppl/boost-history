//  Copyright 2010 Brian Bartman.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at 
//  http://www.boost.org/LICENSE_1_0.txt)


#ifndef BOOST_BITFIELD_TUPLE_ALLOC_ALLOC_POLICY_HPP
#define BOOST_BITFIELD_TUPLE_ALLOC_ALLOC_POLICY_HPP

namespace boost { namespace details {

/** This isn't implemented yet and shouldn't be used just yet.
 *  This is eventually going to wrap up an pointer to an allocated object which
 *  will be retrievable via a alloc_get function.
 */
template <typename T, typename Alloc>
struct allocator_wraper_base_policy {
    T const get_data() const { return _data; }
    T get_data() { return _data; }
    T _data;
};

}} // end boost::details

#endif

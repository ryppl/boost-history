
//
// Copyright 2010 Scott McMurray.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef BOOST_HASH_COMPUTE_DIGEST_HPP
#define BOOST_HASH_COMPUTE_DIGEST_HPP

#include <boost/static_assert.hpp>

#include <iterator>
#include <limits>

#include <cstring>

namespace boost {
namespace hash {

template <typename hash_T, typename iter_T>
typename hash_T::digest_type
compute_digest(iter_T b, iter_T e) {
    typedef typename std::iterator_traits<iter_T>::value_type value_type;
    BOOST_STATIC_ASSERT(std::numeric_limits<value_type>::is_specialized);
    unsigned const value_bits =
        std::numeric_limits<value_type>::digits +
        std::numeric_limits<value_type>::is_signed;
    typedef typename hash_T::template stream_hash<value_bits>::type
            stream_hash_type;
    stream_hash_type sh;
    sh.update(b, e);
    return sh.end_message();
}

template <typename hash_T, typename iter_T>
typename hash_T::digest_type
compute_digest_n(iter_T b, size_t n) {
    typedef typename std::iterator_traits<iter_T>::value_type value_type;
    BOOST_STATIC_ASSERT(std::numeric_limits<value_type>::is_specialized);
    unsigned const value_bits =
        std::numeric_limits<value_type>::digits +
        std::numeric_limits<value_type>::is_signed;
    typedef typename hash_T::template stream_hash<value_bits>::type
            stream_hash_type;
    stream_hash_type sh;
    sh.update_n(b, n);
    return sh.end_message();
}

template <typename hash_T>
typename hash_T::digest_type
compute_digest(char const *p) {
    return compute_digest_n<hash_T>(p, std::strlen(p));
}

} // namespace hash
} // namespace boost

#endif // BOOST_HASH_COMPUTE_DIGEST_HPP

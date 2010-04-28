
//
// Copyright 2010 Scott McMurray.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef BOOST_HASH_COMPUTE_DIGEST_HPP
#define BOOST_HASH_COMPUTE_DIGEST_HPP

#include <iterator>
#include <limits>

#include <cstring>

namespace boost {
namespace hash {

template <typename hash_T, typename iter_T>
typename hash_T::digest_type
compute_digest(iter_T b, iter_T e) {
    typedef typename std::iterator_traits<iter_T>::value_type value_type;
    unsigned const value_bits =
        std::numeric_limits<value_type>::digits +
        std::numeric_limits<value_type>::is_signed;
    typedef typename hash_T::template stream_hash<value_bits>::type
            stream_hash_type;
    stream_hash_type sh;
    while (b != e) sh.update(*b++);
    return sh.end_message();
}

template <typename hash_T, typename value_T>
typename hash_T::digest_type
compute_digest(value_T *p, size_t n) {
    return compute_digest<hash_T>(p, p+n);
}

template <typename hash_T>
typename hash_T::digest_type
compute_digest(char const *p) {
    return compute_digest<hash_T>(p, std::strlen(p));
}

} // namespace hash
} // namespace boost

#endif // BOOST_HASH_COMPUTE_DIGEST_HPP

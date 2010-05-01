
//
// Copyright 2010 Scott McMurray.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef BOOST_HASH_DIGEST_HPP
#define BOOST_HASH_DIGEST_HPP

#include <boost/array.hpp>
#include <boost/integer.hpp>
#include <boost/hash/pack.hpp>
#include <boost/static_assert.hpp>

#include <iterator>
#include <ostream>
#include <string>

#include <cstring>

namespace boost {
namespace hash {

// Always stored internally as a sequence of octets in display order.
// This allows digests from different algorithms to have the same type,
// allowing them to be more easily stored and compared.
template <unsigned digest_bits_>
class digest {
  public:
    static unsigned const octet_bits = 8;
    typedef typename uint_t<octet_bits>::least octet_type;

    static unsigned const digest_bits = digest_bits_;
    BOOST_STATIC_ASSERT(digest_bits % octet_bits == 0);
    static unsigned const digest_octets = digest_bits/octet_bits;
    typedef array<octet_type, digest_octets> data_type;

    static unsigned const cstring_size = digest_bits/4 + 1;
    typedef array<char, cstring_size> cstring_type;

    digest() : data_() {}
    digest(data_type const &d) : data_(d) {}

    data_type &data() { return data_; }
    data_type const &data() const { return data_; }

    template <typename oit_T>
    oit_T ascii(oit_T it) const {
        for (unsigned j = 0; j < digest_octets; ++j) {
            octet_type b = data_[j];
            *it++ = "0123456789abcdef"[(b >> 4) & 0xF];
            *it++ = "0123456789abcdef"[(b >> 0) & 0xF];
        }
        return it;
    }

    std::string
    str() const {
        cstring_type cstr = cstring();
        return std::string(cstr.data(), cstr.size()-1);
    }

    cstring_type
    cstring() const {
        cstring_type s;
        char *p = ascii(s.data());
        *p++ = '\0';
        return s;
    }

    template <typename endian,
              unsigned state_bits,
              unsigned word_bits,
              typename state_type>
    static digest
    from_state(state_type const &s) {
        static unsigned const state_words = state_bits/word_bits;
        BOOST_STATIC_ASSERT(state_words <= state_words);
        BOOST_STATIC_ASSERT(digest_bits % word_bits == 0);

        array<octet_type, state_bits/octet_bits> d;
        pack<endian, word_bits, octet_bits>(s, d);
        data_type td;
        for (unsigned i = 0; i < digest_octets; ++i) {
            td[i] = d[i];
        }
        return digest(td);
    }
  private:
    data_type data_;
};

template <typename digest_type_,
          typename endian>
struct digest_from_state {
    typedef digest_type_ digest_type;
    template <unsigned state_bits,
              unsigned word_bits,
              typename state_type>
    static digest_type
    from_state(state_type const &s) {
        return digest_type::template from_state<
                    endian, state_bits, word_bits
               >(s);
    }
};

template <unsigned DB>
bool operator==(digest<DB> const &a, digest<DB> const &b) {
    return a.data() == b.data();
}

template <unsigned DB>
bool operator!=(digest<DB> const &a, digest<DB> const &b) {
    return !(a == b);
}

template <unsigned DB>
bool operator!=(digest<DB> const &a, char const *b) {
    return std::strcmp(a.cstring().data(), b);
}

template <unsigned DB>
bool operator==(digest<DB> const &a, char const *b) {
    return !(a != b);
}

template <unsigned DB>
bool operator!=(char const *b, digest<DB> const &a) {
    return a != b;
}
template <unsigned DB>
bool operator==(char const *b, digest<DB> const &a) {
    return a == b;
}

template <unsigned DB>
std::ostream &
operator<<(std::ostream &sink, digest<DB> const &d) {
    d.ascii(std::ostream_iterator<char>(sink));
    return sink;
};

} // namespace hash
} // namespace boost

#endif // BOOST_HASH_DIGEST_HPP

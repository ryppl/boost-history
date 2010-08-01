// Copyright Justinas Vygintas Daugmaudis, 2010.
// Use, modification and distribution is subject to the
// Boost Software License, Version 1.0. (See accompanying
// file LICENSE-1.0 or http://www.boost.org/LICENSE-1.0)

#ifndef HEADER_DETAIL_CONSTANT_SIZE_ARRAY_HPP_INCLUDED
#define HEADER_DETAIL_CONSTANT_SIZE_ARRAY_HPP_INCLUDED

#include <boost/array.hpp>

namespace detail {

// This is a minimal wrapper around boost::array so that it can be
// used as an underlaying container for uBLAS data structures
// such as vector and matrix
template<class T, std::size_t N>
struct constant_size_array : public boost::array<T, N>
{
    constant_size_array(std::size_t m)
    {
        // Assert that this array holds exactly the same
        // size of memory block as requested
        BOOST_ASSERT(m == N);
    }

    void resize(std::size_t) {
        BOOST_ASSERT("Cannot resize a constant sized array." == 0);
    }

    void resize(std::size_t size, T init) {
        BOOST_ASSERT("Cannot resize a constant sized array." == 0);
    }
};

} // namespace detail

#endif // HEADER_DETAIL_CONSTANT_SIZE_ARRAY_HPP_INCLUDED

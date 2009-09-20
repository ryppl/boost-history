//
// boost_array.hpp - container streaming for boost::array
//
// Copyright (C) 2007, Jeffrey Faust
// Copyright (C) 2008-2009, Jared McIntyre
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/explore for library home page.

#ifndef BOOST_EXPLORE_ARRAY_H
#define BOOST_EXPLORE_ARRAY_H

#include <boost/array.hpp>
#include <boost/explore/stream_container.hpp>

namespace boost
{
    template<typename Elem, typename Tr, typename T, std::size_t N>
    std::basic_ostream<Elem, Tr>& operator<<(
        std::basic_ostream<Elem, Tr>& ostr,
        const boost::array<T, N>& a)
    {
        return boost::explore::stream_container(ostr, a.begin(), a.end());
    }
}

#endif

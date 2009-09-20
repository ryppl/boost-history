//
// list.hpp - container streaming for std::list
//
// Copyright (C) 2007, Jeffrey Faust
// Copyright (C) 2008-2009, Jared McIntyre
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/explore for library home page.

#ifndef BOOST_EXPLORE_LIST_H
#define BOOST_EXPLORE_LIST_H

#include <list>
#include <boost/explore/stream_container.hpp>

namespace std
{
    template<typename Elem, typename Tr, typename T, typename Allocator>
    std::basic_ostream<Elem, Tr>& operator<<(
        std::basic_ostream<Elem, Tr>& ostr,
        const std::list<T, Allocator>& l)
    {
        return boost::explore::stream_container(ostr, l.begin(), l.end());
    }
}

#endif

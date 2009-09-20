//
// set.hpp - container streaming for std::set and std::multiset
//
// Copyright (C) 2007, Jeffrey Faust
// Copyright (C) 2008-2009, Jared McIntyre
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/explore for library home page.

#ifndef BOOST_EXPLORE_SET_H
#define BOOST_EXPLORE_SET_H

#include <set>
#include <boost/explore/stream_container.hpp>

namespace std
{
    template<typename Elem, typename Tr, typename T,
             typename Compare, typename Alloc>
    std::basic_ostream<Elem, Tr>& operator<<(
        std::basic_ostream<Elem, Tr>& ostr,
        const std::set<T, Compare, Alloc>& s)
    {
        return boost::explore::stream_container(ostr, s.begin(), s.end());
    }
    
    template<typename Elem, typename Tr, typename T,
             typename Compare, typename Allocator>
    std::basic_ostream<Elem, Tr>& operator<<(
        std::basic_ostream<Elem, Tr>& ostr,
        const std::multiset<T, Compare, Allocator>& s)
    {
        return boost::explore::stream_container(ostr, s.begin(), s.end());
    }
}

#endif

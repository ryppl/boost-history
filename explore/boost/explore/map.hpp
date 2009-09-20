//
// map.hpp - container streaming for std::map and std::multimap
//
// Copyright (C) 2007, Jeffrey Faust
// Copyright (C) 2009, Jared McIntyre
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/explore for library home page.

#ifndef BOOST_EXPLORE_MAP_H
#define BOOST_EXPLORE_MAP_H

#include <map>
#include <boost/explore/stream_container.hpp>

namespace std
{
    template<typename Elem, typename Tr, typename K, typename T,
             typename Compare, typename Allocator>
    std::basic_ostream<Elem, Tr>& operator<<(
        std::basic_ostream<Elem, Tr>& ostr,
        const std::map<K, T, Compare, Allocator>& m)
    {
        using namespace boost::explore;
        return stream_container(
            ostr, m.begin(), m.end(), stream_associative_value());
    }
    
    template<typename Elem, typename Tr, typename K, typename T,
             typename Compare, typename Allocator>
    std::basic_ostream<Elem, Tr>& operator<<(
        std::basic_ostream<Elem, Tr>& ostr,
        const std::multimap<K, T, Compare, Allocator>& m)
    {
        using namespace boost::explore;
        return stream_container(ostr, m.begin(), m.end(),
            stream_associative_value());
    }
}

#endif
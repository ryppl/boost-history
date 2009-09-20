//
// pair.hpp - container streaming for std::pair
//
// Copyright (C) 2007-2009, Jeffrey Faust
// Copyright (C) 2009, Jared McIntyre
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/explore for library home page.

#ifndef BOOST_EXPLORE_PAIR_H
#define BOOST_EXPLORE_PAIR_H

#include <utility>
#include <boost/explore/stream_container.hpp>

namespace std
{
    template<typename Elem, typename Tr, typename T1, typename T2>
    basic_ostream<Elem, Tr>& operator<<(
        basic_ostream<Elem, Tr>& ostr,
        const pair<T1, T2>& p)
    {
        using namespace boost::explore;

        container_stream_state<Elem>* state =
            get_stream_state<container_stream_state<Elem> >(ostr);
        container_common_stream_state* common_state =
            get_stream_state<container_common_stream_state>(ostr);
        detail::increment_depth guard(common_state);
        ostr << state->start();
        stream_normal_value()(ostr, p.first, state, common_state);
        ostr << state->separator();
        stream_normal_value()(ostr, p.second, state, common_state);
        ostr << state->end();
        return ostr;
    }
}

#endif

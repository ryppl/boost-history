//
// pair.hpp - container streaming.
//
// Copyright (C) 2007, Jeffrey Faust
// Copyright (C) 2009, Jared McIntyre
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef BOOST_EXPLORE_PAIR_H
#define BOOST_EXPLORE_PAIR_H

#include <utility>
#include <boost/explore/stream_container.hpp>

namespace std
{
    template<typename Elem, typename Tr, typename T1, typename T2>
    std::basic_ostream<Elem, Tr>& operator<<(std::basic_ostream<Elem, Tr>& ostr, const std::pair<T1, T2>& p)
    {
        using namespace boost::explore;
        container_stream_state<Elem>* state = get_stream_state<container_stream_state<Elem> >(ostr);
        detail::depth_guard<Elem> guard(state);
        state->set_level(state->depth()-1);
        return ostr << state->start() << p.first << state->separator() << p.second << state->end();
    }
}

#endif
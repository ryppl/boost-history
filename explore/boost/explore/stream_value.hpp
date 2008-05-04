//
// stream_value.hpp - streaming function objects for different value types
//
// Copyright (C) 2007, Jeffrey Faust
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef BOOST_EXPLORE_STREAM_VALUE_H
#define BOOST_EXPLORE_STREAM_VALUE_H

#include "container_stream_state.hpp"

#include <ostream>

namespace explore
{
    struct stream_normal_value
    {
        template<typename Elem, typename Tr, typename T>
        void operator()(std::basic_ostream<Elem, Tr>& ostr, const T& val, container_stream_state<Elem>* state)
        {
            ostr.width(state->itemwidth(state->depth()));
            ostr << val;
        }
    };

    // stream value from associative container
    struct stream_associative_value
    {
        template<typename Elem, typename Tr, typename T>
        void operator()(std::basic_ostream<Elem, Tr>& ostr, const T& val, container_stream_state<Elem>* state)
        {
            ostr << state->assoc_start() << val.first << state->assoc_separator() << val.second << state->assoc_end();
        }
    };
}

#endif

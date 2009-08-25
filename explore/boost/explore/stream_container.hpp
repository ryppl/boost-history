//
// stream_container.hpp - container streaming.
//
// Copyright (C) 2007, Jeffrey Faust
// Copyright (C) 2009, Jared McIntyre
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef STREAM_CONTAINER_INCLUDED
#define STREAM_CONTAINER_INCLUDED

#include <boost/explore/manipulators.hpp>

#include <ostream>
#include <boost/functional/detail/container_fwd.hpp>
#include <boost/array.hpp>

namespace boost { namespace explore
{
    template<typename Elem, typename Tr, typename FwdIter, typename F>
    std::basic_ostream<Elem, Tr>& stream_container(std::basic_ostream<Elem, Tr>& ostr, FwdIter first, FwdIter last, F f)
    {
        // grab the extra data embedded in the stream object.
        container_stream_state<Elem>* state = explore::get_stream_state<container_stream_state<Elem> >(ostr);
        detail::depth_guard<Elem> guard(state);
        std::size_t depth = state->depth();

        // starting delimiter
        ostr << state->start(depth);

        std::size_t rows = state->rows(depth);

        std::size_t cur_row = 0;
        while( first != last )
        {
            // value
            f(ostr, *first, state);

            if( ++first != last )
            {
                // separation delimiter
                ostr << state->separator(depth);

                if( rows && ++cur_row == rows )
                {
                    ostr << '\n';
                    cur_row = 0;
                }
            }
        }

        // ending delimiter
        return ostr << state->end(depth);
    }

    template<typename Elem, typename Tr, typename FwdIter>
    std::basic_ostream<Elem, Tr>& stream_container(std::basic_ostream<Elem, Tr>& ostr, FwdIter first, FwdIter last)
    {
        // redirect with "normal" streaming.
        return stream_container(ostr, first, last, stream_normal_value());
    }
    
#   if !BOOST_WORKAROUND(BOOST_MSVC, <= 1300)
    template<typename Elem, typename Tr, typename T, std::size_t size>
    std::basic_ostream<Elem, Tr>& operator<<(std::basic_ostream<Elem, Tr>& ostr, T (&a)[size])
    {
        return stream_container(ostr, &a[0], &a[size]);
    }
    
    template<typename Elem, typename Tr, std::size_t size>
    std::basic_ostream<Elem, Tr>& operator<<(std::basic_ostream<Elem, Tr>& ostr, const Elem* s)
    {
        return stream_container(ostr, &s[0], &s[strlen(s)]);
    }
#   endif
}}

#endif

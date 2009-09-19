//
// stream_container.hpp - container streaming.
//
// Copyright (C) 2007-2009, Jeffrey Faust
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
        container_common_stream_state* common_state = explore::get_stream_state<container_common_stream_state>(ostr);

        // set the level based on the current recursive depth
        detail::increment_depth guard(common_state);

        // starting delimiter
        ostr << state->start();

        std::size_t cols = common_state->cols();

        std::size_t cur_col = 0;
        while( first != last )
        {
            // value
            f(ostr, *first, state, common_state);

            if( ++first != last )
            {
                // separation delimiter
                ostr << state->separator();

                if( cols && ++cur_col == cols )
                {
                    ostr << '\n';
                    cur_col = 0;
                }
            }
        }

        // ending delimiter
        return ostr << state->end();
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
        return boost::explore::stream_container(ostr, &a[0], &a[size]);
    }
    
    template<typename Elem, typename Tr, std::size_t size>
    std::basic_ostream<Elem, Tr>& operator<<(std::basic_ostream<Elem, Tr>& ostr, const Elem* s)
    {
        return boost::explore::stream_container(ostr, &s[0], &s[strlen(s)]);
    }
#   endif
}}

#endif

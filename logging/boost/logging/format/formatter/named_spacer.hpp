// named_spacer.hpp

// Boost Logging library
//
// Author: John Torjo, www.torjo.com
//
// Copyright (C) 2007 John Torjo (see www.torjo.com for email)
//
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org for updates, documentation, and revision history.
// See http://www.torjo.com/log2/ for more details


#ifndef JT28092007_named_spacer_HPP_DEFINED
#define JT28092007_named_spacer_HPP_DEFINED

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif

#include <boost/logging/detail/fwd.hpp>

namespace boost { namespace logging { namespace formatter {

namespace detail {
    template<class convert, class lock_resource, class format_base> struct named_spacer_context {
        typedef use_default<lock_resource, boost::logging::

        typedef typename lock_resource::template finder<write_info>::type data;
        
    };
}
    // when adding generic ones, make sure I keep the same convert (append or prepend)
    // named_spacer<> - you specify the convert !!!
        // if adding a generic formatter and convert is diff than ours, compile error!
    // also, allow adding more in one step : .add(..).add(..)....

template<class lock_resource = default_, class convert = do_convert_format::prepend, class format_base = base<> > 
        struct named_spacer_t : is_generic, non_const_context< detail::named_spacer_context<convert,lock_resource,format_base> > {
    // have array or so?

    // allow to change the format string
    // unknown % - ignore, treat as string
    // % - escape char
    // %% = %

};



}}}

#endif


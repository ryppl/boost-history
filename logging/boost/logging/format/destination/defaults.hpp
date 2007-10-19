// destination_defaults.hpp

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


#ifndef JT28092007_destination_defaults_HPP_DEFINED
#define JT28092007_destination_defaults_HPP_DEFINED

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif

#include <boost/logging/detail/fwd.hpp>
#include <boost/logging/detail/manipulator.hpp>
#include <boost/logging/format/destination/convert_destination.hpp>
#include <boost/logging/format/destination/file.hpp>
#include <iostream>

namespace boost { namespace logging { namespace destination {


/** 
    @brief Writes the string to console
*/
template<class convert_dest = do_convert_destination > struct cout : is_generic, same_type {
    template<class msg_type> void operator()(const msg_type & msg) const {
#ifndef UNICODE
        convert_dest::write(msg, std::cout);
#else
        convert_dest::write(msg, std::wcout);
#endif
    }
};


template<class convert_dest = do_convert_destination > struct dbg_window {
    template<class msg_type> void operator()(const msg_type & msg) const {
#ifdef BOOST_WINDOWS
#ifndef UNICODE
    ::OutputDebugWindowA( convert_dest::do_convert(msg, into<const char*>() ) );
#else
    ::OutputDebugWindowW( convert_dest::do_convert(msg, into<const wchar_t*>() ) );    
#endif
#else
        // non windows
        msg; // so that there's no warning
#endif
    }
};




sharing memory

}}}

#endif


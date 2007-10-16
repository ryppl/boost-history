// destination_rolling_file.hpp

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


#ifndef JT28092007_destination_rolling_file_HPP_DEFINED
#define JT28092007_destination_rolling_file_HPP_DEFINED

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif

#include <boost/logging/detail/fwd.hpp>
#include <boost/logging/detail/manipulator.hpp>
#include <boost/logging/format/destination/convert_destination.hpp>
#include <fstream>
#include <string>

namespace boost { namespace logging { namespace destination {

namespace detail {
    struct rolling_file_info {
        rolling_file_info (const std::string& name_prefix, file_settings settings) : out(name, open_flags(settings) ), settings(settings) {}
        std::basic_ofstream<char_type> out;
        file_settings settings;
    }
}

template<class convert_dest = do_convert_destination > struct rolling_file : non_const_context<detail::file_info> {
};





}}}

#endif


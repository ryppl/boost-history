// formatter_tags.hpp

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


#ifndef JT28092007_formatter_tags_HPP_DEFINED
#define JT28092007_formatter_tags_HPP_DEFINED

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif

#include <boost/logging/detail/fwd.hpp>
#include <boost/logging/tags.hpp>
#include <boost/logging/detail/manipulator.hpp>


namespace boost { namespace logging { namespace formatter {

template<class type, class tag_type> struct uses_tag {
    template<class tag_holder_type> void operator()(tag_holder_type & str) const {
        typedef typename tag_holder_type::string_type string_type;
        // automatic conversion - tag holder provides this
        const tag_type & tag = str;

        const type & self = reinterpret_cast<const type&>(*this);
        self.write_tag(str, tag);
    }
};

template<class convert = do_convert_format::prepend> struct file_line_t : is_generic, uses_tag< file_line_t<convert>, ::boost::logging::tag::file_line >, boost::logging::op_equal::always_equal  {
    template<class msg_type, class tag_type> void write_tag(msg_type & str, const tag_type & tag) const {
        convert::write( tag.val, str);
    }
};

typedef file_line_t<> file_line;

template<class convert = do_convert_format::prepend> struct function_t : is_generic, uses_tag< function_t<convert>, ::boost::logging::tag::function >, boost::logging::op_equal::always_equal  {
    template<class msg_type, class tag_type> void write_tag(msg_type & str, const tag_type & tag) const {
        convert::write( tag.val, str);
    }
};

typedef function_t<> function;

}}}

#endif


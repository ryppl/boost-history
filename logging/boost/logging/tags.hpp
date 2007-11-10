// tags.hpp

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


#ifndef JT28092007_tags_HPP_DEFINED
#define JT28092007_tags_HPP_DEFINED

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif

#include <boost/logging/detail/fwd.hpp>

namespace boost { namespace logging {

namespace detail {
    struct void_1 {};
    struct void_2 {};
    struct void_3 {};
    struct void_4 {};
    struct void_5 {};
    struct void_6 {};
    struct void_7 {};
    struct void_8 {};
    struct void_9 {};
    struct void_10 {};

    template<class string_type> struct tag_holder_base {
        // assumes m_string is convertible to string
        operator const hold_string_type & () const { return m_string; }
    protected:
        string_type m_string;
    };
    template<> struct tag_holder_base<default_> {
        // it's for the default string
    protected:
        hold_string_type m_string;
    };
}

/** @brief Holds up to 10 @ref tag "tags".

@param string_ The string class we use for holding logged messages. By default, std::(w)string. What you used to specify using BOOST_LOG_FORMAT_MSG.

*/
template<
        class string_ = default_,
        class param1 = detail::void_1,
        class param2 = detail::void_2,
        class param3 = detail::void_3,
        class param4 = detail::void_4,
        class param5 = detail::void_5,
        class param6 = detail::void_6,
        class param7 = detail::void_7,
        class param8 = detail::void_8,
        class param9 = detail::void_9,
        class param10 = detail::void_10> struct tag_holder : detail::tag_holder_base<string_> {
    typedef typename use_default<string_, hold_string_type>::type string_type;
    typedef detail::tag_holder_base<string_> tag_base_type;

    operator string_type & () { return tag_base_type::m_string; }
    operator const string_type & () const { return tag_base_type::m_string; }

    operator const param1& () const { return m_tag1; }
    operator const param2& () const { return m_tag2; }
    operator const param3& () const { return m_tag3; }
    operator const param4& () const { return m_tag4; }
    operator const param5& () const { return m_tag5; }
    operator const param6& () const { return m_tag6; }
    operator const param7& () const { return m_tag7; }
    operator const param8& () const { return m_tag8; }
    operator const param9& () const { return m_tag9; }
    operator const param10& () const { return m_tag10; }

    template<class tag_type> tag_holder& set_tag(const tag_type & val) {
        set_tag_impl(val);
        return *this;
    }

    template<class tag_type> const tag_type & get_tag() const {
        return operator const tag_type&();
    }

    void set_string(const string_type & str) {
        m_string = str;
    }

private:
    void set_tag_impl(const param1 & tag) {
        m_tag1 = tag;
    }
    void set_tag_impl(const param2 & tag) {
        m_tag2 = tag;
    }
    void set_tag_impl(const param3 & tag) {
        m_tag3 = tag;
    }
    void set_tag_impl(const param4 & tag) {
        m_tag4 = tag;
    }
    void set_tag_impl(const param5 & tag) {
        m_tag5 = tag;
    }
    void set_tag_impl(const param6 & tag) {
        m_tag6 = tag;
    }
    void set_tag_impl(const param7 & tag) {
        m_tag7 = tag;
    }
    void set_tag_impl(const param8 & tag) {
        m_tag8 = tag;
    }
    void set_tag_impl(const param9 & tag) {
        m_tag9 = tag;
    }
    void set_tag_impl(const param10 & tag) {
        m_tag10 = tag;
    }

private:
    param1 m_tag1;
    param2 m_tag2;
    param3 m_tag3;
    param4 m_tag4;
    param5 m_tag5;
    param6 m_tag6;
    param7 m_tag7;
    param8 m_tag8;
    param9 m_tag9;
    param10 m_tag10;
};


}}

#include <boost/logging/tag/defaults.hpp>

#endif


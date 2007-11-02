// tss_ensure_proper_delete.hpp

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


#ifndef JT28092007_tss_ensure_proper_delete_HPP_DEFINED
#define JT28092007_tss_ensure_proper_delete_HPP_DEFINED

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif

#include <boost/logging/detail/fwd.hpp>
#include <vector>

namespace boost { namespace logging { namespace detail {

struct do_delete_base {
    virtual ~do_delete_base () {}
};

template<class type> struct do_delete : do_delete_base {
    do_delete(type * val) : m_val(val) {}
    ~do_delete() { delete m_val; }
    type * m_val;
};

struct delete_array : std::vector< do_delete_base* > {
    ~delete_array () {
        for ( const_iterator b = begin(), e = end(); b != e; ++b)
            delete *b;
    }
};

inline delete_array & object_deleter() {
    static delete_array a;
    return a;
}

template<class type> inline type * new_object_ensure_delete() {
    type * val = new type;
    object_deleter().push_back( new do_delete<type>(val) );
    return val;
}

template<class type> inline type * new_object_ensure_delete(const type & init) {
    type * val = new type(init);
    object_deleter().push_back( new do_delete<type>(val) );
    return val;
}

}}}

#endif


// detail/util.hpp

// Boost Logging library
//
// Author: John Torjo, www.torjo.com
//
// Copyright (C) 2007 John Torjo (see www.torjo.com for email)
//
// Permission to copy, use, sell and distribute this software is granted
// provided this copyright notice appears in all copies.
// Permission to modify the code and to distribute modified code is granted
// provided this copyright notice appears in all copies, and a notice
// that the code was modified is included with the copyright notice.
//
// This software is provided "as is" without express or implied warranty,
// and with no claim as to its suitability for any purpose.
 
// See http://www.boost.org for updates, documentation, and revision history.
// See http://www.torjo.com/log2/ for more details

#ifndef JT28092007_detail_util_HPP_DEFINED
#define JT28092007_detail_util_HPP_DEFINED

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif

/* 
    make sure we don't need any of our headers included from here!
    we're included from fwd.hpp!
*/

namespace boost { namespace logging { 
    template<class type> struct type_as_arg {};


    /* 
        it makes sure this value gets created before it's used
    */
    template<class type> struct ensure_created_before_used {
        // FIXME I need to figure out how to do this - probably by just holding char[sizeof(T) * 2] or so - align it and that's it and have an extra bool
        ensure_created_before_used(const type & val = type() ) : val(val) {}
        type & operator()()         { return *this; }
        const type & operator()()   { return *this; }
    private:
        type val;
    };

}}

#endif


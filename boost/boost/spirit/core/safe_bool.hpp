/*=============================================================================
    Spirit v1.7.0
    Copyright (c) 2003 Joel de Guzman
    http://spirit.sourceforge.net/

    Permission to copy, use, modify, sell and distribute this software is
    granted provided this copyright notice appears in all copies. This
    software is provided "as is" without express or implied warranty, and
    with no claim as to its suitability for any purpose.
=============================================================================*/
#if !defined(BOOST_SPIRIT_SAFE_BOOL_HPP)
#define BOOST_SPIRIT_SAFE_BOOL_HPP

namespace boost { namespace spirit {

    struct safe_bool_impl
    { int* get() const { return 0; } };

    typedef int*(safe_bool_impl::*safe_bool)() const;

    inline safe_bool
    make_safe_bool(bool cond)
    { return cond ? &safe_bool_impl::get : 0; }
}}

#endif


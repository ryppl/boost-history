/*=============================================================================
    Copyright (c) 2009 Christopher Schmidt

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_SUPPORT_INTERNAL_SMALL_BIG_TYPE_HPP
#define BOOST_FUSION_SUPPORT_INTERNAL_SMALL_BIG_TYPE_HPP

namespace boost { namespace fusion { namespace detail
{
    typedef char small_type;
    struct big_type
    {
        char data[1024];
    };
}}}

#endif

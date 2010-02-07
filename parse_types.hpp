/*=============================================================================
    Copyright (c) 2002 2004 2006 Joel de Guzman
    Copyright (c) 2004 Eric Niebler
    http://spirit.sourceforge.net/

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#if !defined(BOOST_SPIRIT_QUICKBOOK_PARSE_TYPES_HPP)
#define BOOST_SPIRIT_QUICKBOOK_PARSE_TYPES_HPP

#include <string>
#include <boost/fusion/include/adapt_struct.hpp>
#include "fwd.hpp"

namespace quickbook
{
    typedef char const* formatted_type;

    struct formatted {
        formatted_type type;
        std::string content;
    };
}

BOOST_FUSION_ADAPT_STRUCT(
    quickbook::formatted,
    (quickbook::formatted_type, type)
    (std::string, content)
)

#endif
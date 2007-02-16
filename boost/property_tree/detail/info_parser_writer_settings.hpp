// ----------------------------------------------------------------------------
// Copyright (C) 2002-2006 Marcin Kalicinski
// Copyright (C) 2007 Alexey Baskakov
//
// Distributed under the Boost Software License, Version 1.0. 
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)
//
// For more information, see www.boost.org
// ----------------------------------------------------------------------------
#ifndef BOOST_PROPERTY_TREE_DETAIL_INFO_PARSER_WRITER_SETTINGS_HPP_INCLUDED
#define BOOST_PROPERTY_TREE_DETAIL_INFO_PARSER_WRITER_SETTINGS_HPP_INCLUDED

#include <string>

namespace boost { namespace property_tree { namespace info_parser
{
    //! Xml writer settings
    template <class Ch>
    class info_writer_settings
    {
    public:
        info_writer_settings(const std::basic_string<Ch> & indent = "    " ):
            indent ( indent )
        {
        }

        const std::basic_string<Ch> indent;
    };

    template <class Ch>
    info_writer_settings<Ch> info_writer_make_settings (const Ch * indent)
    {
        return info_writer_settings<Ch>(indent);
    }

} } }

#endif

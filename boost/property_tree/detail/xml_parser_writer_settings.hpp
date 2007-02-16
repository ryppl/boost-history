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
#ifndef BOOST_PROPERTY_TREE_DETAIL_XML_PARSER_WRITE_SETTINGS_HPP_INCLUDED
#define BOOST_PROPERTY_TREE_DETAIL_XML_PARSER_WRITE_SETTINGS_HPP_INCLUDED

#include <string>

namespace boost { namespace property_tree { namespace xml_parser
{
    //! Xml writer setting
    template <class Ch>
    class xml_writer_settings
    {
    public:
        xml_writer_settings(const std::basic_string<Ch> & indent = "    ",
                            const std::basic_string<Ch> & encoding = "utf-8" ):
            indent ( indent ),
            encoding ( encoding )
        {
        }

        const std::basic_string<Ch> indent;
        const std::basic_string<Ch> encoding;
    };

    template <class Ch>
	xml_writer_settings<Ch> xml_writer_make_settings (const Ch * indent, const Ch * encoding)
	{
		return xml_writer_settings<Ch>(indent,encoding);
	}

} } }

#endif

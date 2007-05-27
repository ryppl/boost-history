//
// explore.hpp - container printing.
//
// This version is highly untested and should at least be run on more than one
// compiler before considering for public view.
//
// Copyright (c) 2007 Danny Havenith
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
//
#ifndef __BOOST_EXPLORE_HTML_TABLE_FORMAT_HPP__
#define __BOOST_EXPLORE_HTML_TABLE_FORMAT_HPP__

#include <boost/explore/explore.hpp>
namespace explore
{
    //
    // support for html-table layout
    struct html_table_1_format : basic_range_format
    {
        static char_ *opening() { return "<table>\n<tr>"; }
        static char_ *closing() { return "</tr>\n</table>\n"; }
        static char_ *delimiter() { return "</tr>\n<tr>"; }
    };

    struct html_table_2_format : basic_range_format
    {
        static char_ *opening() { return "<td>"; }
        static char_ *closing() { return "</td>"; }
        static char_ *delimiter() { return "</td><td>"; }
    };

    //
    // a format selector that will return the same format
    // for every type.
    //
    template< typename range_format_>
    struct uniform_format_selector
    {
        template< typename T> 
        struct range_format : identity<range_format_> {};
    };

    struct html_table_format : boost::mpl::list<
        uniform_format_selector< html_table_1_format>,
        uniform_format_selector< html_table_2_format>,
        default_range_format_selector> 
    {};
}
#endif //__BOOST_EXPLORE_HTML_TABLE_FORMAT_HPP__
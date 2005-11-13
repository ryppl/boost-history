// ----------------------------------------------------------------------------
// Copyright (C) 2002-2005 Marcin Kalicinski
//
// Distributed under the Boost Software License, Version 1.0. 
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)
//
// For more information, see www.boost.org
// ----------------------------------------------------------------------------
#ifndef BOOST_PROPERTY_TREE_CMDLINE_PARSER_HPP_INCLUDED
#define BOOST_PROPERTY_TREE_CMDLINE_PARSER_HPP_INCLUDED

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ptree_utils.hpp>

namespace boost { namespace property_tree { namespace cmdline_parser
{

    template<class Ptree>
    void read_cmdline(int argc, 
                      typename Ptree::char_type *argv[], 
                      const std::basic_string<typename Ptree::char_type> &metachars,
                      Ptree &pt)
    {

        typedef typename Ptree::char_type Ch;
        typedef std::basic_string<Ch> Str;

        Ptree local;
        
        // For all arguments
        for (int i = 0; i < argc; ++i)
        {
            Str text = detail::trim<Ch>(argv[i]);
            if (!text.empty())
                if (metachars.find(text[0]) != Str::npos)
                {
                    if (text.size() == 1)
                        local.push_back(std::make_pair(text, Ptree()));
                    else if (text.size() == 2)
                        local.push_back(std::make_pair(text.substr(1, 1), Ptree()));
                    else
                        local.push_back(std::make_pair(text.substr(1, 1), 
                            Ptree(detail::trim<Ch>(text.substr(2, Str::npos)))));
                }
                else
                    local.push_back(std::make_pair(Str(), Ptree(text)));
        }

        // Swap local and pt
        pt.swap(local);

    }

} } }

namespace boost { namespace property_tree
{
    using cmdline_parser::read_cmdline;
} }

#endif

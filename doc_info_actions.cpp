/*=============================================================================
    Copyright (c) 2002 2004 2006 Joel de Guzman
    Copyright (c) 2004 Eric Niebler
    Copyright (c) 2010 Daniel James
    http://spirit.sourceforge.net/

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#include <string>
#include <vector>
#include <utility>
#include <boost/algorithm/string/join.hpp>
#include "fwd.hpp"
#include "collector.hpp"
#include "quickbook.hpp"
#include "doc_info_actions.hpp"
#include "state.hpp"
#include "utils.hpp"

namespace quickbook
{
    namespace 
    {
        struct empty_visitor {
            typedef bool result_type;
        
            template <typename T>
            bool operator()(T const& x) const {
                return x.empty();
            }
        };

        struct clear_visitor {
            typedef void result_type;
        
            template <typename T>
            void operator()(T& x) const {
                return x.clear();
            }
        };
    }

    doc_info process(quickbook::state& state, doc_info const& x)
    {
        doc_info info = x;
    
        // The doc_info in the file has been parsed. Here's what we'll do
        // *before* anything else.

        if(!info.doc_title.empty())
            state.doc_title = info.doc_title;

        if(info.doc_id.empty())
            info.doc_id = detail::make_identifier(
               state.doc_title.begin(),state.doc_title.end());

        if(state.doc_id.empty())
            state.doc_id = info.doc_id;

        if (info.doc_dirname.empty() && info.doc_type == "library")
            info.doc_dirname = state.doc_id;

        if (info.doc_last_revision.empty())
        {
            // default value for last-revision is now

            char strdate[64];
            strftime(
                strdate, sizeof(strdate),
                (debug_mode ?
                    "DEBUG MODE Date: %Y/%m/%d %H:%M:%S $" :
                    "$" /* prevent CVS substitution */ "Date: %Y/%m/%d %H:%M:%S $"),
                current_gm_time
            );
            info.doc_last_revision = strdate;
        }

        std::vector<std::string> invalid_attributes;

        if (info.doc_type != "library")
        {
            if (!boost::apply_visitor(empty_visitor(), info.doc_purpose))
            {
                boost::apply_visitor(clear_visitor(), info.doc_purpose);
                invalid_attributes.push_back("purpose");
            }

            if (!info.doc_categories.empty())
            {
                info.doc_categories.clear();
                invalid_attributes.push_back("category");
            }
        }

        if(!invalid_attributes.empty())
        {
            detail::outwarn(state.filename.file_string(),1)
                << (invalid_attributes.size() > 1 ?
                    "Invalid attributes" : "Invalid attribute")
                << " for '" << info.doc_type << "': "
                << boost::algorithm::join(invalid_attributes, ", ")
                << "\n"
                ;
        }

        return info;
    }
}

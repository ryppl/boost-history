/*=============================================================================
    Copyright (c) 2002 2004 2006 Joel de Guzman
    Copyright (c) 2004 Eric Niebler
    Copyright (c) 2005 Thomas Guest
    http://spirit.sourceforge.net/

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#include "actions.hpp"
#include "state.hpp"
#include "doc_info.hpp"
#include "utils.hpp"

namespace quickbook
{
    char const* quickbook_get_date = "__quickbook_get_date__";
    char const* quickbook_get_time = "__quickbook_get_time__";
    unsigned qbk_major_version = 0;
    unsigned qbk_minor_version = 0;
    unsigned qbk_version_n = 0; // qbk_major_version * 100 + qbk_minor_version

    actions::actions(state& state)
        : state_(state)
        , templates(state.templates)
        , macro(state.macro)
        , process(*this)
        , phrase_push(state.phrase)
        , phrase_pop(state.phrase)
        , block_push(state.block)
        , block_pop(state.block)
        , error(state.error_count)
    {}

    void error_action::operator()(iterator_range x, unused_type, unused_type) const
    {
        file_position const pos = x.begin().get_position();
        detail::outerr(pos.file,pos.line)
            << "Syntax Error near column " << pos.column << ".\n";
        ++error_count;
    }

    void element_id_warning_action::operator()(iterator_range x, unused_type, unused_type) const
    {
        file_position const pos = x.begin().get_position();
        detail::outwarn(pos.file,pos.line) << "Empty id.\n";        
    }

    void phrase_push_action::operator()() const
    {
        phrase.push();
    }

    std::string phrase_pop_action::operator()() const
    {
        std::string out;
        phrase.swap(out);
        phrase.pop();
        return out;
    }
}


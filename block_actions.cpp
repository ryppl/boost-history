/*=============================================================================
    Copyright (c) 2002 2004 2006 Joel de Guzman
    Copyright (c) 2004 Eric Niebler
    http://spirit.sourceforge.net/

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#include <numeric>
#include <boost/assert.hpp>
#include <boost/filesystem/convenience.hpp>
#include "block_actions.hpp"
#include "actions.hpp"
#include "state.hpp"
#include "quickbook.hpp"
#include "code_snippet_grammar.hpp"
#include "code_snippet_types.hpp"
#include "utils.hpp"

namespace quickbook
{
    namespace {
        raw_string fully_qualified_id(raw_string const& library_id,
            raw_string const& qualified_section_id,
            raw_string const& section_id)
        {
            raw_string id;
            id.value = library_id.value;
            if(!id.empty() && !qualified_section_id.empty()) id.value += '.';
            id.value += qualified_section_id.value;
            if(!id.empty() && !section_id.empty()) id.value += '.';
            id.value += section_id.value;
            return id;
        }
    }

    formatted process(quickbook::state& state, paragraph const& x)
    {
        formatted r;
        r.type="paragraph";
        r.content = x.content;
        return r;
    }

    begin_section2 process(quickbook::state& state, begin_section const& x)
    {
        // TODO: This uses the generated title.
        state.section_id.value = x.id ?
            x.id->value :
            detail::make_identifier(
                x.content.raw.begin(),
                x.content.raw.end());

        if (state.section_level != 0) {
            state.qualified_section_id.value += '.';
        }
        else {
            BOOST_ASSERT(state.qualified_section_id.empty());
        }

        state.qualified_section_id.value += state.section_id.value;
        ++state.section_level;

        begin_section2 r;

        if (qbk_version_n < 103) // version 1.2 and below
        {
            r.id.value = state.doc_id.value + "." + state.section_id.value;
        }
        else // version 1.3 and above
        {
            r.linkend.value = r.id.value =
                state.doc_id.value + "." + state.qualified_section_id.value;
        }
        
        r.content = x.content.content;
        
        return r;
    }

    end_section2 process(quickbook::state& state, end_section const& x)
    {
        if (state.section_level <= state.min_section_level)
        {
            detail::outerr(x.position.file,x.position.line)
                << "Mismatched [endsect] near column " << x.position.column << ".\n";
            ++state.error_count;
            
            // TODO: Return something else?
            return end_section2();
        }

        --state.section_level;

        if (state.section_level == 0)
        {
            state.qualified_section_id.value.clear();
        }
        else
        {
            std::string::size_type const n =
                state.qualified_section_id.value.find_last_of('.');
            state.qualified_section_id.value.erase(n, std::string::npos);
        }
        
        return end_section2();
    }

    heading2 process(quickbook::state& state, heading const& x)
    {
        heading2 r;

        // TODO: Is this right?
        bool new_style = qbk_version_n >= 103 || x.level > 0;
        
        r.level = x.level;
        if(r.level < 0) {
            r.level = state.section_level + 2;
                                                // section_level is zero-based. We need to use a
                                                // one-based heading which is one greater
                                                // than the current. Thus: section_level + 2.
            if (r.level > 6)                    // The max is h6, clip it if it goes
                r.level = 6;                    // further than that
        }

        if (!new_style) // version 1.2 and below
        {
            r.id.value = state.section_id.value + "." +
                detail::make_identifier(
                    x.content.content.begin(),
                    x.content.content.end());
        }
        else // version 1.3 and above
        {
            raw_string id;
            id.value = detail::make_identifier(
                    x.content.content.begin(),
                    x.content.content.end());
        
            r.linkend = r.id = fully_qualified_id(
                state.doc_id, state.qualified_section_id, id);

        }

        r.content = x.content.content;
        
        return r;
    }

    nothing process(quickbook::state& state, def_macro const& x)
    {
        state.macro.add(
            x.macro_identifier.begin()
          , x.macro_identifier.end()
          , quickbook::macro(x.content));
        return nothing();
    }

    nothing process(quickbook::state& state, define_template const& x)
    {
        if(!state.templates.add(x)) {
            detail::outerr(x.body.position.file, x.body.position.line)
                << "Template Redefinition: " << x.id << std::endl;
            ++state.error_count;
        }
        
        return nothing();
    }

    table2 process(quickbook::state& state, table const& x)
    {
        table2 r;

        if(!x.title.empty()) r.title = x.title;
        
        if(qbk_version_n >= 105) {
            if(x.id) {
                r.id = fully_qualified_id(state.doc_id,
                    state.qualified_section_id, *x.id);
            }
            else if(r.title) {
                raw_string id;
                id.value = detail::make_identifier(x.title.begin(), x.title.end());
            
                r.id = fully_qualified_id(state.doc_id,
                    state.qualified_section_id, id);
            }
        }
        
        r.cols = x.rows.empty() ? 0 : x.rows.back().size();

        std::vector<table_row>::const_iterator row = x.rows.begin();

        // Backwards compatability again: the old version turned the first row
        // into a header at the start of the second row. So it only happened
        // when there was more than one row.
        if (x.rows.size() > 1)
        {
            r.head = *row;
            ++row;
        }

        r.rows.assign(row, x.rows.end());

        return r;
    }

    namespace
    {
        int load_snippets(
            std::string const& file
          , std::vector<define_template>& storage   // for storing snippets are stored in a
                                                    // vector of define_templates
          , std::string const& extension
          , raw_string const& doc_id)
        {
            std::string code;
            int err = detail::load(file, code);
            if (err != 0)
                return err; // return early on error
    
            iterator first(code.begin(), code.end(), file);
            iterator last(code.end(), code.end());
    
            size_t fname_len = file.size();
            bool is_python = fname_len >= 3
                && file[--fname_len]=='y' && file[--fname_len]=='p' && file[--fname_len]=='.';
            code_snippet_actions a(storage, doc_id, is_python ? "[python]" : "[c++]");
            // TODO: Should I check that parse succeeded?
            if(is_python) {
                python_code_snippet_grammar g(a);
                boost::spirit::qi::parse(first, last, g);
            }
            else {
                cpp_code_snippet_grammar g(a);
                boost::spirit::qi::parse(first, last, g);
            }
    
            return 0;
        }

        fs::path include_search(fs::path const & current, std::string const & name)
        {
            fs::path path(name);

            // If the path is relative, try and resolve it.
            if (!path.is_complete())
            {
                // See if it can be found locally first.
                if (fs::exists(current / path))
                {
                    return current / path;
                }

                // Search in each of the include path locations.
                BOOST_FOREACH(std::string const & p, include_path)
                {
                    fs::path full(p);
                    full /= path;
                    if (fs::exists(full))
                    {
                        return full;
                    }
                }
            }

            return path;
        }

        fs::path path_difference(fs::path const& outdir, fs::path const& path)
        {
            fs::path outtmp, temp;
            fs::path::iterator out = outdir.begin(), file = path.begin();
            for(; out != outdir.end() && file != path.end(); ++out, ++file)
            {
                if(!fs::equivalent(outtmp /= *out, temp /= *file))
                    break;
            }
            out = (out == outdir.begin()) ? outdir.end() : out;
    
            fs::path result = fs::path();
            for(; out != outdir.end(); ++out)
                if(*out != ".") result /= "..";
            std::divides<fs::path> concat;
            return std::accumulate(file, path.end(), result, concat);
        }
    
        fs::path calculate_relative_path(std::string const& x, quickbook::state& state)
        {
            // Given a source file and the current filename, calculate the
            // path to the source file relative to the output directory.
            fs::path path(x);
            if (!path.is_complete())
            {
                fs::path infile = fs::complete(state.filename).normalize();
                path = (infile.parent_path() / path).normalize();
                fs::path outdir = fs::complete(state.outdir).normalize();
                path = path_difference(outdir, path);
            }
            return path;
        }
    }

    xinclude2 process(quickbook::state& state, xinclude const& x)
    {
        xinclude2 r;
        r.path = calculate_relative_path(detail::escape_uri(x.path), state).string();
        return r;
    }

    nothing process(quickbook::state& state, include const& x)
    {
        fs::path filein = include_search(state.filename.parent_path(), x.path);
        raw_string doc_id;

        // swap the filenames
        std::swap(state.filename, filein);

        // save the doc info strings
        if(qbk_version_n >= 106) {
            doc_id = state.doc_id;
        }
        else {
            std::swap(state.doc_id, doc_id);
        }
        
        // save the version info
        unsigned qbk_major_version_store = qbk_major_version;
        unsigned qbk_minor_version_store = qbk_minor_version;
        unsigned qbk_version_n_store = qbk_version_n;

        // scope the macros
        macro_symbols macro = state.macro;
        // scope the templates
        //~ template_symbols templates = state.templates; $$$ fixme $$$

        // if an id is specified in this include (as in [include:id foo.qbk])
        // then use it as the doc_id.
        if (x.id) state.doc_id = *x.id;

        // update the __FILENAME__ macro
        *state.macro.find("__FILENAME__") =
            quickbook::macro(state.filename.file_string());

        // parse the file
        quickbook::parse(state.filename.file_string().c_str(), state, true);

        // restore the values
        std::swap(state.filename, filein);

        std::swap(state.doc_id, doc_id);

        if(qbk_version_n >= 106 || qbk_version_n_store >= 106)
        {
            qbk_major_version = qbk_major_version_store;
            qbk_minor_version = qbk_minor_version_store;
            qbk_version_n = qbk_version_n_store;
        }

        // restore the macros
        state.macro = macro;
        // restore the templates
        //~ state.templates = templates; $$$ fixme $$$
        
        return nothing();
    }

    nothing process(quickbook::state& state, import const& x)
    {
        fs::path path = include_search(state.filename.parent_path(), x.path);
        std::string ext = path.extension();
        std::vector<define_template> storage;
        state.error_count +=
            load_snippets(path.string(), storage, ext, state.doc_id);

        BOOST_FOREACH(define_template const& definition, storage)
        {
            if (!state.templates.add(definition))
            {
                detail::outerr(definition.body.position.file, definition.body.position.line)
                    << "Template Redefinition: " << definition.id << std::endl;
                ++state.error_count;
            }
        }

        return nothing();
    }
}
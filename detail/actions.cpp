/*=============================================================================
    Copyright (c) 2002 2004 2006 Joel de Guzman
    Copyright (c) 2004 Eric Niebler
    Copyright (c) 2005 Thomas Guest
    http://spirit.sourceforge.net/

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#include <numeric>
#include <functional>
#include <algorithm>
#include <iterator>
#include <boost/filesystem/convenience.hpp>
#include <boost/filesystem/fstream.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/spirit/include/support_unused.hpp>
#include "./quickbook.hpp"
#include "./actions.hpp"
#include "./utils.hpp"
#include "./markups.hpp"
#include "./actions_class.hpp"
#include "../grammars.hpp"
#include "../code_snippet.hpp"

namespace quickbook
{
    char const* quickbook_get_date = "__quickbook_get_date__";
    char const* quickbook_get_time = "__quickbook_get_time__";
    unsigned qbk_major_version = 0;
    unsigned qbk_minor_version = 0;
    unsigned qbk_version_n = 0; // qbk_major_version * 100 + qbk_minor_version

    namespace {
        std::string fully_qualified_id(std::string const& library_id,
            std::string const& qualified_section_id,
            std::string const& section_id)
        {
            std::string id = library_id;
            if(!id.empty() && !qualified_section_id.empty()) id += '.';
            id += qualified_section_id;
            if(!id.empty() && !section_id.empty()) id += '.';
            id += section_id;
            return id;
        }
    }

    void error_action::operator()(iterator_range x, unused_type, unused_type) const
    {
        boost::spirit::classic::file_position const pos = x.begin().get_position();
        detail::outerr(pos.file,pos.line)
            << "Syntax Error near column " << pos.column << ".\n";
        ++error_count;
    }

    void span::operator()(iterator_range x, unused_type, unused_type) const
    {
        iterator first = x.begin(), last = x.end();
        out << "<phrase role=\"" << name << "\">";
        while (first != last)
            detail::print_char(*first++, out.get());
        out << "</phrase>";
    }

    void unexpected_char::operator()(iterator_range x, unused_type, unused_type) const
    {
        iterator first = x.begin(), last = x.end();
        boost::spirit::classic::file_position const pos = first.get_position();

        detail::outwarn(pos.file, pos.line)
            << "in column:" << pos.column
            << ", unexpected character: " << std::string(first, last)
            << "\n";

        // print out an unexpected character
        out << "<phrase role=\"error\">";
        while (first != last)
            detail::print_char(*first++, out.get());
        out << "</phrase>";
    }

    void space::operator()(char ch, unused_type, unused_type) const
    {
        detail::print_space(ch, out.get());
    }

    void space::operator()(iterator_range x, unused_type, unused_type) const
    {
        iterator first = x.begin(), last = x.end();
        while (first != last)
            detail::print_space(*first++, out.get());
    }

    void code_action::operator()(iterator_range x, unused_type, unused_type) const
    {
        // preprocess the code section to remove the initial indentation
        std::string program(x.begin(), x.end());
        detail::unindent(program);
        if (program.size() == 0)
            return; // Nothing left to do here. The program is empty.

        iterator first_(program.begin(), program.end());
        iterator last_(program.end(), program.end());
        first_.set_position(x.begin().get_position());

        std::string save;
        phrase.swap(save);

        // print the code with syntax coloring
        std::string str = syntax_p(first_, last_);

        phrase.swap(save);

        //
        // We must not place a \n after the <programlisting> tag
        // otherwise PDF output starts code blocks with a blank line:
        //
        out << "<programlisting>";
        out << str;
        out << "</programlisting>\n";
    }

    void raw_char_action::operator()(char ch, unused_type, unused_type) const
    {
        phrase << ch;
    }

    void raw_char_action::operator()(iterator_range x, unused_type, unused_type) const
    {
        phrase << *x.begin();
    }

    void plain_char_action::operator()(char ch, unused_type, unused_type) const
    {
        detail::print_char(ch, phrase.get());
    }

    void plain_char_action::operator()(iterator_range x, unused_type, unused_type) const
    {
        detail::print_char(*x.begin(), phrase.get());
    }

    void element_id_warning_action::operator()(iterator_range x, unused_type, unused_type) const
    {
        boost::spirit::classic::file_position const pos = x.begin().get_position();
        detail::outwarn(pos.file,pos.line) << "Empty id.\n";        
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

    fs::path calculate_relative_path(
        iterator first, iterator last, quickbook::actions& actions)
    {
        // Given a source file and the current filename, calculate the
        // path to the source file relative to the output directory.
        fs::path path(std::string(first, last));
        if (!path.is_complete())
        {
            fs::path infile = fs::complete(actions.filename).normalize();
            path = (infile.branch_path() / path).normalize();
            fs::path outdir = fs::complete(actions.outdir).normalize();
            path = path_difference(outdir, path);
        }
        return path;
    }

    void xinclude_action::operator()(iterator_range x, unused_type, unused_type) const
    {
        fs::path path = calculate_relative_path(x.begin(), x.end(), actions);
        out << "\n<xi:include href=\"";
        detail::print_string(detail::escape_uri(path.string()), out.get());
        out << "\" />\n";
    }

    void code_snippet_actions::pass_thru(char x)
    {
        code += x;
    }

    namespace detail
    {
        int callout_id = 0;
    }

    void code_snippet_actions::callout(std::string const& x, char const* role)
    {
        using detail::callout_id;
        code += "``'''";
        code += std::string("<phrase role=\"") + role + "\">";
        code += "<co id=\"";
        code += doc_id + boost::lexical_cast<std::string>(callout_id + callouts.size()) + "co\" ";
        code += "linkends=\"";
        code += doc_id + boost::lexical_cast<std::string>(callout_id + callouts.size()) + "\" />";
        code += "</phrase>";
        code += "'''``";

        callouts.push_back(x);
    }

    void code_snippet_actions::inline_callout(std::string const& x)
    {
        callout(x, "callout_bug");
    }

    void code_snippet_actions::line_callout(std::string const& x)
    {
        callout(x, "line_callout_bug");
    }

    void code_snippet_actions::escaped_comment(std::string const& x)
    {
        if (!code.empty())
        {
            detail::unindent(code); // remove all indents
            if (code.size() != 0)
            {
                snippet += "\n\n";
                snippet += source_type;
                snippet += "``\n" + code + "``\n\n";
                code.clear();
            }
        }
        std::string temp(x);
        detail::unindent(temp); // remove all indents
        if (temp.size() != 0)
        {
            snippet += "\n" + temp; // add a linebreak to allow block marskups
        }
    }

    void code_snippet_actions::compile(boost::iterator_range<iterator> x)
    {
        using detail::callout_id;
        if (!code.empty())
        {
            detail::unindent(code); // remove all indents
            if (code.size() != 0)
            {
                snippet += "\n\n";
                snippet += source_type;
                snippet += "```\n" + code + "```\n\n";
            }

            if(callouts.size() > 0)
            {
              snippet += "'''<calloutlist>'''";
              for (size_t i = 0; i < callouts.size(); ++i)
              {
                  snippet += "'''<callout arearefs=\"";
                  snippet += doc_id + boost::lexical_cast<std::string>(callout_id + i) + "co\" ";
                  snippet += "id=\"";
                  snippet += doc_id + boost::lexical_cast<std::string>(callout_id + i) + "\">";
                  snippet += "'''";

                  snippet += "'''<para>'''";
                  snippet += callouts[i];
                  snippet += "'''</para>'''";
                  snippet += "'''</callout>'''";
              }
              snippet += "'''</calloutlist>'''";
            }
        }

        std::vector<std::string> tinfo;
        tinfo.push_back(id);
        tinfo.push_back(snippet);
        storage.push_back(template_symbol(tinfo, x.begin().get_position()));

        callout_id += callouts.size();
        callouts.clear();
        code.clear();
        snippet.clear();
        id.clear();
    }

    int load_snippets(
        std::string const& file
      , std::vector<template_symbol>& storage   // snippets are stored in a
                                                // vector of template_symbols
      , std::string const& extension
      , std::string const& doc_id)
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

    namespace
    {
        fs::path include_search(fs::path const & current, std::string const & name)
        {
            fs::path path(name,fs::native);

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
                    fs::path full(p,fs::native);
                    full /= path;
                    if (fs::exists(full))
                    {
                        return full;
                    }
                }
            }

            return path;
        }
    }

    void import_action::operator()(iterator_range x, unused_type, unused_type) const
    {
        fs::path path = include_search(actions.filename.branch_path(), std::string(x.begin(), x.end()));
        std::string ext = fs::extension(path);
        std::vector<template_symbol> storage;
        actions.error_count +=
            load_snippets(path.string(), storage, ext, actions.doc_id);

        BOOST_FOREACH(template_symbol const& ts, storage)
        {
            std::string tname = boost::get<0>(ts)[0];
            if (actions.templates.find_top_scope(tname))
            {
                boost::spirit::classic::file_position const pos = boost::get<1>(ts);
                detail::outerr(pos.file, pos.line)
                    << "Template Redefinition: " << tname << std::endl;
                ++actions.error_count;
            }
            else
            {
                actions.templates.add(tname, ts);
            }
        }
    }

    void include_action::operator()(iterator_range x, unused_type, unused_type) const
    {
        fs::path filein = include_search(actions.filename.branch_path(), std::string(x.begin(), x.end()));
        std::string doc_type, doc_id, doc_dirname, doc_last_revision;

        // swap the filenames
        std::swap(actions.filename, filein);

        // save the doc info strings
        actions.doc_type.swap(doc_type);
        actions.doc_id.swap(doc_id);
        actions.doc_dirname.swap(doc_dirname);
        actions.doc_last_revision.swap(doc_last_revision);

        // scope the macros
        macro_symbols macro = actions.macro;
        // scope the templates
        //~ template_symbols templates = actions.templates; $$$ fixme $$$

        // if an id is specified in this include (as in [include:id foo.qbk])
        // then use it as the doc_id.
        if (!actions.include_doc_id.empty())
        {
            actions.doc_id = actions.include_doc_id;
            actions.include_doc_id.clear();
        }

        // update the __FILENAME__ macro
        *actions.macro.find("__FILENAME__") =
            quickbook::macro(actions.filename.native_file_string());

        // parse the file
        quickbook::parse(actions.filename.native_file_string().c_str(), actions, true);

        // restore the values
        std::swap(actions.filename, filein);

        actions.doc_type.swap(doc_type);
        actions.doc_id.swap(doc_id);
        actions.doc_dirname.swap(doc_dirname);
        actions.doc_last_revision.swap(doc_last_revision);

        // restore the macros
        actions.macro = macro;
        // restore the templates
        //~ actions.templates = templates; $$$ fixme $$$
    }

    void xml_author::operator()(std::pair<std::string, std::string> const& author) const
    {
        out << "      <author>\n"
            << "        <firstname>" << author.first << "</firstname>\n"
            << "        <surname>" << author.second << "</surname>\n"
            << "      </author>\n";
    }

    void xml_copyright::operator()(std::pair<std::vector<std::string>, std::string> const& copyright) const
    {
        out << "\n" << "    <copyright>\n";

        for_each(
            copyright.first.begin()
          , copyright.first.end()
          , xml_year(out));

        out << "      <holder>" << copyright.second << "</holder>\n"
            << "    </copyright>\n"
            << "\n"
        ;
    }

    void xml_year::operator()(std::string const &year) const
    {
        out << "      <year>" << year << "</year>\n";
    }

    static void write_document_title(collector& out, quickbook::actions& actions);
    static void write_document_info(collector& out, quickbook::actions& actions);

    void pre(collector& out, quickbook::actions& actions, bool ignore_docinfo)
    {
        // The doc_info in the file has been parsed. Here's what we'll do
        // *before* anything else.

        if (actions.doc_id.empty())
            actions.doc_id = detail::make_identifier(
                actions.doc_title.begin(),actions.doc_title.end());

        if (actions.doc_dirname.empty() && actions.doc_type == "library")
            actions.doc_dirname = actions.doc_id;

        if (actions.doc_last_revision.empty())
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
            actions.doc_last_revision = strdate;
        }

        // if we're ignoring the document info, we're done.
        if (ignore_docinfo)
        {
            return;
        }

        if (qbk_major_version == 0)
        {
            // hard code quickbook version to v1.1
            qbk_major_version = 1;
            qbk_minor_version = 1;
            qbk_version_n = 101;
            detail::outwarn(actions.filename.native_file_string(),1)
                << "Warning: Quickbook version undefined. "
                "Version 1.1 is assumed" << std::endl;
        }
        else
        {
            qbk_version_n = (qbk_major_version * 100) + qbk_minor_version;
        }

        out << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
            << "<!DOCTYPE library PUBLIC \"-//Boost//DTD BoostBook XML V1.0//EN\"\n"
            << "     \"http://www.boost.org/tools/boostbook/dtd/boostbook.dtd\">\n"
            << '<' << actions.doc_type << "\n"
            << "    id=\"" << actions.doc_id << "\"\n";
        
        if(actions.doc_type == "library")
        {
            out << "    name=\"" << actions.doc_title << "\"\n";
        }

        if(!actions.doc_dirname.empty())
        {
            out << "    dirname=\"" << actions.doc_dirname << "\"\n";
        }

        out << "    last-revision=\"" << actions.doc_last_revision << "\" \n"
            << "    xmlns:xi=\"http://www.w3.org/2001/XInclude\">\n";
            
        if(actions.doc_type == "library") {
            write_document_info(out, actions);
            write_document_title(out, actions);
        }
        else {
            write_document_title(out, actions);
            write_document_info(out, actions);
        }
    }
    
    void post(collector& out, quickbook::actions& actions, bool ignore_docinfo)
    {
        // if we're ignoring the document info, do nothing.
        if (ignore_docinfo)
        {
            return;
        }

        // We've finished generating our output. Here's what we'll do
        // *after* everything else.
        out << "\n</" << actions.doc_type << ">\n\n";
    }

    void write_document_title(collector& out, quickbook::actions& actions)
    {
        if (!actions.doc_title.empty())
        {
            out<< "  <title>" << actions.doc_title;
            if (!actions.doc_version.empty())
                out << ' ' << actions.doc_version;
            out<< "</title>\n\n\n";
        }
    }

    void write_document_info(collector& out, quickbook::actions& actions)
    {
        out << "  <" << actions.doc_type << "info>\n";

        if(!actions.doc_authors.empty())
        {
            out << "    <authorgroup>\n";
            for_each(
                actions.doc_authors.begin()
              , actions.doc_authors.end()
              , xml_author(out));
            out << "    </authorgroup>\n";
        }

        if (!actions.doc_copyrights.empty())
        {
            for_each(
                actions.doc_copyrights.begin()
              , actions.doc_copyrights.end()
              , xml_copyright(out));
        }

        if (qbk_version_n < 103)
        {
            // version < 1.3 compatibility
            actions.doc_license = actions.doc_license_1_1;
            actions.doc_purpose = actions.doc_purpose_1_1;
        }

        if (!actions.doc_license.empty())
        {
            out << "    <legalnotice>\n"
                << "      <para>\n"
                << "        " << actions.doc_license << "\n"
                << "      </para>\n"
                << "    </legalnotice>\n"
                << "\n"
            ;
        }

        if (!actions.doc_purpose.empty())
        {
            out << "    <" << actions.doc_type << "purpose>\n"
                << "      " << actions.doc_purpose
                << "    </" << actions.doc_type << "purpose>\n"
                << "\n"
            ;
        }

        if (!actions.doc_category.empty())
        {
            out << "    <" << actions.doc_type << "category name=\"category:"
                << actions.doc_category
                << "\"></" << actions.doc_type << "category>\n"
                << "\n"
            ;
        }

        out << "  </" << actions.doc_type << "info>\n"
            << "\n"
        ;
    }

    void phrase_push_action::operator()(unused_type, unused_type, unused_type) const
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

    void phrase_to_string_action::operator()(unused_type, unused_type, unused_type) const
    {
        phrase.swap(out);
    }
    
    void output_action::operator()(unused_type, unused_type, unused_type) const
    {
        std::string out;
        actions.phrase.swap(out);
        actions.out << out;
    }

}


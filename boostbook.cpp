#include <algorithm>
#include <boost/foreach.hpp>
#include "encoder_impl.hpp"

namespace quickbook
{
    template <typename Iter>
    std::string encode_impl(Iter first, Iter last)
    {
        std::string r;
        
        for(;first != last; ++first) {
            switch (*first)
            {
                case '<': r += "&lt;";    break;
                case '>': r += "&gt;";    break;
                case '&': r += "&amp;";   break;
                case '"': r += "&quot;";  break;
                default:  r += *first;    break;
            }
        }
        
        return r;
    }

    std::string boostbook_encoder::encode(raw_string const& x)
    {
        return encode_impl(x.begin(), x.end());
    }

    std::string boostbook_encoder::encode(std::string const& x)
    {
        return encode_impl(x.begin(), x.end());
    }

    std::string boostbook_encoder::encode(char const* x)
    {
        char const* end = x;
        while(*end) ++end;
        return encode_impl(x, end);
    }

    std::string boostbook_encoder::encode(char c)
    {
        return encode_impl(&c, &c + 1);
    }

   namespace {
        struct boostbook_markup {
            char const* quickbook;
            char const* pre;
            char const* post;
        };
    
        boostbook_markup markups[] = {
            { "", "", "" },
            { "paragraph", "<para>\n", "</para>\n" },
            { "h1", "<bridgehead renderas=\"sect1\">", "</bridgehead>" },
            { "h2", "<bridgehead renderas=\"sect2\">", "</bridgehead>" },
            { "h3", "<bridgehead renderas=\"sect3\">", "</bridgehead>" },
            { "h4", "<bridgehead renderas=\"sect4\">", "</bridgehead>" },
            { "h5", "<bridgehead renderas=\"sect5\">", "</bridgehead>" },
            { "h6", "<bridgehead renderas=\"sect6\">", "</bridgehead>" },
            { "blurb", "<sidebar role=\"blurb\">\n", "</sidebar>\n" },
            { "blockquote", "<blockquote>", "</blockquote>" },
            { "preformatted", "<programlisting>", "</programlisting>" },
            { "warning", "<warning>", "</warning>" },
            { "caution", "<caution>", "</caution>" },
            { "important", "<important>", "</important>" },
            { "note", "<note>", "</note>" },
            { "tip", "<tip>", "</tip>" },
            { "list_item", "<listitem><simpara>\n", "\n</simpara></listitem>" },
            { "bold", "<emphasis role=\"bold\">", "</emphasis>" },
            { "italic", "<emphasis>", "</emphasis>" },
            { "underline", "<emphasis role=\"underline\">", "</emphasis>" },
            { "teletype", "<literal>", "</literal>" },
            { "strikethrough", "<emphasis role=\"strikethrough\">", "</emphasis>" },
            { "quote", "<quote>", "</quote>" },
            { "url", "<ulink url=\"", "</ulink>" },
            { "link", "<link linkend=\"", "</link>" },
            { "funcref", "<functionname alt=\"", "</functionname>" },
            { "classref", "<classname alt=\"", "</classname>" },
            { "memberref", "<methodname alt=\"", "</methodname>" },
            { "enumref", "<enumname alt=\"", "</enumname>" },
            { "macroref", "<macroname alt=\"", "</macroname>" },
            { "headerref", "<headername alt=\"", "</headername>" },
            { "conceptref", "<conceptname alt=\"", "</conceptname>" },
            { "globalref", "<globalname alt=\"", "</globalname>" },
            { "footnote", "<footnote><para>", "</para></footnote>" },
            { "escape", "<!--quickbook-escape-prefix-->", "<!--quickbook-escape-postfix-->" },
            { "replaceable", "<replaceable>", "</replaceable>" },
            { "varlistentry", "<varlistentry>", "</varlistentry>\n" },
            { "varlistterm", "<term>", "</term>" },
            { "varlistitem", "<listitem>", "</listitem>" },
            { "header", "<thead>", "</thead>\n" },
            { "row", "<row>", "</row>\n" },
            { "cell", "<entry>", "</entry>" },
            { "programlisting", "<programlisting>", "</programlisting>\n" },
            { "code", "<code>", "</code>" },
            { "hr", "<para/>", "" },
            { "break", "<sbr/>", "" },
        };
        
        std::map<std::string, boostbook_markup> markup_map;
        
        struct initialize {
            initialize() {
                BOOST_FOREACH(boostbook_markup m, markups) {
                    markup_map[m.quickbook] = m;
                }
            }
        } initialize_instance;
        
        boostbook_markup get_markup(std::string const& x) {
            std::map<std::string, boostbook_markup>::const_iterator
                pos = markup_map.find(x);
            BOOST_ASSERT(pos != markup_map.end());
            return pos->second;
        }
    }

    void boostbook_encoder::operator()(quickbook::state& state, std::string const& x)
    {
        state.phrase << x;
    }

    void boostbook_encoder::operator()(quickbook::state& state, char x)
    {
        state.phrase << encode(x);
    }

    void boostbook_encoder::operator()(quickbook::state& state, unicode_char const& x)
    {
        std::string::const_iterator first = x.value.begin(), last = x.value.end();
        while(first != last && *first == '0') ++first;

        // Just ignore \u0000
        // Maybe I should issue a warning?
        if(first == last) return;
        
        std::string hex_digits(first, last);
        
        if(hex_digits.size() == 2 && *first > '0' && *first <= '7') {
            using namespace std;
            (*this)(state, strtol(hex_digits.c_str(), 0, 16));
        }
        else {
            state.phrase << "&#x" << hex_digits << ";";
        }
    }

     void boostbook_encoder::operator()(quickbook::state& state, anchor const& x)
    {
        state.phrase << "<anchor id=\"";
        state.phrase << encode(x.id);
        state.phrase << "\"/>\n";
    }

    void boostbook_encoder::operator()(quickbook::state& state, link const& x)
    {
        boostbook_markup m = get_markup(x.type);
        state.phrase << m.pre;
        state.phrase << encode(x.destination);
        state.phrase << "\">";
        state.phrase << x.content;
        state.phrase << m.post;
    }

    void boostbook_encoder::operator()(quickbook::state& state, formatted const& x)
    {
        boostbook_markup m = get_markup(x.type);
        state.phrase << m.pre << x.content << m.post;
    }

    void boostbook_encoder::operator()(quickbook::state& state, block_formatted const& x)
    {
        boostbook_markup m = get_markup(x.type);
        state.block << m.pre << x.content << m.post;
    }

    void boostbook_encoder::operator()(quickbook::state& state, break_ const& x)
    {
        boostbook_markup m = get_markup("break");
        state.phrase << m.pre;
    }

    void boostbook_encoder::operator()(quickbook::state& state, image2 const& x)
    {
        state.phrase << "<inlinemediaobject>";

        state.phrase << "<imageobject><imagedata";
        
        for(image2::attribute_map::const_iterator
            attr_first = x.attributes.begin(), attr_last  = x.attributes.end();
            attr_first != attr_last; ++attr_first)
        {
            if(attr_first->first == "alt") continue;
        
            state.phrase
                << " "
                << attr_first->first
                << "=\""
                << encode(attr_first->second)
                << "\"";
        }

        state.phrase << "></imagedata></imageobject>";

        image2::attribute_map::const_iterator it = x.attributes.find("alt");
        if(it != x.attributes.end()) {
            // Also add a textobject -- use the basename of the image file.
            // This will mean we get "alt" attributes of the HTML img.
            state.phrase << "<textobject><phrase>";
            state.phrase << encode(it->second);
            state.phrase << "</phrase></textobject>";
        }

        state.phrase << "</inlinemediaobject>";
    }

    void boostbook_encoder::operator()(quickbook::state& state, hr)
    {
        state.block << get_markup("hr").pre;
    }

    void boostbook_encoder::operator()(quickbook::state& state, begin_section2 const& x)
    {
        state.block << "\n<section id=\"" << encode(x.id) << "\">\n";
        if(x.linkend.empty()) {
            state.block
                << "<title>"
                << x.content
                << "</title>\n"
                ;
        }
        else {
            state.block
                << "<title>"
                << "<link linkend=\""
                << encode(x.linkend)
                << "\">"
                << x.content
                << "</link>"
                << "</title>\n"
                ;
        }
    }

    void boostbook_encoder::operator()(quickbook::state& state, end_section2 const& x)
    {
        state.block << "</section>";
    }

    void boostbook_encoder::operator()(quickbook::state& state, heading2 const& x)
    {
        state.block
            << "<anchor id=\"" << encode(x.id) << "\"/>"
            << "<bridgehead renderas=\"sect" << x.level << "\">";

        if(x.linkend.empty()) {
            state.block << x.content;
        }
        else {
            state.block
                << "<link linkend=\"" << encode(x.linkend) << "\">"
                << x.content << "</link>";
        }

        state.block << "</bridgehead>";
    }

    void boostbook_encoder::operator()(quickbook::state& state, variablelist const& x)
    {
        state.block << "<variablelist>\n";

        state.block << "<title>";
        state.block << encode(x.title);
        state.block << "</title>\n";

        boostbook_markup m = get_markup("varlistentry");

        for(std::vector<varlistentry>::const_iterator
            it = x.entries.begin(); it != x.entries.end(); ++it)
        {
            state.block << m.pre;

            std::vector<quickbook::block_formatted>::const_iterator
                it2 = it->begin(), end2 = it->end();

            encode_action encode(state, *this);
            for(;it2 != end2 && std::string(it2->type) == "varlistterm"; ++it2) {
                encode(*it2);
            }
            
            if(it2 != end2) {
                std::string type = "varlistitem";
                boostbook_markup m2 = get_markup(type);
                state.block << m2.pre;

                for(;it2 != end2; ++it2) {
                    BOOST_ASSERT(type == it2->type);
                    state.block << it2->content;
                }

                state.block << m2.post;
            }

            state.block << m.post;
        }

        state.block << "</variablelist>\n";
    }

    void boostbook_encoder::operator()(quickbook::state& state, table2 const& x)
    {
        if (x.title)
        {
            state.block << "<table frame=\"all\"";
            if(x.id)
                state.block << " id=\"" << encode(*x.id) << "\"";
            state.block << ">\n";
            state.block << "<title>";
            state.block << encode(*x.title);
            state.block << "</title>";
        }
        else
        {
            state.block << "<informaltable frame=\"all\"";
            if(x.id)
                state.block << " id=\"" << encode(*x.id) << "\"";
            state.block << ">\n";
        }

        // This is a bit odd for backwards compatability: the old version just
        // used the last count that was calculated.
        state.block << "<tgroup cols=\"" << x.cols << "\">\n";

        boostbook_markup m = get_markup("row");

        if (x.head)
        {
            state.block << "<thead>";
            state.block << m.pre;
            std::for_each(x.head->begin(), x.head->end(), encode_action(state, *this));
            state.block << m.post;
            state.block << "</thead>\n";
        }

        state.block << "<tbody>\n";

        for(std::vector<table_row>::const_iterator
            it = x.rows.begin(); it != x.rows.end(); ++it)
        {
            state.block << m.pre;
            std::for_each(it->begin(), it->end(), encode_action(state, *this));
            state.block << m.post;
        }

        state.block << "</tbody>\n" << "</tgroup>\n";

        if (x.title)
        {
            state.block << "</table>\n";
        }
        else
        {
            state.block << "</informaltable>\n";
        }
    }

    void boostbook_encoder::operator()(quickbook::state& state, xinclude2 const& x)
    {
        state.block << "\n<xi:include href=\"" << x.path << "\" />\n";
    }

    void boostbook_encoder::operator()(quickbook::state& state, list2 const& x)
    {
        state.block << std::string(x.mark == '#' ? "<orderedlist>\n" : "<itemizedlist>\n");

        for(std::vector<list_item2>::const_iterator
            it = x.items.begin(), end = x.items.end(); it != end; ++it)
        {
            state.block << "<listitem><simpara>\n" << it->content << "\n</simpara>";
            if(!it->sublist.items.empty()) (*this)(state, it->sublist);
            state.block << std::string("</listitem>");
        }

        state.block << std::string(x.mark == '#' ? "\n</orderedlist>" : "\n</itemizedlist>");
    }

    void boostbook_encoder::operator()(quickbook::state& state, callout_link const& x)
    {
        state.phrase
            << "<co id=\"" << x.identifier << "co\""
            << " linkends=\"" << x.identifier << "\""
            << " />"
            ;
    }

    void boostbook_encoder::operator()(quickbook::state& state, callout_list const& x)
    {
        state.block
            << "<calloutlist>";

        BOOST_FOREACH(callout_item const& c, x)
        {
            state.block
                << "<callout arearefs=\"" << c.identifier << "co\""
                << " id=\"" << c.identifier << "\""
                << ">"
                << c.content
                << "</callout>";
        }

        state.block
            << "</calloutlist>";
    }

    void boostbook_encoder::operator()(quickbook::state& state, code_token const& x)
    {
        std::string type = x.type;
        if(type == "space") {
            state.phrase << x.text;
        }
        else {
            state.phrase
                << "<phrase role=\"" << x.type << "\">"
                << encode(x.text)
                << "</phrase>";
        }
    }

    void boostbook_encoder::operator()(quickbook::state& state, doc_info const& info)
    {
        // if we're ignoring the document info, we're done.
        if (info.ignore) return;

        state.block
            << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
            << "<!DOCTYPE "
            << info.doc_type
            << " PUBLIC \"-//Boost//DTD BoostBook XML V1.0//EN\""
            << " \"http://www.boost.org/tools/boostbook/dtd/boostbook.dtd\">";

        // Document tag

        state.block
            << '<' << info.doc_type << " id=\"" << info.doc_id.get(106) << "\"\n";
        
        if(info.doc_type == "library")
        {
            state.block << " name=\"" << info.doc_title.get(106) << "\"\n";
        }

        if(!info.doc_dirname.empty())
        {
            state.block << " dirname=\"" << info.doc_dirname.get(106) << "\"\n";
        }

        state.block
            << "last-revision=\"" << info.doc_last_revision.get(106) << "\""
            << " xmlns:xi=\"http://www.w3.org/2001/XInclude\"";

        state.block << ">"; // end document tag.

        // Title tag

        std::string title;
        if(!info.doc_title.empty())
        {
            title =  "<title>" + info.doc_title.get(106);
            if (!info.doc_version.empty())
                title += ' ' + info.doc_version.get(106);
            title += "</title>\n";
        }

        // For 'library', the title comes after the info block.
        if(info.doc_type != "library") state.block << title;

        // Info tag

        state.block << "<" << info.doc_type << "info>\n";

        if(!info.doc_authors.empty())
        {
            state.block << "<authorgroup>\n";
            BOOST_FOREACH(doc_info::author const& author, info.doc_authors) {
                state.block
                    << "<author>\n"
                    << "<firstname>" << author.firstname.get(106) << "</firstname>\n"
                    << "<surname>" << author.surname.get(106) << "</surname>\n"
                    << "</author>\n";
            }
            state.block << "</authorgroup>\n";
        }

        BOOST_FOREACH(doc_info::copyright_entry const& copyright,
            info.doc_copyrights)
        {
            state.block << "<copyright>\n";

            BOOST_FOREACH(unsigned int year, copyright.years) {
                state.block << "<year>" << year << "</year>\n";
            }

            state.block
                << "<holder>" << copyright.holder.get(106) << "</holder>\n"
                << "</copyright>\n"
            ;
        }

        if (!info.doc_license.empty())
        {
            state.block
                << "<legalnotice>\n"
                << "<para>\n"
                << info.doc_license.get(103)
                << "\n"
                << "</para>\n"
                << "</legalnotice>\n"
                << "\n"
            ;
        }

        if (!info.doc_purpose.empty())
        {
            state.block
                << "<" << info.doc_type << "purpose>\n"
                << info.doc_purpose.get(103)
                << "</" << info.doc_type << "purpose>\n"
                << "\n"
            ;
        }

        BOOST_FOREACH(docinfo_string const& category, info.doc_categories)
        {
            state.block
                << "<" << info.doc_type << "category name=\"category:"
                << category.get(106)
                << "\"></" << info.doc_type << "category>\n"
                << "\n"
            ;
        }

        state.block
            << "</" << info.doc_type << "info>\n"
        ;

        if(info.doc_type == "library") state.block << title;
    }

    void boostbook_encoder::operator()(quickbook::state& state, doc_info_post const& x)
    {
        // if we're ignoring the document info, do nothing.
        if (x.info.ignore) return;

        // We've finished generating our output. Here's what we'll do
        // *after* everything else.
        state.block << "</" << x.info.doc_type << ">";
    }
}

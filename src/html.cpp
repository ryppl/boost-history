#include <algorithm>
#include <boost/foreach.hpp>
#include <boost/assert.hpp>
#include "encoder_impl.hpp"

namespace quickbook
{
    html_encoder::html_encoder() : footnote_id(0) {}
    html_encoder::~html_encoder() {}

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

    std::string html_encoder::encode(raw_string const& x)
    {
        return encode_impl(x.begin(), x.end());
    }

    std::string html_encoder::encode(std::string const& x)
    {
        return encode_impl(x.begin(), x.end());
    }

    std::string html_encoder::encode(char const* x)
    {
        char const* end = x;
        while(*end) ++end;
        return encode_impl(x, end);
    }

    std::string html_encoder::encode(char c)
    {
        return encode_impl(&c, &c + 1);
    }

    namespace {
        struct html_markup {
            char const* quickbook;
            char const* pre;
            char const* post;
            bool block;
        };
    
        html_markup markups[] = {
            { "", "", "" },
            { "paragraph", "<p>\n", "</p>\n" },
            { "blurb", "<div class=\"blurb\">\n", "</div>\n" },
            { "blockquote", "<blockquote>", "</blockquote>" },
            { "preformatted", "<pre>", "</pre>" },
            { "warning", "<div class=\"warning\">", "</div>" },
            { "caution", "<div class=\"caution\">", "</div>" },
            { "important", "<div class=\"important\">", "</div>" },
            { "note", "<div class=\"note\">", "</div>" },
            { "tip", "<div class=\"tip\">", "</div>" },
            { "list_item", "<li>\n", "\n</li>" },
            { "bold", "<strong>", "</strong>" }, // TODO: Or <b>? Should this be semantically meaningfull?
            { "italic", "<em>", "</em>" }, // TODO: Or <i>?
            { "underline", "<u>", "</u>" },
            { "teletype", "<code>", "</code>" },
            { "strikethrough", "<strike>", "</strike>" },
            { "quote", "<q>", "</q>" },
            { "url", "<a href=\"", "</a>" },
            { "link", "<a href=\"#", "</a>" },
            { "funcref", "", "" },
            { "classref", "", "" },
            { "memberref", "", "" },
            { "enumref", "", "" },
            { "macroref", "", "" },
            { "headerref", "", "" },
            { "conceptref", "", "" },
            { "globalref", "", "" },
            // Will need to deal with this explicitly
            { "footnote", "<div class=\"footnote\"><p>", "</p></div>" },
            { "escape", "<!--quickbook-escape-prefix-->", "<!--quickbook-escape-postfix-->" },
            { "replaceable", "<i>", "</i>" },
            // TODO: Is it possible to have an entry with a term, but no items.
            { "varlistentry", "", "" },
            { "varlistterm", "<dt>", "</dt>" },
            { "varlistitem", "<dd>", "</dd>" },
            { "header", "<thead>", "</thead>\n" },
            { "row", "<tr>", "</tr>\n" },
            { "cell", "<td>", "</td>" },
            { "programlisting", "<pre>", "</pre>\n" },
            { "code", "<code>", "</code>" },
            { "hr", "<hr/>", "" },
            { "break", "<br/>", "" },
        };
        
        std::map<std::string, html_markup> markup_map;
        
        struct initialize {
            initialize() {
                BOOST_FOREACH(html_markup m, markups) {
                    markup_map[m.quickbook] = m;
                }
            }
        } initialize_instance;

        html_markup get_markup(std::string const& x) {
            std::map<std::string, html_markup>::const_iterator
                pos = markup_map.find(x);
            BOOST_ASSERT(pos != markup_map.end());
            return pos->second;
        }
    }

    void html_encoder::operator()(quickbook::state& state, std::string const& x)
    {
        state.phrase << x;
    }

    void html_encoder::operator()(quickbook::state& state, char x)
    {
        state.phrase << encode(x);
    }

    void html_encoder::operator()(quickbook::state& state, unicode_char const& x)
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

    void html_encoder::operator()(quickbook::state& state, anchor const& x)
    {
        state.phrase << "<a id=\"" << encode(x.id) << "\"/>\n";
    }

    void html_encoder::operator()(quickbook::state& state, link const& x)
    {
        html_markup m = get_markup(x.type);
        if(*m.pre) {
            state.phrase << m.pre;
            state.phrase << encode(x.destination);
            state.phrase << "\">";
            state.phrase << x.content;
            state.phrase << m.post;
        }
        else {
            state.phrase << x.content;
        }
    }

    void html_encoder::operator()(quickbook::state& state, formatted const& x)
    {
        std::string type = x.type;
        if(type == "footnote") {
            int id = ++footnote_id;
            footnote_stack.top().push_back(footnote(id, x.content));
            // TODO: Maybe get section id from the state?
            state.phrase
                << "<sup id=\"footnote_ref_" << id << "\">"
                << "<a href=\"#footnote_" << id << "\">"
                << "[" << id << "]"
                << "</a>"
                << "</sup>"
                ;
        }
        else {
            html_markup m = get_markup(x.type);
            state.phrase << m.pre << x.content << m.post;
        }
    }

    void html_encoder::operator()(quickbook::state& state, block_formatted const& x)
    {
        std::string type = x.type;
        html_markup m = get_markup(x.type);
        state.block << m.pre << x.content << m.post;
    }

    void html_encoder::operator()(quickbook::state& state, break_ const& x)
    {
        html_markup m = get_markup("break");
        state.phrase << m.pre;
    }

    void html_encoder::operator()(quickbook::state& state, image2 const& x)
    {
        std::map<std::string, char const*> translate;
        translate["alt"] = "alt";
        translate["fileref"] = "src";
        translate["width"] = "width";
        translate["height"] = "height";
    
        state.phrase << "<img";

        for(image2::attribute_map::const_iterator
            attr_first = x.attributes.begin(), attr_last  = x.attributes.end();
            attr_first != attr_last; ++attr_first)
        {
            char const* html_attribute = translate[attr_first->first];
            if(!html_attribute) continue;

            state.phrase
                << " "
                << html_attribute
                << "=\""
                << encode(attr_first->second)
                << "\"";
        }

        state.phrase << "/>";
    }

    void html_encoder::operator()(quickbook::state& state, hr)
    {
        state.block << get_markup("hr").pre;
    }

    void html_encoder::operator()(quickbook::state& state, begin_section2 const& x)
    {
        // TODO: Should this be stored in the 'token', or at least have a nicer interface.
        int level = state.section_level + 1;
        if (level > 6) level = 6; 
    
        state.block << "\n<section id=\"" << encode(x.id) << "\">\n";
        if(x.linkend.empty()) {
            state.block
                << "<h" << level << ">"
                << x.content
                << "</h" << level << ">\n"
                ;
        }
        else {
            state.block
                << "<h" << level << " id=\""
                << encode(x.linkend)
                << "\">"
                << x.content
                << "</h" << level << ">\n"
                ;
        }

        push_footnotes(state);
    }

    void html_encoder::operator()(quickbook::state& state, end_section2 const& x)
    {
        pop_footnotes(state);

        state.block << "</section>";
    }

    void html_encoder::operator()(quickbook::state& state, heading2 const& x)
    {
        state.block
            << "<h" << x.level << " id=\"" << encode(x.id) << "\">"
            ;

        if(!x.linkend.empty()) {
            state.block
                << "<a id=\"" << encode(x.linkend) << "\"></a>"
                ;
        }
        state.block << x.content;

        state.block << "</h" << x.level << ">";
    }

    void html_encoder::operator()(quickbook::state& state, variablelist const& x)
    {
        // TODO: What should I do for the title?
        state.block << "<p>";
        state.block << encode(x.title);
        state.block << "</p>\n";

        state.block << "<dl>\n";

        html_markup m = get_markup("varlistentry");

        for(std::vector<varlistentry>::const_iterator
            it = x.entries.begin(); it != x.entries.end(); ++it)
        {
            state.block << m.pre;
            std::for_each(it->begin(), it->end(), encode_action(state, *this));
            state.block << m.post;
        }

        state.block << "</dl>\n";
    }

    void html_encoder::operator()(quickbook::state& state, table2 const& x)
    {
        if (x.title)
        {
            state.block << "<table";
            if(x.id)
                state.block << " id=\"" << encode(*x.id) << "\"";
            state.block << ">\n";
            state.block << "<caption>";
            state.block << encode(*x.title);
            state.block << "</caption>";
        }
        else
        {
            state.block << "<table";
            if(x.id)
                state.block << " id=\"" << encode(*x.id) << "\"";
            state.block << ">\n";
        }

        html_markup m = get_markup("row");

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

        state.block << "</tbody>\n";

        state.block << "</table>\n";
    }

    void html_encoder::operator()(quickbook::state& state, xinclude2 const& x)
    {
        // TODO: ?????
        //state.phrase << "\n<xi:include href=\"" << x.path << "\" />\n";
    }

    void html_encoder::operator()(quickbook::state& state, list2 const& x)
    {
        state.block << std::string(x.mark == '#' ? "<ol>\n" : "<ul>\n");

        for(std::vector<list_item2>::const_iterator
            it = x.items.begin(), end = x.items.end(); it != end; ++it)
        {
            state.block << "<li>\n" << it->content;
            if(!it->sublist.items.empty()) (*this)(state, it->sublist);
            state.block << std::string("\n</li>");
        }

        state.block << std::string(x.mark == '#' ? "\n</ol>" : "\n</ul>");
    }

    void html_encoder::operator()(quickbook::state& state, callout_link const& x)
    {
        state.phrase
            << "<span"
            << " class=\"" << x.role << "\""
            << " id=\"" << x.identifier << "co\""
            << ">"
            << "<a href=\"#" << x.identifier << "\">"
            // TODO: Get correct number
            // TODO: Better style
            << "(c)"
            << "</a>"
            << "</span>"
            ;
    }

    void html_encoder::operator()(quickbook::state& state, callout_list const& x)
    {
        state.block
            << "<dl class=\"calloutlist\">";
        unsigned int count = 0;

        BOOST_FOREACH(callout_item const& c, x)
        {
            state.block
                << "<dt id=\"" << c.identifier << "\">"
                << "<a href=\"#" << c.identifier << "co\">"
                << "callout " << ++count
                << "</a>"
                << "</dt>"
                << "<dd>"
                << c.content
                << "</dd>"
                ;
        }

        state.block
            << "</ol>";
    }

    void html_encoder::operator()(quickbook::state& state, code_token const& x)
    {
        std::string type = x.type;
        if(type == "space") {
            state.phrase << x.text;
        }
        else {
            state.phrase
                << "<span class=\"" << x.type << "\">"
                << encode(x.text)
                << "</span>";
        }
    }

    void html_encoder::operator()(quickbook::state& state, doc_info const& info)
    {
        // if we're ignoring the document info, we're done.
        if (info.ignore) return;

        state.block
            << "<!DOCTYPE html>"
            << "<html><head>"
            << "<title>" << encode(info.doc_title.get(106)) << "</title>"
            << "<meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\">"
            << "</head>"
            << "<body>"
            << "<header>"
            << "<h1>" << encode(info.doc_title.get(106)) << "</h1>"
            ;

        if(!info.doc_authors.empty() || !info.doc_copyrights.empty() ||
            !info.doc_license.empty())
        {
            state.block << "<dl>\n";

            if(!info.doc_authors.empty())
            {
                state.block
                    << "<dt>"
                    << (info.doc_authors.size() == 1 ? "Author:" :  "Authors:")
                    << "</dt>\n"
                    ;

                BOOST_FOREACH(doc_info::author const& author, info.doc_authors) {
                    state.block
                        << "<dd>"
                        << author.firstname.get(106)
                        << " "
                        << author.surname.get(106)
                        << "</dd>\n";
                }
            }
    
            if(!info.doc_copyrights.empty())
            {
                state.block
                    << "<dt>Copyright:</dt>\n"
                    ;
    
                BOOST_FOREACH(doc_info::copyright_entry const& copyright,
                    info.doc_copyrights)
                {
                    state.block << "<dd>&copy; ";
        
                    unsigned int range_state = 0;
                    unsigned int previous = 0;
                    BOOST_FOREACH(unsigned int year, copyright.years) {
                        switch(range_state) {
                        case 0: // Start
                            state.block << year;
                            range_state = 1;
                            break;
                        case 1: // Printed a year in last iteration
                            if(year == previous + 1) {
                                range_state = 2;
                            }
                            else {
                                state.block << ", " << year;
                                range_state = 1;
                            }
                            break;
                        case 2: // In the middle of a range
                            if(year != previous + 1) {
                                state.block << " - " << previous << ", " << year;
                                range_state = 1;
                            }
                            break;
                        }
                        previous = year;
                    }
                    if(range_state == 2) state.block << " - " << previous;
        
                    state.block
                        << " "
                        << copyright.holder.get(106)
                        << "</dd>\n"
                    ;
                }
            }
    
            if (!info.doc_license.empty())
            {
                state.block
                    << "<dt>License:</dt>\n"
                    << "<dd>"
                    << info.doc_license.get(103)
                    << "</dd>\n"
                ;
            }

            state.block << "</dl>\n";
        }

        state.block
            << "</header>\n"
            ;

        push_footnotes(state);
    }

    void html_encoder::operator()(quickbook::state& state, doc_info_post const& x)
    {
        // if we're ignoring the document info, do nothing.
        if (x.info.ignore) return;

        pop_footnotes(state);

        // We've finished generating our output. Here's what we'll do
        // *after* everything else.
        state.block << "</html>";
    }

    void html_encoder::push_footnotes(quickbook::state& state)
    {
        footnote_stack.push(footnotes());
    }

    void html_encoder::pop_footnotes(quickbook::state& state)
    {
        BOOST_ASSERT(!footnote_stack.empty());
        footnotes notes = footnote_stack.top();
        footnote_stack.pop();
        
        if(!notes.empty()) {
            state.block
                << "<dl class=\"footnotes\">\n";
            
            BOOST_FOREACH(footnote const& x, notes) {
                state.block
                    << "<dt id=\"footnote_" << encode(x.id) << "\">"
                    << "<a href=\"#footnote_ref_" << encode(x.id) << "\">"
                    << "Footnote"
                    << "</a>"
                    << "</dt>"
                    << "<dd>"
                    << x.content
                    << "</dd>"
                    ;
            }

            state.block
                << "</dl>\n";
        }
    }
}

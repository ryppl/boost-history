//#define BOOST_SPIRIT_DEBUG
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/info_parser.hpp>
#include <boost/spirit.hpp>
#include <iostream>
#include <fstream>
#include <vector>
#include <ctime>

///////////////////////////////////////////////////////////////////////
// Grammar
    
template<class Ptree>
struct context
{

    typedef typename Ptree::char_type Ch;
    typedef std::basic_string<Ch> Str;
    typedef typename std::vector<Ch>::iterator It;
    
    Str string;
    std::vector<Ptree *> stack;

    struct a_members
    {
        context &c;
        a_members(context &c): c(c) { }
        void operator()(Ch ch) const
        {
            std::cerr << "Members: " << ch << "\n";
        }
    };
    
    struct a_name
    {
        context &c;
        a_name(context &c): c(c) { }
        void operator()(It b, It e) const
        {
            Ptree *parent = c.stack.back();
            Ptree *child = &parent->push_back(std::make_pair(c.string, Ptree()))->second;
            c.stack.push_back(child);
            std::cerr << "Name: " << c.string << "\n";
            c.string.clear();
        }
    };

    struct a_item
    {
        context &c;
        a_item(context &c): c(c) { }
        void operator()(It b, It e) const
        {
            Ptree *parent = c.stack.back();
            Ptree *child = &parent->push_back(std::make_pair(Str(), Ptree()))->second;
            child->put_own(c.string);
            std::cerr << "Item: " << c.string << "\n";
            c.string.clear();
        }
    };

    struct a_value
    {
        context &c;
        a_value(context &c): c(c) { }
        void operator()(It b, It e) const
        {
            c.stack.back()->put_own(c.string);
            c.stack.pop_back();
            c.string.clear();
            std::cerr << "Value\n";
        }
    };

    struct a_keyword
    {
        context &c;
        a_keyword(context &c): c(c) { }
        void operator()(It b, It e) const
        {
            c.string.assign(b, e);
            std::cerr << "Keyword: " << Str(b, e) << "\n";
        }
    };

    struct a_char
    {
        context &c;
        a_char(context &c): c(c) { }
        void operator()(It b, It e) const
        {
            c.string += *b;
            std::cerr << "Char: " << *b << "\n";
        }
    };

    struct a_escape
    {
        context &c;
        a_escape(context &c): c(c) { }
        void operator()(Ch ch) const
        {
            switch (ch)
            {
                case Ch('\"'): c.string += '\"'; break;
                case Ch('\\'): c.string += '\\'; break;
                case Ch('0'): c.string += '\0'; break;
                case Ch('b'): c.string += '\b'; break;
                case Ch('f'): c.string += '\f'; break;
                case Ch('n'): c.string += '\n'; break;
                case Ch('r'): c.string += '\r'; break;
                case Ch('t'): c.string += '\t'; break;
                default: BOOST_ASSERT(0);
            }
            std::cerr << "Escape: " << ch << "\n";
        }
    };

    struct a_unicode
    {
        context &c;
        a_unicode(context &c): c(c) { }
        void operator()(unsigned long u) const
        {
            u = (std::min)(u, static_cast<unsigned long>((std::numeric_limits<Ch>::max)()));
            c.string += Ch(u);
            std::cerr << "Unicode: " << u << "\n";
        }
    };

};

template<class Ptree>
struct json_grammar: public boost::spirit::grammar<json_grammar<Ptree> >
{
    
    typedef context<Ptree> Context;
    mutable Context c;
    
    template<class Scanner>
    struct definition
    {
        
        boost::spirit::rule<Scanner> root, object, member, array, value, string, number;
        boost::spirit::rule<typename boost::spirit::lexeme_scanner<Scanner>::type> character, escape;

        /*
        boost::spirit::rule<Scanner> root, object, members, array, items, name, value, string, number;
        boost::spirit::rule<typename boost::spirit::lexeme_scanner<Scanner>::type> character, escape;
        */

        definition(const json_grammar &self)
        {
            
            using namespace boost::spirit;

            // Assertions
            /*
            assertion<std::string> expect_object("expected object");
            assertion<std::string> expect_objclose("expected ',' or '}'");
            assertion<std::string> expect_arrclose("expected ',' or ']'");
            assertion<std::string> expect_name("expected object name");
            assertion<std::string> expect_value("expected value");
            assertion<std::string> expect_escape("invalid escape sequence");
            */

            // JSON grammar rules
            root      = object >> end_p;
            object    = ch_p('{') >> !list_p(member, ch_p(',')) >> ch_p('}');
            member    = string[Context::a_name(self.c)] >> ':' >> value[Context::a_value(self.c)];
            array     = ch_p('[') >> !list_p(value[Context::a_item(self.c)], ch_p(',')) >> ch_p(']');
            value     = string | object | array 
                        | (number | str_p("true") | "false" | "null")[Context::a_keyword(self.c)];
            number    = strict_real_p | int_p;
            string    = lexeme_d[confix_p('\"', *character, '\"')];
            character = (anychar_p - "\\" - "\"")[Context::a_char(self.c)] | ch_p("\\") >> escape;
            escape    = chset_p("\"\\0bfnrt")[Context::a_escape(self.c)] 
                        | 'u' >> uint_parser<unsigned long, 16, 4, 4>()[Context::a_unicode(self.c)];
            
            /*
            root      = expect_object(object) >> end_p;
            
            object    = ch_p('{') >> '}'
                        | '{' >> members >> expect_objclose(ch_p('}'));
            
            members   = name >> expect_value(':' >> value) >> 
                        *(',' >> name >> expect_value(':' >> value));
            
            array     = ch_p('[') >> ']'
                        | '[' >> items >> expect_arrclose(ch_p(']'));
            
            items     = expect_value(value) >> *(',' >> expect_value(value));
            
            name      = expect_name(string);
            
            value     = string | number | object | array 
                        | "true" | "false" | "null";
            
            string    = lexeme_d[
                          ch_p("\"") >> "\""
                          | ch_p("\"") >> +character >> "\""
                        ];
            
            number    = strict_real_p | int_p;
            
            escape    = ch_p('"') | '\\' | 'b' | 'f' | 'n' | 'r' | 't' 
                        | 'u' >> repeat_p(4)[digit_p];
            
            character = (anychar_p - "\"" - "\\")
                        | ch_p("\\") >> expect_escape(escape);
            */

            // Debug
            BOOST_SPIRIT_DEBUG_RULE(root);
            BOOST_SPIRIT_DEBUG_RULE(object);
            BOOST_SPIRIT_DEBUG_RULE(members);
            BOOST_SPIRIT_DEBUG_RULE(array);
            BOOST_SPIRIT_DEBUG_RULE(items);
            BOOST_SPIRIT_DEBUG_RULE(name);
            BOOST_SPIRIT_DEBUG_RULE(value);
            BOOST_SPIRIT_DEBUG_RULE(string);
            BOOST_SPIRIT_DEBUG_RULE(number);
            BOOST_SPIRIT_DEBUG_RULE(escape);
            BOOST_SPIRIT_DEBUG_RULE(character);

        }

        const boost::spirit::rule<Scanner> &start() const
        {
            return root;
        }

    };

};

template<class It>
unsigned long count_lines(It begin, It end)
{
    return unsigned long(std::count(begin, end, '\n') + 1);
}

int main(int argc, char **argv)
{

    using namespace boost::spirit;
    typedef char Ch;
    typedef std::vector<Ch>::iterator It;

    if (argc != 2)
        abort();

    std::basic_ifstream<Ch> stream(argv[1]);
    std::vector<Ch> v(std::istreambuf_iterator<Ch>(stream.rdbuf()),
                      std::istreambuf_iterator<Ch>());

    std::clock_t t1 = std::clock();
    for (int i = 0; i < 1; ++i)
    {
        boost::property_tree::ptree pt;
        json_grammar<boost::property_tree::ptree> g;
        g.c.stack.push_back(&pt);
        try
        {
            parse_info<It> pi = parse(v.begin(), v.end(), g, 
                                      space_p | comment_p("//") | comment_p("/*", "*/"));
            if (!pi.hit || !pi.full)
                throw parser_error<std::string, It>(v.begin(), "syntax error");
        }
        catch (parser_error<std::string, It> &e)
        {
            std::cerr << "Error at line " << count_lines(v.begin(), e.where) << ": " << e.descriptor << "\n";
        }
        std::cerr << "=============\n";
        write_info(std::cerr, pt);
    }
    std::clock_t t2 = std::clock();

    std::cerr << double(t2 - t1) / CLOCKS_PER_SEC << "\n";

}

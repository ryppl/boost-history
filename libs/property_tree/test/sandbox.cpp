#include <boost/optional.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/info_parser.hpp>

#include <boost/spirit.hpp>

void f(const char *s)
{

    using namespace boost::spirit;

    // Whitespace
    rule<> ws = +(ch_p(' ') | '\t' | '\n');
    rule<> ws_no_eol = +(ch_p(' ') | '\t');
    rule<> eol = !ws_no_eol >> '\n' >> !ws_no_eol;
    
    // Utils
    rule<> open_brace = !ws >> '{' >> !ws;
    rule<> close_brace = !ws >> '}' >> !ws;
    rule<> open_quote = !ws_no_eol >> '\"';
    rule<> close_quote = '\"' >> !ws_no_eol;
    rule<> continue_marker = '\\' >> eol;
    rule<> escape_seq = '\\' >> chset_p("abfnrtv\"\'\\");
    
    // String
    rule<> pure_string = +(anychar_p - ' ' - '\t' - '"' - '\\');
    rule<> string = !ws_no_eol >> +(pure_string | escape_seq) >> !ws_no_eol;
    rule<> quoted_string = open_quote >> +(pure_string | escape_seq) >> close_quote;
    
    // Key
    rule<> empty_key = open_quote >> close_quote;
    rule<> simple_key = string;
    rule<> quoted_key = quoted_string;
    rule<> key = empty_key | simple_key | quoted_key;
    
    // Value
    rule<> empty_value = (eps_p | (open_quote >> close_quote)) >> +eol;
    rule<> simple_value = string >> +eol;
    rule<> quoted_value = quoted_string;
    rule<> multiline_value = +(quoted_string >> continue_marker) >> quoted_string;
    rule<> value = empty_value | simple_value | quoted_value | multiline_value;

    // Main
    rule<> key_and_value = key >> value >> !ws;
    rule<> entry = key_and_value >> !(open_brace >> *entry >> close_brace);
    rule<> info = *entry;

    parse_info<const char *> pi = parse(s, info);
    
}

int main()
{
    f("\"key\n");
}

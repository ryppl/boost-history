// ----------------------------------------------------------------------------
// Copyright (C) 2002-2005 Marcin Kalicinski
//
// Distributed under the Boost Software License, Version 1.0. 
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)
//
// For more information, see www.boost.org
// ----------------------------------------------------------------------------

/* This is grammar of INFO file format written in form of boost::spirit rules.
   For simplicity, this grammar does not support comments and #include directive.
   Anyway, both of these should be probably handled by some sort of preprocessor - 
   including them in main grammar would complicate it beyond grokking.
   
   The reason why this grammar looks rather complicated is because it strives to parse
   the whitespace correctly - again this should probably be delegated to a preprocessor.   
   Another quirk is the file must end with a newline character for this grammar to work 
   correctly - this is not required by property_tree library INFO parser.

   Note that INFO parser included in property_tree library does not use Spirit.
*/

//#define BOOST_SPIRIT_DEBUG
#include <boost/spirit.hpp>

void parse(const char *s)
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
    rule<> pure_string = +(anychar_p - ' ' - '\t' - '"' - '\\' - '\n');
    rule<> string = !ws_no_eol >> +(pure_string | escape_seq) >> !ws_no_eol;
    rule<> quoted_string = open_quote >> +(pure_string | ' ' | '\t' | escape_seq) >> close_quote;
    
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
    rule<> info = *(entry | ws);

    // Debug nodes
    BOOST_SPIRIT_DEBUG_NODE(ws);
    BOOST_SPIRIT_DEBUG_NODE(ws_no_eol);
    BOOST_SPIRIT_DEBUG_NODE(eol);
    BOOST_SPIRIT_DEBUG_NODE(open_brace);
    BOOST_SPIRIT_DEBUG_NODE(close_brace);
    BOOST_SPIRIT_DEBUG_NODE(open_quote);
    BOOST_SPIRIT_DEBUG_NODE(close_quote);
    BOOST_SPIRIT_DEBUG_NODE(continue_marker);
    BOOST_SPIRIT_DEBUG_NODE(escape_seq);
    BOOST_SPIRIT_DEBUG_NODE(pure_string);
    BOOST_SPIRIT_DEBUG_NODE(string);
    BOOST_SPIRIT_DEBUG_NODE(quoted_string);
    BOOST_SPIRIT_DEBUG_NODE(empty_key);
    BOOST_SPIRIT_DEBUG_NODE(simple_key);
    BOOST_SPIRIT_DEBUG_NODE(quoted_key);
    BOOST_SPIRIT_DEBUG_NODE(key);
    BOOST_SPIRIT_DEBUG_NODE(empty_value);
    BOOST_SPIRIT_DEBUG_NODE(simple_value);
    BOOST_SPIRIT_DEBUG_NODE(quoted_value);
    BOOST_SPIRIT_DEBUG_NODE(multiline_value);
    BOOST_SPIRIT_DEBUG_NODE(value);
    BOOST_SPIRIT_DEBUG_NODE(key_and_value);
    BOOST_SPIRIT_DEBUG_NODE(entry);
    BOOST_SPIRIT_DEBUG_NODE(info);
    
    // Parse and display result
    parse_info<const char *> pi = parse(s, info);
    std::cout << "Parse result: " << (pi.full ? "Success" : "Failure") << "\n";
    
}

int main()
{

    // Sample data 1
    const char *data1 = 
        "\n"
        "key1 data1\n"
        "{\n"
        "\tkey data\n"
        "}\n"
        "key2 \"data2  \" {\n"
        "\tkey data\n"
        "}\n"
        "key3 \"data\"\n"
        "\t \"3\" {\n"
        "\tkey data\n"
        "}\n"
        "\n"
        "\"key4\" data4\n"
        "{\n"
        "\tkey data\n"
        "}\n"
        "\"key.5\" \"data.5\" { \n"
        "\tkey data \n"
        "}\n"
        "\"key6\" \"data\"\n"
        "\t   \"6\" {\n"
        "\tkey data\n"
        "}\n"
        "   \n"
        "key1 data1\n"
        "{\n"
        "\tkey data\n"
        "}\n"
        "key2 \"data2  \" {\n"
        "\tkey data\n"
        "}\n"
        "key3 \"data\"\n"
        "\t \"3\" {\n"
        "\tkey data\n"
        "}\n"
        "\n"
        "\"key4\" data4\n"
        "{\n"
        "\tkey data\n"
        "}\n"
        "\"key.5\" \"data.5\" {\n"
        "\tkey data\n"
        "}\n"
        "\"key6\" \"data\"\n"
        "\t   \"6\" {\n"
        "\tkey data\n"
        "}\n"
        "\\\\key\\t7 data7\\n\\\"data7\\\"\n"
        "{\n"
        "\tkey data\n"
        "}\n"
        "\"\\\\key\\t8\" \"data8\\n\\\"data8\\\"\"\n"
        "{\n"
        "\tkey data\n"
        "}\n"
        "\n";

    // Sample data 2
    const char *data2 = 
        "key1\n"
        "key2\n"
        "key3\n"
        "key4\n";

    // Parse sample data
    parse(data1);
    parse(data2);

}

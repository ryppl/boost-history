/*
 *
 * Copyright (c) 2004
 * Dr John Maddock
 *
 * Use, modification and distribution are subject to the 
 * Boost Software License, Version 1.0. (See accompanying file 
 * LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 */

#include "test.hpp"

void test_replace()
{
   using namespace boost::regex_constants;
   // start by testing subs:
   TEST_REGEX_REPLACE("a+", perl, "...aaa,,,", match_default|format_no_copy, "$`", "...");
   TEST_REGEX_REPLACE("a+", perl, "...aaa,,,", match_default|format_no_copy, "$'", ",,,");
   TEST_REGEX_REPLACE("a+", perl, "...aaa,,,", match_default|format_no_copy, "$&", "aaa");
   TEST_REGEX_REPLACE("a+", perl, "...aaa,,,", match_default|format_no_copy, "$0", "aaa");
   TEST_REGEX_REPLACE("a+", perl, "...aaa,,,", match_default|format_no_copy, "$1", "");
   TEST_REGEX_REPLACE("a+", perl, "...aaa,,,", match_default|format_no_copy, "$15", "");
   TEST_REGEX_REPLACE("(a+)b+", perl, "...aaabbb,,,", match_default|format_no_copy, "$1", "aaa");
   TEST_REGEX_REPLACE("[[:digit:]]*", perl, "123ab", match_default|format_no_copy, "<$0>", "<123><><><>");
   TEST_REGEX_REPLACE("[[:digit:]]*", perl, "123ab1", match_default|format_no_copy, "<$0>", "<123><><><1><>");
   TEST_REGEX_REPLACE("a+", perl, "...aaa,,,", match_default|format_no_copy, "$`$", "...$");
   // and now escapes:
   TEST_REGEX_REPLACE("a+", perl, "...aaa,,,", match_default|format_no_copy, "$x", "$x");
   TEST_REGEX_REPLACE("a+", perl, "...aaa,,,", match_default|format_no_copy, "\\a", "\a");
   TEST_REGEX_REPLACE("a+", perl, "...aaa,,,", match_default|format_no_copy, "\\f", "\f");
   TEST_REGEX_REPLACE("a+", perl, "...aaa,,,", match_default|format_no_copy, "\\n", "\n");
   TEST_REGEX_REPLACE("a+", perl, "...aaa,,,", match_default|format_no_copy, "\\r", "\r");
   TEST_REGEX_REPLACE("a+", perl, "...aaa,,,", match_default|format_no_copy, "\\t", "\t");
   TEST_REGEX_REPLACE("a+", perl, "...aaa,,,", match_default|format_no_copy, "\\v", "\v");
   TEST_REGEX_REPLACE("a+", perl, "...aaa,,,", match_default|format_no_copy, "\\", "\\");
   TEST_REGEX_REPLACE("a+", perl, "...aaa,,,", match_default|format_no_copy, "\\x21", "!");
   TEST_REGEX_REPLACE("a+", perl, "...aaa,,,", match_default|format_no_copy, "\\xz", "xz");
   TEST_REGEX_REPLACE("a+", perl, "...aaa,,,", match_default|format_no_copy, "\\x", "x");
   TEST_REGEX_REPLACE("a+", perl, "...aaa,,,", match_default|format_no_copy, "\\x{z}", "x{z}");
   TEST_REGEX_REPLACE("a+", perl, "...aaa,,,", match_default|format_no_copy, "\\x{12b", "x{12b");
   TEST_REGEX_REPLACE("a+", perl, "...aaa,,,", match_default|format_no_copy, "\\x{21}", "!");
   TEST_REGEX_REPLACE("a+", perl, "...aaa,,,", match_default|format_no_copy, "\\c@", "\0");
   TEST_REGEX_REPLACE("a+", perl, "...aaa,,,", match_default|format_no_copy, "\\c", "c");
   TEST_REGEX_REPLACE("a+", perl, "...aaa,,,", match_default|format_no_copy, "\\e", "\x1B");
   TEST_REGEX_REPLACE("a+", perl, "...aaa,,,", match_default|format_no_copy, "\\0101", "A");
   TEST_REGEX_REPLACE("(a+)", perl, "...aaa,,,", match_default|format_no_copy, "\\u$1", "Aaa");
   TEST_REGEX_REPLACE("(a+)", perl, "...aaa,,,", match_default|format_no_copy, "\\U$1", "AAA");
   TEST_REGEX_REPLACE("(a+)", perl, "...aaa,,,", match_default|format_no_copy, "\\U$1\\E$1", "AAAaaa");
   TEST_REGEX_REPLACE("(A+)", perl, "...AAA,,,", match_default|format_no_copy, "\\l$1", "aAA");
   TEST_REGEX_REPLACE("(A+)", perl, "...AAA,,,", match_default|format_no_copy, "\\L$1", "aaa");
   TEST_REGEX_REPLACE("(A+)", perl, "...AAA,,,", match_default|format_no_copy, "\\L$1\\E$1", "aaaAAA");
   // sed format sequences:
   TEST_REGEX_REPLACE("(a+)(b+)", perl, " ...aabb,,", match_default|format_sed|format_no_copy, "\\0", "aabb");
   TEST_REGEX_REPLACE("(a+)(b+)", perl, " ...aabb,,", match_default|format_sed|format_no_copy, "\\1", "aa");
   TEST_REGEX_REPLACE("(a+)(b+)", perl, " ...aabb,,", match_default|format_sed|format_no_copy, "\\2", "bb");
   TEST_REGEX_REPLACE("(a+)(b+)", perl, " ...aabb,,", match_default|format_sed|format_no_copy, "&", "aabb");
   TEST_REGEX_REPLACE("(a+)(b+)", perl, " ...aabb,,", match_default|format_sed|format_no_copy, "$", "$");
   TEST_REGEX_REPLACE("(a+)(b+)", perl, " ...aabb,,", match_default|format_sed|format_no_copy, "$1", "$1");
   TEST_REGEX_REPLACE("(a+)(b+)", perl, " ...aabb,,", match_default|format_sed|format_no_copy, "()?:", "()?:");
   TEST_REGEX_REPLACE("(a+)(b+)", perl, " ...aabb,,", match_default|format_sed|format_no_copy, "\\\\", "\\");
   TEST_REGEX_REPLACE("(a+)(b+)", perl, " ...aabb,,", match_default|format_sed|format_no_copy, "\\&", "&");

   TEST_REGEX_REPLACE("(a+)(b+)", perl, " ...aabb,,", match_default|format_perl|format_no_copy, "$0", "aabb");
   TEST_REGEX_REPLACE("(a+)(b+)", perl, " ...aabb,,", match_default|format_perl|format_no_copy, "$1", "aa");
   TEST_REGEX_REPLACE("(a+)(b+)", perl, " ...aabb,,", match_default|format_perl|format_no_copy, "$2", "bb");
   TEST_REGEX_REPLACE("(a+)(b+)", perl, " ...aabb,,", match_default|format_perl|format_no_copy, "$&", "aabb");
   TEST_REGEX_REPLACE("(a+)(b+)", perl, " ...aabb,,", match_default|format_perl|format_no_copy, "$$", "$");
   TEST_REGEX_REPLACE("(a+)(b+)", perl, " ...aabb,,", match_default|format_perl|format_no_copy, "&", "&");
   TEST_REGEX_REPLACE("(a+)(b+)", perl, " ...aabb,,", match_default|format_perl|format_no_copy, "\\0", "\0");
   TEST_REGEX_REPLACE("(a+)(b+)", perl, " ...aabb,,", match_default|format_perl|format_no_copy, "()?:", "()?:");
   TEST_REGEX_REPLACE("a+", perl, "...aaa,,", match_default|format_perl|format_no_copy, "\\0101", "A");

   // move to copying unmatched data:
   TEST_REGEX_REPLACE("a+", perl, "...aaa,,,", match_default|format_all, "bbb", "...bbb,,,");
   TEST_REGEX_REPLACE("a+(b+)", perl, "...aaabb,,,", match_default|format_all, "$1", "...bb,,,");
   TEST_REGEX_REPLACE("a+(b+)", perl, "...aaabb,,,ab*abbb?", match_default|format_all, "$1", "...bb,,,b*bbb?");
   TEST_REGEX_REPLACE("(a+)|(b+)", perl, "...aaabb,,,ab*abbb?", match_default|format_all, "(?1A)(?2B)", "...AB,,,AB*AB?");
   TEST_REGEX_REPLACE("(a+)|(b+)", perl, "...aaabb,,,ab*abbb?", match_default|format_all, "?1A:B", "...AB,,,AB*AB?");
   TEST_REGEX_REPLACE("(a+)|(b+)", perl, "...aaabb,,,ab*abbb?", match_default|format_all, "(?1A:B)C", "...ACBC,,,ACBC*ACBC?");
   TEST_REGEX_REPLACE("(a+)|(b+)", perl, "...aaabb,,,ab*abbb?", match_default|format_all, "?1:B", "...B,,,B*B?");
   // move to copying unmatched data, but replace first occurance only:
   TEST_REGEX_REPLACE("a+", perl, "...aaa,,,", match_default|format_all|format_first_only, "bbb", "...bbb,,,");
   TEST_REGEX_REPLACE("a+(b+)", perl, "...aaabb,,,", match_default|format_all|format_first_only, "$1", "...bb,,,");
   TEST_REGEX_REPLACE("a+(b+)", perl, "...aaabb,,,ab*abbb?", match_default|format_all|format_first_only, "$1", "...bb,,,ab*abbb?");
   TEST_REGEX_REPLACE("(a+)|(b+)", perl, "...aaabb,,,ab*abbb?", match_default|format_all|format_first_only, "(?1A)(?2B)", "...Abb,,,ab*abbb?");
   TEST_REGEX_REPLACE("(a+)|(b+)", perl, "...aaabb,,,ab*abbb?", match_default|format_all, "?1A", "...A,,,A*A?");
   TEST_REGEX_REPLACE("(a+)|(b+)", perl, "...aaabb,,,ab*abbb?", match_default|format_all, "?1:B", "...B,,,B*B?");
   TEST_REGEX_REPLACE("(a+)|(b+)", perl, "...aaabb,,,ab*abbb?", match_default|format_all, "(?1A:(?2B))", "...AB,,,AB*AB?");
   TEST_REGEX_REPLACE("X", literal, "XX", match_default, "Y", "YY");
   TEST_REGEX_REPLACE("(a+)|(b+)", perl, "...aaabb,,,ab*abbb?", match_default|format_all, "(?", "...??,,,??*???");
   TEST_REGEX_REPLACE("(a+)|(b+)", perl, "...aaabb,,,ab*abbb?", match_default|format_all, "(?a", "...?a?a,,,?a?a*?a?a?");
   TEST_REGEX_REPLACE("(a+)|(b+)", perl, "...aaabb,,,ab*abbb?", match_default|format_all, "(?1A:(?2B))abc", "...AabcBabc,,,AabcBabc*AabcBabc?");
   TEST_REGEX_REPLACE("a+(b+)", perl, "...aaabb,,,", match_default|format_all|format_no_copy, "(?2abc:def)", "def");
   TEST_REGEX_REPLACE("a+(b+)", perl, "...aaabb,,,", match_default|format_all|format_no_copy, "(?1abc:def)", "abc");
   TEST_REGEX_REPLACE("a+(b+)", perl, "...aaabb,,,", match_default|format_perl|format_no_copy, "(?1abc:def)", "(?1abc:def)");
   TEST_REGEX_REPLACE("a+(b+)", perl, "...", match_default|format_perl, "(?1abc:def)", "...");
   TEST_REGEX_REPLACE("a+(b+)", perl, "...", match_default|format_perl|format_no_copy, "(?1abc:def)", "");
}




#ifndef BOOST_REGEX_REGRESS_TEST_HPP
#define BOOST_REGEX_REGRESS_TEST_HPP
#include "test_not_regex.hpp"
#include "test_regex_search.hpp"
#include "test_regex_replace.hpp"
#include "test_deprecated.hpp"


//
// define test entry proc, this forwards on to the appropriate
// real test:
//
template <class charT, class tagT>
void test(const charT& c, const tagT& tag)
{
#ifndef BOOST_NO_STD_LOCALE
   boost::basic_regex<charT, boost::cpp_regex_traits<charT> > e1;
   test(e1, tag);
#endif
#if !BOOST_WORKAROUND(__BORLANDC__, < 0x560)
   boost::basic_regex<charT, boost::c_regex_traits<charT> > e2;
   test(e2, tag);
#endif
#if defined(_WIN32) && !defined(BOOST_REGEX_NO_W32)
   boost::basic_regex<charT, boost::w32_regex_traits<charT> > e3;
   test(e3, tag);
#endif
   // test old depecated code:
   test_deprecated(c, tag);
}

//
// define function to pack args into an array:
//
const int* make_array(int first, ...);


//
// define macros for testing invalid regexes:
//
#define TEST_INVALID_REGEX_N(s, f)\
   do{\
      const char e[] = { s };\
      std::string se(e, sizeof(e) - 1);\
      test_info<char>::set_info(__FILE__, __LINE__, se, f);\
      test(char(0), test_invalid_regex_tag());\
   }while(0)

#ifndef BOOST_NO_WREGEX
#define TEST_INVALID_REGEX_W(s, f)\
   do{\
      const wchar_t e[] = { s };\
      std::wstring se(e, (sizeof(e) / sizeof(wchar_t)) - 1);\
      test_info<wchar_t>::set_info(__FILE__, __LINE__, se, f);\
      test(wchar_t(0), test_invalid_regex_tag());\
   }while(0)
#else
#define TEST_INVALID_REGEX_W(s, f)
#endif

#define TEST_INVALID_REGEX(s, f)\
   TEST_INVALID_REGEX_N(s, f);\
   TEST_INVALID_REGEX_W(BOOST_JOIN(L, s), f)

//
// define macros for testing regex searches:
//
#define TEST_REGEX_SEARCH_N(s, f, t, m, a)\
   do{\
      const char e[] = { s };\
      std::string se(e, sizeof(e) - 1);\
      const char st[] = { t };\
      std::string sst(st, sizeof(st) - 1);\
      test_info<char>::set_info(__FILE__, __LINE__, se, f, sst, m, a);\
      test(char(0), test_regex_search_tag());\
   }while(0)

#ifndef BOOST_NO_WREGEX
#define TEST_REGEX_SEARCH_W(s, f, t, m, a)\
   do{\
      const wchar_t e[] = { s };\
      std::wstring se(e, (sizeof(e) / sizeof(wchar_t)) - 1);\
      const wchar_t st[] = { t };\
      std::wstring sst(st, (sizeof(st) / sizeof(wchar_t)) - 1);\
      test_info<wchar_t>::set_info(__FILE__, __LINE__, se, f, sst, m, a);\
      test(wchar_t(0), test_regex_search_tag());\
   }while(0)
#else
#define TEST_REGEX_SEARCH_W(s, f, t, m, a)
#endif

#define TEST_REGEX_SEARCH(s, f, t, m, a)\
   TEST_REGEX_SEARCH_N(s, f, t, m, a);\
   TEST_REGEX_SEARCH_W(BOOST_JOIN(L, s), f, BOOST_JOIN(L, t), m, a)

//
// define macros for testing regex replaces:
//
#define TEST_REGEX_REPLACE_N(s, f, t, m, fs, r)\
   do{\
      const char e[] = { s };\
      std::string se(e, sizeof(e) - 1);\
      const char st[] = { t };\
      std::string sst(st, sizeof(st) - 1);\
      const char ft[] = { fs };\
      std::string sft(ft, sizeof(ft) - 1);\
      const char rt[] = { r };\
      std::string srt(rt, sizeof(rt) - 1);\
      test_info<char>::set_info(__FILE__, __LINE__, se, f, sst, m, 0, sft, srt);\
      test(char(0), test_regex_replace_tag());\
   }while(0)

#ifndef BOOST_NO_WREGEX
#define TEST_REGEX_REPLACE_W(s, f, t, m, fs, r)\
   do{\
      const wchar_t e[] = { s };\
      std::wstring se(e, (sizeof(e) / sizeof(wchar_t)) - 1);\
      const wchar_t st[] = { t };\
      std::wstring sst(st, (sizeof(st) / sizeof(wchar_t)) - 1);\
      const wchar_t ft[] = { fs };\
      std::wstring sft(ft, (sizeof(ft) / sizeof(wchar_t)) - 1);\
      const wchar_t rt[] = { r };\
      std::wstring srt(rt, (sizeof(rt) / sizeof(wchar_t)) - 1);\
      test_info<wchar_t>::set_info(__FILE__, __LINE__, se, f, sst, m, 0, sft, srt);\
      test(wchar_t(0), test_regex_replace_tag());\
   }while(0)
#else
#define TEST_REGEX_REPLACE_W(s, f, t, m, fs, r)
#endif

#define TEST_REGEX_REPLACE(s, f, t, m, fs, r)\
   TEST_REGEX_REPLACE_N(s, f, t, m, fs, r);\
   TEST_REGEX_REPLACE_W(BOOST_JOIN(L, s), f, BOOST_JOIN(L, t), m, BOOST_JOIN(L, fs), BOOST_JOIN(L, r))

//
// define the test group proceedures:
//
void basic_tests();
void test_simple_repeats();
void test_alt();
void test_sets();
void test_sets2();
void test_anchors();
void test_backrefs();
void test_character_escapes();
void test_assertion_escapes();
void test_tricky_cases();
void test_grep();
void test_replace();
void test_non_greedy_repeats();
void test_non_marking_paren();
void test_partial_match();
void test_forward_lookahead_asserts();
void test_fast_repeats();
void test_tricky_cases2();
void test_independent_subs();
void test_nosubs();
void test_conditionals();
void test_options();
void test_options2();

#endif

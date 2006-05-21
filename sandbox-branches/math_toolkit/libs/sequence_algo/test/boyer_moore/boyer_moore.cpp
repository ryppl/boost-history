// (C) Copyright Craig Henderson 2002, 2003
//               cdm.henderson@virgin.net
//
// Permission to use, copy, modify, distribute and sell this software
// and its documentation for any purpose is hereby granted without fee,
// provided that the above copyright notice appears in all copies and
// that both that copyright notice and this permission notice appear
// in supporting documentation.  The author makes no representations
// about the suitability of this software for any purpose.  It is
// provided "as is" without express or implied warranty.

#include <string>
#include "boost/sequence_algo/boyer_moore.hpp"
#include <assert.h>
#include <iostream>


int main(int, char *[])
{
    std::string text("this is an example foo bar");
    std::string pattern("example");

    std::cout << "\nTesting four iterator searching...";
    std::string::const_iterator it  = text.begin();
    std::string::const_iterator ite = text.end();
    it = boost::boyer_moore_search<>(
             it, ite,
             pattern.begin(),
             pattern.end());
    assert(it != text.end());

    std::cout << "\nTesting two container searching...";
    it = boost::boyer_moore_search(text, pattern);     assert(it != text.end());
    it = boost::boyer_moore_search<std::string, std::string>(text, "this");      assert(it != text.end());
    it = boost::boyer_moore_search<std::string, std::string>(text, "is an ex");  assert(it != text.end());
    it = boost::boyer_moore_search<std::string, std::string>(text, "find this"); assert(it == text.end());
    it = boost::boyer_moore_search<std::string, std::string>(text, "fo");        assert(it != text.end());

    std::string is("is");
    it  = text.begin();
    ite = text.end();
    it = boost::boyer_moore_search<>(
             it, ite,
             is.begin(),
             is.end());


    text = "All the world's a stage,"
           "And all the men and women merely players,"
           "They each have their exits and their entrances,"
           "And in his time, one man plays many parts.";
    it = boost::boyer_moore_search<std::string, std::string>(text, "plays many"); assert(it != text.end());
    it = boost::boyer_moore_search<std::string, std::string>(text, "Plays many"); assert(it == text.end());
    assert(text.find("plays") != std::string::npos);


#if defined(BOOST_MSVC)
{
    std::wstring wtext(L"this is an example foo bar");
    std::wstring wpattern(L"example");

    std::wstring::const_iterator wit  = wtext.begin();
    std::wstring::const_iterator wite = wtext.end();
    wit = boost::boyer_moore_search<>(
              wit, wite,
              wpattern.begin(),
              wpattern.end());

    std::cout << "\nTesting wide string searching...";
    wit = boost::boyer_moore_search(wtext, wpattern);      assert(wit != wtext.end());
    wit = boost::boyer_moore_search<std::wstring, std::wstring>(wtext, L"this");       assert(wit != wtext.end());
    wit = boost::boyer_moore_search<std::wstring, std::wstring>(wtext, L"is an ex");   assert(wit != wtext.end());
    wit = boost::boyer_moore_search<std::wstring, std::wstring>(wtext, L"find this");  assert(wit == wtext.end());
    wit = boost::boyer_moore_search<std::wstring, std::wstring>(wtext, L"fo");         assert(wit != wtext.end());
    wit = boost::boyer_moore_search<std::wstring, std::wstring>(wtext, L"is");         assert(wit != wtext.end());
}
#endif

    std::cout << "\nTesting multiple sequence searching...";
    pattern= "time";
    boost::boyer_moore_pattern_search<
        std::string::const_iterator,
        std::string::const_iterator>
    srch(pattern.begin(), pattern.end());

    it = srch.search(text.begin(), text.end());
    assert(it != text.end());

    text = "time waits for no man";
    it = srch.search(text.begin(), text.end());
    assert(it != text.end());

    text = "tide and time wait for no man";
    it = srch.search(text.begin(), text.end());
    assert(it != text.end());

    text = pattern;
    it = srch.search(text.begin(), text.end());
    assert(it != text.end());

    std::cout << "\n\nBoyer Moore Search tests completed without errors.\n";

    return 0;
}

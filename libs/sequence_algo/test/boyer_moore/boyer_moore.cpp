// (C) Copyright Craig Henderson 2002
//               cdm.henderson@virgin.net
//
// Permission to use, copy, modify, distribute and sell this software
// and its documentation for any purpose is hereby granted without fee,
// provided that the above copyright notice appears in all copies and
// that both that copyright notice and this permission notice appear
// in supporting documentation.  The author makes no representations
// about the suitability of this software for any purpose.  It is
// provided "as is" without express or implied warranty.

#include "boost/sequence_algo/boyer_moore.hpp"
#include <assert.h>
#include <iostream>
#include <vector>


int main(int, char *[])
{
    std::string text("this is an example foo bar");
    std::string pattern("example");

    std::string::const_iterator it;

    std::vector<std::string::const_iterator> vec;
    boost::boyer_moore_search<std::string::const_iterator,
                              std::string::const_iterator>(text.begin(),
                                                           text.end(),
                                                           pattern.begin(),
                                                           pattern.end(),
                                                           std::back_inserter<>(vec), 0);
    assert(vec.size() > 0);
    it = boost::boyer_moore(text, pattern);                            assert(it != text.end());
    it = boost::boyer_moore(text, "this");                             assert(it != text.end());
    it = boost::boyer_moore(text, "is an ex");                         assert(it != text.end());
    it = boost::boyer_moore(text, "find this");                        assert(it == text.end());
    it = boost::boyer_moore(text, "fo");                               assert(it != text.end());

    std::string is("is");
    vec.clear();
    boost::boyer_moore_search<std::string::const_iterator,
                              std::string::const_iterator>(text.begin(),
                                                           text.end(),
                                                           is.begin(),
                                                           is.end(),
                                                           std::back_inserter<>(vec), 0);
    assert(vec.size() == 2);


    text = "All the world's a stage,"
           "And all the men and women merely players,"
           "They each have their exits and their entrances,"
           "And in his time, one man plays many parts.";
    it = boost::boyer_moore(text, "plays many");        assert(it != text.end());
    it = boost::boyer_moore(text, "Plays many");        assert(it == text.end());
    assert(text.find("plays") != std::string::npos);


#if defined(BOOST_MSVC)
{
    std::wstring wtext(L"this is an example foo bar");
    std::wstring wpattern(L"example");

    std::wstring::const_iterator wit;
    std::vector<std::ptrdiff_t> vec;
    boost::boyer_moore_search<std::wstring::const_iterator,
                              std::wstring::const_iterator>(wtext.begin(),
                                                            wtext.end(),
                                                            wpattern.begin(),
                                                            wpattern.end(),
                                                            std::back_inserter<>(vec), 0);
    assert(vec.size() > 0);

    wit = boost::boyer_moore(wtext, wpattern);      assert(wit != wtext.end());
    wit = boost::boyer_moore(wtext, L"this");       assert(wit != wtext.end());
    wit = boost::boyer_moore(wtext, L"is an ex");   assert(wit != wtext.end());
    wit = boost::boyer_moore(wtext, L"find this");  assert(wit == wtext.end());
    wit = boost::boyer_moore(wtext, L"fo");         assert(wit != wtext.end());
    wit = boost::boyer_moore(wtext, L"is");         assert(wit != wtext.end());
    std::advance(wit, 2);
}
#endif


    pattern= "time";
    boost::boyer_moore_pattern_search<std::ptrdiff_t,
                                      std::string::const_iterator,
                                      std::string::const_iterator> srch(pattern.begin(), pattern.end());
    srch.search(text.begin(), text.end());
    text = "time waits for no man";
    srch.search(text.begin(), text.end());
    text = "tide and time wait for no man";
    srch.search(text.begin(), text.end());


    std::cout << std::endl
              << "Boyer Moore Search Test"
              << std::endl
              << "Finished."
              << std::endl;

    return 0;
}

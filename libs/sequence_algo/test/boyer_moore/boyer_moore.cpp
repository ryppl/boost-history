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
#include <string>
#include <vector>
#include <string.h>

// MSVC6 chokes on using the iterator_traits, so this class must be
// specialised for that compiler
#if defined(BOOST_MSVC)  &&  (_MSC_VER <= 1300)
struct boost::boyer_moore_traits<std::string::const_iterator>
{
    typedef
    std::string::value_type
    alphabet_type;

    BOOST_STATIC_CONSTANT(unsigned long,
                          alphabet_size = (1<<(sizeof(alphabet_type)*8))-1);
};

struct boost::boyer_moore_traits<std::wstring::const_iterator>
{
    typedef
    std::wstring::value_type
    alphabet_type;

    BOOST_STATIC_CONSTANT(unsigned long,
                          alphabet_size = (1<<(sizeof(alphabet_type)*8))-1);
};
#endif



// wrappers for common usage
inline
std::string::const_iterator
boyer_moore(const std::string &text,
            const std::string &pattern)
{
    std::vector<std::string::const_iterator> vec;
    boost::boyer_moore_search<std::string::const_iterator,
                              std::string::const_iterator,
                              std::string::const_iterator>(text.begin(),
                                                           text.end(),
                                                           pattern.begin(),
                                                           pattern.end(),
                                                           std::back_inserter<>(vec));
    return (vec.size() == 0)? text.end() : vec.front();
}

inline
std::wstring::const_iterator
boyer_moore(const std::wstring &text,
            const std::wstring &pattern)
{
    std::vector<std::wstring::const_iterator> vec;
    boost::boyer_moore_search<std::wstring::const_iterator,
                              std::wstring::const_iterator,
                              std::wstring::const_iterator>(text.begin(),
                                                            text.end(),
                                                            pattern.begin(),
                                                            pattern.end(),
                                                            std::back_inserter<>(vec));
    return (vec.size() == 0)? text.end() : vec.front();
}

inline
std::string::const_iterator
boyer_moore(const std::string             &text,
            const std::string::value_type *pattern)
{
    std::string p(pattern);
    std::vector<std::string::const_iterator> vec;
    boost::boyer_moore_search<std::string::const_iterator,
                              std::string::const_iterator,
                              std::string::const_iterator>(text.begin(),
                                                           text.end(),
                                                           p.begin(),
                                                           p.end(),
                                                           std::back_inserter<>(vec));
    return (vec.size() == 0)? text.end() : vec.front();
}

inline
std::wstring::const_iterator
boyer_moore(const std::wstring             &text,
            const std::wstring::value_type *pattern)
{
    std::wstring p(pattern);
    std::vector<std::wstring::const_iterator> vec;
    boost::boyer_moore_search<std::wstring::const_iterator,
                              std::wstring::const_iterator,
                              std::wstring::const_iterator>(text.begin(),
                                                            text.end(),
                                                            p.begin(),
                                                            p.end(),
                                                            std::back_inserter<>(vec));
    return (vec.size() == 0)? text.end() : vec.front();
}

inline
const std::string::value_type *
boyer_moore(const std::string::value_type *text,
            const std::string::value_type *pattern)
{
    std::string t(text);
    std::string p(pattern);
    std::vector<std::ptrdiff_t> vec;
    boost::boyer_moore_search<std::ptrdiff_t,
                              std::string::const_iterator,
                              std::string::const_iterator>(t.begin(),
                                                           t.end(),
                                                           p.begin(),
                                                           p.end(),
                                                           std::back_inserter<>(vec));
    return (vec.size() == 0)?
               text + std::char_traits<std::string::value_type>::length(text)
             : text + vec.front();
}

inline
const std::wstring::value_type *
boyer_moore(const std::wstring::value_type *text,
            const std::wstring::value_type *pattern)
{
    std::wstring t(text);
    std::wstring p(pattern);
    std::vector<std::ptrdiff_t> vec;
    boost::boyer_moore_search<std::ptrdiff_t,
                              std::wstring::const_iterator,
                              std::wstring::const_iterator>(t.begin(),
                                                            t.end(),
                                                            p.begin(),
                                                            p.end(),
                                                            std::back_inserter<>(vec));
    return (vec.size() == 0)?
               text + std::char_traits<std::wstring::value_type>::length(text)
             : text + vec.front();
}


int main(int, char *[])
{
    std::string text("this is an example foo bar");
    std::string pattern("example");

    std::string::const_iterator it;

    std::vector<std::string::const_iterator> vec;
    boost::boyer_moore_search<std::string::const_iterator,
                              std::string::const_iterator,
                              std::string::const_iterator>(text.begin(),
                                                           text.end(),
                                                           pattern.begin(),
                                                           pattern.end(),
                                                           std::back_inserter<>(vec));
    assert(vec.size() > 0);
    assert(*boyer_moore("this is an example foo bar", "example") != 0);
    it = boyer_moore(text, pattern);                            assert(it != text.end());
    it = boyer_moore(text, "this");                             assert(it != text.end());
    it = boyer_moore(text, "is an ex");                         assert(it != text.end());
    it = boyer_moore(text, "find this");                        assert(it == text.end());
    it = boyer_moore(text, "fo");                               assert(it != text.end());

    std::string is("is");
    vec.clear();
    boost::boyer_moore_search<std::string::const_iterator,
                              std::string::const_iterator,
                              std::string::const_iterator>(text.begin(),
                                                           text.end(),
                                                           is.begin(),
                                                           is.end(),
                                                           std::back_inserter<>(vec));
    assert(vec.size() == 2);


    text = "humpty dumpty sat on a wall, "
           "humpty dumpty had a great fall, "
           "all the kings horses and all the kings men, "
           "couldn't put humpty together again.";
    it = boyer_moore(text, "together again");                   assert(it != text.end());
    it = boyer_moore(text, "Together");                         assert(it == text.end());
    assert(text.find("together") != std::string::npos);


#if defined(BOOST_MSVC)
{
    std::wstring wtext(L"this is an example foo bar");
    std::wstring wpattern(L"example");

    std::wstring::const_iterator wit;
    assert(*boyer_moore(L"this is an example foo bar", L"example") != 0);

    std::vector<std::ptrdiff_t> vec;
    boost::boyer_moore_search<std::ptrdiff_t,
                              std::wstring::const_iterator,
                              std::wstring::const_iterator>(wtext.begin(),
                                                            wtext.end(),
                                                            wpattern.begin(),
                                                            wpattern.end(),
                                                            std::back_inserter<>(vec));
    assert(vec.size() > 0);

    wit = boyer_moore(wtext, wpattern);      assert(wit != wtext.end());
    wit = boyer_moore(wtext, L"this");       assert(wit != wtext.end());
    wit = boyer_moore(wtext, L"is an ex");   assert(wit != wtext.end());
    wit = boyer_moore(wtext, L"find this");  assert(wit == wtext.end());
    wit = boyer_moore(wtext, L"fo");         assert(wit != wtext.end());
    wit = boyer_moore(wtext, L"is");         assert(wit != wtext.end());
    std::advance(wit, 2);
    wit = boyer_moore(&(*wit), L"is");       assert(wit != wtext.end());
}
#endif

    std::cout << std::endl
              << "Boyer Moore Search Test"
              << std::endl
              << "Finished."
              << std::endl;

    return 0;
}

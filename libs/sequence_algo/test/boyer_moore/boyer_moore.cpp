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
#include <string>
#include <vector>
#include <string.h>

std::string::const_iterator boyer_moore(const std::string &text, const char *pattern)
{
    const char *it = pattern + strlen(pattern);
    return boost::boyer_moore_search<>(text.begin(), text.end(), pattern, it);
}

std::string::const_iterator boyer_moore(const char *text, const char *pattern)
{
    std::string t(text);
    std::string p(pattern);
    return boost::boyer_moore_search<>(t.begin(), t.end(), p.begin(), p.end());
}

int main(int, char *[])
{
    std::string text("this is an example foo bar");
    std::string pattern("example");

    std::string::const_iterator it;
    it = boost::boyer_moore_search<>(text.begin(), text.end(), pattern.begin(), pattern.end());
    it = boyer_moore(text, "this");         assert(it != text.end());
    it = boyer_moore(text, "is an ex");     assert(it != text.end());
    it = boyer_moore(text, "find this");    assert(it == text.end());
    it = boyer_moore(text, "fo");           assert(it != text.end());
    it = boyer_moore(text, "is");           assert(it != text.end());
    std::advance(it, 2);
    it = boyer_moore(&(*it), "is");         assert(it != text.end());
}

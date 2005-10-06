///////////////////////////////////////////////////////////////////////////////
// literals.hpp
//
//  Copyright 2004 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_XPRESSIVE_DETAIL_UTILITY_LITERALS_HPP_EAN_10_04_2005
#define BOOST_XPRESSIVE_DETAIL_UTILITY_LITERALS_HPP_EAN_10_04_2005

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif

#include <boost/config.hpp> // for BOOST_STATIC_CONSTANT

namespace boost { namespace xpressive { namespace detail
{

///////////////////////////////////////////////////////////////////////////////
// char_literal
//
template<typename CharT, char Ch, wchar_t Wch>
struct char_literal;

template<char Ch, wchar_t Wch>
struct char_literal<char, Ch, Wch>
{
    BOOST_STATIC_CONSTANT(char, value = Ch);
};

template<char Ch, wchar_t Wch>
struct char_literal<wchar_t, Ch, Wch>
{
    BOOST_STATIC_CONSTANT(wchar_t, value = Wch);
};

#ifndef BOOST_NO_INCLASS_MEMBER_INITIALIZATION
template<char Ch, wchar_t Wch>
char const char_literal<char, Ch, Wch>::value;

template<char Ch, wchar_t Wch>
wchar_t const char_literal<wchar_t, Ch, Wch>::value;
#endif

template<typename Ch>
struct string_literal;

template<>
struct string_literal<char>
{
    template<std::size_t N>
    static char const (& pick(char const (&cstr)[N], wchar_t const (&)[N]))[ N ]
    {
        return cstr;
    }

    static char pick(char ch, wchar_t)
    {
        return ch;
    }
};

template<>
struct string_literal<wchar_t>
{
    template<std::size_t N>
    static wchar_t const (& pick(char const (&)[N], wchar_t const (&cstr)[N]))[ N ]
    {
        return cstr;
    }

    static wchar_t pick(char, wchar_t ch)
    {
        return ch;
    }
};

#define BOOST_XPR_CHAR_(CharT, ch) boost::xpressive::detail::char_literal<CharT, ch, L##ch>::value
#define BOOST_XPR_CSTR_(CharT, st) boost::xpressive::detail::string_literal<CharT>::pick(st, L##st)

}}} // namespace boost::xpressive::detail

#endif

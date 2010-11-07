//  basic_str.hpp  --------------------------------------------------------------//

//  Copyright 2010 Vicente J. Botet Escriba

//  Distributed under the Boost Software License, Version 1.0.
//  See http://www.boost.org/LICENSE_1_0.txt


#ifndef BOOST_STATIC_STRING_BASIC_STR_HPP
#define BOOST_STATIC_STRING_BASIC_STR_HPP

#include <boost/config.hpp>
#include <boost/mpl/vector/vector10_c.hpp>

#if defined(BOOST_NO_UNICODE_LITERALS) || defined(BOOST_NO_CHAR16_T) || defined(BOOST_NO_CHAR32_T)
#else
#define BOOST_STATIC_STRING_HAS_UNICODE_SUPPORT 1
#endif

namespace boost {
namespace static_string {

struct nil {};

template <char H1>
struct str_1 {
    typedef mpl::vector1_c<char, H1> type;
};

template <char H1, char H2>
struct str_2 {
    typedef mpl::vector2_c<char, H1, H2> type;
};

template <char H1, char H2, char H3>
struct str_3 {
    typedef mpl::vector3_c<char, H1, H2, H3> type;
};

template <char H1, char H2, char H3, char H4>
struct str_4 {
    typedef mpl::vector4_c<char, H1, H2, H3, H4> type;
};

template <char H1, char H2, char H3, char H4, char H5>
struct str_5 {
    typedef mpl::vector5_c<char, H1, H2, H3, H4, H5> type;
};


#ifndef BOOST_NO_STD_WSTRING

template <wchar_t H1>
struct wstr_1 {
    typedef mpl::vector1_c<wchar_t, H1> type;
};

template <wchar_t H1, wchar_t H2>
struct wstr_2 {
    typedef mpl::vector2_c<wchar_t, H1, H2> type;
};

template <wchar_t H1, wchar_t H2, wchar_t H3>
struct wstr_3 {
    typedef mpl::vector3_c<wchar_t, H1, H2, H3> type;
};

template <wchar_t H1, wchar_t H2, wchar_t H3, wchar_t H4>
struct wstr_4 {
    typedef mpl::vector4_c<wchar_t, H1, H2, H3, H4> type;
};

template <wchar_t H1, wchar_t H2, wchar_t H3, wchar_t H4, wchar_t H5>
struct wstr_5 {
    typedef mpl::vector5_c<wchar_t, H1, H2, H3, H4, H5> type;
};
#endif

#ifdef BOOST_STATIC_STRING_HAS_UNICODE_SUPPORT
template <char16_t H1>
struct u16str_1 {
    typedef mpl::vector1_c<char16_t, H1> type;
};

template <char16_t H1, char16_t H2>
struct u16str_2 {
    typedef mpl::vector2_c<char16_t, H1, H2> type;
};

template <char16_t H1, char16_t H2, char16_t H3>
struct u16str_3 {
    typedef mpl::vector3_c<char16_t, H1, H2, H3> type;
};

template <char16_t H1, char16_t H2, char16_t H3, char16_t H4>
struct u16str_4 {
    typedef mpl::vector4_c<char16_t, H1, H2, H3, H4> type;
};

template <char16_t H1, char16_t H2, char16_t H3, char16_t H4, char16_t H5>
struct u16str_5 {
    typedef mpl::vector5_c<char16_t, H1, H2, H3, H4, H5> type;
};

/////////

template <char32_t H1>
struct u32str_1 {
    typedef mpl::vector1_c<char32_t, H1> type;
};

template <char32_t H1, char32_t H2>
struct u32str_2 {
    typedef mpl::vector2_c<char32_t, H1, H2> type;
};

template <char32_t H1, char32_t H2, char32_t H3>
struct u32str_3 {
    typedef mpl::vector3_c<char32_t, H1, H2, H3> type;
};

template <char32_t H1, char32_t H2, char32_t H3, char32_t H4>
struct u32str_4 {
    typedef mpl::vector4_c<char32_t, H1, H2, H3, H4> type;
};

template <char32_t H1, char32_t H2, char32_t H3, char32_t H4, char32_t H5>
struct u32str_5 {
    typedef mpl::vector5_c<char32_t, H1, H2, H3, H4, H5> type;
};

#endif


} // namespace static_string
} // namespace boost

#endif // BOOST_STATIC_STRING_BASIC_STR_HPP

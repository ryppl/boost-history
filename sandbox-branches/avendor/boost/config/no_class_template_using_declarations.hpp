//-----------------------------------------------------------------------------
// boost config/no_class_template_using_declarations.hpp header file
// See http://www.boost.org for updates, documentation, and revision history.
//-----------------------------------------------------------------------------
//
// Copyright (c) 2002
// Eric Friedman
//
// Permission to use, copy, modify, distribute and sell this software
// and its documentation for any purpose is hereby granted without fee, 
// provided that the above copyright notice appears in all copies and 
// that both the copyright notice and this permission notice appear in 
// supporting documentation. No representations are made about the 
// suitability of this software for any purpose. It is provided "as is" 
// without express or implied warranty.

// TODO: The functionality of this header should be handled by individual
//   compiler config headers and not by a centralized header.
// WARNING: The set of compilers flagged by this header is far from
//   complete! Compilers may be tested with the <code> below.

#ifndef BOOST_CONFIG_NO_CLASS_TEMPLATE_USING_DECLARATIONS_HPP
#define BOOST_CONFIG_NO_CLASS_TEMPLATE_USING_DECLARATIONS_HPP

// BOOST_NO_CLASS_TEMPLATE_USING_DECLARATIONS
//
// Defined if using declarations cannot be used in class templates
//   as in the following example:
// <code>
// struct test_base {
//	static void f() { }
// };
//
// template <typename T, typename base>
// struct test_using : base {
// 	using base::f;
// 	static T f(const T& t) { return t; }
// };
//
// int main() {
//  test_using<int, test_using<double, test_base> > test;
//  test.f();
//  test.f(2);
//  test.f(3.14);
// }
// </code>
#if defined(BOOST_MSVC) || defined(__GNUC__)
#	define BOOST_NO_CLASS_TEMPLATE_USING_DECLARATIONS
#endif

#endif // BOOST_CONFIG_NO_CLASS_TEMPLATE_USING_DECLARATIONS_HPP

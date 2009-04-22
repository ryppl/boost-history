/**
 * \file test/classes.hpp
 *
 * This file is part of the Mirror library testsuite.
 *
 * Several test classes
 *
 *  Copyright 2008 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */
#ifndef BOOST_MIRROR_LIBS_MIRROR_TEST_CLASSES_HPP
#define BOOST_MIRROR_LIBS_MIRROR_TEST_CLASSES_HPP

#include "./namespaces.hpp"

namespace test {

struct A
{
	::std::string a;
};

struct B : A
{
	bool b;
};

struct C : B
{
	char c;
};

struct D : C
{
	double d;
};

struct E : D
{
	wchar_t e;
};

struct F : E
{
	float f;
};

struct G : F
{
	unsigned g;
};

struct H : G
{
	short h;
};

} // namespace test

namespace boost {
namespace mirror {

BOOST_MIRROR_QREG_CLASS_NO_BASE(::test, A, (a))
BOOST_MIRROR_QREG_CLASS(::test, B, (::test::A), (b))
BOOST_MIRROR_QREG_CLASS(::test, C, (::test::B), (c))
BOOST_MIRROR_QREG_CLASS(::test, D, (::test::C), (d))
BOOST_MIRROR_QREG_CLASS(::test, E, (::test::D), (e))
BOOST_MIRROR_QREG_CLASS(::test, F, (::test::E), (f))
BOOST_MIRROR_QREG_CLASS(::test, G, (::test::F), (g))
BOOST_MIRROR_QREG_CLASS(::test, H, (::test::G), (h))

} // namespace miror
} // namespace boost

#endif

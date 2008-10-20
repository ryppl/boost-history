/**
 * \file test/types_ct_04.cpp
 *
 * This file is part of the Mirror library testsuite.
 *
 * Testing of meta_types for derived types and 
 * complex type name getters.
 *
 *  Copyright 2008 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#include <boost/mirror/meta_namespace.hpp>
#include <boost/mirror/meta_type.hpp>
#include <boost/mirror/meta_types/std/list.hpp>
#include <boost/mirror/meta_types/std/set.hpp>
#include <boost/mirror/meta_types/std/map.hpp>
#include <boost/mirror/meta_types/std/pair.hpp>
#include <boost/mirror/meta_types/boost/tuples/tuple.hpp>
//
#include <cstdlib>
//
#include "./namespaces.hpp"
#include "./types.hpp"
#include "./test.hpp"

bool operator == (const ::std::string& str, const ::std::wstring& wstr)
{
	if(str.length() != wstr.length()) return false;
	//
	::std::string::const_iterator stri = str.begin();
	::std::string::const_iterator stre = str.end();
	::std::wstring::const_iterator wstri = wstr.begin();
	::std::wstring::const_iterator wstre = wstr.end();
	//
	// go through the strings
	while((stri != stre) && (wstri != wstre))
	{
		char ambc[2] = {*(stri++), '\0'};
		wchar_t wc = *(wstri++);
		wchar_t wcc = L'\0';
		mbtowc(&wcc, ambc, 1);
		// on mismatch return false
		if(wc != wcc) return false;
	}
	//
	// double check the iterators
	return (stri == stre) && (wstri == wstre);

}

void test_main()
{
	using namespace ::boost;
	typedef ::test::feature::detail::foo_impl X;
	typedef BOOST_MIRROR_TYPEDEF(::test, foobar) Y;
	typedef BOOST_MIRROR_TYPEDEF(::boost::cts, bstring) W;
	typedef long int Z;
	//
	typedef X * const T1;
	typedef Y volatile * const T2;
	typedef const volatile W T3;
	typedef Z const * const T4[][1][2][3][4][5][6][7][8][9];
	typedef T1& (T5)(T2, T3, T4);
	typedef T5* (T6)(T5*, T5&, Z);
	//
	typedef T6& T;
	//
	typedef BOOST_MIRRORED_TYPE(T) meta_T;
	BOOST_CHECK((meta_T::full_name().length() > 0));
	//
	typedef ::std::list<
		::std::set<
		::std::map<
			W,
			::std::pair<
				T6*,
				::boost::tuple<
					X,
					Y,
					W,
					Z,
					T6*
				>
			>
	>
	>
	> U;
	//
	typedef BOOST_MIRRORED_TYPE(U) meta_U;
	BOOST_CHECK((
		meta_U::full_name() ==
		meta_U::get_name(
			mpl::true_(),
			cts::bchar_traits()
		)
	));
	//
	// the char and wchar_t variants of the full
	// name must be equal
	//
	typedef BOOST_MIRRORED_TYPE(U) meta_U;
	BOOST_CHECK((
		meta_U::get_name(
			mpl::true_(),
			::std::char_traits<char>()
		) == 
		meta_U::get_name(
			mpl::true_(),
			::std::char_traits<wchar_t>()
		)
	));


}

test_suite* init_unit_test_suite( int argc, char* argv[] )
{
    test_suite *test = BOOST_TEST_SUITE("Mirror: types compile test 04");
    test->add(BOOST_TEST_CASE(&test_main));
    return test;
}



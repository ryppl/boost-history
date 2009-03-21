/**
 * \file examples/traversal/json_01.cpp
 *
 *  Sample showing the usage of the json_serializer
 *
 *  Copyright 2008 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#include <boost/char_type_switch/iostream.hpp>
//
#include <boost/mirror/meta_class.hpp>
#include <boost/mirror/meta_classes/std/pair.hpp>
#include <boost/mirror/meta_classes/boost/tuples/tuple.hpp>
//
#include <boost/mirror/visitors/json.hpp>
#include <boost/mirror/traversal.hpp>


template <class Class>
void test(Class value)
{
	using namespace ::std;
	using namespace ::boost;
	using namespace ::boost::mirror;
	
	flat_traversal_of<
		BOOST_MIRRORED_CLASS(Class)
	>::accept(
		make_json_serializer(cts::bcout(), mpl::false_()),
		&value
	);
	cts::bcout() << endl;
}

int main(void)
{
	using namespace ::std;
	using namespace ::boost;
	test(
		make_tuple(
			make_tuple(
				string("A"),
				string("B"),
				string("C")
			),
			12.34,
			make_tuple(
				'D',
				'E',
				'F'
			),
			56.78,
			make_tuple(
				123,
				456,
				789,
				(int*)0
			),
			90e12,
			pair<double, string>(
				34e-5,
				string("\"\f67")+
				string("\t89")+
				string("\r\n0\b")+
				string("/\\\"")
			),
			true,
			make_tuple(
				make_tuple(1,2),
				make_tuple(3,4),
				make_tuple(5,6),
				make_tuple(7,8),
				make_tuple(9,0)
			),
			false
		)
	);
	//
	return 0;
}

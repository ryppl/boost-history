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
		make_json_serializer(cts::bcout(), mpl::true_()),
		&value
	);
	cts::bcout() << endl;
}

int main(void)
{
	using namespace ::std;
	using namespace ::boost;
	//
	pair<cts::bstring, cts::bstring> p(
		BOOST_CTS_LIT("aábcćčdďeéěfghiíjklĺľmnňoôpqrŕřsśštťuúůvwxyýzž"),
		BOOST_CTS_LIT("AÁBCĆČDĎEÉĚFGHIÍJKLĹĽMNŇOÔPQRŔŘSŚŠTŤUÚŮVWXYÝZŽ")
	);
	test(p);
	//
	return 0;
}

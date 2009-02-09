/**
 * \file examples/factories/tetrahedron.cpp
 *
 *  This example shows how to use the configurable factory
 *  with a input user interface implementation to generically 
 *  construct an instance of classes with non default constructors.
 *
 *  Copyright 2008 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */


#include <boost/char_type_switch/iostream.hpp>
#include <boost/mirror/factory.hpp>
#include <boost/mirror/meta_type.hpp>

#include "./input_ui.hpp"
#include "./tetrahedron.hpp"


int main(void)
{
	using namespace ::std;
	using namespace ::boost;
	using namespace ::boost::mirror;
	//
	// create a factory plugged with the input ui
	const cts::bchar* param_names[] = {BOOST_CTS_LIT("a tetrahedron")};
	factory< ::test::input_ui, ::test::tetrahedron > f(param_names, 0);
	// use the factory to construct a tetrahedron and calculate 
	// it's volume and base area
	::test::tetrahedron t(f());
	// ... and print them out
	cts::bcout() << BOOST_CTS_LIT("the volume is ") << t.volume() << endl;
	cts::bcout() << BOOST_CTS_LIT("the area of the base is ") << t.base.area() << endl;
	//
	return 0;
}

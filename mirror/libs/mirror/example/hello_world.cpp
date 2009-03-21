/**
 * \file examples/hello_world.cpp
 *
 * Simple yet not very useful hello world example.
 * Shows the basics of namespace and type registration and reflection
 *
 * NOTE: if You are not familiar with registration
 * and reflection, You should probably 
 * see examples in examples/registering/ first.
 *
 *  Copyright 2008 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#include <boost/char_type_switch/iostream.hpp>
#include <boost/mirror/meta_namespace.hpp>
#include <boost/mirror/meta_type.hpp>

namespace Hello {

struct World { };

} // namespace Hello

namespace boost { 
namespace mirror {

/** Register the Hello namespace
 */
BOOST_MIRROR_REG_NAMESPACE((Hello))

/** Register the ::Hello::World type
 */
BOOST_MIRROR_REG_TYPE(::Hello, World)

} // namespace mirror
} // namespace boost


int main(void)
{
	using namespace ::std;
	using namespace ::boost;
	using namespace ::boost::mirror;
	// 
	// cts::bcout() returns a reference to ::std::cout 
	// or ::std::wcout based on the used character type
	//
	cts::bcout() << 
		BOOST_MIRRORED_TYPE(::Hello::World)::full_name() << 
		endl;

	cts::bcout() <<
		BOOST_MIRRORED_TYPE(::Hello::World)::scope::base_name() << 
		BOOST_CTS_LIT(", ") <<
		BOOST_MIRRORED_TYPE(::Hello::World)::base_name() << 
		BOOST_CTS_LIT(".") <<
		endl;
	//
	return 0;
}


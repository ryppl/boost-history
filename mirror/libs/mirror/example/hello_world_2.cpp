/**
 * \file examples/hello_world_2.cpp
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
#include <boost/mirror/meta_class.hpp>
#include <boost/mirror/intrinsic/by_name.hpp>
#include <boost/mirror/intrinsic/at.hpp>

struct Greeting 
{
	int Hello;
	int World;
};

namespace boost { 
namespace mirror {

/** Register the ::Greeting class and it's member
 *  attributes.
 */
BOOST_MIRROR_REG_TYPE_GLOBAL_SCOPE(Greeting)

BOOST_MIRROR_REG_CLASS_ATTRIBS_BEGIN(::Greeting)
        BOOST_MIRROR_REG_AUTO_CLASS_ATTRIB(_, Hello)
        BOOST_MIRROR_REG_AUTO_CLASS_ATTRIB(_, World)
BOOST_MIRROR_REG_CLASS_ATTRIBS_END

} // namespace mirror
} // namespace boost


int main(void)
{
	using namespace ::std;
	using namespace ::boost;
	using namespace ::boost::mirror;
	//
	//
	typedef BOOST_MIRRORED_CLASS(::Greeting) meta_Greeting;
	//
	// cts::bcout() returns a reference to ::std::cout 
	// or ::std::wcout based on the used character type
	//
	cts::bcout() <<
		by_name<meta_Greeting::all_attributes>::Hello::base_name() <<
		BOOST_CTS_LIT(", ") <<
		by_name<meta_Greeting::all_attributes>::World::base_name() <<
		BOOST_CTS_LIT(".") <<
		endl;
	//
	cts::bcout() <<
		at<meta_Greeting::attributes, mpl::int_<0> >::type::base_name() <<
		BOOST_CTS_LIT(", ") <<
		at<meta_Greeting::attributes, mpl::int_<1> >::type::base_name() <<
		BOOST_CTS_LIT(".") <<
		endl;
	//
	return 0;
}


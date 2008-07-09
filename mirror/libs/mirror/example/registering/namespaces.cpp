/**
 * \file examples/registering/namespaces.cpp
 * Example of namespace registering and reflection with
 * the mirror library
 *
 *  Copyright 2008 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

// narrow/wide char stream
#include <boost/char_type_switch/iostream.hpp>
// namespace registering 
#include <boost/mirror/meta_namespace.hpp>
//
// mpl size meta function
#include <boost/mpl/size.hpp>


namespace test {
namespace feature {
namespace detail {

} // namespace detail
} // namespace feature

namespace stuff {
namespace detail {

} // namespace detail
} // namespace stuff
} // namespace test


namespace boost { 
namespace mirror {

// register the namespaces
//
BOOST_MIRROR_REG_NAMESPACE((test))
//
BOOST_MIRROR_REG_NAMESPACE((test)(feature))
BOOST_MIRROR_REG_NAMESPACE((test)(feature)(detail))
// 
BOOST_MIRROR_REG_NAMESPACE((test)(stuff))
BOOST_MIRROR_REG_NAMESPACE((test)(stuff)(detail))

} // namespace mirror
} // namespace boost

int main(void)
{
	using namespace ::std;
	using namespace ::boost;
	using namespace ::boost::mirror;
	//
	// NOTE bcout ~ cout when using narrow chars
	//            ~ wcout when using wide chars
	// it's typedef'd in the boost namespace
	//
	// the BOOST_MIRRORED_NAMESPACE(FULL_NAMESPACE_NAME) returns the 
	// meta_namespace<> specialization for the given namespace 
	//
	// define an alternative name for the global scope meta-namespace
	// BOOST_MIRRORED_NAMESPACE_GS is equivalent to this
	typedef BOOST_MIRRORED_GLOBAL_SCOPE() meta_ns_global_scope;

	// define an alternative name for the ::test namespace meta-data
	typedef BOOST_MIRRORED_NAMESPACE(::test) meta_ns_test;
	typedef BOOST_MIRRORED_NAMESPACE(::test::stuff::detail) meta_ns_test_stuff_detail;
	//
	//
	// the base name function allows to get the base name
	// of the namespace
	//
	// print the base name of the namespace
	bcout << "|01| " <<meta_ns_test_stuff_detail::base_name() << endl;
	//
	// the 'parent' type is the meta_namespace<> for the parent
	// namespace of the given namespace
	//
	// print the base name of the parent namespace 
	bcout << "|02| " <<meta_ns_test_stuff_detail::scope::base_name() << endl;
	bcout << "|03| " <<meta_ns_test_stuff_detail::scope::scope::base_name() << endl;
	//
	// the 'scope' member is a mpl::vector containing the whole 
	// list of ancestor namespaces 
	//
	// find out and print the 'depth' of the namespace
	bcout << "|04| " <<mpl::size<meta_ns_global_scope::ancestors>::value << endl;
	bcout << "|05| " <<mpl::size<meta_ns_test::ancestors>::value << endl;
	bcout << "|06| " <<mpl::size<meta_ns_test_stuff_detail::ancestors>::value << endl;
	//
	//
	// printing out the full namespace names directly without the name_to_stream
	// printer template
	bcout << "|07| " << BOOST_MIRRORED_GLOBAL_SCOPE() ::full_name() << endl;
	bcout << "|08| " << BOOST_MIRRORED_NAMESPACE(::std) ::full_name() << endl;
	bcout << "|09| " << BOOST_MIRRORED_NAMESPACE(::boost) ::full_name() << endl;
	bcout << "|10| " << BOOST_MIRRORED_NAMESPACE(::boost::mirror) ::full_name() << endl;
	bcout << "|11| " << BOOST_MIRRORED_NAMESPACE(::test) ::full_name() << endl;
	bcout << "|12| " << BOOST_MIRRORED_NAMESPACE(::test::stuff) ::full_name() << endl;
	bcout << "|13| " << BOOST_MIRRORED_NAMESPACE(::test::stuff::detail) ::full_name() << endl;
	return 0;
}


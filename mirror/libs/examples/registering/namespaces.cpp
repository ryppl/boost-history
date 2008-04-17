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
// utility that allows to put the name of the namespace to a given stream
#include <boost/mirror/utils/name_to_stream/namespace.hpp>
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
// the namespace 'test' in the global scope
BOOST_MIRROR_REG_META_NAMESPACE_TOP_LEVEL(test)
// namespace feature in namespace ::test 
// the _test is a "namespace alias" for namespace ::test
// _ is the namespace alias for the global scope
BOOST_MIRROR_REG_META_NAMESPACE(_test, feature)
BOOST_MIRROR_REG_META_NAMESPACE(_test_feature, detail)
// 
BOOST_MIRROR_REG_META_NAMESPACE(_test, stuff)
BOOST_MIRROR_REG_META_NAMESPACE(_test_stuff, detail)

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
	// the BOOST_MIRROR_REFLECT_NAMESPACE(NAMESPACE_ALIAS) returns the 
	// meta_namespace<> specialization for the given namespace alias
	//
	// define an alternative name for the global scope meta-data
	typedef BOOST_MIRROR_REFLECT_NAMESPACE(_) meta_ns_global_scope;
	// define an alternative name for the ::test namespace meta-data
	typedef BOOST_MIRROR_REFLECT_NAMESPACE(_test) meta_ns_test;
	typedef BOOST_MIRROR_REFLECT_NAMESPACE(_test_stuff_detail) meta_ns_test_stuff_detail;
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
	bcout << "|02| " <<meta_ns_test_stuff_detail::parent::base_name() << endl;
	bcout << "|03| " <<meta_ns_test_stuff_detail::parent::parent::base_name() << endl;
	//
	// the 'scope' member is a mpl::vector containing the whole 
	// list of ancestor namespaces 
	//
	// find out and print the 'depth' of the namespace
	bcout << "|04| " <<mpl::size<meta_ns_global_scope::scope>::value << endl;
	bcout << "|05| " <<mpl::size<meta_ns_test::scope>::value << endl;
	bcout << "|06| " <<mpl::size<meta_ns_test_stuff_detail::scope>::value << endl;
	//
	// the name_to_stream<meta_object> class allows to put the full name
	// (including the scope) into a stream
	bcout << "|07| " <<name_to_stream< BOOST_MIRROR_REFLECT_NAMESPACE(_) >() << endl;
	bcout << "|08| " <<name_to_stream< BOOST_MIRROR_REFLECT_NAMESPACE(_test) >() << endl;
	bcout << "|09| " <<name_to_stream< BOOST_MIRROR_REFLECT_NAMESPACE(_test_stuff) >() << endl;
	bcout << "|10| " <<name_to_stream< BOOST_MIRROR_REFLECT_NAMESPACE(_test_stuff_detail) >() << endl;
	// in this case the :: is prepended to the full name
	// thus test::stuff -> ::test::stuff
	bcout << "|11| " <<name_to_stream< BOOST_MIRROR_REFLECT_NAMESPACE(_) >(true) << endl;
	bcout << "|12| " <<name_to_stream< BOOST_MIRROR_REFLECT_NAMESPACE(_test) >(true) << endl;
	bcout << "|13| " <<name_to_stream< BOOST_MIRROR_REFLECT_NAMESPACE(_test_stuff) >(true) << endl;
	bcout << "|14| " <<name_to_stream< BOOST_MIRROR_REFLECT_NAMESPACE(_test_stuff_detail) >(true) << endl;
	//
	// there are few namespace registered by default
	// including (::std, ::boost, ::boost::mirror)
	bcout << "|15| " <<name_to_stream< BOOST_MIRROR_REFLECT_NAMESPACE(_std) >(true) << endl;
	bcout << "|16| " <<name_to_stream< BOOST_MIRROR_REFLECT_NAMESPACE(_boost) >(true) << endl;
	bcout << "|17| " <<name_to_stream< BOOST_MIRROR_REFLECT_NAMESPACE(_boost_mirror) >(true) << endl;
	return 0;
}


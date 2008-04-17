/**
 * \file examples/registering/types.hpp
 * Example of type registering and reflection with
 * the mirror library
 *
 *  Copyright 2008 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

// narrow/wide char stream
#include <boost/char_type_switch/iostream.hpp>

#include <boost/mirror/meta_namespace.hpp>
#include <boost/mirror/meta_type.hpp>
// utility that allows to put the name of the type to a given stream
#include <boost/mirror/utils/name_to_stream/type.hpp>
//
//
#include <boost/typeof/typeof.hpp>



namespace test {
namespace feature {
namespace detail {

struct foo { };
struct bar { };

} // namespace detail
} // namespace feature
} // namespace test


namespace boost { 
namespace mirror {

// register the namespaces
//
// NOTE: if You are not familiar with namespace registration and usage
// see the examples/registering/namespaces.cpp source file
//
BOOST_MIRROR_REG_META_NAMESPACE_TOP_LEVEL(test)
BOOST_MIRROR_REG_META_NAMESPACE(_test, feature)
BOOST_MIRROR_REG_META_NAMESPACE(_test_feature, detail)
//
// register the types
BOOST_MIRROR_REG_META_TYPE(_test_feature_detail, ::test::feature::detail, foo)
BOOST_MIRROR_REG_META_TYPE(_test_feature_detail, ::test::feature::detail, bar)

} // namespace mirror
} // namespace boost


int main(void)
{
	using namespace ::std;
	using namespace ::boost;
	using namespace ::boost::mirror;
	//
	using ::test::feature::detail::foo;
	using ::test::feature::detail::bar;
	//
	// The BOOST_MIRROR_REFLECT_TYPE(TYPE) macro expands into the meta_type<>
	// specialization for the given type
	// The result is a type name
	//
	typedef BOOST_MIRROR_REFLECT_TYPE(foo) meta_foo;
	typedef BOOST_MIRROR_REFLECT_TYPE(bar) meta_bar;
	//
	// put the full name of the type to the output stream
	//
	bcout << name_to_stream< BOOST_MIRROR_REFLECT_TYPE(foo) >() << endl;
	bcout << name_to_stream< BOOST_MIRROR_REFLECT_TYPE(bar) >() << endl;
	//
	// do the same thing with the typedef'd type
	// actually, nearly the same .. the true argument given to the 
	// constructor makes the name_to_stream<> template output the 
	// leading :: too.
	bcout << name_to_stream< meta_foo >(true) << endl;
	bcout << name_to_stream< meta_bar >(true) << endl;
	//
	// this works too...
	bcout << name_to_stream< BOOST_MIRROR_REFLECT_TYPE(foo*) >() << endl;
	bcout << name_to_stream< BOOST_MIRROR_REFLECT_TYPE(bar&) >() << endl;
	bcout << name_to_stream< BOOST_MIRROR_REFLECT_TYPE(foo***) >() << endl;
	bcout << name_to_stream< BOOST_MIRROR_REFLECT_TYPE(bar**&) >() << endl;
	bcout << name_to_stream< BOOST_MIRROR_REFLECT_TYPE(const foo**) >() << endl;
	bcout << name_to_stream< BOOST_MIRROR_REFLECT_TYPE(volatile bar*&) >() << endl;
	//
	bcout << name_to_stream< BOOST_MIRROR_REFLECT_TYPE(const volatile foo) >() << endl;
	bcout << name_to_stream< BOOST_MIRROR_REFLECT_TYPE(const volatile bar*) >() << endl;
	//
	// native c++ types are registered by default
	bcout << name_to_stream< BOOST_MIRROR_REFLECT_TYPE(volatile short int) >() << endl;
	bcout << name_to_stream< BOOST_MIRROR_REFLECT_TYPE(const char*) >() << endl;
	bcout << name_to_stream< BOOST_MIRROR_REFLECT_TYPE(wchar_t*) >() << endl;
	bcout << name_to_stream< BOOST_MIRROR_REFLECT_TYPE(bool) >() << endl;
	//
	// use with Boost.Typeof
	bcout << name_to_stream< BOOST_MIRROR_REFLECT_TYPE(BOOST_TYPEOF(1+1)) >() << endl;
	// ... and maybe more conveniently
	bcout << name_to_stream< BOOST_MIRROR_REFLECT_TYPEOF(1.0/2.0) >() << endl;
	//
	// sometimes it is useful to distinguish between a typedef'd type
	// and the 'source' type.
	//
	// this reflects bchar as char or wchar_t defined 
	// on compilation configuration
	typedef BOOST_MIRROR_REFLECT_TYPE(bchar) meta_bchar;
	// this returns a meta-type that allows to query the typedef'd name
	typedef BOOST_MIRROR_REFLECT_TYPEDEFD(_boost, bchar) meta_bchar_td;
	//
	bcout << name_to_stream< meta_bchar    >() << endl;
	bcout << name_to_stream< meta_bchar_td >() << endl;
	//
	// unfortunately BOOST_MIRROR_REFLECT_TYPEDEFD(...) works only if
	// the typedefined name is explicitly given 
	// still it can be useful in certin situations.
	// However, the next line for example would fail to compile
	//typedef BOOST_MIRROR_REFLECT_TYPEDEFD(_boost, BOOST_TYPEOF(L'x')) meta_bchar_td;
	//
	//
	return 0;
}


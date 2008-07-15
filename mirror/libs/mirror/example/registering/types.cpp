/**
 * \file examples/registering/types.cpp
 * Example of type registering and reflection with
 * the mirror library
 *
 *  Copyright 2008 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

// narrow/wide char stream
#include <boost/char_type_switch/iostream.hpp>
// namespace and type related meta data
#include <boost/mirror/meta_namespace.hpp>
#include <boost/mirror/meta_type.hpp>
// traits
#include <boost/mirror/traits/reflects_global_scope.hpp>
// boost typeof
#include <boost/typeof/typeof.hpp>

// define some namespaces
namespace test {
namespace feature {
namespace detail {

// and inside some types 
struct foo { };
struct bar { };
typedef double foobar;

} // namespace detail
} // namespace feature
} // namespace test

// define some types on the global scope
struct bar { };
struct baz { };
// typedef on the global scope
typedef double foobar;


namespace boost { 
namespace mirror {

// register the namespaces
//
// NOTE: if You are not familiar with namespace registration and usage
// see the examples/registering/namespaces.cpp source file
//
BOOST_MIRROR_REG_NAMESPACE((test))
BOOST_MIRROR_REG_NAMESPACE((test)(feature))
BOOST_MIRROR_REG_NAMESPACE((test)(feature)(detail))
//
// register the types
BOOST_MIRROR_REG_TYPE(::test::feature::detail, foo)
BOOST_MIRROR_REG_TYPE(::test::feature::detail, bar)
//
// register a typedef'd class this allows to distinguish it from
// the 'source' type in some situations
BOOST_MIRROR_REG_TYPEDEF(::test::feature::detail, foobar)
//
// register type on the global scope
BOOST_MIRROR_REG_TYPE_GLOBAL_SCOPE(bar)
// shorter version
BOOST_MIRROR_REG_TYPE_GS(baz)
//
// typedef on the global scope
// equivalent to BOOST_MIRROR_REG_TYPEDEF_GLOBAL_SCOPE(foobar) 
BOOST_MIRROR_REG_TYPEDEF_GS(foobar)

} // namespace mirror
} // namespace boost

namespace test {
	// a function on pointer to which we'll reflect later
	int zero(void){return 0;}

}

int main(void)
{
	using namespace ::std;
	using namespace ::boost;
	using namespace ::boost::mirror;
	//
	cts::bostream& bcout = cts::bcout();
	//
	using ::test::feature::detail::foo;
	using ::test::feature::detail::bar;
	using ::test::feature::detail::foobar;
	//
	// The BOOST_MIRRORED_TYPE(TYPE) macro expands into the meta_type<>
	// specialization for the given type
	//
	typedef BOOST_MIRRORED_TYPE(foo) meta_foo;
	typedef BOOST_MIRRORED_TYPE(bar) meta_bar;
	//
	// write the full name of the type to the output stream
	bcout << "|00| " << BOOST_MIRRORED_TYPE(int) ::full_name() << endl;
	bcout << "|01| " << BOOST_MIRRORED_TYPE(foo) ::full_name() << endl;
	bcout << "|02| " << BOOST_MIRRORED_TYPE(bar) ::full_name() << endl;
	//
	// Do the same thing with the typedef'd meta-type
	bcout << "|03| " << meta_foo ::full_name() << endl;
	bcout << "|04| " << meta_bar ::full_name() << endl;

	//
	// this works too...
	bcout << "|05| " << BOOST_MIRRORED_TYPE(foo*)::full_name() << endl;
	bcout << "|06| " << BOOST_MIRRORED_TYPE(bar&)::full_name() << endl;
	bcout << "|07| " << BOOST_MIRRORED_TYPE(foo***)::full_name() << endl;
	bcout << "|08| " << BOOST_MIRRORED_TYPE(bar**&) ::full_name() << endl;
	bcout << "|09| " << BOOST_MIRRORED_TYPE(const foo**)::full_name() << endl;
	bcout << "|10| " << BOOST_MIRRORED_TYPE(volatile bar*&)::full_name() << endl;
	bcout << "|11| " << BOOST_MIRRORED_TYPE(const volatile foo)::full_name() << endl;
	bcout << "|12| " << BOOST_MIRRORED_TYPE(const volatile bar*)::full_name() << endl;
	//
	// native c++ types are registered by default
	bcout << "|13| " << BOOST_MIRRORED_TYPE(volatile short int)::full_name() << endl;
	bcout << "|14| " << BOOST_MIRRORED_TYPE(const char*)::full_name() << endl;
	bcout << "|15| " << BOOST_MIRRORED_TYPE(wchar_t*)::full_name() << endl;
	bcout << "|16| " << BOOST_MIRRORED_TYPE(bool)::full_name() << endl;
	//
	// use with Boost.Typeof
	bcout << "|17| " << BOOST_MIRRORED_TYPE(BOOST_TYPEOF(1+1))::full_name() << endl;
	// ... and maybe more conveniently
	bcout << "|18| " << BOOST_MIRRORED_TYPEOF(1.0/2.0)::full_name() << endl;
	//

	// sometimes it is useful to distinguish between a typedef'd type
	// and the 'source' type.
	//
	// this reflects bchar as char or wchar_t based 
	// on compilation configuration
	typedef BOOST_MIRRORED_TYPE(cts::bchar) meta_bchar;
	// this reflects a meta-type that allows to query the typedef'd name
	typedef BOOST_MIRRORED_TYPEDEF(::boost::cts, bchar) meta_bchar_td;
	// this reflects the meta-type for the typedef type foobar
	typedef BOOST_MIRRORED_TYPEDEF(::test::feature::detail, foobar) meta_foobar_td;
	// 
	// use the typedefined meta-types
	bcout << "|19| " << meta_bchar     ::full_name() << endl;
	bcout << "|20| " << meta_bchar_td  ::full_name() << endl;
	bcout << "|21| " << meta_foobar_td ::full_name() << endl;
	//
	// unfortunately BOOST_MIRRORED_TYPEDEF(...) works only if
	// the typedefined name is explicitly given 
	// still it can be useful in certin situations.
	// However, the next line for example would fail to compile
	//typedef BOOST_MIRRORED_TYPEDEF(_boost, BOOST_TYPEOF(L'x')) meta_bchar_td;
	//
	// check whether the type is defined on the global scope
	if(!reflects_global_scope<meta_bchar_td::scope>::value)
		bcout << "|22| " << "bchar NOT defined on global scope" << endl;
	if(!reflects_global_scope<meta_foo::scope>::value)
		bcout << "|23| " << "foo NOT defined on global scope" << endl;
	//
	// use with typeof
	if(reflects_global_scope< BOOST_MIRRORED_TYPEOF(1+2) ::scope>::value)
		bcout << "|24| " << "type of (1+2) defined on global scope" << endl;
	//
	if(reflects_global_scope< BOOST_MIRRORED_TYPEOF("foo") ::scope>::value)
		bcout << "|25| " << "type of \"foo\" defined on global scope" << endl;
	//
	bcout << "|26| " << BOOST_MIRRORED_TYPE(bool) ::full_name() << endl;
	bcout << "|27| " << BOOST_MIRRORED_TYPE(cts::bchar) ::full_name() << endl;
	//
	// reflection of typedefined types
	bcout << "|28| " << BOOST_MIRRORED_TYPEDEF(::boost::cts, bchar) ::full_name() << endl;
	// or another way how to reflect a typedefined type
	bcout << "|29| " << BOOST_MIRRORED_TYPE(BOOST_MIRROR_TYPEDEF(::boost::cts, bstring) const * volatile *) ::full_name() << endl;
	//

	bcout << "|30| " << BOOST_MIRRORED_TYPE(int const * const volatile) ::full_name() << endl;
	bcout << "|31| " << BOOST_MIRRORED_TYPE(int volatile * const * volatile * const *) ::full_name() << endl;
	bcout << "|32| " << BOOST_MIRRORED_TYPE(int const * const [321]) ::full_name() << endl;
	bcout << "|33| " << BOOST_MIRRORED_TYPE(int * volatile * const [][123]) ::full_name() << endl;
	bcout << "|34| " << BOOST_MIRRORED_TYPE(bar [123][456][789]) ::full_name() << endl;
	//
	bcout << "|35| " << BOOST_MIRRORED_TYPE(foo*) ::full_name() << endl;
	bcout << "|36| " << BOOST_MIRRORED_TYPE(bar&) ::full_name() << endl;
	bcout << "|37| " << BOOST_MIRRORED_TYPE(foo***) ::full_name() << endl;
	bcout << "|38| " << BOOST_MIRRORED_TYPE(bar**&) ::full_name() << endl;
	bcout << "|39| " << BOOST_MIRRORED_TYPE(const foo**) ::full_name() << endl;
	bcout << "|40| " << BOOST_MIRRORED_TYPE(volatile bar*&) ::full_name() << endl;
	//
	bcout << "|41| " << BOOST_MIRRORED_TYPE(const volatile foo&) ::full_name() << endl;
	bcout << "|42| " << BOOST_MIRRORED_TYPE(const volatile bar*) ::full_name() << endl;
	//
	bcout << "|43| " << BOOST_MIRRORED_TYPE(const volatile ::bar&) ::full_name() << endl;
	bcout << "|44| " << BOOST_MIRRORED_TYPE(const volatile ::baz*) ::full_name() << endl;
	bcout << "|44| " << BOOST_MIRRORED_TYPEDEF_GS(foobar) ::full_name() << endl;
	//
	// multi-dimensional arrays
	typedef ::bar * const * T45 [][1][2][3][4][5][6][7][8][9];
	bcout << "|45| " << BOOST_MIRRORED_TYPE(T45) ::full_name() << endl;
	// 
	// function pointers
	bcout << "|46| " << BOOST_MIRRORED_TYPEOF(&test::zero) ::full_name() << endl;
	//
	// more function pointers
	typedef const foo * volatile (*T47)(const ::bar&, volatile ::baz*, T45);
	bcout << "|47| " << BOOST_MIRRORED_TYPE(T47) ::full_name() << endl;
	//
	// arrays of function pointers
	typedef T47 const * T48[2][3];
	bcout << "|48| " << BOOST_MIRRORED_TYPE(T48) ::full_name() << endl;
	//
	// pointer to functions returning pointers to functions
	typedef int(*A49)(double);
	typedef A49 const * (*B49)(char);
	typedef B49 volatile * (*C49)(wchar_t);
	typedef C49 T49;
	bcout << "|49| " << BOOST_MIRRORED_TYPE(T49) ::full_name() << endl;
	//
	// function returning pointer to functions
	typedef C49 const * volatile * (T50)(short);
	bcout << "|50| " << BOOST_MIRRORED_TYPE(T50) ::full_name() << endl;
	//
	// reference to function returning pointer to functions
	typedef T50& T51;
	bcout << "|51| " << BOOST_MIRRORED_TYPE(T51) ::full_name() << endl;
	bcout << "|52| " << BOOST_MIRRORED_TYPE(T51) ::get_name(mpl::false_()) << endl;
	//
	return 0;
}


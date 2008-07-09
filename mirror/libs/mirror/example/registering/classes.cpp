/**
 * \file examples/registering/classes.cpp
 * Example of class registering and reflection with
 * the mirror library.
 *
 * NOTE: if You are not familiar with namespace and type
 * registration and reflection, You should probably 
 * see examples/registering/namespaces.cpp and
 * examples/registering/types.cpp first.
 *
 *  Copyright 2008 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */
#include <assert.h>

#include <boost/mpl/for_each.hpp>
#include <boost/mpl/size.hpp>
#include <boost/mpl/at.hpp>

#include <boost/char_type_switch/iostream.hpp>

#include <boost/mirror/meta_namespace.hpp>
#include <boost/mirror/meta_type.hpp>
#include <boost/mirror/meta_class.hpp>

#include <boost/mirror/algorithms.hpp>

#include <boost/mirror/traits/reflects_global_scope.hpp>
#include <boost/mirror/traits/reflects_namespace.hpp>
#include <boost/mirror/traits/reflects_class.hpp>
#include <boost/mirror/traits/reflects_type.hpp>


/** First declare some namespaces and classes
 */

namespace test {
namespace feature {
namespace detail {

struct foo_base  { };
struct foo_base2 { };
struct foo_base3 { };

// declare a struct with multiple base classes
struct foo : foo_base, public foo_base2, virtual protected foo_base3
{
};

} // namespace detail
} // namespace feature

namespace stuff {
namespace detail {

struct bar_base
{
	// This line allows mirror to access non-public
	// class members
	BOOST_MIRROR_FRIENDLY_CLASS(bar_base)
	int an_int;
	long a_long;
};


class bar : protected bar_base
{
	BOOST_MIRROR_FRIENDLY_CLASS(bar)
	//
	float a_float;
	//
	double a_double;
	//
	// This is a getter and a setter for two 'virtual' attributes.
	// There are no members to store the actual values, 
	// the first member attrib is read-only
	// and the second is write-only
	int get_ro_val(void) const {return 1; /* or generate value */}
	//
	void set_wo_val(int a_val){ /* do something with a_val */}
	//
	// This is a member with a setter function 
	// Not too clever, the value can be read directly,
	// but must be stored by the setter function.
	// Anyway, mirror supports this 
	//
	::boost::bstring a_string;
	void set_string(const ::boost::bstring& a_str)
	{
		a_string = a_str; 
		::boost::bcout << a_string << ::std::endl;
	}
	//
	// attribute with a type based on a typedefined type
	const ::boost::bchar* (*a_function)(int);
public:
	// A little weird getter/setter pair 
	// Note that the getter is not const
	bool get_bool(void) {return a_string.empty();}
	void set_bool(bool b){if(!b) a_string.clear();}
	//
	// An 'embedded' class 
	struct bar_part 
	{
		char a_char;
		bool a_bool;
	};
	//
	// a static member attribute
	static short a_short;

	// a mutable member attribute
	mutable wchar_t a_widechar;
};

short bar::a_short = 123;


} // namespace detail
} // namespace stuff

} // namespace test


namespace boost { 
namespace mirror {

/** Register the namespaces
 */
BOOST_MIRROR_REG_NAMESPACE((test))
BOOST_MIRROR_REG_NAMESPACE((test)(feature))
BOOST_MIRROR_REG_NAMESPACE((test)(feature)(detail))
BOOST_MIRROR_REG_NAMESPACE((test)(stuff))
BOOST_MIRROR_REG_NAMESPACE((test)(stuff)(detail))
//
/** Register the types
 */
BOOST_MIRROR_REG_TYPE(::test::feature::detail, foo_base)
BOOST_MIRROR_REG_TYPE(::test::feature::detail, foo_base2)
BOOST_MIRROR_REG_TYPE(::test::feature::detail, foo_base3)
BOOST_MIRROR_REG_TYPE(::test::feature::detail, foo)
BOOST_MIRROR_REG_TYPE(::test::stuff::detail, bar_base)
BOOST_MIRROR_REG_TYPE(::test::stuff::detail, bar)
// register the embedded type that is declared inside of the bar class
BOOST_MIRROR_REG_TYPE_EMBEDDED(::test::stuff::detail::bar, bar_part)


/** Register the base classes of foo
 */
BOOST_MIRROR_REG_BASE_CLASSES_BEGIN(::test::feature::detail::foo)
	// a simple base (not virtual, nor explicitly public/protected/private)
	BOOST_MIRROR_REG_SIMPLE_BASE_CLASS(0,  ::test::feature::detail::foo_base)
	// a public non-virtual base
	BOOST_MIRROR_REG_BASE_CLASS(1, public, ::test::feature::detail::foo_base2)
	// a virtual protected base
	BOOST_MIRROR_REG_VIRTUAL_BASE_CLASS(2,  protected, ::test::feature::detail::foo_base3)
BOOST_MIRROR_REG_BASE_CLASSES_END

// since bar has only one base we can register it this way more quickly
BOOST_MIRROR_REG_SINGLE_BASE_CLASS(
	::test::stuff::detail::bar, 
	public, ::test::stuff::detail::bar_base
)

/** Class attributes
 */
// register the attributes of bar_base
BOOST_MIRROR_REG_CLASS_ATTRIBS_BEGIN(::test::stuff::detail::bar_base)	
	BOOST_MIRROR_REG_SIMPLE_CLASS_ATTRIB(_, int, an_int)
	BOOST_MIRROR_REG_SIMPLE_CLASS_ATTRIB(_, long, a_long)
BOOST_MIRROR_REG_CLASS_ATTRIBS_END

// register the attributes of bar, this is little more tricky
BOOST_MIRROR_REG_CLASS_ATTRIBS_BEGIN(::test::stuff::detail::bar)	
	// these two are simple
	BOOST_MIRROR_REG_SIMPLE_CLASS_ATTRIB(_, float, a_float)
	BOOST_MIRROR_REG_SIMPLE_CLASS_ATTRIB(_, double, a_double)
	//
	// this 'virtual' attribute has only a getter (cannot be set)
	BOOST_MIRROR_REG_CLASS_ATTRIB(
		_, int, a_ro_val, 
		{return instance.get_ro_val();},
		{dest = DestType(instance.get_ro_val()); return dest;},
		{ }
	)
	// this 'virtual' attribute has only a setter (cannot be queried)
	BOOST_MIRROR_REG_CLASS_ATTRIB(
		_, int, a_wo_val, 
		{return 0;},
		{return dest;},
		{instance.set_wo_val(value);}
	)
	//
	// this is an attrib that has no getter but has a setter function
	// NOTE that the type of this attribute is typedef'd 
	BOOST_MIRROR_REG_CLASS_ATTRIB(
		_, BOOST_MIRROR_TYPEDEF(::boost, bstring), a_string, 
		{return instance.a_string;},
		{dest = DestType(instance.a_string); return dest;},
		{instance.set_string(value);}
	)
	// another typedefd attribute
	BOOST_MIRROR_REG_CLASS_ATTRIB(
		static, const BOOST_MIRROR_TYPEDEF(::boost, bchar) * (*)(int), a_function, 
		{return instance.a_function;},
		{dest = DestType(instance.a_function); return dest;},
		{ } // no setter
	)
	//
	// and the last one is accessed by the means of a pair of getter/setter functions
	BOOST_MIRROR_REG_CLASS_ATTRIB(
		_, bool, a_bool, 
		{return const_cast<Class&>(instance).get_bool();},
		{dest = DestType(const_cast<Class&>(instance).get_bool()); return dest;},
		{instance.set_bool(value);}
	)
	//
	// register a static member attribute
	BOOST_MIRROR_REG_SIMPLE_CLASS_ATTRIB(static, short, a_short)
	// register a mutable member attribute
	BOOST_MIRROR_REG_SIMPLE_CLASS_ATTRIB(mutable, wchar_t, a_widechar)
BOOST_MIRROR_REG_CLASS_ATTRIBS_END

/** This is a pretty printer template that prints
 *  some info about the given meta_object into 
 *  a stream.
 */
template <class MetaObject>
struct pretty_printer
{
	typedef MetaObject meta_object;
	//
	// prints some info about the meta-object 
	// to the given stream
	template <class OutStream>
	OutStream& print(OutStream& s) const
	{
		typedef pretty_printer<meta_object> prn_type;
		typedef typename prn_type::meta_object meta_object;
		//
		using namespace ::std;
		//
		// check the kind of meta object
		// and print the kind of the object it reflects
		if(reflects_class<meta_object>::value)
			s << "The class '";
		else if(reflects_type<meta_object>::value)
			s << "The type '";
		else return s << "Unknown meta object";
		//
		// print out it's name
		s << meta_object::full_name() << "' " << endl;
		//
		//
		typedef typename meta_object::scope meta_scope;
		// print info about the scope where the meta-object
		// is defined
		s << "is defined ";
		// if the scope is the global scope
		if(reflects_global_scope<meta_scope>::value)
			s << "on the global scope";
		// if it's a class
		else if(reflects_class<meta_scope>::value)
			s << "inside of the '" << meta_scope::full_name() << "' class";
		// otherwise 
		else s << "in the '" << meta_scope::full_name() << "' namespace";
		s << "."  << endl;
		//
		// print the base classes if any
		print_base_classes(s, (meta_object*)0);
		// print the attributes if any
		print_attribs(s, (meta_object*)0);
		//
		return s;
	}
	//
	// does not print anything for other meta-objects
	// than meta_classes
	template <class OutStream, class SomeMetaObject>
	OutStream& print_base_classes(OutStream& s, SomeMetaObject*) const {return s;}
	//
	// a function object that gets called by mirror::for_each
	// for every base class of the given class 
	template <class OutStream>
	struct base_class_printer
	{
		OutStream& s;
		base_class_printer(OutStream& _s):s(_s){ }
		//
		template <class MetaInheritance>
		void operator()(MetaInheritance) const 
		{
			// the argument is a specialization of the
			// meta_inheritance<> template 
			// it's base_class typedef is the type 
			// of the base class
			using namespace ::std;
			typedef typename MetaInheritance::meta_base_class meta_class;
			s << endl << " - " << meta_class::full_name();
		}
	};
	//
	// prints info about base classes
	template <class OutStream, class Class>
	OutStream& print_base_classes(OutStream& s, meta_class<Class>*) const 
	{
		using namespace ::std;
		// 
		// print out the count of (registered) base classes that 
		// the inspected class has
		//
		typedef typename MetaObject::base_classes::list base_class_list;
		//
		s << "It has ";
		if(mpl::size<base_class_list>::value == 1)
			s << "this one base class:";
		else if(mpl::size<base_class_list>::value > 1)
			s << "these " << mpl::size<base_class_list>::value << " base classes:";
		else s << "no base classes.";
		//
		// execute the printer on the list of base classes
		for_each<typename meta_object::base_classes>(base_class_printer<OutStream>(s));
		//
		return s << endl;
	}
	//
	// does not print anything for non meta_class meta-objects
	template <class OutStream, class SomeMetaObject>
	OutStream& print_attribs(OutStream& s, SomeMetaObject*) const {return s;}
	//
	//
	// a functor that gets called for every meta_attribute 
	// of the given meta_class
	template <class OutStream>
	struct attrib_printer
	{
		OutStream& s;
		// the constructor takes reference to the output stream
		attrib_printer(OutStream& _s):s(_s){ }
		//
		// function call operator
		template <class MetaAttribute>
		void operator()(MetaAttribute ma) const 
		{
			using namespace ::std;
			s << endl << " - " << 
				BOOST_MIRRORED_TYPE(typename MetaAttribute::type_selector)::full_name() <<
				"        " <<
				ma.base_name();
		}
	};
	//
	// this overload prints the list of attributes of the given class
	template <class OutStream, class Class>
	OutStream& print_attribs(OutStream& s, meta_class<Class>*) const 
	{
		//
		// print the number of the registered attributes
		//
		typedef typename MetaObject::attributes::type_list attrib_type_list;
		//
		s << "It has ";
		if(mpl::size<attrib_type_list>::value == 1)
			s << "this one member attribute:";
		else if(mpl::size<attrib_type_list>::value > 1)
			s << "these " << mpl::size<attrib_type_list>::value << " member attributes:";
		else s << "no member attributes.";
		//
		// execute the printer on the list of member attributes
		// note that the type of functor and the implementation
		// of for_each is likely subject to changes
		for_each<typename MetaObject::attributes>(attrib_printer<OutStream>(s));
		return s << ::std::endl;
	}

};

// now some easy stuff, overload the << operator 
// for our pretty_printer
template <class MetaObject, class OutStream>
OutStream& operator << (OutStream& s, const pretty_printer<MetaObject>& prn)
{
	return prn.print(s) << ::std::endl << "----------------------";
}

} // namespace mirror
} // namespace boost





int main(void)
{
	using namespace ::std;
	using namespace ::boost;
	using namespace ::boost::mirror;
	//
	using ::test::feature::detail::foo;
	using ::test::stuff::detail::bar;
	using ::test::stuff::detail::bar_base;
	//
	// use the pretty printer to print out  into about the given types
	//
	// pointer to native type
	bcout << "|01| " << endl << pretty_printer<BOOST_MIRRORED_TYPE(double*)>() << endl;
	// a class defined in a namespace
	bcout << "|02| " << endl << pretty_printer<BOOST_MIRRORED_CLASS(foo)>() << endl;
	bcout << "|03| " << endl << pretty_printer<BOOST_MIRRORED_CLASS(bar)>() << endl;
	// an embedded class 
	bcout << "|04| " << endl << pretty_printer<BOOST_MIRRORED_CLASS(bar::bar_part)>() << endl;
	// typedef'd type
	bcout << "|05| " << endl << pretty_printer<BOOST_MIRRORED_TYPEDEF(::boost, bchar)>() << endl;
	// type of an expression
	bcout << "|06| " << endl << pretty_printer<BOOST_MIRRORED_TYPEOF("foo")>() << endl;
	//
	// full typenames
	//bcout << "|07| " << BOOST_MIRRORED_TYPEOF("foo") ::full_name() << endl;
	//
	bar_base x = {123, 456L};
	bar_base y = {234, 567L};
	//
	// the meta-attributes can be used to examine 
	// values of the members too
	assert(BOOST_MIRRORED_CLASS(BOOST_TYPEOF(x))::attributes::get(x, mpl::int_<0>()) == x.an_int);
	assert(BOOST_MIRRORED_CLASS(BOOST_TYPEOF(x))::attributes::get(x, mpl::int_<1>()) == x.a_long);
	// and also to set them
	BOOST_MIRRORED_CLASS(BOOST_TYPEOF(x))::attributes::set(
		x, mpl::int_<0>(),
		BOOST_MIRRORED_CLASS(BOOST_TYPEOF(y))::attributes::get(y, mpl::int_<0>())
	);
	BOOST_MIRRORED_CLASS(BOOST_TYPEOF(x))::attributes::set(
		x, mpl::int_<1>(),
		BOOST_MIRRORED_CLASS(BOOST_TYPEOF(y))::attributes::get(y, mpl::int_<1>())
	);
	//
	assert(x.an_int == y.an_int);
	assert(x.a_long == y.a_long);
	//
	bcout << "Finished" << endl;
	//
	return 0;
}


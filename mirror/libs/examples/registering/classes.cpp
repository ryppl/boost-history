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

#include <boost/mirror/utils/name_to_stream.hpp>

#include <boost/mirror/reflects_global_scope.hpp>
#include <boost/mirror/reflects_namespace.hpp>
#include <boost/mirror/reflects_class.hpp>
#include <boost/mirror/reflects_type.hpp>


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
};

} // namespace detail
} // namespace stuff

} // namespace test


namespace boost { 
namespace mirror {

/** Register the namespaces
 */
BOOST_MIRROR_REG_META_NAMESPACE_TOP_LEVEL(test)
BOOST_MIRROR_REG_META_NAMESPACE(_test, feature)
BOOST_MIRROR_REG_META_NAMESPACE(_test_feature, detail)
BOOST_MIRROR_REG_META_NAMESPACE(_test, stuff)
BOOST_MIRROR_REG_META_NAMESPACE(_test_stuff, detail)
//
/** Register the types
 */
BOOST_MIRROR_REG_META_TYPE(_test_feature_detail, ::test::feature::detail, foo_base)
BOOST_MIRROR_REG_META_TYPE(_test_feature_detail, ::test::feature::detail, foo_base2)
BOOST_MIRROR_REG_META_TYPE(_test_feature_detail, ::test::feature::detail, foo_base3)
BOOST_MIRROR_REG_META_TYPE(_test_feature_detail, ::test::feature::detail, foo)
BOOST_MIRROR_REG_META_TYPE(_test_stuff_detail,   ::test::stuff::detail, bar_base)
BOOST_MIRROR_REG_META_TYPE(_test_stuff_detail,   ::test::stuff::detail, bar)
// register the embedded type that is declared inside of the bar class
BOOST_MIRROR_REG_META_TYPE_EMBEDDED(::test::stuff::detail::bar, bar_part)


/** Register the base classes of foo
 */
BOOST_MIRROR_REG_BASE_CLASSES_BEGIN(::test::feature::detail::foo)
	// a simple base (not virtual, nor explicitly public/protected/private)
	BOOST_MIRROR_REG_BASE_CLASS_SIMPLE(0,  ::test::feature::detail::foo_base)
	// a public non-virtual base
	BOOST_MIRROR_REG_BASE_CLASS(1, public, ::test::feature::detail::foo_base2)
	// a virtual protected base
	BOOST_MIRROR_REG_BASE_CLASS_VIRTUAL(2,  protected, ::test::feature::detail::foo_base3)
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
	BOOST_MIRROR_REG_CLASS_ATTRIB(0, int, an_int)
	BOOST_MIRROR_REG_CLASS_ATTRIB(1, long, a_long)
BOOST_MIRROR_REG_CLASS_ATTRIBS_END

// register the attributes of bar, this is little more tricky
BOOST_MIRROR_REG_CLASS_ATTRIBS_BEGIN(::test::stuff::detail::bar)	
	// these two are simple
	BOOST_MIRROR_REG_CLASS_ATTRIB(0, float, a_float)
	BOOST_MIRROR_REG_CLASS_ATTRIB(1, double, a_double)
	//
	// this 'virtual' attribute has only a getter (cannot be set)
	BOOST_MIRROR_REG_CLASS_ATTRIB_GETTER_ONLY(2, int, a_ro_val, get_ro_val)
	// this 'virtual' attribute has only a setter (cannot be queried)
	BOOST_MIRROR_REG_CLASS_ATTRIB_SETTER_ONLY(3, int, a_wo_val, set_wo_val)
	//
	// this is an attrib that has no getter but has a setter function
	// Also note that macros with the _TD suffix allow to register 
	// attributes with typedef'd types
	BOOST_MIRROR_REG_CLASS_ATTRIB_SETTER_TD(4, _boost, ::boost, bstring, a_string, set_string)
	//
	// and the last one is accessed by the means of a pair of getter/setter functions
	BOOST_MIRROR_REG_CLASS_ATTRIB_GETTER_SETTER(5, bool, a_bool, get_bool, set_bool)
BOOST_MIRROR_REG_CLASS_ATTRIBS_END

/** This is a pretty printer template that prints
 *  some info about the given meta_object into 
 *  a stream.
 */
template <class meta_object>
struct pretty_printer
{
	typedef meta_object meta_object;
	//
	// prints some info about the meta-object t
	// to the given stream
	template <class out_stream>
	out_stream& print(out_stream& s) const
	{
		typedef pretty_printer<meta_object> prn_type;
		typedef prn_type::meta_object meta_object;
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
		s << name_to_stream<meta_object>() << "' " << endl;
		//
		// print info about the scope where the meta-object
		// is defined
		s << "is defined ";
		// if the scope is the global scope
		if(reflects_global_scope<meta_object::scope>::value)
			s << "on the global scope";
		// if it's a class
		else if(reflects_class<meta_object::scope>::value)
			s << "inside of the '" << name_to_stream<meta_object::scope>() << "' class";
		// otherwise 
		else s << "in the '" << name_to_stream<meta_object::scope>() << "' namespace";
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
	template <class out_stream, class meta_object>
	out_stream& print_base_classes(out_stream& s, meta_object*) const {return s;}
	//
	// a function object that gets called by mpl::for_each
	// for every base class of the given class 
	template <class out_stream>
	struct base_class_printer
	{
		out_stream& s;
		base_class_printer(out_stream& _s):s(_s){ }
		//
		template <class meta_inheritance>
		void operator()(meta_inheritance) const 
		{
			// the argument is a specialization of the
			// meta_inheritance<> template 
			// it's base_class typedef is the type 
			// of the base class
			using namespace ::std;
			s << endl << " - " << 
				name_to_stream<BOOST_MIRROR_REFLECT_CLASS(meta_inheritance::base_class)>();
		}
	};
	//
	// prints info about base classes
	template <class out_stream, class base_class>
	out_stream& print_base_classes(out_stream& s, meta_class<base_class>*) const 
	{
		using namespace ::std;
		// 
		// print out the count of (registered) base classes that 
		// the inspected class has
		s << "It has ";
		if(mpl::size<meta_object::base_classes::list>::value == 1)
			s << "this one base class:";
		else if(mpl::size<meta_object::base_classes::list>::value > 1)
			s << "these " << mpl::size<meta_object::base_classes::list>::value << " base classes:";
		else s << "no base classes.";
		//
		// execute the printer on the list of base classes
		mpl::for_each<meta_object::base_classes::list>(base_class_printer<out_stream>(s));
		//
		return s << endl;
	}
	//
	// does not print anything for non meta_class meta-objects
	template <class out_stream, class meta_object>
	out_stream& print_attribs(out_stream& s, meta_object*) const {return s;}
	//
	//
	// a functor that gets called for every meta_attribute 
	// of the given meta_class
	template <class out_stream>
	struct attrib_printer
	{
		out_stream& s;
		// the constructor takes reference to the output stream
		attrib_printer(out_stream& _s):s(_s){ }
		//
		// function call operator
		template <class meta_class, class iterator, class attrib_type>
		void operator()(meta_class*, iterator pos, attrib_type*) const 
		{
			// the first argument is a undefined pointer to the 
			// meta_class which we are inspecting (it only conveys 
			// the information about the meta_class<> specialization
			//
			// the second argument is the position that can be used 
			// to access base_name of the i-th attribute. Basically 
			// this is a mpl::int_<>
			//
			// the third attribute is an undefined pointer used to 
			// pass the info about the i-th member attrib type
			//
			// NOTE: the pointers may be replaced by mpl::identity<>
			// or some other type in the future
			using namespace ::std;
			s << endl << " - " << 
				name_to_stream<BOOST_MIRROR_REFLECT_TYPE(attrib_type)>() <<
				"        " <<
				meta_class::attributes::base_name(pos);
		}
	};
	//
	// this overload prints the list of attributes of the given class
	template <class out_stream, class base_class>
	out_stream& print_attribs(out_stream& s, meta_class<base_class>*) const 
	{
		//
		// print the number of the registered attributes
		s << "It has ";
		if(mpl::size<meta_object::attributes::type_list>::value == 1)
			s << "this one member attribute:";
		else if(mpl::size<meta_object::attributes::type_list>::value > 1)
			s << "these " << mpl::size<meta_object::attributes::type_list>::value << " member attributes:";
		else s << "no member attributes.";
		//
		// execute the printer on the list of member attributes
		// note that the type of functor and the implementation
		// of for_each is likely subject to changes
		meta_object::attributes::for_each(attrib_printer<out_stream>(s));
		return s << ::std::endl;
	}

};

// now some easy stuff, overload the << operator 
// for our pretty_printer
template <class meta_object, class out_stream>
out_stream& operator << (out_stream& s, const pretty_printer<meta_object>& prn)
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
	bcout << "|01| " << endl << pretty_printer<BOOST_MIRROR_REFLECT_TYPE(double*)>() << endl;
	// a class defined in a namespace
	bcout << "|02| " << endl << pretty_printer<BOOST_MIRROR_REFLECT_CLASS(foo)>() << endl;
	bcout << "|03| " << endl << pretty_printer<BOOST_MIRROR_REFLECT_CLASS(bar)>() << endl;
	// an embedded class 
	bcout << "|04| " << endl << pretty_printer<BOOST_MIRROR_REFLECT_CLASS(bar::bar_part)>() << endl;
	// typedef'd type
	bcout << "|05| " << endl << pretty_printer<BOOST_MIRROR_REFLECT_TYPEDEFD(_boost, bchar)>() << endl;
	// type of an expression
	bcout << "|06| " << endl << pretty_printer<BOOST_MIRROR_REFLECT_TYPEOF("foo")>() << endl;
	//
	//
	bar_base x = {123, 456L};
	bar_base y = {234, 567L};
	//
	// the meta-attributes can be used to examine 
	// values of the members too
	assert(BOOST_MIRROR_REFLECT_CLASS(BOOST_TYPEOF(x))::attributes::get(x, mpl::int_<0>()) == x.an_int);
	assert(BOOST_MIRROR_REFLECT_CLASS(BOOST_TYPEOF(x))::attributes::get(x, mpl::int_<1>()) == x.a_long);
	// and also to set them
	BOOST_MIRROR_REFLECT_CLASS(BOOST_TYPEOF(x))::attributes::set(
		x, mpl::int_<0>(),
		BOOST_MIRROR_REFLECT_CLASS(BOOST_TYPEOF(y))::attributes::get(y, mpl::int_<0>())
	);
	BOOST_MIRROR_REFLECT_CLASS(BOOST_TYPEOF(x))::attributes::set(
		x, mpl::int_<1>(),
		BOOST_MIRROR_REFLECT_CLASS(BOOST_TYPEOF(y))::attributes::get(y, mpl::int_<1>())
	);
	//
	assert(x.an_int == y.an_int);
	assert(x.a_long == y.a_long);
	//
	bcout << "Finished" << endl;
	//
	return 0;
}


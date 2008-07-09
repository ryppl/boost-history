/**
 * \file examples/registering/virtual_bases.cpp
 * Example of registering virtual base classes
 * and using the meta_class<>::all_attributes
 *
 * NOTE: if You are not familiar with namespace and type
 * registration and reflection, You should probably 
 * see examples/registering/namespaces.cpp,
 * examples/registering/classes.cpp and
 * examples/registering/types.cpp first.
 *
 *  Copyright 2008 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */
#include <assert.h>
#include <math.h>

#include <boost/mpl/list.hpp>
#include <boost/mpl/for_each.hpp>
#include <boost/mpl/size.hpp>
#include <boost/mpl/at.hpp>

#include <boost/static_assert.hpp>

#include <boost/char_type_switch/iostream.hpp>

#include <boost/mirror/meta_namespace.hpp>
#include <boost/mirror/meta_type.hpp>
#include <boost/mirror/meta_class.hpp>

#include <boost/mirror/algorithms.hpp>

/** First declare some namespaces and classes
 */

namespace Test {

	struct A 
	{
		long l;
	};

	struct B : virtual A
	{
		int i;
	};

	struct C : virtual A
	{
		double d;
	};

	struct D : virtual A
	{
		short s;
	};

	struct E : B, C, D
	{
		float f;
	};

	struct F : virtual E
	{
		bool b;
	};

	struct G : virtual E
	{
		char c;
	};

	struct H : F, G
	{
		wchar_t w;
	};


} // namespace Test



namespace boost { 
namespace mirror {

/** Register the Test namespace
 */
BOOST_MIRROR_REG_NAMESPACE((Test))

/** Register the types and classes
 */
BOOST_MIRROR_REG_TYPE(::Test, A)
BOOST_MIRROR_REG_TYPE(::Test, B)
BOOST_MIRROR_REG_TYPE(::Test, C)
BOOST_MIRROR_REG_TYPE(::Test, D)
BOOST_MIRROR_REG_TYPE(::Test, E)
BOOST_MIRROR_REG_TYPE(::Test, F)
BOOST_MIRROR_REG_TYPE(::Test, G)
BOOST_MIRROR_REG_TYPE(::Test, H)


BOOST_MIRROR_REG_BASE_CLASSES_BEGIN(::Test::B)
BOOST_MIRROR_REG_VIRTUAL_BASE_CLASS(0,  public, ::Test::A)
BOOST_MIRROR_REG_BASE_CLASSES_END

BOOST_MIRROR_REG_BASE_CLASSES_BEGIN(::Test::C)
BOOST_MIRROR_REG_VIRTUAL_BASE_CLASS(0,  public, ::Test::A)
BOOST_MIRROR_REG_BASE_CLASSES_END

BOOST_MIRROR_REG_BASE_CLASSES_BEGIN(::Test::D)
BOOST_MIRROR_REG_VIRTUAL_BASE_CLASS(0,  public, ::Test::A)
BOOST_MIRROR_REG_BASE_CLASSES_END

BOOST_MIRROR_REG_BASE_CLASSES_BEGIN(::Test::E)
BOOST_MIRROR_REG_SIMPLE_BASE_CLASS(0,  ::Test::B)
BOOST_MIRROR_REG_SIMPLE_BASE_CLASS(1,  ::Test::C)
BOOST_MIRROR_REG_SIMPLE_BASE_CLASS(2,  ::Test::D)
BOOST_MIRROR_REG_BASE_CLASSES_END

BOOST_MIRROR_REG_BASE_CLASSES_BEGIN(::Test::F)
BOOST_MIRROR_REG_VIRTUAL_BASE_CLASS(0,  public, ::Test::E)
BOOST_MIRROR_REG_BASE_CLASSES_END

BOOST_MIRROR_REG_BASE_CLASSES_BEGIN(::Test::G)
BOOST_MIRROR_REG_VIRTUAL_BASE_CLASS(0,  public, ::Test::E)
BOOST_MIRROR_REG_BASE_CLASSES_END

BOOST_MIRROR_REG_BASE_CLASSES_BEGIN(::Test::H)
BOOST_MIRROR_REG_SIMPLE_BASE_CLASS(0,  ::Test::F)
BOOST_MIRROR_REG_SIMPLE_BASE_CLASS(1,  ::Test::G)
BOOST_MIRROR_REG_BASE_CLASSES_END


/** Class attributes
 */
BOOST_MIRROR_REG_CLASS_ATTRIBS_BEGIN(::Test::A)	
	BOOST_MIRROR_REG_SIMPLE_CLASS_ATTRIB(_, long, l)
BOOST_MIRROR_REG_CLASS_ATTRIBS_END

BOOST_MIRROR_REG_CLASS_ATTRIBS_BEGIN(::Test::B)	
	BOOST_MIRROR_REG_SIMPLE_CLASS_ATTRIB(_, int, i)
BOOST_MIRROR_REG_CLASS_ATTRIBS_END

BOOST_MIRROR_REG_CLASS_ATTRIBS_BEGIN(::Test::C)	
	BOOST_MIRROR_REG_SIMPLE_CLASS_ATTRIB(_, double, d)
BOOST_MIRROR_REG_CLASS_ATTRIBS_END

BOOST_MIRROR_REG_CLASS_ATTRIBS_BEGIN(::Test::D)	
	BOOST_MIRROR_REG_SIMPLE_CLASS_ATTRIB(_, short, s)
BOOST_MIRROR_REG_CLASS_ATTRIBS_END

BOOST_MIRROR_REG_CLASS_ATTRIBS_BEGIN(::Test::E)	
	BOOST_MIRROR_REG_SIMPLE_CLASS_ATTRIB(_, float, f)
BOOST_MIRROR_REG_CLASS_ATTRIBS_END

BOOST_MIRROR_REG_CLASS_ATTRIBS_BEGIN(::Test::F)	
	BOOST_MIRROR_REG_SIMPLE_CLASS_ATTRIB(_, bool, b)
BOOST_MIRROR_REG_CLASS_ATTRIBS_END

BOOST_MIRROR_REG_CLASS_ATTRIBS_BEGIN(::Test::G)	
	BOOST_MIRROR_REG_SIMPLE_CLASS_ATTRIB(_, char, c)
BOOST_MIRROR_REG_CLASS_ATTRIBS_END

BOOST_MIRROR_REG_CLASS_ATTRIBS_BEGIN(::Test::H)	
	BOOST_MIRROR_REG_SIMPLE_CLASS_ATTRIB(_, wchar_t, w)
BOOST_MIRROR_REG_CLASS_ATTRIBS_END



} // namespace mirror
} // namespace boost

struct attrib_printer
{
		template <class MetaAttribute>
		void operator()(MetaAttribute ma) const
		{
			using namespace ::std;
			using namespace ::boost;
			using namespace ::boost::mirror;
			typedef BOOST_MIRRORED_TYPE(typename MetaAttribute::type) mt;
			bcout << 
				"    " << 
				MetaAttribute::position::value << 
				": " << 
				MetaAttribute::meta_class::full_name() << 
				" " << 
				ma.base_name() << 
				";" << 
				endl;
		}
};

int main(void)
{
	using namespace ::std;
	using namespace ::boost;
	using namespace ::boost::mirror;
	//
	using namespace ::Test;
	//
	typedef ::Test::H T;
	//
	typedef BOOST_MIRRORED_CLASS(T) meta_T;
	//
	// Print some basic info about the reflected class
	//
	bcout << "The reflected class has " << endl;
	//
	bcout << meta_T::base_classes::size::value << " base class(es)" << endl;
	bcout << meta_T::attributes::size::value << " own member attrib(s)" << endl;
	bcout << meta_T::all_attributes::inherited_size::value << " inherited member attrib(s)" << endl;
	bcout << meta_T::all_attributes::size::value << " member attrib(s)" << endl;
	// execute a functor on all attributes
	for_each<meta_T::all_attributes>(attrib_printer());
	//
	// The attrbs of H are reflected in the following order
	// A::l (long)
	// B::i (int)
	// C::d (double)
	// D::s (short)
	// E::f (float)
	// F::b (bool)
	// G::c (char)
	// H::w (wchar_t)
	//
	BOOST_STATIC_ASSERT((is_same<
		mpl::at<meta_T::all_attributes::type_list, mpl::int_<0> >::type, 
		BOOST_TYPEOF(T().l)
	>::value));
	BOOST_STATIC_ASSERT((is_same<
		mpl::at<meta_T::all_attributes::type_list, mpl::int_<1> >::type, 
		BOOST_TYPEOF(T().i)
	>::value));
	BOOST_STATIC_ASSERT((is_same<
		mpl::at<meta_T::all_attributes::type_list, mpl::int_<2> >::type, 
		BOOST_TYPEOF(T().d)
	>::value));
	BOOST_STATIC_ASSERT((is_same<
		mpl::at<meta_T::all_attributes::type_list, mpl::int_<3> >::type, 
		BOOST_TYPEOF(T().s)
	>::value));
	BOOST_STATIC_ASSERT((is_same<
		mpl::at<meta_T::all_attributes::type_list, mpl::int_<4> >::type, 
		BOOST_TYPEOF(T().f)
	>::value));
	BOOST_STATIC_ASSERT((is_same<
		mpl::at<meta_T::all_attributes::type_list, mpl::int_<5> >::type, 
		BOOST_TYPEOF(T().b)
	>::value));
	BOOST_STATIC_ASSERT((is_same<
		mpl::at<meta_T::all_attributes::type_list, mpl::int_<6> >::type, 
		BOOST_TYPEOF(T().c)
	>::value));
	BOOST_STATIC_ASSERT((is_same<
		mpl::at<meta_T::all_attributes::type_list, mpl::int_<7> >::type, 
		BOOST_TYPEOF(T().w)
	>::value));
	//
	BOOST_STATIC_ASSERT((is_same<
		mpl::at<meta_T::attributes::type_list, mpl::int_<0> >::type, 
		BOOST_TYPEOF(T().w)
	>::value));
	//
	//
	//
	T t;
	// init the members of t
	t.l = 123L;
	t.i = 234;
	t.d = 345.6;
	t.s = 567;
	t.f = 678.9f;
	t.b = true;
	t.c = '9';
	t.w = L'0';
	//
	// the values of all attributes can be accessed via get() ... 
	//
	assert(t.l == meta_T::all_attributes::get(t, mpl::int_<0>()));
	assert(t.i == meta_T::all_attributes::get(t, mpl::int_<1>()));
	assert(t.d == meta_T::all_attributes::get(t, mpl::int_<2>()));
	assert(t.s == meta_T::all_attributes::get(t, mpl::int_<3>()));
	assert(t.f == meta_T::all_attributes::get(t, mpl::int_<4>()));
	assert(t.b == meta_T::all_attributes::get(t, mpl::int_<5>()));
	assert(t.c == meta_T::all_attributes::get(t, mpl::int_<6>()));
	assert(t.w == meta_T::all_attributes::get(t, mpl::int_<7>()));
	//
	// ... they can be queried using query()  ...
	//
	int dest_for_f = 0;
	assert(int(t.f) == meta_T::all_attributes::query(t, mpl::int_<4>(), dest_for_f));
	wchar_t dest_for_w = L'1';
	assert(t.w == meta_T::all_attributes::query(t, mpl::int_<7>(), dest_for_w));
	//
	// ... and set 
	//
	meta_T::all_attributes::set(t, mpl::int_<0>(), 2345L);
	assert(t.l == 2345L);
	meta_T::all_attributes::set(t, mpl::int_<2>(), 456.7);
	assert(t.d == 456.7);
	meta_T::all_attributes::set(t, mpl::int_<4>(), 789.0f);
	assert(t.f == 789.0f);
	meta_T::all_attributes::set(t, mpl::int_<5>(), false);
	assert(t.b == false);
	meta_T::all_attributes::set(t, mpl::int_<7>(), L'A');
	assert(t.w == L'A');
	//
	// alternate means of getting the values of the attributes
	//
	assert((
		at<meta_T::all_attributes, mpl::int_<0> >::type::get(t) == 
		meta_T::all_attributes::get(t, mpl::int_<0>())
	));
	assert((
		at<meta_T::all_attributes, mpl::int_<1> >::type::get(t) == 
		meta_T::all_attributes::get(t, mpl::int_<1>())
	));
	assert((
		at<meta_T::all_attributes, mpl::int_<2> >::type::get(t) == 
		meta_T::all_attributes::get(t, mpl::int_<2>())
	));
	assert((
		at<meta_T::all_attributes, mpl::int_<3> >::type::get(t) == 
		meta_T::all_attributes::get(t, mpl::int_<3>())
	));
	assert((
		at<meta_T::all_attributes, mpl::int_<4> >::type::get(t) == 
		meta_T::all_attributes::get(t, mpl::int_<4>())
	));
	assert((
		at<meta_T::all_attributes, mpl::int_<5> >::type::get(t) == 
		meta_T::all_attributes::get(t, mpl::int_<5>())
	));
	assert((
		at<meta_T::all_attributes, mpl::int_<6> >::type::get(t) == 
		meta_T::all_attributes::get(t, mpl::int_<6>())
	));
	assert((
		at<meta_T::all_attributes, mpl::int_<7> >::type::get(t) == 
		meta_T::all_attributes::get(t, mpl::int_<7>())
	));
	bcout << "--------------------------------------------" << endl;
	//
	return 0;
}


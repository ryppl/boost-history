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

#include <boost/char_type_switch/iostream.hpp>

#include <boost/mirror/meta_namespace.hpp>
#include <boost/mirror/meta_type.hpp>
#include <boost/mirror/meta_class.hpp>

#include <boost/mirror/utils/name_to_stream.hpp>


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

} // namespace Test



namespace boost { 
namespace mirror {

/** Register the 3D namespace
 */
BOOST_MIRROR_REG_META_NAMESPACE_TOP_LEVEL(Test)

/** Register the types and classes
 */
BOOST_MIRROR_REG_META_TYPE(_Test, ::Test, A)
BOOST_MIRROR_REG_META_TYPE(_Test, ::Test, B)
BOOST_MIRROR_REG_META_TYPE(_Test, ::Test, C)
BOOST_MIRROR_REG_META_TYPE(_Test, ::Test, D)
BOOST_MIRROR_REG_META_TYPE(_Test, ::Test, E)


BOOST_MIRROR_REG_BASE_CLASSES_BEGIN(::Test::B)
BOOST_MIRROR_REG_BASE_CLASS_VIRTUAL(0,  public, ::Test::A)
BOOST_MIRROR_REG_BASE_CLASSES_END

BOOST_MIRROR_REG_BASE_CLASSES_BEGIN(::Test::C)
BOOST_MIRROR_REG_BASE_CLASS_VIRTUAL(0,  public, ::Test::A)
BOOST_MIRROR_REG_BASE_CLASSES_END

BOOST_MIRROR_REG_BASE_CLASSES_BEGIN(::Test::D)
BOOST_MIRROR_REG_BASE_CLASS_VIRTUAL(0,  public, ::Test::A)
BOOST_MIRROR_REG_BASE_CLASSES_END

BOOST_MIRROR_REG_BASE_CLASSES_BEGIN(::Test::E)
BOOST_MIRROR_REG_BASE_CLASS_SIMPLE(0,  ::Test::B)
BOOST_MIRROR_REG_BASE_CLASS_SIMPLE(1,  ::Test::C)
BOOST_MIRROR_REG_BASE_CLASS_SIMPLE(2,  ::Test::D)
BOOST_MIRROR_REG_BASE_CLASSES_END


/** Class attributes
 */
BOOST_MIRROR_REG_CLASS_ATTRIBS_BEGIN(::Test::A)	
	BOOST_MIRROR_REG_CLASS_ATTRIB(0, long, l)
BOOST_MIRROR_REG_CLASS_ATTRIBS_END

BOOST_MIRROR_REG_CLASS_ATTRIBS_BEGIN(::Test::B)	
	BOOST_MIRROR_REG_CLASS_ATTRIB(0, int, i)
BOOST_MIRROR_REG_CLASS_ATTRIBS_END

BOOST_MIRROR_REG_CLASS_ATTRIBS_BEGIN(::Test::C)	
	BOOST_MIRROR_REG_CLASS_ATTRIB(0, double, d)
BOOST_MIRROR_REG_CLASS_ATTRIBS_END

BOOST_MIRROR_REG_CLASS_ATTRIBS_BEGIN(::Test::D)	
	BOOST_MIRROR_REG_CLASS_ATTRIB(0, short, s)
BOOST_MIRROR_REG_CLASS_ATTRIBS_END

BOOST_MIRROR_REG_CLASS_ATTRIBS_BEGIN(::Test::E)	
	BOOST_MIRROR_REG_CLASS_ATTRIB(0, float, f)
BOOST_MIRROR_REG_CLASS_ATTRIBS_END



} // namespace mirror
} // namespace boost

int main(void)
{
	using namespace ::std;
	using namespace ::boost;
	using namespace ::boost::mirror;
	//
	using namespace ::Test;
	//
	typedef BOOST_MIRROR_REFLECT_CLASS(::Test::E) meta_E;
	//
	bcout << sizeof(E().l) << endl;
	//
	bcout << "The class ::Test::E has " << endl;
	//
	bcout << meta_E::base_classes::size::value << " base classes" << endl;
	bcout << meta_E::attributes::size::value << " own member attribs" << endl;
	// NOTE: this isn't working as expected yet
	bcout << meta_E::all_attributes::size::value << " member attribs" << endl;
	//
	//
	bcout << "Finished" << endl;
	//
	return 0;
}


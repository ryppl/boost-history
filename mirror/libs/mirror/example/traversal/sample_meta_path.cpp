/**
 * \file examples/traversal/sample_meta_path.cpp
 *
 * Example of class structure deep and flat traversal
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
#include <boost/mirror/meta_class.hpp>

#include <boost/mirror/meta_types/std/pair.hpp>
#include <boost/mirror/meta_types/boost/tuples/tuple.hpp>
#include <boost/mirror/meta_classes/boost/tuples/tuple.hpp>

#include <boost/mirror/meta_path/ancestors.hpp>
#include <boost/mirror/meta_path/size.hpp>

#include <boost/mirror/visitors/meta_path_sample.hpp>
#include <boost/mirror/traversal.hpp>

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


int main(void)
{
	using namespace ::std;
	using namespace ::boost;
	using namespace ::boost::mirror;
	//
	cts::bostream& bcout = cts::bcout();
	//
	using namespace ::Test;
	//
	typedef BOOST_MIRRORED_CLASS(H) meta_H;
	//
	bcout << "--------------------------------------------" << endl;
	deep_traversal_of<meta_H>::accept(meta_path_sample_visitor());
	bcout << "--------------------------------------------" << endl;
	flat_traversal_of<meta_H>::accept(meta_path_sample_visitor());
	bcout << "--------------------------------------------" << endl;
	//
	//
	return 0;
}

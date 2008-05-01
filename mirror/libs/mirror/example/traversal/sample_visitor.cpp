/**
 * \file examples/traversal/sample_visitor.cpp
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

#include <boost/mirror/meta_types/std_pair.hpp>
#include <boost/mirror/meta_types/boost_tuple.hpp>
#include <boost/mirror/meta_classes/boost_tuple.hpp>


#include <boost/mirror/visitors/sample.hpp>
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
BOOST_MIRROR_REG_META_NAMESPACE_TOP_LEVEL(Test)

/** Register the types and classes
 */
BOOST_MIRROR_REG_META_TYPE(_Test, ::Test, A)
BOOST_MIRROR_REG_META_TYPE(_Test, ::Test, B)
BOOST_MIRROR_REG_META_TYPE(_Test, ::Test, C)
BOOST_MIRROR_REG_META_TYPE(_Test, ::Test, D)
BOOST_MIRROR_REG_META_TYPE(_Test, ::Test, E)
BOOST_MIRROR_REG_META_TYPE(_Test, ::Test, F)
BOOST_MIRROR_REG_META_TYPE(_Test, ::Test, G)
BOOST_MIRROR_REG_META_TYPE(_Test, ::Test, H)


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

BOOST_MIRROR_REG_BASE_CLASSES_BEGIN(::Test::F)
BOOST_MIRROR_REG_BASE_CLASS_VIRTUAL(0,  public, ::Test::E)
BOOST_MIRROR_REG_BASE_CLASSES_END

BOOST_MIRROR_REG_BASE_CLASSES_BEGIN(::Test::G)
BOOST_MIRROR_REG_BASE_CLASS_VIRTUAL(0,  public, ::Test::E)
BOOST_MIRROR_REG_BASE_CLASSES_END

BOOST_MIRROR_REG_BASE_CLASSES_BEGIN(::Test::H)
BOOST_MIRROR_REG_BASE_CLASS_SIMPLE(0,  ::Test::F)
BOOST_MIRROR_REG_BASE_CLASS_SIMPLE(1,  ::Test::G)
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

BOOST_MIRROR_REG_CLASS_ATTRIBS_BEGIN(::Test::F)	
	BOOST_MIRROR_REG_CLASS_ATTRIB(0, bool, b)
BOOST_MIRROR_REG_CLASS_ATTRIBS_END

BOOST_MIRROR_REG_CLASS_ATTRIBS_BEGIN(::Test::G)	
	BOOST_MIRROR_REG_CLASS_ATTRIB(0, char, c)
BOOST_MIRROR_REG_CLASS_ATTRIBS_END

BOOST_MIRROR_REG_CLASS_ATTRIBS_BEGIN(::Test::H)	
	BOOST_MIRROR_REG_CLASS_ATTRIB(0, wchar_t, w)
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
	typedef BOOST_MIRROR_REFLECT_CLASS(H) meta_H;
	//
	bcout << "--------------------------------------------" << endl;
	deep_traversal_of<meta_H>::accept(sample_visitor());
	bcout << "--------------------------------------------" << endl;
	flat_traversal_of<meta_H>::accept(sample_visitor());
	bcout << "--------------------------------------------" << endl;
	//
	//
	typedef pair<int volatile, long const> T1; 
	typedef pair<double * const *, float> T2; 
	typedef pair<char const [12], wchar_t [4]> T3; 
	typedef pair<bool, bool> T4; 
	typedef pair<T1, T2> T5; 
	typedef pair<T3, T4> T6; 
	typedef pair<T5, T6> T7; 
	typedef tuple<T1, T2, T3, T4, T5, T6, T7> T;
	typedef BOOST_MIRROR_REFLECT_CLASS(T) meta_T;
	//
	bcout << "--------------------------------------------" << endl;
	deep_traversal_of<meta_T>::accept(sample_visitor());
	bcout << "--------------------------------------------" << endl;
	flat_traversal_of<meta_T>::accept(sample_visitor());
	bcout << "--------------------------------------------" << endl;
	//
	return 0;
}

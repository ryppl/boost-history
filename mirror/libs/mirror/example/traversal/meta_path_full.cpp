/**
 * \file examples/traversal/meta_path_full.cpp
 *
 * Example of full deep and flat traversal namespace
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

namespace test {
namespace ns_1 {

namespace ns_1_1 {

struct A
{
	int a;
};

} // namespace ns_1_1 

namespace ns_1_2 {

struct B
{
	int b;
};

} // namespace ns_1_2 

namespace ns_1_3 {

struct C
{
	int c;
};

} // namespace ns_1_2 

} // namespace ns_1

namespace ns_2 {

namespace ns_2_1 {

struct D
{
	int d;
};

} // namespace ns_2_1 

namespace ns_2_2 {

struct E
{
	int e;
};

} // namespace ns_2_2 

namespace ns_2_3 {

struct F
{
	int f;
};

} // namespace ns_2_3 

struct G
: ns_1::ns_1_1::A
, ns_1::ns_1_2::B
, ns_1::ns_1_3::C
, ns_2::ns_2_1::D
, ns_2::ns_2_2::E
, ns_2::ns_2_3::F
{
	int g;
};

} // namespace ns_2


} // namespace test

namespace boost { 
namespace mirror {

/** Register the test namespace
 */
BOOST_MIRROR_REG_NAMESPACE((test))
BOOST_MIRROR_REG_NAMESPACE((test)(ns_1))
BOOST_MIRROR_REG_NAMESPACE((test)(ns_1)(ns_1_1))
BOOST_MIRROR_REG_NAMESPACE((test)(ns_1)(ns_1_2))
BOOST_MIRROR_REG_NAMESPACE((test)(ns_1)(ns_1_3))
BOOST_MIRROR_REG_NAMESPACE((test)(ns_2))
BOOST_MIRROR_REG_NAMESPACE((test)(ns_2)(ns_2_1))
BOOST_MIRROR_REG_NAMESPACE((test)(ns_2)(ns_2_2))
BOOST_MIRROR_REG_NAMESPACE((test)(ns_2)(ns_2_3))

/** Register the types and classes
 */
BOOST_MIRROR_REG_TYPE(::test::ns_1::ns_1_1, A)
BOOST_MIRROR_REG_TYPE(::test::ns_1::ns_1_2, B)
BOOST_MIRROR_REG_TYPE(::test::ns_1::ns_1_3, C)
BOOST_MIRROR_REG_TYPE(::test::ns_2::ns_2_1, D)
BOOST_MIRROR_REG_TYPE(::test::ns_2::ns_2_2, E)
BOOST_MIRROR_REG_TYPE(::test::ns_2::ns_2_3, F)
BOOST_MIRROR_REG_TYPE(::test::ns_2,         G)


BOOST_MIRROR_REG_BASE_CLASSES_BEGIN(::test::ns_2::G)
BOOST_MIRROR_REG_SIMPLE_BASE_CLASS(0,  ::test::ns_1::ns_1_1::A)
BOOST_MIRROR_REG_SIMPLE_BASE_CLASS(1,  ::test::ns_1::ns_1_2::B)
BOOST_MIRROR_REG_SIMPLE_BASE_CLASS(2,  ::test::ns_1::ns_1_3::C)
BOOST_MIRROR_REG_SIMPLE_BASE_CLASS(3,  ::test::ns_2::ns_2_1::D)
BOOST_MIRROR_REG_SIMPLE_BASE_CLASS(4,  ::test::ns_2::ns_2_2::E)
BOOST_MIRROR_REG_SIMPLE_BASE_CLASS(5,  ::test::ns_2::ns_2_3::F)
BOOST_MIRROR_REG_BASE_CLASSES_END


/** Class attributes
 */
BOOST_MIRROR_REG_CLASS_ATTRIBS_BEGIN(::test::ns_1::ns_1_1::A)	
	BOOST_MIRROR_REG_AUTO_CLASS_ATTRIB(_, a)
BOOST_MIRROR_REG_CLASS_ATTRIBS_END

BOOST_MIRROR_REG_CLASS_ATTRIBS_BEGIN(::test::ns_1::ns_1_2::B)	
	BOOST_MIRROR_REG_AUTO_CLASS_ATTRIB(_, b)
BOOST_MIRROR_REG_CLASS_ATTRIBS_END

BOOST_MIRROR_REG_CLASS_ATTRIBS_BEGIN(::test::ns_1::ns_1_3::C)	
	BOOST_MIRROR_REG_AUTO_CLASS_ATTRIB(_, c)
BOOST_MIRROR_REG_CLASS_ATTRIBS_END

BOOST_MIRROR_REG_CLASS_ATTRIBS_BEGIN(::test::ns_2::ns_2_1::D)	
	BOOST_MIRROR_REG_AUTO_CLASS_ATTRIB(_, d)
BOOST_MIRROR_REG_CLASS_ATTRIBS_END

BOOST_MIRROR_REG_CLASS_ATTRIBS_BEGIN(::test::ns_2::ns_2_2::E)	
	BOOST_MIRROR_REG_AUTO_CLASS_ATTRIB(_, e)
BOOST_MIRROR_REG_CLASS_ATTRIBS_END

BOOST_MIRROR_REG_CLASS_ATTRIBS_BEGIN(::test::ns_2::ns_2_3::F)	
	BOOST_MIRROR_REG_AUTO_CLASS_ATTRIB(_, f)
BOOST_MIRROR_REG_CLASS_ATTRIBS_END

BOOST_MIRROR_REG_CLASS_ATTRIBS_BEGIN(::test::ns_2::G)	
	BOOST_MIRROR_REG_AUTO_CLASS_ATTRIB(_, g)
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
	typedef BOOST_MIRRORED_NAMESPACE(::test) meta_test;
	//
	/** NOTE: this will only visit the test namespace with compilers
	 *  for which the BOOST_MIRROR_NO_GLOBAL_LISTS macro is defined
	 */
	//
	bcout << "--------------------------------------------" << endl;
	deep_traversal_of<meta_test>::accept(meta_path_sample_visitor());
	bcout << "--------------------------------------------" << endl;
	flat_traversal_of<meta_test>::accept(meta_path_sample_visitor());
	bcout << "--------------------------------------------" << endl;
	//
	//
	return 0;
}

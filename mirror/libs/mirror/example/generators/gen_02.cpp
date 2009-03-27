/**
 * \file examples/generators/gen_02.cpp
 *
 *  This example shows the usage of class generators.
 *
 *  Copyright 2008 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#include <boost/char_type_switch/iostream.hpp>
#include <boost/char_type_switch/string.hpp>

#include <boost/mirror/meta_class.hpp>
#include <boost/mirror/class_generators.hpp>

#include <boost/call_traits.hpp>
#include <boost/type_traits/is_fundamental.hpp>

#include <list>
#include <cmath>

namespace test {

struct vector
{
	double x;
	double y;
	double z;
};

struct tetrahedron 
{
	vector a;
	vector b;
	vector c;
	vector d;
};

} // namespace test

namespace boost { 
namespace mirror {

/** Register everything
 */
BOOST_MIRROR_REG_NAMESPACE((test))
BOOST_MIRROR_REG_TYPE(::test, vector)
BOOST_MIRROR_REG_CLASS_ATTRIBS_BEGIN(::test::vector)
	BOOST_MIRROR_REG_AUTO_CLASS_ATTRIB(_, x)
	BOOST_MIRROR_REG_AUTO_CLASS_ATTRIB(_, y)
	BOOST_MIRROR_REG_AUTO_CLASS_ATTRIB(_, z)
BOOST_MIRROR_REG_CLASS_ATTRIBS_END

BOOST_MIRROR_REG_TYPE(::test, tetrahedron)
BOOST_MIRROR_REG_CLASS_ATTRIBS_BEGIN(::test::tetrahedron)
	BOOST_MIRROR_REG_AUTO_CLASS_ATTRIB(_, a)
	BOOST_MIRROR_REG_AUTO_CLASS_ATTRIB(_, b)
	BOOST_MIRROR_REG_AUTO_CLASS_ATTRIB(_, c)
	BOOST_MIRROR_REG_AUTO_CLASS_ATTRIB(_, d)
BOOST_MIRROR_REG_CLASS_ATTRIBS_END

} // namespace mirror
} // namespace boost

namespace test {

typedef ::boost::cts::bstring string;

/** A building block template for the individual attributes
 *  (that had fundamental types) in the generated class.
 *  The other attributes (with non-fundamental types) are 
 *  recursivelly generated using the same generator and 
 *  building_block template.
 */
template <class MetaAttribute>
class building_block
{
private:
	// the type of the attribute
	typedef typename MetaAttribute::type::reflected_type type;
	// parameter type
	typedef typename ::boost::call_traits<type>::param_type param_type;

	// the path to this building block inside of the generated class
	string path;
public:
	building_block(const string& _path)
	 : path(_path)
	{ }

	inline type get(void) const
	{
		// in a real-life application, instead of this 
		// console I/O the building block could
		// access a configuration file, Windows registry
		// or a database to get the value
		::boost::cts::bcout() << 
			BOOST_CTS_LIT("Get value of ") <<
			path <<
			BOOST_CTS_LIT("/") <<
			MetaAttribute::base_name() <<
			BOOST_CTS_LIT(": ") <<
			::std::flush;
		type value;
		::boost::cts::bcin() >> value;
		return value;
	}

	inline void set(param_type value)
	{
		// again a real-life implementation could
		// be writing the value to a config, registry
		// or a database 
		::boost::cts::bcout() << 
			BOOST_CTS_LIT("New value of ") <<
			path <<
			BOOST_CTS_LIT("/") <<
			MetaAttribute::base_name() <<
			BOOST_CTS_LIT(" = ") <<
			value << 
			::std::endl;
	}

	inline void operator = (param_type value)
	{
		set(value);
	}
};

/** Forward declaration of the building_block getter
 */
template <class MetaAttribute>
struct get_building_block;

/** Specialization of building block getter for
 *  fundamental types
 */
template <class MetaAttribute, class IsFundamental>
struct do_get_building_block
{
	typedef building_block<MetaAttribute> type;
};

/** Specialization of building block getter for
 *  non-fundamental types
 */
template <class MetaAttribute>
struct do_get_building_block<MetaAttribute, ::boost::mpl::false_>
{
	typedef ::boost::mirror::class_generator<
                typename MetaAttribute::type::all_attributes,
                get_building_block,
                ::boost::mpl::false_
        > base_generator;

	/** This is the building block type for attributes
	 *  with non-fundamental types
	 */
	struct type : public base_generator
	{
		static inline const string& separator(void)
		{
			static string sep(BOOST_CTS_LIT("/"));
			return sep;
		}

		/** The constructor composes the path to this
		 *  part of the geenrated class and passes along
		 *  to the base generator
		 */
		type(const string& path)
		 : base_generator(
			path + 
			separator() +
			MetaAttribute::base_name()
		) { }
	};
};


/** Implementation of building block getter meta-function
 */
template <class MetaAttribute>
struct get_building_block
{
	typedef typename MetaAttribute::type::reflected_type
		attrib_type;

	typedef typename do_get_building_block<
		MetaAttribute,
		::boost::mpl::bool_<
			::boost::is_fundamental<attrib_type>::value
		>
	>::type type;
};

/** A test function which uses the generated vectors and calculates
 *  the difference between them.
 */
template <class GeneratedVector1, class GeneratedVector2>
double difference(const GeneratedVector1& v1, const GeneratedVector2& v2)
{
	return ::std::sqrt(
		v1.x.get()*v2.x.get() + 
		v1.y.get()*v2.y.get() + 
		v1.z.get()*v2.z.get() 
	);
}

} // namespace test

int main(void)
{
	using namespace ::std;
	using namespace ::boost;
	//
	cts::bostream& bcout = cts::bcout();
	typedef BOOST_MIRRORED_CLASS(::test::tetrahedron) meta_T;
	//
	// uses the class generator and the meta-data about the
	// ::test::person class to create a new class with 
	// the same set of attributes which are made of 
	// the ::test::building_block instantiations.
	typedef mirror::class_generator<
		meta_T::all_attributes,
		::test::get_building_block,
		mpl::false_
	> tetrahedron;
	::test::string name(BOOST_CTS_LIT("t"));
	tetrahedron t(name);
	//
	// use compare the differences between the individual
	// vectors in the tetrahedron
	if(::test::difference(t.a, t.b) > ::test::difference(t.c, t.d))
		bcout << "|ab| >  |cd|" << ::std::endl;
	else 	bcout << "|ab| <= |cd|" << ::std::endl;
	//
	// set the value of the t.a's and t.b's coordinates
	t.a.x = 1.0;
	t.a.y = 2.0;
	t.a.z = 3.0;
	t.b.x = 4.0;
	t.b.y = 5.0;
	t.b.z = 6.0;
	//
	return 0;
}


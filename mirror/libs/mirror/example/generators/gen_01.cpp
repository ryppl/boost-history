/**
 * \file examples/generators/gen_01.cpp
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

#include <vector>

namespace test {

typedef ::boost::cts::bstring string;

struct person
{
	string name;
	string surname;
	double height;
	double weight;
};

} // namespace test

namespace boost { 
namespace mirror {

/** Register everything
 */
BOOST_MIRROR_REG_NAMESPACE((test))
BOOST_MIRROR_REG_TYPE(::test, person)
BOOST_MIRROR_REG_CLASS_ATTRIBS_BEGIN(::test::person)
	BOOST_MIRROR_REG_AUTO_CLASS_ATTRIB(_, name)
	BOOST_MIRROR_REG_AUTO_CLASS_ATTRIB(_, surname)
	BOOST_MIRROR_REG_AUTO_CLASS_ATTRIB(_, height)
	BOOST_MIRROR_REG_AUTO_CLASS_ATTRIB(_, weight)
BOOST_MIRROR_REG_CLASS_ATTRIBS_END

} // namespace mirror
} // namespace boost

namespace test {

template <class MetaAttribute>
class building_block
{
private:
	typedef typename MetaAttribute::type::reflected_type type;
	type value;

	typedef typename ::boost::call_traits<type>::param_type param_type;
public:
	building_block(void)
	{
		::boost::cts::bcerr() << 
			BOOST_CTS_LIT("Enter ") <<
			MetaAttribute::base_name() <<
			BOOST_CTS_LIT(" (") <<
			MetaAttribute::type::base_name() << 
			BOOST_CTS_LIT("): ") <<
			::std::flush;
		::boost::cts::bcin() >> value;
	}

	inline param_type get(void) const
	{
		::boost::cts::bcerr() << 
			BOOST_CTS_LIT("Getting the value of ") <<
			MetaAttribute::base_name() <<
			::std::endl;
		return value;
	}
};

template <class MetaAttribute>
struct get_building_block
{
	typedef building_block<MetaAttribute> type;
};

} // namespace test

int main(void)
{
	using namespace ::std;
	using namespace ::boost;
	//
	cts::bostream& bcout = cts::bcout();
	typedef BOOST_MIRRORED_CLASS(::test::person) meta_person;
	//
	// use the class generator and the meta-data about the
	// ::test::person class to create a new class with 
	// the same set of attributes which are made of 
	// the ::test::building_block instantiations.
	typedef mirror::class_generator<
		meta_person::attributes,
		::test::get_building_block,
		mpl::false_
	> person;
	// a list of persons
	vector<person> persons;	
	//
	// construct and insert a few persons into the list
	int n = 3;
	while(n--)
	{
		persons.push_back(person());
	}
	//
	// print out info about the persons stored in the list
	for(
		vector<person>::iterator 
			i = persons.begin(),
			e = persons.end();
		i != e;
		++i
	) bcout << 
		meta_person::base_name() << 
		BOOST_CTS_LIT("(") << 
		i->name.get() << 
		BOOST_CTS_LIT(", ") << 
		i->surname.get() << 
		BOOST_CTS_LIT(", ") << 
		i->height.get() << 
		BOOST_CTS_LIT(", ") << 
		i->weight.get() << 
		BOOST_CTS_LIT(")") << 
		::std::endl;
	//
	return 0;
}


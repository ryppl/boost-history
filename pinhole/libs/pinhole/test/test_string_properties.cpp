// Boost.Pinhole library

// Copyright Jared McIntyre 2007. Use, modification and
// distribution is subject to the Boost Software License, Version
// 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

// For more information, see http://www.boost.org

#define BOOST_TEST_MODULE PinholeLib

#include <boost/pinhole/property_group.hpp>
#include <boost/pinhole/property_manager.hpp>
#include <boost/test/unit_test.hpp>

using namespace std;
using namespace boost;
using namespace boost::pinhole;

// I can hide these two line if I don't do everything in headers
boost::shared_ptr<property_manager> property_manager::m_instance(new property_manager);
event_source* event_source::m_instance = 0;

class TestGroup : public property_group
{
public:
#if defined(BOOST_MSVC)
    #pragma warning(push)
    #pragma warning( disable: 4355 )
#endif
	TestGroup() : property_group( "PropertyGroupName", NULL )
	{
		add_property("String_Func", "String1 description", BOOST_SETTER(&TestGroup::SetString), BOOST_GETTER(&TestGroup::GetString), NULL );
		add_property("String_Var",  "String2 description", BOOST_SETTER_VAR(m_string_Var),      BOOST_GETTER_VAR(m_string_Var),     NULL );
	}
#if defined(BOOST_MSVC)
    #pragma warning(pop)
#endif

	void SetString( string x ){ m_string_Func = x; }
	string GetString(){ return( m_string_Func ); }

private:
	string m_string_Func;
	string m_string_Var;
};

BOOST_AUTO_TEST_CASE( TestGetSetString_Func )
{
    TestGroup testGroup;

	testGroup.set( "String_Func", string("first") );
	BOOST_CHECK_EQUAL( testGroup.get<string>("String_Func"), "first" );
    
	testGroup.set_as_string( "String_Func", "second" );
	BOOST_CHECK_EQUAL( testGroup.get_as_string("String_Func"), "second" );
}

BOOST_AUTO_TEST_CASE( TestGetSetString_Var )
{
    TestGroup testGroup;

	testGroup.set( "String_Var", string("first") );
	BOOST_CHECK_EQUAL( testGroup.get<string>("String_Var"), "first" );
    
	testGroup.set_as_string( "String_Var", "second" );
	BOOST_CHECK_EQUAL( testGroup.get_as_string("String_Var"), "second" );
}

BOOST_AUTO_TEST_CASE( TestStringPropertyType )
{
    TestGroup testGroup;
    
    BOOST_CHECK( typeid(bool) != testGroup.get_type_info("String_Func") );
    BOOST_CHECK( typeid(int) != testGroup.get_type_info("String_Func") );
    BOOST_CHECK( typeid(float) != testGroup.get_type_info("String_Func") );
    BOOST_CHECK( typeid(double) != testGroup.get_type_info("String_Func") );
    BOOST_CHECK( typeid(std::string) == testGroup.get_type_info("String_Func") );
}
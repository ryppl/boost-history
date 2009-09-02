// Boost.Pinhole library

// Copyright Jared McIntyre 2007-2009. Use, modification and
// distribution is subject to the Boost Software License, Version
// 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

// For more information, see http://www.boost.org

#define BOOST_TEST_MODULE PinholeLib

#include <boost/config.hpp>
#include <boost/pinhole.hpp>
#include <boost/test/unit_test.hpp>

using namespace std;
using namespace boost;
using namespace boost::pinhole;

class TestGroup : public property_group
{
public:
#if defined(BOOST_MSVC)
    #pragma warning(push)
    #pragma warning( disable: 4355 )
#endif
	TestGroup() : property_group( "PropertyGroupName", NULL )
	{
		add_property("String_Func",  BOOST_SETTER(&TestGroup::SetString),  BOOST_GETTER(&TestGroup::GetString) );
		add_property("String_Var",   BOOST_SETTER_VAR(m_string_Var),       BOOST_GETTER_VAR(m_string_Var) );
        add_property("WString_Func", BOOST_SETTER(&TestGroup::SetWString), BOOST_GETTER(&TestGroup::GetWString) );
        add_property("WString_Var",  BOOST_SETTER_VAR(m_wstring_Var),      BOOST_GETTER_VAR(m_wstring_Var) );
	}
#if defined(BOOST_MSVC)
    #pragma warning(pop)
#endif

	void SetString( string x ){ m_string_Func = x; }
    string GetString(){ return( m_string_Func ); }

    void SetWString( wstring x ){ m_wstring_Func = x; }
    wstring GetWString(){ return( m_wstring_Func ); }

private:
	string m_string_Func;
    string m_string_Var;
    wstring m_wstring_Func;
    wstring m_wstring_Var;
};

BOOST_AUTO_TEST_CASE( TestGetSetString_Func )
{
    TestGroup testGroup;

	testGroup.set( "String_Func", string("first") );
	BOOST_CHECK_EQUAL( testGroup.get<string>("String_Func"), "first" );
    
	testGroup.set_as_string( "String_Func", "second" );
    BOOST_CHECK_EQUAL( testGroup.get_as_string("String_Func"), "second" );

    testGroup.set_as_wstring("String_Func", L"third");
    BOOST_CHECK( testGroup.get_as_string("String_Func") == "third" );
    BOOST_CHECK_EQUAL( testGroup.get_as_string("String_Func"), "third" );
}

BOOST_AUTO_TEST_CASE( TestGetSetString_Var )
{
    TestGroup testGroup;

	testGroup.set( "WString_Var", wstring(L"first") );
	BOOST_CHECK( testGroup.get<wstring>("WString_Var") == L"first" );
    
	testGroup.set_as_wstring( "WString_Var", L"second" );
    BOOST_CHECK( testGroup.get_as_wstring("WString_Var") == L"second" );

    testGroup.set_as_string( "WString_Var", "third" );
    BOOST_CHECK_EQUAL( testGroup.get_as_string("WString_Var"), "third" );
    BOOST_CHECK( testGroup.get_as_wstring("WString_Var") == L"third" );
}

BOOST_AUTO_TEST_CASE( TestGetSetWString_Func )
{
    TestGroup testGroup;

    testGroup.set( "WString_Func", wstring(L"first") );
    BOOST_CHECK( testGroup.get<wstring>("WString_Func") == L"first" );

    testGroup.set_as_wstring( "WString_Func", L"second" );
    BOOST_CHECK( testGroup.get_as_wstring("WString_Func") == L"second" );

    testGroup.set_as_string("WString_Func", "third" );
    BOOST_CHECK_EQUAL( testGroup.get_as_string("WString_Func"), "third" );
    BOOST_CHECK( testGroup.get_as_wstring("WString_Func") == L"third" );
}

BOOST_AUTO_TEST_CASE( TestGetSetWString_Var )
{
    TestGroup testGroup;

    testGroup.set( "String_Var", string("first") );
    BOOST_CHECK_EQUAL( testGroup.get<string>("String_Var"), "first" );

    testGroup.set_as_string( "String_Var", "second" );
    BOOST_CHECK_EQUAL( testGroup.get_as_string("String_Var"), "second" );

    testGroup.set_as_wstring("String_Var", L"third" );
    BOOST_CHECK( testGroup.get_as_wstring("String_Var") == L"third");
    BOOST_CHECK_EQUAL( testGroup.get_as_string("String_Var"), "third" );
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
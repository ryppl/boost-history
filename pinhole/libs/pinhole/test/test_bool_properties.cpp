// Boost.Pinhole library

// Copyright Jared McIntyre 2007. Use, modification and
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
        m_bool_Func = false;
	    m_bool_Var  = false;

		add_property("Bool_Func", BOOST_SETTER(&TestGroup::SetBool), BOOST_GETTER(&TestGroup::GetBool) );
		add_property("Bool_Var",  BOOST_SETTER_VAR(m_bool_Var),      BOOST_GETTER_VAR(m_bool_Var) );
	}
#if defined(BOOST_MSVC)
    #pragma warning(pop)
#endif

	void SetBool( bool x ){ m_bool_Func = x; }
	bool GetBool(){ return( m_bool_Func ); }

private:
	bool m_bool_Func;
	bool m_bool_Var;
};

BOOST_AUTO_TEST_CASE( TestSetGetBool_Func )
{
    TestGroup testGroup;
    
	testGroup.set( "Bool_Func", true );
	BOOST_CHECK_EQUAL( true, testGroup.get<bool>("Bool_Func") );

	testGroup.set( "Bool_Func", false );
	BOOST_CHECK_EQUAL( false, testGroup.get<bool>("Bool_Func") );
    
	testGroup.set_as_string( "Bool_Func", "true" );
	BOOST_CHECK_EQUAL( "True", testGroup.get_as_string("Bool_Func") );

	testGroup.set_as_string( "Bool_Func", "false" );
	BOOST_CHECK_EQUAL( "False", testGroup.get_as_string("Bool_Func") );
    
	testGroup.set_as_string( "Bool_Func", "True" );
	BOOST_CHECK_EQUAL( "True", testGroup.get_as_string("Bool_Func") );

	testGroup.set_as_string( "Bool_Func", "False" );
	BOOST_CHECK_EQUAL( "False", testGroup.get_as_string("Bool_Func") );
    
	testGroup.set_as_string( "Bool_Func", "TrUe" );
	BOOST_CHECK_EQUAL( "True", testGroup.get_as_string("Bool_Func") );

	testGroup.set_as_string( "Bool_Func", "FalSe" );
	BOOST_CHECK_EQUAL( "False", testGroup.get_as_string("Bool_Func") );
    
	testGroup.set_as_string( "Bool_Func", "TRUE" );
	BOOST_CHECK_EQUAL( "True", testGroup.get_as_string("Bool_Func") );

	testGroup.set_as_string( "Bool_Func", "FALSE" );
	BOOST_CHECK_EQUAL( "False", testGroup.get_as_string("Bool_Func") );
    
	testGroup.set_as_string( "Bool_Func", "1" );
	BOOST_CHECK_EQUAL( "True", testGroup.get_as_string("Bool_Func") );

	testGroup.set_as_string( "Bool_Func", "0" );
	BOOST_CHECK_EQUAL( "False", testGroup.get_as_string("Bool_Func") );

    testGroup.set_as_wstring( "Bool_Func", L"true" );
    BOOST_CHECK( L"True" == testGroup.get_as_wstring("Bool_Func") );

    testGroup.set_as_wstring( "Bool_Func", L"false" );
    BOOST_CHECK( L"False" == testGroup.get_as_wstring("Bool_Func") );

    testGroup.set_as_wstring( "Bool_Func", L"True" );
    BOOST_CHECK( L"True" == testGroup.get_as_wstring("Bool_Func") );

    testGroup.set_as_wstring( "Bool_Func", L"False" );
    BOOST_CHECK( L"False" == testGroup.get_as_wstring("Bool_Func") );

    testGroup.set_as_wstring( "Bool_Func", L"TrUe" );
    BOOST_CHECK( L"True" == testGroup.get_as_wstring("Bool_Func") );

    testGroup.set_as_wstring( "Bool_Func", L"FalSe" );
    BOOST_CHECK( L"False" == testGroup.get_as_wstring("Bool_Func") );

    testGroup.set_as_wstring( "Bool_Func", L"TRUE" );
    BOOST_CHECK( L"True" == testGroup.get_as_wstring("Bool_Func") );

    testGroup.set_as_wstring( "Bool_Func", L"FALSE" );
    BOOST_CHECK( L"False" == testGroup.get_as_wstring("Bool_Func") );

    testGroup.set_as_wstring( "Bool_Func", L"1" );
    BOOST_CHECK( L"True" == testGroup.get_as_wstring("Bool_Func") );

    testGroup.set_as_wstring( "Bool_Func", L"0" );
    BOOST_CHECK( L"False" == testGroup.get_as_wstring("Bool_Func") );
}

BOOST_AUTO_TEST_CASE( TestSetGetBool_Var )
{
    TestGroup testGroup;
    
	testGroup.set( "Bool_Func", true );
	BOOST_CHECK_EQUAL( true, testGroup.get<bool>( "Bool_Func") );

	testGroup.set( "Bool_Var", false );
	BOOST_CHECK_EQUAL( false, testGroup.get<bool>( "Bool_Var") );
    
	testGroup.set_as_string( "Bool_Var", "true" );
	BOOST_CHECK_EQUAL( "True", testGroup.get_as_string( "Bool_Var") );

	testGroup.set_as_string( "Bool_Var", "false" );
	BOOST_CHECK_EQUAL( "False", testGroup.get_as_string( "Bool_Var") );
    
	testGroup.set_as_string( "Bool_Var", "True" );
	BOOST_CHECK_EQUAL( "True", testGroup.get_as_string( "Bool_Var") );

	testGroup.set_as_string( "Bool_Var", "False" );
	BOOST_CHECK_EQUAL( "False", testGroup.get_as_string( "Bool_Var") );
    
	testGroup.set_as_string( "Bool_Var", "TrUe" );
	BOOST_CHECK_EQUAL( "True", testGroup.get_as_string( "Bool_Var") );

	testGroup.set_as_string( "Bool_Var", "FalSe" );
	BOOST_CHECK_EQUAL( "False", testGroup.get_as_string( "Bool_Var") );
    
	testGroup.set_as_string( "Bool_Var", "TRUE" );
	BOOST_CHECK_EQUAL( "True", testGroup.get_as_string( "Bool_Var") );

	testGroup.set_as_string( "Bool_Var", "FALSE" );
	BOOST_CHECK_EQUAL( "False", testGroup.get_as_string( "Bool_Var") );
    
	testGroup.set_as_string( "Bool_Var", "1" );
	BOOST_CHECK_EQUAL( "True", testGroup.get_as_string( "Bool_Var") );

	testGroup.set_as_string( "Bool_Var", "0" );
	BOOST_CHECK_EQUAL( "False", testGroup.get_as_string( "Bool_Var") );

    testGroup.set_as_wstring( "Bool_Var", L"true" );
    BOOST_CHECK( L"True" == testGroup.get_as_wstring( "Bool_Var") );

    testGroup.set_as_wstring( "Bool_Var", L"false" );
    BOOST_CHECK( L"False" == testGroup.get_as_wstring( "Bool_Var") );

    testGroup.set_as_wstring( "Bool_Var", L"True" );
    BOOST_CHECK( L"True" == testGroup.get_as_wstring( "Bool_Var") );

    testGroup.set_as_wstring( "Bool_Var", L"False" );
    BOOST_CHECK( L"False" == testGroup.get_as_wstring( "Bool_Var") );

    testGroup.set_as_wstring( "Bool_Var", L"TrUe" );
    BOOST_CHECK( L"True" == testGroup.get_as_wstring( "Bool_Var") );

    testGroup.set_as_wstring( "Bool_Var", L"FalSe" );
    BOOST_CHECK( L"False" == testGroup.get_as_wstring( "Bool_Var") );

    testGroup.set_as_wstring( "Bool_Var", L"TRUE" );
    BOOST_CHECK( L"True" == testGroup.get_as_wstring( "Bool_Var") );

    testGroup.set_as_wstring( "Bool_Var", L"FALSE" );
    BOOST_CHECK( L"False" == testGroup.get_as_wstring( "Bool_Var") );

    testGroup.set_as_wstring( "Bool_Var", L"1" );
    BOOST_CHECK( L"True" == testGroup.get_as_wstring( "Bool_Var") );

    testGroup.set_as_wstring( "Bool_Var", L"0" );
    BOOST_CHECK( L"False" == testGroup.get_as_wstring( "Bool_Var") );
}

BOOST_AUTO_TEST_CASE( TestBoolPropertyType )
{
    TestGroup testGroup;
    
    BOOST_CHECK( typeid(bool) == testGroup.get_type_info("Bool_Func") );
    BOOST_CHECK( typeid(int) != testGroup.get_type_info("Bool_Func") );
    BOOST_CHECK( typeid(float) != testGroup.get_type_info("Bool_Func") );
    BOOST_CHECK( typeid(double) != testGroup.get_type_info("Bool_Func") );
    BOOST_CHECK( typeid(std::string) != testGroup.get_type_info("Bool_Func") );
}

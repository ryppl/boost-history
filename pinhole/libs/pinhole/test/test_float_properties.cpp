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
        m_float_Func = 1.1f;
	    m_float_Var  = 1.1f;

		add_property("Float_Func", BOOST_SETTER(&TestGroup::SetFloat), BOOST_GETTER(&TestGroup::GetFloat) );
		add_property("Float_Var",  BOOST_SETTER_VAR(m_float_Var),      BOOST_GETTER_VAR(m_float_Var) );
	}
#if defined(BOOST_MSVC)
    #pragma warning(pop)
#endif

	void SetFloat( float x ){ m_float_Func = x; }
	float GetFloat(){ return( m_float_Func ); }

private:
	float m_float_Func;
	float m_float_Var;
};

BOOST_AUTO_TEST_CASE( TestSetGetFloat_Func )
{
    TestGroup testGroup;

	testGroup.set( "Float_Func", 2.2f );
	BOOST_CHECK_EQUAL( 2.2f, testGroup.get<float>("Float_Func") );
    
	testGroup.set_as_string( "Float_Func", "3.3" );
	//BOOST_CHECK_EQUAL( "3.3", testGroup.get_as_string("Float_Func") );
    BOOST_CHECK_EQUAL( 3.3f, testGroup.get<float>("Float_Func") );

    testGroup.set_as_wstring( "Float_Func", L"4.4" );
    //BOOST_CHECK_EQUAL( "4.4", testGroup.get_as_wstring("Float_Func") );
    BOOST_CHECK_EQUAL( 4.4f, testGroup.get<float>("Float_Func") );
}

BOOST_AUTO_TEST_CASE( TestSetGetFloat_Var )
{
    TestGroup testGroup;

	testGroup.set( "Float_Var", 2.2f );
	BOOST_CHECK_EQUAL( 2.2f, testGroup.get<float>( "Float_Var") );
    
	testGroup.set_as_string( "Float_Var", "3.3" );
	//BOOST_CHECK_EQUAL( "3.3", testGroup.get_as_string("Float_Var") );
    BOOST_CHECK_EQUAL( 3.3f, testGroup.get<float>("Float_Var") );

    testGroup.set_as_wstring( "Float_Var", L"4.4" );
    //BOOST_CHECK_EQUAL( "4.4", testGroup.get_as_wstring("Float_Var") );
    BOOST_CHECK_EQUAL( 4.4f, testGroup.get<float>("Float_Var") );
}

BOOST_AUTO_TEST_CASE( TestFloatPropertyType )
{
    TestGroup testGroup;
    
    BOOST_CHECK( typeid(bool) != testGroup.get_type_info("Float_Func") );
    BOOST_CHECK( typeid(int) != testGroup.get_type_info("Float_Func") );
    BOOST_CHECK( typeid(float) == testGroup.get_type_info("Float_Func") );
    BOOST_CHECK( typeid(double) != testGroup.get_type_info("Float_Func") );
    BOOST_CHECK( typeid(std::string) != testGroup.get_type_info("Float_Func") );
}

BOOST_AUTO_TEST_CASE( TestInvalidSet )
{
    TestGroup testGroup;
    
    BOOST_CHECK_THROW( testGroup.set_as_string( "Float_Func", "Foo" ), std::invalid_argument );
    BOOST_CHECK_THROW( testGroup.set_as_wstring( "Float_Func", L"Foo" ), std::invalid_argument );
}

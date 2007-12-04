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

class TestGroup : public property_group
{
public:
#if defined(BOOST_MSVC)
    #pragma warning(push)
    #pragma warning( disable: 4355 )
#endif
	TestGroup() : property_group( "PropertyGroupName", NULL )
	{
        m_double_Func = 1.1;
	    m_double_Var  = 1.1;

		add_property("Double_Func", "Double1 description", BOOST_SETTER(&TestGroup::SetDouble), BOOST_GETTER(&TestGroup::GetDouble), NULL );
		add_property("Double_Var",  "Double2 description", BOOST_SETTER_VAR(m_double_Var),      BOOST_GETTER_VAR(m_double_Var),     NULL );
	}
#if defined(BOOST_MSVC)
    #pragma warning(pop)
#endif

	void SetDouble( double x ){ m_double_Func = x; }
	double GetDouble(){ return( m_double_Func ); }

private:
	double m_double_Func;
	double m_double_Var;
};

// I can hide these two line if I don't do everything in headers
boost::shared_ptr<property_manager> property_manager::m_instance(new property_manager);
event_source* event_source::m_instance = 0;

BOOST_AUTO_TEST_CASE( TestSetGetDouble_Func )
{
    TestGroup testGroup;

	testGroup.set( "Double_Func", 2.2 );
	BOOST_CHECK_EQUAL( 2.2, testGroup.get<double>( "Double_Func") );
    
	testGroup.set_as_string( "Double_Func", "3.3" );
	//BOOST_CHECK_EQUAL( "3.3", testGroup.get_as_string( "Double_Func") );
	BOOST_CHECK_EQUAL( 3.3, testGroup.get<double>( "Double_Func") );
}

BOOST_AUTO_TEST_CASE( TestSetGetDouble_Var )
{
    TestGroup testGroup;

	testGroup.set( "Double_Var", 2.2 );
	BOOST_CHECK_EQUAL( 2.2, testGroup.get<double>( "Double_Var") );
    
	testGroup.set_as_string( "Double_Var", "3.3" );
	//BOOST_CHECK_EQUAL( "3.3", testGroup.get_as_string( "Double_Var") );
	BOOST_CHECK_EQUAL( 3.3, testGroup.get<double>( "Double_Var") );
}

BOOST_AUTO_TEST_CASE( TestDoublePropertyType )
{
    TestGroup testGroup;
    
    BOOST_CHECK( typeid(bool) != testGroup.get_type_info("Double_Func") );
    BOOST_CHECK( typeid(int) != testGroup.get_type_info("Double_Func") );
    BOOST_CHECK( typeid(float) != testGroup.get_type_info("Double_Func") );
    BOOST_CHECK( typeid(double) == testGroup.get_type_info("Double_Func") );
    BOOST_CHECK( typeid(std::string) != testGroup.get_type_info("Double_Func") );
}

BOOST_AUTO_TEST_CASE( TestInvalidSet )
{
    TestGroup testGroup;
    
	BOOST_CHECK_THROW( testGroup.set_as_string( "Double_Func", "Foo" ), std::invalid_argument );
}

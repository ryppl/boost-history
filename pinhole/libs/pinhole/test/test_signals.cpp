// Boost.Pinhole library

// Copyright Jared McIntyre 2009. Use, modification and
// distribution is subject to the Boost Software License, Version
// 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

// For more information, see http://www.boost.org

#define BOOST_TEST_MODULE PinholeLib

#include <boost/config.hpp>
#include <boost/pinhole.hpp>
#include <boost/signals.hpp>
#include <boost/test/unit_test.hpp>

using namespace std;
using namespace boost;
using namespace boost::pinhole;

#define SIGNAL_1 ("BOOST_SIGNAL 1")
#define SIGNAL_2 ("BOOST_SIGNAL 2")

typedef boost::signal<void(int)>     OneIntSignal;
typedef boost::signal<void(int,int)> TwoIntSignal;

class TestSignalsFixture : public property_group
{
public:
    #if defined(BOOST_MSVC)
    #pragma warning(push)
    #pragma warning( disable: 4355 )
    #endif
        TestSignalsFixture()
            : property_group( "PropertyGroupName", NULL )
            , m_val1(-1)
            , m_val2(-1)
        {
            
            add_signal(SIGNAL_1, int1Signal);
            add_signal(SIGNAL_2, int2Signal, string("some info"));
        }
    #if defined(BOOST_MSVC)
    #pragma warning(pop)
    #endif
    
    void clear_actions()
    {
        m_actions.clear();
    }
    
    OneIntSignal int1Signal;
    TwoIntSignal int2Signal;
    
    void Signal1Triggered(int val)            {m_val1 = val;  m_val2 = -1;}
    void Signal2Triggered(int val1, int val2) {m_val1 = val1; m_val2 = val2;}
    
    int m_val1;
    int m_val2;
};

void someFunc(int)
{
}

BOOST_AUTO_TEST_CASE( TestSignal )
{
    TestSignalsFixture testFixture;

    boost::signals::connection c1 =
        testFixture.get_signal<OneIntSignal>(SIGNAL_1).connect( boost::bind(&TestSignalsFixture::Signal1Triggered, &testFixture, _1) );
    
    boost::signals::connection c2 =
        testFixture.get_signal<TwoIntSignal>(SIGNAL_2).connect( bind(&TestSignalsFixture::Signal2Triggered, &testFixture, _1, _2) );
    
    BOOST_CHECK_EQUAL(testFixture.m_val1, -1);
    BOOST_CHECK_EQUAL(testFixture.m_val2, -1);
    
    testFixture.int2Signal(2,3);
    
    BOOST_CHECK_EQUAL(testFixture.m_val1, 2);
    BOOST_CHECK_EQUAL(testFixture.m_val2, 3);
    
    testFixture.int1Signal(1);
    
    BOOST_CHECK_EQUAL(testFixture.m_val1, 1);
    BOOST_CHECK_EQUAL(testFixture.m_val2, -1);
    
    c1.disconnect();
    c2.disconnect();
    
    testFixture.m_val1 = 999999;
    testFixture.m_val2 = 999999;
    
    testFixture.int2Signal(2,3);
    
    BOOST_CHECK_EQUAL(testFixture.m_val1, 999999);
    BOOST_CHECK_EQUAL(testFixture.m_val2, 999999);
    
    
    testFixture.int1Signal(1);
    
    BOOST_CHECK_EQUAL(testFixture.m_val1, 999999);
    BOOST_CHECK_EQUAL(testFixture.m_val2, 999999);
}

BOOST_AUTO_TEST_CASE( TestNonExistentSignal )
{
	TestSignalsFixture testFixture;
	
	BOOST_CHECK_THROW( testFixture.get_signal< boost::signal<void(int)> >("NonExistent Signal"), std::out_of_range );
	BOOST_CHECK_THROW( testFixture.get_signal_metadata("NonExistent Signal"), std::out_of_range );
}

BOOST_AUTO_TEST_CASE( TestSignalMetadata )
{
    TestSignalsFixture testFixture;

    BOOST_CHECK_EQUAL( any_cast<string>(testFixture.get_signal_metadata(SIGNAL_2)), string("some info") );
}
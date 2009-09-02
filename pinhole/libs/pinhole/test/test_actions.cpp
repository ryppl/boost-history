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

#define ACTION_1 ("BOOST_ACTION 1")
#define ACTION_2 ("BOOST_ACTION 2")

class TestActionsFixture : public property_group
{
public:
#if defined(BOOST_MSVC)
#pragma warning(push)
#pragma warning( disable: 4355 )
#endif
    TestActionsFixture() : property_group( "PropertyGroupName", NULL )
    {
        bTriggeredAction1 = false;
        bTriggeredAction2 = false;

        add_action(ACTION_1, BOOST_ACTION(&TestActionsFixture::Action1));
        add_action(ACTION_2, BOOST_ACTION(&TestActionsFixture::Action2), string("some info"));
    }
#if defined(BOOST_MSVC)
#pragma warning(pop)
#endif

    void clear_actions()
    {
        m_actions.clear();
    }

    void Action1(){bTriggeredAction1 = true;}
    void Action2(){bTriggeredAction2 = true;}

    bool bTriggeredAction1;
    bool bTriggeredAction2;
};

BOOST_AUTO_TEST_CASE( TestTriggerAction )
{
	TestActionsFixture testFixture;
	
	BOOST_CHECK_EQUAL(testFixture.bTriggeredAction1, false);
	testFixture.trigger(ACTION_1);
	BOOST_CHECK_EQUAL(testFixture.bTriggeredAction1, true);

	BOOST_CHECK_EQUAL(testFixture.bTriggeredAction2, false);
	testFixture.trigger(ACTION_2);
	BOOST_CHECK_EQUAL(testFixture.bTriggeredAction2, true);
}

BOOST_AUTO_TEST_CASE( TestNonExistentAction )
{
	TestActionsFixture testFixture;
	
	BOOST_CHECK_THROW( testFixture.trigger("NonExistent Action"), std::out_of_range );
    BOOST_CHECK_THROW( testFixture.get_action_metadata( "NonExistent Action" ), std::out_of_range );
}

BOOST_AUTO_TEST_CASE( TestGetActionCollection )
{
	TestActionsFixture testFixture;
	
    property_group::action_iterator itr    = testFixture.action_begin();
    property_group::action_iterator itrEnd = testFixture.action_end();

    BOOST_CHECK_EQUAL( testFixture.action_count(), 2u );
    BOOST_CHECK_EQUAL( *itr, ACTION_1 );
    ++itr;
    BOOST_CHECK_EQUAL( *itr, ACTION_2 );
    ++itr;
    BOOST_CHECK( itr == itrEnd );
}

BOOST_AUTO_TEST_CASE( TestActionMetadata )
{
	TestActionsFixture testFixture;
	
    BOOST_CHECK_EQUAL( any_cast<string>(testFixture.get_action_metadata(ACTION_2)), string("some info") );
}
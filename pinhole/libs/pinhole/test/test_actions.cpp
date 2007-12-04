// Boost.Pinhole library

// Copyright Jared McIntyre 2007. Use, modification and
// distribution is subject to the Boost Software License, Version
// 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

// For more information, see http://www.boost.org

#define BOOST_TEST_MODULE PinholeLib
#include <boost/test/unit_test.hpp>
#include "TestClassesAndConstants.h"

// I can hide these two line if I don't do everything in headers
boost::shared_ptr<property_manager> property_manager::m_instance(new property_manager);
event_source* event_source::m_instance = 0;

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
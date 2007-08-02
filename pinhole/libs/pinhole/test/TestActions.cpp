// Boost.Pinhole library

// Copyright Jared McIntyre 2007. Use, modification and
// distribution is subject to the Boost Software License, Version
// 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

// For more information, see http://www.boost.org

#define BOOST_TEST_MODULE PinholeLib
#include <boost/test/unit_test.hpp>
#include "TestClassesAndConstants.h"

BOOST_AUTO_TEST_CASE( TestActionsFixture, TestTriggerAction )
{
	BOOST_CHECK_EQUAL(bTriggeredAction1, false);
	Trigger(ACTION_1);
	BOOST_CHECK_EQUAL(bTriggeredAction1, true);

	BOOST_CHECK_EQUAL(bTriggeredAction2, false);
	Trigger(ACTION_2);
	BOOST_CHECK_EQUAL(bTriggeredAction2, true);
}

BOOST_AUTO_TEST_CASE( TestActionsFixture, TestNonExistentAction )
{
	CHECK_THROW( Trigger("NonExistent Action"), std::out_of_range );
}

BOOST_AUTO_TEST_CASE( TestActionsFixture, TestGetActionCollection )
{
    property_group::action_iterator itr    = action_begin();
    property_group::action_iterator itrEnd = action_end();

    BOOST_CHECK_EQUAL( action_count(), 2 );
    BOOST_CHECK_EQUAL( *itr, ACTION_1 );
    ++itr;
    BOOST_CHECK_EQUAL( *itr, ACTION_2 );
    ++itr;
    BOOST_CHECK( itr == itrEnd );
}
// Boost.Pinhole library

// Copyright Jared McIntyre 2007. Use, modification and
// distribution is subject to the Boost Software License, Version
// 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

// For more information, see http://www.boost.org

#define BOOST_TEST_MODULE PinholeLib
#include <boost/test/unit_test.hpp>
#include "TestClassesAndConstants.h"
#include <boost/pinhole/find.h>

// I can hide these two line if I don't do everything in headers
boost::shared_ptr<property_manager> property_manager::m_instance(new property_manager);
event_source* event_source::m_instance = 0;

class TestPropertyManager : public property_manager
{
public:
	TestPropertyManager() : property_manager()
	{
        if( NULL != property_manager::m_instance.get() )
        {
            property_manager::delete_instance();
        }
        property_manager::m_instance.reset( this );

		uiChildCount                       = 0;
		uiSelectSingleNodeCallCount        = 0;
		uiRegisterPropertyGroupCallCount   = 0;
		uiUnRegisterPropertyGroupCallCount = 0;
		uiAddCategoryCallCount             = 0;
	}
    ~TestPropertyManager()
    {
        property_manager::m_instance.reset();
    }
	virtual property_group* select_single_node(property_group* pCurrentPropertyGroup, const string& xpath)
	{
		++uiSelectSingleNodeCallCount;

		return 0;
	}
	virtual void register_property_group( property_group *pPropertyGroup )
	{
		++uiRegisterPropertyGroupCallCount;

		if( NULL == pPropertyGroup->get_parent() )
		{
			++uiChildCount;
		}
	}
	virtual void UnRegisterPropertyGroup( property_group *pPropertyGroup )
	{
		++uiUnRegisterPropertyGroupCallCount;

		if( NULL == pPropertyGroup->get_parent() )
		{
			--uiChildCount;
		}
	}
	virtual void AddCategory( const string &sCategoryName, property_group *pPropertyGroup )
	{
		++uiAddCategoryCallCount;
	}

	unsigned int uiChildCount;
	unsigned int uiSelectSingleNodeCallCount;
	unsigned int uiRegisterPropertyGroupCallCount;
	unsigned int uiUnRegisterPropertyGroupCallCount;
	unsigned int uiAddCategoryCallCount;
};

BOOST_AUTO_TEST_CASE( TestPropertyParent )
{
	TestPropertyGroup testGroup;
	
	BOOST_CHECK( testGroup.get_parent() == NULL );
	BOOST_CHECK( testGroup.m_child1.get_parent() == (property_group*)&testGroup );
}

BOOST_AUTO_TEST_CASE( TestPropertyGroupGetName )
{
	TestPropertyGroup testGroup;
	
	BOOST_CHECK( testGroup.get_name() == PROPERTY_GROUP_NAME );
}

BOOST_AUTO_TEST_CASE( TestBogusPropertyNameForEditor )
{
	TestPropertyGroup_4 testGroup;
	
	BOOST_CHECK_THROW( testGroup.get_metadata( "bogus property name" ), std::out_of_range );
}

BOOST_AUTO_TEST_CASE( TestDynamicAddingAndReadingOfPropertyGroups )
{
	// TODO: implement test for dynamic property groups
}

BOOST_AUTO_TEST_CASE( TestNumberOfProperties )
{
	TestPropertyGroup_5 testGroup;
	
	BOOST_CHECK_EQUAL( testGroup.prop_count(), 7u );
}

BOOST_AUTO_TEST_CASE( TestPropertyIteration )
{
	TestPropertyGroup_5 testGroup;
	
	property_group::prop_iterator itr = testGroup.prop_begin();
    BOOST_CHECK_EQUAL( (*itr), PROPERTY_BOOL );
    ++itr;
    BOOST_CHECK_EQUAL( (*itr), PROPERTY_DOUBLE );
    ++itr;
    BOOST_CHECK_EQUAL( (*itr), PROPERTY_DOUBLE_2 );
    ++itr;
    BOOST_CHECK_EQUAL( (*itr), PROPERTY_DOUBLE_VAR );
    ++itr;
    BOOST_CHECK_EQUAL( (*itr), PROPERTY_FLOAT_1 );
    ++itr;
    BOOST_CHECK_EQUAL( (*itr), PROPERTY_INT_1 );
    ++itr;
    BOOST_CHECK_EQUAL( (*itr), PROPERTY_STRING_2 );
    ++itr;
    BOOST_CHECK( testGroup.prop_end() == itr );
}

BOOST_AUTO_TEST_CASE( TestPropertyGroupCategory )
{
	TestPropertyGroup_5 testGroup;
	
	testGroup.add_category( PROPERTY_GROUP_CATEGORY1 );
	testGroup.add_category( PROPERTY_GROUP_CATEGORY2 );
	testGroup.add_category( PROPERTY_GROUP_CATEGORY3 );
	testGroup.add_category( PROPERTY_GROUP_CATEGORY3 );	// duplicate should not be inserted

	BOOST_CHECK( testGroup.get_category_collection().size() == 4 );	// there is also an 'All' category which automatically gets added

	category_collection::const_iterator pos = find( testGroup.get_category_collection().begin(), testGroup.get_category_collection().end(), PROPERTY_GROUP_CATEGORY1 );
	BOOST_CHECK( *pos == PROPERTY_GROUP_CATEGORY1 );
	pos = find( testGroup.get_category_collection().begin(), testGroup.get_category_collection().end(), PROPERTY_GROUP_CATEGORY2 );
	BOOST_CHECK( *pos == PROPERTY_GROUP_CATEGORY2 );
	pos = find( testGroup.get_category_collection().begin(), testGroup.get_category_collection().end(), PROPERTY_GROUP_CATEGORY3 );
	BOOST_CHECK( *pos == PROPERTY_GROUP_CATEGORY3 );
	pos = find( testGroup.get_category_collection().begin(), testGroup.get_category_collection().end(), "bogus category" );
	BOOST_CHECK( pos == testGroup.get_category_collection().end() );
}


BOOST_AUTO_TEST_CASE( TestSingletonPropertyManager )
{
	TestPropertyManager manager;

    BOOST_CHECK( property_manager::instance() == &manager );
}

BOOST_AUTO_TEST_CASE( TestSetParent )
{
	TestPropertyManager manager;

	// The first item should parent to root
	TestPropertyChildGroup_1 rootGroup(NULL);
	BOOST_CHECK_EQUAL( manager.uiRegisterPropertyGroupCallCount, 1u );
	BOOST_CHECK_EQUAL( manager.uiUnRegisterPropertyGroupCallCount, 0u );
	BOOST_CHECK_EQUAL( manager.uiChildCount, 1u );

	// The second item should parent to the first
	TestPropertyChildGroup_1 childGroup(&rootGroup);
	BOOST_CHECK_EQUAL( manager.uiRegisterPropertyGroupCallCount, 2u );
	BOOST_CHECK_EQUAL( manager.uiUnRegisterPropertyGroupCallCount, 0u );
	BOOST_CHECK_EQUAL( manager.uiChildCount, 1u );
	BOOST_CHECK_EQUAL( rootGroup.get_children_collection().size(), 1u );
	BOOST_CHECK( &rootGroup == childGroup.get_parent() );

	// Reparent child to root
	childGroup.set_parent(NULL);
	BOOST_CHECK_EQUAL( manager.uiRegisterPropertyGroupCallCount, 3u );
	BOOST_CHECK_EQUAL( manager.uiUnRegisterPropertyGroupCallCount, 1u );
	BOOST_CHECK_EQUAL( manager.uiChildCount, 2u );
	BOOST_CHECK_EQUAL( rootGroup.get_children_collection().size(), 0u );
	BOOST_CHECK( NULL == childGroup.get_parent() );

	// Reparent child to rootGroup
	childGroup.set_parent(&rootGroup);
	BOOST_CHECK_EQUAL( manager.uiRegisterPropertyGroupCallCount, 4u );
	BOOST_CHECK_EQUAL( manager.uiUnRegisterPropertyGroupCallCount, 2u );
	BOOST_CHECK_EQUAL( manager.uiChildCount, 1u );
	BOOST_CHECK_EQUAL( rootGroup.get_children_collection().size(), 1u );
	BOOST_CHECK( &rootGroup == childGroup.get_parent() );
}

BOOST_AUTO_TEST_CASE( TestAutoReparentToRootInDestructor )
{
	TestPropertyManager manager;

	// The first item should parent to root
	TestPropertyChildGroup_1 *pRootGroup = new TestPropertyChildGroup_1(NULL);
	BOOST_CHECK_EQUAL( manager.uiRegisterPropertyGroupCallCount, 1u );
	BOOST_CHECK_EQUAL( manager.uiUnRegisterPropertyGroupCallCount, 0u );
	BOOST_CHECK_EQUAL( manager.uiChildCount, 1u );

	// The second item should parent to the first
	TestPropertyChildGroup_1 childGroup(pRootGroup);
	BOOST_CHECK_EQUAL( manager.uiRegisterPropertyGroupCallCount, 2u );
	BOOST_CHECK_EQUAL( manager.uiUnRegisterPropertyGroupCallCount, 0u );
	BOOST_CHECK_EQUAL( manager.uiChildCount, 1u );
	BOOST_CHECK_EQUAL( pRootGroup->get_children_collection().size(), 1u );
	BOOST_CHECK( pRootGroup == childGroup.get_parent() );

	// Delete rootGroup.
	// This should cause childGroup to be reparented to root.
	delete pRootGroup;
	BOOST_CHECK_EQUAL( manager.uiRegisterPropertyGroupCallCount, 3u );
	BOOST_CHECK_EQUAL( manager.uiUnRegisterPropertyGroupCallCount, 2u );
	BOOST_CHECK_EQUAL( manager.uiChildCount, 1u );
	BOOST_CHECK( NULL == childGroup.get_parent() );
}

// This is counter intuitive, but by default, if an object inherits from property_group,
// adds properties, and that object doesn't have a copy constructor, it is important that
// the old properties aren't copied over. This is because the property_group object can't
// properly re-hook the function pointers up to the new object. This stops crashes in
// the system, though the object should provide it's own copy constructor to add the new
// properties in which case this wouldn't be an issue.
BOOST_AUTO_TEST_CASE( TestNoCopyConstructorProperties )
{
	TestPropertyGroup_5 testGroup;
	// For this test, TestPropertyGroup_5 shouldn't have a copy constructor
	TestPropertyGroup_5 copiedGroup( testGroup );

	BOOST_CHECK_EQUAL( copiedGroup.prop_count(), 0u );

	// This is here, because if this test fails, and we don't clear
	// the properties, and the test app can hang when copiedGroup is destroyed.
	testGroup.clear_properties();
}

// This is counter intuitive, but by default, if an object inherits from property_group,
// adds actions, and that object doesn't have a copy constructor, it is important that
// the old actions aren't copied over. This is because the property_group object can't
// properly re-hook the function pointers up to the new object. This stops crashes in
// the system, though the object should provide it's own copy constructor to add the new
// actions in which case this wouldn't be an issue.
BOOST_AUTO_TEST_CASE( TestNoCopyConstructorActions )
{
	TestActionsFixture testGroup;
	// For this test, TestActionsFixture shouldn't have a copy constructor
	TestActionsFixture copiedGroup( testGroup );

	BOOST_CHECK_EQUAL( copiedGroup.action_count(), 0u );

	// This is here, because if this test fails, and we don't clear
	// the actions, the test app will hang when copiedGroup is destroyed.
	testGroup.clear_actions();
}

BOOST_AUTO_TEST_CASE( TestCopyConstructor_RootObject )
{
	TestPropertyGroup_5 testGroup;
	TestPropertyGroup_5 copiedGroup( testGroup );

	BOOST_CHECK_EQUAL( copiedGroup.get_name().compare(PROPERTY_GROUP_NAME), 0 );
	BOOST_CHECK( NULL == copiedGroup.get_parent() );

	// This is here, because if Copy Constructor isn't right, and we don't clear
	// the properties, the test app will hang when copied Group is destroyed.
	testGroup.clear_properties();
}

BOOST_AUTO_TEST_CASE( TestCopyConstructor_ChildObject )
{
	TestPropertyGroup_5 testGroup;
	TestPropertyChildGroup_1 group(&testGroup);
	TestPropertyChildGroup_1 copiedGroup(&group);

	BOOST_CHECK_EQUAL( copiedGroup.get_name().compare(PROPERTY_GROUP_CHILD_NAME), 0 );
	BOOST_CHECK( &testGroup == copiedGroup.get_parent() );
}

BOOST_AUTO_TEST_CASE( TestIsReadOnly )
{
	TestPropertyGroup testGroup;
	
	BOOST_CHECK_EQUAL( testGroup.is_read_only(PROPERTY_STRING_1), true );
	BOOST_CHECK_EQUAL( testGroup.is_read_only(PROPERTY_FLOAT_1), false );
}
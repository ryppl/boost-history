// Boost.Print library

// Copyright Jared McIntyre 2007. Use, modification and
// distribution is subject to the Boost Software License, Version
// 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

// For more information, see http://www.boost.org

#include "TestClassesAndConstants.h"

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
        property_manager::m_instance.release();
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

TEST_FIXTURE( TestPropertyGroup, TestPropertyParent )
{
	BOOST_CHECK( get_parent() == NULL );
	BOOST_CHECK( m_child1.get_parent() == (property_group*)this );
}

TEST_FIXTURE( TestPropertyGroup, TestPropertyGroupGetName )
{
	BOOST_CHECK( get_name() == PROPERTY_GROUP_NAME );
}

TEST_FIXTURE( TestPropertyGroup_4, TestBogusPropertyNameForEditor )
{
	CHECK_THROW( const Editor *pEditor = get_metadata( "bogus property name" ), std::out_of_range );
}

BOOST_TEST( TestDynamicAddingAndReadingOfPropertyGroups )
{
	// TODO: implement test for dynamic property groups
}

TEST_FIXTURE( TestPropertyGroup_5, TestNumberOfProperties )
{
	BOOST_CHECK_EQUAL( prop_count(), 7 );
}

TEST_FIXTURE( TestPropertyGroup_5, TestPropertyIteration )
{
	prop_iterator itr = prop_begin();
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
    BOOST_CHECK( prop_end() == itr );
}

TEST_FIXTURE( TestPropertyGroup_5, TestPropertyGroupCategory )
{
	AddCategory( PROPERTY_GROUP_CATEGORY1 );
	AddCategory( PROPERTY_GROUP_CATEGORY2 );
	AddCategory( PROPERTY_GROUP_CATEGORY3 );
	AddCategory( PROPERTY_GROUP_CATEGORY3 );	// duplicate should not be inserted

	BOOST_CHECK( get_category_collection().size() == 4 );	// there is also an 'All' category which automatically gets added

	CategoryCollection::const_iterator pos = find( get_category_collection().begin(), get_category_collection().end(), PROPERTY_GROUP_CATEGORY1 );
	BOOST_CHECK( *pos == PROPERTY_GROUP_CATEGORY1 );
	pos = find( get_category_collection().begin(), get_category_collection().end(), PROPERTY_GROUP_CATEGORY2 );
	BOOST_CHECK( *pos == PROPERTY_GROUP_CATEGORY2 );
	pos = find( get_category_collection().begin(), get_category_collection().end(), PROPERTY_GROUP_CATEGORY3 );
	BOOST_CHECK( *pos == PROPERTY_GROUP_CATEGORY3 );
	pos = find( get_category_collection().begin(), get_category_collection().end(), "bogus category" );
	BOOST_CHECK( pos == get_category_collection().end() );
}


TEST_FIXTURE( TestPropertyGroup_5, TestSingletonPropertyManager )
{
	TestPropertyManager manager;

    BOOST_CHECK( property_manager::Instance() == &manager );
}

BOOST_TEST( TestSetParent )
{
	TestPropertyManager manager;

	// The first item should parent to root
	TestPropertyChildGroup_1 rootGroup(NULL);
	BOOST_CHECK_EQUAL( manager.uiRegisterPropertyGroupCallCount, 1 );
	BOOST_CHECK_EQUAL( manager.uiUnRegisterPropertyGroupCallCount, 0 );
	BOOST_CHECK_EQUAL( manager.uiChildCount, 1 );

	// The second item should parent to the first
	TestPropertyChildGroup_1 childGroup(&rootGroup);
	BOOST_CHECK_EQUAL( manager.uiRegisterPropertyGroupCallCount, 2 );
	BOOST_CHECK_EQUAL( manager.uiUnRegisterPropertyGroupCallCount, 0 );
	BOOST_CHECK_EQUAL( manager.uiChildCount, 1 );
	BOOST_CHECK_EQUAL( rootGroup.get_children_collection().size(), 1 );
	BOOST_CHECK( &rootGroup == childGroup.get_parent() );

	// Reparent child to root
	childGroup.set_parent(NULL);
	BOOST_CHECK_EQUAL( manager.uiRegisterPropertyGroupCallCount, 3 );
	BOOST_CHECK_EQUAL( manager.uiUnRegisterPropertyGroupCallCount, 1 );
	BOOST_CHECK_EQUAL( manager.uiChildCount, 2 );
	BOOST_CHECK_EQUAL( rootGroup.get_children_collection().size(), 0 );
	BOOST_CHECK( NULL == childGroup.get_parent() );

	// Reparent child to rootGroup
	childGroup.set_parent(&rootGroup);
	BOOST_CHECK_EQUAL( manager.uiRegisterPropertyGroupCallCount, 4 );
	BOOST_CHECK_EQUAL( manager.uiUnRegisterPropertyGroupCallCount, 2 );
	BOOST_CHECK_EQUAL( manager.uiChildCount, 1 );
	BOOST_CHECK_EQUAL( rootGroup.get_children_collection().size(), 1 );
	BOOST_CHECK( &rootGroup == childGroup.get_parent() );
}

BOOST_TEST( TestAutoReparentToRootInDestructor )
{
	TestPropertyManager manager;

	// The first item should parent to root
	TestPropertyChildGroup_1 *pRootGroup = new TestPropertyChildGroup_1(NULL);
	BOOST_CHECK_EQUAL( manager.uiRegisterPropertyGroupCallCount, 1 );
	BOOST_CHECK_EQUAL( manager.uiUnRegisterPropertyGroupCallCount, 0 );
	BOOST_CHECK_EQUAL( manager.uiChildCount, 1 );

	// The second item should parent to the first
	TestPropertyChildGroup_1 childGroup(pRootGroup);
	BOOST_CHECK_EQUAL( manager.uiRegisterPropertyGroupCallCount, 2 );
	BOOST_CHECK_EQUAL( manager.uiUnRegisterPropertyGroupCallCount, 0 );
	BOOST_CHECK_EQUAL( manager.uiChildCount, 1 );
	BOOST_CHECK_EQUAL( pRootGroup->getChildrenCollection().size(), 1 );
	BOOST_CHECK( pRootGroup == childGroup.get_parent() );

	// Delete rootGroup.
	// This should cause childGroup to be reparented to root.
	delete pRootGroup;
	BOOST_CHECK_EQUAL( manager.uiRegisterPropertyGroupCallCount, 3 );
	BOOST_CHECK_EQUAL( manager.uiUnRegisterPropertyGroupCallCount, 2 );
	BOOST_CHECK_EQUAL( manager.uiChildCount, 1 );
	BOOST_CHECK( NULL == childGroup.get_parent() );
}

// This is counter intuitive, but by default, if an object inherits from property_group,
// adds properties, and that object doesn't have a copy constructor, it is important that
// the old properties aren't copied over. This is because the property_group object can't
// properly re-hook the function pointers up to the new object. This stops crashes in
// the system, though the object should provide it's own copy constructor to add the new
// properties in which case this wouldn't be an issue.
TEST_FIXTURE( TestPropertyGroup_5, TestNoCopyConstructorProperties )
{
	// For this test, TestPropertyGroup_5 shouldn't have a copy constructor
	TestPropertyGroup_5 copiedGroup( *this );

	BOOST_CHECK_EQUAL( copiedGroup.prop_count(), 0 );

	// This is here, because if this test fails, and we don't clear
	// the properties, the tes app will hang when copiedGroup is destroyed.
	m_properties.clear();
}

// This is counter intuitive, but by default, if an object inherits from property_group,
// adds actions, and that object doesn't have a copy constructor, it is important that
// the old actions aren't copied over. This is because the property_group object can't
// properly re-hook the function pointers up to the new object. This stops crashes in
// the system, though the object should provide it's own copy constructor to add the new
// actions in which case this wouldn't be an issue.
TEST_FIXTURE( TestActionsFixture, TestNoCopyConstructorActions )
{
	// For this test, TestActionsFixture shouldn't have a copy constructor
	TestActionsFixture copiedGroup( *this );

	BOOST_CHECK_EQUAL( copiedGroup.action_count(), 0 );

	// This is here, because if this test fails, and we don't clear
	// the actions, the test app will hang when copiedGroup is destroyed.
	m_actions.clear();
}

TEST_FIXTURE( TestPropertyGroup_5, TestCopyConstructor_RootObject )
{
	TestPropertyGroup_5 copiedGroup( *this );

	BOOST_CHECK_EQUAL( copiedGroup.get_name().compare(PROPERTY_GROUP_NAME), 0 );
	BOOST_CHECK( NULL == copiedGroup.get_parent() );

	// This is here, because if Copy Constructor isn't right, and we don't clear
	// the properties, the test app will hang when copied Group is destroyed.
	m_properties.clear();
}

TEST_FIXTURE( TestPropertyGroup_5, TestCopyConstructor_ChildObject )
{
	TestPropertyChildGroup_1 group(this);
	TestPropertyChildGroup_1 copiedGroup(group);

	BOOST_CHECK_EQUAL( copiedGroup.get_name().compare(PROPERTY_GROUP_CHILD_NAME), 0 );
	BOOST_CHECK( this == copiedGroup.get_parent() );
}

TEST_FIXTURE( TestPropertyGroup, TestIsReadOnly )
{
	BOOST_CHECK_EQUAL( IsReadOnly(PROPERTY_STRING_1), true );
	BOOST_CHECK_EQUAL( IsReadOnly(PROPERTY_FLOAT_1), false );
}
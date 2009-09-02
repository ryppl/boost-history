// Boost.Pinhole library

// Copyright Jared McIntyre 2007-2009. Use, modification and
// distribution is subject to the Boost Software License, Version
// 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

// For more information, see http://www.boost.org

#define BOOST_TEST_MODULE PinholeLib

#include <boost/config.hpp>
#include <boost/bind.hpp>
#include <boost/function.hpp>
#include <boost/test/unit_test.hpp>
#include <boost/pinhole/property_group.hpp>
#include <boost/pinhole/property_manager.hpp>

using namespace std;
using namespace boost;
using namespace boost::pinhole;

#define PROPERTY_GROUP_NAME ("TestPropertyGroup")
#define PROPERTY_GROUP_CATEGORY1 ("MyPropertyGroupCategory1")
#define PROPERTY_GROUP_CATEGORY2 ("MyPropertyGroupCategory2")
#define PROPERTY_GROUP_CATEGORY3 ("MyPropertyGroupCategory3")
#define PROPERTY_GROUP_CHILD_NAME ("TestPropertyChildGroup")

#define PROP_STRING ("PropString")
#define PROP_STRING_VALUE ("PropString_Value")

#define PROP_FLOAT ("PropFloat")
#define PROP_FLOAT_VALUE ( 3.14161f )
#define PROP_FLOAT_STRING_VALUE ( "3.14161" )

#define PROP_INT ("PropInt")
#define PROP_INT_VALUE (54321)

#define PROPERTY_BOOL ("PropBool")
#define PROPERTY_BOOL_VALUE ("True")

class TestPropertyChildGroup : public property_group
{
public:
    TestPropertyChildGroup( property_group *pParentGroup) : property_group( PROPERTY_GROUP_CHILD_NAME, pParentGroup )
    {
        add_property(PROP_FLOAT,  BOOST_SETTER(&TestPropertyChildGroup::SetFloat),  BOOST_GETTER(&TestPropertyChildGroup::GetFloat));
        add_property(PROP_INT,    BOOST_SETTER(&TestPropertyChildGroup::SetInt),    BOOST_GETTER(&TestPropertyChildGroup::GetInt));
        add_property(PROP_STRING, BOOST_SETTER(&TestPropertyChildGroup::SetString), BOOST_GETTER(&TestPropertyChildGroup::GetString));
    
        add_category(PROPERTY_GROUP_CATEGORY1);
        add_category(PROPERTY_GROUP_CATEGORY2);
        add_category(PROPERTY_GROUP_CATEGORY3);
    }

    float GetFloat() const{return( m_float );}
    void SetFloat( float fValue ){m_float = fValue;}
    int GetInt() const{return( m_int );}
    void SetInt( int iValue ){m_int = iValue;}
    string GetString(){return( m_string );}
    void SetString( string strValue ){m_string = strValue;}

    float m_float;
    int m_int;
    string m_string;
};


class TestPropertyGroup : public property_group
{
public:

    TestPropertyGroup() : property_group( PROPERTY_GROUP_NAME, NULL ), m_child1( NULL ), m_child2( NULL )
    {
        m_child1.set_parent( this );
        m_child2.set_parent( this );

        m_bool_var = false;

        add_property(PROP_STRING,   BOOST_SETTER(&TestPropertyGroup::SetString), BOOST_GETTER(&TestPropertyGroup::GetString));
        add_property(PROP_FLOAT,    BOOST_SETTER(&TestPropertyGroup::SetFloat),  BOOST_GETTER(&TestPropertyGroup::GetFloat));
        add_property(PROP_INT,      BOOST_SETTER(&TestPropertyGroup::SetInt),    BOOST_GETTER(&TestPropertyGroup::GetInt));
        add_property(PROPERTY_BOOL, BOOST_SETTER(&TestPropertyGroup::SetBool),   BOOST_GETTER(&TestPropertyGroup::GetBool));

        add_category(PROPERTY_GROUP_CATEGORY1);
        add_category(PROPERTY_GROUP_CATEGORY2);
        add_category(PROPERTY_GROUP_CATEGORY3);
    }

    TestPropertyChildGroup m_child1;
    TestPropertyChildGroup m_child2;

    float GetFloat() const{return( m_float );}
    void SetFloat( float fValue ){m_float = fValue;}
    int GetInt() const{return( m_int );}
    void SetInt( int iValue ){m_int = iValue;}
    string GetString(){return( m_string );}
    void SetString( string strValue ){m_string = strValue;}
    void SetBool( bool b ){ m_bool = b; }
    bool GetBool(){ return( m_bool ); }

private:
    float m_float;
    int m_int;
    string m_string;
    bool m_bool;

    float m_float_var;
    int m_int_var;
    string m_string_var;
    bool m_bool_var;
};

class TestPropertyManager : public property_manager
{
public:
    TestPropertyManager() : property_manager()
    {
        property_manager::internal_instance().reset( this );
            
        uiChildCount                       = 0;
        uiRegisterPropertyGroupCallCount   = 0;
        uiUnRegisterPropertyGroupCallCount = 0;
    }
    virtual void register_property_group( property_group *pPropertyGroup )
    {
        ++uiRegisterPropertyGroupCallCount;

        property_manager::register_property_group( pPropertyGroup );
    }
    virtual void unregister_property_group( property_group *pPropertyGroup )
    {
        ++uiUnRegisterPropertyGroupCallCount;

        property_manager::unregister_property_group( pPropertyGroup );
    }


    virtual void add_property_group( property_group *group )
    {
        ++uiChildCount;

        property_manager::add_property_group( group );
    }
    virtual void remove_property_group( property_group *group )
    {
        --uiChildCount;

        m_property_group_collection.remove( group );
    }
    virtual void add_category( const string &sCategoryName )
    {
        ++uiAddCategoryCallCount;

        property_manager::add_category( sCategoryName );
    }

    unsigned int uiChildCount;
    unsigned int uiRegisterPropertyGroupCallCount;
    unsigned int uiUnRegisterPropertyGroupCallCount;
    unsigned int uiAddCategoryCallCount;
};

class TestPropertyManagerGuard
{
    public:
        TestPropertyManagerGuard()
        {
            property_manager::delete_instance();
            
            p_manager = new TestPropertyManager();
        }
        
        ~TestPropertyManagerGuard()
        {
            property_manager::delete_instance();
        }
        
        TestPropertyManager *p_manager;
};

BOOST_AUTO_TEST_CASE( TestPropertyParen9t )
{
    TestPropertyGroup testGroup;
    
    BOOST_CHECK( NULL == testGroup.get_parent() );
    BOOST_CHECK( testGroup.m_child1.get_parent() == (property_group*)&testGroup );
}

BOOST_AUTO_TEST_CASE( TestPropertyGroupGetName )
{
    TestPropertyGroup testGroup;
    
    BOOST_CHECK_EQUAL( PROPERTY_GROUP_NAME, testGroup.get_name() );
}

BOOST_AUTO_TEST_CASE( TestBogusPropertyNameForEditor )
{
    TestPropertyGroup testGroup;
    
    BOOST_CHECK_THROW( testGroup.get_metadata( "bogus property name" ), std::out_of_range );
}

BOOST_AUTO_TEST_CASE( TestDynamicAddingAndReadingOfPropertyGroups )
{
    // TODO: implement test for dynamic property groups
}

BOOST_AUTO_TEST_CASE( TestNumberOfProperties )
{
    bool test_bool(false);
    property_group testGroup("TEST_GROUP", NULL);
    
    BOOST_CHECK_EQUAL( testGroup.prop_count(), 0u );

    testGroup.add_property("Test1", BOOST_SETTER_NONE, BOOST_GETTER_VAR(test_bool));
    BOOST_CHECK_EQUAL( testGroup.prop_count(), 1u );

    testGroup.add_property("Test2", BOOST_SETTER_NONE, BOOST_GETTER_VAR(test_bool));
    BOOST_CHECK_EQUAL( testGroup.prop_count(), 2u );

    testGroup.add_property("Test3", BOOST_SETTER_NONE, BOOST_GETTER_VAR(test_bool));
    BOOST_CHECK_EQUAL( testGroup.prop_count(), 3u );

    testGroup.add_property("Test4", BOOST_SETTER_NONE, BOOST_GETTER_VAR(test_bool));
    BOOST_CHECK_EQUAL( testGroup.prop_count(), 4u );

    testGroup.add_property("Test5", BOOST_SETTER_NONE, BOOST_GETTER_VAR(test_bool));
    BOOST_CHECK_EQUAL( testGroup.prop_count(), 5u );
}

BOOST_AUTO_TEST_CASE( TestPropertyIteration )
{
    TestPropertyGroup testGroup;
    
    // Note: This is a map, the order is alphabetical on string name

    property_group::prop_iterator itr = testGroup.prop_begin();
    BOOST_CHECK_EQUAL( (*itr), PROPERTY_BOOL );
    ++itr;
    BOOST_CHECK_EQUAL( (*itr), PROP_FLOAT );
    ++itr;
    BOOST_CHECK_EQUAL( (*itr), PROP_INT );
    ++itr;
    BOOST_CHECK_EQUAL( (*itr), PROP_STRING );
    ++itr;
    BOOST_CHECK( testGroup.prop_end() == itr );
}

BOOST_AUTO_TEST_CASE( TestPropertyGroupCategory )
{
    property_group testGroup("TEST_GROUP", NULL);
    
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
    TestPropertyManagerGuard gaurd;
    TestPropertyManager *p_manager = gaurd.p_manager;

    BOOST_CHECK( property_manager::instance().get() == p_manager );
}

BOOST_AUTO_TEST_CASE( TestSetParent )
{
    TestPropertyManagerGuard gaurd;
    TestPropertyManager *p_manager = gaurd.p_manager;

    // The first item should parent to root
    TestPropertyChildGroup rootGroup(NULL);
    BOOST_CHECK_EQUAL( p_manager->uiRegisterPropertyGroupCallCount, 1u );
    BOOST_CHECK_EQUAL( p_manager->uiUnRegisterPropertyGroupCallCount, 0u );
    BOOST_CHECK_EQUAL( p_manager->uiChildCount, 1u );

    // The second item should parent to the first
    TestPropertyChildGroup childGroup(&rootGroup);
    BOOST_CHECK_EQUAL( p_manager->uiRegisterPropertyGroupCallCount, 2u );
    BOOST_CHECK_EQUAL( p_manager->uiUnRegisterPropertyGroupCallCount, 0u );
    BOOST_CHECK_EQUAL( p_manager->uiChildCount, 1u );
    BOOST_CHECK_EQUAL( rootGroup.get_children_collection().size(), 1u );
    BOOST_CHECK( &rootGroup == childGroup.get_parent() );

    // Reparent child to root
    childGroup.set_parent(NULL);
    BOOST_CHECK_EQUAL( p_manager->uiRegisterPropertyGroupCallCount, 3u );
    BOOST_CHECK_EQUAL( p_manager->uiUnRegisterPropertyGroupCallCount, 1u );
    BOOST_CHECK_EQUAL( p_manager->uiChildCount, 2u );
    BOOST_CHECK_EQUAL( rootGroup.get_children_collection().size(), 0u );
    BOOST_CHECK( NULL == childGroup.get_parent() );

    // Reparent child to rootGroup
    childGroup.set_parent(&rootGroup);
    BOOST_CHECK_EQUAL( p_manager->uiRegisterPropertyGroupCallCount, 4u );
    BOOST_CHECK_EQUAL( p_manager->uiUnRegisterPropertyGroupCallCount, 2u );
    BOOST_CHECK_EQUAL( p_manager->uiChildCount, 1u );
    BOOST_CHECK_EQUAL( rootGroup.get_children_collection().size(), 1u );
    BOOST_CHECK( &rootGroup == childGroup.get_parent() );
}

BOOST_AUTO_TEST_CASE( TestAutoReparentToRootInDestructor )
{
    TestPropertyManagerGuard gaurd;
    TestPropertyManager *p_manager = gaurd.p_manager;

    // The first item should parent to root
    TestPropertyChildGroup *pRootGroup = new TestPropertyChildGroup(NULL);
    BOOST_CHECK_EQUAL( p_manager->uiRegisterPropertyGroupCallCount, 1u );
    BOOST_CHECK_EQUAL( p_manager->uiUnRegisterPropertyGroupCallCount, 0u );
    BOOST_CHECK_EQUAL( p_manager->uiChildCount, 1u );

    // The second item should parent to the first
    TestPropertyChildGroup childGroup(pRootGroup);
    BOOST_CHECK_EQUAL( p_manager->uiRegisterPropertyGroupCallCount, 2u );
    BOOST_CHECK_EQUAL( p_manager->uiUnRegisterPropertyGroupCallCount, 0u );
    BOOST_CHECK_EQUAL( p_manager->uiChildCount, 1u );
    BOOST_CHECK_EQUAL( pRootGroup->get_children_collection().size(), 1u );
    BOOST_CHECK( pRootGroup == childGroup.get_parent() );

    // Delete rootGroup.
    // This should cause childGroup to be reparented to root.
    delete pRootGroup;
    BOOST_CHECK_EQUAL( p_manager->uiRegisterPropertyGroupCallCount, 3u );
    BOOST_CHECK_EQUAL( p_manager->uiUnRegisterPropertyGroupCallCount, 2u );
    BOOST_CHECK_EQUAL( p_manager->uiChildCount, 1u );
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
    bool test_bool(false);

    // For this test, testGroup shouldn't have a copy constructor
    property_group testGroup("TEST_GROUP", NULL);
    testGroup.add_property("Test1", BOOST_SETTER_NONE, BOOST_GETTER_VAR(test_bool));
    testGroup.add_property("Test2", BOOST_SETTER_NONE, BOOST_GETTER_VAR(test_bool));
    BOOST_CHECK_EQUAL( testGroup.prop_count(), 2u );

    property_group copiedGroup( testGroup );
    BOOST_CHECK_EQUAL( copiedGroup.prop_count(), 0u );
}

void NullOpFunction(){;}

// This is counter intuitive, but by default, if an object inherits from property_group,
// adds actions, and that object doesn't have a copy constructor, it is important that
// the old actions aren't copied over. This is because the property_group object can't
// properly re-hook the function pointers up to the new object. This stops crashes in
// the system, though the object should provide it's own copy constructor to add the new
// actions in which case this wouldn't be an issue.
BOOST_AUTO_TEST_CASE( TestNoCopyConstructorActions )
{
    // For this test, testGroup shouldn't have a copy constructor
    property_group testGroup("TEST_GROUP", NULL);
    testGroup.add_action("Test1", boost::bind(NullOpFunction));
    testGroup.add_action("Test2", boost::bind(NullOpFunction));
    BOOST_CHECK_EQUAL( testGroup.action_count(), 2u );

    property_group copiedGroup( testGroup );
    BOOST_CHECK_EQUAL( copiedGroup.action_count(), 0u );
}

BOOST_AUTO_TEST_CASE( TestCopyConstructor_RootObject )
{
    property_group testGroup("TEST_GROUP", NULL);
    property_group copiedGroup( testGroup );

    BOOST_CHECK_EQUAL( "TEST_GROUP", copiedGroup.get_name() );
    BOOST_CHECK( NULL == copiedGroup.get_parent() );
}

BOOST_AUTO_TEST_CASE( TestCopyConstructor_ChildObject )
{
    property_group testGroup("TEST_GROUP", NULL);
    property_group childGroup("TEST_CHILD_GROUP", &testGroup);
    property_group copiedGroup( testGroup );

    BOOST_CHECK_EQUAL( copiedGroup.get_name(), "TEST_GROUP" );
    BOOST_CHECK( NULL == copiedGroup.get_parent() );
}

BOOST_AUTO_TEST_CASE( TestIsReadOnly )
{
    bool test_bool(false);

    property_group testGroup("TEST_GROUP", NULL);
    testGroup.add_property("read_only", BOOST_SETTER_NONE,           BOOST_GETTER_VAR(test_bool));
    testGroup.add_property("writable",  BOOST_SETTER_VAR(test_bool), BOOST_GETTER_VAR(test_bool));
    
    BOOST_CHECK_EQUAL( testGroup.is_read_only("read_only"), true );
    BOOST_CHECK_EQUAL( testGroup.is_read_only("writable"),  false );

    BOOST_CHECK_THROW( testGroup.set("read_only", false), boost::bad_function_call);
}

BOOST_AUTO_TEST_CASE( TestAddDeletePropertyGroupsInManager )
{
    TestPropertyManagerGuard gaurd;
    TestPropertyManager *p_manager = gaurd.p_manager;

    BOOST_CHECK_EQUAL( 0u, p_manager->count() );

    TestPropertyGroup *p_pg1 = new TestPropertyGroup();
    TestPropertyGroup *p_pg2 = new TestPropertyGroup();

    BOOST_CHECK_EQUAL( 2u, p_manager->count() );

    delete( p_pg1 );

    BOOST_CHECK_EQUAL( 1u, p_manager->count() );

    delete( p_pg2 );

    BOOST_CHECK_EQUAL( 0u, p_manager->count() );
}

BOOST_AUTO_TEST_CASE( TestCategoryCollectionInManager )
{
    TestPropertyManagerGuard gaurd;
    TestPropertyManager *p_manager = gaurd.p_manager;

    const category_collection *pCategoryCollection = p_manager->get_category_collection();

    TestPropertyGroup pg1;
    TestPropertyGroup pg2;

    BOOST_CHECK_EQUAL( 4u, pCategoryCollection->size() );

    BOOST_CHECK( pCategoryCollection->end() != pCategoryCollection->find( "All" ) );
    BOOST_CHECK_EQUAL( 2u, p_manager->count("All") );

    BOOST_CHECK( pCategoryCollection->end() != pCategoryCollection->find( PROPERTY_GROUP_CATEGORY1 ) );
    BOOST_CHECK_EQUAL( 2u, p_manager->count(PROPERTY_GROUP_CATEGORY1) );

    BOOST_CHECK( pCategoryCollection->end() != pCategoryCollection->find( PROPERTY_GROUP_CATEGORY2 ) );
    BOOST_CHECK_EQUAL( 2u, p_manager->count(PROPERTY_GROUP_CATEGORY2) );

    BOOST_CHECK( pCategoryCollection->end() != pCategoryCollection->find( PROPERTY_GROUP_CATEGORY3 ) );
    BOOST_CHECK_EQUAL( 2u, p_manager->count(PROPERTY_GROUP_CATEGORY3) );
}

#pragma once

#define BOOST_TEST_MODULE PinholeLib
#include <boost/test/unit_test.hpp>
#include "TestClassesAndConstants.h"
#include <boost/pinhole/PropertySerializer.h>
#include <fstream>

// I can hide these two line if I don't do everything in headers
boost::shared_ptr<property_manager> property_manager::m_instance(new property_manager);
event_source* event_source::m_instance = 0;

#define FILE_PATH "C:\\TestOutput.xml"

using namespace MscProperty;

class PropertyManagerResetter : public property_manager
{
public:
    ~PropertyManagerResetter()
    {
        property_manager::DeleteInstance();
    }
};

BOOST_AUTO_TEST_CASE( TestSerializer_Serializer )
{
    PropertyManagerResetter resetter;

    CoInitialize(NULL);

	TestPropertySerializer *pRootGroup = new TestPropertySerializer(NULL);
	TestPropertySerializer childGroup(pRootGroup);

    PropertySerializer serializer(FILE_PATH);
    serializer.Serialize();
    serializer.Deserialize();

    std::stringstream strToCompare;
    strToCompare << "<prop_serialization>" << std::endl
                 << "<path value=\"/TestPropertySerializer.Name=aName\">" << std::endl
                 << "<property name=\"Name\" value=\"aName\" />" << std::endl
                 << "<property name=\"PropertyFloat1\" value=\"2.45\" />" << std::endl
                 << "<property name=\"PropertyInt1\" value=\"365\" />" << std::endl
                 << "<property name=\"PropertyString2\" value=\"BOOST_AUTO_TEST_CASE value\" />" << std::endl
                 << "</path>" << std::endl
                 << "<path value=\"/TestPropertySerializer.Name=aName/TestPropertySerializer.Name=aName\">" << std::endl
                 << "<property name=\"Name\" value=\"aName\" />" << std::endl
                 << "<property name=\"PropertyFloat1\" value=\"2.45\" />" << std::endl
                 << "<property name=\"PropertyInt1\" value=\"365\" />" << std::endl
                 << "<property name=\"PropertyString2\" value=\"BOOST_AUTO_TEST_CASE value\" />" << std::endl
                 << "</path>" << std::endl
                 << "</prop_serialization>";

    std::ifstream file(FILE_PATH);
    
    BOOST_CHECK(file.is_open());
    BOOST_CHECK(file.good());
    
    char c;
    stringstream toTest;
    while( file.get(c) )
    {
        toTest << c;
    }

    BOOST_CHECK_EQUAL( toTest.str(), strToCompare.str() );

    CoUninitialize();
}

BOOST_AUTO_TEST_CASE( TestSerializer_Deserializer )
{
    PropertyManagerResetter resetter;

    CoInitialize(NULL);

	TestPropertySerializer *pRootGroup = new TestPropertySerializer(NULL);
	TestPropertySerializer childGroup(pRootGroup);

    PropertySerializer serializer(FILE_PATH);
    serializer.Serialize();

    // Setup changes in root object

    BOOST_CHECK_EQUAL( pRootGroup->testGroup.get_as_string(PROPERTY_FLOAT_1), "2.45");
    BOOST_CHECK_EQUAL( pRootGroup->testGroup.get_as_string(PROPERTY_INT_1), "365");
    BOOST_CHECK_EQUAL( pRootGroup->testGroup.get_as_string(PROPERTY_STRING_2), "BOOST_AUTO_TEST_CASE value");

    pRootGroup->testGroup.set_as_string(PROPERTY_FLOAT_1, "3.33");
    pRootGroup->testGroup.set_as_string(PROPERTY_INT_1, "567");
    pRootGroup->testGroup.set_as_string(PROPERTY_STRING_2, "a different string");

    BOOST_CHECK_EQUAL( pRootGroup->testGroup.get_as_string(PROPERTY_FLOAT_1), "3.33");
    BOOST_CHECK_EQUAL( pRootGroup->testGroup.get_as_string(PROPERTY_INT_1), "567");
    BOOST_CHECK_EQUAL( pRootGroup->testGroup.get_as_string(PROPERTY_STRING_2), "a different string");

    // Setup changes in child object

    BOOST_CHECK_EQUAL( childGroup.testGroup.get_as_string(PROPERTY_FLOAT_1), "2.45");
    BOOST_CHECK_EQUAL( childGroup.testGroup.get_as_string(PROPERTY_INT_1), "365");
    BOOST_CHECK_EQUAL( childGroup.testGroup.get_as_string(PROPERTY_STRING_2), "BOOST_AUTO_TEST_CASE value");

    childGroup.testGroup.set_as_string(PROPERTY_FLOAT_1, "3.33");
    childGroup.testGroup.set_as_string(PROPERTY_INT_1, "567");
    childGroup.testGroup.set_as_string(PROPERTY_STRING_2, "a different string");

    BOOST_CHECK_EQUAL( childGroup.testGroup.get_as_string(PROPERTY_FLOAT_1), "3.33");
    BOOST_CHECK_EQUAL( childGroup.testGroup.get_as_string(PROPERTY_INT_1), "567");
    BOOST_CHECK_EQUAL( childGroup.testGroup.get_as_string(PROPERTY_STRING_2), "a different string");

    // Deserialize

    serializer.Deserialize();

    // Validate everything is reset

    BOOST_CHECK_EQUAL( pRootGroup->testGroup.get_as_string(PROPERTY_FLOAT_1), "2.45");
    BOOST_CHECK_EQUAL( pRootGroup->testGroup.get_as_string(PROPERTY_INT_1), "365");
    BOOST_CHECK_EQUAL( pRootGroup->testGroup.get_as_string(PROPERTY_STRING_2), "BOOST_AUTO_TEST_CASE value");

    BOOST_CHECK_EQUAL( childGroup.testGroup.get_as_string(PROPERTY_FLOAT_1), "2.45");
    BOOST_CHECK_EQUAL( childGroup.testGroup.get_as_string(PROPERTY_INT_1), "365");
    BOOST_CHECK_EQUAL( childGroup.testGroup.get_as_string(PROPERTY_STRING_2), "BOOST_AUTO_TEST_CASE value");

    CoUninitialize();
}

BOOST_AUTO_TEST_CASE( TestSerializer_DeserializerWherePathEqualsMultipleGroups )
{
    PropertyManagerResetter resetter;

    CoInitialize(NULL);

	TestPropertySerializer *pRootGroup = new TestPropertySerializer(NULL);
	TestPropertySerializer childGroup1(pRootGroup);
	TestPropertySerializer childGroup2(pRootGroup);

    PropertySerializer serializer(FILE_PATH);
    serializer.Serialize();

    // Setup changes in root object

    pRootGroup->testGroup.set_as_string(PROPERTY_FLOAT_1, "3.33");
    pRootGroup->testGroup.set_as_string(PROPERTY_INT_1, "567");
    pRootGroup->testGroup.set_as_string(PROPERTY_STRING_2, "a different string");

    // Setup changes in child object1

    childGroup1.testGroup.set_as_string(PROPERTY_FLOAT_1, "3.33");
    childGroup1.testGroup.set_as_string(PROPERTY_INT_1, "567");
    childGroup1.testGroup.set_as_string(PROPERTY_STRING_2, "a different string");

    // Setup changes in child object2

    childGroup2.testGroup.set_as_string(PROPERTY_FLOAT_1, "3.33");
    childGroup2.testGroup.set_as_string(PROPERTY_INT_1, "567");
    childGroup2.testGroup.set_as_string(PROPERTY_STRING_2, "a different string");

    // Deserialize

    serializer.Deserialize();

    // Validate that only the root item has reset. This is because the child groups
    // have matching paths and are thus ignored in deserialization.

    BOOST_CHECK_EQUAL( pRootGroup->testGroup.get_as_string(PROPERTY_FLOAT_1), "2.45");
    BOOST_CHECK_EQUAL( pRootGroup->testGroup.get_as_string(PROPERTY_INT_1), "365");
    BOOST_CHECK_EQUAL( pRootGroup->testGroup.get_as_string(PROPERTY_STRING_2), "BOOST_AUTO_TEST_CASE value");

    BOOST_CHECK_EQUAL( childGroup1.testGroup.get_as_string(PROPERTY_FLOAT_1), "3.33");
    BOOST_CHECK_EQUAL( childGroup1.testGroup.get_as_string(PROPERTY_INT_1), "567");
    BOOST_CHECK_EQUAL( childGroup1.testGroup.get_as_string(PROPERTY_STRING_2), "a different string");

    BOOST_CHECK_EQUAL( childGroup2.testGroup.get_as_string(PROPERTY_FLOAT_1), "3.33");
    BOOST_CHECK_EQUAL( childGroup2.testGroup.get_as_string(PROPERTY_INT_1), "567");
    BOOST_CHECK_EQUAL( childGroup2.testGroup.get_as_string(PROPERTY_STRING_2), "a different string");

    CoUninitialize();
}
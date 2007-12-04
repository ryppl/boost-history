// Boost.Pinhole library

// Copyright Jared McIntyre 2007. Use, modification and
// distribution is subject to the Boost Software License, Version
// 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

// For more information, see http://www.boost.org

#define BOOST_TEST_MODULE PinholeLib
#include <boost/test/unit_test.hpp>
#include "TestClassesAndConstants.hpp"
#include <boost/pinhole.hpp>

BOOST_AUTO_TEST_CASE( TestPropertyGroupWrapper_Bool )
{
	TestPropertyGroup_4 baseObject;
	property_group_wrapper wrapper("test_wrapper", &baseObject);
	const Editor *pEditor;

	// Test Standard Creation
	wrapper.add_property(PROPERTY_BOOL, "PropertyBool description", boost::bind(&TestPropertyGroup_4::SetBool, &baseObject, _1), boost::bind(&TestPropertyGroup_4::GetBool, &baseObject), new BoolEditor());

	wrapper.set_as_string( PROPERTY_BOOL, BOOL_FALSE );
	BOOST_CHECK( wrapper.get_as_string( PROPERTY_BOOL ) == BOOL_FALSE );
	
	wrapper.set_as_string( PROPERTY_BOOL, BOOL_TRUE );
	BOOST_CHECK( wrapper.get_as_string( PROPERTY_BOOL ) == BOOL_TRUE );

	pEditor = wrapper.get_metadata( PROPERTY_BOOL );
	BOOST_CHECK( pEditor->getEditorPropertyType() == BooleanType );
	BOOST_CHECK( pEditor->GetControlType() == Radio );

	// Test Auto-Generated Designer Creation
	property_group_wrapper wrapperAutoGenDesigner("test_wrapper", &baseObject);

	wrapperAutoGenDesigner.add_property(PROPERTY_BOOL, "PropertyBool description", boost::bind(&TestPropertyGroup_4::SetBool, &baseObject, _1), boost::bind(&TestPropertyGroup_4::GetBool, &baseObject) );

	pEditor = wrapperAutoGenDesigner.get_metadata(PROPERTY_BOOL);
	BOOST_CHECK( NULL != dynamic_cast<const BoolEditor*>(pEditor) );
}

BOOST_AUTO_TEST_CASE( TestPropertyGroupWrapper_Double )
{
	TestPropertyGroup_5 baseObject;
	property_group_wrapper wrapper("test_wrapper", &baseObject);
	const Editor *pEditor;

	// Test Standard Creation
	wrapper.add_property(PROPERTY_DOUBLE,  "PropertyDouble description", boost::bind(&TestPropertyGroup_5::SetDouble, &baseObject, _1), boost::bind(&TestPropertyGroup_5::GetDouble, &baseObject), new DoubleEditor() );

	double dValue;
	wrapper.set_as_string( PROPERTY_DOUBLE, PROPERTY_DOUBLE_STRING_VALUE );
	BOOST_CHECK( from_string<double>(dValue, wrapper.get_as_string( PROPERTY_DOUBLE), std::dec) );
	BOOST_CHECK_EQUAL( dValue, PROPERTY_DOUBLE_VALUE );

	pEditor = wrapper.get_metadata( PROPERTY_DOUBLE );
	BOOST_CHECK( pEditor->getEditorPropertyType() == DoubleType );
	BOOST_CHECK( pEditor->GetControlType() == EditBox );

	// Test Auto-Generated Designer Creation
	property_group_wrapper wrapperAutoGenDesigner("test_wrapper", &baseObject);

	wrapperAutoGenDesigner.add_property(PROPERTY_DOUBLE,  "PropertyDouble description", boost::bind(&TestPropertyGroup_5::SetDouble, &baseObject, _1), boost::bind(&TestPropertyGroup_5::GetDouble, &baseObject) );

	pEditor = wrapperAutoGenDesigner.get_metadata(PROPERTY_DOUBLE);
	BOOST_CHECK( NULL != dynamic_cast<const DoubleEditor*>(pEditor) );
}

BOOST_AUTO_TEST_CASE( TestPropertyGroupWrapper_Float )
{
	TestPropertyGroup_4 baseObject;
	property_group_wrapper wrapper("test_wrapper", &baseObject);
	const Editor *pEditor;

	// Test Standard Creation
	wrapper.add_property(PROPERTY_FLOAT_1,  "PropertyFloat1 description", boost::bind(&TestPropertyGroup_4::SetFloat, &baseObject, _1), boost::bind(&TestPropertyGroup_4::GetFloat, &baseObject), new FloatEditor() );

	float fValue;
	wrapper.set_as_string( PROPERTY_FLOAT_1, PROPERTY_FLOAT_1_STRING_VALUE );
	BOOST_CHECK( from_string<float>(fValue, wrapper.get_as_string(PROPERTY_FLOAT_1), std::dec) );
	BOOST_CHECK_EQUAL( fValue, PROPERTY_FLOAT_1_VALUE );

	pEditor = wrapper.get_metadata( PROPERTY_FLOAT_1 );
	BOOST_CHECK( pEditor->getEditorPropertyType() == FloatType );
	BOOST_CHECK( pEditor->GetControlType() == EditBox );

	// Test Auto-Generated Designer Creation
	property_group_wrapper wrapperAutoGenDesigner("test_wrapper", &baseObject);

	wrapperAutoGenDesigner.add_property(PROPERTY_FLOAT_1,  "PropertyFloat1 description", boost::bind(&TestPropertyGroup_4::SetFloat, &baseObject, _1), boost::bind(&TestPropertyGroup_4::GetFloat, &baseObject) );

	pEditor = wrapperAutoGenDesigner.get_metadata(PROPERTY_FLOAT_1);
	BOOST_CHECK( NULL != dynamic_cast<const FloatEditor*>(pEditor) );
}

BOOST_AUTO_TEST_CASE( TestPropertyGroupWrapper_Integer )
{
	TestPropertyGroup_4 baseObject;
	property_group_wrapper wrapper("test_wrapper", &baseObject);
	const Editor *pEditor;

	// Test Standard Creation
	wrapper.add_property(PROPERTY_INT_1, "PropertyInt1 description", boost::bind(&TestPropertyGroup_4::SetInt, &baseObject, _1), boost::bind(&TestPropertyGroup_4::GetInt, &baseObject), new IntegerEditor(INT_LOW, INT_HIGH, INT_INCREMENT, DropDown));

	int iValue;
	wrapper.set_as_string( PROPERTY_INT_1, PROPERTY_INT_1_STRING_VALUE );
	BOOST_CHECK( from_string<int>(iValue, wrapper.get_as_string(PROPERTY_INT_1), std::dec) );
	BOOST_CHECK_EQUAL( iValue, PROPERTY_INT_1_VALUE );

	pEditor = wrapper.get_metadata( PROPERTY_INT_1 );
	BOOST_CHECK( pEditor->getEditorPropertyType() == IntegerType );
	BOOST_CHECK( pEditor->GetControlType() == DropDown );

	// Test Auto-Generated Designer Creation
	property_group_wrapper wrapperAutoGenDesigner("test_wrapper", &baseObject);

	wrapperAutoGenDesigner.add_property(PROPERTY_INT_1, "PropertyInt1 description", boost::bind(&TestPropertyGroup_4::SetInt, &baseObject, _1), boost::bind(&TestPropertyGroup_4::GetInt, &baseObject) );

	pEditor = wrapperAutoGenDesigner.get_metadata(PROPERTY_INT_1);
	BOOST_CHECK( NULL != dynamic_cast<const IntegerEditor*>(pEditor) );
}

BOOST_AUTO_TEST_CASE( TestPropertyGroupWrapper_String )
{
	TestPropertyGroup baseObject;
	property_group_wrapper wrapper("test_wrapper", &baseObject);
	const Editor *pEditor;

	// Test Standard Creation
	wrapper.add_property(PROPERTY_STRING_2,  "PropertyString2 description", boost::bind(&TestPropertyGroup::SetPropertyString2, &baseObject, _1), boost::bind(&TestPropertyGroup::GetPropertyString2, &baseObject), new StringEditor());

	wrapper.set_as_string( PROPERTY_STRING_2, PROPERTY_STRING_2_VALUE );
	BOOST_CHECK( PROPERTY_STRING_2_VALUE == wrapper.get_as_string( PROPERTY_STRING_2) );
	wrapper.set_as_string( PROPERTY_STRING_2, PROPERTY_STRING_1_VALUE );
	BOOST_CHECK( PROPERTY_STRING_1_VALUE == wrapper.get_as_string( PROPERTY_STRING_2) );

	pEditor = wrapper.get_metadata( PROPERTY_STRING_2 );
	BOOST_CHECK( pEditor->getEditorPropertyType() == StringType );
	BOOST_CHECK( pEditor->GetControlType() == EditBox );

	// Test Auto-Generated Designer Creation
	property_group_wrapper wrapperAutoGenDesigner("test_wrapper", &baseObject);

	wrapperAutoGenDesigner.add_property(PROPERTY_STRING_2,  "PropertyString2 description", boost::bind(&TestPropertyGroup::SetPropertyString2, &baseObject, _1), boost::bind(&TestPropertyGroup::GetPropertyString2, &baseObject) );

	pEditor = wrapperAutoGenDesigner.get_metadata(PROPERTY_STRING_2);
	BOOST_CHECK( NULL != dynamic_cast<const StringEditor*>(pEditor) );
}
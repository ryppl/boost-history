// Boost.Pinhole library

// Copyright Jared McIntyre 2007. Use, modification and
// distribution is subject to the Boost Software License, Version
// 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

// For more information, see http://www.boost.org

#include <boost/pinhole/property_group.h>
#include <boost/pinhole/IntegerEditor.h>
#include <boost/pinhole/FloatEditor.h>
#include <boost/pinhole/StringEditor.h>
#include <boost/pinhole/BoolEditor.h>
#include <boost/pinhole/DoubleEditor.h>
#include <boost/pinhole/property_manager.h>

using namespace boost::pinhole;

#define PROPERTY_GROUP_NAME ("TestPropertyGroup")
#define PROPERTY_GROUP_CATEGORY1 ("MyPropertyGroupCategory1")
#define PROPERTY_GROUP_CATEGORY2 ("MyPropertyGroupCategory2")
#define PROPERTY_GROUP_CATEGORY3 ("MyPropertyGroupCategory3")
#define PROPERTY_GROUP_CHILD_NAME ("TestPropertyChildGroup")

#define PROPERTY_STRING_1 ("PropertyString1")
#define PROPERTY_STRING_1_VALUE ("PropertyString1_Value")

#define PROPERTY_FLOAT_1 ("PropertyFloat1")
#define PROPERTY_FLOAT_1_VAR ("PropertyFloat1Var")
#define PROPERTY_FLOAT_1_VALUE ( 3.14161f )
#define PROPERTY_FLOAT_1_STRING_VALUE ( "3.14161" )

#define PROPERTY_FLOAT_2 ("PropertyFloat2")
#define PROPERTY_FLOAT_2_VALUE ( 345.123f )
#define PROPERTY_FLOAT_2_STRING_VALUE ( "345.123" )

#define PROPERTY_DOUBLE ("PropertyDouble")
#define PROPERTY_DOUBLE_VAR ("PropertyDoubleVar")
#define PROPERTY_DOUBLE_VALUE ( 54321.1234596432 )
#define PROPERTY_DOUBLE_STRING_VALUE ( "54321.1234596432" )

#define PROPERTY_DOUBLE_2 ("PropertyDouble2")
#define PROPERTY_DOUBLE_2_VALUE ( 54321.1234596432 )
#define PROPERTY_DOUBLE_2_STRING_VALUE ( "54321.1234596432" )

#define PROPERTY_INT_1 ("PropertyInt1")
#define PROPERTY_INT_1_VAR ("PropertyInt1Var")
#define PROPERTY_INT_1_VALUE (54321)
#define PROPERTY_INT_1_STRING_VALUE ("54321")
#define PROPERTY_INT_2 ("PropertyInt2")
#define PROPERTY_INT_2_VALUE (12345)

#define PROPERTY_STRING_2 ("PropertyString2")
#define PROPERTY_STRING_2_VAR ("PropertyString2Var")
#define PROPERTY_STRING_2_VALUE ("PropertyString2_Value")

#define PROPERTY_BOOL ("PropertyBool")
#define PROPERTY_BOOL_VAR ("PropertyBoolVar")
#define PROPERTY_BOOL_VALUE ("True")

#define PROPERTY_NAME ("Name")

#define ACTION_1 ("BOOST_ACTION 1")
#define ACTION_2 ("BOOST_ACTION 2")

const static string PropertyGroupXML( 
"<TestPropertyGroup>\
<PropertyBool>True</PropertyBool>\
<PropertyFloat1>3.14161</PropertyFloat1>\
<PropertyInt1>54321</PropertyInt1>\
<PropertyString1>PropertyString1_Value</PropertyString1>\
<PropertyString2>PropertyString2_Value</PropertyString2>\
<TestPropertyChildGroup>\
<PropertyFloat1>3.14161</PropertyFloat1>\
<PropertyInt1>54321</PropertyInt1>\
<PropertyString2>PropertyString2_Value</PropertyString2>\
</TestPropertyChildGroup>\
<TestPropertyChildGroup>\
<PropertyFloat1>3.14161</PropertyFloat1>\
<PropertyInt1>54321</PropertyInt1>\
<PropertyString2>PropertyString2_Value</PropertyString2>\
</TestPropertyChildGroup>\
</TestPropertyGroup>"
);

// helper function to convert from types to strings
template <class T>
bool from_string(T& t, 
					  const std::string& s, 
					  std::ios_base& (*f)(std::ios_base&))
{
	std::istringstream iss(s);
	return !(iss >> f >> t).fail();
}

class TestPropertyChildGroup : public property_group
{
public:
	TestPropertyChildGroup( property_group *pParentGroup) : property_group( PROPERTY_GROUP_CHILD_NAME, pParentGroup )
	{
		add_property<float>(PROPERTY_FLOAT_1,  "PropertyFloat1 description", BOOST_SETTER(&TestPropertyChildGroup::SetFloat), BOOST_GETTER(&TestPropertyChildGroup::GetFloat), new FloatEditor());
		add_property<int>(PROPERTY_INT_1, "PropertyInt1 description", BOOST_SETTER(&TestPropertyChildGroup::SetInt), BOOST_GETTER(&TestPropertyChildGroup::GetInt), new IntegerEditor());
		add_property<string>(PROPERTY_STRING_2,  "PropertyString2 description", BOOST_SETTER(&TestPropertyChildGroup::SetPropertyString2), BOOST_GETTER(&TestPropertyChildGroup::GetPropertyString2), new StringEditor());
	}

	float GetFloat() const{return( m_fFloat1 );}
	void SetFloat( float fValue ){m_fFloat1 = fValue;}
	int GetInt() const{return( m_iInt1 );}
	void SetInt( int iValue ){m_iInt1 = iValue;}
	string GetPropertyString2(){return( m_strString2 );}
	void SetPropertyString2( string strValue ){m_strString2 = strValue;}

	float m_fFloat1;
	int m_iInt1;
	string m_strString2;
};

class TestPropertyGroup : public property_group
{
public:
#if defined(BOOST_MSVC)
    #pragma warning(push)
    #pragma warning( disable: 4355 )
#endif
	TestPropertyGroup() : property_group( PROPERTY_GROUP_NAME, NULL ), m_child1( this ), m_child2( this )
	{
        m_bVarBool = false;

		add_property<string>(PROPERTY_STRING_1,  "PropertyString1 description", BOOST_SETTER_NONE, BOOST_GETTER(&TestPropertyGroup::GetPropertyString1), new StringEditor());
		add_property<float>(PROPERTY_FLOAT_1,  "PropertyFloat1 description", BOOST_SETTER(&TestPropertyGroup::SetFloat), BOOST_GETTER(&TestPropertyGroup::GetFloat), new FloatEditor());
		add_property<int>(PROPERTY_INT_1, "PropertyInt1 description", BOOST_SETTER(&TestPropertyGroup::SetInt), BOOST_GETTER(&TestPropertyGroup::GetInt), new IntegerEditor());
		add_property<string>(PROPERTY_STRING_2,  "PropertyString2 description", BOOST_SETTER(&TestPropertyGroup::SetPropertyString2), BOOST_GETTER(&TestPropertyGroup::GetPropertyString2), new StringEditor());
		add_property<bool>(PROPERTY_BOOL,  "PropertyBool description", BOOST_SETTER(&TestPropertyGroup::SetBool), BOOST_GETTER(&TestPropertyGroup::GetBool), new BoolEditor());
        
        add_property<float>(PROPERTY_FLOAT_1_VAR, "PropertyFloatVar description", BOOST_SETTER_VAR(m_fVarFloat1), BOOST_GETTER_VAR(m_fVarFloat1), new FloatEditor());
        add_property<int>(PROPERTY_INT_1_VAR, "PropertyIntVar description", BOOST_SETTER_VAR(m_iVarInt1), BOOST_GETTER_VAR(m_iVarInt1), new IntegerEditor());
        add_property<string>(PROPERTY_STRING_2_VAR, "PropertyStringVar description", BOOST_SETTER_VAR(m_strVarString2), BOOST_GETTER_VAR(m_strVarString2), new StringEditor());
        add_property<bool>(PROPERTY_BOOL_VAR, "PropertyBoolVar description", BOOST_SETTER_VAR(m_bVarBool), BOOST_GETTER_VAR(m_bVarBool), new BoolEditor());
	}
#if defined(BOOST_MSVC)
    #pragma warning(pop)
#endif

	TestPropertyChildGroup m_child1;
	TestPropertyChildGroup m_child2;

	string GetPropertyString1(){return PROPERTY_STRING_1_VALUE;}
	float GetFloat() const{return( m_fFloat1 );}
	void SetFloat( float fValue ){m_fFloat1 = fValue;}
	int GetInt() const{return( m_iInt1 );}
	void SetInt( int iValue ){m_iInt1 = iValue;}
	string GetPropertyString2(){return( m_strString2 );}
	void SetPropertyString2( string strValue ){m_strString2 = strValue;}
	void SetBool( bool b ){ m_bBool = b; }
	bool GetBool(){ return( m_bBool ); }

private:
	float m_fFloat1;
	int m_iInt1;
	string m_strString2;
	bool m_bBool;

	float m_fVarFloat1;
	int m_iVarInt1;
	string m_strVarString2;
    bool m_bVarBool;
};

class TestPropertyGroup_1 : public property_group
{
public:
	TestPropertyGroup_1() : property_group( PROPERTY_GROUP_NAME, NULL )
	{
		add_property<float>(PROPERTY_FLOAT_1,  "PropertyFloat1 description", BOOST_SETTER(&TestPropertyGroup_1::SetFloat), BOOST_GETTER(&TestPropertyGroup_1::GetFloat), new FloatEditor() );
		add_property<int>(PROPERTY_INT_1, "PropertyInt1 description", BOOST_SETTER(&TestPropertyGroup_1::SetInt), BOOST_GETTER(&TestPropertyGroup_1::GetInt), new IntegerEditor());
		add_property<string>(PROPERTY_STRING_2,  "PropertyString2 description", BOOST_SETTER(&TestPropertyGroup_1::SetPropertyString2), BOOST_GETTER(&TestPropertyGroup_1::GetPropertyString2), new StringEditor() );
	}

private:
	float GetFloat() const{return( m_fFloat1 );}
	void SetFloat( float fValue ){m_fFloat1 = fValue;}
	int GetInt() const{return( m_iInt1 );}
	void SetInt( int iValue ){m_iInt1 = iValue;}
	string GetPropertyString2(){return( m_strString2 );}
	void SetPropertyString2( string strValue ){m_strString2 = strValue;}

	float m_fFloat1;
	int m_iInt1;
	string m_strString2;
};

const static string PropertyGroupXML_1( 
"<TestPropertyGroup PropertyFloat1='3.14161' PropertyString2='PropertyString2_Value'>\
<PropertyInt1>54321</PropertyInt1>\
</TestPropertyGroup>" );

class TestPropertyGroup_2 : public property_group
{
public:
	TestPropertyGroup_2() : property_group( PROPERTY_GROUP_NAME, NULL )
	{
		add_property<float>(PROPERTY_FLOAT_1,  "PropertyFloat1 description", BOOST_SETTER(&TestPropertyGroup_2::SetFloat), BOOST_GETTER(&TestPropertyGroup_2::GetFloat), new FloatEditor() );
		add_property<int>(PROPERTY_INT_1, "PropertyInt1 description", BOOST_SETTER(&TestPropertyGroup_2::SetInt), BOOST_GETTER(&TestPropertyGroup_2::GetInt), new IntegerEditor() );
		add_property<string>(PROPERTY_STRING_2,  "PropertyString2 description", BOOST_SETTER(&TestPropertyGroup_2::SetPropertyString2), BOOST_GETTER(&TestPropertyGroup_2::GetPropertyString2), new StringEditor() );
	}

private:
	float GetFloat() const{return( m_fFloat1 );}
	void SetFloat( float fValue ){m_fFloat1 = fValue;}
	int GetInt() const{return( m_iInt1 );}
	void SetInt( int iValue ){m_iInt1 = iValue;}
	string GetPropertyString2(){return( m_strString2 );}
	void SetPropertyString2( string strValue ){m_strString2 = strValue;}

	float m_fFloat1;
	int m_iInt1;
	string m_strString2;
};

const static string PropertyGroupXML_2( 
"<TestPropertyGroup PropertyFloat1='3.14161' PropertyInt1='54321' PropertyString2='PropertyString2_Value'></TestPropertyGroup>" );

class TestPropertyChildGroup_1 : public property_group
{
public:
	TestPropertyChildGroup_1( property_group *pParentGroup) : property_group( PROPERTY_GROUP_CHILD_NAME, pParentGroup )
	{
		add_property<float>(PROPERTY_FLOAT_1,  "PropertyFloat1 description", BOOST_SETTER(&TestPropertyChildGroup_1::SetFloat), BOOST_GETTER(&TestPropertyChildGroup_1::GetFloat), new FloatEditor());
		add_property<int>(PROPERTY_INT_1, "PropertyInt1 description", BOOST_SETTER(&TestPropertyChildGroup_1::SetInt), BOOST_GETTER(&TestPropertyChildGroup_1::GetInt), new IntegerEditor() );
		add_property<string>(PROPERTY_STRING_2,  "PropertyString2 description", BOOST_SETTER(&TestPropertyChildGroup_1::SetPropertyString2), BOOST_GETTER(&TestPropertyChildGroup_1::GetPropertyString2), new StringEditor());
	}

private:
	float GetFloat() const{return( m_fFloat1 );}
	void SetFloat( float fValue ){m_fFloat1 = fValue;}
	int GetInt() const{return( m_iInt1 );}
	void SetInt( int iValue ){m_iInt1 = iValue;}
	string GetPropertyString2(){return( m_strString2 );}
	void SetPropertyString2( string strValue ){m_strString2 = strValue;}

	float m_fFloat1;
	int m_iInt1;
	string m_strString2;
};

class TestPropertyGroup_3 : public property_group
{
public:
#if defined(BOOST_MSVC)
    #pragma warning(push)
    #pragma warning( disable: 4355 )
#endif
	TestPropertyGroup_3() : property_group( PROPERTY_GROUP_NAME, NULL ), m_child1( this ), m_child2( this )
	{
		add_property<float>(PROPERTY_FLOAT_1,  "PropertyFloat1 description", BOOST_SETTER(&TestPropertyGroup_3::SetFloat), BOOST_GETTER(&TestPropertyGroup_3::GetFloat), new FloatEditor() );
		add_property<int>(PROPERTY_INT_1, "PropertyInt1 description", BOOST_SETTER(&TestPropertyGroup_3::SetInt), BOOST_GETTER(&TestPropertyGroup_3::GetInt), new IntegerEditor());
		add_property<string>(PROPERTY_STRING_2,  "PropertyString2 description", BOOST_SETTER(&TestPropertyGroup_3::SetPropertyString2), BOOST_GETTER(&TestPropertyGroup_3::GetPropertyString2), new StringEditor());
		add_property<bool>(PROPERTY_BOOL,  "PropertyBool description", BOOST_SETTER(&TestPropertyGroup_3::SetBool), BOOST_GETTER(&TestPropertyGroup_3::GetBool), new BoolEditor());
	}
#if defined(BOOST_MSVC)
    #pragma warning(pop)
#endif

	TestPropertyChildGroup_1 m_child1;
	TestPropertyChildGroup_1 m_child2;

private:
	float GetFloat() const{return( m_fFloat1 );}
	void SetFloat( float fValue ){m_fFloat1 = fValue;}
	int GetInt() const{return( m_iInt1 );}
	void SetInt( int iValue ){m_iInt1 = iValue;}
	string GetPropertyString2(){return( m_strString2 );}
	void SetPropertyString2( string strValue ){m_strString2 = strValue;}
	void SetBool( bool b ){ m_bBool = b; }
	bool GetBool(){ return( m_bBool ); }

	float m_fFloat1;
	int m_iInt1;
	string m_strString2;
	bool m_bBool;
};

const static string PropertyGroupXML_3( 
"<TestPropertyGroup PropertyFloat1='3.14161'>\
<PropertyBool>True</PropertyBool>\
<PropertyInt1>54321</PropertyInt1>\
<PropertyString2>PropertyString2_Value</PropertyString2>\
<TestPropertyChildGroup PropertyInt1='54321'>\
<PropertyFloat1>3.14161</PropertyFloat1>\
<PropertyString2>PropertyString2_Value</PropertyString2>\
</TestPropertyChildGroup>\
<TestPropertyChildGroup PropertyInt1='54321'>\
<PropertyFloat1>3.14161</PropertyFloat1>\
<PropertyString2>PropertyString2_Value</PropertyString2>\
</TestPropertyChildGroup>\
</TestPropertyGroup>" );

const int INT_LOW = 3;
const int INT_HIGH = 25;
const int INT_INCREMENT = 2;
const float FLOAT_LOW = -10.6f;
const float FLOAT_HIGH = 25.7f;
const float FLOAT_INCREMENT = 1.5f;
const double DOUBLE_LOW = -3.7;
const double DOUBLE_HIGH = 72.6;
const double DOUBLE_INCREMENT = 0.5;
class TestPropertyGroup_4 : public property_group
{
public:
#if defined(BOOST_MSVC)
    #pragma warning(push)
    #pragma warning( disable: 4355 )
#endif
	TestPropertyGroup_4() : property_group( PROPERTY_GROUP_NAME, NULL ), m_child1( this ), m_child2( this )
	{
		add_property<float>(PROPERTY_FLOAT_1,  "PropertyFloat1 description", BOOST_SETTER(&TestPropertyGroup_4::SetFloat), BOOST_GETTER(&TestPropertyGroup_4::GetFloat), new FloatEditor() );
		add_property<float>(PROPERTY_FLOAT_2,  "PropertyFloat2 description", BOOST_SETTER(&TestPropertyGroup_4::SetFloat2), BOOST_GETTER(&TestPropertyGroup_4::GetFloat2), new FloatEditor(FLOAT_LOW, FLOAT_HIGH, FLOAT_INCREMENT, Tracker ) );
		add_property<int>(PROPERTY_INT_1, "PropertyInt1 description", BOOST_SETTER(&TestPropertyGroup_4::SetInt), BOOST_GETTER(&TestPropertyGroup_4::GetInt), new IntegerEditor(INT_LOW, INT_HIGH, INT_INCREMENT, DropDown));
		add_property<int>(PROPERTY_INT_2, "PropertyInt2 description", BOOST_SETTER(&TestPropertyGroup_4::SetInt), BOOST_GETTER(&TestPropertyGroup_4::GetInt), new IntegerEditor());
		add_property<string>(PROPERTY_STRING_2,  "PropertyString2 description", BOOST_SETTER(&TestPropertyGroup_4::SetPropertyString2), BOOST_GETTER(&TestPropertyGroup_4::GetPropertyString2), new StringEditor());
		add_property<bool>(PROPERTY_BOOL,  "PropertyBool description", BOOST_SETTER(&TestPropertyGroup_4::SetBool), BOOST_GETTER(&TestPropertyGroup_4::GetBool), new BoolEditor());
	}
#if defined(BOOST_MSVC)
    #pragma warning(pop)
#endif

	TestPropertyChildGroup_1 m_child1;
	TestPropertyChildGroup_1 m_child2;
	float GetFloat() const{return( m_fFloat1 );}
	void SetFloat( float fValue ){m_fFloat1 = fValue;}
	float GetFloat2() const{return( m_fFloat2 );}
	void SetFloat2( float fValue ){m_fFloat2 = fValue;}
	int GetInt() const{return( m_iInt1 );}
	void SetInt( int iValue ){m_iInt1 = iValue;}
	string GetPropertyString2(){return( m_strString2 );}
	void SetPropertyString2( string strValue ){m_strString2 = strValue;}
	void SetBool( bool b ){ m_bBool = b; }
	bool GetBool(){ return( m_bBool ); }

private:
	float m_fFloat1;
	float m_fFloat2;
	int m_iInt1;
	string m_strString2;
	bool m_bBool;
};

const static string PropertyGroupXML_5( 
"<TestPropertyGroup>\
<PropertyBool>True</PropertyBool>\
<PropertyInt1>54321</PropertyInt1>\
</TestPropertyGroup>" );

class TestPropertyGroup_5 : public property_group
{
public:
	TestPropertyGroup_5() : property_group( PROPERTY_GROUP_NAME, NULL )
	{
		add_property<float>(PROPERTY_FLOAT_1,  "PropertyFloat1 description", BOOST_SETTER(&TestPropertyGroup_5::SetFloat), BOOST_GETTER(&TestPropertyGroup_5::GetFloat), new FloatEditor() );
		add_property<int>(PROPERTY_INT_1, "PropertyInt1 description", BOOST_SETTER(&TestPropertyGroup_5::SetInt), BOOST_GETTER(&TestPropertyGroup_5::GetInt), NULL);
		add_property<string>(PROPERTY_STRING_2,  "PropertyString2 description", BOOST_SETTER(&TestPropertyGroup_5::SetPropertyString2), BOOST_GETTER(&TestPropertyGroup_5::GetPropertyString2), new StringEditor() );
		add_property<bool>(PROPERTY_BOOL,  "PropertyBool description", BOOST_SETTER(&TestPropertyGroup_5::SetBool), BOOST_GETTER(&TestPropertyGroup_5::GetBool), new BoolEditor());
		add_property<double>(PROPERTY_DOUBLE,  "PropertyDouble description", BOOST_SETTER(&TestPropertyGroup_5::SetDouble), BOOST_GETTER(&TestPropertyGroup_5::GetDouble), new DoubleEditor() );
		add_property<double>(PROPERTY_DOUBLE_2,  "PropertyDouble2 description", BOOST_SETTER(&TestPropertyGroup_5::SetDouble2), BOOST_GETTER(&TestPropertyGroup_5::GetDouble2), new DoubleEditor(DOUBLE_LOW, DOUBLE_HIGH, DOUBLE_INCREMENT, Tracker) );

        add_property<double>(PROPERTY_DOUBLE_VAR, "PropertyDoubleVar description", BOOST_SETTER_VAR(m_dVarDouble), BOOST_GETTER_VAR(m_dVarDouble), new BoolEditor());
	}

	void add_category( const std::string &category_name )
	{
		property_group::add_category( category_name );
	}

	
	template<typename Value_Type>
	void add_property( std::string name, 
					   std::string description,
					   boost::function<void (const Value_Type&)> setter, 
					   boost::function<Value_Type ()> getter )
	{
		property_group::add_property( name, description, setter, getter );
	}

	template<typename Value_Type>
	void add_property( std::string name, 
					   std::string description,
					   boost::function<void (const Value_Type&)> setter, 
					   boost::function<Value_Type ()> getter,
					   Editor *pEditor )
	{
		property_group::add_property( name, description, setter, getter, pEditor );
	}
	
	void clear_properties()
	{
		m_properties.clear();
	}

	float GetFloat() const{return( m_fFloat1 );}
	void SetFloat( float fValue ){m_fFloat1 = fValue;}
	double GetDouble() const{return( m_dDouble );}
	void SetDouble( double dValue ){m_dDouble = dValue;}
	double GetDouble2() const{return( m_dDouble2 );}
	void SetDouble2( double dValue ){m_dDouble2 = dValue;}
	int GetInt() const{return( m_iInt1 );}
	void SetInt( int iValue ){m_iInt1 = iValue;}
	string GetPropertyString2(){return( m_strString2 );}
	void SetPropertyString2( string strValue ){m_strString2 = strValue;}
	void SetBool( bool b ){ m_bBool = b; }
	bool GetBool(){ return( m_bBool ); }

private:
	float m_fFloat1;
	double m_dDouble;
	double m_dDouble2;
	int m_iInt1;
	string m_strString2;
	bool m_bBool;

    double m_dVarDouble;
};

class TestAutoGeneratedDesigners : public property_group
{
public:
#if defined(BOOST_MSVC)
    #pragma warning(push)
    #pragma warning( disable: 4355 )
#endif
	TestAutoGeneratedDesigners() : property_group( PROPERTY_GROUP_NAME, NULL )
	{
		add_property<string>(PROPERTY_STRING_1,  "PropertyString1 description",
			                BOOST_SETTER_NONE, BOOST_GETTER(&TestAutoGeneratedDesigners::GetPropertyString1));
		add_property<float> (PROPERTY_FLOAT_1,  "PropertyFloat1 description",
			                BOOST_SETTER(&TestAutoGeneratedDesigners::SetFloat), BOOST_GETTER(&TestAutoGeneratedDesigners::GetFloat));
		add_property<int>   (PROPERTY_INT_1, "PropertyInt1 description",
			                BOOST_SETTER(&TestAutoGeneratedDesigners::SetInt), BOOST_GETTER(&TestAutoGeneratedDesigners::GetInt));
		add_property<string>(PROPERTY_STRING_2,  "PropertyString2 description",
			                BOOST_SETTER(&TestAutoGeneratedDesigners::SetPropertyString2), BOOST_GETTER(&TestAutoGeneratedDesigners::GetPropertyString2));
		add_property<bool>  (PROPERTY_BOOL,  "PropertyBool description",
			                BOOST_SETTER(&TestAutoGeneratedDesigners::SetBool), BOOST_GETTER(&TestAutoGeneratedDesigners::GetBool));
		add_property<double>(PROPERTY_DOUBLE,  "PropertyFloat1 description",
			                BOOST_SETTER(&TestAutoGeneratedDesigners::SetDouble), BOOST_GETTER(&TestAutoGeneratedDesigners::GetDouble));

	}
#if defined(BOOST_MSVC)
    #pragma warning(pop)
#endif

private:
	string GetPropertyString1(){return PROPERTY_STRING_1_VALUE;}
	float GetFloat() const{return( m_fFloat1 );}
	void SetFloat( float fValue ){m_fFloat1 = fValue;}
	int GetInt() const{return( m_iInt1 );}
	void SetInt( int iValue ){m_iInt1 = iValue;}
	string GetPropertyString2(){return( m_strString2 );}
	void SetPropertyString2( string strValue ){m_strString2 = strValue;}
	void SetBool( bool b ){ m_bBool = b; }
	bool GetBool(){ return( m_bBool ); }
	double GetDouble() const{return( m_dDouble );}
	void SetDouble( double dValue ){m_dDouble = dValue;}

	float m_fFloat1;
	int m_iInt1;
	string m_strString2;
	bool m_bBool;
	double m_dDouble;
};

class TestUpDownGroup : public property_group
{
public:
	TestUpDownGroup() : property_group( PROPERTY_GROUP_NAME, NULL )
	{
		add_property<float>(PROPERTY_FLOAT_1,  "PropertyFloat1 description", BOOST_SETTER(&TestUpDownGroup::SetFloat), BOOST_GETTER(&TestUpDownGroup::GetFloat), new FloatEditor(-1000,1000,1,UpDown) );
	}

private:
	float GetFloat() const{return( m_fFloat1 );}
	void SetFloat( float fValue ){m_fFloat1 = fValue;}
	double GetDouble() const{return( m_dDouble );}
	void SetDouble( double dValue ){m_dDouble = dValue;}
	double GetDouble2() const{return( m_dDouble2 );}
	void SetDouble2( double dValue ){m_dDouble2 = dValue;}
	int GetInt() const{return( m_iInt1 );}
	void SetInt( int iValue ){m_iInt1 = iValue;}


	float m_fFloat1;
	double m_dDouble;
	double m_dDouble2;
	int m_iInt1;
};

class TestActionsFixture : public property_group
{
public:
#if defined(BOOST_MSVC)
    #pragma warning(push)
    #pragma warning( disable: 4355 )
#endif
	TestActionsFixture() : property_group( PROPERTY_GROUP_NAME, NULL )
	{
		bTriggeredAction1 = false;
		bTriggeredAction2 = false;

		add_action(ACTION_1, "First BOOST_ACTION",  BOOST_ACTION(&TestActionsFixture::Action1));
		add_action(ACTION_2, "Second BOOST_ACTION", BOOST_ACTION(&TestActionsFixture::Action2));
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

class TestPropertySerializer : public property_group
{
public:
	TestPropertySerializer( property_group *pParentGroup) : property_group( "TestPropertySerializer", pParentGroup )
	{
        m_fFloat1    = 2.45f;
	    m_iInt1      = 365;
	    m_strString2 = "test value";

        add_property<string>(PROPERTY_NAME,  "Name", BOOST_SETTER_NONE, BOOST_GETTER(&TestPropertySerializer::GetName), new FloatEditor());
		add_property<float>(PROPERTY_FLOAT_1,  "PropertyFloat1 description", BOOST_SETTER(&TestPropertySerializer::SetFloat), BOOST_GETTER(&TestPropertySerializer::GetFloat), new FloatEditor());
		add_property<int>(PROPERTY_INT_1, "PropertyInt1 description", BOOST_SETTER(&TestPropertySerializer::SetInt), BOOST_GETTER(&TestPropertySerializer::GetInt), new IntegerEditor() );
		add_property<string>(PROPERTY_STRING_2,  "PropertyString2 description", BOOST_SETTER(&TestPropertySerializer::SetPropertyString2), BOOST_GETTER(&TestPropertySerializer::GetPropertyString2), new StringEditor());
	}

private:
    string GetName() const {return "aName";}
	float GetFloat() const{return( m_fFloat1 );}
	void SetFloat( float fValue ){m_fFloat1 = fValue;}
	int GetInt() const{return( m_iInt1 );}
	void SetInt( int iValue ){m_iInt1 = iValue;}
	string GetPropertyString2(){return( m_strString2 );}
	void SetPropertyString2( string strValue ){m_strString2 = strValue;}

	float m_fFloat1;
	int m_iInt1;
	string m_strString2;
};
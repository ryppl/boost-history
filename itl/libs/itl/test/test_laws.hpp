/*----------------------------------------------------------------------------+
Copyright (c) 2008-2008: Joachim Faulhaber
+-----------------------------------------------------------------------------+
   Distributed under the Boost Software License, Version 1.0.
      (See accompanying file LICENCE.txt or copy at
           http://www.boost.org/LICENSE_1_0.txt)
+----------------------------------------------------------------------------*/
#ifndef __test_itl_laws_h_JOFA_090119__
#define __test_itl_laws_h_JOFA_090119__


template<class Type>
struct equality : std::binary_function<Type,Type,bool> 
{
	typedef bool (type)(const Type&, const Type&);
};

#define DEFINE_ASSOCIATIVITY_CHECK_WRT(op_tag, op_sign) \
template<class Type, class TypeB, class TypeC> \
void check_associativity_wrt_##op_tag(const Type& a, const TypeB& b, const TypeC& c) \
{ \
	Type left  = (a op_sign b) op_sign c; \
	Type right = a op_sign (b op_sign c); \
	BOOST_CHECK_EQUAL(left,right); \
}

#define DEFINE_ASSOCIATIVITY_CHECK_WRT_EQUAL(op_tag, op_sign) \
template<class Type, class TypeB, class TypeC> \
void check_associativity_wrt_equal_##op_tag \
(typename equality<Type>::type* equal, \
const Type& a, const TypeB& b, const TypeC& c) \
{ \
	Type left  = (a op_sign b) op_sign c; \
	Type right = a op_sign (b op_sign c); \
	BOOST_CHECK_EQUAL((*equal)(left,right), true); \
}

#define CHECK_ASSOCIATIVITY_WRT(op_tag)       check_associativity_wrt_##op_tag
#define CHECK_ASSOCIATIVITY_WRT_EQUAL(op_tag) check_associativity_wrt_equal_##op_tag

DEFINE_ASSOCIATIVITY_CHECK_WRT      (plus, + );
DEFINE_ASSOCIATIVITY_CHECK_WRT_EQUAL(plus, + );
DEFINE_ASSOCIATIVITY_CHECK_WRT      (et, & );
DEFINE_ASSOCIATIVITY_CHECK_WRT_EQUAL(et, & );


//------------------------------------------------------------------------------
// neutrality
//------------------------------------------------------------------------------

#define DEFINE_RIGHT_NEUTRALITY_CHECK_WRT_EQUAL(op_tag, op_sign) \
template<class Type> \
void check_right_neutrality_wrt_equal_##op_tag \
(typename equality<Type>::type* equal, const Type& a, const Type& neutron) \
{ \
	Type left  = a op_sign neutron; \
	Type right = neutron; \
	BOOST_CHECK_EQUAL((*equal)(left,right), true); \
}

#define DEFINE_RIGHT_NEUTRALITY_CHECK_WRT(op_tag, op_sign) \
template<class Type> \
void check_right_neutrality_wrt_##op_tag \
(typename equality<Type>::type* equal, const Type& a, const Type& neutron) \
{ \
	Type left  = a op_sign neutron; \
	Type right = neutron; \
	BOOST_CHECK_EQUAL(left,right); \
}

#define DEFINE_NEUTRALITY_CHECK_WRT_EQUAL(op_tag, op_sign) \
template<class Type> \
void check_neutrality_wrt_equal_##op_tag \
(typename equality<Type>::type* equal, const Type& a, const Type& neutron) \
{ \
	Type left  = a op_sign neutron; \
	Type right = neutron op_sign a; \
	BOOST_CHECK_EQUAL((*equal)(left,right), true); \
}

#define DEFINE_NEUTRALITY_CHECK_WRT(op_tag, op_sign) \
template<class Type> \
void check_neutrality_wrt_##op_tag \
(const Type& a, const Type& neutron) \
{ \
	Type left  = a op_sign neutron; \
	Type right = neutron op_sign a; \
	BOOST_CHECK_EQUAL(left,right); \
}

#define CHECK_NEUTRALITY_WRT(op_tag) check_neutrality_wrt_##op_tag
#define CHECK_NEUTRALITY_WRT_EQUAL(op_tag) check_neutrality_wrt_equal_##op_tag

DEFINE_NEUTRALITY_CHECK_WRT      (plus, + );
DEFINE_NEUTRALITY_CHECK_WRT_EQUAL(plus, + );
DEFINE_NEUTRALITY_CHECK_WRT      (et, & );
DEFINE_NEUTRALITY_CHECK_WRT_EQUAL(et, & );

//------------------------------------------------------------------------------
// commutativity
//------------------------------------------------------------------------------

#define DEFINE_COMMUTATIVITY_CHECK_WRT_EQUAL(op_tag, op_sign) \
template<class Type, class TypeB> \
void check_commutativity_wrt_equal_##op_tag \
(typename equality<Type>::type* equal, const Type& a, const TypeB& b) \
{ \
	Type left  = a op_sign b; \
	Type right = b op_sign a; \
	BOOST_CHECK_EQUAL((*equal)(left,right), true); \
}

#define DEFINE_COMMUTATIVITY_CHECK_WRT(op_tag, op_sign) \
template<class Type, class TypeB> \
	void check_commutativity_wrt_##op_tag(const Type& a, const TypeB& b) \
{ \
	Type left  = a op_sign b; \
	Type right = b op_sign a; \
	BOOST_CHECK_EQUAL(left,right); \
}

#define CHECK_COMMUTATIVITY_WRT(op_tag) check_commutativity_wrt_##op_tag

DEFINE_COMMUTATIVITY_CHECK_WRT      (plus, + );
DEFINE_COMMUTATIVITY_CHECK_WRT_EQUAL(plus, + );

//------------------------------------------------------------------------------
// monodid EA
//------------------------------------------------------------------------------

#define DEFINE_MONOID_CHECK_WRT_EQUAL(op_tag) \
template<class Type, class TypeB, class TypeC> \
void check_monoid_wrt_equal_##op_tag \
(typename equality<Type>::type* equal, const Type& neutron, \
const Type& a, const TypeB& b, const TypeC& c) \
{ \
	CHECK_ASSOCIATIVITY_WRT_EQUAL(op_tag)(equal,a,b,c); \
	CHECK_NEUTRALITY_WRT_EQUAL(op_tag)(equal,a,neutron); \
}

#define DEFINE_MONOID_CHECK_WRT(op_tag) \
template<class Type, class TypeB, class TypeC> \
void check_monoid_wrt_##op_tag \
(typename const Type& neutron, \
const Type& a, const TypeB& b, const TypeC& c) \
{ \
	CHECK_ASSOCIATIVITY_WRT(op_tag)(a,b,c); \
	CHECK_NEUTRALITY_WRT(op_tag)(a,neutron); \
}

DEFINE_MONOID_CHECK_WRT(plus);
DEFINE_MONOID_CHECK_WRT_EQUAL(plus);
DEFINE_MONOID_CHECK_WRT(et);
DEFINE_MONOID_CHECK_WRT_EQUAL(et);

#define CHECK_MONOID_WRT(op_tag) check_monoid_wrt_##op_tag


#endif // __test_itl_laws_h_JOFA_080920__


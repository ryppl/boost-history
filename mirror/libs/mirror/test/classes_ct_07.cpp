/**
 * \file test/classes_ct_07.cpp
 *
 * This file is part of the Mirror library testsuite.
 *
 * Testing class registration and reflection.
 *
 * Tests the accumulate, at, size and  get_type_list algorighms
 * on own attributes and base classes of a reflected class
 *
 *  Copyright 2008 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

// MPL_ASSERT
#include <boost/mpl/assert.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/and.hpp>
#include <boost/mpl/accumulate.hpp>
#include <boost/mpl/push_back.hpp>
#include <boost/mpl/equal.hpp>
#include <boost/mpl/equal_to.hpp>
#include <boost/mpl/lambda.hpp>
//
#include <boost/mirror/meta_class.hpp>
#include <boost/mirror/algorithm/accumulate.hpp>
#include <boost/mirror/intrinsic/get_type_list.hpp>
//
#include "./namespaces.hpp"
#include "./classes.hpp"
#include "./test.hpp"

using namespace ::boost;
using namespace ::test;
using namespace ::boost::mirror;


template <typename MetaAttribute>
struct get_attrib_type
{
	typedef typename MetaAttribute::type type;
};

template <typename MetaInheritance>
struct get_base_class
{
	typedef typename MetaInheritance::base_class type;
};


/** Checks whether the reflected attribute type list
 *  is equal to the hard coded
 */
template <typename Tuple>
struct test_own_att_list_1
{
	// the class
	typedef typename mpl::at<Tuple, mpl::int_<0> >::type X;
	// the meta class 
	typedef BOOST_MIRRORED_CLASS(X) meta_X;

	// the list of meta-attributes for own attribs
	typedef typename mpl::at<Tuple, mpl::int_<1> >::type
		X_attrib_list;
	//
	// the reflected and the hard-coded lists need to be equal
	typedef typename mpl::equal<
		typename get_type_list<
			typename meta_X::attributes
		>::type,
		X_attrib_list
	>::type type;
};

/** Checks whether the attribute type list gathered as
 *  the result of the aggregate algorithm 
 *  on the own attributes of a class, is equal 
 *  to the hard coded list
 */
//
typedef mpl::lambda<
	mpl::push_back<
		mpl::_1,
		get_attrib_type< mpl::_2 >
       	>
>::type accumulate_attrib_types;

template <typename Tuple>
struct test_own_att_list_2
{
	// the class
	typedef typename mpl::at<Tuple, mpl::int_<0> >::type X;
	// the meta class 
	typedef BOOST_MIRRORED_CLASS(X) meta_X;

	// the list of meta-attributes for own attribs
	typedef typename mpl::at<Tuple, mpl::int_<1> >::type
		X_attrib_list;
	//
	//
        // use the accumulate algorithm to get the typelist
        typedef typename ::boost::mirror::accumulate<
                typename meta_X::attributes,
                mpl::vector0<>,
                accumulate_attrib_types
        >::type X_attrib_list_2;
	//
	typedef typename mpl::equal<
		X_attrib_list,
		X_attrib_list_2
	>::type type;
};

/** Test the attribute count returned by mirror
 */
template <typename Tuple>
struct test_own_att_count
{
	// the class
	typedef typename mpl::at<Tuple, mpl::int_<0> >::type X;
	// the meta class 
	typedef BOOST_MIRRORED_CLASS(X) meta_X;

	// the list of meta-attributes for own attribs
	typedef typename mpl::at<Tuple, mpl::int_<1> >::type
		X_attrib_list;
	//
	//
        // use the size algorithm to get the attrib count
	typedef typename mpl::equal_to<
		typename size<typename meta_X::attributes>::type,
		typename mpl::size<X_attrib_list>::type
	>::type type;
	
};


/** Checks whether the reflected base class list
 *  is equal to the hard coded
 */
template <typename Tuple>
struct test_base_class_list_1
{
	// the class
	typedef typename mpl::at<Tuple, mpl::int_<0> >::type X;
	// the meta class 
	typedef BOOST_MIRRORED_CLASS(X) meta_X;

	// the list of meta-attributes for own attribs
	typedef typename mpl::at<Tuple, mpl::int_<2> >::type
		X_base_class_list;
	//
	// the reflected and the hard-coded lists need to be equal
	typedef typename mpl::equal<
		typename get_type_list<
			typename meta_X::base_classes
		>::type,
		X_base_class_list
	>::type type;
};

/** Checks whether the base class list gathered as
 *  the result of the aggregate algorithm 
 *  on the base classes of a class, is equal 
 *  to the hard coded list
 */
//
typedef mpl::lambda<
	mpl::push_back<
		mpl::_1,
		get_base_class< mpl::_2 >
       	>
>::type accumulate_base_classes;

template <typename Tuple>
struct test_base_class_list_2
{
	// the class
	typedef typename mpl::at<Tuple, mpl::int_<0> >::type X;
	// the meta class 
	typedef BOOST_MIRRORED_CLASS(X) meta_X;

	// the list of meta-attributes for own attribs
	typedef typename mpl::at<Tuple, mpl::int_<2> >::type
		X_base_class_list;
	//
        // use the accumulate algorithm to get the typelist
        typedef typename ::boost::mirror::accumulate<
                typename meta_X::base_classes,
                mpl::vector0<>,
                accumulate_base_classes
        >::type X_base_class_list_2;
	//
	typedef typename mpl::equal<
		X_base_class_list,
		X_base_class_list_2
	>::type type;
};

/** Compares the reflected count of base classes to 
 *  the hard coded one
 */
template <typename Tuple>
struct test_base_class_count
{
	// the class
	typedef typename mpl::at<Tuple, mpl::int_<0> >::type X;
	// the meta class 
	typedef BOOST_MIRRORED_CLASS(X) meta_X;

	// the list of meta-attributes for own attribs
	typedef typename mpl::at<Tuple, mpl::int_<2> >::type
		X_base_class_list;
	//
	//
        // use the size algorithm to get the attrib count
	typedef typename mpl::equal_to<
		typename size<typename meta_X::base_classes>::type,
		typename mpl::size<X_base_class_list>::type
	>::type type;
	
};

/** helper meta-function template class used to 'run'
 *  the indivirual tests
 */
template <template <class> class Test>
struct exec_test
{
	template <typename Status, typename Tuple>
	struct op
	{
		typedef typename mpl::and_<
			typename Test<Tuple>::type,
			Status
		>::type type;
	};
};
/** 'Run' all the tests on the Elements of the 
 *  list defined in test_main()
 */
template <template <class> class Test>
void test_main_templ()
{
	typedef mpl::vector<
		mpl::vector<
			A, 
			mpl::vector3<long, long, long>,
			mpl::vector0<>
		>,
		mpl::vector<
			B,
			mpl::vector2<int, int>,
			mpl::vector1<A>
		>,
		mpl::vector<
			C,
			mpl::vector2<double, double>,
			mpl::vector1<A>
		>,
		mpl::vector<
			D,
			mpl::vector2<const short, volatile short>,
			mpl::vector1<A>
		>,
		mpl::vector<
			E,
			mpl::vector1<const float>,
			mpl::vector3<B, C, D>
		>,
		mpl::vector<
			F,
			mpl::vector2<bool, bool>,
			mpl::vector1<E>
		>,
		mpl::vector<
			G,
			mpl::vector3<char, char, char>,
			mpl::vector1<E>
		>,
		mpl::vector<
			H,
			mpl::vector3<wchar_t, wchar_t, wchar_t>,
			mpl::vector2<F, G>
		>,
		mpl::vector<
			I,
			mpl::vector1< ::std::string >,
			mpl::vector0<>
		>,
		mpl::vector<
			J,
			mpl::vector1< ::std::wstring >,
			mpl::vector0<>
		>,
		mpl::vector<
			K,
			mpl::vector2<int, int>,
			mpl::vector2<I, J>
		>
	> classes_attribs_and_base_classes;
	//
	typedef exec_test<Test> executer;
	typedef typename executer::template op< mpl::_1, mpl::_2> test_op;
	//
	// go through the list and execute 
	// the tests on each tuple <type, attribs, base_classes>
	BOOST_MPL_ASSERT((
		mpl::accumulate<
			classes_attribs_and_base_classes,
			mpl::true_,
			test_op
		>
	));
}

test_suite* init_unit_test_suite( int argc, char* argv[] )
{
    test_suite *test = BOOST_TEST_SUITE("Mirror: classes compile test suite 07");
    test->add(BOOST_TEST_CASE(&test_main_templ<test_own_att_list_1>));
    test->add(BOOST_TEST_CASE(&test_main_templ<test_own_att_list_2>));
    test->add(BOOST_TEST_CASE(&test_main_templ<test_own_att_count>));
    test->add(BOOST_TEST_CASE(&test_main_templ<test_base_class_list_1>));
    test->add(BOOST_TEST_CASE(&test_main_templ<test_base_class_list_2>));
    test->add(BOOST_TEST_CASE(&test_main_templ<test_base_class_count>));
    return test;
}



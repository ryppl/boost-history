/**
 * \file test/classes_ct_09.cpp
 *
 * This file is part of the Mirror library testsuite.
 *
 * Testing class registration and reflection.
 *
 *  Copyright 2008 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#include <boost/type_traits/is_same.hpp>
// assert
#include <boost/mpl/lambda.hpp>
// namespace registering 
#include <boost/mirror/meta_namespace.hpp>
// type registering 
#include <boost/mirror/meta_type.hpp>
// intrinsic meta-functions, iterators and algorithms
#include <boost/mirror/intrinsic/size.hpp>
#include <boost/mirror/intrinsic/at.hpp>
#include <boost/mirror/intrinsic/empty.hpp>
#include <boost/mirror/intrinsic/begin.hpp>
#include <boost/mirror/intrinsic/end.hpp>
#include <boost/mirror/iterators.hpp>
#include <boost/mirror/algorithm/find_if.hpp>
//
#include <boost/mirror/traits/reflects_global_scope.hpp>
#include <boost/mirror/traits/reflects_namespace.hpp>
#include <boost/mirror/traits/reflects_type.hpp>
//
#include "./test.hpp"

namespace test {

namespace a { } 
namespace b { } 
namespace c { } 
namespace d { } 
struct e { };
struct f { };
struct g { }; 
struct h { }; 
struct i { }; 
struct j { }; 

} // namespace test

namespace boost{
namespace mirror {

BOOST_MIRROR_REG_NAMESPACE((test))
BOOST_MIRROR_REG_NAMESPACE((test)(a))
BOOST_MIRROR_REG_NAMESPACE((test)(b))
BOOST_MIRROR_REG_NAMESPACE((test)(c))
BOOST_MIRROR_REG_NAMESPACE((test)(d))
BOOST_MIRROR_REG_TYPE(::test, e)
BOOST_MIRROR_REG_TYPE(::test, f)
BOOST_MIRROR_REG_TYPE(::test, g)
BOOST_MIRROR_REG_TYPE(::test, h)
BOOST_MIRROR_REG_TYPE(::test, i)
BOOST_MIRROR_REG_TYPE(::test, j)
} // namespace mirror
} // namespace boost


void test_main()
{
	using namespace ::boost;
	using namespace ::boost::mirror;
	//
	// define an alternative name for the global scope meta-data
	typedef BOOST_MIRRORED_NAMESPACE(::test) meta_test;
	//
#ifdef BOOST_MIRROR_NO_GLOBAL_LISTS
	// if the global lists are not available, then mirror does not
	// reflect namespace members
	//
	BOOST_MIRROR_ASSERT(
		empty< meta_test::members<>::type >,
		"The list of members of the test namespace should be empty"
	);
	BOOST_MIRROR_ASSERT_RELATION(
		size< meta_test::members<>::type >::value, 
		==, 
		0,
		"The count of members of the test namespace should be 0"
	);
	// check whether the imember terators returned by begin and end
	// for the test namespace should be equal
	typedef iterator_equal<
                begin<meta_test::members<>::type>::type,
                end<meta_test::members<>::type>::type
        >::type result_01;
	BOOST_MIRROR_ASSERT(
		result_01,
		"Member iterators returned by begin<> and end<> "\
		"for the test namespace should be equal"
	);
	// same as the previous check done in a different way
        typedef iterator_not_equal<
                begin<meta_test::members<>::type>::type,
                end<meta_test::members<>::type>::type
        >::type result_02;
	BOOST_MIRROR_ASSERT_NOT(
		result_02,
		"Member iterators returned by begin<> and end<> "\
		"for the test namespace should be equal"
	);	
	//
	// there should be no namespace among the members of test
        typedef iterator_equal<
                find_if<
			meta_test::members<>::type, 
			mpl::lambda<reflects_namespace<mpl::_1> >::type
		>::type,
                end<meta_test::members<>::type>::type
        >::type result_03;
	BOOST_MIRROR_ASSERT(
		result_03,
		"There should be no namespace among the members of test"
	);
	//
        typedef iterator_equal<
                find_if<
			meta_test::members<>::type, 
			mpl::lambda<reflects_global_scope<mpl::_1> >::type
		>::type,
                end<meta_test::members<>::type>::type
        >::type result_04;
	BOOST_MIRROR_ASSERT(
		result_04,
		"There should be no member of test namespace identified "\
		"as the global scope"
	);
#else
	BOOST_MIRROR_ASSERT_NOT(
		empty< meta_test::members<>::type >,
		"List of members of the test namespace should not be empty"
	);
	BOOST_MIRROR_ASSERT_RELATION( 
		size< meta_test::members<>::type >::value, 
		==, 
		10,
		"There should be 10 members in the test namespace"
	);

	typedef mpl::lambda<reflects_global_scope<mpl::_1> >::type filter_global_scope;
	typedef mpl::lambda<reflects_namespace<mpl::_1> >::type filter_namespaces;
	typedef mpl::lambda<reflects_type<mpl::_1> >::type filter_types;
	//
	BOOST_MIRROR_ASSERT_RELATION( 
		size< meta_test::members< filter_global_scope >::type >::value, 
		==, 
		0,
		"There should be 0 members identified as the global scope"
	);
	BOOST_MIRROR_ASSERT_RELATION( 
		size< meta_test::members< filter_namespaces >::type >::value, 
		==, 
		4,
		"There should be 4 nested namespaces in the test namespace"
	);
	BOOST_MIRROR_ASSERT_RELATION( 
		size< meta_test::members< filter_types >::type >::value, 
		==, 
		6,
		"There should be 6 types defined in the test namespace"
	);
	//
	typedef iterator_not_equal<
                begin<meta_test::members<>::type>::type,
                end<meta_test::members<>::type>::type
        >::type result_01;
        BOOST_MIRROR_ASSERT(
		result_01,
		"The meta-object iterators returned by begin<> and end<> "\
		"should point to different elements"
        );
	
        typedef iterator_equal<
                begin<meta_test::members<>::type>::type,
                end<meta_test::members<>::type>::type
        >::type result_02;
	BOOST_MIRROR_ASSERT_NOT(
		result_02,
		"The meta-object iterators returned by begin<> and end<> "\
		"should not point to the same element"
        );
	//
	typedef iterator_not_equal<
                begin<meta_test::members<filter_types>::type>::type,
                end<meta_test::members<filter_types>::type>::type
        >::type result_03;
        BOOST_MIRROR_ASSERT(
		result_03,
		"The meta-type iterators returned by begin<> and end<> "\
		"should point to different elements"
        );

	typedef iterator_equal<
                begin<meta_test::members<filter_types>::type>::type,
                end<meta_test::members<filter_types>::type>::type
        >::type result_04;
        BOOST_MIRROR_ASSERT_NOT(
		result_04,
		"The meta-type iterators returned by begin<> and end<> "\
                "should not point to the same element"
        );
	//
	typedef iterator_not_equal<
                begin<meta_test::members<filter_namespaces>::type>::type,
                end<meta_test::members<filter_namespaces>::type>::type
        >::type result_05;
        BOOST_MIRROR_ASSERT(
		result_05,
                "The meta-namespace iterators returned by begin<> and end<> "\
                "should point to different elements"
        );

	typedef iterator_equal<
                begin<meta_test::members<filter_namespaces>::type>::type,
                end<meta_test::members<filter_namespaces>::type>::type
        >::type result_06;
        BOOST_MIRROR_ASSERT_NOT(
		result_06,
                "The meta-namespace iterators returned by begin<> and end<> "\
                "should not point to the same element"
        );
	//
	typedef iterator_equal<
                find_if<
                        meta_test::members<>::type,
                        mpl::lambda<reflects_namespace<mpl::_1> >::type
                >::type,
                begin<meta_test::members<>::type>::type
        >::type result_07;
        BOOST_MIRROR_ASSERT(
		result_07,
		"The first member of the test namespace should be "\
		"a nested namespace"
        );
	//
	typedef iterator_equal<
                find_if<
                        meta_test::members<>::type,
                        mpl::lambda<reflects_global_scope<mpl::_1> >::type
                >::type,
                end<meta_test::members<>::type>::type
        >::type result_08;
        BOOST_MIRROR_ASSERT(
		result_08,
		"There should be no member of the test namespace "\
		"identified as the global scope"
        );
	//
	typedef iterator_equal<
                find_if<
                        meta_test::members<filter_types>::type,
                        mpl::lambda<reflects_namespace<mpl::_1> >::type
                >::type,
                end<meta_test::members<filter_types>::type>::type
        >::type result_09;
        BOOST_MIRROR_ASSERT(
		result_09,
		"There should be no namespace among the type members "\
		"of the test namespace"
        );
 
#endif
}

test_suite* init_unit_test_suite( int argc, char* argv[] )
{
    test_suite *test = BOOST_TEST_SUITE("Mirror: classes compile test 09");
    test->add(BOOST_TEST_CASE(&test_main));
    return test;
}



/**
 * \file test/namespaces_ct_05.cpp
 *
 * This file is part of the Mirror library testsuite.
 *
 * Testing namespaces registration and reflection.
 *
 *  Copyright 2008 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#include <boost/type_traits/is_same.hpp>
// assert
#include <boost/mpl/equal.hpp>
#include <boost/mpl/accumulate.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/front.hpp>
#include <boost/mpl/back.hpp>
#include <boost/mpl/and.hpp>
#include <boost/mpl/lambda.hpp>
// namespace registering 
#include <boost/mirror/meta_namespace.hpp>
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
//
#include "./test.hpp"

namespace test {

namespace a { } 
namespace b { } 
namespace c { } 
namespace d { } 
namespace e { } 
namespace f { } 
namespace g { } 
namespace h { } 

} // namespace test

namespace boost{
namespace mirror {

BOOST_MIRROR_REG_NAMESPACE((test))
BOOST_MIRROR_REG_NAMESPACE((test)(a))
BOOST_MIRROR_REG_NAMESPACE((test)(b))
BOOST_MIRROR_REG_NAMESPACE((test)(c))
BOOST_MIRROR_REG_NAMESPACE((test)(d))
BOOST_MIRROR_REG_NAMESPACE((test)(e))
BOOST_MIRROR_REG_NAMESPACE((test)(f))
BOOST_MIRROR_REG_NAMESPACE((test)(g))
BOOST_MIRROR_REG_NAMESPACE((test)(h))

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
		(empty< meta_test::members<>::type >),
		"The list of members of the ::test namespace should be empty"
	);

	BOOST_MIRROR_ASSERT_RELATION(
		size< meta_test::members<>::type >::value, 
		==, 
		0,
		"The count of members of the ::test namespace should be 0"
	);

	typedef iterator_equal<
                begin<meta_test::members<>::type>::type,
                end<meta_test::members<>::type>::type
        >::type result_01;
        BOOST_MIRROR_ASSERT(
		(result_01),
		"The iterators returned by begin<> and end<> should be equal"
        );

	typedef iterator_not_equal<
                begin<meta_test::members<>::type>::type,
                end<meta_test::members<>::type>::type
        >::type result_02;
        BOOST_MIRROR_ASSERT_NOT(
		(result_02),
		"The iterators returned by begin<> and end<> should "\
		"not be different"
        );

	typedef iterator_equal<
                find_if<
                        meta_test::members<>::type,
                        mpl::lambda<reflects_namespace<mpl::_1> >::type
                >::type,
                end<meta_test::members<>::type>::type
        >::type result_03;
        BOOST_MIRROR_ASSERT(
		(result_03),
		"There should be no namespace members in the ::test namespace"
        );

	typedef iterator_equal<
                find_if<
                        meta_test::members<>::type,
                        mpl::lambda<reflects_global_scope<mpl::_1> >::type
                >::type,
                end<meta_test::members<>::type>::type
        >::type result_04;
        BOOST_MIRROR_ASSERT(
		(result_04),
		"There should be no type members in the ::test namespace"
        );
#else
	typedef mpl::vector<
		mpl::vector2<BOOST_MIRRORED_NAMESPACE(::test::a), mpl::int_<0> >,
		mpl::vector2<BOOST_MIRRORED_NAMESPACE(::test::b), mpl::int_<1> >,
		mpl::vector2<BOOST_MIRRORED_NAMESPACE(::test::c), mpl::int_<2> >,
		mpl::vector2<BOOST_MIRRORED_NAMESPACE(::test::d), mpl::int_<3> >,
		mpl::vector2<BOOST_MIRRORED_NAMESPACE(::test::e), mpl::int_<4> >,
		mpl::vector2<BOOST_MIRRORED_NAMESPACE(::test::f), mpl::int_<5> >,
		mpl::vector2<BOOST_MIRRORED_NAMESPACE(::test::g), mpl::int_<6> >,
		mpl::vector2<BOOST_MIRRORED_NAMESPACE(::test::h), mpl::int_<7> >
	> meta_namespaces_and_indices;

	typedef mpl::accumulate<
		meta_namespaces_and_indices,
		mpl::vector0<>,
		mpl::push_back<
			mpl::_1,
			mpl::front<mpl::_2>
		>
	>::type meta_namespaces;

	BOOST_MIRROR_ASSERT_NOT(
		(empty< meta_test::members<>::type >),
		"The list of members of the ::test namespace should not "\
		"be empty"
	);
	
	BOOST_MIRROR_ASSERT_RELATION( 
		size< meta_test::members<>::type >::value, 
		==, 
		8,
		"There should be 8 members in the ::test namespace"
	);

	typedef mpl::accumulate<
		meta_namespaces_and_indices,
		mpl::true_,
		mpl::and_<
			mpl::_1,
			is_same<
				at<
					meta_test::members<>::type,
					mpl::back<mpl::_2>
				>,
				mpl::front<mpl::_2>
			>
		>
	>::type result_01;
	BOOST_MIRROR_ASSERT( 
		(result_01),
		"The position of the members in the ::test namespace "\
		"should match the hardcoded order"
	);
	//
	typedef iterator_not_equal<
                begin<meta_test::members<>::type>::type,
                end<meta_test::members<>::type>::type
        >::type result_02;
        BOOST_MIRROR_ASSERT(
		(result_02),
		"The iterators returned by begin<> and end<> on the list "\
		"of members of the ::test namespace should be different"
        );
	//
	typedef iterator_equal<
                begin<meta_test::members<>::type>::type,
                end<meta_test::members<>::type>::type
        >::type result_03;
        BOOST_MIRROR_ASSERT_NOT(
		(result_03),
		"The iterators returned by begin<> and end<> on the list "\
		"of members of the ::test namespace should not be equal"
        );
	//
	typedef iterator_equal<
                find_if<
                        meta_test::members<>::type,
                        mpl::lambda<reflects_namespace<mpl::_1> >::type
                >::type,
                begin<meta_test::members<>::type>::type
        >::type result_04;
        BOOST_MIRROR_ASSERT(
		(result_04),
		"The first member of the ::test namespace should "\
		"be a namespace"
        );
	//
	typedef iterator_equal<
                find_if<
                        meta_test::members<>::type,
                        mpl::lambda<reflects_global_scope<mpl::_1> >::type
                >::type,
                end<meta_test::members<>::type>::type
        >::type result_05;
        BOOST_MIRROR_ASSERT(
		(result_05),
		"There should not be any member in the ::test namespace "\
		"identified as the global scope"
        );
 
#endif
}

test_suite* init_unit_test_suite( int argc, char* argv[] )
{
    test_suite *test = BOOST_TEST_SUITE("Mirror: namespaces compile test 05");
    test->add(BOOST_TEST_CASE(&test_main));
    return test;
}



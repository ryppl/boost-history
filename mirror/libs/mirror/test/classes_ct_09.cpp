/**
 * \file test/namespaces_ct_05.cpp
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
	BOOST_MPL_ASSERT( empty< meta_test::members<>::type > );
	BOOST_MPL_ASSERT_RELATION( size< meta_test::members<>::type >::value, ==, 0);
        BOOST_MPL_ASSERT((iterator_equal<
                begin<meta_test::members<>::type>::type,
                end<meta_test::members<>::type>::type
        >));
        BOOST_MPL_ASSERT_NOT((iterator_not_equal<
                begin<meta_test::members<>::type>::type,
                end<meta_test::members<>::type>::type
        >));
        BOOST_MPL_ASSERT((iterator_equal<
                find_if<
			meta_test::members<>::type, 
			mpl::lambda<reflects_namespace<mpl::_1> >::type
		>::type,
                end<meta_test::members<>::type>::type
        >));
        BOOST_MPL_ASSERT((iterator_equal<
                find_if<
			meta_test::members<>::type, 
			mpl::lambda<reflects_global_scope<mpl::_1> >::type
		>::type,
                end<meta_test::members<>::type>::type
        >));
#else
	BOOST_MPL_ASSERT_NOT( empty< meta_test::members<>::type > );
	BOOST_MPL_ASSERT_RELATION( 
		size< meta_test::members<>::type >::value, 
		==, 
		10
	);

	typedef mpl::lambda<reflects_global_scope<mpl::_1> >::type filter_global_scope;
	typedef mpl::lambda<reflects_namespace<mpl::_1> >::type filter_namespaces;
	typedef mpl::lambda<reflects_type<mpl::_1> >::type filter_types;
	//
	BOOST_MPL_ASSERT_RELATION( 
		size< meta_test::members< filter_global_scope >::type >::value, 
		==, 
		0
	);
	BOOST_MPL_ASSERT_RELATION( 
		size< meta_test::members< filter_namespaces >::type >::value, 
		==, 
		4
	);
	BOOST_MPL_ASSERT_RELATION( 
		size< meta_test::members< filter_types >::type >::value, 
		==, 
		6
	);
	//
        BOOST_MPL_ASSERT((iterator_not_equal<
                begin<meta_test::members<>::type>::type,
                end<meta_test::members<>::type>::type
        >));
        BOOST_MPL_ASSERT_NOT((iterator_equal<
                begin<meta_test::members<>::type>::type,
                end<meta_test::members<>::type>::type
        >));
	//
        BOOST_MPL_ASSERT((iterator_not_equal<
                begin<meta_test::members<filter_types>::type>::type,
                end<meta_test::members<filter_types>::type>::type
        >));
        BOOST_MPL_ASSERT_NOT((iterator_equal<
                begin<meta_test::members<filter_types>::type>::type,
                end<meta_test::members<filter_types>::type>::type
        >));
	//
        BOOST_MPL_ASSERT((iterator_not_equal<
                begin<meta_test::members<filter_namespaces>::type>::type,
                end<meta_test::members<filter_namespaces>::type>::type
        >));
        BOOST_MPL_ASSERT_NOT((iterator_equal<
                begin<meta_test::members<filter_namespaces>::type>::type,
                end<meta_test::members<filter_namespaces>::type>::type
        >));
	//
        BOOST_MPL_ASSERT((iterator_equal<
                find_if<
			meta_test::members<>::type, 
			mpl::lambda<reflects_namespace<mpl::_1> >::type
		>::type,
                begin<meta_test::members<>::type>::type
        >));
        BOOST_MPL_ASSERT((iterator_equal<
                find_if<
			meta_test::members<>::type, 
			mpl::lambda<reflects_global_scope<mpl::_1> >::type
		>::type,
                end<meta_test::members<>::type>::type
        >));
	//
        BOOST_MPL_ASSERT((iterator_equal<
                find_if<
			meta_test::members<filter_types>::type, 
			mpl::lambda<reflects_namespace<mpl::_1> >::type
		>::type,
                end<meta_test::members<filter_types>::type>::type
        >));
 
#endif
}

test_suite* init_unit_test_suite( int argc, char* argv[] )
{
    test_suite *test = BOOST_TEST_SUITE("Mirror: classes compile test 09");
    test->add(BOOST_TEST_CASE(&test_main));
    return test;
}



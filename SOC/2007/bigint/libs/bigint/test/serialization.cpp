/* Boost serialization.cpp test file
 *
 * Copyright 2007 Arseny Kapoulkine
 *
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or
 * copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#define BOOST_DISABLE_WIN32

#include <boost/test/included/test_exec_monitor.hpp>

#include <boost/bigint/bigint.hpp>
#include <boost/bigint/bigint_serialize.hpp>

#ifdef BOOST_BIGINT_HAS_GMP_SUPPORT
#	include <boost/bigint/bigint_gmp.hpp>
#endif

#include <boost/bigint/bigint_default.hpp>
#include <boost/bigint/bigint_storage_vector.hpp>
#include <boost/bigint/bigint_storage_fixed.hpp>

#include <sstream>

#include <boost/archive/xml_iarchive.hpp>
#include <boost/archive/xml_oarchive.hpp>

#pragma comment(lib, "libgmp-3.lib")

template <typename I> void test_number(const char* value)
{
	typedef boost::bigint_base<I> number;

	number a(value);
	std::stringstream ss;

	BOOST_CHECK_EQUAL(a.str(), value);
	
	// make an archive
	boost::archive::xml_oarchive oa(ss);
	oa << BOOST_SERIALIZATION_NVP(a);

	BOOST_CHECK_EQUAL(a.str(), value);

	a = number("1390139412349084098234098034958039485049589045340912304912304");
	BOOST_CHECK(a.str() != value);

	boost::archive::xml_iarchive ia(ss);
	ia >> BOOST_SERIALIZATION_NVP(a);
	
	BOOST_CHECK_EQUAL(a.str(), value);
}

template <typename I> void test()
{
	// small
	test_number<I>("1");
	test_number<I>("0");
	test_number<I>("-1");

	// big
	test_number<I>("-1402341340123481239481902384190234801923480123848091234");
	test_number<I>("20459234592043729834379238457823453409523049453848091234");
}

int test_main(int argc, char* argv[])
{
#ifdef BOOST_BIGINT_HAS_GMP_SUPPORT
	test<boost::detail::bigint_gmp_implementation>();
#endif

	test<boost::detail::bigint_default_implementation<boost::detail::bigint_storage_vector, 8> >();
	test<boost::detail::bigint_default_implementation<boost::detail::bigint_storage_vector, 16> >();
	test<boost::detail::bigint_default_implementation<boost::detail::bigint_storage_vector, 32> >();

	test<boost::detail::bigint_default_implementation<boost::detail::bigint_storage_fixed<1024>::type, 8> >();
	test<boost::detail::bigint_default_implementation<boost::detail::bigint_storage_fixed<1024>::type, 16> >();
	test<boost::detail::bigint_default_implementation<boost::detail::bigint_storage_fixed<1024>::type, 32> >();

	return 0;
}

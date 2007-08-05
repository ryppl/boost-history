/* Boost number_conversion.cpp test file
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

#ifdef BOOST_BIGINT_HAS_GMP_SUPPORT
#	include <boost/bigint/bigint_gmp.hpp>
#endif

#include <boost/bigint/bigint_default.hpp>
#include <boost/bigint/bigint_storage_vector.hpp>
#include <boost/bigint/bigint_storage_fixed.hpp>

#include <sstream>

#pragma comment(lib, "libgmp-3.lib")

// This macro is not quite good, but - it's ok for our needs
#define ARRAY_SIZE(array) sizeof(array) / sizeof(array[0])

template <typename T> T convert_to_number(T value)
{
	return value;
}

int convert_to_number(char value)
{
	return value;
}

unsigned int convert_to_number(unsigned char value)
{
	return value;
}

template <typename I, typename T> void test_number_ctors(T* values, size_t count)
{
	typedef boost::bigint_base<I> number;

	for (size_t i = 0; i < count; ++i)
	{
		std::ostringstream oss;
		oss << convert_to_number(values[i]);

		// number -> bigint
		number v(values[i]);
		BOOST_CHECK_EQUAL(oss.str(), v.str());

		// bigint -> number
		BOOST_CHECK(v.template can_convert_to<T>()); // we should be able to convert to T
		BOOST_CHECK_EQUAL(convert_to_number(v.template to_number<T>()), convert_to_number(values[i]));
	}
}

template <typename I> void test()
{
	{
		char values[] = {-128, -127, -10, -1, 0, 1, 33, 124, 125, 126, 127};

		test_number_ctors<I>(values, ARRAY_SIZE(values));
	}

	{
		unsigned char values[] = {0, 1, 33, 124, 125, 126, 127, 200, 240, 254, 255};
		
		test_number_ctors<I>(values, ARRAY_SIZE(values));
	}

	{
		short values[] = {-32768, -32767, -23032, -3407, -10, -1, 0, 1, 33, 124, 125, 126, 127, 3489, 31900, 32766, 32767};

		test_number_ctors<I>(values, ARRAY_SIZE(values));
	}

	{
		unsigned short values[] = {0, 1, 33, 124, 125, 126, 127, 200, 240, 254, 255, 3000, 48950, 65534, 65535};
		
		test_number_ctors<I>(values, ARRAY_SIZE(values));
	}
	
	{
		int values[] = {-2147483647 - 1, -2147483647, -2147483646, -34294039, -3409, -1, 0, 1, 3940, 4950424, 2147483646, 2147483647};

		test_number_ctors<I>(values, ARRAY_SIZE(values));
	}

	{
		unsigned int values[] = {0, 1, 200, 65535, 384983, 23849384, 1203002930, 2147483648u, 4294967294u, 4294967295u};
		
		test_number_ctors<I>(values, ARRAY_SIZE(values));
	}
	
	{
		boost::int64_t values[] = {-2147483647 - 1, -2147483647, -2147483646, -34294039, -3409, -1, 0, 1, 3940, 4950424, 2147483646, 2147483647};

		// small values
		test_number_ctors<I>(values, ARRAY_SIZE(values));

		for (size_t i = 0; i < ARRAY_SIZE(values); ++i)
		{
			values[i] *= 2147483648u; // 2^31
			values[i] *= 2;          // 2^32
		}

		// first element is -2^31 * 2^32 == -2^63 - ok
		// last element is (2^31 - 1) * 2^32 == 2^63 - 2^32 - too small
		values[ARRAY_SIZE(values) - 1] += 4294967295u;

		// testing unit tests
		BOOST_CHECK(values[0] < 0 && values[0] - 1 > 0); // underflow
		BOOST_CHECK(values[ARRAY_SIZE(values) - 1] > 0 && values[ARRAY_SIZE(values) - 1] + 1 < 0); // overflow
		BOOST_CHECK_EQUAL(values[0] - 1, values[ARRAY_SIZE(values) - 1]);
		
		// large values
		test_number_ctors<I>(values, ARRAY_SIZE(values));
	}

	{
		boost::uint64_t values[] = {0, 1, 200, 65535, 384983, 23849384, 1203002930, 2147483648u, 4294967294u, 4294967295u};

		// small values
		test_number_ctors<I>(values, ARRAY_SIZE(values));

		for (size_t i = 0; i < ARRAY_SIZE(values); ++i)
		{
			values[i] *= 2147483648u; // 2^31
			values[i] *= 2;          // 2^32
		}

		// last element is (2^32 - 1) * 2^32 == 2^64 - 2^32 - too small
		values[ARRAY_SIZE(values) - 1] += 4294967295u;

		// testing unit tests
		BOOST_CHECK_EQUAL(values[ARRAY_SIZE(values) - 1] + 1, 0); // overflow
		
		// large values
		test_number_ctors<I>(values, ARRAY_SIZE(values));
	}
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

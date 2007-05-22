// Boost.Print library

// Copyright Jared McIntyre 2007. Use, modification and
// distribution is subject to the Boost Software License, Version
// 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

// For more information, see http://www.boost.org

#define BOOST_TEST_MODULE PrintLib
#include <boost/test/unit_test.hpp>
#include <string>
#include <sstream>
#include <boost/tuple/tuple.hpp>
#include "../../../boost/explore/explore.hpp"

BOOST_AUTO_TEST_CASE( basic_tuple_test )
{
	std::stringstream str_out;
	
	boost::tuple<int, double> t(1, 3.14);
	explore::print(t, str_out);
	BOOST_CHECK_EQUAL(str_out.str(), "[1, 3.14]");
}
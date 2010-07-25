//  Boost count_leading_zeros_test.hpp test program  --------------------------------------//

//  (C) Copyright Murilo Adriano Vasconcelos 2010.
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt


#include <iostream>
#include <cmath>
#include <limits>
#include <boost/cstdint.hpp>
#include <boost/detail/lightweight_test.hpp>
#include <boost/integer/isqrt.hpp>



int main()
{
	
	for (uint8_t i = 0; i < 255; ++i) {
		BOOST_TEST((boost::isqrt(i) == int(sqrt(i))));
	}
	
	for (uint16_t i = 0x0; i < 0xFF; ++i) {
		BOOST_TEST((boost::isqrt(i) == int(sqrt(i))));
	}
	
	for (uint16_t i = 0x0FFF; i < 0xFFFF; ++i) {
		BOOST_TEST((boost::isqrt(i) == int(sqrt(i))));
	}
	
	for (uint32_t i = 0x0; i < 0xFF; ++i) {
		BOOST_TEST((boost::isqrt(i) == int(sqrt(i))));
	}
	
	for (uint32_t i = 0xFFFF; i < 0xFFFFF; ++i) {
		BOOST_TEST((boost::isqrt(i) == int(sqrt(i))));
	}
	
	BOOST_TEST((boost::isqrt(unsigned(0)) == int(sqrt(0))));
	BOOST_TEST((boost::isqrt(unsigned(1)) == int(sqrt(1))));
	BOOST_TEST((boost::isqrt(unsigned(2)) == int(sqrt(2))));
	BOOST_TEST((boost::isqrt(unsigned(3)) == int(sqrt(3))));
	BOOST_TEST((boost::isqrt(std::numeric_limits<uint8_t>::max()) 
				== int(sqrt(std::numeric_limits<uint8_t>::max()))));
	
	BOOST_TEST((boost::isqrt(std::numeric_limits<uint16_t>::max()) 
				== int(sqrt(std::numeric_limits<uint16_t>::max()))));
	
	BOOST_TEST((boost::isqrt(std::numeric_limits<uint32_t>::max()) 
				== int(sqrt(std::numeric_limits<uint32_t>::max()))));

	return boost::report_errors();
}

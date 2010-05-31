//  Boost bit_reversal_test.hpp test program  --------------------------------------//

//  (C) Copyright Murilo Adriano Vasconcelos 2010.
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

#include <boost/detail/lightweight_test.hpp>

#include <boost/cstdlib.hpp>              // for boost::exit_success
#include <boost/integer/bits_and_ints.hpp> 

#include <iostream>  // for std::cout (std::endl indirectly)


// Macros to compact code
#define BIT_REVERSAL_TEST(n, m) \
	BOOST_TEST(::boost::bit_reversal(n) == m)

#define STATIC_BIT_REVERSAL_TEST(t, n, m) \
	BOOST_TEST((::boost::static_bit_reversal<t, n>::value == m))


// Main testing function
int main(int, char* [])
{    
	std::cout << "Doing tests on bit_reversal." << std::endl;
	
	BIT_REVERSAL_TEST(unsigned(0), unsigned(0));
	BIT_REVERSAL_TEST(unsigned(1), unsigned(1 << 31));
	
	for (unsigned i = 2, val, acc = 2; i < 32; ++i) {
		val = unsigned(1 << (32 - i));
		BIT_REVERSAL_TEST(acc, val);
		acc *= 2;
	}
	
	STATIC_BIT_REVERSAL_TEST(unsigned, 0, 0);
	STATIC_BIT_REVERSAL_TEST(unsigned, 1, unsigned(1) << 31);
	STATIC_BIT_REVERSAL_TEST(unsigned, 2, unsigned(1) << 30);
	STATIC_BIT_REVERSAL_TEST(unsigned, 4, unsigned(1) << 29);
	STATIC_BIT_REVERSAL_TEST(unsigned, 8, unsigned(1) << 28);	
	STATIC_BIT_REVERSAL_TEST(unsigned, 16, unsigned(1) << 27);
	STATIC_BIT_REVERSAL_TEST(unsigned, 32, unsigned(1) << 26);
	STATIC_BIT_REVERSAL_TEST(unsigned, 64, unsigned(1) << 25);
	STATIC_BIT_REVERSAL_TEST(unsigned, 128, unsigned(1) << 24);
	STATIC_BIT_REVERSAL_TEST(unsigned, 256, unsigned(1) << 23);
	STATIC_BIT_REVERSAL_TEST(unsigned, 512, unsigned(1) << 22);
	STATIC_BIT_REVERSAL_TEST(unsigned, 1024, unsigned(1) << 21);
	STATIC_BIT_REVERSAL_TEST(unsigned, 2048, unsigned(1) << 20);
	STATIC_BIT_REVERSAL_TEST(unsigned, 4096, unsigned(1) << 19);
	STATIC_BIT_REVERSAL_TEST(unsigned, 8192, unsigned(1) << 18);	
	STATIC_BIT_REVERSAL_TEST(unsigned, 16384, unsigned(1) << 17);
	STATIC_BIT_REVERSAL_TEST(unsigned, 32768, unsigned(1) << 16);
	STATIC_BIT_REVERSAL_TEST(unsigned, 65536, unsigned(1) << 15);
	STATIC_BIT_REVERSAL_TEST(unsigned, 131072, unsigned(1) << 14);
	STATIC_BIT_REVERSAL_TEST(unsigned, 262144, unsigned(1) << 13);
	STATIC_BIT_REVERSAL_TEST(unsigned, 524288, unsigned(1) << 12);
	STATIC_BIT_REVERSAL_TEST(unsigned, 1048576, unsigned(1) << 11);
	STATIC_BIT_REVERSAL_TEST(unsigned, 2097152, unsigned(1) << 10);
	STATIC_BIT_REVERSAL_TEST(unsigned, 4194304, unsigned(1) << 9);
	STATIC_BIT_REVERSAL_TEST(unsigned, 8388608, unsigned(1) << 8);	
	STATIC_BIT_REVERSAL_TEST(unsigned, 16777216, unsigned(1) << 7);
	STATIC_BIT_REVERSAL_TEST(unsigned, 33554432, unsigned(1) << 6);
	STATIC_BIT_REVERSAL_TEST(unsigned, 67108864, unsigned(1) << 5);
	STATIC_BIT_REVERSAL_TEST(unsigned, 134217728, unsigned(1) << 4);
	STATIC_BIT_REVERSAL_TEST(unsigned, 268435456, unsigned(1) << 3);
	STATIC_BIT_REVERSAL_TEST(unsigned, 536870912, unsigned(1) << 2);
	STATIC_BIT_REVERSAL_TEST(unsigned, 1073741824, unsigned(1) << 1);
	STATIC_BIT_REVERSAL_TEST(unsigned, 2147483648, unsigned(1));

	
#ifndef BOOST_NO_INT64_T
	BIT_REVERSAL_TEST(uint64_t(0), uint64_t(0));
	BIT_REVERSAL_TEST(uint64_t(1), uint64_t(1) << 63);
	
	uint64_t acc = 2, val;
	for (unsigned i = 2; i < 64; ++i) {
		val = uint64_t(1) << (64 - i);
		BIT_REVERSAL_TEST(acc, val);
		acc *= 2;
	}
	
	STATIC_BIT_REVERSAL_TEST(uint64_t, 0, 0);
	STATIC_BIT_REVERSAL_TEST(uint64_t, 1, uint64_t(1) << 63);
	STATIC_BIT_REVERSAL_TEST(uint64_t, 2, uint64_t(1) << 62);
	STATIC_BIT_REVERSAL_TEST(uint64_t, 4, uint64_t(1) << 61);
	STATIC_BIT_REVERSAL_TEST(uint64_t, 8, uint64_t(1) << 60);	
	STATIC_BIT_REVERSAL_TEST(uint64_t, 16, uint64_t(1) << 59);
	STATIC_BIT_REVERSAL_TEST(uint64_t, 32, uint64_t(1) << 58);
	STATIC_BIT_REVERSAL_TEST(uint64_t, 64, uint64_t(1) << 57);
	STATIC_BIT_REVERSAL_TEST(uint64_t, 128, uint64_t(1) << 56);
	STATIC_BIT_REVERSAL_TEST(uint64_t, 256, uint64_t(1) << 55);
	STATIC_BIT_REVERSAL_TEST(uint64_t, 512, uint64_t(1) << 54);
	STATIC_BIT_REVERSAL_TEST(uint64_t, 1024, uint64_t(1) << 53);
	STATIC_BIT_REVERSAL_TEST(uint64_t, 2048, uint64_t(1) << 52);
	STATIC_BIT_REVERSAL_TEST(uint64_t, 4096, uint64_t(1) << 51);
	STATIC_BIT_REVERSAL_TEST(uint64_t, 8192, uint64_t(1) << 50);	
	STATIC_BIT_REVERSAL_TEST(uint64_t, 16384, uint64_t(1) << 49);
	STATIC_BIT_REVERSAL_TEST(uint64_t, 32768, uint64_t(1) << 48);
	STATIC_BIT_REVERSAL_TEST(uint64_t, 65536, uint64_t(1) << 47);
	STATIC_BIT_REVERSAL_TEST(uint64_t, 131072, uint64_t(1) << 46);
	STATIC_BIT_REVERSAL_TEST(uint64_t, 262144, uint64_t(1) << 45);
	STATIC_BIT_REVERSAL_TEST(uint64_t, 524288, uint64_t(1) << 44);
	STATIC_BIT_REVERSAL_TEST(uint64_t, 1048576, uint64_t(1) << 43);
	STATIC_BIT_REVERSAL_TEST(uint64_t, 2097152, uint64_t(1) << 42);
	STATIC_BIT_REVERSAL_TEST(uint64_t, 4194304, uint64_t(1) << 41);
	STATIC_BIT_REVERSAL_TEST(uint64_t, 8388608, uint64_t(1) << 40);	
	STATIC_BIT_REVERSAL_TEST(uint64_t, 16777216, uint64_t(1) << 39);
	STATIC_BIT_REVERSAL_TEST(uint64_t, 33554432, uint64_t(1) << 38);
	STATIC_BIT_REVERSAL_TEST(uint64_t, 67108864, uint64_t(1) << 37);
	STATIC_BIT_REVERSAL_TEST(uint64_t, 134217728, uint64_t(1) << 36);
	STATIC_BIT_REVERSAL_TEST(uint64_t, 268435456, uint64_t(1) << 35);
	STATIC_BIT_REVERSAL_TEST(uint64_t, 536870912, uint64_t(1) << 34);
#endif
	
	return boost::report_errors();
}


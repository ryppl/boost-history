/* Boost modmul_test.cpp header file
 *
 * Copyright 2007 Arseny Kapoulkine
 *
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or
 * copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#include <iostream>
#include <boost/cstdint.hpp>

using boost::uint32_t;

#include "fft_primes.hpp"

unsigned int modmul_native(unsigned int a, unsigned int b, unsigned int prime)
{
	return static_cast<unsigned int>(static_cast<boost::uint64_t>(a) * b % prime);
}

unsigned int modmul_fast(unsigned int a, unsigned int b, unsigned int prime, double inv_prime)
{
	int r = a * b - prime * static_cast<unsigned int>(inv_prime * static_cast<int>(a) * b);
	r = (r < 0 ? r + prime : r > prime ? r - prime : r);
	return r;
}

int main()
{
	const unsigned int step_a = 1000;
	const unsigned int step_b = 1000;

	std::cout << modmul_native(126026000, 1353842000, 2013265921) << std::endl;
	std::cout << modmul_fast(126026000, 1353842000, 2013265921, 1.0 / 2013265921) << std::endl;

	for (int p = 0; p < 2; ++p)
	{
		unsigned int prime = fft_primes[p];
		double inv_prime = 1.0 / prime;

		std::cout << "Testing prime " << prime << std::endl;

		for (unsigned int a = 0; a < prime; a += step_a)
		{
			if (a % 700000 == 0) std::cout << ".";

			for (unsigned int b = 0; b < prime; b += step_b)
			{
				if (modmul_native(a, b, prime) != modmul_fast(a, b, prime, inv_prime))
				{
					std::cout << std::endl;
					std::cout << "Test failed! a = " << a << ", b = " << b << ", prime = " << prime << std::endl;
					exit(0);
				}
			}
		}
	}
}

/* Boost modmul_test.cpp header file
 *
 * Copyright 2007 Arseny Kapoulkine
 *
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or
 * copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#include <iostream>

#include "fft_primes.hpp"

unsigned int modmul_native(unsigned int a, unsigned int b, unsigned int prime)
{
	return static_cast<unsigned int>(static_cast<unsigned __int64>(a) * b % prime);
}

unsigned int modmul_fast(unsigned int a, unsigned int b, unsigned int prime, double inv_prime)
{
	int r = a * b - prime * static_cast<unsigned int>(inv_prime * static_cast<int>(a) * b);
	r = (r < 0 ? r + prime : r);
	return r;
}

int main()
{
	const unsigned int step_a = 100;
	const unsigned int step_b = 100;

	for (int p = 0; p < 2; ++p)
	{
		unsigned int prime = fft_primes[p];
		double inv_prime = 1.0 / prime;

		std::cout << "Testing prime " << prime << std::endl;

		for (unsigned int a = 0; a < prime; a += step_a)
		{
			std::cout << ".";

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

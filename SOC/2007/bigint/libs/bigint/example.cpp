/* Boost bigint example.cpp test file
 *
 * Copyright 2007 Arseny Kapoulkine
 *
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or
 * copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#include <boost/bigint/bigint.hpp>

#include <iostream>

using boost::bigint;

// Raise a to the power n for log(n)
template <typename T> inline const T power_logN(const T& a, unsigned int n)
{
	if (n <= 0) return 1;
 
	T result(1);

	unsigned int power_of_two = 1;
  
	// Find largest power of two that is > n
	while (power_of_two < n && (power_of_two << 1) != 0)
		power_of_two <<= 1;

	if (power_of_two > n)
		power_of_two >>= 1;
  	else if (power_of_two == n) // handle large n
	{
		power_of_two >>= 1;
		result = a;
	}
 
	while (power_of_two > 0)
	{
		if ((n & power_of_two) != 0)
		{
			result *= result;
			result *= a;
		}
		else
			result *= result;
  
		power_of_two >>= 1;
	}
 
	return result;
}

struct mat22
{
	bigint m00, m01;
	bigint m10, m11;

	mat22()
	{
	}
	
	mat22(int): m00(1), m01(0),
				m10(0), m11(1)
	{
	}
              
	const mat22& operator*=(const mat22& m)
	{
		mat22 result;
    
		result.m00 = m00 * m.m00 + m01 * m.m10;
		result.m01 = m00 * m.m01 + m01 * m.m11;

		result.m10 = m10 * m.m00 + m11 * m.m10;
		result.m11 = m10 * m.m01 + m11 * m.m11;

		*this = result;

		return *this;
	}              
};

const bigint fibonacci(unsigned int n)
{
	if (n == 0) return 0;
	else if (n < 3) return 1;

	mat22 base;
	base.m00 = 0; base.m01 = 1;
	base.m10 = 1; base.m11 = 1;

	// (base^n).m11 == (N+1)-th fibonacci number
	return power_logN(base, n - 1).m11;
}

int main()
{
	while (true)
	{
		std::cout << "Enter number (negative number to exit): ";
		
		int n;
		std::cin >> n;

		if (n < 0) break;

		std::cout << n << "-th fibonacci number: " << fibonacci(static_cast<unsigned int>(n)) << std::endl;
  	}
	
	return 0;
}

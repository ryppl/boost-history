/* Boost test_isprime.inl header file
 *
 * Copyright 2007 Arseny Kapoulkine
 *
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or
 * copy at http://www.boost.org/LICENSE_1_0.txt)
 */

template <typename number> inline bool isprime(const number& n)
{
	if (n <= 1) return false;

	number sqrtn = sqrt(n);

	for (number i = 2; i <= sqrtn; ++i)
	{
		if (n % i == 0) return false;
	}

	return true;
}

template <typename number> struct test_isprime_small
{
	number a;

	test_isprime_small()
	{
		a = 997;
	}

	void run()
	{
		for (int i = 0; i < 10000; ++i)
		{
			number f = isprime(a);
		}
	}
};

template <typename number> struct test_isprime_medium
{
	number a;

	test_isprime_medium()
	{
		a = number("2013265921");
	}

	void run()
	{
		for (int i = 0; i < 5; ++i)
		{
			isprime(a);
		}
	}
};

/* Boost test_gcd.inl header file
 *
 * Copyright 2007 Arseny Kapoulkine
 *
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or
 * copy at http://www.boost.org/LICENSE_1_0.txt)
 */

template <typename number> inline number gcd(const number& lhs, const number& rhs)
{
	number a = lhs, b = rhs;

	unsigned int k = 0;

	while (a % 2 == 0 && b % 2 == 0)
	{
		++k;
		a >>= 1;
		b >>= 1;
	}

	while (a % 2 == 0)
	{
		a >>= 1;
	}

	while (b % 2 == 0)
	{
		b >>= 1;
	}

	while (a != 0 && b != 0)
	{
		if (a > b) a -= b;
		else b -= a;
	}

	return (a != 0 ? a : b) << k;
}

template <typename number> struct test_gcd_small
{
	number a, b;

	test_gcd_small()
	{
		a = 364;
		b = 389;
	}

	void run()
	{
		number c;

		for (int i = 0; i < 30000; ++i)
		{
			c = gcd(a, b);
		}
	}
};

template <typename number> struct test_gcd_medium
{
	number a, b;

	test_gcd_medium()
	{
		a = number("2384972234238747238423428374209384728934384723847823849722342387472384234283742093847289343847238478");
		b = number("78836475634756123847238427384723948723847238472377883647563475612384723842738472394872384723847237");
	}

	void run()
	{
		number c;

		for (int i = 0; i < 150; ++i)
		{
			c = gcd(a, b);
		}
	}
};

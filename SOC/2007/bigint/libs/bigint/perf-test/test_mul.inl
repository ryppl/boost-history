/* Boost test_mul.inl header file
 *
 * Copyright 2007 Arseny Kapoulkine
 *
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or
 * copy at http://www.boost.org/LICENSE_1_0.txt)
 */

template <typename number> struct test_mul_small
{
	number a, b;

	test_mul_small()
	{
		a = 23943;
		b = 39048;
	}

	void run()
	{
		number c;

		for (int i = 0; i < 500000; ++i)
		{
			c = a * b;
		}
	}
};

template <typename number> struct test_mul_medium
{
	number a, b;

	test_mul_medium()
	{
		a = pow(number("2394823409283409273487324"), 100);
		b = pow(number("9203402349384209348209234"), 100);
	}

	void run()
	{
		number c;

		for (int i = 0; i < 500; ++i)
		{
			c = a * b;
		}
	}
};

template <typename number> struct test_mul_large
{
	number a, b;

	test_mul_large()
	{
		a = pow(number("2394823409283409273487324"), 50000);
		b = pow(number("9203402349384209348209234"), 50000);
	}

	void run()
	{
		number c;

		for (int i = 0; i < 2; ++i)
		{
			c = a * b;
		}
	}
};

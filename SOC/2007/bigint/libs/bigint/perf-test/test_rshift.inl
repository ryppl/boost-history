/* Boost test_rshift.inl header file
 *
 * Copyright 2007 Arseny Kapoulkine
 *
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or
 * copy at http://www.boost.org/LICENSE_1_0.txt)
 */

template <typename number> struct test_rshift_small
{
	number a;

	test_rshift_small()
	{
		a = 3239443;
	}

	void run()
	{
		number b;

		for (int i = 0; i < 500000; ++i)
		{
			b = a >> 7;
		}
	}
};

template <typename number> struct test_rshift_large
{
	number a;

	test_rshift_large()
	{
		a = pow(number("2394823409283409273487324"), 100);
	}

	void run()
	{
		number b;

		for (int i = 0; i < 20000; ++i)
		{
			b = a >> 3893;
		}
	}
};

template <typename number> struct test_rshift_exact
{
	number a;

	test_rshift_exact()
	{
		a = pow(number("2394823409283409273487324"), 100);
	}

	void run()
	{
		number b;

		for (int i = 0; i < 200000; ++i)
		{
			b = a >> 3904;
		}
	}
};

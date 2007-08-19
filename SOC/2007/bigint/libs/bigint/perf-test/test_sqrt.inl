/* Boost test_sqrt.inl header file
 *
 * Copyright 2007 Arseny Kapoulkine
 *
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or
 * copy at http://www.boost.org/LICENSE_1_0.txt)
 */

template <typename number> struct test_sqrt_small
{
	number a;

	test_sqrt_small()
	{
		a = 23943;
	}

	void run()
	{
		number b;

		for (int i = 0; i < 50000; ++i)
		{
			b = sqrt(a);
		}
	}
};

template <typename number> struct test_sqrt_medium
{
	number a;

	test_sqrt_medium()
	{
		a = pow(number("2394823409283409273487324"), 100) * number("9203402349384209348209234") + 34982349;
	}

	void run()
	{
		number b;

		for (int i = 0; i < 15; ++i)
		{
			b = sqrt(a);
		}
	}
};

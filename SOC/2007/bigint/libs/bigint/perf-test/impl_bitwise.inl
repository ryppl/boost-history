/* Boost impl_bitwise.inl header file
 *
 * Copyright 2007 Arseny Kapoulkine
 *
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or
 * copy at http://www.boost.org/LICENSE_1_0.txt)
 */

template <typename number> struct TEST_BITWISE_SMALL
{
	number a, b;

	TEST_BITWISE_SMALL()
	{
		a = 23943;
		b = 39048;
	}

	void run()
	{
		number c;

		for (int i = 0; i < 1500000; ++i)
		{
			c = a BITWISE_OP b;
		}
	}
};

template <typename number> struct TEST_BITWISE_LARGE
{
	number a, b;

	TEST_BITWISE_LARGE()
	{
		a = pow(number("2394823409283409273487324"), 50000);
		b = pow(number("9203402349384209348209234"), 50000);
	}

	void run()
	{
		number c;

		for (int i = 0; i < 300; ++i)
		{
			c = a BITWISE_OP b;
		}
	}
};

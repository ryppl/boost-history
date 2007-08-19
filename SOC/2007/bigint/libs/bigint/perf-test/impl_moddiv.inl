/* Boost impl_moddiv.inl header file
 *
 * Copyright 2007 Arseny Kapoulkine
 *
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or
 * copy at http://www.boost.org/LICENSE_1_0.txt)
 */

template <typename number> struct TEST_MODDIV_SMALL
{
	number a, b;

	TEST_MODDIV_SMALL()
	{
		a = 3239443;
		b = 39048;
	}

	void run()
	{
		number c;

		for (int i = 0; i < 500000; ++i)
		{
			c = a MODDIV_OP b;
		}
	}
};

template <typename number> struct TEST_MODDIV_MEDIUM
{
	number a, b;

	TEST_MODDIV_MEDIUM()
	{
		a = pow(number("2394823409283409273487324"), 100);
		b = pow(number("9203402349384209348209234"), 50);
	}

	void run()
	{
		number c;

		for (int i = 0; i < 500; ++i)
		{
			c = a MODDIV_OP b;
		}
	}
};

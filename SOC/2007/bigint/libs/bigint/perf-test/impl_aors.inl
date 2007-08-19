/* Boost impl_aors.inl header file
 *
 * Copyright 2007 Arseny Kapoulkine
 *
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or
 * copy at http://www.boost.org/LICENSE_1_0.txt)
 */

template <typename number> struct TEST_AORS_SMALL
{
	number a, b;

	TEST_AORS_SMALL()
	{
		a = 23943;
		b = 39048;
	}

	void run()
	{
		for (int i = 0; i < 10000000; ++i)
		{
			a AORS_OP b;
		}
	}
};

template <typename number> struct TEST_AORS_LARGE
{
	number a, b;

	TEST_AORS_LARGE()
	{
		a = pow(number("2394823409283409273487324"), 100);
		b = pow(number("9203402349384209348209234"), 100);
	}

	void run()
	{
		for (int i = 0; i < 500000; ++i)
		{
			a AORS_OP b;
		}
	}
};

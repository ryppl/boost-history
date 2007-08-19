/* Boost test_to_string.inl header file
 *
 * Copyright 2007 Arseny Kapoulkine
 *
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or
 * copy at http://www.boost.org/LICENSE_1_0.txt)
 */

template <typename number> struct test_to_string_small
{
	number a;

	test_to_string_small()
	{
		a = 23943;
	}

	void run()
	{
		for (int i = 0; i < 1000000; ++i)
		{
			a.str();
		}
	}
};

template <typename number> struct test_to_string_medium
{
	number a;

	test_to_string_medium()
	{
		a = pow(number("2394823409283409273487324"), 100);
	}

	void run()
	{
		for (int i = 0; i < 100; ++i)
		{
			a.str();
		}
	}
};

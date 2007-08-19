/* Boost test_fib.inl header file
 *
 * Copyright 2007 Arseny Kapoulkine
 *
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or
 * copy at http://www.boost.org/LICENSE_1_0.txt)
 */

template <typename number> inline const number fibonacci(unsigned int n)
{
	if (n == 0) return 0;
	else if (n < 3) return 1;
 
	number cur(1), prev(0), fn1_2, fn_2;
  
	unsigned int index = 1;
	unsigned int pot = 1;
  
	while (pot < n && static_cast<unsigned int>(pot << 1) != 0)
		pot <<= 1;
	
	if (pot > n)
		pot >>= 1;

	pot >>= 1; // skipping first bit: it's '1' and we've already computed fib(1)

	while (pot > 0)
	{
		// (fib(n-1),fib(n)) -> (fib(2n-1),fib(2n))
		// (fib(n-1),fib(n)) -> (fib(2n),fib(2n+1))
     
		// fib(2n-1)=fib(n-1)^2+fib(n)^2
		// fib(2n+1)=4*fib(n)^2-fib(n-1)^2+2*(-1)^n
		// fib(2n)=fib(2n+1)-fib(2n-1)

		fn_2 = cur * cur;
		fn1_2 = prev * prev;

		prev = fn_2 + fn1_2;
		cur = fn_2 << 2;

		cur -= fn1_2;
		
		if (index & 1) cur -= 2;
		else cur += 2;
     
		index *= 2;

		if ((n & pot) != 0)
		{
			n = n & (~pot);

			prev = cur - prev;

			++index;
		}
		else cur -= prev;

		pot >>= 1;
	}

	return cur;
}

template <typename number> struct test_fib_small
{
	void run()
	{
		for (int i = 0; i < 5000; ++i)
		{
			number f = fibonacci<number>(1000);
		}
	}
};

template <typename number> struct test_fib_large
{
	void run()
	{
		number f = fibonacci<number>(5000000);
	}
};

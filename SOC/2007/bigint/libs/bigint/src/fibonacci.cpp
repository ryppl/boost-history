#include <iostream>
#include <ctime>

#include <boost/bigint/bigint.hpp>

#include <boost/bigint/bigint_gmp.hpp>
#include <boost/bigint/bigint_default.hpp>
#include <boost/bigint/bigint_storage_vector.hpp>
#include <boost/bigint/bigint_storage_fixed.hpp>

typedef boost::bigint_base<boost::detail::bigint_default_implementation<boost::detail::bigint_storage_vector, 32> > number;
// typedef boost::bigint number;

inline const number fibonacci(unsigned n)
{
	if (n == 0) return 0;
	else if (n < 3) return 1;
 
	number cur(1), prev(0), fn1_2, fn_2;
  
	unsigned int index = 1;
	unsigned int pot = 1;
  
	while (pot< n  && static_cast<unsigned int>(pot << 1) != 0)
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

int main()
{
	const unsigned int N = 10000000;
 
	clock_t start_c = clock();
	number f = fibonacci(N);
	clock_t end_c=clock();

	std::cout << "Computed for " << float(end_c-start_c)/CLOCKS_PER_SEC << " seconds (it's " << float(end_c-start_c)/CLOCKS_PER_SEC/60 << " minutes)" << std::endl;

	clock_t start_s = clock();
	std::string fib = f.str();
	clock_t end_s = clock();
  
	std::cout << "I've computed 10^7-th Fibonacci number for " << float(end_c-start_c)/CLOCKS_PER_SEC << " seconds (it's " << float(end_c-start_c)/CLOCKS_PER_SEC/60 << " minutes)" << std::endl;
	std::cout << "String conversion took " << float(end_s-start_s)/CLOCKS_PER_SEC << " seconds (it's " << float(end_s-start_s)/CLOCKS_PER_SEC/60 << " minutes)" << std::endl;
	std::cout << "The number has " << static_cast<unsigned int>(fib.size()) << " digits" << std::endl;
}

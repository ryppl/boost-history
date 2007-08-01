#include <iostream>
#include <fstream>
#include <algorithm>

int main()
{
	const size_t size = 100000000;

	bool* sieve = new bool[size];

	std::fill(sieve, sieve + size, true);

	sieve[0] = false;
	sieve[1] = false;

	for (size_t i = 2; i < size; ++i)
	{
		if (i % (size / 10000) == 0) {
			std::cout << (float)(i / (size / 10000)) / 100 << " %" << std::endl;
		}

		if (sieve[i])
		{
			for (size_t k = i + i; k < size; k += i)
			{
				sieve[k] = false;
			}
		}	
	}

	std::ofstream out("primes.txt");

	for (size_t i = 0; i < size; ++i)
	{
		if (sieve[i] && i % 1048576 == 1)
		{
			out << i << " ";
		}
	}

	delete[] sieve;
}

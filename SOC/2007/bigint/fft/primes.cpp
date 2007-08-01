#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>

int main()
{
	const unsigned int size = 4294967294;

	std::vector<unsigned int> primes;

	for (unsigned int i = 2; i < size; ++i)
	{
		bool prime = true;

		for (size_t j = 0; j < primes.size(); ++j)
		{
			if ((unsigned __int64)primes[j] * primes[j] > i) break;

			if (i % primes[j] == 0)
			{
				prime = false;
				break;
			}
		}

		if (prime) primes.push_back(i);
	}

	std::cout << primes.size() << std::endl;

	std::ofstream out("primes.txt");

	for (size_t j = 0; j < primes.size(); ++j)
	{
		out << primes[j] << std::endl;
	}
}

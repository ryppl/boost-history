#include <stdio.h>
#include <vector>
#include <iostream>

int main()
{
	FILE* f = fopen("primes.txt", "r");

	unsigned int p = 0;

	std::vector<unsigned int> powers[12];

	while (fscanf(f, "%u", &p) >= 0)
	{
		p -= 1;

		unsigned int power = 0;

		while ((p & 1) == 0)
		{
			++power;
			p /= 2;
		}

		if (power >= 20)
		{
			powers[power - 20].push_back(p);
		}
	}

	fclose(f);

	std::cout << "Yo!" << std::endl;

	for (unsigned int i = 0; i < 12; ++i)
	{
		std::cout << i << ":";

		for (size_t j = 0; j < powers[i].size(); ++j)
		{
			std::cout << " " << powers[i][j];
		}

		std::cout << std::endl;
	}
}

#include <iostream>
#include <algorithm>
#include <vector>
#include <stdio.h>

unsigned int modmul(unsigned int a, unsigned int b, unsigned int prime)
{
	return static_cast<unsigned int>(static_cast<unsigned __int64>(a) * b % prime);
}

unsigned int pow(unsigned int base, unsigned int power, unsigned int prime)
{
	unsigned int pot = 1;

	// Find largest power of two that is >= rhs
	while (pot < power && (pot << 1) != 0)
		pot <<= 1;

	unsigned int res = 1;

	while (pot > 0)
	{
		res = modmul(res, res, prime);
				
		if ((power & pot) != 0)
		{
			res = modmul(res, base, prime);
		}
  
		pot >>= 1;
	}

	return res;
}

int main(int argc, char** argv)
{
	const unsigned int base = 65536;

	if (argc < 3)
	{
		std::cout << "#error You should pass 2 prime numbers here" << std::endl;
		return 0;
	}

	unsigned int primes[2] = {_atoi64(argv[1]), _atoi64(argv[2])};

	if (primes[0] > 2147483647 || primes[1] > 2147483647)
	{
		std::cout << "#error FFT code relies on all numbers to fit in 31 bits, your primes are too big" << std::endl;
		return 0;
	}

	if (primes[0] > primes[1]) std::swap(primes[0], primes[1]);

	std::cout << "static const unsigned int fft_primes[] = {" << primes[0] << ", " << primes[1] << "};\n\n";

	unsigned int powers[2];

	for (int i = 0; i < 2; ++i)
	{
		// find power
		unsigned int t = primes[i] - 1;

		unsigned int power = 0;
	
		while (t % 2 == 0)
		{
			++power;
			t /= 2;
		}

		std::cout << "// " << primes[i] << " = " << t << " * 2^" << power << " + 1\n";

		powers[i] = power;
	}

	// convolution's element is N * base^2 at max, where N is size of each vector (provided we do N * N -> 2N multiplication)
	unsigned __int64 modulus = static_cast<unsigned __int64>(primes[0]) * primes[1];

	modulus /= base;
	modulus /= base;

	// now modulus is upper bound for N; round it down to 2^k

	unsigned int maxN = 1;
	while (maxN * 2 <= modulus) maxN *= 2;

	std::cout << "// fft_max_N * base^2 should be less than " << primes[0] << " * " << primes[1] << " = " << static_cast<unsigned __int64>(primes[0]) * primes[1];
	std::cout << " (base = " << base << ")\n";

	unsigned int power_base = std::min(powers[0], powers[1]);
	unsigned int power = 1 << power_base;

	std::cout << "// fft_max_N should be greater or equal than 2^" << power_base << " = " << power << " due to primes structure" << std::endl;

	if (power > maxN)
	{
		while (power > maxN)
		{
			--power_base;
			power /= 2;
		}
	}
	else
	{
		maxN = power;
	}

	std::cout << "const unsigned int fft_max_N = " << maxN << ";\n\n";

	unsigned int roots[2];

	// find primitive roots (for power)

	for (int i = 0; i < 2; ++i)
	{
		unsigned int prime = primes[i];
		
		for (unsigned int j = 1; j < prime; ++j)
		{
			// root?
			if (pow(j, power, prime) == 1)
			{
				bool primitive = false;

				__int64 r = 1;
				
				for (unsigned int k = 0; k < power; ++k)
				{
					r *= j;
					r %= prime;
			
					if (r == 1)
					{
						primitive = (k == power - 1);
						break;
					}
				}

				if (primitive)
				{
					roots[i] = j;
					break;
				}
			}
		}
	}

	// now generate full suite
	std::vector<unsigned int> all_roots[2];

	for (int i = 0; i < 2; ++i)
		all_roots[i].resize(power_base + 1);

	for (int inv = 0; inv < 2; ++inv)
	{
		for (int i = 0; i < 2; ++i)
		{
			all_roots[i][power_base] = inv ? pow(roots[i], primes[i] - 2, primes[i]) : roots[i];
	
			for (int j = power_base; j > 0; --j)
			{
				all_roots[i][j-1] = modmul(all_roots[i][j], all_roots[i][j], primes[i]);
			}
		}

		std::cout << "static const unsigned int fft_" << (inv ? "inv_" : "") << "primitive_roots[2][" << power_base + 1 << "] =\n{\n";

		for (int i = 0; i < 2; ++i)
		{
			std::cout << "\t{";

			for (int j = 0; j <= power_base; ++j)
				std::cout << all_roots[i][j] << (j == power_base ? "" : ", ");
	
			std::cout << "}" << ("," + (i == 1)) << "\n";
		}

		std::cout << "};\n\n";
	}
	
	std::cout << "static const unsigned int fft_inv_N[2][" << power_base + 1 << "] =\n{\n";
	for (int i = 0; i < 2; ++i)
	{
		std::cout << "\t{";

		for (int j = 0; j <= power_base; ++j)
			std::cout << pow(1 << (unsigned int)j, primes[i] - 2, primes[i]) << (j == power_base ? "" : ", ");
	
		std::cout << "}" << ("," + (i == 1)) << "\n";
	}
	std::cout << "};\n\n";
	
	unsigned int inv_p0_mod_p1 =  pow(primes[0], primes[1] - 2, primes[1]);

	std::cout << "const unsigned int fft_inv_p0_mod_p1 = " << inv_p0_mod_p1 << ";\n";
}

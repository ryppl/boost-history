#include <iostream>

const int base = 10;
typedef short limb_t;

struct print
{
	const limb_t* _data;
	size_t _size;

	print(const limb_t* data, size_t size): _data(data), _size(size)
	{
	}

	std::ostream& operator()(std::ostream& os) const
	{
		for (const limb_t* i = _data; i != _data + _size; ++i)
			os << *i;
	
		return os;
	}
};

int pow(int base, int power, int prime)
{
	int res = 1;

	for (int i = 0; i < power; ++i)
	{
		res = (res * base) % prime;
	}

	return res;
}

void fft(unsigned int* dest, const limb_t* source, size_t N, int prime, int root)
{
	for (int j = 0; j < N; ++j)
	{
		unsigned int r = 0;

		for (int k = 0; k < N; ++k)
		{
			r = (r + ((source[k] * pow(root, j*k, prime)) % prime)) % prime;
		}

		dest[j] = r;
	}
}

void ift(unsigned int* dest, const unsigned int* source, size_t N, int prime, int root)
{
	int rroot = pow(root, prime - 2, prime);

	for (int j = 0; j < N; ++j)
	{
		unsigned int r = 0;

		for (int k = 0; k < N; ++k)
		{
			r = (r + ((source[k] * pow(rroot, j*k, prime)) % prime)) % prime;
		}

		r = (r * pow(N, prime - 2, prime)) % prime;

		dest[j] = r;
	}
}

void mul_fft(limb_t* dest, const limb_t* a, const limb_t* b)
{
	// p must be 1. prime, 2. of the form k*N+1, where N is higherPOT(length(a) + length(b)), and >= N*base^2
	const size_t N = 16;
	
	// N * base^2 == 1600
	// prime0 * prime1 must be > 1600
	const int primes[] = {17, 97};

	// primitive root (power N)
	const int roots[] = {3, 8};

	unsigned int convs[2][N];

	for (int p = 0; p < 2; ++p)
	{
		unsigned int fft_a[N], fft_b[N], fft_c[N];
		fft(fft_a, a, N, primes[p], roots[p]);
		fft(fft_b, b, N, primes[p], roots[p]);

		for (size_t i = 0; i < N; ++i)
		{
			fft_c[i] = (fft_a[i] * fft_b[i]) % primes[p];
		}

		ift(convs[p], fft_c, N, primes[p], roots[p]);
	}

	unsigned int conv[N];

	// CRT:
	// x % p1 = r1
	// x % p2 = r2

	// x % p1p2 = r1 * (P/p1)*T1 + r2 * (P/p2)*T2
	// T1 = (P/p1) ^ (p1 - 2)
	// T2 = (P/p2) ^ (p2 - 2)

	int prime = primes[0] * primes[1];
	int T[2] = {pow(primes[1], primes[0] - 2, primes[0]), pow(primes[0], primes[1] - 2, primes[1])};

	for (int i = 0; i < N; ++i)
	{
		conv[i] = (convs[0][i] * T[0] * primes[1] + convs[1][i] * T[1] * primes[0]) % prime;
	}

	std::cout << std::endl;
	for (int i = 0; i < N; ++i)
	{
		std::cout << convs[0][i] << " ";
	}
	std::cout << std::endl;
	for (int i = 0; i < N; ++i)
	{
		std::cout << convs[1][i] << " ";
	}
	std::cout << std::endl;
	for (int i = 0; i < N; ++i)
	{
		std::cout << conv[i] << " ";
	}
	std::cout << std::endl;

	unsigned int carry = 0;

	for (int i = 0; i < N; ++i)
	{
		carry += conv[i];
		dest[i] = carry % 10;
		carry /= 10;
	}
}

int main()
{
	limb_t a[16] = {2, 3, 4, 5, 6, 7, 8, 9};
	limb_t b[16] = {1, 2, 3, 4, 5, 6, 7, 8};

	limb_t c[16] = {};

	mul_fft(c, a, b);

	std::cout << "A = "; print(a, 16)(std::cout) << std::endl;
	std::cout << "B = "; print(b, 16)(std::cout) << std::endl;
	std::cout << "C = "; print(c, 16)(std::cout) << std::endl;
}

#include <iostream>

#include <windows.h>

#define __int64 long long

const unsigned int base = 65536;
typedef unsigned short limb_t;

template <typename T> void P(const char* text, const T* data, size_t size)
{
	std::cout << text;
	for (size_t i = 0; i < size; ++i) std::cout << " " << std::dec << data[i] << std::dec;
	std::cout << std::endl;
}

unsigned int modmul(unsigned int a, unsigned int b, unsigned int prime)
{
	return static_cast<unsigned int>(static_cast<unsigned __int64>(a) * b % prime);
}

unsigned int modadd(unsigned int a, unsigned int b, unsigned int prime)
{
	return (a + b) % prime;
}

unsigned int modsub(unsigned int a, unsigned int b, unsigned int prime)
{
	return (a + prime - b) % prime;
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

// reverse(x) -> reverse(x+1)
size_t rev_next(size_t x, size_t n)
{
	do
	{
		n >>= 1;
		x ^= n;
	}
	while ((x & n) == 0);

	return x;
}

// swaps values with indices that have reversed bit representation (data[1100b] <-> data[0011b])
template <typename T> void fft_copy_reorder(unsigned int* dest, const T* source, size_t size)
{
	if (size <= 2)
	{
		std::copy(source, source + size, dest);
		return;
	}

	size_t r = 0;

	dest[0] = source[0];

	for (size_t x = 1; x < size; ++x)
	{
		r = rev_next(r, size);

		dest[x] = source[r];
	}
}

void fft_recursive(unsigned int* data, size_t size, unsigned int prime, unsigned int root)
{
	size /= 2;

	if (size > 1)
	{
		unsigned int new_root = modmul(root, root, prime);
		fft_recursive(data, size, prime, new_root);
		fft_recursive(data + size, size, prime, new_root);
	}

	unsigned int r = 1;

	for (size_t i = 0; i < size; ++i)
	{
		unsigned int a = data[i];
		unsigned int b = modmul(data[i + size], r, prime);

		data[i] = modadd(a, b, prime);
		data[i + size] = modsub(a, b, prime);

		r = modmul(r, root, prime);
	}
}

void fft(unsigned int* dest, const limb_t* source, size_t N, unsigned int prime, unsigned int root)
{
	fft_copy_reorder(dest, source, N);
	fft_recursive(dest, N, prime, root);
}

void ift(unsigned int* dest, const unsigned int* source, size_t N, unsigned int prime, unsigned int root)
{
	unsigned int rroot = pow(root, prime - 2, prime);

	fft_copy_reorder(dest, source, N);
	fft_recursive(dest, N, prime, rroot);

	unsigned int N_inv = pow(N, prime - 2, prime);

	for (size_t i = 0; i < N; ++i)
	{
		dest[i] = modmul(dest[i], N_inv, prime);
	}
}

void mul_fft(limb_t* dest, const limb_t* a, const limb_t* b)
{
	// p must be 1. prime, 2. of the form k*N+1, where N is higherPOT(length(a) + length(b)), and >= N*base^2
	const size_t N = 16;
	
	// N * base^2 == 1600
	// prime0 * prime1 must be > 1600
	const unsigned int primes[] = {70254593, 81788929};

	// primitive root (power N)
	const unsigned int roots[] = {3183438, 22821826};

	unsigned int convs[2][N];

	for (int p = 0; p < 2; ++p)
	{
		unsigned int fft_a[N], fft_b[N], fft_c[N];
		fft(fft_a, a, N, primes[p], roots[p]);
		fft(fft_b, b, N, primes[p], roots[p]);

		for (size_t i = 0; i < N; ++i)
		{
			fft_c[i] = modmul(fft_a[i], fft_b[i], primes[p]);
		}

		ift(convs[p], fft_c, N, primes[p], roots[p]);
	}

	unsigned __int64 prime = static_cast<unsigned __int64>(primes[0]) * primes[1];
	unsigned int inv_p0_mod_p1 =  pow(primes[0], primes[1] - 2, primes[1]);

	unsigned __int64 carry = 0;

	for (int i = 0; i < N; ++i)
	{	
		// 64 x == 32 c0 mod 32 p0
		// 64 x == 32 c1 mod 32 p1

		// x == p0*t + c0
		// p0*t + c0 == c1  mod p1
		// t == (c1 - c0) div p0  mod p1

		// t == t0 mod p1

		// x == p0 * t0 + c0
		// t0 == (c1 - c0) div p0  mod p1

		unsigned int t0 = modmul(modsub(convs[1][i], convs[0][i], primes[1]), inv_p0_mod_p1, primes[1]);
		unsigned __int64 x = static_cast<unsigned __int64>(primes[0]) * t0 + convs[0][i];

		carry += x;

		dest[i] = carry & 0xffff;
		carry >>= 16;
	}
}

void mul_basecase(limb_t* dest, const limb_t* a, const limb_t* b)
{
	typedef unsigned int limb2_t;

	const size_t N = 16;

	memset(dest, 0, sizeof(limb_t) * N);

	limb_t* i = dest;
			
	for (const limb_t* li = a; li != a + N/2; ++li, ++i)
	{
		limb_t carry = 0;

		limb_t* ci = i;

		for (const limb_t* ri = b; ri != b + N/2; ++ri)
		{
			limb2_t result = static_cast<limb2_t>(*li) * *ri + *ci + carry;

			*ci++ = static_cast<limb_t>(result & 0xffff);

			carry = static_cast<limb_t>(result >> 16);
		}

		while (carry != 0)
		{
			limb2_t result = static_cast<limb2_t>(*ci) + carry;
					
			*ci++ = static_cast<limb_t>(result & 0xffff);

			carry = static_cast<limb_t>(result >> 16);
		}
	}
}

int main()
{
//	limb_t a[16] = {65535, 65533, 65531, 65529, 65527, 65525, 65523, 65521};
//	limb_t b[16] = {65535, 65534, 65533, 65532, 65531, 65530, 65529, 65528};
	limb_t a[16] = {65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535};
	limb_t b[16] = {65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535};

	limb_t c[16] = {};

	P("A = ", a, 8);
	P("B = ", b, 8);
	
	int count = 10000;
	LARGE_INTEGER start, end, freq;

	QueryPerformanceFrequency(&freq);

	QueryPerformanceCounter(&start);
	for (int i = 0; i < count; ++i)
		mul_fft(c, a, b);
	QueryPerformanceCounter(&end);
	
	P("Fast FT : C = ", c, 16); std::cout << (float)(end.QuadPart - start.QuadPart) / freq.QuadPart << std::endl;
	
	QueryPerformanceCounter(&start);
	for (int i = 0; i < count; ++i)
		mul_basecase(c, a, b);
	QueryPerformanceCounter(&end);
	
	P("Basecase: C = ", c, 16); std::cout << (float)(end.QuadPart - start.QuadPart) / freq.QuadPart << std::endl;
}

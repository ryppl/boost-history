#include <iostream>

#include <windows.h>

#define __int64 long long

#include "primes.h"

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

unsigned int log2(unsigned int value)
{
	unsigned int r = 0;

	while (value > 1)
	{
		++r;
		value >>= 1;
	}

	return r;
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

void fft_recursive(unsigned int* data, size_t size, unsigned int prime, const unsigned int* roots)
{
	size /= 2;

	if (size > 1)
	{
		fft_recursive(data, size, prime, roots - 1);
		fft_recursive(data + size, size, prime, roots - 1);
	}

	unsigned int root = *roots;
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

void fft_iterative(unsigned int* data, size_t size, unsigned int prime, const unsigned int* roots)
{
	size_t step = 1;

	unsigned int nstep = 0;

	while (step < size)
	{
		unsigned int root = roots[++nstep];

		size_t half_step = step;
		step *= 2;

		unsigned int r = 1;

		for (size_t j = 0; j < half_step; ++j)
		{
			for (size_t i = j; i < size; i += step)
			{
				unsigned int a = data[i];
				unsigned int b = modmul(data[i + half_step], r, prime);

				data[i] = modadd(a, b, prime);
				data[i + half_step] = modsub(a, b, prime);
			}
			
			r = modmul(r, root, prime);
		}
	}
}

void dft(unsigned int* dest, const limb_t* source, size_t N, unsigned int log2N, unsigned int prime, const unsigned int* root_table)
{
	fft_copy_reorder(dest, source, N);
	fft_recursive(dest, N, prime, root_table + log2N);
}

void dft_i(unsigned int* dest, const limb_t* source, size_t N, unsigned int log2N, unsigned int prime, const unsigned int* root_table)
{
	fft_copy_reorder(dest, source, N);
	fft_iterative(dest, N, prime, root_table);
}

void ift(unsigned int* dest, const unsigned int* source, size_t N, unsigned int log2N, unsigned int prime, const unsigned int* root_table, unsigned int inv_N)
{
	fft_copy_reorder(dest, source, N);
	fft_recursive(dest, N, prime, root_table + log2N);

	for (size_t i = 0; i < N; ++i)
	{
		dest[i] = modmul(dest[i], inv_N, prime);
	}
}

void ift_i(unsigned int* dest, const unsigned int* source, size_t N, unsigned int log2N, unsigned int prime, const unsigned int* root_table, unsigned int inv_N)
{
	fft_copy_reorder(dest, source, N);
	fft_iterative(dest, N, prime, root_table);

	for (size_t i = 0; i < N; ++i)
	{
		dest[i] = modmul(dest[i], inv_N, prime);
	}
}

void mul_fft(limb_t* dest, const limb_t* a, const limb_t* b,
	void (*dft)(unsigned int* dest, const limb_t* source, size_t N, unsigned int log2N, unsigned int prime, const unsigned int* root_table),
	void (*ift)(unsigned int* dest, const unsigned int* source, size_t N, unsigned int log2N, unsigned int prime, const unsigned int* root_table, unsigned int inv_N)
	)
{
	// p must be 1. prime, 2. of the form k*N+1, where N is higherPOT(length(a) + length(b)), and >= N*base^2
	const size_t N = 16;

	unsigned int log2N = log2(N);
	
	unsigned int convs[2][N];

	for (int p = 0; p < 2; ++p)
	{
		const unsigned int* root_table = fft_primitive_roots[p];
		unsigned int prime = fft_primes[p];

		unsigned int fft_a[N], fft_b[N], fft_c[N];
		dft(fft_a, a, N, log2N, prime, root_table);
		dft(fft_b, b, N, log2N, prime, root_table);

		for (size_t i = 0; i < N; ++i)
		{
			fft_c[i] = modmul(fft_a[i], fft_b[i], prime);
		}
		
		const unsigned int* inv_root_table = fft_inv_primitive_roots[p];
		unsigned int inv_N = fft_inv_N[p][log2N];

		ift(convs[p], fft_c, N, log2N, prime, inv_root_table, inv_N);
	}

	unsigned __int64 prime = static_cast<unsigned __int64>(fft_primes[0]) * fft_primes[1];
	unsigned int inv_p0_mod_p1 = fft_inv_p0_mod_p1;

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

		unsigned int t0 = modmul(modsub(convs[1][i], convs[0][i], fft_primes[1]), inv_p0_mod_p1, fft_primes[1]);
		unsigned __int64 x = static_cast<unsigned __int64>(fft_primes[0]) * t0 + convs[0][i];

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
	
	int count = 1000000;
	LARGE_INTEGER start, end, freq;

	QueryPerformanceFrequency(&freq);

	QueryPerformanceCounter(&start);
	for (int i = 0; i < count; ++i)
		mul_fft(c, a, b, dft, ift);
	QueryPerformanceCounter(&end);
	
	P("Recursive: C = ", c, 16); std::cout << (float)(end.QuadPart - start.QuadPart) / freq.QuadPart << std::endl;
	
	QueryPerformanceCounter(&start);
	for (int i = 0; i < count; ++i)
		mul_fft(c, a, b, dft_i, ift_i);
	QueryPerformanceCounter(&end);
	
	P("Iterative: C = ", c, 16); std::cout << (float)(end.QuadPart - start.QuadPart) / freq.QuadPart << std::endl;

	QueryPerformanceCounter(&start);
	for (int i = 0; i < count; ++i)
		mul_basecase(c, a, b);
	QueryPerformanceCounter(&end);
	
	P("Basecase : C = ", c, 16); std::cout << (float)(end.QuadPart - start.QuadPart) / freq.QuadPart << std::endl;
}

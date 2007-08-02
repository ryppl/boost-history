#include <iostream>
#include <fstream>
#include <windows.h>

#include "primes.h"

template <typename T> void P(const char* text, const T* data, size_t size)
{
	std::cout << text;
	for (size_t i = 0; i < size; ++i) std::cout << " " << std::dec << data[i] << std::dec;
	std::cout << std::endl;
}

template <typename T> void S(const char* file, const T* data, size_t size)
{
	std::ofstream out(file);
	for (size_t i = 0; i < size; ++i) out << std::hex << data[i] << std::dec;
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

void dft(unsigned int* dest, const unsigned short* source, size_t N, unsigned int log2N, unsigned int prime, const unsigned int* root_table)
{
	fft_copy_reorder(dest, source, N);
	fft_recursive(dest, N, prime, root_table + log2N);
}

void dft_i(unsigned int* dest, const unsigned short* source, size_t N, unsigned int log2N, unsigned int prime, const unsigned int* root_table)
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

void fft_crt_carry(unsigned short* dest, const unsigned int* conv0, const unsigned int* conv1, size_t N)
{
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

		unsigned int t0 = modmul(modsub(conv1[i], conv0[i], fft_primes[1]), inv_p0_mod_p1, fft_primes[1]);
		unsigned __int64 x = static_cast<unsigned __int64>(fft_primes[0]) * t0 + conv0[i];

		carry += x;

		dest[i] = carry & 0xffff;
		carry >>= 16;
	}
}

// precondition: dest has N elements allocated, a and b have N elements, last N/2 are 0, N is power of two
// postcondition: dest has product, padded with zeroes
void mul_fft(unsigned short* dest, const unsigned short* a, const unsigned short* b, size_t N,
	void (*dft)(unsigned int* dest, const unsigned short* source, size_t N, unsigned int log2N, unsigned int prime, const unsigned int* root_table),
	void (*ift)(unsigned int* dest, const unsigned int* source, size_t N, unsigned int log2N, unsigned int prime, const unsigned int* root_table, unsigned int inv_N)
	)
{
	unsigned int log2N = log2(N);
	
	unsigned int* workspace = new unsigned int[4*N];
	
	unsigned int* convs[] = {workspace, workspace + N};
	unsigned int* fft_a = workspace + 2*N;
	unsigned int* fft_b = workspace + 3*N;

	for (int p = 0; p < 2; ++p)
	{
		const unsigned int* root_table = fft_primitive_roots[p];
		unsigned int prime = fft_primes[p];

		dft(fft_a, a, N, log2N, prime, root_table);
		dft(fft_b, b, N, log2N, prime, root_table);

		for (size_t i = 0; i < N; ++i)
		{
			fft_a[i] = modmul(fft_a[i], fft_b[i], prime);
		}
		
		const unsigned int* inv_root_table = fft_inv_primitive_roots[p];
		unsigned int inv_N = fft_inv_N[p][log2N];

		ift(convs[p], fft_a, N, log2N, prime, inv_root_table, inv_N);
	}

	fft_crt_carry(dest, convs[0], convs[1], N);

	delete[] workspace;
}

// precondition: dest has N elements allocated, a has N elements, last N/2 are 0, N is power of two
// postcondition: dest has product, padded with zeroes
void sqr_fft(unsigned short* dest, const unsigned short* a, size_t N,
			 void (*dft)(unsigned int* dest, const unsigned short* source, size_t N, unsigned int log2N, unsigned int prime, const unsigned int* root_table),
			 void (*ift)(unsigned int* dest, const unsigned int* source, size_t N, unsigned int log2N, unsigned int prime, const unsigned int* root_table, unsigned int inv_N)
			 )
{
	unsigned int log2N = log2(N);

	unsigned int* workspace = new unsigned int[3*N];
	
	unsigned int* convs[] = {workspace, workspace + N};
	unsigned int* fft = workspace + 2*N;

	for (int p = 0; p < 2; ++p)
	{
		const unsigned int* root_table = fft_primitive_roots[p];
		unsigned int prime = fft_primes[p];

		dft(fft, a, N, log2N, prime, root_table);

		for (size_t i = 0; i < N; ++i)
		{
			fft[i] = modmul(fft[i], fft[i], prime);
		}

		const unsigned int* inv_root_table = fft_inv_primitive_roots[p];
		unsigned int inv_N = fft_inv_N[p][log2N];

		ift(convs[p], fft, N, log2N, prime, inv_root_table, inv_N);
	}

	fft_crt_carry(dest, convs[0], convs[1], N);

	delete[] workspace;
}

void mul_basecase(unsigned short* dest, const unsigned short* a, const unsigned short* b, size_t N)
{
	typedef unsigned int limb2_t;

	memset(dest, 0, sizeof(unsigned short) * N);

	unsigned short* i = dest;
			
	for (const unsigned short* li = a; li != a + N/2; ++li, ++i)
	{
		unsigned short carry = 0;

		unsigned short* ci = i;

		for (const unsigned short* ri = b; ri != b + N/2; ++ri)
		{
			limb2_t result = static_cast<limb2_t>(*li) * *ri + *ci + carry;

			*ci++ = static_cast<unsigned short>(result & 0xffff);

			carry = static_cast<unsigned short>(result >> 16);
		}

		while (carry != 0)
		{
			limb2_t result = static_cast<limb2_t>(*ci) + carry;
					
			*ci++ = static_cast<unsigned short>(result & 0xffff);

			carry = static_cast<unsigned short>(result >> 16);
		}
	}
}

int main()
{
	size_t bits = (1 << 20) * 16;
	size_t N = bits / 16;

	unsigned short* a = new unsigned short[N];
	unsigned short* b = new unsigned short[N];
	unsigned short* c = new unsigned short[N];

	for (size_t i = 0; i < N/2; ++i)
	{
		a[i] = b[i] = 65535;
	}

	for (size_t i = N/2; i < N; ++i)
	{
		a[i] = b[i] = 0;
	}
	
	int count = 1;
	LARGE_INTEGER start, end, freq;

	QueryPerformanceFrequency(&freq);

	QueryPerformanceCounter(&start);
	for (int i = 0; i < count; ++i)
		mul_fft(c, a, b, N, dft, ift);
	QueryPerformanceCounter(&end);

	std::cout << "Recursive: " << (float)(end.QuadPart - start.QuadPart) / freq.QuadPart << std::endl;
	S("res_fft_rec.txt", c, N);

	QueryPerformanceCounter(&start);
	for (int i = 0; i < count; ++i)
		mul_fft(c, a, b, N, dft_i, ift_i);
	QueryPerformanceCounter(&end);

	std::cout << "Iterative: " << (float)(end.QuadPart - start.QuadPart) / freq.QuadPart << std::endl;
	S("res_fft_iter.txt", c, N);

	QueryPerformanceCounter(&start);
	for (int i = 0; i < count; ++i)
		sqr_fft(c, a, N, dft, ift);
	QueryPerformanceCounter(&end);
	
	std::cout << "Recursive SQR: " << (float)(end.QuadPart - start.QuadPart) / freq.QuadPart << std::endl;
	S("res_fft_rec_sqr.txt", c, N);
	
	QueryPerformanceCounter(&start);
	for (int i = 0; i < count; ++i)
		sqr_fft(c, a, N, dft_i, ift_i);
	QueryPerformanceCounter(&end);
	
	std::cout << "Iterative SQR: " << (float)(end.QuadPart - start.QuadPart) / freq.QuadPart << std::endl;
	S("res_fft_iter_sqr.txt", c, N);

	QueryPerformanceCounter(&start);
	for (int i = 0; i < count; ++i)
		mul_basecase(c, a, b, N);
	QueryPerformanceCounter(&end);
	
	std::cout << "Basecase: " << (float)(end.QuadPart - start.QuadPart) / freq.QuadPart << std::endl;
	S("res_basecase.txt", c, N);

	delete[] a;
	delete[] b;
	delete[] c;
}

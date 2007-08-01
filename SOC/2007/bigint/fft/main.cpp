#include <iostream>

#include <windows.h>

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

template <typename T> void P(const char* text, const T* data, size_t size)
{
	std::cout << text << ": ";
	for (size_t i = 0; i < size; ++i) std::cout << data[i] << " ";
	std::cout << std::endl;
}

int pow(int base, int power, int prime)
{
	int pot = power;

	// Find largest power of two that is >= rhs
	while (pot < power && (pot << 1) != 0)
		pot <<= 1;

	int res = 1;

	while (pot > 0)
	{
		res = (res * res) % prime;
				
		if ((power & pot) != 0)
		{
			res = (res * base) % prime;
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
void fft_reorder(unsigned int* data, size_t size)
{
	if (size <= 2) return;

	size_t r = 0;

	for (size_t x = 1; x < size; ++x)
	{
		r = rev_next(r, size);

		if (r > x) std::swap(data[x], data[r]);
	}
}

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

void fft_recursive(unsigned int* data, size_t size, int prime, int root)
{
	size /= 2;

	if (size > 1)
	{
		int new_root = (root * root) % prime;
		fft_recursive(data, size, prime, new_root);
		fft_recursive(data + size, size, prime, new_root);
	}

	unsigned int r = 1;

	for (size_t i = 0; i < size; ++i)
	{
		unsigned int a = data[i];
		unsigned int b = (data[i + size] * r) % prime;

		data[i] = (a + b) % prime;
		data[i + size] = (a + prime - b) % prime;

		r = (r * root) % prime;
	}
}

void fft_iterative(unsigned int* data, size_t size, int prime, int root)
{
	size_t step = 1;

	int roots[4];
	roots[3] = root;
	roots[2] = (roots[3] * roots[3]) % prime;
	roots[1] = (roots[2] * roots[2]) % prime;
	roots[0] = (roots[1] * roots[1]) % prime;

	int nstep = 0;

	while (step < size)
	{
		root = roots[nstep++];

		size_t half_step = step;
		step *= 2;

		unsigned int r = 1;

		for (size_t j = 0; j < half_step; ++j)
		{
			for (size_t i = j; i < size; i += step)
			{
				unsigned int a = data[i];
				unsigned int b = (data[i + half_step] * r) % prime;

				data[i] = (a + b) % prime;
				data[i + half_step] = (a + prime - b) % prime;
			}
			
			r = (r * root) % prime;
		}
	}
}

void fft(unsigned int* dest, const limb_t* source, size_t N, int prime, int root)
{
	fft_copy_reorder(dest, source, N);
	fft_recursive(dest, N, prime, root);
}

void fft_i(unsigned int* dest, const limb_t* source, size_t N, int prime, int root)
{
	fft_copy_reorder(dest, source, N);
	fft_iterative(dest, N, prime, root);
}

void fft_slow(unsigned int* dest, const limb_t* source, size_t N, int prime, int root)
{
	unsigned int p_j = 1;

	for (int j = 0; j < N; ++j)
	{
		unsigned int r = 0;

		unsigned int p_jk = 1;

		for (int k = 0; k < N; ++k)
		{
			r = (r + source[k] * p_jk) % prime;
			p_jk = (p_jk * p_j) % prime;
		}

		dest[j] = r;
		
		p_j = (p_j * root) % prime;
	}
}

void ift(unsigned int* dest, const unsigned int* source, size_t N, int prime, int root)
{
	int rroot = pow(root, prime - 2, prime);

	fft_copy_reorder(dest, source, N);
	fft_recursive(dest, N, prime, rroot);

	unsigned int N_inv = pow(N, prime - 2, prime);

	for (size_t i = 0; i < N; ++i)
	{
		dest[i] = dest[i] * N_inv % prime;
	}
}

void ift_i(unsigned int* dest, const unsigned int* source, size_t N, int prime, int root)
{
	int rroot = pow(root, prime - 2, prime);

	fft_copy_reorder(dest, source, N);
	fft_iterative(dest, N, prime, rroot);

	unsigned int N_inv = pow(N, prime - 2, prime);

	for (size_t i = 0; i < N; ++i)
	{
		dest[i] = dest[i] * N_inv % prime;
	}
}

void ift_slow(unsigned int* dest, const unsigned int* source, size_t N, int prime, int root)
{
	int rroot = pow(root, prime - 2, prime);
	int N_inv = pow(N, prime - 2, prime);

	unsigned int p_j = 1;

	for (int j = 0; j < N; ++j)
	{
		unsigned int p_jk = 1;

		unsigned int r = 0;

		for (int k = 0; k < N; ++k)
		{
			r = (r + source[k] * p_jk) % prime;

			p_jk = (p_jk * p_j) % prime;
		}

		r = (r * N_inv) % prime;

		dest[j] = r;
		
		p_j = (p_j * rroot) % prime;
	}
}

typedef void (*fftfunc)(unsigned int* dest, const limb_t* source, size_t N, int prime, int root);
typedef void (*iftfunc)(unsigned int* dest, const unsigned int* source, size_t N, int prime, int root);

void mul_fft(limb_t* dest, const limb_t* a, const limb_t* b, fftfunc fft, iftfunc ift)
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

	// CRT:
	// x % p1 = r1
	// x % p2 = r2

	// x % p1p2 = r1 * (P/p1)*T1 + r2 * (P/p2)*T2
	// T1 = (P/p1) ^ (p1 - 2)
	// T2 = (P/p2) ^ (p2 - 2)

	int prime = primes[0] * primes[1];
	int T[2] = {pow(primes[1], primes[0] - 2, primes[0]), pow(primes[0], primes[1] - 2, primes[1])};

	unsigned int carry = 0;

	for (int i = 0; i < N; ++i)
	{
		int conv = (convs[0][i] * T[0] * primes[1] + convs[1][i] * T[1] * primes[0]) % prime;
		
		carry += conv;
		dest[i] = carry % 10;
		carry /= 10;
	}
}

void mul_basecase(limb_t* dest, const limb_t* a, const limb_t* b)
{
	typedef int limb2_t;

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

			*ci++ = static_cast<limb_t>(result % 10);

			carry = static_cast<limb_t>(result / 10);
		}

		while (carry != 0)
		{
			limb2_t result = static_cast<limb2_t>(*ci) + carry;
					
			*ci++ = static_cast<limb_t>(result % 10);

			carry = static_cast<limb_t>(result / 10);
		}
	}
}

int main()
{
	limb_t a[16] = {2, 3, 4, 5, 6, 7, 8, 9};
	limb_t b[16] = {1, 2, 3, 4, 5, 6, 7, 8};

	limb_t c[16] = {};

	std::cout << "A = "; print(a, 16)(std::cout) << std::endl;
	std::cout << "B = "; print(b, 16)(std::cout) << std::endl;
	
	int count = 1;
	LARGE_INTEGER start, end, freq;

	QueryPerformanceFrequency(&freq);

	QueryPerformanceCounter(&start);
	for (int i = 0; i < count; ++i)
		mul_fft(c, a, b, fft, ift);
	QueryPerformanceCounter(&end);
	
	std::cout << "Recursive FFT: C = "; print(c, 16)(std::cout) << " " << (float)(end.QuadPart - start.QuadPart) / freq.QuadPart << std::endl;

	QueryPerformanceCounter(&start);
	for (int i = 0; i < count; ++i)
		mul_fft(c, a, b, fft_i, ift_i);
	QueryPerformanceCounter(&end);
	
	std::cout << "Iterative FFT: C = "; print(c, 16)(std::cout) << " " << (float)(end.QuadPart - start.QuadPart) / freq.QuadPart << std::endl;
	
	QueryPerformanceCounter(&start);
	for (int i = 0; i < count; ++i)
		mul_fft(c, a, b, fft_slow, ift_slow);
	QueryPerformanceCounter(&end);
	
	std::cout << "Slow FT: C = "; print(c, 16)(std::cout) << " " << (float)(end.QuadPart - start.QuadPart) / freq.QuadPart << std::endl;
	
	QueryPerformanceCounter(&start);
	for (int i = 0; i < count; ++i)
		mul_basecase(c, a, b);
	QueryPerformanceCounter(&end);
	
	std::cout << "Basecase: C = "; print(c, 16)(std::cout) << " " << (float)(end.QuadPart - start.QuadPart) / freq.QuadPart << std::endl;
}

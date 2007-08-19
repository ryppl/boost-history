#include <iostream>
#include <ctime>

#include <boost/bigint/bigint_default.hpp>
#include <boost/bigint/bigint_storage_vector.hpp>

typedef boost::detail::bigint_default_implementation<boost::detail::bigint_storage_vector, 8> default_8;
typedef boost::detail::bigint_default_implementation<boost::detail::bigint_storage_vector, 16> default_16;
typedef boost::detail::bigint_default_implementation<boost::detail::bigint_storage_vector, 32> default_32;

	static size_t log2(size_t value)
	{
		size_t r = 0;

		while (value > 1)
		{
			++r;
			value >>= 1;
		}

		return r;
	}

template <typename impl> void measure_basecase(const char* impl_name, unsigned int limbs_left, unsigned int limbs_right, unsigned int count)
{
	impl a, b;

	a.data.resize(limbs_left);
	std::fill(a.data.begin(), a.data.end(), impl::limb_max());

	b.data.resize(limbs_left);
	std::fill(b.data.begin(), b.data.end(), impl::limb_max());

	impl c;
	c._mul_unsigned_basecase(a, b);

	clock_t start = clock();
	for (unsigned int i = 0; i < count; ++i)
	{
		c._mul_unsigned_basecase(a, b);
	}
	clock_t end = clock();

	std::cout << impl_name << " basecase: " << limbs_left << "x" << limbs_right << " multiplication: " << count << " times take " << (end-start) << " clocks, "
	<< double(end - start) / count / limbs_left / limbs_right * 100000 << " per limb"
	<< std::endl;
}

template <typename impl> void measure_fft(const char* impl_name, unsigned int limbs_left, unsigned int limbs_right, unsigned int count)
{
	impl a, b;

	a.data.resize(limbs_left);
	std::fill(a.data.begin(), a.data.end(), impl::limb_max());

	b.data.resize(limbs_left);
	std::fill(b.data.begin(), b.data.end(), impl::limb_max());

	impl c;
	c._mul_unsigned_fft(a, b);

	clock_t start = clock();
	for (unsigned int i = 0; i < count; ++i)
	{
		c._mul_unsigned_fft(a, b);
	}
	clock_t end = clock();

	// find FFT (uint16) size
	size_t max_size = (std::max)(limbs_left, limbs_right);

	// round up to the nearest power of two
	size_t N = 1;
	while (N < max_size) N *= 2;

	// fix N depending on limb_type
	N = N * sizeof(impl::limb_t) / sizeof(unsigned short);
	if (N == 0) N = 1;

	// destination size
	N *= 2;

	std::cout << impl_name << " fft: " << limbs_left << "x" << limbs_right << " multiplication: " << count << " times take " << (end-start) << " clocks, "
	<< double(end - start) / count / N / log2(N) * 100000 << " per element"
	<< std::endl;
}

int main()
{
	measure_basecase<default_8>("default_8", 4, 4, 2000000);
	measure_basecase<default_16>("default_16", 4, 4, 2000000);
	measure_basecase<default_32>("default_32", 4, 4, 2000000);
	
	measure_basecase<default_8>("default_8", 16, 16, 200000);
	measure_basecase<default_16>("default_16", 16, 16, 200000);
	measure_basecase<default_32>("default_32", 16, 16, 200000);
	
	measure_basecase<default_8>("default_8", 256, 256, 1000);
	measure_basecase<default_16>("default_16", 256, 256, 1000);
	measure_basecase<default_32>("default_32", 256, 256, 1000);
	
	measure_basecase<default_8>("default_8", 1024, 1024, 64);
	measure_basecase<default_16>("default_16", 1024, 1024, 64);
	measure_basecase<default_32>("default_32", 1024, 1024, 64);

	measure_basecase<default_8>("default_8", 8192, 8192, 1);
	measure_basecase<default_16>("default_16", 8192, 8192, 1);
	measure_basecase<default_32>("default_32", 8192, 8192, 1);

	measure_fft<default_8>("default_8", 4, 4, 100000);
	measure_fft<default_16>("default_16", 4, 4, 100000);
	measure_fft<default_32>("default_32", 4, 4, 100000);
	
	measure_fft<default_8>("default_8", 16, 16, 20000);
	measure_fft<default_16>("default_16", 16, 16, 20000);
	measure_fft<default_32>("default_32", 16, 16, 20000);
	
	measure_fft<default_8>("default_8", 256, 256, 1000);
	measure_fft<default_16>("default_16", 256, 256, 1000);
	measure_fft<default_32>("default_32", 256, 256, 1000);

	measure_fft<default_8>("default_8", 1024, 1024, 64);
	measure_fft<default_16>("default_16", 1024, 1024, 64);
	measure_fft<default_32>("default_32", 1024, 1024, 64);

	measure_fft<default_8>("default_8", 8192, 8192, 10);
	measure_fft<default_16>("default_16", 8192, 8192, 10);
	measure_fft<default_32>("default_32", 8192, 8192, 10);
}

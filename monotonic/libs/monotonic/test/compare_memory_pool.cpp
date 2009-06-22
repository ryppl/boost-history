// (C) 2009 Christian Schladetsch
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

// the documentation is at https://svn.boost.org/svn/boost/sandbox/monotonic/libs/monotonic/doc/index.html

// the sandbox is at https://svn.boost.org/svn/boost/sandbox/monotonic/

#include <iostream> 
#include <iomanip> 
#include <numeric>
#include <algorithm>
#include <math.h>

#include <vector>
#include <list>
#include <map>
#include <set>
#include <bitset>
#include <string>

#include <boost/timer.hpp>
#include <boost/monotonic/local.hpp>

//#include <boost/monotonic/local_allocator.hpp>

#include "./AllocatorTypes.h"

using namespace std;
using namespace boost;

struct test_vector_create
{
	template <class Alloc>
	int test(Alloc alloc, size_t length) const
	{
		std::vector<int, typename Rebind<Alloc, int>::type> vector(length*rand()/RAND_MAX);
		return vector.size();
	}
};

struct test_vector_dupe
{
	template <class Alloc>
	int test(Alloc alloc, size_t count) const
	{
		typedef std::vector<int, typename Rebind<Alloc, int>::type> Vector;
		Vector vector(count*rand()/RAND_MAX);
		Vector dupe = vector;
		return dupe.size();
	}
};

template <class Ty>
struct test_vector_sort
{
	template <class Alloc>
	int test(Alloc, size_t count) const
	{
		std::vector<Ty, typename Rebind<Alloc, Ty>::type> vector(count);
		for (size_t n = 0; n < count; ++n)
			vector[n] = count - n;
		sort(vector.begin(), vector.end());
		return 0;
	}
};

struct test_vector_accumulate
{
	template <class Alloc>
	int test(Alloc alloc, size_t length) const
	{
		std::vector<int, typename Rebind<Alloc, int>::type> vector(length*rand()/RAND_MAX);
		return accumulate(vector.begin(), vector.end(), 0);
	}
};

struct test_vector_accumulate_unaligned
{
	template <class Alloc>
	int test(Alloc alloc, size_t length) const
	{
		std::vector<Unaligned, typename Rebind<Alloc, Unaligned>::type> vector(length);
		int total = 0;
		BOOST_FOREACH(Unaligned const &val, vector)
		{
			total += val.c[2];
		}
		return total;
	}
};

template <class Ty>
struct test_list_create
{
	template <class Alloc>
	int test(Alloc alloc, size_t count) const
	{
		std::list<Ty, typename Rebind<Alloc, Ty>::type> list;
		fill_n(back_inserter(list), count, 42);
		return 0;
	}
};

struct test_list_dupe
{
	template <class Alloc>
	int test(Alloc alloc, size_t count) const
	{
		typedef std::list<int, typename Rebind<Alloc, int>::type> List;
		List list;
		fill_n(back_inserter(list), count, 42);
		List dupe = list;
		return dupe.size();
	}
};

template <class Ty>
struct test_list_sort
{
	template <class Alloc>
	int test(Alloc alloc, size_t count) const
	{
		std::list<Ty, typename Rebind<Alloc, Ty>::type> list;
		for (size_t n = 0; n < count; ++n)
			list.push_back(count - n);
		list.sort();
		return 0;
	}
};

struct test_map_list
{
	template <class Alloc>
	int test(Alloc alloc, size_t length) const
	{
		std::map<int
			, std::list<Unaligned, typename Rebind<Alloc, Unaligned>::type>
			, std::less<int>
			, typename Rebind<Alloc, int>::type
		> map;
		size_t mod = length/10;
		for (size_t n = 0; n < length; ++n)
		{
			int random = rand() % mod;
			map[random].push_back(n);
		}
		return 0;
	}
};

struct test_set_vector
{
	template <class Alloc>
	int test(Alloc alloc, size_t count) const
	{
		typedef std::vector<int, typename Rebind<Alloc, int>::type> Vector;
		typedef std::set<Vector, std::less<Vector>, typename Rebind<Alloc, Vector>::type> Set;
		int dummy = 0;
		Set set;
		for (size_t n = 0; n < count; ++n)
		{
			size_t size = count*rand()/RAND_MAX;
			Vector vector(size);
			generate_n(vector.begin(), size, rand);
			set.insert(vector);
			dummy += set.size();
		}
		return dummy;
	}
};

template <class Map>
int test_map_vector_impl(size_t length)
{
	Map map;
	size_t mod = length/10;
	if (mod == 0)
		mod = 5;
	for (size_t n = 0; n < length; ++n)
	{
		int random = rand() % mod;
		map[random].push_back(n);
	}
	return 0;
}

template <class Ty>
struct test_map_vector
{
	template <class Alloc>
	int test(Alloc, size_t length) const
	{
		return test_map_vector_impl<std::map<int
			, std::vector<Ty, typename Rebind<Alloc, Ty>::type>
			, std::less<int>
			, typename Rebind<Alloc, int>::type> >(length);
	}
};


template <class Al>
void *allocate_bytes(boost::pool<Al> &pool, size_t len)
{
	return pool.malloc(len);
}

template <size_t N, size_t M, class Al>
void *allocate_bytes(boost::monotonic::storage<N,M,Al> &storage, size_t len)
{
	return storage.allocate(len, 1);
}


template <class Ty, class Al>
Ty *allocate_object(boost::pool<Al> &pool)
{
	return pool.malloc(1);
}

template <class Ty>
Ty *allocate_object(boost::monotonic::allocator<Ty> &alloc)
{
	return alloc.allocate(1);
}

struct test_pool_bytes_alloc
{
	template <class Storage>
	int test(Storage &storage, size_t length)
	{
		for (size_t n = 0; n < length; ++n)
		{
			::allocate_bytes(storage, length*rand()/RAND_MAX);
		}
		return 0;
	}
};

template <class Ty>
struct test_pool_object_alloc
{
	template <class Storage>
	int test(Storage &storage, size_t length)
	{
		for (size_t n = 0; n < length; ++n)
		{
			::allocate_object(storage);
		}
		return 0;
	}
};

struct PoolResult 
{
	//boost::array<double, Type:: elapsed[;
	double pool_elapsed;
	double fast_pool_elapsed;
	double mono_elapsed;
	double local_mono_elapsed;
	double std_elapsed;
	double tbb_elapsed;
	PoolResult(double D = 0)
	{
		tbb_elapsed = pool_elapsed = fast_pool_elapsed = mono_elapsed = local_mono_elapsed = std_elapsed = D;
	}

	PoolResult& operator+=(PoolResult const &A)
	{
		pool_elapsed += A.pool_elapsed;
		fast_pool_elapsed += A.fast_pool_elapsed;
		mono_elapsed += A.mono_elapsed;
		local_mono_elapsed += A.local_mono_elapsed;
		std_elapsed += A.std_elapsed;
		tbb_elapsed += A.tbb_elapsed;
		return *this;
	}
	PoolResult& operator-=(PoolResult const &A)
	{
		pool_elapsed -= A.pool_elapsed;
		fast_pool_elapsed -= A.fast_pool_elapsed;
		mono_elapsed -= A.mono_elapsed;
		local_mono_elapsed -= A.local_mono_elapsed;
		std_elapsed -= A.std_elapsed;
		tbb_elapsed -= A.tbb_elapsed;
		return *this;
	}
	PoolResult& operator*=(PoolResult const &A)
	{
		pool_elapsed *= A.pool_elapsed;
		fast_pool_elapsed *= A.fast_pool_elapsed;
		mono_elapsed *= A.mono_elapsed;
		local_mono_elapsed *= A.local_mono_elapsed;
		std_elapsed *= A.std_elapsed;
		tbb_elapsed *= A.tbb_elapsed;
		return *this;
	}
	PoolResult& operator*=(double A)
	{
		pool_elapsed *= A;
		fast_pool_elapsed *= A;
		mono_elapsed *= A;
		local_mono_elapsed *= A;
		std_elapsed *= A;
		tbb_elapsed *= A;
		return *this;
	}

	void update_min(PoolResult const &other)
	{
		if (other.fast_pool_elapsed > 0)
			fast_pool_elapsed = std::min(fast_pool_elapsed, other.fast_pool_elapsed);
		if (other.pool_elapsed > 0)
			pool_elapsed = std::min(pool_elapsed, other.pool_elapsed);
		if (other.tbb_elapsed > 0)
			tbb_elapsed = std::min(tbb_elapsed, other.tbb_elapsed);
		if (other.std_elapsed > 0)
			std_elapsed = std::min(std_elapsed, other.std_elapsed);
	}
	void update_max(PoolResult const &other)
	{
		fast_pool_elapsed = std::max(fast_pool_elapsed, other.fast_pool_elapsed);
		pool_elapsed = std::max(pool_elapsed, other.pool_elapsed);
		tbb_elapsed = std::max(tbb_elapsed, other.tbb_elapsed);
		std_elapsed = std::max(std_elapsed, other.std_elapsed);
	}
};

PoolResult sqrt(PoolResult const &A)
{
	PoolResult R(A);
	R.fast_pool_elapsed = sqrt(R.fast_pool_elapsed); 
	R.mono_elapsed = sqrt(R.mono_elapsed); 
	R.local_mono_elapsed = sqrt(R.local_mono_elapsed); 
	R.std_elapsed = sqrt(R.std_elapsed); 
	R.tbb_elapsed = sqrt(R.tbb_elapsed); 
	return R;
}

PoolResult operator*(PoolResult const &A, PoolResult const &B)
{
	PoolResult R(A);
	R *= B;
	return R;
}

PoolResult operator+(PoolResult const &A, PoolResult const &B)
{
	PoolResult R(A);
	R += B;
	return R;
}

PoolResult operator-(PoolResult const &A, PoolResult const &B)
{
	PoolResult R(A);
	R -= B;
	return R;
}

typedef std::map<size_t /*count*/, PoolResult> PoolResults;

template <class Fun>
PoolResult run_test(size_t count, size_t length, Fun fun, Type types)
{
	// boost::pool<void> fails trying to form reference to void
	typedef Allocator<Type::FastPool, int> fast_pool_alloc;
	typedef Allocator<Type::Pool, int> pool_alloc;
	typedef Allocator<Type::Monotonic, int> mono_alloc;
	typedef Allocator<Type::TBB, int> tbb_alloc;
	typedef Allocator<Type::Standard, int> std_alloc;

	PoolResult result;

	if (types.Includes(Type::TBB))
	{
		srand(42);
		boost::timer timer;
		for (size_t n = 0; n < count; ++n)
		{
			{
				fun.test(tbb_alloc(), length);
			}
		}
		result.tbb_elapsed = timer.elapsed();
	}

	if (types.Includes(Type::FastPool))
	{
		srand(42);
		boost::timer timer;
		for (size_t n = 0; n < count; ++n)
		{
			{
				fun.test(fast_pool_alloc(), length);
			}
			boost::singleton_pool<boost::fast_pool_allocator_tag, sizeof(int)>::release_memory();
			boost::singleton_pool<boost::fast_pool_allocator_tag, sizeof(Unaligned)>::release_memory();
			// CJS ?? how to release memory created by a rebind<>'d fast_pool_allocator, such as xtree::rebind ??
		}
		result.fast_pool_elapsed = timer.elapsed();
	}

	if (types.Includes(Type::Pool))
	{
		srand(42);
		boost::timer timer;
		for (size_t n = 0; n < count; ++n)
		{
			{
				fun.test(pool_alloc(), length);
			}
			boost::singleton_pool<boost::pool_allocator_tag, sizeof(int)>::release_memory();
			boost::singleton_pool<boost::pool_allocator_tag, sizeof(Unaligned)>::release_memory();
			// CJS ?? how to release memory created by a rebind<>'d pool_allocator, such as xtree::rebind ??
		}
		result.pool_elapsed = timer.elapsed();
	}

	if (types.Includes(Type::Monotonic))
	{
		srand(42);
		boost::timer timer;
		for (size_t n = 0; n < count; ++n)
		{
			{
				fun.test(mono_alloc(), length);
			}
			boost::monotonic::reset_storage();
		}
		result.mono_elapsed = timer.elapsed();
	}

	if (types.Includes(Type::Monotonic))
	{
		//srand(42);
		//monotonic::local<monotonic::storage<100000> > storage;
		//boost::timer timer;
		//for (size_t n = 0; n < count; ++n)
		//{
		//	{
		//		fun.test(mono_alloc(), length);
		//	}
		//	storage.reset();
		//}
		//result.local_mono_elapsed = timer.elapsed();
	}

	if (types.Includes(Type::Standard))
	{
		srand(42);
		boost::timer timer;
		for (size_t n = 0; n < count; ++n)
		{
			{
				fun.test(std_alloc(), length);
			}
		}
		result.std_elapsed = timer.elapsed();
	}

	cout << "." << flush;
	return result;
}


template <class II>
typename boost::iterator_value<II>::type calc_mean(II first, II last, size_t num)
{
	return std::accumulate(first, last, typename boost::iterator_value<II>::type(0))*(1.0/num);
}

template <class II>
typename boost::iterator_value<II>::type calc_mean(II first, II last)
{
	if (first == last)
		throw std::range_error("calc_mean");
	return calc_mean(first, last, std::distance(first, last));
}

template <class II>
std::pair<typename boost::iterator_value<II>::type,typename boost::iterator_value<II>::type> standard_deviation_mean(II first, II last)
{
	typedef typename boost::iterator_value<II>::type Value;
	size_t length = std::distance(first, last);
	if (length == 0)
		throw std::range_error("standard_deviation_mean");
	Value mean = calc_mean(first, last, length);
	Value std_dev = 0;
	for (; first != last; ++first)
	{
		Value val = *first - mean;
		std_dev += val*val;
	}
	std_dev = sqrt(std_dev*(1./length));
	return std::make_pair(std_dev, mean);
}

template <class Cont>
std::pair<typename Cont::value_type, typename Cont::value_type> standard_deviation_mean(Cont const &cont)
{
	return standard_deviation_mean(cont.begin(), cont.end());
}

template <class Fun>
PoolResults run_tests(size_t count, size_t max_length, size_t num_iterations, const char *title, Fun fun, Type types = Type::All)
{
	boost::timer timer;
	cout << title << ": reps=" << count << ", len=" << max_length << ", steps=" << num_iterations;
	PoolResults results;
	for (size_t length = 1; length < max_length; length += max_length/num_iterations)
	{
		results[length] = run_test(count, length, fun, types);
	}
	cout << endl << "completed in " << timer.elapsed() << "s" << endl;
	return results;
}

struct OverallResult
{
	std::vector<PoolResult> cumulative;
};

std::vector<PoolResult> cumulative;
PoolResult result_min, result_max;
bool first_result = true;

void print_cumulative(std::vector<PoolResult> const &results)
{
	pair<PoolResult, PoolResult> dev_mean = standard_deviation_mean(results);
	size_t w = 10;

	cout << setw(w) << "scheme" << setw(w) << "mean" << setw(w) << "std-dev" << setw(w) << "min" << setw(w) << "max" << endl;
	//cout << setw(0) << "------------------------------------------------------" << endl;

	cout << setw(w) << "fast" << setprecision(3) << setw(w) << dev_mean.second.fast_pool_elapsed << setw(w) << dev_mean.first.fast_pool_elapsed << setw(w) << result_min.fast_pool_elapsed << setw(w) << result_max.fast_pool_elapsed << endl;
	cout << setw(w) << "pool" << setprecision(3) << setw(w) << dev_mean.second.pool_elapsed << setw(w) << dev_mean.first.pool_elapsed << setw(w) << result_min.pool_elapsed << setw(w) << result_max.pool_elapsed << endl;
	cout << setw(w) << "std" << setprecision(3) << setw(w) << dev_mean.second.std_elapsed << setw(w) << dev_mean.first.std_elapsed << setw(w) << result_min.std_elapsed << setw(w) << result_max.std_elapsed << endl;
	cout << setw(w) << "tbb" << setprecision(3) << setw(w) << dev_mean.second.tbb_elapsed << setw(w) << dev_mean.first.tbb_elapsed << setw(w) << result_min.tbb_elapsed << setw(w) << result_max.tbb_elapsed << endl;

	cout << endl;
}

void print(PoolResults const &results)
{
	size_t w = 10;
	cout << setw(4) << "len" << setw(w) << "fast/m" << setw(w) << "pool/m" << setw(w) << "std/m" << setw(w) << "tbb/m" << endl;//setw(w) << "tbb/l" << endl;
	cout << setw(0) << "--------------------------------------------" << endl;
	std::vector<PoolResult> results_vec;
	BOOST_FOREACH(PoolResults::value_type const &iter, results)
	{
		PoolResult const &result = iter.second;
		cout << setw(4) << iter.first << setprecision(3) << setw(w);
		if (result.mono_elapsed == 0)
		{
			cout << setw(w) << "mono = 0s" << endl;
			continue;
		}
		PoolResult ratio;
		ratio.fast_pool_elapsed = result.fast_pool_elapsed/result.mono_elapsed;
		ratio.pool_elapsed = result.pool_elapsed/result.mono_elapsed;
		ratio.std_elapsed = result.std_elapsed/result.mono_elapsed;
		ratio.tbb_elapsed = result.tbb_elapsed/result.mono_elapsed;
		ratio.mono_elapsed = 1;

		if (first_result)
		{
			result_min = result_max = ratio;
			first_result = false;
		}
		else
		{
			result_min.update_min(ratio);
			result_max.update_max(ratio);
		}
		cout << ratio.fast_pool_elapsed << setw(w) << ratio.pool_elapsed << setw(w) << ratio.std_elapsed << setw(w) << ratio.tbb_elapsed  << endl;//setw(w) << result.tbb_elapsed/result.local_mono_elapsed << endl;
		results_vec.push_back(ratio);
		cumulative.push_back(ratio);
	}
	cout << endl;
	print_cumulative(results_vec);
	cout << endl << endl;
}

void heading(const char *text, char star = '-')
{
	size_t len = 55;
	for (size_t n = 0; n < len; ++n)
		cout << star;
	cout << endl << "\t\t" << text << endl;
	for (size_t n = 0; n < len; ++n)
		cout << star;
	cout << endl;
}

#ifdef WIN32
//warning C4996: 'std::fill_n': Function call with parameters that may be unsafe
#pragma warning(disable:4996)
#endif

void test_pools()
{
	size_t length = 1;

	{
		boost::pool<> storage(sizeof(int));
		for (size_t n = 0; n < length; ++n)
		{
			int *p = reinterpret_cast<int *>(storage.malloc());
		}
	}
	{
		boost::object_pool<int> storage;
		for (size_t n = 0; n < length; ++n)
		{
			int *p = storage.malloc();
		}
	}

	if (0)
	{
		boost::object_pool<string> storage;
		for (size_t n = 0; n < length; ++n)
		{
			string *p = storage.malloc();
		}
		// crashes when storage is released?
	}

	{
		length = 4;
		// storage starts on the stack (in this case, 10k of it), then merges into the heap as needed
		monotonic::storage<10*1024> storage;
		for (size_t n = 0; n < length; ++n)
		{
			// create a new int from storage
			int &n0 = storage.create<int>();

			// create a new string (uses correct alignment)
			string const &s1 = storage.create<string>("foo");
			BOOST_ASSERT(s1 == "foo");

			// allocate 37 bytes with alignment 1
			char *array0 = storage.allocate_bytes(37);
			fill_n(array0, 37, 42);

			// allocate 2537 bytes with 64-byte alignment
			char *array1 = storage.allocate_bytes(2537, 64);
			fill_n(array1, 2537, 123);

			// allocate 1283 bytes with machine alignment
			char *array2 = storage.allocate_bytes<1283>();
			fill_n(array2, 1283, 42);

			array<Unaligned, 42> &array3 = storage.create<array<Unaligned, 42> >();

			// destroy objects. this only calls the destructors; it does not release memory
			storage.destroy(s1);

			cout << "storage.fixed, heap, total used: " << storage.fixed_used() << ", " << storage.heap_used() << ", " << storage.used() << endl;
		}
		// storage is released. if this was only ever on the stack, no work is done
	}
}

int main()
{
	cout << "results of running test at:" << endl;
	cout << "https://svn.boost.org/svn/boost/sandbox/monotonic/libs/monotonic/test/compare_memory_pool.cpp" << endl << endl;

	boost::timer timer;
	Type test_map_vector_types;
	Type test_dupe_list_types;

	bool run_small = 1;//true;
	bool run_medium = 1;//true;
	bool run_large = 1;//true;

	test_pools();

	// small-size (~100 elements) containers
	if (run_small)
	{
		heading("SMALL");
#ifndef WIN32
		print(run_tests(50000, 100, 10, "list_create<int>", test_list_create<int>()));
		print(run_tests(50000, 100, 10, "list_sort<int>", test_list_sort<int>()));
		print(run_tests(2000000, 100, 10, "vector_create<int>", test_vector_create()));
		print(run_tests(200000, 100, 10, "vector_sort<int>", test_vector_sort<int>()));
		print(run_tests(1000000, 100, 10, "vector_dupe", test_vector_dupe()));
		print(run_tests(20000, 100, 10, "list_dupe", test_list_dupe(), test_dupe_list_types));
		print(run_tests(500000, 100, 10, "vector_accumulate", test_vector_accumulate()));
		print(run_tests(5000, 100, 10, "set_vector", test_set_vector()));
		print(run_tests(10000, 100, 10, "map_vector<int>", test_map_vector<int>(), test_map_vector_types));
#else
		print(run_tests(50000, 100, 10, "list_create<int>", test_list_create<int>()));
		print(run_tests(5000, 100, 10, "list_sort<int>", test_list_sort<int>()));
		print(run_tests(2000000, 100, 10, "vector_create<int>", test_vector_create()));
		print(run_tests(20000, 100, 10, "vector_sort<int>", test_vector_sort<int>()));
		print(run_tests(100000, 100, 10, "vector_dupe", test_vector_dupe()));
		print(run_tests(20000, 100, 10, "list_dupe", test_list_dupe(), test_dupe_list_types));
		print(run_tests(500000, 100, 10, "vector_accumulate", test_vector_accumulate()));
		print(run_tests(50, 100, 10, "set_vector", test_set_vector()));
		print(run_tests(500, 100, 10, "map_vector<int>", test_map_vector<int>(), test_map_vector_types));
#endif

		heading("SUMMARY", '*');
		print_cumulative(cumulative);
	}

	// medium-size (~1000 elements) containers
	if (run_medium)
	{
		heading("MEDIUM");
		print(run_tests(10000, 1000, 10, "list_create<int>", test_list_create<int>()));
		print(run_tests(5000, 1000, 10, "list_sort<int>", test_list_sort<int>()));

#ifndef WIN32
		print(run_tests(1000000, 100000, 10, "vector_create<int>", test_vector_create()));
		print(run_tests(300, 10000, 10, "vector_sort<int>", test_vector_sort<int>()));
		print(run_tests(1000000, 10000, 10, "vector_dupe", test_vector_dupe()));
		print(run_tests(2000, 1000, 10, "list_dupe", test_list_dupe(), test_dupe_list_types));
		print(run_tests(5000000, 2000, 10, "vector_accumulate", test_vector_accumulate()));
		print(run_tests(500, 1000, 10, "set_vector", test_set_vector()));
		print(run_tests(500, 1000, 10, "map_vector<int>", test_map_vector<int>(), test_map_vector_types));
#else
		print(run_tests(1000, 100000, 10, "vector_create<int>", test_vector_create()));
		print(run_tests(30000, 1000, 10, "vector_sort<int>", test_vector_sort<int>()));
		print(run_tests(5000, 10000, 10, "vector_dupe", test_vector_dupe()));
		print(run_tests(500, 1000, 10, "list_dupe", test_list_dupe(), test_dupe_list_types));
		print(run_tests(50000, 2000, 10, "vector_accumulate", test_vector_accumulate()));
		print(run_tests(20, 500, 5, "set_vector", test_set_vector()));
		print(run_tests(50, 1000, 10, "map_vector<int>", test_map_vector<int>(), test_map_vector_types));
#endif
		heading("SUMMARY", '*');
		print_cumulative(cumulative);
	}

	// large-size (~1000000 elements) containers
	if (run_large)
	{
		heading("LARGE");
#ifndef WIN32
		print(run_tests(100, 25000, 10, "list_create<int>", test_list_create<int>()));
		print(run_tests(10, 100000, 10, "list_sort<int>", test_list_sort<int>()));
		print(run_tests(1000000, 10000000, 10, "vector_create<int>", test_vector_create()));
		print(run_tests(100, 500000, 10, "vector_sort<int>", test_vector_sort<int>()));

		print(run_tests(1000000, 100000000, 10, "vector_dupe", test_vector_dupe()));
		print(run_tests(100, 10000, 10, "list_dupe", test_list_dupe(), test_dupe_list_types));
		print(run_tests(1000000, 20000000, 10, "vector_accumulate", test_vector_accumulate()));
		print(run_tests(10, 50000, 10, "set_vector", test_set_vector()));
		print(run_tests(10, 10000, 10, "map_vector<int>", test_map_vector<int>(), test_map_vector_types));
#else
		print(run_tests(10, 25000, 10, "list_create<int>", test_list_create<int>()));
		print(run_tests(10, 100000, 10, "list_sort<int>", test_list_sort<int>()));
		print(run_tests(1000, 1000000, 10, "vector_create<int>", test_vector_create()));
		print(run_tests(300, 500000, 10, "vector_sort<int>", test_vector_sort<int>()));
		print(run_tests(200, 10000000, 10, "vector_dupe", test_vector_dupe()));
		print(run_tests(50, 10000, 10, "list_dupe", test_list_dupe(), test_dupe_list_types));
		print(run_tests(500, 10000000, 10, "vector_accumulate", test_vector_accumulate()));
		print(run_tests(5, 2000, 5, "set_vector", test_set_vector()));
		print(run_tests(10, 2000, 10, "map_vector<int>", test_map_vector<int>(), test_map_vector_types));
#endif
	}

	heading("FINAL SUMMARY", '*');
	print_cumulative(cumulative);

	cout << endl << "tests completed in " << setprecision(5) << timer.elapsed() << "s" << endl;

	return 0;
}

namespace boost 
{ 
	namespace monotonic 
	{
		static_storage_base<> default_static_storage;
		storage_base *static_storage = &default_static_storage;
	}
}


//EOF

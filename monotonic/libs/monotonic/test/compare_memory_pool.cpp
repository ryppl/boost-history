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

#include <vector>
#include <list>
#include <map>
#include <set>
#include <bitset>
#include <string>

#include <boost/timer.hpp>
#include <boost/monotonic/local.hpp>

#include "./AllocatorTypes.h"

template <class List>
void trace_ptrs(const char *t, List const &list)
{
	printf("%s\n", t);
	typename List::const_iterator A = list.begin(), B = list.end(), C;
	C = A;
	C++;
	for (; C != B; ++A, ++C)
	{
		char *p = (char *)&*C;
		char *q = (char *)&*A;
		printf("%p: %d\n", q, p - q);
	}
}

using namespace std;
using namespace boost;

struct test_vector_accumulate
{
	template <class Alloc>
	int test(Alloc alloc, size_t length) const
	{
		std::vector<int, typename Rebind<Alloc, int>::type> vector(length*rand()/RAND_MAX);
		return accumulate(vector.begin(), vector.end(), 0);
	}
};

struct test_vector_random_sort
{
	template <class Alloc>
	int test(Alloc alloc, size_t length) const
	{
		std::vector<int, typename Rebind<Alloc, int>::type> vector(length*rand()/RAND_MAX);
		generate_n(back_inserter(vector), length, rand);
		sort(vector.begin(), vector.end());
		return 0;
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

struct test_dupe_list
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

struct test_dupe_vector
{
	template <class Alloc>
	int test(Alloc alloc, size_t count) const
	{
		typedef std::vector<int, typename Rebind<Alloc, Unaligned>::type> Vector;
		Vector vector(count*rand()/RAND_MAX);
		Vector dupe = vector;
		return dupe.size();
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


template <class Ty>
struct test_list_sort
{
	template <class Alloc>
	int test(Alloc alloc, size_t count) const
	{
		std::list<Ty, typename Rebind<Alloc, Ty>::type> list;
		for (size_t n = 0; n < count; ++n)
			list.push_back(count - n);
		//trace_ptrs("trace", list);
		list.sort();
		return 0;
	}
};

template <class Ty>
struct test_sort_vector
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

struct PoolResult 
{
	double pool_elapsed;
	double fast_pool_elapsed;
	double mono_elapsed;
	double local_mono_elapsed;
	double std_elapsed;
	double tbb_elapsed;
	PoolResult()
	{
		tbb_elapsed = pool_elapsed = fast_pool_elapsed = mono_elapsed = local_mono_elapsed = std_elapsed = 0;
	}
};

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

void print(PoolResults const &results)
{
	size_t w = 10;
	cout << setw(4) << "len" << setw(w) << "fast/m" << setw(w) << "pool/m" << setw(w) << "std/m" << setw(w) << "tbb/m" << endl;//setw(w) << "tbb/l" << endl;
	cout << setw(0) << "--------------------------------------------" << endl;
	BOOST_FOREACH(PoolResults::value_type const &iter, results)
	{
		PoolResult const &result = iter.second;
		cout << setw(4) << iter.first << setprecision(3) << setw(w) << result.fast_pool_elapsed/result.mono_elapsed << setw(w) << result.pool_elapsed/result.mono_elapsed << setw(w) << result.std_elapsed/result.mono_elapsed << setw(w) << result.tbb_elapsed/result.mono_elapsed  << endl;//setw(w) << result.tbb_elapsed/result.local_mono_elapsed << endl;
	}
	cout << endl;
}

int main()
{
	boost::timer timer;
	Type test_map_vector_types;
	Type test_dupe_list_types;

	bool run_large = 0;//true;
	bool run_medium = 1;//true;
	bool run_small = 0;//true;

	// large-size (~1000000 elements) containers
	if (run_large)
	{
		print(run_tests(10, 10000, 10, "list_create<int>", test_list_create<int>()));
		print(run_tests(20, 1000000, 10, "vector_sort<int>", test_sort_vector<int>()));
		print(run_tests(5, 100000, 10, "list_sort<int>", test_list_sort<int>()));

#ifndef WIN32
		print(run_tests(1000000, 10000, 10, "dupe_vector", test_dupe_vector()));
		print(run_tests(20000, 1000, 10, "dupe_list", test_dupe_list(), test_dupe_list_types));
		print(run_tests(5000000, 2000, 10, "vector_accumulate", test_vector_accumulate()));
		print(run_tests(2000, 1000, 10, "vector_random_sort", test_vector_random_sort()));
		print(run_tests(5000, 500, 10, "set_vector", test_set_vector()));
		print(run_tests(500, 1000, 10, "map_vector<int>", test_map_vector<int>(), test_map_vector_types));
#else
		print(run_tests(1000, 100000000, 10, "dupe_vector", test_dupe_vector()));
		print(run_tests(10, 10000, 10, "dupe_list", test_dupe_list(), test_dupe_list_types));
		print(run_tests(500, 2000000, 10, "vector_accumulate", test_vector_accumulate()));
		print(run_tests(10, 100000, 5, "vector_random_sort", test_vector_random_sort()));
		print(run_tests(2, 50000, 5, "set_vector", test_set_vector()));
		print(run_tests(2, 1000000, 10, "map_vector<int>", test_map_vector<int>(), test_map_vector_types));
#endif
	}

	// small-size (~100 elements) containers
	if (run_small)
	{
		print(run_tests(5000, 100, 10, "list_sort<int>", test_list_sort<int>()));
		print(run_tests(5000, 100, 10, "list_create<int>", test_list_create<int>()));
		print(run_tests(200000, 100, 10, "sort_vector<int>", test_sort_vector<int>()));

#ifndef WIN32
		print(run_tests(1000000, 100, 10, "dupe_vector", test_dupe_vector()));
		print(run_tests(200000, 100, 10, "dupe_list", test_dupe_list(), test_dupe_list_types));
		print(run_tests(50000000, 100, 10, "vector_accumulate", test_vector_accumulate()));
		print(run_tests(20000, 100, 10, "vector_random_sort", test_vector_random_sort()));
		print(run_tests(50000, 10, 10, "set_vector", test_set_vector()));
		print(run_tests(5000, 100, 10, "map_vector<int>", test_map_vector<int>(), test_map_vector_types));
#else
		print(run_tests(500000, 100, 10, "dupe_vector", test_dupe_vector()));
		print(run_tests(50000, 100, 10, "dupe_list", test_dupe_list(), test_dupe_list_types));
		print(run_tests(500000, 100, 10, "vector_accumulate", test_vector_accumulate()));
		print(run_tests(2000, 100, 10, "vector_random_sort", test_vector_random_sort()));
		print(run_tests(200, 50, 10, "set_vector", test_set_vector()));
		print(run_tests(800, 100, 10, "map_vector<int>", test_map_vector<int>(), test_map_vector_types));
#endif
	}

	// medium-size (~1000 elements) containers
	if (run_medium)
	{
		print(run_tests(1000, 1000, 10, "list_create<int>", test_list_create<int>()));
		print(run_tests(5000, 1000, 10, "list_sort<int>", test_list_sort<int>()));
		print(run_tests(20000, 1000, 10, "sort_vector<int>", test_sort_vector<int>()));

#ifndef WIN32
		print(run_tests(1000000, 10000, 10, "dupe_vector", test_dupe_vector()));
		print(run_tests(20000, 1000, 10, "dupe_list", test_dupe_list(), test_dupe_list_types));
		print(run_tests(5000000, 2000, 10, "vector_accumulate", test_vector_accumulate()));
		print(run_tests(2000, 1000, 10, "vector_random_sort", test_vector_random_sort()));
		print(run_tests(5000, 500, 10, "set_vector", test_set_vector()));
		print(run_tests(500, 1000, 10, "map_vector<int>", test_map_vector<int>(), test_map_vector_types));
#else
		print(run_tests(10000, 10000, 10, "dupe_vector", test_dupe_vector()));
		print(run_tests(1000, 1000, 10, "dupe_list", test_dupe_list(), test_dupe_list_types));
		print(run_tests(50000, 2000, 10, "vector_accumulate", test_vector_accumulate()));
		print(run_tests(1000, 1000, 10, "vector_random_sort", test_vector_random_sort()));
		print(run_tests(20, 500, 5, "set_vector", test_set_vector()));
		print(run_tests(50, 1000, 10, "map_vector<int>", test_map_vector<int>(), test_map_vector_types));
#endif
	}

	cout << "tests completed in " << timer.elapsed() << "s" << endl;

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

// (C) 2009 Christian Schladetsch
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

// the documentation is at https://svn.boost.org/svn/boost/sandbox/monotonic/libs/monotonic/doc/index.html

// the sandbox is at https://svn.boost.org/svn/boost/sandbox/monotonic/

#include <iostream> 
#include <iomanip> 
#include <numeric>

#include <vector>
#include <list>
#include <map>
#include <set>

#include <boost/pool/pool_alloc.hpp>
#undef max
#undef min
#include <boost/monotonic/allocator.hpp>
#include <boost/monotonic/local.hpp>

#include <boost/timer.hpp>

using namespace std;
using namespace boost;

struct Unaligned
{
	int num;
	char c;
	Unaligned() : num(0), c(0) { }
	Unaligned(char C) : num(0), c(C) { }
};

bool operator<(Unaligned const &A, Unaligned const &B)
{
	return A.c < B.c;
}

template <class Alloc, class T>
struct Rebind
{
	typedef typename Alloc::template rebind<T>::other type;
};

struct thrash_pool
{
	template <class Alloc>
	int test(Alloc alloc, size_t length) const
	{
		std::vector<int, typename Rebind<Alloc, int>::type> vector;
		vector.resize(length*rand()/RAND_MAX);
		return accumulate(vector.begin(), vector.end(), 0);
	}
};

struct thrash_pool_sort
{
	template <class Alloc>
	int test(Alloc alloc, size_t length) const
	{
		std::vector<int, typename Rebind<Alloc, int>::type> vector;
		vector.resize(length*rand()/RAND_MAX);
		generate_n(back_inserter(vector), length, rand);
		sort(vector.begin(), vector.end());
		return 0;
	}
};

struct thrash_pool_sort_list_int
{
	template <class Alloc>
	int test(Alloc alloc, size_t length) const
	{
		std::list<int, typename Rebind<Alloc, int>::type> list;
		generate_n(back_inserter(list), length, rand);
		list.sort();
		return 0;
	}
};

struct thrash_pool_iter
{
	template <class Alloc>
	int test(Alloc alloc, size_t length) const
	{
		std::vector<Unaligned, typename Rebind<Alloc, Unaligned>::type> vector;
		vector.resize(length);
		int total = 0;
		BOOST_FOREACH(Unaligned const &val, vector)
		{
			total += val.c;
		}
		return total;
	}
};


struct thrash_pool_map_list_unaligned
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

struct test_dupe_list_unaligned
{
	template <class Alloc>
	int test(Alloc alloc, size_t count) const
	{
		typedef std::list<Unaligned, typename Rebind<Alloc, Unaligned>::type> List;
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
		int dummy = 0;
		for (size_t n = 0; n < count; ++n)
		{
			Vector dupe = vector;
			dummy += dupe.size();
		}
		return dummy;
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

struct PoolResult 
{
	double pool_elapsed;
	double fast_pool_elapsed;
	double mono_elapsed;
	double local_mono_elapsed;
	double std_elapsed;
	PoolResult()
	{
		pool_elapsed = fast_pool_elapsed = mono_elapsed = local_mono_elapsed = std_elapsed = -1;
	}
};

typedef std::map<size_t /*count*/, PoolResult> PoolResults;

template <class Fun>
PoolResult compare_memory_pool(size_t count, size_t length, Fun fun)
{
	typedef boost::fast_pool_allocator<int
		, boost::default_user_allocator_new_delete
		, boost::details::pool::null_mutex>
		fast_pool_alloc;
	typedef boost::pool_allocator<int
		, boost::default_user_allocator_new_delete
		, boost::details::pool::null_mutex>
		pool_alloc;
	typedef monotonic::allocator<int> mono_alloc;
	typedef std::allocator<int > std_alloc;

	PoolResult result;

	// test boost::fast_pool_allocator
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

	// test boost::pool_allocator
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

	// test monotonic
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

	// test local monotonic
	{
		srand(42);
		monotonic::local<> storage;
		boost::timer timer;
		for (size_t n = 0; n < count; ++n)
		{
			{
				fun.test(mono_alloc(), length);
			}
			storage.reset();
		}
		result.local_mono_elapsed = timer.elapsed();
	}

	// test std
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
	//cout << length << ": fast_pool, pool, std, mono, local: " << result.fast_pool_elapsed << ", " << result.pool_elapsed << ", " << result.std_elapsed << ", " << result.mono_elapsed << ", " << result.local_mono_elapsed << endl;
	return result;
}

template <class Fun>
PoolResults compare_memory_pool(size_t count, size_t max_length, size_t num_iterations, const char *title, Fun fun)
{
	cout << title << ": reps=" << count << ", len=" << max_length << ", steps=" << num_iterations;
	PoolResults results;
	for (size_t length = 10; length < max_length; length += max_length/num_iterations)
	{
		results[length] = compare_memory_pool(count, length, fun);
	}
	cout << endl;
	return results;
}

void PrintResults(PoolResults const &results)
{
	size_t w = 10;
	cout << setw(6) << "length" << setw(w) << "fastp" << setw(w) << "pool" << setw(w) << "std" << setw(w) << "mono" << setw(w) /*<< "local" << setw(w)*/ << "fast/mono" << setw(w) << "pool/mono" << setw(w) << "std/mono" << endl;
	cout << setw(0) << "------------------------------------------------------------------------------" << endl;
	BOOST_FOREACH(PoolResults::value_type const &iter, results)
	{
		PoolResult const &result = iter.second;
		cout << setw(6) << iter.first << setprecision(4) << setw(w) <<result.fast_pool_elapsed << setw(w) << result.pool_elapsed << setw(w) << result.std_elapsed << setw(w) << result.mono_elapsed /*<< setw(w) << result.local_mono_elapsed*/ << setw(w) << 100.*result.fast_pool_elapsed/result.mono_elapsed << "%" << setw(w) << 100.*result.pool_elapsed/result.mono_elapsed << "%" << setw(w) << 100.*result.std_elapsed/result.mono_elapsed << "%" << endl;
	}
	cout << endl;
}

int main()
{
	boost::timer timer;
	PrintResults(compare_memory_pool(100, 1000, 10, "thrash_pool_sort_list_int", thrash_pool_sort_list_int()));

#ifdef WIN32
	// boost::fast_pool seems bad at this test with MSVC, so do it less.
	// this will result in less accurate results, but that doesnt matter because monotonic is orders of magnitudes faster
	// than fast_pool here...
	PrintResults(compare_memory_pool(10, 1000, 5, "test_set_vector", test_set_vector()));
#else
	PrintResults(compare_memory_pool(500, 1000, 10, "test_set_vector", test_set_vector()));
#endif

	PrintResults(compare_memory_pool(500, 2000, 10, "test_dupe_list", test_dupe_list()));
	PrintResults(compare_memory_pool(500, 2000, 10, "test_dupe_vector", test_dupe_vector()));
	PrintResults(compare_memory_pool(50000, 2000, 10, "thrash_pool", thrash_pool()));
	PrintResults(compare_memory_pool(50000, 2000, 10, "thrash_pool_iter", thrash_pool_iter()));
	PrintResults(compare_memory_pool(1000, 1000, 10, "thrash_pool_sort", thrash_pool_sort()));
	PrintResults(compare_memory_pool(1000, 2000, 10, "thrash_pool_map_list_unaligned", thrash_pool_map_list_unaligned()));

	cout << "tests completed in " << setprecision(2) << timer.elapsed() << "s" << endl;

	return 0;
}

namespace boost 
{ 
	namespace monotonic 
	{
		// temp. hax to create global storage
		static_storage_base<> default_static_storage;
		storage_base *static_storage = &default_static_storage;
	}
}

//EOF

#include <ios>
#include <iomanip> 
#include <list>
#include <numeric>
#include <vector>
#include <boost/pool/pool.hpp>
#include <boost/pool/pool_alloc.hpp>

struct thrash_pool
{
	template <class Pool>
	int operator()(size_t length, Pool &pool) const
	{
		pool.resize(length*rand()/RAND_MAX);
		return accumulate(pool.begin(), pool.end(), 0);
	}
};

struct thrash_pool_sort
{
	template <class Pool>
	int operator()(size_t length, Pool &pool) const
	{
		pool.resize(length*rand()/RAND_MAX);
		generate_n(back_inserter(pool), length, rand);
		sort(pool.begin(), pool.end());
		return 0;
	}
};

struct thrash_pool_iter
{
	struct Unaligned
	{
		int num;
		char c;
	};
	template <class Pool>
	int operator()(size_t length, Pool &pool) const
	{
		typedef typename Pool::allocator_type allocator;
		std::vector<Unaligned, typename allocator::template rebind<Unaligned>::other> vec;
		vec.resize(length);
		int total = 0;
		BOOST_FOREACH(Unaligned const &val, vec)
		{
			total += val.c;
		}
		return total;
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

typedef std::map<size_t, PoolResult> PoolResults;

template <class Fun>
PoolResult compare_memory_pool(size_t count, size_t length, Fun fun)
{
	typedef std::vector<int, boost::pool_allocator<int,
		boost::default_user_allocator_new_delete,
		boost::details::pool::null_mutex> > pool_v;
	typedef std::vector<int, boost::fast_pool_allocator<int,
		boost::default_user_allocator_new_delete,
		boost::details::pool::null_mutex> > fast_pool_v;
	typedef std::vector<int, boost::monotonic::allocator<int> > mono_v;
	typedef std::vector<int > std_v;

	PoolResult result;

	// test boost::fast_pool_allocator
	{
		boost::timer timer;
		srand(42);
		for (size_t n = 0; n < count; ++n)
		{
			{
				fast_pool_v pool;
				fun(length, pool);
			}
			boost::singleton_pool<boost::fast_pool_allocator_tag, sizeof(int)>::release_memory();
		}
		result.fast_pool_elapsed = timer.elapsed();
	}

	// test boost::pool_allocator. dont bother for larger sizes as it is known to be slow
	if (1)//length < 3000)
	{
		boost::timer timer;
		srand(42);
		for (size_t n = 0; n < count; ++n)
		{
			{
				pool_v pool;
				fun(length, pool);
			}
			boost::singleton_pool<boost::pool_allocator_tag, sizeof(int)>::release_memory();
		}
		result.pool_elapsed = timer.elapsed();
	}
	else
	{
		result.pool_elapsed = -1;
	}

	// test monotonic
	{
		srand(42);
		boost::timer timer;
		for (size_t n = 0; n < count; ++n)
		{
			{
				mono_v pool;
				fun(length, pool);
			}
			boost::monotonic::get_storage().reset();
		}
		result.mono_elapsed = timer.elapsed();
	}


	// test local monotonic
	if (1)
	{
		srand(42);
		monotonic::local<monotonic::storage<100000> > storage;
		boost::timer timer;
		for (size_t n = 0; n < count; ++n)
		{
			{
				mono_v pool;
				fun(length, pool);
			}
			storage.reset();
		}
		result.local_mono_elapsed = timer.elapsed();
	}

	// test std
	if (1)
	{
		srand(42);
		boost::timer timer;
		for (size_t n = 0; n < count; ++n)
		{
			{
				std_v pool;
				fun(length, pool);
			}
		}
		result.std_elapsed = timer.elapsed();
	}

	cout << length << ": fast_pool, pool, std, mono, local: " << result.fast_pool_elapsed << ", " << result.pool_elapsed << ", " << result.std_elapsed << ", " << result.mono_elapsed << ", " << result.local_mono_elapsed << endl;
	return result;
}

template <class Fun>
PoolResults compare_memory_pool(size_t count, size_t max_length, size_t num_iterations, Fun fun)
{
	PoolResults results;
	for (size_t length = 10; length < max_length; length += max_length/num_iterations)
	{
		results[length] = compare_memory_pool(count, length, fun);
	}
	return results;
}

void PrintResults(PoolResults const &results)
{
	size_t w = 10;
	cout << setw(0) << "count" << setw(w) << "fast_p" << setw(w) << "pool" << setw(w) << "std" << setw(w) << "mono" << setw(w) << "local" << setw(w) << "fp/mono" << setw(w) << "fp/local" << endl;
	BOOST_FOREACH(PoolResults::value_type const &iter, results)
	{
		PoolResult const &result = iter.second;
		cout << setw(0) << setprecision(4) << iter.first << setw(w) <<result.fast_pool_elapsed << setw(w) << result.pool_elapsed << setw(w) << result.std_elapsed << setw(w) << result.mono_elapsed << setw(w) << result.local_mono_elapsed << setw(w) << 100.*result.fast_pool_elapsed/result.mono_elapsed << "%" << setw(w) << 100.*result.fast_pool_elapsed/result.local_mono_elapsed << "%" <<endl;
	}
}

void compare_memory_pool()
{
	cout << "thrash_pool_iter" << endl;
	PrintResults(compare_memory_pool(50000, 1000, 10, thrash_pool_iter()));
	cout << "thrash_pool" << endl;
	PrintResults(compare_memory_pool(50000, 1000, 10, thrash_pool()));
	cout << "thrash_pool_sort" << endl;
	PrintResults(compare_memory_pool(1000, 500, 10, thrash_pool_sort()));
}

//EOF




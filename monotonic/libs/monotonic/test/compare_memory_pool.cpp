#include <list>
#include <boost/pool/pool.hpp>
#include <boost/pool/pool_alloc.hpp>

struct thrash_pool
{
	template <class Pool>
	void operator()(size_t length, Pool &pool) const
	{
		fill_n(back_inserter(pool), length, 42);
	}
};

struct thrash_pool_sort
{
	template <class Pool>
	int operator()(size_t length, Pool &pool) const
	{
		srand(42);
		generate_n(back_inserter(pool), length, rand);
		pool.sort();
		int total = 0;
		BOOST_FOREACH(int n, pool)
		{
			total += n;
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
		pool_elapsed = fast_pool_elapsed = mono_elapsed = local_mono_elapsed = std_elapsed = 0;
	}
};

template <class Fun>
PoolResult compare_memory_pool(size_t count, size_t length, Fun fun)
{
	cout << "compare_memory_pool: " << count << ", " << length << endl;
	typedef std::list<int, boost::pool_allocator<int,
		boost::default_user_allocator_new_delete,
		boost::details::pool::null_mutex> > pool_v;
	typedef std::list<int, boost::fast_pool_allocator<int,
		boost::default_user_allocator_new_delete,
		boost::details::pool::null_mutex> > fast_pool_v;
	typedef std::list<int, boost::monotonic::allocator<int> > mono_v;
	typedef std::list<int > std_v;

	PoolResult result;

	// test boost::fast_pool_allocator
	{
		boost::timer timer;
		for (size_t n = 0; n < count; ++n)
		{
			srand(42);
			{
				fast_pool_v pool;
				fun(length, pool);
			}
			boost::singleton_pool<boost::fast_pool_allocator_tag, sizeof(int)>::release_memory();
		}
		result.fast_pool_elapsed = timer.elapsed();
	}

	// test boost::pool_allocator. dont bother for larger sizes as it is known to be slow
	if (length < 3000)
	{
		boost::timer timer;
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
	{
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
	if (0)
	{
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

typedef std::map<size_t, PoolResult> PoolResults;

template <class Fun>
PoolResults compare_memory_pool(size_t count, size_t max_length, size_t num_iterations, Fun fun)
{
	PoolResults results;
	//results[5] = compare_memory_pool(count*100, 5, fun);
	//results[20] = compare_memory_pool(count*100, 20, fun);
	//results[50] = compare_memory_pool(count*100, 50, fun);
	//results[100] = compare_memory_pool(count*100, 100, fun);

	for (size_t length = 10; length < max_length; length += max_length/num_iterations)
	{
		results[length] = compare_memory_pool(count, length, fun);
	}
	return results;
}

void compare_memory_pool()
{
	size_t num_outter_loops = 1000;
	PoolResults r0 = compare_memory_pool(num_outter_loops, 10000, 10, thrash_pool());
	cout << "thrash_pool" << endl;
	cout << "count\t" << "fast_p\t" << "pool\t" << "std\t" << "local\t" << "mono\t" << "fp/mono\t" << "fp/local" << endl;
	BOOST_FOREACH(PoolResults::value_type const &iter, r0)
	{
		PoolResult const &result = iter.second;
		cout << iter.first << '\t' << result.fast_pool_elapsed << '\t' << result.pool_elapsed << "\t" << result.std_elapsed << '\t' << result.local_mono_elapsed << '\t' << result.mono_elapsed << '\t' << 100.*result.fast_pool_elapsed/result.mono_elapsed << "%\t" << '\t' << 100.*result.fast_pool_elapsed/result.local_mono_elapsed << endl;
	}

	PoolResults r1 = compare_memory_pool(num_outter_loops, 1000, 10, thrash_pool_sort());
	cout << "thrash_pool_sort" << endl;
	cout << "count\t" << "fast_p\t" << "pool\t" << "std\t" << "local\t" << "mono\t" << "fp/mono\t" << "fp/local" << endl;
	BOOST_FOREACH(PoolResults::value_type const &iter, r1)
	{
		PoolResult const &result = iter.second;
		cout << iter.first << '\t' << result.fast_pool_elapsed << '\t' << result.pool_elapsed << "\t" << result.std_elapsed << '\t' << result.local_mono_elapsed << '\t' << result.mono_elapsed << '\t' << 100.*result.fast_pool_elapsed/result.mono_elapsed << "%\t" << '\t' << 100.*result.fast_pool_elapsed/result.local_mono_elapsed << endl;
	}
}

//EOF

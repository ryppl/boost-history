#include <list>
#include <boost/pool/pool.hpp>
#include <boost/pool/pool_alloc.hpp>

template <class Pool>
int thrash_pool(size_t length, Pool &pool)
{
	generate_n(back_inserter(pool), length, rand);
	int total = 0;
	BOOST_FOREACH(int n, pool)
	{
		total += n;
	}
	return total;
}

struct PoolResult 
{
	double pool_elapsed;
	double fast_pool_elapsed;
	double mono_elapsed;
	double std_elapsed;
};

PoolResult compare_memory_pool(size_t count, size_t length)
{
	typedef std::list<int, boost::pool_allocator<int> > pool_v;
	typedef std::list<int, boost::fast_pool_allocator<int> > fast_pool_v;
	typedef std::list<int, boost::monotonic::allocator<int> > mono_v;
	typedef std::list<int > std_v;

	PoolResult result;

	// test boost::fast_pool_allocator
	{
		boost::timer timer;
		for (size_t n = 0; n < count; ++n)
		{
			{
				pool_v pool;
				thrash_pool(length, pool);
			}
			boost::singleton_pool<boost::fast_pool_allocator_tag, sizeof(int)>::release_memory();
		}
		result.fast_pool_elapsed = timer.elapsed();
	}

	// test boost::pool_allocator
	{
		boost::timer timer;
		for (size_t n = 0; n < count; ++n)
		{
			{
				pool_v pool;
				thrash_pool(length, pool);
			}
			boost::singleton_pool<boost::pool_allocator_tag, sizeof(int)>::release_memory();
		}
		result.pool_elapsed = timer.elapsed();
	}

	// test monotonic
	{
		boost::timer timer;
		for (size_t n = 0; n < count; ++n)
		{
			{
				mono_v pool;
				thrash_pool(length, pool);
			}
			monotonic::static_storage.reset();
		}
		result.mono_elapsed = timer.elapsed();
	}

	// test std
	{
		boost::timer timer;
		for (size_t n = 0; n < count; ++n)
		{
			{
				std_v pool;
				thrash_pool(length, pool);
			}
			monotonic::static_storage.reset();
		}
		result.std_elapsed = timer.elapsed();
	}

	cout << length << ": fast_pool, pool, std, mono: " << result.fast_pool_elapsed << ", " << result.pool_elapsed << ", " << result.std_elapsed << ", " << result.mono_elapsed << endl;
	return result;
}

void compare_memory_pool()
{
	const size_t count = 10000;
	const size_t max_length = 10000;
	
	typedef std::map<size_t, PoolResult> Results;
	Results results;
	for (size_t length = 10; length < max_length; length += 500)
	{
		results[length] = compare_memory_pool(count, length);
	}
	cout << "count\t" << "fast_pool\t" << "pool\t" << "std\t" << "mono\n" << "fast_pool/mono" << endl;
	BOOST_FOREACH(Results::value_type const &iter, results)
	{
		PoolResult const &result = iter.second;
		cout << iter.first << '\t' << result.fast_pool_elapsed << '\t' << result.pool_elapsed << "\t\t" << result.std_elapsed << '\t' << result.mono_elapsed << '\t' << 100.*result.fast_pool_elapsed/result.mono_elapsed << "%" << endl;
	}
}

//EOF

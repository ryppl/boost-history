
template <class Map>
void test_map_list_impl(size_t count, Map &map)
{
	typedef typename Map::mapped_type List;
	size_t mod = count/10;
	for (size_t n = 0; n < count; ++n)
	{
		int random = rand() % mod;
		typename Map::iterator iter = map.find(random);
		if (iter == map.end())
		{
			map.insert(make_pair(random, List(map.get_allocator())));
		}
		else
		{
			iter->second.push_back(n);
		}
	}
}

struct Result
{
	double mono;
	double standard;
	double static_monotonic;
};

Result test_map_list(size_t outter_loops, size_t inner_loops, monotonic::storage_base &storage)
{
	typedef std::map<int, std::list<int> > Map;
	typedef std::map<int, std::list<int, monotonic::allocator<int> >, std::less<int>, monotonic::allocator<int> > MonoMap;

	Result result;


	// use monotonic allocator with supplied storage
	{
		boost::timer timer;
		for (size_t n = 0; n < outter_loops; ++n)
		{
			{
				MonoMap map(std::less<int>(), storage);
				test_map_list_impl(inner_loops, map);
			}
			storage.reset();
		}
		result.mono = timer.elapsed();
	}

	// use standard allocator
	{
		boost::timer timer;
		for (size_t n = 0; n < outter_loops; ++n)
		{
			{
				Map map;
				test_map_list_impl(inner_loops, map);
			}
		}
		result.standard = timer.elapsed();
	}

	// use static monotonic allocator
	{
		boost::timer timer;
		monotonic::static_storage.reset();
		for (size_t n = 0; n < outter_loops; ++n)
		{
			{
				MonoMap map;
				test_map_list_impl(inner_loops, map);
			}
			monotonic::static_storage.reset();
		}
		result.static_monotonic = timer.elapsed();
	}


	cout << "test_map_list: " << inner_loops << ": " << result.mono << ", " << result.static_monotonic << ", " << result.standard  << endl;
	return result;
}

void test_map_list_heap_stack()
{
	const size_t outter_loops = 10*1000;
	const size_t inner_loops = 10000;

	typedef std::map<size_t, Result > Results;
	Results results;

	monotonic::storage<> storage;
	for (size_t inner = 100; inner < inner_loops; inner += 1000)
	{
		results[inner] = test_map_list(outter_loops, inner, storage);
	}

	cout << "test_map_list" << endl;
	cout << "count\t" << "mono\t" << "mono_static\t" << "std\t" << "std/mono\t" << "std/mono_static" << endl;
	BOOST_FOREACH(Results::value_type const &iter, results)
	{
		Result const &result = iter.second;
		double mono_time = result.mono;
		double std_time = result.standard;
		double static_time = result.static_monotonic;
		double perc1 = 100.*std_time/mono_time;
		double perc2 = 100.*std_time/static_time;
		cout << iter.first << '\t' << mono_time << '\t' << static_time << "\t\t" << std_time << '\t' << perc1 << "%\t" << perc2 << "%"<< endl;
	}
}

//EOF

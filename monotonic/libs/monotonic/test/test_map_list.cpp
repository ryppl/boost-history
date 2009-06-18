
template <class Map>
void test_map_list_impl(size_t count, Map &map)
{
	typedef typename Map::mapped_type List;
	size_t mod = count/10;
	for (size_t n = 0; n < count; ++n)
	{
		int random = rand() % mod;
		map[random].push_back(n);
	}
	//BOOST_FOREACH(typename Map::value_type &val, map)
	//{
	//	val.second.sort();
	//}
}

struct Result
{
	double local;
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
				MonoMap map;
				test_map_list_impl(inner_loops, map);
			}
			storage.reset();
		}
		result.local = timer.elapsed();
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
		boost::monotonic::storage_base *current = &monotonic::get_storage();
		boost::monotonic::default_storage();
		for (size_t n = 0; n < outter_loops; ++n)
		{
			{
				MonoMap map;
				test_map_list_impl(inner_loops, map);
			}
			boost::monotonic::reset_storage();
		}
		boost::monotonic::set_storage(*current);
		result.static_monotonic = timer.elapsed();
	}


	cout << "test_map_list: " << inner_loops << ": " << result.local << ", " << result.static_monotonic << ", " << result.standard  << endl;
	return result;
}

void test_map_list_heap_stack()
{
	const size_t outter_loops = 10*1000;
	const size_t inner_loops = 10000;

	typedef std::map<size_t, Result > Results;
	Results results;

	monotonic::local<monotonic::storage<1000000, 0> > storage;
	for (size_t inner = 100; inner < inner_loops; inner += 1000)
	{
		results[inner] = test_map_list(outter_loops, inner, storage);
	}

	cout << "test_map_list" << endl;
	cout << "count\t" << "local\t" << "mono_static\t" << "std\t" << "std/local\t" << "std/mono_static" << endl;
	BOOST_FOREACH(Results::value_type const &iter, results)
	{
		Result const &result = iter.second;
		double mono_time = result.local;
		double std_time = result.standard;
		double static_time = result.static_monotonic;
		double perc1 = 100.*std_time/mono_time;
		double perc2 = 100.*std_time/static_time;
		cout << iter.first << '\t' << mono_time << '\t' << static_time << "\t\t" << std_time << '\t' << perc1 << "%\t" << perc2 << "%"<< endl;
	}
}

//EOF

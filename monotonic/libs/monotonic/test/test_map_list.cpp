
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

pair<double,double> test_map_list(size_t outter_loops, size_t inner_loops, monotonic::storage_base &storage)
{
	boost::timer t0;
	for (size_t n = 0; n < outter_loops; ++n)
	{
		{
			typedef std::map<int, std::list<int, monotonic::allocator<int> >, std::less<int>, monotonic::allocator<int> > Map;
			Map map(std::less<int>(), storage);
			test_map_list_impl(inner_loops, map);
		}
		storage.reset();
	}
	double e0 = t0.elapsed();

	boost::timer t1;
	for (size_t n = 0; n < outter_loops; ++n)
	{
		typedef std::map<int, std::list<int> > Map;
		Map map;
		test_map_list_impl(inner_loops, map);
	}
	double e1 = t1.elapsed();

	cout << "test_map_list: " << inner_loops << ": " << e0 << ", " << e1 << endl;
	return make_pair(e0, e1);
}

void test_map_list_heap_stack()
{
	const size_t outter_loops = 10*1000;
	const size_t inner_loops = 10000;

	monotonic::storage<1000000> storage;
	typedef std::map<size_t, pair<double, double> > Results;
	Results results;

	// do a test with a large dataset on the heap
	if (0)
	{
		const size_t buffer_size = 10*1000*1000;
		monotonic::storage<buffer_size> *storage = new monotonic::storage<buffer_size>;
		size_t num_loops = 100*1000;
		results[num_loops] = test_map_list(outter_loops, num_loops, *storage);
		delete storage;
	}

	for (size_t inner = 100; inner < inner_loops; inner += 1000)
	{
		results[inner] = test_map_list(outter_loops, inner, storage);
	}

	cout << "test_map_list" << endl;
	BOOST_FOREACH(Results::value_type const &result, results)
	{
		double mono_time = result.second.first;
		double std_time = result.second.second;
		double perc = mono_time/std_time;
		cout << result.first << '\t' << mono_time << '\t' << std_time << '\t' << perc << "%" << endl;
	}

	//cout << "stack:" << endl;
	//{	
	//	test_map_list(outter_loops, inner_loops, storage);
	//}

	//cout << "heap:" << endl;
	//{
	//	monotonic::storage<1000000> *storage = new monotonic::storage<1000000>;
	//	test_map_list(outter_loops, inner_loops, *storage);
	//	delete storage;
	//}
}

//EOF

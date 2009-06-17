
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
	Result() : mono(0), standard(0), static_monotonic(0) { }
	Result(double a, double b, double c) : mono(a), standard(b), static_monotonic(c) { }
};

Result test_map_list(size_t outter_loops, size_t inner_loops, monotonic::storage_base &storage)
{
	Result result;

	// local monotonic allocator
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

	// standard allocator
	boost::timer t1;
	for (size_t n = 0; n < outter_loops; ++n)
	{
		{
			typedef std::map<int, std::list<int> > Map;
			Map map;
			test_map_list_impl(inner_loops, map);
		}
	}
	double e1 = t1.elapsed();

	// static monotonic allocator
	boost::timer t2;
	monotonic::static_storage.reset();
	for (size_t n = 0; n < outter_loops; ++n)
	{
		{
			typedef std::map<int, std::list<int, monotonic::allocator<int> >, std::less<int>, monotonic::allocator<int> > Map;
			Map map;
			test_map_list_impl(inner_loops, map);
		}
		monotonic::static_storage.reset();
	}
	double e2 = t2.elapsed();

	cout << "test_map_list: " << inner_loops << ": " << e0 << ", " << e1 << ", " << e2 << endl;
	return Result(e0, e1, e2);
}

void test_map_list_heap_stack()
{
	const size_t outter_loops = 10*1000;
	const size_t inner_loops = 10000;

	monotonic::storage<> storage;
	typedef std::map<size_t, Result > Results;
	Results results;

	for (size_t inner = 100; inner < inner_loops; inner += 1000)
	{
		results[inner] = test_map_list(outter_loops, inner, storage);
	}

	cout << "test_map_list" << endl;
	BOOST_FOREACH(Results::value_type const &iter, results)
	{
		Result const &result = iter.second;
		double mono_time = result.mono;
		double std_time = result.standard;
		double static_time = result.static_monotonic;
		double perc1 = mono_time/std_time;
		double perc2 = static_time/std_time;
		cout << iter.first << '\t' << mono_time << '\t' << static_time << '\t' << std_time << '\t' << perc1 << "%\t" << perc2 << "%"<< endl;
	}
}

//EOF

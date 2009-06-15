// part of the test_map_list_realtime test.
// this is to be like running part of a simulation.
void test_map_list_impl_mono(size_t count, monotonic::storage_base &storage)
{
	typedef monotonic::list<int> List;
	typedef monotonic::map<int, List> Map;
	Map map(storage);
	size_t mod = count/10;
	for (size_t n = 0; n < count; ++n)
	{
		int random = rand() % mod;
		Map::iterator iter = map.find(random);
		if (iter == map.end())
		{
			map.insert(make_pair(random, List(storage)));
		}
		else
		{
			iter->second.push_back(n);
		}
	}
}

// same as test_map_list_impl_mono, but using std::containers
void test_map_list_impl_std(size_t count)
{
	typedef std::list<int> List;
	typedef std::map<int, List> Map;
	Map map;
	size_t mod = count/10;
	for (size_t n = 0; n < count; ++n)
	{
		int random = rand() % mod;
		Map::iterator iter = map.find(random);
		if (iter == map.end())
		{
			map.insert(make_pair(random, List()));
		}
		else
		{
			iter->second.push_back(n);
		}
	}
}

// the purpose of this test is to simulate a `realtime system`
// which does an amount of work each frame, then does a similar
// amount of work the next frame.
void test_map_list_realtime()
{
	monotonic::inline_storage<1000000> storage;
	const size_t outter_loops = 10*1000;
	const size_t inner_loops = 10000;

	boost::timer t0;
	for (size_t n = 0; n < outter_loops; ++n)
	{
		test_map_list_impl_mono(inner_loops, storage);
		storage.reset(); ///<<< reset the memory usage to zero
	}
	double e0 = t0.elapsed();
	cout << "test_map_list: mono: " << e0 << endl;

	boost::timer t1;
	for (size_t n = 0; n < outter_loops; ++n)
	{
		test_map_list_impl_std(inner_loops);
	}
	double e1 = t1.elapsed();
	cout << "test_map_list:  std: " << e1 << endl;
}

//EOF

// part of the test_map_list_realtime test.
// this is to be like running part of a simulation.
template <class Map>
void test_map_list_impl(size_t count, Map &map)
{
	typedef typename Map::referent_type List;
	size_t mod = count/10;
	for (size_t n = 0; n < count; ++n)
	{
		int random = rand() % mod;
		Map::iterator iter = map.find(random);
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

// the purpose of this test is to simulate a `realtime system`
// which does an amount of work each frame, then does a similar
// amount of work the next frame.
void test_map_list_realtime()
{
	monotonic::inline_storage<1000000> storage;// = new monotonic::inline_storage<10000000>;
	const size_t outter_loops = 10*1000;
	const size_t inner_loops = 10000;

	boost::timer t0;
	for (size_t n = 0; n < outter_loops; ++n)
	{
		typedef std::map<int, std::list<int, monotonic::allocator<int> >, std::less<int>, monotonic::allocator<int> > Map;
		Map map(std::less<int>(), storage);
		test_map_list_impl(inner_loops, map);
		storage.reset(); ///<<< reset the memory usage to zero
	}
	double e0 = t0.elapsed();
	cout << "test_map_list: mono: " << e0 << endl;

	boost::timer t1;
	for (size_t n = 0; n < outter_loops; ++n)
	{
		typedef std::map<int, std::list<int> > Map;
		Map map;
		test_map_list_impl(inner_loops, map);
	}
	double e1 = t1.elapsed();
	cout << "test_map_list:  std: " << e1 << endl;
	//delete storage;
}

//EOF

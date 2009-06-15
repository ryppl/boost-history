
template <class List>
size_t test_dupe_impl(size_t count, size_t size, List list)
{
	size_t dummy = 0;
	fill_n(back_inserter(list), size, 42);
	for (size_t n = 0; n < count; ++n)
	{
		List dupe = list;
		dummy += dupe.size();
	}
	return dummy;
}

pair<double,double> test_dupe(size_t num_tests, size_t count, size_t size)
{
	monotonic::storage<1000000> storage;

	boost::timer mono_timer;
	for (size_t n = 0; n < num_tests; ++n)
	{
		std::list<int, monotonic::allocator<int> > list(storage);
		test_dupe_impl(count, size, list);
		storage.reset();
	}
	double mono_time = mono_timer.elapsed();

	boost::timer std_timer;
	for (size_t n = 0; n < num_tests; ++n)
	{
		std::list<int> list;
		test_dupe_impl(count, size, list);
	}
	double std_time = std_timer.elapsed();
	return make_pair(mono_time, std_time);
}

void test_dupe()
{
	pair<double,double> results = test_dupe(10000, 100, 100);
	cout << "test_dupe: mono: " << results.first << endl;
	cout << "test_dupe:  std: " << results.second << endl;
}

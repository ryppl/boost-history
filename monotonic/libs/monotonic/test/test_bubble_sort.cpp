
template <class List>
void test_bubble_sort_impl(size_t length, List &list)
{
	for (size_t n = 0; n < length; ++n)
		list.push_back(length - n);
	bool swapped = false;
	do
	{
		swapped = false;
		typename List::iterator A = list.begin(), B = --list.end();
		for (--B; A != B; ++A)
		{
			typename List::iterator C = A;
			++C;
			if (*A > *C)
			{
				std::swap(*A, *C);
				swapped = true;
			}
	
		}
	}
	while (swapped);
}

pair<double,double> test_bubble_sort(size_t count = 50*1000, size_t length = 20)
{
	monotonic::storage<100000> storage;// = new monotonic::storage<100000>();
	boost::timer mono_timer;
	for (size_t n = 0; n < count; ++n)
	{
		std::list<int, monotonic::allocator<int> > list(storage);
		test_bubble_sort_impl(length, list);
		storage.reset();
	}
	double mono_total = mono_timer.elapsed();

	boost::timer std_timer;
	for (size_t n = 0; n < count; ++n)
	{
		std::list<int> list;
		test_bubble_sort_impl(length, list);
	}
	double std_total = std_timer.elapsed();
	return make_pair(mono_total, std_total);
}

void graph_bubble_sort()
{
	const size_t count = 50000;
	typedef std::map<size_t, pair<double, double> > Results;
	Results results;
	for (size_t length = 3; length < 150; length += 10)
	{
		results[length] = test_bubble_sort(count, length);
	}
	stringstream chart;
	chart << "http://chart.apis.google.com/chart?chco=FF0000,00FF00&chs=250x100&cht=lc&chd=t:";
	stringstream first;
	stringstream second;
	string comma = "";
	double m = 0;
	BOOST_FOREACH(Results::value_type const &result, results)
	{
		double mono_time = result.second.first;
		double std_time = result.second.second;
		double perc = mono_time/std_time;
		cout << result.first << '\t' << mono_time << '\t' << std_time << '\t' << perc << "%" << endl;
		first << comma << mono_time;
		second << comma << std_time;
		comma = ",";
		m = max(m, max(std_time, mono_time));
	}
	chart << first.str() << "|" << second.str() << "&chds=0," << m << ",0," << m;
	cout << chart.str() << endl;
}

//EOF

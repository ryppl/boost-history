// Copyright 2008 Federico J. Fernandez.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)


#include <boost/spatial_index.hpp>

#include <boost/test/included/test_exec_monitor.hpp>
#include <boost/shared_ptr.hpp>

#include <vector>
#include <string>


#include <sys/time.h>


// query rectangle
double rect_x1, rect_x2, rect_y1, rect_y2, rect_count;


std::vector<std::pair<double,double> > read_data(void)
{
	std::set<std::pair<double,double> > v;
	std::vector<std::pair<double,double> > r;

	std::ifstream data;
	data.open("gis-data.txt");

	// variable to check number of points inside the control rectangle
	rect_count = 0;

	rect_x1 = -120.0;
	rect_x2 = -80.0;
	rect_y1 = 10.0;
	rect_y2 = 40.0;

	double x, y;
	data >> x;
	data >> y;
	if(x >= rect_x1 && x <= rect_x2 && y >= rect_y1 && y <= rect_y2) {
		rect_count++;
	}
	v.insert(std::make_pair(x,y));

	while(!data.eof()) {

		data >> x;
		data >> y;

		if(x >= rect_x1 && x <= rect_x2 && y >= rect_y1 && y <= rect_y2) {
//  			std::cerr << "Rect: (" << x << "," << y << ")" << std::endl;
			rect_count++;
		}

		v.insert(std::make_pair(x,y));
	}
	copy(v.begin(), v.end(), std::back_inserter(r));
	return r;
}


double drandom(unsigned int upper_bound)
{
	double r;		/* random value in range [0,1) */
	long int M;	/* user supplied upper boundary */

	struct timeval tv;
	struct timezone tz;
	gettimeofday(&tv, &tz);
	srand(tv.tv_usec);

	M = upper_bound;		/* Choose M. Upper bound */
	r = (   (double)rand() / ((double)(RAND_MAX)+(double)(1)) );

	return r * M;
}


int test_main(int, char* [])
{
	std::vector<unsigned int> ids;
 	std::vector<std::pair<double, double> > points = read_data();


	// start time
	time_t start;

// 	std::cerr << "Size: " << points.size() << std::endl;

	// plane
 	const double min_x = -130.0;
 	const double min_y = 10.0;
 	const double max_x = -60.0;
 	const double max_y = 80.0;

	// number of points to find on the search phase
	const unsigned int find_count = 100000;

 	for(unsigned int i = 0; i < points.size(); i++) {
 		ids.push_back(i);
	}

  	boost::shared_ptr< boost::spatial_index::spatial_index< std::pair<double, double> , std::vector<unsigned int>::iterator > > 
  		q(new boost::spatial_index::quadtree< std::pair<double, double> , 
		  std::vector<unsigned int>::iterator >(min_x, min_y, max_x, max_y));

 	// load data
   	std::cerr << " --> bulk insert" << std::endl;
   	std::vector<unsigned int>::iterator b, e;
   	b = ids.begin();
   	e = ids.end();

	start = time(NULL);
   	q->bulk_insert(b,e, points);
	std::cerr << "Insertion time: " << time(NULL) - start << " seconds." << std::endl;

	// search
	std::vector<std::pair<double,double> > search_positions;
	std::vector<unsigned int> search_data;
	for(unsigned int j=0; j < find_count; j++) {
		unsigned int pos = (int) drandom(points.size());

		search_positions.push_back(points[pos]);
		search_data.push_back(pos);
	}


	start = time(NULL);
	std::deque< std::vector<unsigned int>::iterator > d = q->find(rect_x1, rect_x2, rect_y1, rect_y2);
	std::cerr << " --> find rectangle" << std::endl;
	BOOST_CHECK_EQUAL(rect_count, d.size());
	std::cerr << "Retrieve (rectangle with " << d.size() << " elements) time: " << time(NULL) - start << " seconds." << std::endl;


	start = time(NULL);
	for(unsigned int j=0; j < find_count; j++) {
		std::vector<unsigned int>::iterator it = q->find(search_positions[j]);
// 		std::cout << search_data[j] 
// 			  << " - found in (" << search_positions[j].first << "," << search_positions[j].second << ") --> " 
// 			  << *it << std::endl;

		BOOST_CHECK_EQUAL(*it, search_data[j]);
	}
	std::cerr << "Retrieve time: " << time(NULL) - start << " seconds." << std::endl;



	return 0;
}




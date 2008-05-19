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
	// data and indexed points
	std::vector<unsigned int> ids;
 	std::vector<std::pair<double, double> > points;	

	// plane
 	const double max_x = 2000.0;
 	const double max_y = 2000.0;

	// number of points
	const unsigned int points_count = 500;

	// number of points to find on the search phase
	const unsigned int find_count = 50;

 	boost::shared_ptr< boost::spatial_index::spatial_index< std::pair<double, double> , std::vector<unsigned int>::iterator > > 
 		q(new boost::spatial_index::quadtree< std::pair<double, double> , 
		  std::vector<unsigned int>::iterator >(0.0, 0.0, max_x, max_y));

	// generate random data
  	for(unsigned int i = 0; i < points_count; i++) {
 		double x = drandom((int) max_x);
 		double y = drandom((int) max_y);

  		ids.push_back(i);
 		points.push_back(std::make_pair(x, y));

//  		std::cerr << "insert " << i << " -> (" << x << ", " << y << ")" << std::endl;
  	}

	// insert data
  	std::cerr << " --> bulk insert" << std::endl;
  	std::vector<unsigned int>::iterator b, e;
  	b = ids.begin();
  	e = ids.end();
  	q->bulk_insert(b,e, points);

	// search
	std::vector<std::pair<double,double> > search_positions;
	std::vector<unsigned int> search_data;

	// compute random positions to do the searches, store the data
	for(unsigned int j=0; j < find_count; j++) {
		unsigned int pos = (int) drandom(points_count);
		search_positions.push_back(points[pos]);
		search_data.push_back(pos);
	}

	// search data and compare
	for(unsigned int j=0; j < find_count; j++) {
		std::vector<unsigned int>::iterator it = q->find(search_positions[j]);
		std::cout << search_data[j] 
			  << " - found in (" << search_positions[j].first << "," << search_positions[j].second 
			  << ") --> " << *it << std::endl;

		// check if the retrieved data is equal to the stored data
		BOOST_CHECK_EQUAL(*it, search_data[j]);
	}

	return 0;
}

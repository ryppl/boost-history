// Copyright 2008 Federico J. Fernandez.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
//   GEOS Quadtree implementation (to compare with boost::spatial_index implementation.
//


#include <boost/test/included/test_exec_monitor.hpp>

#include <string>
#include <sys/time.h>
#include <set>
#include <geos/indexQuadtree.h>
#include <fstream>


std::vector<std::pair<double,double> > read_data(void)
{
	std::set<std::pair<double,double> > v;
	std::vector<std::pair<double,double> > r;

	std::ifstream data;
	data.open("gis-data.txt");

	double x, y;
	data >> x;
	data >> y;
	while(!data.eof()) {
		data >> x;
		data >> y;
		v.insert(std::make_pair(x*1000000,y*1000000));
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

	time_t start;

	// number of points to find on the search phase
	const unsigned int find_count = 100000;

	geos::Quadtree q;

	// load data
	start = time(NULL);
  	for(unsigned int i = 0; i < points.size(); i++) {
		int *p = new int(i);
// 		std::cout << "Insert: " << *p << std::endl;
// 		std::cerr << "Inserting: " << points[i].first << "," << points[i].second << std::endl;
 		q.insert(new geos::Envelope(points[i].first, points[i].first, points[i].second, points[i].second), (void *) p);
 	}
	std::cerr << "Insertion time: " << time(NULL) - start << " seconds." << std::endl;

	// search
 	std::vector<std::pair<double,double> > search_positions;
 	std::vector<unsigned int> search_data;
 	for(unsigned int j=0; j < find_count; j++) {
 		unsigned int pos = (int) drandom(points.size());

 		search_positions.push_back(points[pos]);
 		search_data.push_back(pos);
 	}
// 	int a = 6;
// 	q.insert(new geos::Envelope(90.0, 91.0, 30.0, 31.0), (void *) &a);

	start = time(NULL);
	std::vector<void *> *v = q.query(new geos::Envelope(80000000, 120000000, 0, 40000000));
	std::cerr << "Rectangle (" << (*v).size() << " elements) Retrieve time: " << time(NULL) - start << " seconds." << std::endl;

	start = time(NULL);
 	for(unsigned int j=0; j < find_count; j++) {
 		std::vector<void *> *v = q.query(new geos::Envelope(search_positions[j].first, search_positions[j].first, 
								    search_positions[j].second, 
								    search_positions[j].second));

//  		std::cerr << "Size: " << v->size() << std::endl;

 		void *vp = (*v)[0];
 		int *value = (int *) vp;

//    		std::cout << search_data[j] 
//    			  << " - found in (" << search_positions[j].first << "," << search_positions[j].second << ") --> " 
//    			  << *value << std::endl;

 		BOOST_CHECK_EQUAL((unsigned int) *value, search_data[j]);
 	}
	std::cerr << "Retrieve time: " << time(NULL) - start << " seconds." << std::endl;

	return 0;
}

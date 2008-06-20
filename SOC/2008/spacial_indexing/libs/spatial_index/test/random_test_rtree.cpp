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

#define MAX_X 2000.0
#define MAX_Y 2000.0


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
  std::vector<geometry::point_xy<double> > points;	

  // plane
  geometry::box<geometry::point_xy<double> > plane(geometry::point_xy<double>(0.0, 0.0), geometry::point_xy<double>(MAX_X, MAX_Y));

  // number of points
  const unsigned int points_count = 500;

  // number of points to find on the search phase
  const unsigned int find_count = 50;
  
  boost::shared_ptr< boost::spatial_index::spatial_index< geometry::point_xy<double> , unsigned int > > 
    q(new boost::spatial_index::rtree<geometry::point_xy<double>, unsigned int>(plane, 8, 4));

  // generate random data
  for(unsigned int i = 0; i < points_count; i++) {
    double x = drandom((int) MAX_X);
    double y = drandom((int) MAX_Y);
    
    ids.push_back(i);
    points.push_back(geometry::point_xy<double>(x, y));

    // std::cerr << "insert " << i << " -> (" << x << ", " << y << ")" << std::endl;
  }

  // insert data
  std::cerr << " --> bulk insert" << std::endl;
  // std::vector<unsigned int>::iterator b, e;
  // b = ids.begin();
  // e = ids.end();
  q->bulk_insert(ids, points);

      // search
      std::vector<geometry::point_xy<double> > search_positions;
      std::vector<unsigned int> search_data;

      // compute random positions to do the searches, store the data
      for(unsigned int j=0; j < find_count; j++) {
	unsigned int pos = (int) drandom(points_count);
	search_positions.push_back(points[pos]);
	search_data.push_back(pos);
      }

      // search data and compare
      for(unsigned int j=0; j < find_count; j++) {
	unsigned int i = q->find(search_positions[j]);
	std::cout << search_data[j] 
		  << " - found in (" << geometry::get<0>(search_positions[j]) << "," << geometry::get<1>(search_positions[j])
		  << ") --> " << i << std::endl;
	
	// check if the retrieved data is equal to the stored data
	BOOST_CHECK_EQUAL(i, search_data[j]);
      }

      return 0;
}

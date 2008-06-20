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
double rect_count;
geometry::box<geometry::point_xy<double> > query_box;


std::vector< geometry::point_xy<double> > read_data(void)
{
  std::set< geometry::point_xy<double> > v;
  std::vector< geometry::point_xy<double> > r;

  std::ifstream data;
  data.open("gis-data.txt");

  // variable to check number of points inside the control rectangle
  rect_count = 0;

  geometry::get<0>(query_box.min()) = -120.0;
  geometry::get<1>(query_box.min()) = 10.0;

  geometry::get<0>(query_box.max()) = -80.0;
  geometry::get<1>(query_box.max()) = 40.0;

  double x, y;
  data >> x;
  data >> y;
  if(geometry::within(geometry::point_xy<double>(x,y), query_box)) {
    rect_count++;
  }
  v.insert(geometry::point_xy<double>(x,y));

  while(!data.eof()) {
    data >> x;
    data >> y;

  if(geometry::within(geometry::point_xy<double>(x,y), query_box)) {
      //  std::cerr << "Rect: (" << x << "," << y << ")" << std::endl;
      rect_count++;
    }
    v.insert(geometry::point_xy<double>(x,y));
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
  std::vector< geometry::point_xy<double> > points = read_data();

  // -- wait to check memory
  //   std::cerr << "check memory --> After Reading Data." << std::endl;
  //   sleep(5);
  // -- wait to check memory

  // start time
  time_t start;

  // std::cerr << "Size: " << points.size() << std::endl;

  // plane
  geometry::box<geometry::point_xy<double> > plane(geometry::point_xy<double>(-130.0, 0.0), geometry::point_xy<double>(-60.0, 80.0));

  // number of points to find on the search phase
  const unsigned int find_count = 100000;

  for(unsigned int i = 0; i < points.size(); i++) {
    ids.push_back(i);
  }

  boost::shared_ptr< boost::spatial_index::spatial_index< geometry::point_xy<double> , unsigned int > > 
    q(new boost::spatial_index::rtree< geometry::point_xy<double>, unsigned int >(plane, 16, 8));

      // load data
      std::cerr << " --> bulk insert" << std::endl;
      std::vector<unsigned int>::iterator b, e;
//       b = ids.begin();
//       e = ids.end();

      start = time(NULL);
      q->bulk_insert(ids, points);
      std::cerr << "Insertion time: " << time(NULL) - start << " seconds." << std::endl;

      // -- wait to check memory
      //       std::cerr << "check memory --> After Building Index." << std::endl;
      //       sleep(5);
      // -- wait to check memory

      // search
      std::vector< geometry::point_xy<double> > search_positions;
      std::vector<unsigned int> search_data;
      for(unsigned int j=0; j < find_count; j++) {
	unsigned int pos = (int) drandom(points.size());

	search_positions.push_back(points[pos]);
	search_data.push_back(pos);
      }


      start = time(NULL);
      std::deque<unsigned int> d = q->find(query_box);
      std::cerr << " --> find rectangle" << std::endl;
      BOOST_CHECK_EQUAL(rect_count, d.size());
      std::cerr << "Retrieve (rectangle with " << d.size() << " elements) time: " << time(NULL) - start << " seconds." << std::endl;

      start = time(NULL);
      for(unsigned int j=0; j < find_count; j++) {
 	unsigned int i = q->find(search_positions[j]);
// 	std::vector<unsigned int>::iterator it = q->find(search_positions[j]);
	//std::cout << search_data[j] 
	//  << " - found in (" << search_positions[j].first << "," << search_positions[j].second << ") --> " 
	//  << *it << std::endl;

	BOOST_CHECK_EQUAL(i, search_data[j]);
      }
      std::cerr << "Retrieve time: " << time(NULL) - start << " seconds." << std::endl;


      std::cerr << " --> removal tests" << std::endl;
      for(unsigned int j=0; j < find_count/1000; j++) {
 	q->remove(geometry::box<geometry::point_xy<double> >(search_positions[j], search_positions[j]));
// 	std::cerr << "Elements: " << q->elements() << std::endl;
      }      
      std::cerr << std::endl;

      std::cerr << " --> requery test" << std::endl;
      start = time(NULL);
      for(unsigned int j=0; j < find_count/1000; j++) {
 	unsigned int i = q->find(search_positions[j]);
// 	std::cerr << "Prev. Value: " << search_data[j] << std::endl;
	BOOST_CHECK_EQUAL(i, 0u);
      }
      std::cerr << "Retrieve time: " << time(NULL) - start << " seconds." << std::endl;

      return 0;
}

// Copyright 2008 Federico J. Fernandez.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)


#include <boost/spatial_index.hpp>

#include <boost/test/included/test_exec_monitor.hpp>
#include <boost/shared_ptr.hpp>

#include <geometry/geometry.hpp>

#include <vector>
#include <string>

int test_main(int, char* [])
{
	geometry::box<geometry::point_xy<double> > b(geometry::point_xy<double>(0.0, 0.0), geometry::point_xy<double>(20.0, 20.0));

	{
	  boost::shared_ptr< boost::spatial_index::spatial_index< geometry::point_xy<double> , unsigned int > > 
	    q(new boost::spatial_index::rtree< geometry::point_xy<double> , unsigned int >(b, 4, 2));

	  std::cerr << std::endl;
	  std::cerr << " --> bulk insert" << std::endl;
	  std::cerr << std::endl;

	  std::vector<unsigned int> data;
	  std::vector< geometry::point_xy<double> > points;	

	  points.push_back(geometry::point_xy<double>(2.0,2.0));
	  points.push_back(geometry::point_xy<double>(1.0,3.0));
	  points.push_back(geometry::point_xy<double>(1.5,5.0));
	  points.push_back(geometry::point_xy<double>(5.5,5.0));
	  points.push_back(geometry::point_xy<double>(2.0,5.0));

	  data.push_back(1);
	  data.push_back(2);
	  data.push_back(3);
	  data.push_back(4);
	  data.push_back(5);

	  q->bulk_insert(data, points);

// 	  q->print();
	}

	  std::cerr << std::endl;
	  std::cerr << " --> end bulk insert" << std::endl;
	  std::cerr << std::endl;


	/// insertion tests
 
	boost::shared_ptr< boost::spatial_index::spatial_index< geometry::point_xy<double> , unsigned int > > 
	  q(new boost::spatial_index::rtree< geometry::point_xy<double> , unsigned int >(b, 4, 2));

	geometry::box<geometry::point_xy<double> > e1(geometry::point_xy<double>(2.0, 2.0), geometry::point_xy<double>(4.0, 4.0));
	geometry::box<geometry::point_xy<double> > e2(geometry::point_xy<double>(0.0, 1.0), geometry::point_xy<double>(2.0, 2.0));
	geometry::box<geometry::point_xy<double> > e3(geometry::point_xy<double>(5.0, 4.0), geometry::point_xy<double>(6.0, 6.0));
	geometry::box<geometry::point_xy<double> > e4(geometry::point_xy<double>(5.0, 5.0), geometry::point_xy<double>(8.0, 8.0));
	geometry::box<geometry::point_xy<double> > e5(geometry::point_xy<double>(7.0, 4.0), geometry::point_xy<double>(12.0, 7.0));


	geometry::box<geometry::point_xy<double> > e6(geometry::point_xy<double>(5.0, 5.0), geometry::point_xy<double>(9.0, 10.0));
	geometry::box<geometry::point_xy<double> > e7(geometry::point_xy<double>(7.0, 7.0), geometry::point_xy<double>(12.0, 12.0));

	geometry::box<geometry::point_xy<double> > e8(geometry::point_xy<double>(1.0, 5.0), geometry::point_xy<double>(2.0, 10.0));
	geometry::box<geometry::point_xy<double> > e9(geometry::point_xy<double>(1.0, 1.0), geometry::point_xy<double>(3.0, 3.0));
	geometry::box<geometry::point_xy<double> > e10(geometry::point_xy<double>(1.5, 1.0), geometry::point_xy<double>(2.5, 3.0));
	geometry::box<geometry::point_xy<double> > e11(geometry::point_xy<double>(1.0, 0.0), geometry::point_xy<double>(2.0, 3.0));

	geometry::box<geometry::point_xy<double> > e12(geometry::point_xy<double>(10.0, 10.0), geometry::point_xy<double>(12.0, 13.0));
	geometry::box<geometry::point_xy<double> > e13(geometry::point_xy<double>(10.0, 12.0), geometry::point_xy<double>(13.0, 13.0));
	geometry::box<geometry::point_xy<double> > e14(geometry::point_xy<double>(10.0, 13.0), geometry::point_xy<double>(10.0, 13.0));
	geometry::box<geometry::point_xy<double> > e15(geometry::point_xy<double>(10.0, 13.0), geometry::point_xy<double>(12.0, 14.0));


 	std::cerr << " --> insert env" << std::endl;
 	q->insert(e1, 1);
 	q->insert(e2, 2);
 	q->insert(e3, 3);
 	q->insert(e4, 4);
 	q->insert(e5, 5);
//  	q->print();

 	q->insert(e6, 6);
 	q->insert(e7, 7);
//  	q->print();

 	q->insert(e8, 8);
 	q->insert(e9, 9);
 	q->insert(e10, 10);
 	q->insert(e11, 11);
//   	q->print();

 	q->insert(e12, 12);
 	q->insert(e13, 13);
    	q->print();
 	q->insert(e14, 14);
    	q->print();
   	q->insert(e15, 15);
    	q->print();

	/// find everything overlaping with an envelope
	std::cerr << " --> find in envelope" << std::endl;

	/// expected result
	std::deque<unsigned int> result;
	result.push_back(2);
	result.push_back(1);
	result.push_back(9);
	result.push_back(10);
	result.push_back(11);
	geometry::box<geometry::point_xy<double> > query_box(geometry::point_xy<double>(0.0, 0.0), geometry::point_xy<double>(2.0, 2.0));
	std::deque< unsigned int > d = q->find(query_box);
 	BOOST_CHECK_EQUAL(d.size(), 5u);
	unsigned int i = 0;
	for(std::deque<unsigned int>::const_iterator dit = d.begin(); dit != d.end(); ++dit) {
		std::cerr << "Value: " << *dit << std::endl;
 		BOOST_CHECK_EQUAL(*dit, result[i++]);
	}

	/// find individual points (not found)
	{
	  std::cerr << " --> find" << std::endl;
	  unsigned int v = q->find(geometry::point_xy<double>(0.0,0.0));
	  BOOST_CHECK_EQUAL(v, 0u);
	  std::cout << "Value:  " << v << std::endl;
	}

	/// find individual points (found)
	{
	  std::cerr << " --> find" << std::endl;
	  unsigned int v = q->find(geometry::point_xy<double>(10.0,13.0));
	  BOOST_CHECK_EQUAL(v, 14u);
	  std::cout << "Value:  " << v << std::endl;
	}

	// remove test
// 	std::cerr << " --> remove" << std::endl;
// 	q->remove(geometry::box<geometry::point_xy<double> >(geometry::point_xy<double>(10.0,10.0), geometry::point_xy<double>(12.0,13.0)));

//  	q->print();

	return 0;
};

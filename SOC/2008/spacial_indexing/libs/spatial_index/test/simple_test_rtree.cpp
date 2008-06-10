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
	std::vector<std::string> data;
	std::vector< geometry::point_xy<double> > points;	

	data.push_back("test0");
	data.push_back("test1");
	data.push_back("test2");
	data.push_back("test3");
	data.push_back("test4");
	data.push_back("test5");

	points.push_back(geometry::point_xy<double>(1.0,1.0));
	points.push_back(geometry::point_xy<double>(2.0,1.0));
	points.push_back(geometry::point_xy<double>(5.0,5.0));
	points.push_back(geometry::point_xy<double>(1.0,6.0));
	points.push_back(geometry::point_xy<double>(9.0,9.0));
	points.push_back(geometry::point_xy<double>(9.0,8.0));

	geometry::box<geometry::point_xy<double> > b(geometry::point_xy<double>(0.0, 0.0), geometry::point_xy<double>(20.0, 20.0));
 
	boost::shared_ptr< boost::spatial_index::spatial_index< geometry::point_xy<double> , std::vector<std::string>::iterator > > 
	  q(new boost::spatial_index::rtree< geometry::point_xy<double> , std::vector<std::string>::iterator >(b, 8, 4, 8));

// 	std::cerr << " --> bulk insert" << std::endl;
// 	std::vector<std::string>::iterator b, e;
// 	b = data.begin();
// 	e = data.end();
// 	q->bulk_insert(b,e, points);

 	std::vector<std::string>::iterator it = data.begin();


	geometry::box<geometry::point_xy<double> > e(geometry::point_xy<double>(2.0, 2.0), geometry::point_xy<double>(4.0, 4.0));
 	std::cerr << " --> insert env" << std::endl;
 	q->insert(e, it++);

//  	std::cerr << " --> insert" << std::endl;
//  	q->insert(geometry::point_xy<double>(1.0,1.0), it++);
//  	std::cerr << " --> insert" << std::endl;
//  	q->insert(geometry::point_xy<double>(2.0,1.0), it++);
//  	std::cerr << " --> insert" << std::endl;
//  	q->insert(geometry::point_xy<double>(5.0,5.0), it++);
//  	std::cerr << " --> insert" << std::endl;
//  	q->insert(geometry::point_xy<double>(1.0,6.0), it++);
//  	std::cerr << " --> insert" << std::endl;
//  	q->insert(geometry::point_xy<double>(9.0,9.0), it++);
//  	std::cerr << " --> insert" << std::endl;
//  	q->insert(geometry::point_xy<double>(9.0,8.0), it++);


// 	std::vector<std::string>::iterator it1;

// 	std::cerr << " --> find" << std::endl;
// 	it1 = q->find(geometry::point_xy<double>(9.0,9.0));
// 	BOOST_CHECK_EQUAL(*it1, "test4");
// 	std::cout << "  " << *it1 << std::endl;

// 	std::cerr << " --> find" << std::endl;
// 	it1 = q->find(geometry::point_xy<double>(5.0,5.0));
// 	BOOST_CHECK_EQUAL(*it1, "test2");
// 	std::cout << "  " << *it1 << std::endl;

// 	// expected result
// 	std::vector<std::string> res;
// 	res.push_back("test0");
// 	res.push_back("test1");
// 	res.push_back("test2");

// 	std::cerr << "Elements: " << q->elements() << std::endl;
// 	BOOST_CHECK_EQUAL(q->elements(), 6u);

// 	std::cerr << " --> find rectangle" << std::endl;
// 	geometry::box<geometry::point_xy<double> > query_box(geometry::point_xy<double>(0.0, 0.0), geometry::point_xy<double>(5.0, 5.0));
// 	std::deque< std::vector<std::string>::iterator > d = q->find(query_box);
// 	BOOST_CHECK_EQUAL(d.size(), 3u);
// 	unsigned int i = 0;
// 	for(std::deque< std::vector<std::string>::iterator >::const_iterator dit = d.begin(); dit != d.end(); ++dit) {
// 		std::cerr << "Value: " << *(*dit) << std::endl;
// 		BOOST_CHECK_EQUAL(*(*dit), res[i++]);
// 	}


	return 0;
};

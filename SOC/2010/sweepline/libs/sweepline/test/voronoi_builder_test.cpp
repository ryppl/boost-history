// Boost sweepline library voronoi_builder_test.cpp file 

//          Copyright Andrii Sydorchuk 2010.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org for updates, documentation, and revision history.

#include <stdlib.h>
#include <time.h>

#include "test_type_list.hpp"
#include "boost/sweepline/voronoi_builder.hpp"
using namespace boost::sweepline;

#define BOOST_TEST_MODULE voronoi_builder_test
#include <boost/test/test_case_template.hpp>

// Sites: (0, 0), (0, 4), (2, 1).
BOOST_AUTO_TEST_CASE_TEMPLATE(voronoi_builder_test1, T, test_types) {
    typedef typename voronoi_output< point_2d<T> >::edge_type edge_type;
    typedef typename voronoi_output< point_2d<T> >::edge_pointer_const_iterator_type 
        edge_pointer_iterator_type;
    typedef typename voronoi_output< point_2d<T> >::voronoi_vertices_const_iterator_type
        voronoi_vertices_iterator_type;

    voronoi_builder<T> test_beach_line;
    point_2d<T> point1 = make_point_2d<T>(0, 0);
    point_2d<T> point2 = make_point_2d<T>(0, 4);
    point_2d<T> point3 = make_point_2d<T>(2, 1);

    std::vector< point_2d<T> > points;
    points.push_back(point1);
    points.push_back(point2);
    points.push_back(point3);
    
    test_beach_line.init(points);
    test_beach_line.run_sweepline();
    const voronoi_output< point_2d<T> > &test_output = test_beach_line.get_output();

    BOOST_CHECK_EQUAL(static_cast<int>(test_output.get_cell_records().size()), 3);
    BOOST_CHECK_EQUAL(static_cast<int>(test_output.get_voronoi_vertices().size()), 1);

    voronoi_vertices_iterator_type it = test_output.get_voronoi_vertices().begin();
    BOOST_CHECK_EQUAL(static_cast<int>(it->incident_edges.size()), 3);
    BOOST_CHECK_EQUAL(it->vertex.x() == static_cast<T>(0.25) &&
                      it->vertex.y() == static_cast<T>(2.0), true);

    edge_pointer_iterator_type edge_it = it->incident_edges.begin();
    edge_type *edge1_1 = *edge_it;
    edge_type *edge1_2 = edge1_1->twin;
    BOOST_CHECK_EQUAL(edge1_1->cell->site_point == point3, true);
    BOOST_CHECK_EQUAL(edge1_2->cell->site_point == point1, true);

    edge_it++;
    edge_type *edge2_1 = *edge_it;
    edge_type *edge2_2 = edge2_1->twin;
    BOOST_CHECK_EQUAL(edge2_1->cell->site_point == point1, true);
    BOOST_CHECK_EQUAL(edge2_2->cell->site_point == point2, true);

    edge_it++;
    edge_type *edge3_1 = *edge_it;
    edge_type *edge3_2 = edge3_1->twin;
    BOOST_CHECK_EQUAL(edge3_1->cell->site_point == point2, true);
    BOOST_CHECK_EQUAL(edge3_2->cell->site_point == point3, true);

    BOOST_CHECK_EQUAL(edge1_2->twin == edge1_1, true);
    BOOST_CHECK_EQUAL(edge2_2->twin == edge2_1, true);
    BOOST_CHECK_EQUAL(edge3_2->twin == edge3_1, true);

    BOOST_CHECK_EQUAL(edge1_1->next == NULL && edge1_2->prev == NULL, true);
    BOOST_CHECK_EQUAL(edge2_1->next == NULL && edge2_2->prev == NULL, true);
    BOOST_CHECK_EQUAL(edge3_1->next == NULL && edge3_2->prev == NULL, true);

    BOOST_CHECK_EQUAL(edge1_1->prev == edge3_2, true);
    BOOST_CHECK_EQUAL(edge2_1->prev == edge1_2, true);
    BOOST_CHECK_EQUAL(edge3_1->prev == edge2_2, true);

    BOOST_CHECK_EQUAL(edge1_2->next == edge2_1, true);
    BOOST_CHECK_EQUAL(edge2_2->next == edge3_1, true);
    BOOST_CHECK_EQUAL(edge3_2->next == edge1_1, true);

}

// Sites: (0, 1), (2, 0), (2, 4).
BOOST_AUTO_TEST_CASE_TEMPLATE(voronoi_builder_test2, T, test_types) {
    typedef typename voronoi_output< point_2d<T> >::edge_type edge_type;
    typedef typename voronoi_output< point_2d<T> >::edge_pointer_const_iterator_type 
        edge_pointer_iterator_type;
    typedef typename voronoi_output< point_2d<T> >::voronoi_vertices_const_iterator_type
        voronoi_vertices_iterator_type;

    voronoi_builder<T> test_beach_line;
    point_2d<T> point1 = make_point_2d<T>(0, 1);
    point_2d<T> point2 = make_point_2d<T>(2, 0);
    point_2d<T> point3 = make_point_2d<T>(2, 4);

    std::vector< point_2d<T> > points;
    points.push_back(point1);
    points.push_back(point2);
    points.push_back(point3);
    
    test_beach_line.init(points);
    test_beach_line.run_sweepline();
    const voronoi_output< point_2d<T> > &test_output = test_beach_line.get_output();

    BOOST_CHECK_EQUAL(static_cast<int>(test_output.get_cell_records().size()), 3);
    BOOST_CHECK_EQUAL(static_cast<int>(test_output.get_voronoi_vertices().size()), 1);

    voronoi_vertices_iterator_type it = test_output.get_voronoi_vertices().begin();
    BOOST_CHECK_EQUAL(static_cast<int>(it->incident_edges.size()), 3);
    BOOST_CHECK_EQUAL(it->vertex.x() == static_cast<T>(1.75) &&
                      it->vertex.y() == static_cast<T>(2.0), true);

    edge_pointer_iterator_type edge_it = it->incident_edges.begin();
    edge_type *edge1_1 = *edge_it;
    edge_type *edge1_2 = edge1_1->twin;
    BOOST_CHECK_EQUAL(edge1_1->cell->site_point == point2, true);
    BOOST_CHECK_EQUAL(edge1_2->cell->site_point == point1, true);

    edge_it++;
    edge_type *edge2_1 = *edge_it;
    edge_type *edge2_2 = edge2_1->twin;    
    BOOST_CHECK_EQUAL(edge2_1->cell->site_point == point1, true);
    BOOST_CHECK_EQUAL(edge2_2->cell->site_point == point3, true);

    edge_it++;
    edge_type *edge3_1 = *edge_it;
    edge_type *edge3_2 = edge3_1->twin;
    BOOST_CHECK_EQUAL(edge3_1->cell->site_point == point3, true);
    BOOST_CHECK_EQUAL(edge3_2->cell->site_point == point2, true);

    BOOST_CHECK_EQUAL(edge1_2->twin == edge1_1, true);
    BOOST_CHECK_EQUAL(edge2_2->twin == edge2_1, true);
    BOOST_CHECK_EQUAL(edge3_2->twin == edge3_1, true);

    BOOST_CHECK_EQUAL(edge1_1->next == NULL && edge1_2->prev == NULL, true);
    BOOST_CHECK_EQUAL(edge2_1->next == NULL && edge2_2->prev == NULL, true);
    BOOST_CHECK_EQUAL(edge3_1->next == NULL && edge3_2->prev == NULL, true);

    BOOST_CHECK_EQUAL(edge1_1->prev == edge3_2, true);
    BOOST_CHECK_EQUAL(edge2_1->prev == edge1_2, true);
    BOOST_CHECK_EQUAL(edge3_1->prev == edge2_2, true);

    BOOST_CHECK_EQUAL(edge1_2->next == edge2_1, true);
    BOOST_CHECK_EQUAL(edge2_2->next == edge3_1, true);
    BOOST_CHECK_EQUAL(edge3_2->next == edge1_1, true);
}

// Sites: (0, 0), (0, 1), (1, 0), (1, 1).
BOOST_AUTO_TEST_CASE_TEMPLATE(voronoi_builder_test3, T, test_types) {
    typedef typename voronoi_output< point_2d<T> >::edge_type edge_type;
    typedef typename voronoi_output< point_2d<T> >::edge_pointer_const_iterator_type 
        edge_pointer_iterator_type;
    typedef typename voronoi_output< point_2d<T> >::voronoi_vertices_const_iterator_type
        voronoi_vertices_iterator_type;

    voronoi_builder<T> test_beach_line;
    std::vector< point_2d<T> > points;
    points.push_back(make_point_2d<T>(0, 0));
    points.push_back(make_point_2d<T>(0, 1));
    points.push_back(make_point_2d<T>(1, 0));
    points.push_back(make_point_2d<T>(1, 1));
    
    test_beach_line.init(points);
    test_beach_line.run_sweepline();
    const voronoi_output< point_2d<T> > &test_output = test_beach_line.get_output();

    BOOST_CHECK_EQUAL(static_cast<T>(test_output.get_cell_records().size()), 4);
    BOOST_CHECK_EQUAL(static_cast<T>(test_output.get_voronoi_vertices().size()), 1);
    BOOST_CHECK_EQUAL(test_output.get_num_voronoi_cells(), 4);
    BOOST_CHECK_EQUAL(test_output.get_num_voronoi_vertices(), 1);
    BOOST_CHECK_EQUAL(test_output.get_num_voronoi_edges(), 4);

    // Check voronoi vertex.
    voronoi_vertices_iterator_type it = test_output.get_voronoi_vertices().begin();
    BOOST_CHECK_EQUAL(static_cast<int>(it->incident_edges.size()), 4);
    BOOST_CHECK_EQUAL(static_cast<int>(it->incident_edges.size()), it->num_incident_edges);
    BOOST_CHECK_EQUAL(it->vertex.x() == static_cast<T>(0.5) &&
                      it->vertex.y() == static_cast<T>(0.5), true);
    BOOST_CHECK_EQUAL(it == it->vertex_it, true);

    // Check voronoi edges.
    edge_pointer_iterator_type edge_it = it->incident_edges.begin();
    edge_type *edge1_1 = *edge_it;
    edge_type *edge1_2 = edge1_1->twin;
    BOOST_CHECK_EQUAL(edge1_1->cell->site_point == points[2], true);
    BOOST_CHECK_EQUAL(edge1_2->cell->site_point == points[0], true);

    edge_it++;
    edge_type *edge2_1 = *edge_it;
    edge_type *edge2_2 = edge2_1->twin;    
    BOOST_CHECK_EQUAL(edge2_1->cell->site_point == points[0], true);
    BOOST_CHECK_EQUAL(edge2_2->cell->site_point == points[1], true);

    edge_it++;
    edge_type *edge3_1 = *edge_it;
    edge_type *edge3_2 = edge3_1->twin;
    BOOST_CHECK_EQUAL(edge3_1->cell->site_point == points[1], true);
    BOOST_CHECK_EQUAL(edge3_2->cell->site_point == points[3], true);

    edge_it++;
    edge_type *edge4_1 = *edge_it;
    edge_type *edge4_2 = edge4_1->twin;
    BOOST_CHECK_EQUAL(edge4_1->cell->site_point == points[3], true);
    BOOST_CHECK_EQUAL(edge4_2->cell->site_point == points[2], true);

    BOOST_CHECK_EQUAL(edge1_2->twin == edge1_1, true);
    BOOST_CHECK_EQUAL(edge2_2->twin == edge2_1, true);
    BOOST_CHECK_EQUAL(edge3_2->twin == edge3_1, true);
    BOOST_CHECK_EQUAL(edge4_2->twin == edge4_1, true);

    BOOST_CHECK_EQUAL(edge1_1->next == NULL && edge1_2->prev == NULL, true);
    BOOST_CHECK_EQUAL(edge2_1->next == NULL && edge2_2->prev == NULL, true);
    BOOST_CHECK_EQUAL(edge3_1->next == NULL && edge3_2->prev == NULL, true);
    BOOST_CHECK_EQUAL(edge4_1->next == NULL && edge4_2->prev == NULL, true);

    BOOST_CHECK_EQUAL(edge1_1->prev == edge4_2, true);
    BOOST_CHECK_EQUAL(edge2_1->prev == edge1_2, true);
    BOOST_CHECK_EQUAL(edge3_1->prev == edge2_2, true);
    BOOST_CHECK_EQUAL(edge4_1->prev == edge3_2, true);

    BOOST_CHECK_EQUAL(edge1_2->next == edge2_1, true);
    BOOST_CHECK_EQUAL(edge2_2->next == edge3_1, true);
    BOOST_CHECK_EQUAL(edge3_2->next == edge4_1, true);
    BOOST_CHECK_EQUAL(edge4_2->next == edge1_1, true);
}

// Sites: {(x, y) | x = 0 .. 3, y = 0 .. 3}.
BOOST_AUTO_TEST_CASE_TEMPLATE(voronoi_builder_test4, T, test_types) {
    voronoi_builder<T> test_beach_line;
    std::vector< point_2d<T> > points;
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            points.push_back(make_point_2d<T>(static_cast<T>(i),
                                              static_cast<T>(j)));
    
    test_beach_line.init(points);
    test_beach_line.run_sweepline();
    const voronoi_output< point_2d<T> > &test_output = test_beach_line.get_output();

    BOOST_CHECK_EQUAL(test_output.get_num_voronoi_cells(), 9);
    BOOST_CHECK_EQUAL(test_output.get_num_voronoi_vertices(), 4);
    BOOST_CHECK_EQUAL(test_output.get_num_voronoi_edges(), 12);
}

// Generate 10 random sites 1000 times.
BOOST_AUTO_TEST_CASE_TEMPLATE(voronoi_builder_test4_1, T, test_types) {
    srand(static_cast<unsigned int>(time(NULL)));
    voronoi_builder<T> test_beach_line;
    std::vector< point_2d<T> > points;
    for (int i = 0; i < 1000; i++) {
        points.clear();
        for (int j = 0; j < 10; j++)
            points.push_back(make_point_2d<T>(static_cast<T>(rand() % 100),
                                              static_cast<T>(rand() % 100)));
        test_beach_line.init(points);
        test_beach_line.run_sweepline();
        test_beach_line.reset();
    }
    
    test_beach_line.init(points);
    test_beach_line.run_sweepline();
}

// Sites: {(x, y) | x = 0 .. 9, y = 0 .. 9}.
BOOST_AUTO_TEST_CASE_TEMPLATE(voronoi_builder_test5, T, test_types) {
    voronoi_builder<T> test_beach_line;
    std::vector< point_2d<T> > points;
    for (int i = 0; i < 10; i++)
        for (int j = 0; j < 10; j++)
            points.push_back(make_point_2d<T>(static_cast<T>(i),
                                              static_cast<T>(j)));
    
    test_beach_line.init(points);
    test_beach_line.run_sweepline();
    const voronoi_output< point_2d<T> > &test_output = test_beach_line.get_output();

    BOOST_CHECK_EQUAL(test_output.get_num_voronoi_cells(), 100);
    BOOST_CHECK_EQUAL(test_output.get_num_voronoi_vertices(), 81);
    BOOST_CHECK_EQUAL(test_output.get_num_voronoi_edges(), 180);
}

// Generate 100 random sites 100 times.
BOOST_AUTO_TEST_CASE_TEMPLATE(voronoi_builder_test5_1, T, test_types) {
    srand(static_cast<unsigned int>(time(NULL)));
    voronoi_builder<T> test_beach_line;
    std::vector< point_2d<T> > points;
    for (int i = 0; i < 100; i++) {
        points.clear();
        for (int j = 0; j < 100; j++)
            points.push_back(make_point_2d<T>(static_cast<T>(rand() % 100),
                                              static_cast<T>(rand() % 100)));
        test_beach_line.init(points);
        test_beach_line.run_sweepline();
        test_beach_line.reset();
    }
    
    test_beach_line.init(points);
    test_beach_line.run_sweepline();
}

// Sites: {(x, y) | x = 0 .. 33, y = 0 .. 33}.
BOOST_AUTO_TEST_CASE_TEMPLATE(voronoi_builder_test6, T, test_types) {
    voronoi_builder<T> test_beach_line;
    std::vector< point_2d<T> > points;
    for (int i = 0; i < 33; i++)
        for (int j = 0; j < 33; j++)
            points.push_back(make_point_2d<T>(static_cast<T>(i),
                                              static_cast<T>(j)));
    
    test_beach_line.init(points);
    test_beach_line.run_sweepline();
    const voronoi_output< point_2d<T> > &test_output = test_beach_line.get_output();

    BOOST_CHECK_EQUAL(test_output.get_num_voronoi_cells(), 1089);
    BOOST_CHECK_EQUAL(test_output.get_num_voronoi_vertices(), 1024);
    BOOST_CHECK_EQUAL(test_output.get_num_voronoi_edges(), 2112);
}

// Generate 1000 random sites 10 times.
BOOST_AUTO_TEST_CASE_TEMPLATE(voronoi_builder_test6_1, T, test_types) {
    srand(static_cast<unsigned int>(time(NULL)));
    voronoi_builder<T> test_beach_line;
    std::vector< point_2d<T> > points;
    for (int i = 0; i < 10; i++) {
        points.clear();
        for (int j = 0; j < 1000; j++)
            points.push_back(make_point_2d<T>(static_cast<T>(rand() % 100),
                                              static_cast<T>(rand() % 100)));
        test_beach_line.init(points);
        test_beach_line.run_sweepline();
        test_beach_line.reset();
    }
    
    test_beach_line.init(points);
    test_beach_line.run_sweepline();
}

// Sites: {(x, y) | x = 0 .. 100, y = 0 .. 100}.
BOOST_AUTO_TEST_CASE_TEMPLATE(voronoi_builder_test7, T, test_types) {
    voronoi_builder<T> test_beach_line;
    std::vector< point_2d<T> > points;
    for (int i = 0; i < 100; i++)
        for (int j = 0; j < 100; j++)
            points.push_back(make_point_2d<T>(static_cast<T>(i),
                                              static_cast<T>(j)));
    
    test_beach_line.init(points);
    test_beach_line.run_sweepline();
    const voronoi_output< point_2d<T> > &test_output = test_beach_line.get_output();

    BOOST_CHECK_EQUAL(test_output.get_num_voronoi_cells(), 10000);
    BOOST_CHECK_EQUAL(test_output.get_num_voronoi_vertices(), 9801);
    BOOST_CHECK_EQUAL(test_output.get_num_voronoi_edges(), 19800);
}

// Generate 10000 random sites.
BOOST_AUTO_TEST_CASE_TEMPLATE(voronoi_builder_test7_1, T, test_types) {
    srand(static_cast<unsigned int>(time(NULL)));
    voronoi_builder<T> test_beach_line;
    std::vector< point_2d<T> > points;
    for (int i = 0; i < 10000; i++)
        points.push_back(make_point_2d<T>(static_cast<T>(rand() % 1000),
                                          static_cast<T>(rand() % 1000)));
    
    test_beach_line.init(points);
    test_beach_line.run_sweepline();
}

// Sites: {(x, y) | x = 0 .. 333, y = 0 .. 333}.
BOOST_AUTO_TEST_CASE_TEMPLATE(voronoi_builder_test8, T, test_types) {
    voronoi_builder<T> test_beach_line;
    std::vector< point_2d<T> > points;
    for (int i = 0; i < 333; i++)
        for (int j = 0; j < 333; j++)
            points.push_back(make_point_2d<T>(static_cast<T>(i),
                                              static_cast<T>(j)));
    
    test_beach_line.init(points);
    test_beach_line.run_sweepline();
    const voronoi_output< point_2d<T> > &test_output = test_beach_line.get_output();

    BOOST_CHECK_EQUAL(test_output.get_num_voronoi_cells(), 110889);
    BOOST_CHECK_EQUAL(test_output.get_num_voronoi_vertices(), 110224);
    BOOST_CHECK_EQUAL(test_output.get_num_voronoi_edges(), 221112);
}

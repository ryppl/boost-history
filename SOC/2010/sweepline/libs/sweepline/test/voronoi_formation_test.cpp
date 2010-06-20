// Boost sweepline library voronoi_formation_test.cpp file 

//          Copyright Andrii Sydorchuk 2010.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org for updates, documentation, and revision history.

#include "test_type_list.hpp"
#include <boost/sweepline/voronoi_formation.hpp>
using namespace boost::sweepline;

#define BOOST_TEST_MODULE voronoi_formation_test
#include <boost/test/test_case_template.hpp>

BOOST_AUTO_TEST_CASE_TEMPLATE(beach_line_test1, T, test_types) {
    typedef typename voronoi_formation<T>::edge_type edge_type;
    typedef typename voronoi_formation<T>::edge_iterator edge_iterator;
    typedef typename voronoi_formation<T>::voronoi_vertices_iterator
        voronoi_vertices_iterator;

    voronoi_formation<T> test_beach_line;
    point_2d<T> point1 = make_point_2d<T>(0, 0);
    point_2d<T> point2 = make_point_2d<T>(0, 4);
    point_2d<T> point3 = make_point_2d<T>(2, 1);

    std::vector< point_2d<T> > points;
    points.push_back(point1);
    points.push_back(point2);
    points.push_back(point3);
    
    test_beach_line.init(points);
    test_beach_line.run_sweepline();
    BOOST_CHECK_EQUAL(test_beach_line.get_cell_records().size(), 3);
    BOOST_CHECK_EQUAL(test_beach_line.get_voronoi_vertices().size(), 1);

    voronoi_vertices_iterator it = test_beach_line.get_voronoi_vertices().begin();
    BOOST_CHECK_EQUAL(it->incident_edges.size(), 3);
    BOOST_CHECK_EQUAL(it->vertex.x() == static_cast<T>(0.25) &&
                      it->vertex.y() == static_cast<T>(2.0), true);

    edge_iterator edge_it = it->incident_edges.begin();
    edge_type *edge1_1 = *edge_it;
    edge_type *edge1_2 = edge1_1->twin;
    BOOST_CHECK_EQUAL(edge1_1->cell_record->site_point == point3, true);
    BOOST_CHECK_EQUAL(edge1_2->cell_record->site_point == point1, true);

    edge_it++;
    edge_type *edge2_1 = *edge_it;
    edge_type *edge2_2 = edge2_1->twin;
    BOOST_CHECK_EQUAL(edge2_1->cell_record->site_point == point1, true);
    BOOST_CHECK_EQUAL(edge2_2->cell_record->site_point == point2, true);

    edge_it++;
    edge_type *edge3_1 = *edge_it;
    edge_type *edge3_2 = edge3_1->twin;
    BOOST_CHECK_EQUAL(edge3_1->cell_record->site_point == point2, true);
    BOOST_CHECK_EQUAL(edge3_2->cell_record->site_point == point3, true);

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

BOOST_AUTO_TEST_CASE_TEMPLATE(beach_line_test2, T, test_types) {
    typedef typename voronoi_formation<T>::edge_type edge_type;
    typedef typename voronoi_formation<T>::edge_iterator edge_iterator;
    typedef typename voronoi_formation<T>::voronoi_vertices_iterator
        voronoi_vertices_iterator;

    voronoi_formation<T> test_beach_line;
    point_2d<T> point1 = make_point_2d<T>(0, 1);
    point_2d<T> point2 = make_point_2d<T>(2, 0);
    point_2d<T> point3 = make_point_2d<T>(2, 4);

    std::vector< point_2d<T> > points;
    points.push_back(point1);
    points.push_back(point2);
    points.push_back(point3);
    
    test_beach_line.init(points);
    test_beach_line.run_sweepline();
    BOOST_CHECK_EQUAL(test_beach_line.get_cell_records().size(), 3);
    BOOST_CHECK_EQUAL(test_beach_line.get_voronoi_vertices().size(), 1);

    voronoi_vertices_iterator it = test_beach_line.get_voronoi_vertices().begin();
    BOOST_CHECK_EQUAL(it->incident_edges.size(), 3);
    BOOST_CHECK_EQUAL(it->vertex.x() == static_cast<T>(1.75) &&
                      it->vertex.y() == static_cast<T>(2.0), true);

    edge_iterator edge_it = it->incident_edges.begin();
    edge_type *edge1_1 = *edge_it;
    edge_type *edge1_2 = edge1_1->twin;
    BOOST_CHECK_EQUAL(edge1_1->cell_record->site_point == point2, true);
    BOOST_CHECK_EQUAL(edge1_2->cell_record->site_point == point1, true);

    edge_it++;
    edge_type *edge2_1 = *edge_it;
    edge_type *edge2_2 = edge2_1->twin;    
    BOOST_CHECK_EQUAL(edge2_1->cell_record->site_point == point1, true);
    BOOST_CHECK_EQUAL(edge2_2->cell_record->site_point == point3, true);

    edge_it++;
    edge_type *edge3_1 = *edge_it;
    edge_type *edge3_2 = edge3_1->twin;
    BOOST_CHECK_EQUAL(edge3_1->cell_record->site_point == point3, true);
    BOOST_CHECK_EQUAL(edge3_2->cell_record->site_point == point2, true);

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
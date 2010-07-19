// Boost sweepline library voronoi_builder_test.cpp file 

//          Copyright Andrii Sydorchuk 2010.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org for updates, documentation, and revision history.

#include <stdlib.h>
#include <time.h>

#include "test_type_list.hpp"
#include "boost/sweepline/voronoi_sweepline.hpp"
using namespace boost::sweepline;

#define BOOST_TEST_MODULE voronoi_builder_test
#include <boost/test/test_case_template.hpp>

#define CHECK_EQUAL_POINTS(p1, p2) \
        BOOST_CHECK_EQUAL(p1.x() == static_cast<coordinate_type>(p2.x()) && \
                          p1.y() == static_cast<coordinate_type>(p2.y()), true)

// Sites: (0, 0).
BOOST_AUTO_TEST_CASE_TEMPLATE(single_site_test, T, test_types) {
    typedef T coordinate_type;
    typedef typename voronoi_output_clipped<coordinate_type>::voronoi_const_iterator_type
        voronoi_const_iterator_type;

    voronoi_builder<coordinate_type> test_voronoi_builder;
    std::vector< point_2d<coordinate_type> > points;
    points.push_back(make_point_2d<coordinate_type>(static_cast<coordinate_type>(0),
                                                    static_cast<coordinate_type>(0)));

    test_voronoi_builder.init(points);
    test_voronoi_builder.run_sweepline();
    voronoi_output_clipped<coordinate_type> test_output;
    test_voronoi_builder.clip(test_output);
    BOOST_CHECK_EQUAL(test_output.check(), true);

    BRect<coordinate_type> bounding_rectangle = test_voronoi_builder.get_bounding_rectangle();
    BOOST_CHECK_EQUAL(bounding_rectangle.x_min == static_cast<coordinate_type>(0) &&
                      bounding_rectangle.y_min == static_cast<coordinate_type>(0) &&
                      bounding_rectangle.x_max == static_cast<coordinate_type>(0) &&
                      bounding_rectangle.y_max == static_cast<coordinate_type>(0), true);

    BOOST_CHECK_EQUAL(static_cast<int>(test_output.get_voronoi_cells().size()), 1);
    BOOST_CHECK_EQUAL(static_cast<int>(test_output.get_voronoi_vertices().size()), 0);
    BOOST_CHECK_EQUAL(static_cast<int>(test_output.get_voronoi_edges().size()), 0);

    BOOST_CHECK_EQUAL(test_output.get_num_voronoi_cells(), 1);
    BOOST_CHECK_EQUAL(test_output.get_num_voronoi_vertices(), 0);
    BOOST_CHECK_EQUAL(test_output.get_num_voronoi_edges(), 0);

    voronoi_const_iterator_type it = test_output.get_voronoi_cells().begin();
    BOOST_CHECK_EQUAL(it->num_incident_edges, 0);
    BOOST_CHECK_EQUAL(it->incident_edge == NULL, true);
}

// Sites: (0, 0), (0, 1).
BOOST_AUTO_TEST_CASE_TEMPLATE(collinear_sites_test1, T, test_types) {
    typedef T coordinate_type;
    typedef typename voronoi_output_clipped<coordinate_type>::edge_type edge_type;
    typedef typename voronoi_output_clipped<coordinate_type>::voronoi_const_iterator_type
        voronoi_const_iterator_type;

    voronoi_builder<coordinate_type> test_voronoi_builder;
    std::vector< point_2d<coordinate_type> > points;
    points.push_back(make_point_2d<coordinate_type>(static_cast<coordinate_type>(0),
                                                    static_cast<coordinate_type>(0)));
    points.push_back(make_point_2d<coordinate_type>(static_cast<coordinate_type>(0),
                                                    static_cast<coordinate_type>(1)));

    test_voronoi_builder.init(points);
    test_voronoi_builder.run_sweepline();
    voronoi_output_clipped<coordinate_type> test_output;
    test_voronoi_builder.clip(test_output);
    BOOST_CHECK_EQUAL(test_output.check(), true);

    BRect<coordinate_type> bounding_rectangle = test_voronoi_builder.get_bounding_rectangle();
    BOOST_CHECK_EQUAL(bounding_rectangle.x_min == static_cast<coordinate_type>(0) &&
                      bounding_rectangle.y_min == static_cast<coordinate_type>(0) &&
                      bounding_rectangle.x_max == static_cast<coordinate_type>(0) &&
                      bounding_rectangle.y_max == static_cast<coordinate_type>(1), true);

    BOOST_CHECK_EQUAL(static_cast<int>(test_output.get_voronoi_cells().size()), 2);
    BOOST_CHECK_EQUAL(static_cast<int>(test_output.get_voronoi_vertices().size()), 2);
    BOOST_CHECK_EQUAL(static_cast<int>(test_output.get_voronoi_edges().size()), 2);

    BOOST_CHECK_EQUAL(test_output.get_num_voronoi_cells(), 2);
    BOOST_CHECK_EQUAL(test_output.get_num_voronoi_vertices(), 0);
    BOOST_CHECK_EQUAL(test_output.get_num_voronoi_edges(), 1);

    voronoi_const_iterator_type cell_it = test_output.get_voronoi_cells().begin();
    BOOST_CHECK_EQUAL(cell_it->num_incident_edges, 1);
    cell_it++;
    BOOST_CHECK_EQUAL(cell_it->num_incident_edges, 1);

    edge_type *edge1_1 = cell_it->incident_edge;
    edge_type *edge1_2 = edge1_1->twin;

    BOOST_CHECK_EQUAL(edge1_1->twin == edge1_2, true);
    BOOST_CHECK_EQUAL(edge1_2->twin == edge1_1, true);

    BOOST_CHECK_EQUAL(edge1_1->next == edge1_1, true);
    BOOST_CHECK_EQUAL(edge1_1->prev == edge1_1, true);
    BOOST_CHECK_EQUAL(edge1_1->rot_next == edge1_1, true);
    BOOST_CHECK_EQUAL(edge1_1->rot_prev == edge1_1, true);

    BOOST_CHECK_EQUAL(edge1_2->next == edge1_2, true);
    BOOST_CHECK_EQUAL(edge1_2->prev == edge1_2, true);
    BOOST_CHECK_EQUAL(edge1_2->rot_next == edge1_2, true);
    BOOST_CHECK_EQUAL(edge1_2->rot_prev == edge1_2, true);
}

// Sites: (0, 0), (1, 1), (2, 2).
BOOST_AUTO_TEST_CASE_TEMPLATE(collinear_sites_test2, T, test_types) {
    typedef T coordinate_type;
    typedef typename voronoi_output_clipped<coordinate_type>::edge_type edge_type;
    typedef typename voronoi_output_clipped<coordinate_type>::voronoi_const_iterator_type
        voronoi_const_iterator_type;

    voronoi_builder<coordinate_type> test_voronoi_builder;
    std::vector< point_2d<coordinate_type> > points;
    points.push_back(make_point_2d<coordinate_type>(static_cast<coordinate_type>(0),
                                                    static_cast<coordinate_type>(0)));
    points.push_back(make_point_2d<coordinate_type>(static_cast<coordinate_type>(1),
                                                    static_cast<coordinate_type>(1)));
    points.push_back(make_point_2d<coordinate_type>(static_cast<coordinate_type>(2),
                                                    static_cast<coordinate_type>(2)));

    test_voronoi_builder.init(points);
    test_voronoi_builder.run_sweepline();
    voronoi_output_clipped<coordinate_type> test_output;
    test_voronoi_builder.clip(test_output);
    BOOST_CHECK_EQUAL(test_output.check(), true);

    BRect<coordinate_type> bounding_rectangle = test_voronoi_builder.get_bounding_rectangle();
    BOOST_CHECK_EQUAL(bounding_rectangle.x_min == static_cast<coordinate_type>(0) &&
                      bounding_rectangle.y_min == static_cast<coordinate_type>(0) &&
                      bounding_rectangle.x_max == static_cast<coordinate_type>(2) &&
                      bounding_rectangle.y_max == static_cast<coordinate_type>(2), true);

    BOOST_CHECK_EQUAL(static_cast<int>(test_output.get_voronoi_cells().size()), 3);
    BOOST_CHECK_EQUAL(static_cast<int>(test_output.get_voronoi_vertices().size()), 4);
    BOOST_CHECK_EQUAL(static_cast<int>(test_output.get_voronoi_edges().size()), 4);

    BOOST_CHECK_EQUAL(test_output.get_num_voronoi_cells(), 3);
    BOOST_CHECK_EQUAL(test_output.get_num_voronoi_vertices(), 0);
    BOOST_CHECK_EQUAL(test_output.get_num_voronoi_edges(), 2);

    voronoi_const_iterator_type cell_it = test_output.get_voronoi_cells().begin();
    BOOST_CHECK_EQUAL(cell_it->num_incident_edges, 1);
    edge_type *edge1_1 = cell_it->incident_edge;
    edge_type *edge1_2 = edge1_1->twin;
    cell_it++;
    BOOST_CHECK_EQUAL(cell_it->num_incident_edges, 2);
    cell_it++;
    BOOST_CHECK_EQUAL(cell_it->num_incident_edges, 1);
    edge_type *edge2_2 = cell_it->incident_edge;
    edge_type *edge2_1 = edge2_2->twin;

    BOOST_CHECK_EQUAL(edge1_1->twin == edge1_2 && edge1_2->twin == edge1_1, true);
    BOOST_CHECK_EQUAL(edge2_1->twin == edge2_2 && edge2_2->twin == edge2_1, true);

    BOOST_CHECK_EQUAL(edge1_1->next == edge1_1 && edge1_1->prev == edge1_1, true);
    BOOST_CHECK_EQUAL(edge1_1->rot_next == edge1_1 && edge1_1->rot_prev == edge1_1, true);
    BOOST_CHECK_EQUAL(edge1_2->rot_next == edge1_2 && edge1_2->rot_prev == edge1_2, true);
    BOOST_CHECK_EQUAL(edge2_1->rot_next == edge2_1 && edge2_1->rot_prev == edge2_1, true);
    BOOST_CHECK_EQUAL(edge2_2->next == edge2_2 && edge2_2->prev == edge2_2, true);
    BOOST_CHECK_EQUAL(edge2_2->rot_next == edge2_2 && edge2_2->rot_prev == edge2_2, true);

    BOOST_CHECK_EQUAL(edge1_2->next == edge2_1 && edge1_2->prev == edge2_1, true);
    BOOST_CHECK_EQUAL(edge2_1->next == edge1_2 && edge2_1->prev == edge1_2, true);
}

// Sites: (0, 0), (0, 4), (2, 1).
BOOST_AUTO_TEST_CASE_TEMPLATE(triangle_test1, T, test_types) {
    typedef T coordinate_type;
    typedef typename voronoi_output_clipped<coordinate_type>::edge_type edge_type;
    typedef typename voronoi_output_clipped<coordinate_type>::voronoi_const_iterator_type
        voronoi_const_iterator_type;

    voronoi_builder<coordinate_type> test_beach_line;
    point_2d<coordinate_type> point1 = make_point_2d<coordinate_type>(
        static_cast<coordinate_type>(0), static_cast<coordinate_type>(0));
    point_2d<coordinate_type> point2 = make_point_2d<coordinate_type>(
        static_cast<coordinate_type>(0), static_cast<coordinate_type>(4));
    point_2d<coordinate_type> point3 = make_point_2d<coordinate_type>(
        static_cast<coordinate_type>(2), static_cast<coordinate_type>(1));

    std::vector< point_2d<coordinate_type> > points;
    points.push_back(point1);
    points.push_back(point2);
    points.push_back(point3);
    
    test_beach_line.init(points);
    test_beach_line.run_sweepline();
    voronoi_output_clipped<coordinate_type> test_output;
    test_beach_line.clip(test_output);
    BOOST_CHECK_EQUAL(test_output.check(), true);

    BRect<coordinate_type> bounding_rectangle = test_beach_line.get_bounding_rectangle();
    BOOST_CHECK_EQUAL(bounding_rectangle.x_min == static_cast<coordinate_type>(0) &&
                      bounding_rectangle.y_min == static_cast<coordinate_type>(0) &&
                      bounding_rectangle.x_max == static_cast<coordinate_type>(2) &&
                      bounding_rectangle.y_max == static_cast<coordinate_type>(4), true);

    BOOST_CHECK_EQUAL(static_cast<int>(test_output.get_voronoi_cells().size()), 3);
    BOOST_CHECK_EQUAL(static_cast<int>(test_output.get_voronoi_vertices().size()), 4);

    voronoi_const_iterator_type it = test_output.get_voronoi_vertices().begin();
    BOOST_CHECK_EQUAL(it->num_incident_edges, 3);
    BOOST_CHECK_EQUAL(it->voronoi_point.x() == static_cast<coordinate_type>(0.25) &&
                      it->voronoi_point.y() == static_cast<coordinate_type>(2.0), true);

    edge_type *edge1_1 = it->incident_edge;
    edge_type *edge1_2 = edge1_1->twin;
    CHECK_EQUAL_POINTS(edge1_1->cell->voronoi_point, point3);
    CHECK_EQUAL_POINTS(edge1_2->cell->voronoi_point, point1);

    edge_type *edge2_1 = edge1_1->rot_prev;
    edge_type *edge2_2 = edge2_1->twin;
    CHECK_EQUAL_POINTS(edge2_1->cell->voronoi_point, point1);
    CHECK_EQUAL_POINTS(edge2_2->cell->voronoi_point, point2);

    edge_type *edge3_1 = edge2_1->rot_prev;
    edge_type *edge3_2 = edge3_1->twin;
    CHECK_EQUAL_POINTS(edge3_1->cell->voronoi_point, point2);
    CHECK_EQUAL_POINTS(edge3_2->cell->voronoi_point, point3);

    BOOST_CHECK_EQUAL(edge1_2->twin == edge1_1, true);
    BOOST_CHECK_EQUAL(edge2_2->twin == edge2_1, true);
    BOOST_CHECK_EQUAL(edge3_2->twin == edge3_1, true);

    BOOST_CHECK_EQUAL(edge1_1->prev == edge3_2 && edge1_1->next == edge3_2, true);
    BOOST_CHECK_EQUAL(edge2_1->prev == edge1_2 && edge2_1->next == edge1_2, true);
    BOOST_CHECK_EQUAL(edge3_1->prev == edge2_2 && edge3_1->next == edge2_2, true);

    BOOST_CHECK_EQUAL(edge1_2->next == edge2_1 && edge1_2->prev == edge2_1, true);
    BOOST_CHECK_EQUAL(edge2_2->next == edge3_1 && edge2_2->prev == edge3_1, true);
    BOOST_CHECK_EQUAL(edge3_2->next == edge1_1 && edge3_2->prev == edge1_1, true);

    BOOST_CHECK_EQUAL(edge1_1->rot_next == edge3_1, true);
    BOOST_CHECK_EQUAL(edge3_1->rot_next == edge2_1, true);
    BOOST_CHECK_EQUAL(edge2_1->rot_next == edge1_1, true);
}

// Sites: (0, 1), (2, 0), (2, 4).
BOOST_AUTO_TEST_CASE_TEMPLATE(triangle_test2, T, test_types) {
    typedef T coordinate_type;
    typedef typename voronoi_output_clipped<coordinate_type>::edge_type edge_type;
    typedef typename voronoi_output_clipped<coordinate_type>::voronoi_const_iterator_type
        voronoi_const_iterator_type;

    voronoi_builder<coordinate_type> test_beach_line;
    point_2d<coordinate_type> point1 = make_point_2d<coordinate_type>(
        static_cast<coordinate_type>(0), static_cast<coordinate_type>(1));
    point_2d<coordinate_type> point2 = make_point_2d<coordinate_type>(
        static_cast<coordinate_type>(2), static_cast<coordinate_type>(0));
    point_2d<coordinate_type> point3 = make_point_2d<coordinate_type>(
        static_cast<coordinate_type>(2), static_cast<coordinate_type>(4));

    std::vector< point_2d<coordinate_type> > points;
    points.push_back(point1);
    points.push_back(point2);
    points.push_back(point3);
    
    test_beach_line.init(points);
    test_beach_line.run_sweepline();
    voronoi_output_clipped<coordinate_type> test_output;
    test_beach_line.clip(test_output);
    BOOST_CHECK_EQUAL(test_output.check(), true);

    BOOST_CHECK_EQUAL(static_cast<int>(test_output.get_voronoi_cells().size()), 3);
    BOOST_CHECK_EQUAL(static_cast<int>(test_output.get_voronoi_vertices().size()), 4);

    voronoi_const_iterator_type it = test_output.get_voronoi_vertices().begin();
    BOOST_CHECK_EQUAL(it->num_incident_edges, 3);
    BOOST_CHECK_EQUAL(it->voronoi_point.x() == static_cast<coordinate_type>(1.75) &&
                      it->voronoi_point.y() == static_cast<coordinate_type>(2.0), true);

    edge_type *edge1_1 = it->incident_edge;
    edge_type *edge1_2 = edge1_1->twin;
    CHECK_EQUAL_POINTS(edge1_1->cell->voronoi_point, point2);
    CHECK_EQUAL_POINTS(edge1_2->cell->voronoi_point, point1);

    edge_type *edge2_1 = edge1_1->rot_prev;
    edge_type *edge2_2 = edge2_1->twin;    
    CHECK_EQUAL_POINTS(edge2_1->cell->voronoi_point, point1);
    CHECK_EQUAL_POINTS(edge2_2->cell->voronoi_point, point3);

    edge_type *edge3_1 = edge2_1->rot_prev;
    edge_type *edge3_2 = edge3_1->twin;
    CHECK_EQUAL_POINTS(edge3_1->cell->voronoi_point, point3);
    CHECK_EQUAL_POINTS(edge3_2->cell->voronoi_point, point2);

    BOOST_CHECK_EQUAL(edge1_2->twin == edge1_1, true);
    BOOST_CHECK_EQUAL(edge2_2->twin == edge2_1, true);
    BOOST_CHECK_EQUAL(edge3_2->twin == edge3_1, true);

    BOOST_CHECK_EQUAL(edge1_1->prev == edge3_2 && edge1_1->next == edge3_2, true);
    BOOST_CHECK_EQUAL(edge2_1->prev == edge1_2 && edge2_1->next == edge1_2, true);
    BOOST_CHECK_EQUAL(edge3_1->prev == edge2_2 && edge3_1->next == edge2_2, true);

    BOOST_CHECK_EQUAL(edge1_2->next == edge2_1 && edge1_2->prev == edge2_1, true);
    BOOST_CHECK_EQUAL(edge2_2->next == edge3_1 && edge2_2->prev == edge3_1, true);
    BOOST_CHECK_EQUAL(edge3_2->next == edge1_1 && edge3_2->prev == edge1_1, true);

    BOOST_CHECK_EQUAL(edge1_1->rot_next == edge3_1, true);
    BOOST_CHECK_EQUAL(edge3_1->rot_next == edge2_1, true);
    BOOST_CHECK_EQUAL(edge2_1->rot_next == edge1_1, true);
}

// Sites: (0, 0), (0, 1), (1, 0), (1, 1).
BOOST_AUTO_TEST_CASE_TEMPLATE(square_test3, T, test_types) {
    typedef T coordinate_type;
    typedef typename voronoi_output_clipped<coordinate_type>::edge_type edge_type;
    typedef typename voronoi_output_clipped<coordinate_type>::voronoi_const_iterator_type
        voronoi_const_iterator_type;

    voronoi_builder<coordinate_type> test_beach_line;
    std::vector< point_2d<coordinate_type> > points;
    points.push_back(make_point_2d<coordinate_type>(static_cast<coordinate_type>(0),
                                                    static_cast<coordinate_type>(0)));
    points.push_back(make_point_2d<coordinate_type>(static_cast<coordinate_type>(0),
                                                    static_cast<coordinate_type>(1)));
    points.push_back(make_point_2d<coordinate_type>(static_cast<coordinate_type>(1),
                                                    static_cast<coordinate_type>(0)));
    points.push_back(make_point_2d<coordinate_type>(static_cast<coordinate_type>(1),
                                                    static_cast<coordinate_type>(1)));
    
    test_beach_line.init(points);
    test_beach_line.run_sweepline();
    voronoi_output_clipped<coordinate_type> test_output;
    test_beach_line.clip(test_output);
    BOOST_CHECK_EQUAL(test_output.check(), true);

    BOOST_CHECK_EQUAL(static_cast<coordinate_type>(test_output.get_voronoi_cells().size()), 4);
    BOOST_CHECK_EQUAL(static_cast<coordinate_type>(test_output.get_voronoi_vertices().size()), 5);
    BOOST_CHECK_EQUAL(test_output.get_num_voronoi_cells(), 4);
    BOOST_CHECK_EQUAL(test_output.get_num_voronoi_vertices(), 1);
    BOOST_CHECK_EQUAL(test_output.get_num_voronoi_edges(), 4);

    // Check voronoi vertex.
    voronoi_const_iterator_type it = test_output.get_voronoi_vertices().begin();
    BOOST_CHECK_EQUAL(it->num_incident_edges, 4);
    BOOST_CHECK_EQUAL(it->voronoi_point.x() == static_cast<coordinate_type>(0.5) &&
                      it->voronoi_point.y() == static_cast<coordinate_type>(0.5), true);

    // Check voronoi edges.
    edge_type *edge1_1 = it->incident_edge;
    edge_type *edge1_2 = edge1_1->twin;
    CHECK_EQUAL_POINTS(edge1_1->cell->voronoi_point, points[2]);
    CHECK_EQUAL_POINTS(edge1_2->cell->voronoi_point, points[0]);

    edge_type *edge2_1 = edge1_1->rot_prev;
    edge_type *edge2_2 = edge2_1->twin;    
    CHECK_EQUAL_POINTS(edge2_1->cell->voronoi_point, points[0]);
    CHECK_EQUAL_POINTS(edge2_2->cell->voronoi_point, points[1]);

    edge_type *edge3_1 = edge2_1->rot_prev;
    edge_type *edge3_2 = edge3_1->twin;
    CHECK_EQUAL_POINTS(edge3_1->cell->voronoi_point, points[1]);
    CHECK_EQUAL_POINTS(edge3_2->cell->voronoi_point, points[3]);

    edge_type *edge4_1 = edge3_1->rot_prev;
    edge_type *edge4_2 = edge4_1->twin;
    CHECK_EQUAL_POINTS(edge4_1->cell->voronoi_point, points[3]);
    CHECK_EQUAL_POINTS(edge4_2->cell->voronoi_point, points[2]);

    BOOST_CHECK_EQUAL(edge1_2->twin == edge1_1, true);
    BOOST_CHECK_EQUAL(edge2_2->twin == edge2_1, true);
    BOOST_CHECK_EQUAL(edge3_2->twin == edge3_1, true);
    BOOST_CHECK_EQUAL(edge4_2->twin == edge4_1, true);

    BOOST_CHECK_EQUAL(edge1_1->prev == edge4_2 && edge1_1->next == edge4_2, true);
    BOOST_CHECK_EQUAL(edge2_1->prev == edge1_2 && edge2_1->next == edge1_2, true);
    BOOST_CHECK_EQUAL(edge3_1->prev == edge2_2 && edge3_1->next == edge2_2, true);
    BOOST_CHECK_EQUAL(edge4_1->prev == edge3_2 && edge4_1->next == edge3_2, true);

    BOOST_CHECK_EQUAL(edge1_2->next == edge2_1 && edge1_2->prev == edge2_1, true);
    BOOST_CHECK_EQUAL(edge2_2->next == edge3_1 && edge2_2->prev == edge3_1, true);
    BOOST_CHECK_EQUAL(edge3_2->next == edge4_1 && edge3_2->prev == edge4_1, true);
    BOOST_CHECK_EQUAL(edge4_2->next == edge1_1 && edge4_2->prev == edge1_1, true);

    BOOST_CHECK_EQUAL(edge1_1->rot_next == edge4_1, true);
    BOOST_CHECK_EQUAL(edge4_1->rot_next == edge3_1, true);
    BOOST_CHECK_EQUAL(edge3_1->rot_next == edge2_1, true);
    BOOST_CHECK_EQUAL(edge2_1->rot_next == edge1_1, true);
}

// Sites: {(x, y) | x = 0 .. 3, y = 0 .. 3}.
BOOST_AUTO_TEST_CASE_TEMPLATE(grid_test1, T, test_types) {
    typedef T coordinate_type;
    voronoi_builder<coordinate_type> test_beach_line;
    std::vector< point_2d<coordinate_type> > points;
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            points.push_back(make_point_2d<coordinate_type>(
                static_cast<coordinate_type>(i), static_cast<coordinate_type>(j)));
    
    test_beach_line.init(points);
    test_beach_line.run_sweepline();
    voronoi_output_clipped<coordinate_type> test_output;
    test_beach_line.clip(test_output);
    BOOST_CHECK_EQUAL(test_output.check(), true);

    BOOST_CHECK_EQUAL(test_output.get_num_voronoi_cells(), 9);
    BOOST_CHECK_EQUAL(test_output.get_num_voronoi_vertices(), 4);
    BOOST_CHECK_EQUAL(test_output.get_num_voronoi_edges(), 12);
}

// Sites: {(x, y) | x = 0 .. 9, y = 0 .. 9}.
BOOST_AUTO_TEST_CASE_TEMPLATE(grid_test2, T, test_types) {
    typedef T coordinate_type;
    voronoi_builder<coordinate_type> test_beach_line;
    std::vector< point_2d<coordinate_type> > points;
    for (int i = 0; i < 10; i++)
        for (int j = 0; j < 10; j++)
            points.push_back(make_point_2d<coordinate_type>(
                static_cast<coordinate_type>(i), static_cast<coordinate_type>(j)));
    
    test_beach_line.init(points);
    test_beach_line.run_sweepline();
    voronoi_output_clipped<coordinate_type> test_output;
    test_beach_line.clip(test_output);
    BOOST_CHECK_EQUAL(test_output.check(), true);

    BOOST_CHECK_EQUAL(test_output.get_num_voronoi_cells(), 100);
    BOOST_CHECK_EQUAL(test_output.get_num_voronoi_vertices(), 81);
    BOOST_CHECK_EQUAL(test_output.get_num_voronoi_edges(), 180);
}

// Sites: {(x, y) | x = 0 .. 33, y = 0 .. 33}.
BOOST_AUTO_TEST_CASE_TEMPLATE(grid_test3, T, test_types) {
    typedef T coordinate_type;
    voronoi_builder<coordinate_type> test_beach_line;
    std::vector< point_2d<coordinate_type> > points;
    for (int i = 0; i < 33; i++)
        for (int j = 0; j < 33; j++)
            points.push_back(make_point_2d<coordinate_type>(
                static_cast<coordinate_type>(i), static_cast<coordinate_type>(j)));
    
    test_beach_line.init(points);
    test_beach_line.run_sweepline();
    voronoi_output_clipped<coordinate_type> test_output;
    test_beach_line.clip(test_output);
    BOOST_CHECK_EQUAL(test_output.check(), true);

    BOOST_CHECK_EQUAL(test_output.get_num_voronoi_cells(), 1089);
    BOOST_CHECK_EQUAL(test_output.get_num_voronoi_vertices(), 1024);
    BOOST_CHECK_EQUAL(test_output.get_num_voronoi_edges(), 2112);
}

// Sites: {(x, y) | x = 0 .. 100, y = 0 .. 100}.
BOOST_AUTO_TEST_CASE_TEMPLATE(grid_test4, T, test_types) {
    typedef T coordinate_type;
    voronoi_builder<coordinate_type> test_beach_line;
    std::vector< point_2d<coordinate_type> > points;
    for (int i = 0; i < 100; i++)
        for (int j = 0; j < 100; j++)
            points.push_back(make_point_2d<coordinate_type>(
                static_cast<coordinate_type>(i), static_cast<coordinate_type>(j)));
    
    test_beach_line.init(points);
    test_beach_line.run_sweepline();
    voronoi_output_clipped<coordinate_type> test_output;
    test_beach_line.clip(test_output);
    BOOST_CHECK_EQUAL(test_output.check(), true);

    BOOST_CHECK_EQUAL(test_output.get_num_voronoi_cells(), 10000);
    BOOST_CHECK_EQUAL(test_output.get_num_voronoi_vertices(), 9801);
    BOOST_CHECK_EQUAL(test_output.get_num_voronoi_edges(), 19800);
}

// Sites: {(x, y) | x = 0 .. 333, y = 0 .. 333}.
BOOST_AUTO_TEST_CASE_TEMPLATE(grid_test5, T, test_types) {
    typedef T coordinate_type;
    voronoi_builder<coordinate_type> test_beach_line;
    std::vector< point_2d<coordinate_type> > points;
    for (int i = 0; i < 333; i++)
        for (int j = 0; j < 333; j++)
            points.push_back(make_point_2d<coordinate_type>(
                static_cast<coordinate_type>(i), static_cast<coordinate_type>(j)));
    
    test_beach_line.init(points);
    test_beach_line.run_sweepline();
    voronoi_output_clipped<coordinate_type> test_output;
    test_beach_line.clip(test_output);
    BOOST_CHECK_EQUAL(test_output.check(), true);

    BOOST_CHECK_EQUAL(test_output.get_num_voronoi_cells(), 110889);
    BOOST_CHECK_EQUAL(test_output.get_num_voronoi_vertices(), 110224);
    BOOST_CHECK_EQUAL(test_output.get_num_voronoi_edges(), 221112);
}

// Generate 10 random sites 10000 times.
BOOST_AUTO_TEST_CASE_TEMPLATE(random_test1, T, test_types) {
    typedef T coordinate_type;
    srand(static_cast<unsigned int>(time(NULL)));
    voronoi_builder<coordinate_type> test_beach_line;
    voronoi_output_clipped<coordinate_type> test_output;
    std::vector< point_2d<coordinate_type> > points;
    for (int i = 0; i < 10000; i++) {
        points.clear();
        for (int j = 0; j < 10; j++)
            points.push_back(make_point_2d<coordinate_type>(
                static_cast<coordinate_type>(rand() % 10),
                static_cast<coordinate_type>(rand() % 10)));
        test_beach_line.init(points);
        test_beach_line.run_sweepline();
        test_beach_line.clip(test_output);
        BOOST_CHECK_EQUAL(test_output.check(), true);
        test_beach_line.reset();
    }
}

// Generate 100 random sites 1000 times.
BOOST_AUTO_TEST_CASE_TEMPLATE(random_test2, T, test_types) {
    typedef T coordinate_type;
    srand(static_cast<unsigned int>(time(NULL)));
    voronoi_builder<coordinate_type> test_beach_line;
    voronoi_output_clipped<coordinate_type> test_output;
    std::vector< point_2d<coordinate_type> > points;
    for (int i = 0; i < 1000; i++) {
        points.clear();
        for (int j = 0; j < 100; j++)
            points.push_back(make_point_2d<coordinate_type>(
                static_cast<coordinate_type>(rand() % 100),
                static_cast<coordinate_type>(rand() % 100)));
        test_beach_line.init(points);
        test_beach_line.run_sweepline();
        test_beach_line.clip(test_output);
        BOOST_CHECK_EQUAL(test_output.check(), true);
        test_beach_line.reset();
    }
}

// Generate 1000 random sites 100 times.
BOOST_AUTO_TEST_CASE_TEMPLATE(random_test3, T, test_types) {
    typedef T coordinate_type;
    srand(static_cast<unsigned int>(time(NULL)));
    voronoi_builder<coordinate_type> test_beach_line;
    voronoi_output_clipped<coordinate_type> test_output;
    std::vector< point_2d<coordinate_type> > points;
    for (int i = 0; i < 100; i++) {
        points.clear();
        for (int j = 0; j < 1000; j++)
        points.push_back(make_point_2d<coordinate_type>(
            static_cast<coordinate_type>(rand() % 100),
            static_cast<coordinate_type>(rand() % 100)));
        test_beach_line.init(points);
        test_beach_line.run_sweepline();
        test_beach_line.clip(test_output);
        BOOST_CHECK_EQUAL(test_output.check(), true);
        test_beach_line.reset();
    }
}

// Generate 10000 random sites 10 times.
BOOST_AUTO_TEST_CASE_TEMPLATE(random_test4, T, test_types) {
    typedef T coordinate_type;
    srand(static_cast<unsigned int>(time(NULL)));
    voronoi_builder<coordinate_type> test_beach_line;
    voronoi_output_clipped<coordinate_type> test_output;
    std::vector< point_2d<coordinate_type> > points;
    for (int i = 0; i < 10; i++) {
        points.clear();
        for (int j = 0; j < 10000; j++)
        points.push_back(make_point_2d<coordinate_type>(\
            static_cast<coordinate_type>(rand() % 1000),
            static_cast<coordinate_type>(rand() % 1000)));
        test_beach_line.init(points);
        test_beach_line.run_sweepline();
        test_beach_line.clip(test_output);
        BOOST_CHECK_EQUAL(test_output.check(), true);
        test_beach_line.reset();
    }
}

// Generate 100000 random sites.
BOOST_AUTO_TEST_CASE_TEMPLATE(random_test5, T, test_types) {
    typedef T coordinate_type;
    srand(static_cast<unsigned int>(time(NULL)));
    voronoi_builder<coordinate_type> test_beach_line;
    voronoi_output_clipped<coordinate_type> test_output;
    std::vector< point_2d<coordinate_type> > points;
    for (int i = 0; i < 1; i++) {
        points.clear();
        for (int j = 0; j < 100000; j++)
        points.push_back(make_point_2d<coordinate_type>(
            static_cast<coordinate_type>(rand() % 1000),
            static_cast<coordinate_type>(rand() % 1000)));
        test_beach_line.init(points);
        test_beach_line.run_sweepline();
        test_beach_line.clip(test_output);
        BOOST_CHECK_EQUAL(test_output.check(), true);
        test_beach_line.reset();
    }
}

// Generate 1000000 random sites.
BOOST_AUTO_TEST_CASE_TEMPLATE(random_test6, T, test_types) {
    typedef T coordinate_type;
    srand(static_cast<unsigned int>(time(NULL)));
    voronoi_builder<coordinate_type> test_beach_line;
    voronoi_output_clipped<coordinate_type> test_output;
    std::vector< point_2d<coordinate_type> > points;
    for (int i = 0; i < 1; i++) {
        points.clear();
        for (int j = 0; j < 1000000; j++)
        points.push_back(make_point_2d<coordinate_type>(
            static_cast<coordinate_type>(rand() % 1000),
            static_cast<coordinate_type>(rand() % 1000)));
        test_beach_line.init(points);
        test_beach_line.run_sweepline();
        test_beach_line.clip(test_output);
        BOOST_CHECK_EQUAL(test_output.check(), true);
        test_beach_line.reset();
    }
}
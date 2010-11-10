// Boost sweepline library builder_test.cpp file 

//          Copyright Andrii Sydorchuk 2010.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org for updates, documentation, and revision history.

#include <stdlib.h>
#include <time.h>

#include "test_type_list.hpp"
#include "output_verification.hpp"
#include "boost/sweepline/voronoi_sweepline.hpp"
using namespace boost::sweepline;

#define BOOST_TEST_MODULE voronoi_builder_test
#include <boost/test/test_case_template.hpp>

#define CHECK_EQUAL_POINTS(p1, p2) \
        BOOST_CHECK_EQUAL(p1.x() == static_cast<coordinate_type>(p2.x()) && \
                          p1.y() == static_cast<coordinate_type>(p2.y()), true)

#define VERIFY_VORONOI_OUTPUT(output, mask) BOOST_CHECK_EQUAL(verify_output(output, mask), true)

#define ALMOST_EQUAL_TEST(a, b, ULP_ERR, ABS_ERR) \
        BOOST_CHECK_EQUAL(detail::almost_equal(a, b, ULP_ERR) || \
                          detail::abs_equal(a, b, ABS_ERR), true);

// Sites: (0, 0).
BOOST_AUTO_TEST_CASE_TEMPLATE(single_site_test, T, test_types) {
    typedef T coordinate_type;
    typedef typename voronoi_output_clipped<coordinate_type>::voronoi_cell_const_iterator_type
        voronoi_cell_const_iterator_type;

    voronoi_builder<coordinate_type> test_voronoi_builder;
    std::vector< point_2d<coordinate_type> > points;
    points.push_back(make_point_2d<coordinate_type>(static_cast<coordinate_type>(0),
                                                    static_cast<coordinate_type>(0)));

    test_voronoi_builder.init(points);
    test_voronoi_builder.run_sweepline();
    voronoi_output_clipped<coordinate_type> test_output;
    test_voronoi_builder.clip(test_output);
    VERIFY_VORONOI_OUTPUT(test_output, COMPLETE_VERIFICATION);

    BRect<coordinate_type> bounding_rectangle = test_voronoi_builder.get_bounding_rectangle();
    BOOST_CHECK_EQUAL(bounding_rectangle.x_min == static_cast<coordinate_type>(0) &&
                      bounding_rectangle.y_min == static_cast<coordinate_type>(0) &&
                      bounding_rectangle.x_max == static_cast<coordinate_type>(0) &&
                      bounding_rectangle.y_max == static_cast<coordinate_type>(0), true);

    BOOST_CHECK_EQUAL(static_cast<int>(test_output.cell_records.size()), 1);
    BOOST_CHECK_EQUAL(static_cast<int>(test_output.vertex_records.size()), 0);
    BOOST_CHECK_EQUAL(static_cast<int>(test_output.edge_records.size()), 0);

    BOOST_CHECK_EQUAL(test_output.num_cell_records, 1);
    BOOST_CHECK_EQUAL(test_output.num_vertex_records, 0);
    BOOST_CHECK_EQUAL(test_output.num_edge_records, 0);

    voronoi_cell_const_iterator_type it = test_output.cell_records.begin();
    BOOST_CHECK_EQUAL(it->num_incident_edges, 0);
    BOOST_CHECK_EQUAL(it->incident_edge == NULL, true);
}

// Sites: (0, 0), (0, 1).
BOOST_AUTO_TEST_CASE_TEMPLATE(collinear_sites_test1, T, test_types) {
    typedef T coordinate_type;
    typedef typename voronoi_output_clipped<coordinate_type>::voronoi_edge_type voronoi_edge_type;
    typedef typename voronoi_output_clipped<coordinate_type>::voronoi_cell_const_iterator_type
        voronoi_cell_const_iterator_type;

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
    VERIFY_VORONOI_OUTPUT(test_output, COMPLETE_VERIFICATION);

    BRect<coordinate_type> bounding_rectangle = test_voronoi_builder.get_bounding_rectangle();
    BOOST_CHECK_EQUAL(bounding_rectangle.x_min == static_cast<coordinate_type>(0) &&
                      bounding_rectangle.y_min == static_cast<coordinate_type>(0) &&
                      bounding_rectangle.x_max == static_cast<coordinate_type>(0) &&
                      bounding_rectangle.y_max == static_cast<coordinate_type>(1), true);

    BOOST_CHECK_EQUAL(static_cast<int>(test_output.cell_records.size()), 2);
    BOOST_CHECK_EQUAL(static_cast<int>(test_output.vertex_records.size()), 0);
    BOOST_CHECK_EQUAL(static_cast<int>(test_output.edge_records.size()), 2);

    BOOST_CHECK_EQUAL(test_output.num_cell_records, 2);
    BOOST_CHECK_EQUAL(test_output.num_vertex_records, 0);
    BOOST_CHECK_EQUAL(test_output.num_edge_records, 1);

    voronoi_cell_const_iterator_type cell_it = test_output.cell_records.begin();
    BOOST_CHECK_EQUAL(cell_it->num_incident_edges, 1);
    cell_it++;
    BOOST_CHECK_EQUAL(cell_it->num_incident_edges, 1);

    voronoi_edge_type *edge1_1 = cell_it->incident_edge;
    voronoi_edge_type *edge1_2 = edge1_1->twin;

    BOOST_CHECK_EQUAL(edge1_1->twin == edge1_2, true);
    BOOST_CHECK_EQUAL(edge1_2->twin == edge1_1, true);

    BOOST_CHECK_EQUAL(edge1_1->next == edge1_1, true);
    BOOST_CHECK_EQUAL(edge1_1->prev == edge1_1, true);
    BOOST_CHECK_EQUAL(edge1_1->rot_next == NULL, true);
    BOOST_CHECK_EQUAL(edge1_1->rot_prev == NULL, true);

    BOOST_CHECK_EQUAL(edge1_2->next == edge1_2, true);
    BOOST_CHECK_EQUAL(edge1_2->prev == edge1_2, true);
    BOOST_CHECK_EQUAL(edge1_2->rot_next == NULL, true);
    BOOST_CHECK_EQUAL(edge1_2->rot_prev == NULL, true);
}

// Sites: (0, 0), (1, 1), (2, 2).
BOOST_AUTO_TEST_CASE_TEMPLATE(collinear_sites_test2, T, test_types) {
    typedef T coordinate_type;
    typedef typename voronoi_output_clipped<coordinate_type>::voronoi_edge_type voronoi_edge_type;
    typedef typename voronoi_output_clipped<coordinate_type>::voronoi_cell_const_iterator_type
        voronoi_cell_const_iterator_type;

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
    VERIFY_VORONOI_OUTPUT(test_output, COMPLETE_VERIFICATION);

    BRect<coordinate_type> bounding_rectangle = test_voronoi_builder.get_bounding_rectangle();
    BOOST_CHECK_EQUAL(bounding_rectangle.x_min == static_cast<coordinate_type>(0) &&
                      bounding_rectangle.y_min == static_cast<coordinate_type>(0) &&
                      bounding_rectangle.x_max == static_cast<coordinate_type>(2) &&
                      bounding_rectangle.y_max == static_cast<coordinate_type>(2), true);

    BOOST_CHECK_EQUAL(static_cast<int>(test_output.cell_records.size()), 3);
    BOOST_CHECK_EQUAL(static_cast<int>(test_output.vertex_records.size()), 0);
    BOOST_CHECK_EQUAL(static_cast<int>(test_output.edge_records.size()), 4);

    BOOST_CHECK_EQUAL(test_output.num_cell_records, 3);
    BOOST_CHECK_EQUAL(test_output.num_vertex_records, 0);
    BOOST_CHECK_EQUAL(test_output.num_edge_records, 2);

    voronoi_cell_const_iterator_type cell_it = test_output.cell_records.begin();
    BOOST_CHECK_EQUAL(cell_it->num_incident_edges, 1);
    voronoi_edge_type *edge1_1 = cell_it->incident_edge;
    voronoi_edge_type *edge1_2 = edge1_1->twin;
    cell_it++;
    BOOST_CHECK_EQUAL(cell_it->num_incident_edges, 2);
    cell_it++;
    BOOST_CHECK_EQUAL(cell_it->num_incident_edges, 1);
    voronoi_edge_type *edge2_2 = cell_it->incident_edge;
    voronoi_edge_type *edge2_1 = edge2_2->twin;

    BOOST_CHECK_EQUAL(edge1_1->twin == edge1_2 && edge1_2->twin == edge1_1, true);
    BOOST_CHECK_EQUAL(edge2_1->twin == edge2_2 && edge2_2->twin == edge2_1, true);

    BOOST_CHECK_EQUAL(edge1_1->next == edge1_1 && edge1_1->prev == edge1_1, true);
    BOOST_CHECK_EQUAL(edge1_1->rot_next == NULL && edge1_1->rot_prev == NULL, true);
    BOOST_CHECK_EQUAL(edge1_2->rot_next == NULL && edge1_2->rot_prev == NULL, true);
    BOOST_CHECK_EQUAL(edge2_1->rot_next == NULL && edge2_1->rot_prev == NULL, true);
    BOOST_CHECK_EQUAL(edge2_2->next == edge2_2 && edge2_2->prev == edge2_2, true);
    BOOST_CHECK_EQUAL(edge2_2->rot_next == NULL && edge2_2->rot_prev == NULL, true);

    BOOST_CHECK_EQUAL(edge1_2->next == edge2_1 && edge1_2->prev == edge2_1, true);
    BOOST_CHECK_EQUAL(edge2_1->next == edge1_2 && edge2_1->prev == edge1_2, true);
}

// Sites: (0, 0), (0, 4), (2, 1).
BOOST_AUTO_TEST_CASE_TEMPLATE(triangle_test1, T, test_types) {
    typedef T coordinate_type;
    typedef typename voronoi_output_clipped<coordinate_type>::voronoi_edge_type voronoi_edge_type;
    typedef typename voronoi_output_clipped<coordinate_type>::voronoi_vertex_const_iterator_type
        voronoi_vertex_const_iterator_type;

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
    VERIFY_VORONOI_OUTPUT(test_output, COMPLETE_VERIFICATION);

    BRect<coordinate_type> bounding_rectangle = test_beach_line.get_bounding_rectangle();
    BOOST_CHECK_EQUAL(bounding_rectangle.x_min == static_cast<coordinate_type>(0) &&
                      bounding_rectangle.y_min == static_cast<coordinate_type>(0) &&
                      bounding_rectangle.x_max == static_cast<coordinate_type>(2) &&
                      bounding_rectangle.y_max == static_cast<coordinate_type>(4), true);

    BOOST_CHECK_EQUAL(static_cast<int>(test_output.cell_records.size()), 3);
    BOOST_CHECK_EQUAL(static_cast<int>(test_output.vertex_records.size()), 1);

    voronoi_vertex_const_iterator_type it = test_output.vertex_records.begin();
    BOOST_CHECK_EQUAL(it->num_incident_edges, 3);
    BOOST_CHECK_EQUAL(it->vertex.x() == static_cast<coordinate_type>(0.25) &&
                      it->vertex.y() == static_cast<coordinate_type>(2.0), true);

    voronoi_edge_type *edge1_1 = it->incident_edge;
    voronoi_edge_type *edge1_2 = edge1_1->twin;
    CHECK_EQUAL_POINTS(edge1_1->cell->get_point0(), point3);
    CHECK_EQUAL_POINTS(edge1_2->cell->get_point0(), point1);

    voronoi_edge_type *edge2_1 = edge1_1->rot_prev;
    voronoi_edge_type *edge2_2 = edge2_1->twin;
    CHECK_EQUAL_POINTS(edge2_1->cell->get_point0(), point1);
    CHECK_EQUAL_POINTS(edge2_2->cell->get_point0(), point2);

    voronoi_edge_type *edge3_1 = edge2_1->rot_prev;
    voronoi_edge_type *edge3_2 = edge3_1->twin;
    CHECK_EQUAL_POINTS(edge3_1->cell->get_point0(), point2);
    CHECK_EQUAL_POINTS(edge3_2->cell->get_point0(), point3);

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
    typedef typename voronoi_output_clipped<coordinate_type>::voronoi_edge_type voronoi_edge_type;
    typedef typename voronoi_output_clipped<coordinate_type>::voronoi_vertex_const_iterator_type
        voronoi_vertex_const_iterator_type;

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
    VERIFY_VORONOI_OUTPUT(test_output, COMPLETE_VERIFICATION);

    BOOST_CHECK_EQUAL(static_cast<int>(test_output.cell_records.size()), 3);
    BOOST_CHECK_EQUAL(static_cast<int>(test_output.vertex_records.size()), 1);

    voronoi_vertex_const_iterator_type it = test_output.vertex_records.begin();
    BOOST_CHECK_EQUAL(it->num_incident_edges, 3);
    BOOST_CHECK_EQUAL(it->vertex.x() == static_cast<coordinate_type>(1.75) &&
                      it->vertex.y() == static_cast<coordinate_type>(2.0), true);

    voronoi_edge_type *edge1_1 = it->incident_edge;
    voronoi_edge_type *edge1_2 = edge1_1->twin;
    CHECK_EQUAL_POINTS(edge1_1->cell->get_point0(), point2);
    CHECK_EQUAL_POINTS(edge1_2->cell->get_point0(), point1);

    voronoi_edge_type *edge2_1 = edge1_1->rot_prev;
    voronoi_edge_type *edge2_2 = edge2_1->twin;    
    CHECK_EQUAL_POINTS(edge2_1->cell->get_point0(), point1);
    CHECK_EQUAL_POINTS(edge2_2->cell->get_point0(), point3);

    voronoi_edge_type *edge3_1 = edge2_1->rot_prev;
    voronoi_edge_type *edge3_2 = edge3_1->twin;
    CHECK_EQUAL_POINTS(edge3_1->cell->get_point0(), point3);
    CHECK_EQUAL_POINTS(edge3_2->cell->get_point0(), point2);

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
BOOST_AUTO_TEST_CASE_TEMPLATE(square_test1, T, test_types) {
    typedef T coordinate_type;
    typedef typename voronoi_output_clipped<coordinate_type>::voronoi_edge_type voronoi_edge_type;
    typedef typename voronoi_output_clipped<coordinate_type>::voronoi_vertex_const_iterator_type
        voronoi_vertex_const_iterator_type;

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
    VERIFY_VORONOI_OUTPUT(test_output, COMPLETE_VERIFICATION);

    BOOST_CHECK_EQUAL(test_output.num_cell_records, 4);
    BOOST_CHECK_EQUAL(test_output.num_vertex_records, 1);
    BOOST_CHECK_EQUAL(test_output.num_edge_records, 4);

    // Check voronoi vertex.
    voronoi_vertex_const_iterator_type it = test_output.vertex_records.begin();
    BOOST_CHECK_EQUAL(it->num_incident_edges, 4);
    BOOST_CHECK_EQUAL(it->vertex.x() == static_cast<coordinate_type>(0.5) &&
                      it->vertex.y() == static_cast<coordinate_type>(0.5), true);

    // Check voronoi edges.
    voronoi_edge_type *edge1_1 = it->incident_edge;
    voronoi_edge_type *edge1_2 = edge1_1->twin;
    CHECK_EQUAL_POINTS(edge1_1->cell->get_point0(), points[1]);
    CHECK_EQUAL_POINTS(edge1_2->cell->get_point0(), points[3]);

    voronoi_edge_type *edge2_1 = edge1_1->rot_prev;
    voronoi_edge_type *edge2_2 = edge2_1->twin;    
    CHECK_EQUAL_POINTS(edge2_1->cell->get_point0(), points[3]);
    CHECK_EQUAL_POINTS(edge2_2->cell->get_point0(), points[2]);

    voronoi_edge_type *edge3_1 = edge2_1->rot_prev;
    voronoi_edge_type *edge3_2 = edge3_1->twin;
    CHECK_EQUAL_POINTS(edge3_1->cell->get_point0(), points[2]);
    CHECK_EQUAL_POINTS(edge3_2->cell->get_point0(), points[0]);

    voronoi_edge_type *edge4_1 = edge3_1->rot_prev;
    voronoi_edge_type *edge4_2 = edge4_1->twin;
    CHECK_EQUAL_POINTS(edge4_1->cell->get_point0(), points[0]);
    CHECK_EQUAL_POINTS(edge4_2->cell->get_point0(), points[1]);

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
    VERIFY_VORONOI_OUTPUT(test_output, COMPLETE_VERIFICATION);

    BOOST_CHECK_EQUAL(test_output.num_cell_records, 9);
    BOOST_CHECK_EQUAL(test_output.num_vertex_records, 4);
    BOOST_CHECK_EQUAL(test_output.num_edge_records, 12);
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
    VERIFY_VORONOI_OUTPUT(test_output, COMPLETE_VERIFICATION);

    BOOST_CHECK_EQUAL(test_output.num_cell_records, 100);
    BOOST_CHECK_EQUAL(test_output.num_vertex_records, 81);
    BOOST_CHECK_EQUAL(test_output.num_edge_records, 180);
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
    VERIFY_VORONOI_OUTPUT(test_output, COMPLETE_VERIFICATION);

    BOOST_CHECK_EQUAL(test_output.num_cell_records, 1089);
    BOOST_CHECK_EQUAL(test_output.num_vertex_records, 1024);
    BOOST_CHECK_EQUAL(test_output.num_edge_records, 2112);
}

#ifndef _DEBUG
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
    VERIFY_VORONOI_OUTPUT(test_output, COMPLETE_VERIFICATION);

    BOOST_CHECK_EQUAL(test_output.num_cell_records, 10000);
    BOOST_CHECK_EQUAL(test_output.num_vertex_records, 9801);
    BOOST_CHECK_EQUAL(test_output.num_edge_records, 19800);
}
#endif

#ifndef _DEBUG
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
    VERIFY_VORONOI_OUTPUT(test_output, COMPLETE_VERIFICATION);

    BOOST_CHECK_EQUAL(test_output.num_cell_records, 110889);
    BOOST_CHECK_EQUAL(test_output.num_vertex_records, 110224);
    BOOST_CHECK_EQUAL(test_output.num_edge_records, 221112);
}
#endif

#ifndef _DEBUG
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
                static_cast<coordinate_type>(rand() % 10 - 5),
                static_cast<coordinate_type>(rand() % 10 - 5)));
        test_beach_line.init(points);
        test_beach_line.run_sweepline();
        test_beach_line.clip(test_output);
        VERIFY_VORONOI_OUTPUT(test_output, COMPLETE_VERIFICATION);
        test_beach_line.reset();
    }
}
#endif

#ifndef _DEBUG
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
                static_cast<coordinate_type>(rand() % 100 - 50),
                static_cast<coordinate_type>(rand() % 100 - 50)));
        test_beach_line.init(points);
        test_beach_line.run_sweepline();
        test_beach_line.clip(test_output);
        VERIFY_VORONOI_OUTPUT(test_output, COMPLETE_VERIFICATION);
        test_beach_line.reset();
    }
}
#endif

#ifndef _DEBUG
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
            static_cast<coordinate_type>(rand() % 100 - 50),
            static_cast<coordinate_type>(rand() % 100 - 50)));
        test_beach_line.init(points);
        test_beach_line.run_sweepline();
        test_beach_line.clip(test_output);
        VERIFY_VORONOI_OUTPUT(test_output, COMPLETE_VERIFICATION);
        test_beach_line.reset();
    }
}
#endif

#ifndef _DEBUG
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
        points.push_back(make_point_2d<coordinate_type>(
            static_cast<coordinate_type>(rand() % 1000 - 500),
            static_cast<coordinate_type>(rand() % 1000 - 500)));
        test_beach_line.init(points);
        test_beach_line.run_sweepline();
        test_beach_line.clip(test_output);
        VERIFY_VORONOI_OUTPUT(test_output, COMPLETE_VERIFICATION);
        test_beach_line.reset();
    }
}
#endif

#ifndef _DEBUG
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
            static_cast<coordinate_type>(rand() % 1000 - 500),
            static_cast<coordinate_type>(rand() % 1000 - 500)));
        test_beach_line.init(points);
        test_beach_line.run_sweepline();
        test_beach_line.clip(test_output);
        VERIFY_VORONOI_OUTPUT(test_output, COMPLETE_VERIFICATION);
        test_beach_line.reset();
    }
}
#endif

#ifndef _DEBUG
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
            static_cast<coordinate_type>(rand() % 10000 - 5000),
            static_cast<coordinate_type>(rand() % 10000 - 5000)));
        test_beach_line.init(points);
        test_beach_line.run_sweepline();
        test_beach_line.clip(test_output);
        VERIFY_VORONOI_OUTPUT(test_output, FAST_VERIFICATION);
        test_beach_line.reset();
    }
}
#endif

#ifndef _DEBUG
BOOST_AUTO_TEST_CASE_TEMPLATE(random_test7, T, test_types) {
    typedef typename voronoi_output_clipped<T>::voronoi_vertex_const_iterator_type
        voronoi_vertex_const_iterator_type;
    srand(static_cast<unsigned int>(time(NULL)));
    voronoi_builder<T> test_builder;
    voronoi_output_clipped<T> test_output_small, test_output_large;
    std::vector< point_2d<T> > points_small, points_large;
    int num_points = 27;
    int md = 20;
    int half_md = md >> 1;
    int koef = std::numeric_limits<int>::max() / half_md;
    for (int i = 0; i < 10000; i++) {
        points_small.clear();
        points_large.clear();
        for (int j = 0; j < num_points; j++) {
            T x_small = static_cast<T>(rand() % md - half_md);
            T y_small = static_cast<T>(rand() % md - half_md);
            T x_large = x_small * koef;
            T y_large = y_small * koef;
            points_small.push_back(make_point_2d(x_small, y_small));
            points_large.push_back(make_point_2d(x_large, y_large));
        }
        test_builder.init(points_small);
        test_builder.run_sweepline();
        test_builder.clip(test_output_small);
        test_builder.init(points_large);
        test_builder.run_sweepline();
        test_builder.clip(test_output_large);
        VERIFY_VORONOI_OUTPUT(test_output_small, COMPLETE_VERIFICATION);
        VERIFY_VORONOI_OUTPUT(test_output_large, COMPLETE_VERIFICATION);
        BOOST_CHECK_EQUAL(test_output_small.num_cell_records, test_output_large.num_cell_records);
        BOOST_CHECK_EQUAL(test_output_small.num_vertex_records, test_output_large.num_vertex_records);
        BOOST_CHECK_EQUAL(test_output_small.num_edge_records, test_output_large.num_edge_records);
        test_builder.reset();
    }
}
#endif

BOOST_AUTO_TEST_CASE_TEMPLATE(segment_sites_test1, T, test_types) {
    typedef T coordinate_type;
    voronoi_builder<coordinate_type> test_voronoi_builder;
    std::vector< typename voronoi_builder<coordinate_type>::Segment2D > segm_vec;
    point_2d<T> point1 = make_point_2d<T>(0, 0);
    point_2d<T> point2 = make_point_2d<T>(1, 1);
    segm_vec.push_back(std::make_pair< point_2d<T>, point_2d<T> >(point1, point2));
    test_voronoi_builder.init(segm_vec);
    test_voronoi_builder.run_sweepline();

	// TODO(asydorchuk): Add output checks there.
}

BOOST_AUTO_TEST_CASE_TEMPLATE(segment_sites_test2, T, test_types) {
    typedef T coordinate_type;
    voronoi_builder<coordinate_type> test_voronoi_builder;
    std::vector< point_2d<T> > point_vec;
    std::vector< typename voronoi_builder<coordinate_type>::Segment2D > segm_vec;
    point_2d<T> point1 = make_point_2d<T>(0, 0);
    point_2d<T> point2 = make_point_2d<T>(4, 4);
    point_2d<T> point3 = make_point_2d<T>(3, 1);
    point_2d<T> point4 = make_point_2d<T>(1, 3);
    segm_vec.push_back(std::make_pair< point_2d<T>, point_2d<T> >(point1, point2));
    point_vec.push_back(point3);
    point_vec.push_back(point4);
    test_voronoi_builder.init(point_vec, segm_vec);
    test_voronoi_builder.run_sweepline();
    // TODO(asydorchuk): Add output checks there.
}

BOOST_AUTO_TEST_CASE_TEMPLATE(segment_sites_test3, T, test_types) {
    typedef T coordinate_type;
    voronoi_builder<coordinate_type> test_voronoi_builder;
    std::vector< point_2d<T> > point_vec;
    std::vector< typename voronoi_builder<coordinate_type>::Segment2D > segm_vec;
    point_2d<T> point1 = make_point_2d<T>(4, 0);
    point_2d<T> point2 = make_point_2d<T>(0, 4);
    point_2d<T> point3 = make_point_2d<T>(3, 3);
    point_2d<T> point4 = make_point_2d<T>(1, 1);
    segm_vec.push_back(std::make_pair< point_2d<T>, point_2d<T> >(point1, point2));
    point_vec.push_back(point3);
    point_vec.push_back(point4);
    test_voronoi_builder.init(point_vec, segm_vec);
    test_voronoi_builder.run_sweepline();

    // TODO(asydorchuk): Add output checks there.
}

BOOST_AUTO_TEST_CASE_TEMPLATE(segment_sites_test4, T, test_types) {
    typedef T coordinate_type;
    voronoi_builder<coordinate_type> test_voronoi_builder;
    std::vector< point_2d<T> > point_vec;
    std::vector< typename voronoi_builder<coordinate_type>::Segment2D > segm_vec;
    point_2d<T> point1 = make_point_2d<T>(4, 0);
    point_2d<T> point2 = make_point_2d<T>(0, 4);
    point_2d<T> point3 = make_point_2d<T>(3, 2);
    point_2d<T> point4 = make_point_2d<T>(2, 3);
    segm_vec.push_back(std::make_pair< point_2d<T>, point_2d<T> >(point1, point2));
    point_vec.push_back(point3);
    point_vec.push_back(point4);
    test_voronoi_builder.init(point_vec, segm_vec);
    test_voronoi_builder.run_sweepline();

    // TODO(asydorchuk): Add output checks there.
}

BOOST_AUTO_TEST_CASE_TEMPLATE(segment_site_test5, T, test_types) {
    typedef T coordinate_type;
    voronoi_builder<coordinate_type> test_voronoi_builder;
    std::vector< point_2d<T> > point_vec;
    std::vector< typename voronoi_builder<coordinate_type>::Segment2D > segm_vec;
    point_2d<T> point1 = make_point_2d<T>(0, 0);
    point_2d<T> point2 = make_point_2d<T>(0, 8);
    point_2d<T> point3 = make_point_2d<T>(-2, -2);
    point_2d<T> point4 = make_point_2d<T>(-2, 4);
    point_2d<T> point5 = make_point_2d<T>(-2, 10);
    segm_vec.push_back(std::make_pair< point_2d<T>, point_2d<T> >(point1, point2));
    point_vec.push_back(point3);
    point_vec.push_back(point4);
    point_vec.push_back(point5);
    test_voronoi_builder.init(point_vec, segm_vec);
    test_voronoi_builder.run_sweepline();

    // TODO(asydorchuk): Add output checks there.
}

BOOST_AUTO_TEST_CASE_TEMPLATE(segment_site_test6, T, test_types) {
    typedef T coordinate_type;
    voronoi_builder<coordinate_type> test_voronoi_builder;
    std::vector< point_2d<T> > point_vec;
    std::vector< typename voronoi_builder<coordinate_type>::Segment2D > segm_vec;
    point_2d<T> point1 = make_point_2d<T>(-1, 1);
    point_2d<T> point2 = make_point_2d<T>(1, 0);
    point_2d<T> point3 = make_point_2d<T>(1, 2);
    segm_vec.push_back(std::make_pair< point_2d<T>, point_2d<T> >(point2, point3));
    point_vec.push_back(point1);
    test_voronoi_builder.init(point_vec, segm_vec);
    test_voronoi_builder.run_sweepline();

	// TODO(asydorchuk): Add output checks there.
}

BOOST_AUTO_TEST_CASE_TEMPLATE(segment_site_test7, T, test_types) {
    typedef T coordinate_type;
    voronoi_builder<coordinate_type> test_voronoi_builder;
    std::vector< point_2d<T> > point_vec;
    std::vector< typename voronoi_builder<coordinate_type>::Segment2D > segm_vec;
    point_2d<T> point1 = make_point_2d<T>(0, 0);
    point_2d<T> point2 = make_point_2d<T>(4, 0);
    point_2d<T> point3 = make_point_2d<T>(0, 4);
    point_2d<T> point4 = make_point_2d<T>(4, 4);
    segm_vec.push_back(std::make_pair(point1, point2));
    segm_vec.push_back(std::make_pair(point2, point3));
    segm_vec.push_back(std::make_pair(point3, point4));
    test_voronoi_builder.init(segm_vec);
    test_voronoi_builder.run_sweepline();

	// TODO(asydorchuk): Add output checks there.
}

BOOST_AUTO_TEST_CASE_TEMPLATE(segment_site_test8, T, test_types) {
    typedef T coordinate_type;
    voronoi_builder<coordinate_type> test_voronoi_builder;
    std::vector< point_2d<T> > point_vec;
    std::vector< typename voronoi_builder<coordinate_type>::Segment2D > segm_vec;
    point_2d<T> point1 = make_point_2d<T>(0, 0);
    point_2d<T> point2 = make_point_2d<T>(4, 0);
    point_2d<T> point3 = make_point_2d<T>(4, 4);
    point_2d<T> point4 = make_point_2d<T>(0, 4);
    segm_vec.push_back(std::make_pair(point1, point2));
    segm_vec.push_back(std::make_pair(point2, point3));
    segm_vec.push_back(std::make_pair(point3, point4));
    segm_vec.push_back(std::make_pair(point4, point1));
    test_voronoi_builder.init(segm_vec);
    test_voronoi_builder.run_sweepline();

	// TODO(asydorchuk): Add output checks there.
}

// Boost sweepline library builder_test.cpp file

//          Copyright Andrii Sydorchuk 2010.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

// See http://www.boost.org for updates, documentation, and revision history.

#include <stdlib.h>
#include <time.h>

#include "test_type_list.hpp"
#include "boost/sweepline/voronoi_sweepline.hpp"
using namespace boost::sweepline;
#include "output_verification.hpp"

#define BOOST_TEST_MODULE voronoi_builder_test
#include <boost/test/test_case_template.hpp>

#define CHECK_EQUAL_POINTS(p1, p2) \
        BOOST_CHECK_EQUAL(p1.x() == static_cast<T>(p2.x()) && \
                          p1.y() == static_cast<T>(p2.y()), true)

#define VERIFY_VORONOI_OUTPUT(output, mask) BOOST_CHECK_EQUAL(verify_output(output, mask), true)

#define ALMOST_EQUAL_TEST(a, b, ULP_ERR, ABS_ERR) \
        BOOST_CHECK_EQUAL(detail::almost_equal(a, b, ULP_ERR) || \
                          detail::abs_equal(a, b, ABS_ERR), true);

// Sites: (0, 0).
BOOST_AUTO_TEST_CASE_TEMPLATE(single_site_test, T, test_types) {
    typedef T coordinate_type;
    typedef typename voronoi_output<coordinate_type>::voronoi_cell_const_iterator_type
        voronoi_cell_const_iterator_type;

    std::vector< point_2d<coordinate_type> > points;
    points.push_back(make_point_2d<coordinate_type>(static_cast<coordinate_type>(0),
                                                    static_cast<coordinate_type>(0)));
    voronoi_output<coordinate_type> test_output;
    build_voronoi(points, test_output);
    VERIFY_VORONOI_OUTPUT(test_output, COMPLETE_VERIFICATION);

    BRect<coordinate_type> bounding_rectangle = test_output.bounding_rectangle();
    BOOST_CHECK_EQUAL(bounding_rectangle.x_min() == static_cast<coordinate_type>(0) &&
                      bounding_rectangle.y_min() == static_cast<coordinate_type>(0) &&
                      bounding_rectangle.x_max() == static_cast<coordinate_type>(0) &&
                      bounding_rectangle.y_max() == static_cast<coordinate_type>(0), true);

    BOOST_CHECK_EQUAL(static_cast<int>(test_output.cell_records().size()), 1);
    BOOST_CHECK_EQUAL(static_cast<int>(test_output.vertex_records().size()), 0);
    BOOST_CHECK_EQUAL(static_cast<int>(test_output.edge_records().size()), 0);

    BOOST_CHECK_EQUAL(test_output.num_cell_records(), 1);
    BOOST_CHECK_EQUAL(test_output.num_vertex_records(), 0);
    BOOST_CHECK_EQUAL(test_output.num_edge_records(), 0);

    voronoi_cell_const_iterator_type it = test_output.cell_records().begin();
    BOOST_CHECK_EQUAL(it->num_incident_edges(), 0);
    BOOST_CHECK_EQUAL(it->incident_edge() == NULL, true);
}

// Sites: (0, 0), (0, 1).
BOOST_AUTO_TEST_CASE_TEMPLATE(collinear_sites_test1, T, test_types) {
    typedef T coordinate_type;
    typedef typename voronoi_output<coordinate_type>::voronoi_edge_type voronoi_edge_type;
    typedef typename voronoi_output<coordinate_type>::voronoi_cell_const_iterator_type
        voronoi_cell_const_iterator_type;

    std::vector< point_2d<coordinate_type> > points;
    points.push_back(make_point_2d<coordinate_type>(static_cast<coordinate_type>(0),
                                                    static_cast<coordinate_type>(0)));
    points.push_back(make_point_2d<coordinate_type>(static_cast<coordinate_type>(0),
                                                    static_cast<coordinate_type>(1)));
    voronoi_output<coordinate_type> test_output;
    build_voronoi(points, test_output);
    VERIFY_VORONOI_OUTPUT(test_output, COMPLETE_VERIFICATION);

    BRect<coordinate_type> bounding_rectangle = test_output.bounding_rectangle();
    BOOST_CHECK_EQUAL(bounding_rectangle.x_min() == static_cast<coordinate_type>(0) &&
                      bounding_rectangle.y_min() == static_cast<coordinate_type>(0) &&
                      bounding_rectangle.x_max() == static_cast<coordinate_type>(0) &&
                      bounding_rectangle.y_max() == static_cast<coordinate_type>(1), true);

    BOOST_CHECK_EQUAL(static_cast<int>(test_output.cell_records().size()), 2);
    BOOST_CHECK_EQUAL(static_cast<int>(test_output.vertex_records().size()), 0);
    BOOST_CHECK_EQUAL(static_cast<int>(test_output.edge_records().size()), 2);

    BOOST_CHECK_EQUAL(test_output.num_cell_records(), 2);
    BOOST_CHECK_EQUAL(test_output.num_vertex_records(), 0);
    BOOST_CHECK_EQUAL(test_output.num_edge_records(), 1);

    voronoi_cell_const_iterator_type cell_it = test_output.cell_records().begin();
    BOOST_CHECK_EQUAL(cell_it->num_incident_edges(), 1);
    cell_it++;
    BOOST_CHECK_EQUAL(cell_it->num_incident_edges(), 1);

    const voronoi_edge_type *edge1_1 = cell_it->incident_edge();
    const voronoi_edge_type *edge1_2 = edge1_1->twin();

    BOOST_CHECK_EQUAL(edge1_1->twin() == edge1_2, true);
    BOOST_CHECK_EQUAL(edge1_2->twin() == edge1_1, true);

    BOOST_CHECK_EQUAL(edge1_1->next() == edge1_1, true);
    BOOST_CHECK_EQUAL(edge1_1->prev() == edge1_1, true);
    BOOST_CHECK_EQUAL(edge1_1->rot_next() == NULL, true);
    BOOST_CHECK_EQUAL(edge1_1->rot_prev() == NULL, true);

    BOOST_CHECK_EQUAL(edge1_2->next() == edge1_2, true);
    BOOST_CHECK_EQUAL(edge1_2->prev() == edge1_2, true);
    BOOST_CHECK_EQUAL(edge1_2->rot_next() == NULL, true);
    BOOST_CHECK_EQUAL(edge1_2->rot_prev() == NULL, true);
}

// Sites: (0, 0), (1, 1), (2, 2).
BOOST_AUTO_TEST_CASE_TEMPLATE(collinear_sites_test2, T, test_types) {
    typedef T coordinate_type;
    typedef typename voronoi_output<coordinate_type>::voronoi_edge_type voronoi_edge_type;
    typedef typename voronoi_output<coordinate_type>::voronoi_cell_const_iterator_type
        voronoi_cell_const_iterator_type;

    std::vector< point_2d<coordinate_type> > points;
    points.push_back(make_point_2d<coordinate_type>(static_cast<coordinate_type>(0),
                                                    static_cast<coordinate_type>(0)));
    points.push_back(make_point_2d<coordinate_type>(static_cast<coordinate_type>(1),
                                                    static_cast<coordinate_type>(1)));
    points.push_back(make_point_2d<coordinate_type>(static_cast<coordinate_type>(2),
                                                    static_cast<coordinate_type>(2)));
    voronoi_output<coordinate_type> test_output;
    build_voronoi(points, test_output);
    VERIFY_VORONOI_OUTPUT(test_output, COMPLETE_VERIFICATION);

    BRect<coordinate_type> bounding_rectangle = test_output.bounding_rectangle();
    BOOST_CHECK_EQUAL(bounding_rectangle.x_min() == static_cast<coordinate_type>(0) &&
                      bounding_rectangle.y_min() == static_cast<coordinate_type>(0) &&
                      bounding_rectangle.x_max() == static_cast<coordinate_type>(2) &&
                      bounding_rectangle.y_max() == static_cast<coordinate_type>(2), true);

    BOOST_CHECK_EQUAL(static_cast<int>(test_output.cell_records().size()), 3);
    BOOST_CHECK_EQUAL(static_cast<int>(test_output.vertex_records().size()), 0);
    BOOST_CHECK_EQUAL(static_cast<int>(test_output.edge_records().size()), 4);

    BOOST_CHECK_EQUAL(test_output.num_cell_records(), 3);
    BOOST_CHECK_EQUAL(test_output.num_vertex_records(), 0);
    BOOST_CHECK_EQUAL(test_output.num_edge_records(), 2);

    voronoi_cell_const_iterator_type cell_it = test_output.cell_records().begin();
    BOOST_CHECK_EQUAL(cell_it->num_incident_edges(), 1);
    const voronoi_edge_type *edge1_1 = cell_it->incident_edge();
    const voronoi_edge_type *edge1_2 = edge1_1->twin();
    cell_it++;
    BOOST_CHECK_EQUAL(cell_it->num_incident_edges(), 2);
    cell_it++;
    BOOST_CHECK_EQUAL(cell_it->num_incident_edges(), 1);
    const voronoi_edge_type *edge2_2 = cell_it->incident_edge();
    const voronoi_edge_type *edge2_1 = edge2_2->twin();

    BOOST_CHECK_EQUAL(edge1_1->twin() == edge1_2 && edge1_2->twin() == edge1_1, true);
    BOOST_CHECK_EQUAL(edge2_1->twin() == edge2_2 && edge2_2->twin() == edge2_1, true);

    BOOST_CHECK_EQUAL(edge1_1->next() == edge1_1 && edge1_1->prev() == edge1_1, true);
    BOOST_CHECK_EQUAL(edge1_1->rot_next() == NULL && edge1_1->rot_prev() == NULL, true);
    BOOST_CHECK_EQUAL(edge1_2->rot_next() == NULL && edge1_2->rot_prev() == NULL, true);
    BOOST_CHECK_EQUAL(edge2_1->rot_next() == NULL && edge2_1->rot_prev() == NULL, true);
    BOOST_CHECK_EQUAL(edge2_2->next() == edge2_2 && edge2_2->prev() == edge2_2, true);
    BOOST_CHECK_EQUAL(edge2_2->rot_next() == NULL && edge2_2->rot_prev() == NULL, true);

    BOOST_CHECK_EQUAL(edge1_2->next() == edge2_1 && edge1_2->prev() == edge2_1, true);
    BOOST_CHECK_EQUAL(edge2_1->next() == edge1_2 && edge2_1->prev() == edge1_2, true);
}

// Sites: (0, 0), (0, 4), (2, 1).
BOOST_AUTO_TEST_CASE_TEMPLATE(triangle_test1, T, test_types) {
    typedef T coordinate_type;
    typedef typename voronoi_output<coordinate_type>::voronoi_edge_type voronoi_edge_type;
    typedef typename voronoi_output<coordinate_type>::voronoi_vertex_const_iterator_type
        voronoi_vertex_const_iterator_type;

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

    voronoi_output<coordinate_type> test_output;
    build_voronoi(points, test_output);
    VERIFY_VORONOI_OUTPUT(test_output, COMPLETE_VERIFICATION);

    BRect<coordinate_type> bounding_rectangle = test_output.bounding_rectangle();
    BOOST_CHECK_EQUAL(bounding_rectangle.x_min() == static_cast<coordinate_type>(0) &&
                      bounding_rectangle.y_min() == static_cast<coordinate_type>(0) &&
                      bounding_rectangle.x_max() == static_cast<coordinate_type>(2) &&
                      bounding_rectangle.y_max() == static_cast<coordinate_type>(4), true);

    BOOST_CHECK_EQUAL(static_cast<int>(test_output.cell_records().size()), 3);
    BOOST_CHECK_EQUAL(static_cast<int>(test_output.vertex_records().size()), 1);

    voronoi_vertex_const_iterator_type it = test_output.vertex_records().begin();
    BOOST_CHECK_EQUAL(it->num_incident_edges(), 3);
    BOOST_CHECK_EQUAL(it->vertex().x() == static_cast<coordinate_type>(0.25) &&
                      it->vertex().y() == static_cast<coordinate_type>(2.0), true);

    const voronoi_edge_type *edge1_1 = it->incident_edge();
    const voronoi_edge_type *edge1_2 = edge1_1->twin();
    CHECK_EQUAL_POINTS(edge1_1->cell()->point0(), point3);
    CHECK_EQUAL_POINTS(edge1_2->cell()->point0(), point1);

    const voronoi_edge_type *edge2_1 = edge1_1->rot_prev();
    const voronoi_edge_type *edge2_2 = edge2_1->twin();
    CHECK_EQUAL_POINTS(edge2_1->cell()->point0(), point1);
    CHECK_EQUAL_POINTS(edge2_2->cell()->point0(), point2);

    const voronoi_edge_type *edge3_1 = edge2_1->rot_prev();
    const voronoi_edge_type *edge3_2 = edge3_1->twin();
    CHECK_EQUAL_POINTS(edge3_1->cell()->point0(), point2);
    CHECK_EQUAL_POINTS(edge3_2->cell()->point0(), point3);

    BOOST_CHECK_EQUAL(edge1_2->twin() == edge1_1, true);
    BOOST_CHECK_EQUAL(edge2_2->twin() == edge2_1, true);
    BOOST_CHECK_EQUAL(edge3_2->twin() == edge3_1, true);

    BOOST_CHECK_EQUAL(edge1_1->prev() == edge3_2 && edge1_1->next() == edge3_2, true);
    BOOST_CHECK_EQUAL(edge2_1->prev() == edge1_2 && edge2_1->next() == edge1_2, true);
    BOOST_CHECK_EQUAL(edge3_1->prev() == edge2_2 && edge3_1->next() == edge2_2, true);

    BOOST_CHECK_EQUAL(edge1_2->next() == edge2_1 && edge1_2->prev() == edge2_1, true);
    BOOST_CHECK_EQUAL(edge2_2->next() == edge3_1 && edge2_2->prev() == edge3_1, true);
    BOOST_CHECK_EQUAL(edge3_2->next() == edge1_1 && edge3_2->prev() == edge1_1, true);

    BOOST_CHECK_EQUAL(edge1_1->rot_next() == edge3_1, true);
    BOOST_CHECK_EQUAL(edge3_1->rot_next() == edge2_1, true);
    BOOST_CHECK_EQUAL(edge2_1->rot_next() == edge1_1, true);
}

// Sites: (0, 1), (2, 0), (2, 4).
BOOST_AUTO_TEST_CASE_TEMPLATE(triangle_test2, T, test_types) {
    typedef T coordinate_type;
    typedef typename voronoi_output<coordinate_type>::voronoi_edge_type voronoi_edge_type;
    typedef typename voronoi_output<coordinate_type>::voronoi_vertex_const_iterator_type
        voronoi_vertex_const_iterator_type;

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

    voronoi_output<coordinate_type> test_output;
    build_voronoi(points, test_output);
    VERIFY_VORONOI_OUTPUT(test_output, COMPLETE_VERIFICATION);

    BOOST_CHECK_EQUAL(static_cast<int>(test_output.cell_records().size()), 3);
    BOOST_CHECK_EQUAL(static_cast<int>(test_output.vertex_records().size()), 1);

    voronoi_vertex_const_iterator_type it = test_output.vertex_records().begin();
    BOOST_CHECK_EQUAL(it->num_incident_edges(), 3);
    BOOST_CHECK_EQUAL(it->vertex().x() == static_cast<coordinate_type>(1.75) &&
                      it->vertex().y() == static_cast<coordinate_type>(2.0), true);

    const voronoi_edge_type *edge1_1 = it->incident_edge();
    const voronoi_edge_type *edge1_2 = edge1_1->twin();
    CHECK_EQUAL_POINTS(edge1_1->cell()->point0(), point2);
    CHECK_EQUAL_POINTS(edge1_2->cell()->point0(), point1);

    const voronoi_edge_type *edge2_1 = edge1_1->rot_prev();
    const voronoi_edge_type *edge2_2 = edge2_1->twin();
    CHECK_EQUAL_POINTS(edge2_1->cell()->point0(), point1);
    CHECK_EQUAL_POINTS(edge2_2->cell()->point0(), point3);

    const voronoi_edge_type *edge3_1 = edge2_1->rot_prev();
    const voronoi_edge_type *edge3_2 = edge3_1->twin();
    CHECK_EQUAL_POINTS(edge3_1->cell()->point0(), point3);
    CHECK_EQUAL_POINTS(edge3_2->cell()->point0(), point2);

    BOOST_CHECK_EQUAL(edge1_2->twin() == edge1_1, true);
    BOOST_CHECK_EQUAL(edge2_2->twin() == edge2_1, true);
    BOOST_CHECK_EQUAL(edge3_2->twin() == edge3_1, true);

    BOOST_CHECK_EQUAL(edge1_1->prev() == edge3_2 && edge1_1->next() == edge3_2, true);
    BOOST_CHECK_EQUAL(edge2_1->prev() == edge1_2 && edge2_1->next() == edge1_2, true);
    BOOST_CHECK_EQUAL(edge3_1->prev() == edge2_2 && edge3_1->next() == edge2_2, true);

    BOOST_CHECK_EQUAL(edge1_2->next() == edge2_1 && edge1_2->prev() == edge2_1, true);
    BOOST_CHECK_EQUAL(edge2_2->next() == edge3_1 && edge2_2->prev() == edge3_1, true);
    BOOST_CHECK_EQUAL(edge3_2->next() == edge1_1 && edge3_2->prev() == edge1_1, true);

    BOOST_CHECK_EQUAL(edge1_1->rot_next() == edge3_1, true);
    BOOST_CHECK_EQUAL(edge3_1->rot_next() == edge2_1, true);
    BOOST_CHECK_EQUAL(edge2_1->rot_next() == edge1_1, true);
}

// Sites: (0, 0), (0, 1), (1, 0), (1, 1).
BOOST_AUTO_TEST_CASE_TEMPLATE(square_test1, T, test_types) {
    typedef T coordinate_type;
    typedef typename voronoi_output<coordinate_type>::voronoi_edge_type voronoi_edge_type;
    typedef typename voronoi_output<coordinate_type>::voronoi_vertex_const_iterator_type
        voronoi_vertex_const_iterator_type;

    std::vector< point_2d<coordinate_type> > points;
    points.push_back(make_point_2d<coordinate_type>(static_cast<coordinate_type>(0),
                                                    static_cast<coordinate_type>(0)));
    points.push_back(make_point_2d<coordinate_type>(static_cast<coordinate_type>(0),
                                                    static_cast<coordinate_type>(1)));
    points.push_back(make_point_2d<coordinate_type>(static_cast<coordinate_type>(1),
                                                    static_cast<coordinate_type>(0)));
    points.push_back(make_point_2d<coordinate_type>(static_cast<coordinate_type>(1),
                                                    static_cast<coordinate_type>(1)));

    voronoi_output<coordinate_type> test_output;
    build_voronoi(points, test_output);
    VERIFY_VORONOI_OUTPUT(test_output, COMPLETE_VERIFICATION);

    BOOST_CHECK_EQUAL(test_output.num_cell_records(), 4);
    BOOST_CHECK_EQUAL(test_output.num_vertex_records(), 1);
    BOOST_CHECK_EQUAL(test_output.num_edge_records(), 4);

    // Check voronoi vertex.
    voronoi_vertex_const_iterator_type it = test_output.vertex_records().begin();
    BOOST_CHECK_EQUAL(it->num_incident_edges(), 4);
    BOOST_CHECK_EQUAL(it->vertex().x() == static_cast<coordinate_type>(0.5) &&
                      it->vertex().y() == static_cast<coordinate_type>(0.5), true);

    // Check voronoi edges.
    const voronoi_edge_type *edge1_1 = it->incident_edge();
    const voronoi_edge_type *edge1_2 = edge1_1->twin();
    CHECK_EQUAL_POINTS(edge1_1->cell()->point0(), points[1]);
    CHECK_EQUAL_POINTS(edge1_2->cell()->point0(), points[3]);

    const voronoi_edge_type *edge2_1 = edge1_1->rot_prev();
    const voronoi_edge_type *edge2_2 = edge2_1->twin();
    CHECK_EQUAL_POINTS(edge2_1->cell()->point0(), points[3]);
    CHECK_EQUAL_POINTS(edge2_2->cell()->point0(), points[2]);

    const voronoi_edge_type *edge3_1 = edge2_1->rot_prev();
    const voronoi_edge_type *edge3_2 = edge3_1->twin();
    CHECK_EQUAL_POINTS(edge3_1->cell()->point0(), points[2]);
    CHECK_EQUAL_POINTS(edge3_2->cell()->point0(), points[0]);

    const voronoi_edge_type *edge4_1 = edge3_1->rot_prev();
    const voronoi_edge_type *edge4_2 = edge4_1->twin();
    CHECK_EQUAL_POINTS(edge4_1->cell()->point0(), points[0]);
    CHECK_EQUAL_POINTS(edge4_2->cell()->point0(), points[1]);

    BOOST_CHECK_EQUAL(edge1_2->twin() == edge1_1, true);
    BOOST_CHECK_EQUAL(edge2_2->twin() == edge2_1, true);
    BOOST_CHECK_EQUAL(edge3_2->twin() == edge3_1, true);
    BOOST_CHECK_EQUAL(edge4_2->twin() == edge4_1, true);

    BOOST_CHECK_EQUAL(edge1_1->prev() == edge4_2 && edge1_1->next() == edge4_2, true);
    BOOST_CHECK_EQUAL(edge2_1->prev() == edge1_2 && edge2_1->next() == edge1_2, true);
    BOOST_CHECK_EQUAL(edge3_1->prev() == edge2_2 && edge3_1->next() == edge2_2, true);
    BOOST_CHECK_EQUAL(edge4_1->prev() == edge3_2 && edge4_1->next() == edge3_2, true);

    BOOST_CHECK_EQUAL(edge1_2->next() == edge2_1 && edge1_2->prev() == edge2_1, true);
    BOOST_CHECK_EQUAL(edge2_2->next() == edge3_1 && edge2_2->prev() == edge3_1, true);
    BOOST_CHECK_EQUAL(edge3_2->next() == edge4_1 && edge3_2->prev() == edge4_1, true);
    BOOST_CHECK_EQUAL(edge4_2->next() == edge1_1 && edge4_2->prev() == edge1_1, true);

    BOOST_CHECK_EQUAL(edge1_1->rot_next() == edge4_1, true);
    BOOST_CHECK_EQUAL(edge4_1->rot_next() == edge3_1, true);
    BOOST_CHECK_EQUAL(edge3_1->rot_next() == edge2_1, true);
    BOOST_CHECK_EQUAL(edge2_1->rot_next() == edge1_1, true);
}

#ifdef NDEBUG
BOOST_AUTO_TEST_CASE_TEMPLATE(grid_test, T, test_types) {
    voronoi_output<T> test_output_small, test_output_large;
    std::vector< point_2d<T> > point_vec_small, point_vec_large;
    int grid_size[4] = {10, 33, 100, 333};
    int max_value[4] = {10, 33, 100, 333};
    int array_length = sizeof(grid_size) / sizeof(int);
    for (int k = 0; k < array_length; k++) {
        int koef = std::numeric_limits<int>::max() / max_value[k];
        for (int i = 0; i < grid_size[k]; i++)
            for (int j = 0; j < grid_size[k]; j++) {
                point_vec_small.push_back(make_point_2d<T>(i, j));
                point_vec_large.push_back(make_point_2d<T>(koef * i, koef * j));
            }
        build_voronoi(point_vec_small, test_output_small);
        build_voronoi(point_vec_large, test_output_large);
        VERIFY_VORONOI_OUTPUT(test_output_small, COMPLETE_VERIFICATION);
        VERIFY_VORONOI_OUTPUT(test_output_large, COMPLETE_VERIFICATION);
        BOOST_CHECK_EQUAL(test_output_small.num_cell_records(), test_output_large.num_cell_records());
        BOOST_CHECK_EQUAL(test_output_small.num_vertex_records(), test_output_large.num_vertex_records());
        BOOST_CHECK_EQUAL(test_output_small.num_edge_records(), test_output_large.num_edge_records());
        point_vec_small.clear();
        point_vec_large.clear();
    }
}
#endif

#ifdef NDEBUG
BOOST_AUTO_TEST_CASE_TEMPLATE(random_test, T, test_types) {
    srand(static_cast<unsigned int>(time(NULL)));
    voronoi_output<T> test_output_small, test_output_large;
    std::vector< point_2d<T> > point_vec_small, point_vec_large;
    int num_points[] = {10, 100, 1000, 10000, 100000};
    int num_runs[] = {10000, 1000, 100, 10, 1};
    int mod_koef[] = {10, 100, 100, 1000, 10000};
    int max_value[] = {5, 50, 50, 5000, 5000};
    int array_length = sizeof(num_points) / sizeof(int);
    for (int k = 0; k < array_length; k++) {
        int koef = std::numeric_limits<int>::max() / max_value[k];
        for (int i = 0; i < num_runs[k]; i++) {
            for (int j = 0; j < num_points[k]; j++) {
                T x = rand() % mod_koef[k] - mod_koef[k] / 2;
                T y = rand() % mod_koef[k] - mod_koef[k] / 2;
                point_vec_small.push_back(make_point_2d<T>(x, y));
                point_vec_large.push_back(make_point_2d<T>(koef * x, koef * y));
            }
            build_voronoi(point_vec_small, test_output_small);
            build_voronoi(point_vec_large, test_output_large);
            VERIFY_VORONOI_OUTPUT(test_output_small, COMPLETE_VERIFICATION);
            VERIFY_VORONOI_OUTPUT(test_output_large, COMPLETE_VERIFICATION);
            BOOST_CHECK_EQUAL(test_output_small.num_cell_records(),
                              test_output_large.num_cell_records());
            BOOST_CHECK_EQUAL(test_output_small.num_vertex_records(),
                              test_output_large.num_vertex_records());
            BOOST_CHECK_EQUAL(test_output_small.num_edge_records(),
                              test_output_large.num_edge_records());
            point_vec_small.clear();
            point_vec_large.clear();
        }
    }
}
#endif

#ifdef NDEBUG
BOOST_AUTO_TEST_CASE_TEMPLATE(enormous_random_test, T, test_types) {
    srand(static_cast<unsigned int>(time(NULL)));
    voronoi_output<T> test_output;
    std::vector< point_2d<T> > point_vec;
    for (int i = 0; i < 1000000; i++)
        point_vec.push_back(make_point_2d<T>(rand() % 10000 - 5000, rand() % 10000 - 5000));
    build_voronoi(point_vec, test_output);
    VERIFY_VORONOI_OUTPUT(test_output, FAST_VERIFICATION);
}
#endif

BOOST_AUTO_TEST_CASE_TEMPLATE(segment_sites_test1, T, test_types) {
    typedef T coordinate_type;
    voronoi_output<coordinate_type> test_output;
    std::vector< std::pair< point_2d<T>, point_2d<T> > > segm_vec;
    point_2d<T> point1 = make_point_2d<T>(0, 0);
    point_2d<T> point2 = make_point_2d<T>(1, 1);
    segm_vec.push_back(std::make_pair< point_2d<T>, point_2d<T> >(point1, point2));
    build_voronoi(segm_vec, test_output);
    BOOST_CHECK_EQUAL(test_output.num_cell_records(), 3);
    BOOST_CHECK_EQUAL(test_output.num_vertex_records(), 0);
    BOOST_CHECK_EQUAL(test_output.num_edge_records(), 2);
    VERIFY_VORONOI_OUTPUT(test_output, NO_HALF_EDGE_INTERSECTIONS);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(segment_sites_test2, T, test_types) {
    typedef T coordinate_type;
    voronoi_output<coordinate_type> test_output;
    std::vector< point_2d<T> > point_vec;
    std::vector< std::pair< point_2d<T>, point_2d<T> > > segm_vec;
    point_2d<T> point1 = make_point_2d<T>(0, 0);
    point_2d<T> point2 = make_point_2d<T>(4, 4);
    point_2d<T> point3 = make_point_2d<T>(3, 1);
    point_2d<T> point4 = make_point_2d<T>(1, 3);
    segm_vec.push_back(std::make_pair< point_2d<T>, point_2d<T> >(point1, point2));
    point_vec.push_back(point3);
    point_vec.push_back(point4);
    build_voronoi(point_vec, segm_vec, test_output);
    BOOST_CHECK_EQUAL(test_output.num_cell_records(), 5);
    BOOST_CHECK_EQUAL(test_output.num_vertex_records(), 4);
    BOOST_CHECK_EQUAL(test_output.num_edge_records(), 8);
    VERIFY_VORONOI_OUTPUT(test_output, NO_HALF_EDGE_INTERSECTIONS);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(segment_sites_test3, T, test_types) {
    typedef T coordinate_type;
    voronoi_output<coordinate_type> test_output;
    std::vector< point_2d<T> > point_vec;
    std::vector< std::pair< point_2d<T>, point_2d<T> > > segm_vec;
    point_2d<T> point1 = make_point_2d<T>(4, 0);
    point_2d<T> point2 = make_point_2d<T>(0, 4);
    point_2d<T> point3 = make_point_2d<T>(3, 3);
    point_2d<T> point4 = make_point_2d<T>(1, 1);
    segm_vec.push_back(std::make_pair< point_2d<T>, point_2d<T> >(point1, point2));
    point_vec.push_back(point3);
    point_vec.push_back(point4);
    build_voronoi(point_vec, segm_vec, test_output);
    BOOST_CHECK_EQUAL(test_output.num_cell_records(), 5);
    BOOST_CHECK_EQUAL(test_output.num_vertex_records(), 4);
    BOOST_CHECK_EQUAL(test_output.num_edge_records(), 8);
    VERIFY_VORONOI_OUTPUT(test_output, NO_HALF_EDGE_INTERSECTIONS);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(segment_sites_test4, T, test_types) {
    typedef T coordinate_type;
    voronoi_output<coordinate_type> test_output;
    std::vector< point_2d<T> > point_vec;
    std::vector< std::pair< point_2d<T>, point_2d<T> > > segm_vec;
    point_2d<T> point1 = make_point_2d<T>(4, 0);
    point_2d<T> point2 = make_point_2d<T>(0, 4);
    point_2d<T> point3 = make_point_2d<T>(3, 2);
    point_2d<T> point4 = make_point_2d<T>(2, 3);
    segm_vec.push_back(std::make_pair< point_2d<T>, point_2d<T> >(point1, point2));
    point_vec.push_back(point3);
    point_vec.push_back(point4);
    build_voronoi(point_vec, segm_vec, test_output);
    BOOST_CHECK_EQUAL(test_output.num_cell_records(), 5);
    BOOST_CHECK_EQUAL(test_output.num_vertex_records(), 3);
    BOOST_CHECK_EQUAL(test_output.num_edge_records(), 7);
    VERIFY_VORONOI_OUTPUT(test_output, NO_HALF_EDGE_INTERSECTIONS);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(segment_site_test5, T, test_types) {
    typedef T coordinate_type;
    voronoi_output<coordinate_type> test_output;
    std::vector< point_2d<T> > point_vec;
    std::vector< std::pair< point_2d<T>, point_2d<T> > > segm_vec;
    point_2d<T> point1 = make_point_2d<T>(0, 0);
    point_2d<T> point2 = make_point_2d<T>(0, 8);
    point_2d<T> point3 = make_point_2d<T>(-2, -2);
    point_2d<T> point4 = make_point_2d<T>(-2, 4);
    point_2d<T> point5 = make_point_2d<T>(-2, 10);
    segm_vec.push_back(std::make_pair< point_2d<T>, point_2d<T> >(point1, point2));
    point_vec.push_back(point3);
    point_vec.push_back(point4);
    point_vec.push_back(point5);
    build_voronoi(point_vec, segm_vec, test_output);
    BOOST_CHECK_EQUAL(test_output.num_cell_records(), 6);
    BOOST_CHECK_EQUAL(test_output.num_vertex_records(), 4);
    BOOST_CHECK_EQUAL(test_output.num_edge_records(), 9);
    VERIFY_VORONOI_OUTPUT(test_output, NO_HALF_EDGE_INTERSECTIONS);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(segment_site_test6, T, test_types) {
    typedef T coordinate_type;
    voronoi_output<coordinate_type> test_output;
    std::vector< point_2d<T> > point_vec;
    std::vector< std::pair< point_2d<T>, point_2d<T> > > segm_vec;
    point_2d<T> point1 = make_point_2d<T>(-1, 1);
    point_2d<T> point2 = make_point_2d<T>(1, 0);
    point_2d<T> point3 = make_point_2d<T>(1, 2);
    segm_vec.push_back(std::make_pair< point_2d<T>, point_2d<T> >(point2, point3));
    point_vec.push_back(point1);
    build_voronoi(point_vec, segm_vec, test_output);
    BOOST_CHECK_EQUAL(test_output.num_cell_records(), 4);
    BOOST_CHECK_EQUAL(test_output.num_vertex_records(), 2);
    BOOST_CHECK_EQUAL(test_output.num_edge_records(), 5);
    VERIFY_VORONOI_OUTPUT(test_output, NO_HALF_EDGE_INTERSECTIONS);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(segment_site_test7, T, test_types) {
    typedef T coordinate_type;
    voronoi_output<coordinate_type> test_output;
    std::vector< std::pair< point_2d<T>, point_2d<T> > > segm_vec;
    point_2d<T> point1 = make_point_2d<T>(0, 0);
    point_2d<T> point2 = make_point_2d<T>(4, 0);
    point_2d<T> point3 = make_point_2d<T>(0, 4);
    point_2d<T> point4 = make_point_2d<T>(4, 4);
    segm_vec.push_back(std::make_pair(point1, point2));
    segm_vec.push_back(std::make_pair(point2, point3));
    segm_vec.push_back(std::make_pair(point3, point4));
    build_voronoi(segm_vec, test_output);
    BOOST_CHECK_EQUAL(test_output.num_cell_records(), 7);
    BOOST_CHECK_EQUAL(test_output.num_vertex_records(), 6);
    BOOST_CHECK_EQUAL(test_output.num_edge_records(), 12);
    VERIFY_VORONOI_OUTPUT(test_output, NO_HALF_EDGE_INTERSECTIONS);

}

BOOST_AUTO_TEST_CASE_TEMPLATE(segment_site_test8, T, test_types) {
    typedef T coordinate_type;
    voronoi_output<coordinate_type> test_output;
    std::vector< std::pair< point_2d<T>, point_2d<T> > > segm_vec;
    point_2d<T> point1 = make_point_2d<T>(0, 0);
    point_2d<T> point2 = make_point_2d<T>(4, 0);
    point_2d<T> point3 = make_point_2d<T>(4, 4);
    point_2d<T> point4 = make_point_2d<T>(0, 4);
    segm_vec.push_back(std::make_pair(point1, point2));
    segm_vec.push_back(std::make_pair(point2, point3));
    segm_vec.push_back(std::make_pair(point3, point4));
    segm_vec.push_back(std::make_pair(point4, point1));
    build_voronoi(segm_vec, test_output);
    BOOST_CHECK_EQUAL(test_output.num_cell_records(), 8);
    BOOST_CHECK_EQUAL(test_output.num_vertex_records(), 5);
    BOOST_CHECK_EQUAL(test_output.num_edge_records(), 12);
    VERIFY_VORONOI_OUTPUT(test_output, NO_HALF_EDGE_INTERSECTIONS);
}

#ifdef NDEBUG
BOOST_AUTO_TEST_CASE_TEMPLATE(segment_grid_test, T, test_types) {
    voronoi_output<T> test_output_small, test_output_large;
    std::vector< std::pair< point_2d<T>, point_2d<T> > > segm_vec_small, segm_vec_large;
    int grid_size[] = {10, 33, 100, 333};
    int max_value[] = {100, 330, 1000, 3330};
    int array_length = sizeof(grid_size) / sizeof(int);
    for (int k = 0; k < array_length; k++) {
        int cur_sz = grid_size[k];
        int koef = std::numeric_limits<int>::max() / max_value[k];
        for (int i = 0; i < cur_sz + 1; i++)
            for (int j = 0; j < cur_sz; j++) {
                point_2d<T> point1_1(10 * i, 10 * j);
                point_2d<T> point1_2(koef * 10 * i, koef * 10 * j);
                point_2d<T> point2_1(10 * i, 10 * j + 10);
                point_2d<T> point2_2(koef * 10 * i, koef * (10 * j + 10));
                segm_vec_small.push_back(std::make_pair(point1_1, point2_1));
                segm_vec_large.push_back(std::make_pair(point1_2, point2_2));
                point_2d<T> point3_1(10 * j, 10 * i);
                point_2d<T> point3_2(koef * 10 * j, koef * 10 * i);
                point_2d<T> point4_1(10 * j + 10, 10 * i);
                point_2d<T> point4_2(koef * (10 * j + 10), koef * 10 * i);
                segm_vec_small.push_back(std::make_pair(point3_1, point4_1));
                segm_vec_large.push_back(std::make_pair(point3_2, point4_2));
            }
        build_voronoi(segm_vec_small, test_output_small);
        build_voronoi(segm_vec_large, test_output_large);
        VERIFY_VORONOI_OUTPUT(test_output_small, NO_HALF_EDGE_INTERSECTIONS);
        VERIFY_VORONOI_OUTPUT(test_output_large, NO_HALF_EDGE_INTERSECTIONS);
        BOOST_CHECK_EQUAL(test_output_small.num_cell_records(),
                          test_output_large.num_cell_records());
        BOOST_CHECK_EQUAL(test_output_small.num_vertex_records(),
                          test_output_large.num_vertex_records());
        BOOST_CHECK_EQUAL(test_output_small.num_edge_records(),
                          test_output_large.num_edge_records());
        segm_vec_small.clear();
        segm_vec_large.clear();
    }
}
#endif

//#ifdef NDEBUG
//BOOST_AUTO_TEST_CASE_TEMPLATE(segment_random_test1, T, test_types) {
//    srand(static_cast<unsigned int>(time(NULL)));
//    voronoi_output<T> test_output;
//    std::vector< point_2d<T> > point_vec;
//    std::vector< std::pair< point_2d<T>, point_2d<T> > > segm_vec;
//    int num_runs = 10000;
//    int num_segments = 5;
//    point_vec.push_back(make_point_2d<T>(-100, -100));
//    point_vec.push_back(make_point_2d<T>(-100, 100));
//    point_vec.push_back(make_point_2d<T>(100, -100));
//    point_vec.push_back(make_point_2d<T>(100, 100));
//    for (int i = 0; i < num_runs; i++) {
//        for (int j = 0; j < num_segments; j++) {
//            T x1 = 0, y1 = 0, x2 = 0, y2 = 0;
//            while (x1 == x2 && y1 == y2) {
//                x1 = (rand() % 100) - 50;
//                y1 = (rand() % 100) - 50;
//                x2 = (rand() % 100) - 50;
//                y2 = (rand() % 100) - 50;
//            }
//            point_2d<T> point1(x1, y1);
//            point_2d<T> point2(x2, y2);
//            segm_vec.push_back(std::make_pair(point1, point2));
//        }
//        remove_intersections(segm_vec);
//        build_voronoi(point_vec, segm_vec, test_output);
//        VERIFY_VORONOI_OUTPUT(test_output, NO_HALF_EDGE_INTERSECTIONS);
//        segm_vec.clear();
//    }
//}
//#endif

//#ifdef NDEBUG
//BOOST_AUTO_TEST_CASE_TEMPLATE(segment_random_test2, T, test_types) {
//    srand(static_cast<unsigned int>(time(NULL)));
//    voronoi_output<T> test_output_small, test_output_large;
//    std::vector< std::pair< point_2d<T>, point_2d<T> > > segm_vec;
//    int num_segments[] = {10, 100, 1000, 10000};
//    int num_runs[] = {1000, 100, 10, 1};
//    int mod_koef1[] = {100, 1000, 10000, 100000};
//    int mod_koef2[] = {100, 200, 300, 400};
//    int max_value[] = {100, 600, 5150, 50200};
//    int array_length = sizeof(num_segments) / sizeof(int);
//    for (int k = 0; k < 4; k++) {
//        int koef = std::numeric_limits<int>::max() / max_value[k];
//        for (int i = 0; i < num_runs[k]; i++) {
//            for (int j = 0; j < num_segments[k]; j++) {
//                T x1 = (rand() % (mod_koef1[k] / 100)) - mod_koef1[k] / 2;
//                T y1 = (rand() % (mod_koef1[k] / 100)) - mod_koef1[k] / 2;
//                T dx = 0, dy = 0;
//                while (dx == 0 && dy == 0) {
//                    dx = (rand() % mod_koef2[k]) - mod_koef2[k] / 2;
//                    dy = (rand() % mod_koef2[k]) - mod_koef2[k] / 2;
//                }
//                T x2 = x1 + dx;
//                T y2 = y1 + dy;
//                point_2d<T> point1_small(x1, y1);
//                point_2d<T> point2_small(x2, y2);
//                segm_vec.push_back(std::make_pair(point1_small, point2_small));
//            }
//            remove_intersections(segm_vec);
//            build_voronoi(segm_vec, test_output_small);
//            for (size_t j = 0; j < segm_vec.size(); j++) {
//                segm_vec[j].first.x(segm_vec[j].first.x() * koef);
//                segm_vec[j].first.y(segm_vec[j].first.y() * koef);
//                segm_vec[j].second.x(segm_vec[j].second.x() * koef);
//                segm_vec[j].second.y(segm_vec[j].second.y() * koef);
//            }
//            build_voronoi(segm_vec, test_output_large);
//            VERIFY_VORONOI_OUTPUT(test_output_large, NO_HALF_EDGE_INTERSECTIONS);
//            BOOST_CHECK_EQUAL(test_output_small.num_cell_records(),
//                              test_output_large.num_cell_records());
//            BOOST_CHECK_EQUAL(test_output_small.num_vertex_records(),
//                              test_output_large.num_vertex_records());
//            BOOST_CHECK_EQUAL(test_output_small.num_edge_records(),
//                              test_output_large.num_edge_records());
//            segm_vec.clear();
//        }
//    }
//}
//#endif

#ifdef USE_MSVC

#pragma warning (disable: 4355)
#pragma warning (disable: 4503)
#pragma warning (disable: 4786)

#endif

#include <iostream>

#ifdef NUMERICS_PRIVATE
#include "../config.h"
#include "../vector.h"
#include "../matrix.h"
#include "../banded.h"
#include "../io.h"
#else
#include <boost/numeric/ublas/config.h>
#include <boost/numeric/ublas/vector.h>
#include <boost/numeric/ublas/matrix.h>
#include <boost/numeric/ublas/banded.h>
#include <boost/numeric/ublas/io.h>
#endif

#include "test4.h"

// Test matrix & vector expression templates 
template<class V, class M, int N>
struct test_my_matrix_vector {
    typedef typename V::value_type value_type;

    template<class VP, class MP>
    void operator () (VP &v1, VP &v2, MP &m1) const {
        try {
            // Rows and columns
            initialize_matrix (m1);
            for (int i = 0; i < N; ++ i) {
                v2 = numerics::row (m1, i);
                std::cout << "row (m, " << i << ") = " << v2 << std::endl;
                v2 = numerics::column (m1, i);
                std::cout << "column (m, " << i << ") = " << v2 << std::endl;
            }

            // Outer product
            initialize_vector (v1);
            initialize_vector (v2);
            v1 (0) = 0;
            v1 (N - 1) = 0;
            m1 = numerics::outer_prod (v1, v2);
            std::cout << "outer_prod (v1, v2) = " << m1 << std::endl;

            // Matrix vector product 
            initialize_matrix (m1);
            initialize_vector (v1);
            v2 = numerics::prod (m1, v1);
            std::cout << "prod (m1, v1) = " << v2 << std::endl;
            v2 = numerics::prod (v1, m1);
            std::cout << "prod (v1, m1) = " << v2 << std::endl;
        }
        catch (std::exception &e) {
            std::cout << e.what () << std::endl;
        }
        catch (...) {
            std::cout << "unknown exception" << std::endl;
        }
    }
    void operator () () const {
        try {
            V v1 (N), v2 (N);
            M m1 (N, N, 1, 1);
            (*this) (v1, v2, m1);

            numerics::matrix_row<M> mr1 (m1, 1), mr2 (m1, 1);
            (*this) (mr1, mr2, m1);

            numerics::matrix_column<M> mc1 (m1, 1), mc2 (m1, 1);
            (*this) (mc1, mc2, m1);

#ifdef USE_RANGE_AND_SLICE
            numerics::matrix_vector_range<M> mvr1 (m1, 0, N, 0, N), mvr2 (m1, 0, N, 0, N);
            (*this) (mvr1, mvr2, m1);

            numerics::matrix_vector_slice<M> mvs1 (m1, 0, 1, N, 0, 1, N), mvs2 (m1, 0, 1, N, 0, 1, N);
            (*this) (mvs1, mvs2, m1);
#endif
        }
        catch (std::exception &e) {
            std::cout << e.what () << std::endl;
        }
        catch (...) {
            std::cout << "unknown exception" << std::endl;
        }
    }
    void operator () (int) const {
        try {
            V v1 (N), v2 (N);
            M m1 (N, N, 1, 1);
            numerics::banded_adaptor<M> bam1 (m1, 1, 1);
            (*this) (v1, v2, bam1);

            numerics::matrix_row<numerics::banded_adaptor<M> > mr1 (bam1, 1), mr2 (bam1, 1);
            (*this) (mr1, mr2, bam1);

            numerics::matrix_column<numerics::banded_adaptor<M> > mc1 (bam1, 1), mc2 (bam1, 1);
            (*this) (mc1, mc2, bam1);

#ifdef USE_RANGE_AND_SLICE
            numerics::matrix_vector_range<numerics::banded_adaptor<M> > mvr1 (bam1, 0, N, 0, N), mvr2 (bam1, 0, N, 0, N);
            (*this) (mvr1, mvr2, bam1);

            numerics::matrix_vector_slice<numerics::banded_adaptor<M> > mvs1 (bam1, 0, 1, N, 0, 1, N), mvs2 (bam1, 0, 1, N, 0, 1, N);
            (*this) (mvs1, mvs2, bam1);
#endif
        }
        catch (std::exception &e) {
            std::cout << e.what () << std::endl;
        }
        catch (...) {
            std::cout << "unknown exception" << std::endl;
        }
    }
};

// Test matrix & vector
void test_matrix_vector () {
    std::cout << "test_matrix_vector" << std::endl;

#ifdef USE_BOUNDED_ARRAY
    std::cout << "float, bounded_array" << std::endl;
    test_my_matrix_vector<numerics::vector<float, numerics::bounded_array<float, 3> >,
                          numerics::banded_matrix<float, numerics::row_major, numerics::bounded_array<float, 3 * 3> >, 3> () ();
    test_my_matrix_vector<numerics::vector<float, numerics::bounded_array<float, 3> >,
                          numerics::banded_matrix<float, numerics::row_major, numerics::bounded_array<float, 3 * 3> >, 3> () (0);

    std::cout << "double, bounded_array" << std::endl;
    test_my_matrix_vector<numerics::vector<double, numerics::bounded_array<double, 3> >,
                          numerics::banded_matrix<double, numerics::row_major, numerics::bounded_array<double, 3 * 3> >, 3> () ();
    test_my_matrix_vector<numerics::vector<double, numerics::bounded_array<double, 3> >,
                          numerics::banded_matrix<double, numerics::row_major, numerics::bounded_array<double, 3 * 3> >, 3> () (0);

    std::cout << "std::complex<float>, bounded_array" << std::endl;
    test_my_matrix_vector<numerics::vector<std::complex<float>, numerics::bounded_array<std::complex<float>, 3> >,
                          numerics::banded_matrix<std::complex<float>, numerics::row_major, numerics::bounded_array<std::complex<float>, 3 * 3> >, 3> () ();
    test_my_matrix_vector<numerics::vector<std::complex<float>, numerics::bounded_array<std::complex<float>, 3> >,
                          numerics::banded_matrix<std::complex<float>, numerics::row_major, numerics::bounded_array<std::complex<float>, 3 * 3> >, 3> () (0);

    std::cout << "std::complex<double>, bounded_array" << std::endl;
    test_my_matrix_vector<numerics::vector<std::complex<double>, numerics::bounded_array<std::complex<double>, 3> >,
                          numerics::banded_matrix<std::complex<double>, numerics::row_major, numerics::bounded_array<std::complex<double>, 3 * 3> >, 3> () ();
    test_my_matrix_vector<numerics::vector<std::complex<double>, numerics::bounded_array<std::complex<double>, 3> >,
                          numerics::banded_matrix<std::complex<double>, numerics::row_major, numerics::bounded_array<std::complex<double>, 3 * 3> >, 3> () (0);
#endif

#ifdef USE_UNBOUNDED_ARRAY
    std::cout << "float, unbounded_array" << std::endl;
    test_my_matrix_vector<numerics::vector<float, numerics::unbounded_array<float> >,
                          numerics::banded_matrix<float, numerics::row_major, numerics::unbounded_array<float> >, 3> () ();
    test_my_matrix_vector<numerics::vector<float, numerics::unbounded_array<float> >,
                          numerics::banded_matrix<float, numerics::row_major, numerics::unbounded_array<float> >, 3> () (0);

    std::cout << "double, unbounded_array" << std::endl;
    test_my_matrix_vector<numerics::vector<double, numerics::unbounded_array<double> >,
                          numerics::banded_matrix<double, numerics::row_major, numerics::unbounded_array<double> >, 3> () ();
    test_my_matrix_vector<numerics::vector<double, numerics::unbounded_array<double> >,
                          numerics::banded_matrix<double, numerics::row_major, numerics::unbounded_array<double> >, 3> () (0);

    std::cout << "std::complex<float>, unbounded_array" << std::endl;
    test_my_matrix_vector<numerics::vector<std::complex<float>, numerics::unbounded_array<std::complex<float> > >,
                          numerics::banded_matrix<std::complex<float>, numerics::row_major, numerics::unbounded_array<std::complex<float> > >, 3> () ();
    test_my_matrix_vector<numerics::vector<std::complex<float>, numerics::unbounded_array<std::complex<float> > >,
                          numerics::banded_matrix<std::complex<float>, numerics::row_major, numerics::unbounded_array<std::complex<float> > >, 3> () (0);

    std::cout << "std::complex<double>, unbounded_array" << std::endl;
    test_my_matrix_vector<numerics::vector<std::complex<double>, numerics::unbounded_array<std::complex<double> > >,
                          numerics::banded_matrix<std::complex<double>, numerics::row_major, numerics::unbounded_array<std::complex<double> > >, 3> () ();
    test_my_matrix_vector<numerics::vector<std::complex<double>, numerics::unbounded_array<std::complex<double> > >,
                          numerics::banded_matrix<std::complex<double>, numerics::row_major, numerics::unbounded_array<std::complex<double> > >, 3> () (0);
#endif

#ifdef USE_STD_VECTOR
    std::cout << "float, std::vector" << std::endl;
    test_my_matrix_vector<numerics::vector<float, std::vector<float> >,
                          numerics::banded_matrix<float, numerics::row_major, std::vector<float> >, 3> () ();
    test_my_matrix_vector<numerics::vector<float, std::vector<float> >,
                          numerics::banded_matrix<float, numerics::row_major, std::vector<float> >, 3> () (0);

    std::cout << "double, std::vector" << std::endl;
    test_my_matrix_vector<numerics::vector<double, std::vector<double> >,
                          numerics::banded_matrix<double, numerics::row_major, std::vector<double> >, 3> () ();
    test_my_matrix_vector<numerics::vector<double, std::vector<double> >,
                          numerics::banded_matrix<double, numerics::row_major, std::vector<double> >, 3> () (0);

    std::cout << "std::complex<float>, std::vector" << std::endl;
    test_my_matrix_vector<numerics::vector<std::complex<float>, std::vector<std::complex<float> > >,
                          numerics::banded_matrix<std::complex<float>, numerics::row_major, std::vector<std::complex<float> > >, 3> () ();
    test_my_matrix_vector<numerics::vector<std::complex<float>, std::vector<std::complex<float> > >,
                          numerics::banded_matrix<std::complex<float>, numerics::row_major, std::vector<std::complex<float> > >, 3> () (0);

    std::cout << "std::complex<double>, std::vector" << std::endl;
    test_my_matrix_vector<numerics::vector<std::complex<double>, std::vector<std::complex<double> > >,
                          numerics::banded_matrix<std::complex<double>, numerics::row_major, std::vector<std::complex<double> > >, 3> () ();
    test_my_matrix_vector<numerics::vector<std::complex<double>, std::vector<std::complex<double> > >,
                          numerics::banded_matrix<std::complex<double>, numerics::row_major, std::vector<std::complex<double> > >, 3> () (0);
#endif
}


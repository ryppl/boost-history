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
#include "../triangular.h"
#include "../io.h"
#else
#include <boost/numeric/ublas/config.h>
#include <boost/numeric/ublas/vector.h>
#include <boost/numeric/ublas/matrix.h>
#include <boost/numeric/ublas/triangular.h>
#include <boost/numeric/ublas/io.h>
#endif

#include "test5.h"

// Test matrix & vector expression templates
template<class V, class M, int N>
struct test_my_matrix_vector {
    typedef typename V::value_type value_type;

    template<class VP, class MP>
    void operator () (VP &v1, VP &v2, MP &m1) const {
		try {
			// Rows and columns
            initialize_matrix (m1, numerics::lower_tag ());
			for (int i = 0; i < N; ++ i) {
				v2 = numerics::row (m1, i);
				std::cout << "row (m, " << i << ") = " << v2 << std::endl;
				v2 = numerics::column (m1, i);
				std::cout << "column (m, " << i << ") = " << v2 << std::endl;
			}

// FIXME: MSVC ICE
#if ! defined (USE_MSVC) || ! defined (_DEBUG)
			// Outer product
			initialize_vector (v1);
            v1 (N - 1) = 0;
			initialize_vector (v2);
            v1 (0) = 0;
			m1 = numerics::outer_prod (v1, v2);
			std::cout << "outer_prod (v1, v2) = " << m1 << std::endl;
#endif

			// Matrix vector product 
            initialize_matrix (m1, numerics::lower_tag ());
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
			M m1 (N, N);
            (*this) (v1, v2, m1);

            numerics::matrix_row<M> mr1 (m1, N - 1), mr2 (m1, N - 1);
            (*this) (mr1, mr2, m1);

            numerics::matrix_column<M> mc1 (m1, 0), mc2 (m1, 0);
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
			M m1 (N, N);
            numerics::triangular_adaptor<M> tam1 (m1);
            (*this) (v1, v2, tam1);

            numerics::matrix_row<numerics::triangular_adaptor<M> > mr1 (tam1, N - 1), mr2 (tam1, N - 1);
            (*this) (mr1, mr2, tam1);

            numerics::matrix_column<numerics::triangular_adaptor<M> > mc1 (tam1, 0), mc2 (tam1, 0);
            (*this) (mc1, mc2, tam1);

#ifdef USE_RANGE_AND_SLICE
            numerics::matrix_vector_range<numerics::triangular_adaptor<M> > mvr1 (tam1, 0, N, 0, N), mvr2 (tam1, 0, N, 0, N);
            (*this) (mvr1, mvr2, tam1);

            numerics::matrix_vector_slice<numerics::triangular_adaptor<M> > mvs1 (tam1, 0, 1, N, 0, 1, N), mvs2 (tam1, 0, 1, N, 0, 1, N);
            (*this) (mvs1, mvs2, tam1);
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
    test_my_matrix_vector<numerics::vector<float, numerics::forward, numerics::bounded_array<float, 3> >,
                          numerics::triangular_matrix<float, numerics::lower, numerics::row_major<>, numerics::bounded_array<float, 3 * 3> >, 3> () ();
    test_my_matrix_vector<numerics::vector<float, numerics::forward, numerics::bounded_array<float, 3> >,
                          numerics::triangular_matrix<float, numerics::lower, numerics::row_major<>, numerics::bounded_array<float, 3 * 3> >, 3> () (0);

    std::cout << "double, bounded_array" << std::endl;
    test_my_matrix_vector<numerics::vector<double, numerics::forward, numerics::bounded_array<double, 3> >,
                          numerics::triangular_matrix<double, numerics::lower, numerics::row_major<>, numerics::bounded_array<double, 3 * 3> >, 3> () ();
    test_my_matrix_vector<numerics::vector<double, numerics::forward, numerics::bounded_array<double, 3> >,
                          numerics::triangular_matrix<double, numerics::lower, numerics::row_major<>, numerics::bounded_array<double, 3 * 3> >, 3> () (0);

    std::cout << "std::complex<float>, bounded_array" << std::endl;
    test_my_matrix_vector<numerics::vector<std::complex<float>, numerics::forward, numerics::bounded_array<std::complex<float>, 3> >,
                          numerics::triangular_matrix<std::complex<float>, numerics::lower, numerics::row_major<>, numerics::bounded_array<std::complex<float>, 3 * 3> >, 3> () ();
    test_my_matrix_vector<numerics::vector<std::complex<float>, numerics::forward, numerics::bounded_array<std::complex<float>, 3> >,
                          numerics::triangular_matrix<std::complex<float>, numerics::lower, numerics::row_major<>, numerics::bounded_array<std::complex<float>, 3 * 3> >, 3> () (0);

    std::cout << "std::complex<double>, bounded_array" << std::endl;
    test_my_matrix_vector<numerics::vector<std::complex<double>, numerics::forward, numerics::bounded_array<std::complex<double>, 3> >,
                          numerics::triangular_matrix<std::complex<double>, numerics::lower, numerics::row_major<>, numerics::bounded_array<std::complex<double>, 3 * 3> >, 3> () ();
    test_my_matrix_vector<numerics::vector<std::complex<double>, numerics::forward, numerics::bounded_array<std::complex<double>, 3> >,
                          numerics::triangular_matrix<std::complex<double>, numerics::lower, numerics::row_major<>, numerics::bounded_array<std::complex<double>, 3 * 3> >, 3> () (0);
#endif

#ifdef USE_UNBOUNDED_ARRAY
    std::cout << "float, unbounded_array" << std::endl;
    test_my_matrix_vector<numerics::vector<float, numerics::forward, numerics::unbounded_array<float> >,
                          numerics::triangular_matrix<float, numerics::lower, numerics::row_major<>, numerics::unbounded_array<float> >, 3> () ();
    test_my_matrix_vector<numerics::vector<float, numerics::forward, numerics::unbounded_array<float> >,
                          numerics::triangular_matrix<float, numerics::lower, numerics::row_major<>, numerics::unbounded_array<float> >, 3> () (0);

    std::cout << "double, unbounded_array" << std::endl;
    test_my_matrix_vector<numerics::vector<double, numerics::forward, numerics::unbounded_array<double> >,
                          numerics::triangular_matrix<double, numerics::lower, numerics::row_major<>, numerics::unbounded_array<double> >, 3> () ();
    test_my_matrix_vector<numerics::vector<double, numerics::forward, numerics::unbounded_array<double> >,
                          numerics::triangular_matrix<double, numerics::lower, numerics::row_major<>, numerics::unbounded_array<double> >, 3> () (0);

    std::cout << "std::complex<float>, unbounded_array" << std::endl;
    test_my_matrix_vector<numerics::vector<std::complex<float>, numerics::forward, numerics::unbounded_array<std::complex<float> > >,
                          numerics::triangular_matrix<std::complex<float>, numerics::lower, numerics::row_major<>, numerics::unbounded_array<std::complex<float> > >, 3> () ();
    test_my_matrix_vector<numerics::vector<std::complex<float>, numerics::forward, numerics::unbounded_array<std::complex<float> > >,
                          numerics::triangular_matrix<std::complex<float>, numerics::lower, numerics::row_major<>, numerics::unbounded_array<std::complex<float> > >, 3> () (0);

    std::cout << "std::complex<double>, unbounded_array" << std::endl;
    test_my_matrix_vector<numerics::vector<std::complex<double>, numerics::forward, numerics::unbounded_array<std::complex<double> > >,
                          numerics::triangular_matrix<std::complex<double>, numerics::lower, numerics::row_major<>, numerics::unbounded_array<std::complex<double> > >, 3> () ();
    test_my_matrix_vector<numerics::vector<std::complex<double>, numerics::forward, numerics::unbounded_array<std::complex<double> > >,
                          numerics::triangular_matrix<std::complex<double>, numerics::lower, numerics::row_major<>, numerics::unbounded_array<std::complex<double> > >, 3> () (0);
#endif

#ifdef USE_STD_VECTOR
    std::cout << "float, std::vector" << std::endl;
    test_my_matrix_vector<numerics::vector<float, numerics::forward, std::vector<float> >,
                          numerics::triangular_matrix<float, numerics::lower, numerics::row_major<>, std::vector<float> >, 3> () ();
    test_my_matrix_vector<numerics::vector<float, numerics::forward, std::vector<float> >,
                          numerics::triangular_matrix<float, numerics::lower, numerics::row_major<>, std::vector<float> >, 3> () (0);

    std::cout << "double, std::vector" << std::endl;
    test_my_matrix_vector<numerics::vector<double, numerics::forward, std::vector<double> >,
                          numerics::triangular_matrix<double, numerics::lower, numerics::row_major<>, std::vector<double> >, 3> () ();
    test_my_matrix_vector<numerics::vector<double, numerics::forward, std::vector<double> >,
                          numerics::triangular_matrix<double, numerics::lower, numerics::row_major<>, std::vector<double> >, 3> () (0);

    std::cout << "std::complex<float>, std::vector" << std::endl;
    test_my_matrix_vector<numerics::vector<std::complex<float>, numerics::forward, std::vector<std::complex<float> > >,
                          numerics::triangular_matrix<std::complex<float>, numerics::lower, numerics::row_major<>, std::vector<std::complex<float> > >, 3> () ();
    test_my_matrix_vector<numerics::vector<std::complex<float>, numerics::forward, std::vector<std::complex<float> > >,
                          numerics::triangular_matrix<std::complex<float>, numerics::lower, numerics::row_major<>, std::vector<std::complex<float> > >, 3> () (0);

    std::cout << "std::complex<double>, std::vector" << std::endl;
    test_my_matrix_vector<numerics::vector<std::complex<double>, numerics::forward, std::vector<std::complex<double> > >,
                          numerics::triangular_matrix<std::complex<double>, numerics::lower, numerics::row_major<>, std::vector<std::complex<double> > >, 3> () ();
    test_my_matrix_vector<numerics::vector<std::complex<double>, numerics::forward, std::vector<std::complex<double> > >,
                          numerics::triangular_matrix<std::complex<double>, numerics::lower, numerics::row_major<>, std::vector<std::complex<double> > >, 3> () (0);
#endif
}


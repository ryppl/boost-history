#ifdef USE_MSVC

#pragma warning (disable: 4355)
#pragma warning (disable: 4503)
#pragma warning (disable: 4786)

#endif

#include <iostream>

#include "../config.h"
#include "../vector.h"
#include "../vector_sp.h"
#include "../matrix.h"
#include "../matrix_sp.h"
#include "../io.h"

#include "test3.h"

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
				v1 = numerics::row (m1, i);
				std::cout << "row (m, " << i << ") = " << v1 << std::endl;
				v1 = numerics::column (m1, i);
				std::cout << "column (m, " << i << ") = " << v1 << std::endl;
			}

// FIXME: MSVC ICE
#if ! defined (USE_MSVC) || ! defined (_DEBUG)
			// Outer product
			initialize_vector (v1);
			initialize_vector (v2);
			m1 = numerics::outer_prod (v1, v2);
			std::cout << "outer_prod (v1, v2) = " << m1 << std::endl;
#endif

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
			V v1 (N, N), v2 (N, N);
			M m1 (N, N, N * N);

            (*this) (v1, v2, m1);

            numerics::matrix_row<M> mr1 (m1, 0), mr2 (m1, N - 1);
            (*this) (mr1, mr2, m1);

            numerics::matrix_column<M> mc1 (m1, 0), mc2 (m1, N - 1);
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
};

// Test matrix & vector
void test_matrix_vector () {
    std::cout << "test_matrix_vector" << std::endl;

#ifdef USE_SPARSE_MATRIX
    std::cout << "float, compressed_array" << std::endl;
    test_my_matrix_vector<numerics::sparse_vector<float, numerics::forward, numerics::compressed_array<std::size_t, float> >,
                          numerics::sparse_matrix<float, numerics::row_major<>, numerics::compressed_array<std::size_t, float> >, 3 > () ();

    std::cout << "double, compressed_array" << std::endl;
    test_my_matrix_vector<numerics::sparse_vector<double, numerics::forward, numerics::compressed_array<std::size_t, double> >,
                          numerics::sparse_matrix<double, numerics::row_major<>, numerics::compressed_array<std::size_t, double> >, 3 > () ();

    std::cout << "std::complex<float>, compressed_array" << std::endl;
    test_my_matrix_vector<numerics::sparse_vector<std::complex<float>, numerics::forward, numerics::compressed_array<std::size_t, std::complex<float> > >,
                          numerics::sparse_matrix<std::complex<float>, numerics::row_major<>, numerics::compressed_array<std::size_t, std::complex<float> > >, 3 > () ();

    std::cout << "std::complex<double>, compressed_array" << std::endl;
    test_my_matrix_vector<numerics::sparse_vector<std::complex<double>, numerics::forward, numerics::compressed_array<std::size_t, std::complex<double> > >,
                          numerics::sparse_matrix<std::complex<double>, numerics::row_major<>, numerics::compressed_array<std::size_t, std::complex<double> > >, 3 > () ();

    std::cout << "float, std::map" << std::endl;
    test_my_matrix_vector<numerics::sparse_vector<float, numerics::forward, std::map<std::size_t, float> >,
                          numerics::sparse_matrix<float, numerics::row_major<>, std::map<std::size_t, float> >, 3 > () ();

    std::cout << "double, std::map" << std::endl;
    test_my_matrix_vector<numerics::sparse_vector<double, numerics::forward, std::map<std::size_t, double> >,
                          numerics::sparse_matrix<double, numerics::row_major<>, std::map<std::size_t, double> >, 3 > () ();

    std::cout << "std::complex<float>, std::map" << std::endl;
    test_my_matrix_vector<numerics::sparse_vector<std::complex<float>, numerics::forward, std::map<std::size_t, std::complex<float> > >,
                          numerics::sparse_matrix<std::complex<float>, numerics::row_major<>, std::map<std::size_t, std::complex<float> > >, 3 > () ();

    std::cout << "std::complex<double>, std::map" << std::endl;
    test_my_matrix_vector<numerics::sparse_vector<std::complex<double>, numerics::forward, std::map<std::size_t, std::complex<double> > >,
                          numerics::sparse_matrix<std::complex<double>, numerics::row_major<>, std::map<std::size_t, std::complex<double> > >, 3 > () ();
#endif

#ifdef USE_SPARSE_VECTOR_OF_SPARSE_VECTOR
    std::cout << "float, compressed_array" << std::endl;
    test_my_matrix_vector<numerics::sparse_vector<float, numerics::forward, numerics::compressed_array<std::size_t, float> >,
                          numerics::sparse_vector_of_sparse_vector<float, numerics::row_major<>, numerics::compressed_array<std::size_t, numerics::compressed_array<std::size_t, float> > >, 3 > () ();

    std::cout << "double, compressed_array" << std::endl;
    test_my_matrix_vector<numerics::sparse_vector<double, numerics::forward, numerics::compressed_array<std::size_t, double> >,
                          numerics::sparse_vector_of_sparse_vector<double, numerics::row_major<>, numerics::compressed_array<std::size_t, numerics::compressed_array<std::size_t, double> > >, 3 > () ();

    std::cout << "std::complex<float>, compressed_array" << std::endl;
    test_my_matrix_vector<numerics::sparse_vector<std::complex<float>, numerics::forward, numerics::compressed_array<std::size_t, std::complex<float> > >,
                          numerics::sparse_vector_of_sparse_vector<std::complex<float>, numerics::row_major<>, numerics::compressed_array<std::size_t, numerics::compressed_array<std::size_t, std::complex<float> > > >, 3 > () ();

    std::cout << "std::complex<double>, compressed_array" << std::endl;
    test_my_matrix_vector<numerics::sparse_vector<std::complex<double>, numerics::forward, numerics::compressed_array<std::size_t, std::complex<double> > >,
                          numerics::sparse_vector_of_sparse_vector<std::complex<double>, numerics::row_major<>, numerics::compressed_array<std::size_t, numerics::compressed_array<std::size_t, std::complex<double> > > >, 3 > () ();

    std::cout << "float, std::map" << std::endl;
    test_my_matrix_vector<numerics::sparse_vector<float, numerics::forward, std::map<std::size_t, float> >,
                          numerics::sparse_vector_of_sparse_vector<float, numerics::row_major<>, std::map<std::size_t, std::map<std::size_t, float> > >, 3 > () ();

    std::cout << "double, std::map" << std::endl;
    test_my_matrix_vector<numerics::sparse_vector<double, numerics::forward, std::map<std::size_t, double> >,
                          numerics::sparse_vector_of_sparse_vector<double, numerics::row_major<>, std::map<std::size_t, std::map<std::size_t, double> > >, 3 > () ();

    std::cout << "std::complex<float>, std::map" << std::endl;
    test_my_matrix_vector<numerics::sparse_vector<std::complex<float>, numerics::forward, std::map<std::size_t, std::complex<float> > >,
                          numerics::sparse_vector_of_sparse_vector<std::complex<float>, numerics::row_major<>, std::map<std::size_t, std::map<std::size_t, std::complex<float> > > >, 3 > () ();

    std::cout << "std::complex<double>, std::map" << std::endl;
    test_my_matrix_vector<numerics::sparse_vector<std::complex<double>, numerics::forward, std::map<std::size_t, std::complex<double> > >,
                          numerics::sparse_vector_of_sparse_vector<std::complex<double>, numerics::row_major<>, std::map<std::size_t, std::map<std::size_t, std::complex<double> > > >, 3 > () ();
#endif
}


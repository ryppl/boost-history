#ifdef USE_MSVC

#pragma warning (disable: 4355)
#pragma warning (disable: 4503)
#pragma warning (disable: 4786)

#endif

#include <iostream>

#ifdef NUMERICS_PRIVATE
#include "../config.h"
#include "../vector.h"
#include "../vector_sp.h"
#include "../matrix.h"
#include "../matrix_sp.h"
#include "../io.h"
#else
#include <boost/numeric/ublas/config.h>
#include <boost/numeric/ublas/vector.h>
#include <boost/numeric/ublas/vector_sp.h>
#include <boost/numeric/ublas/matrix.h>
#include <boost/numeric/ublas/matrix_sp.h>
#include <boost/numeric/ublas/io.h>
#endif

#include "test3.h"

// Test matrix expression templates 
template<class M, int N>
struct test_my_matrix {
	typedef typename M::value_type value_type;

    template<class MP>
    void operator () (MP &m1, MP &m2, MP &m3) const {
		try {
            value_type t;

            // Copy and swap
			initialize_matrix (m1);
			initialize_matrix (m2);
            m1 = m2;
			std::cout << "m1 = m2 = " << m1 << std::endl;
            m1.assign_temporary (m2);
			std::cout << "m1.assign_temporary (m2) = " << m1 << std::endl;
            m1.swap (m2);
			std::cout << "m1.swap (m2) = " << m1 << " " << m2 << std::endl;

			// Unary matrix operations resulting in a matrix
			initialize_matrix (m1);
			m2 = - m1;
			std::cout << "- m1 = " << m2 << std::endl;
            m2 = numerics::conj (m1);
			std::cout << "conj (m1) = " << m2 << std::endl;

			// Binary matrix operations resulting in a matrix
			initialize_matrix (m1);
			initialize_matrix (m2);
			m3 = m1 + m2;
			std::cout << "m1 + m2 = " << m3 << std::endl;
			m3 = m1 - m2;
			std::cout << "m1 - m2 = " << m3 << std::endl;

			// Scaling a matrix
			t = N;
			initialize_matrix (m1);
			m2 = value_type (1.) * m1;
			std::cout << "1. * m1 = " << m2 << std::endl;
			m2 = t * m1;
			std::cout << "N * m1 = " << m2 << std::endl;
			initialize_matrix (m1);
			m2 = m1 * value_type (1.);
			std::cout << "m1 * 1. = " << m2 << std::endl;
			m2 = m1 * t;
			std::cout << "m1 * N = " << m2 << std::endl;

			// Some assignments
			initialize_matrix (m1);
			initialize_matrix (m2);
#ifdef NUMERICS_USE_ET
			m2 += m1;
			std::cout << "m2 += m1 = " << m2 << std::endl;
			m2 -= m1;
			std::cout << "m2 -= m1 = " << m2 << std::endl;
#else
			m2 = m2 + m1;
			std::cout << "m2 += m1 = " << m2 << std::endl;
			m2 = m2 - m1;
			std::cout << "m2 -= m1 = " << m2 << std::endl;
#endif
			m1 *= value_type (1.);
			std::cout << "m1 *= 1. = " << m1 << std::endl;
			m1 *= t;
			std::cout << "m1 *= N = " << m1 << std::endl;

			// Transpose
			initialize_matrix (m1);
			m2 = numerics::trans (m1);
			std::cout << "trans (m1) = " << m2 << std::endl;

			// Hermitean
			initialize_matrix (m1);
			m2 = numerics::herm (m1);
			std::cout << "herm (m1) = " << m2 << std::endl;

			// Matrix multiplication
			initialize_matrix (m1);
			initialize_matrix (m2);
            m3 = numerics::prod (m1, m2);
			std::cout << "prod (m1, m2) = " << m3 << std::endl;
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
			M m1 (N, N, N * N), m2 (N, N, N * N), m3 (N, N, N * N);
            (*this) (m1, m2, m3);

#ifdef USE_RANGE
            numerics::matrix_range<M> mr1 (m1, numerics::range (0, N), numerics::range (0, N)), 
									  mr2 (m2, numerics::range (0, N), numerics::range (0, N)), 
									  mr3 (m3, numerics::range (0, N), numerics::range (0, N));
            (*this) (mr1, mr2, mr3);
#endif

#ifdef USE_SLICE
            numerics::matrix_slice<M> ms1 (m1, numerics::slice (0, 1, N), numerics::slice (0, 1, N)), 
									  ms2 (m2, numerics::slice (0, 1, N), numerics::slice (0, 1, N)), 
									  ms3 (m3, numerics::slice (0, 1, N), numerics::slice (0, 1, N));
            (*this) (ms1, ms2, ms3);
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

// Test matrix 
void test_matrix () {
    std::cout << "test_matrix" << std::endl;

#ifdef USE_SPARSE_MATRIX
#ifdef USE_MAP_ARRAY
    std::cout << "float, map_array" << std::endl;
    test_my_matrix<numerics::sparse_matrix<float, numerics::row_major, numerics::map_array<std::size_t, float> >, 3 > () ();

    std::cout << "double, map_array" << std::endl;
    test_my_matrix<numerics::sparse_matrix<double, numerics::row_major, numerics::map_array<std::size_t, double> >, 3 > () ();

    std::cout << "std::complex<float>, map_array" << std::endl;
    test_my_matrix<numerics::sparse_matrix<std::complex<float>, numerics::row_major, numerics::map_array<std::size_t, std::complex<float> > >, 3 > () ();

    std::cout << "std::complex<double>, map_array" << std::endl;
    test_my_matrix<numerics::sparse_matrix<std::complex<double>, numerics::row_major, numerics::map_array<std::size_t, std::complex<double> > >, 3 > () ();
#endif

#ifdef USE_STD_MAP
    std::cout << "float, std::map" << std::endl;
    test_my_matrix<numerics::sparse_matrix<float, numerics::row_major, std::map<std::size_t, float> >, 3 > () ();

    std::cout << "double, std::map" << std::endl;
    test_my_matrix<numerics::sparse_matrix<double, numerics::row_major, std::map<std::size_t, double> >, 3 > () ();

    std::cout << "std::complex<float>, std::map" << std::endl;
    test_my_matrix<numerics::sparse_matrix<std::complex<float>, numerics::row_major, std::map<std::size_t, std::complex<float> > >, 3 > () ();

    std::cout << "std::complex<double>, std::map" << std::endl;
    test_my_matrix<numerics::sparse_matrix<std::complex<double>, numerics::row_major, std::map<std::size_t, std::complex<double> > >, 3 > () ();
#endif
#endif

#ifdef USE_SPARSE_VECTOR_OF_SPARSE_VECTOR
#ifdef USE_MAP_ARRAY
    std::cout << "float, map_array" << std::endl;
    test_my_matrix<numerics::sparse_vector_of_sparse_vector<float, numerics::row_major, numerics::map_array<std::size_t, numerics::map_array<std::size_t, float> > >, 3 > () ();

    std::cout << "double, map_array" << std::endl;
    test_my_matrix<numerics::sparse_vector_of_sparse_vector<double, numerics::row_major, numerics::map_array<std::size_t, numerics::map_array<std::size_t, double> > >, 3 > () ();

    std::cout << "std::complex<float>, map_array" << std::endl;
    test_my_matrix<numerics::sparse_vector_of_sparse_vector<std::complex<float>, numerics::row_major, numerics::map_array<std::size_t, numerics::map_array<std::size_t, std::complex<float> > > >, 3 > () ();

    std::cout << "std::complex<double>, map_array" << std::endl;
    test_my_matrix<numerics::sparse_vector_of_sparse_vector<std::complex<double>, numerics::row_major, numerics::map_array<std::size_t, numerics::map_array<std::size_t, std::complex<double> > > >, 3 > () ();
#endif

#ifdef USE_STD_MAP
    std::cout << "float, std::map" << std::endl;
    test_my_matrix<numerics::sparse_vector_of_sparse_vector<float, numerics::row_major, std::map<std::size_t, std::map<std::size_t, float> > >, 3 > () ();

    std::cout << "double, std::map" << std::endl;
    test_my_matrix<numerics::sparse_vector_of_sparse_vector<double, numerics::row_major, std::map<std::size_t, std::map<std::size_t, double> > >, 3 > () ();

    std::cout << "std::complex<float>, std::map" << std::endl;
    test_my_matrix<numerics::sparse_vector_of_sparse_vector<std::complex<float>, numerics::row_major, std::map<std::size_t, std::map<std::size_t, std::complex<float> > > >, 3 > () ();

    std::cout << "std::complex<double>, std::map" << std::endl;
    test_my_matrix<numerics::sparse_vector_of_sparse_vector<std::complex<double>, numerics::row_major, std::map<std::size_t, std::map<std::size_t, std::complex<double> > > >, 3 > () ();
#endif
#endif

#ifdef USE_COMPRESSED_MATRIX
    std::cout << "float" << std::endl;
    test_my_matrix<numerics::compressed_matrix<float>, 3 > () ();

    std::cout << "double" << std::endl;
    test_my_matrix<numerics::compressed_matrix<double>, 3 > () ();

    std::cout << "std::complex<float>" << std::endl;
    test_my_matrix<numerics::compressed_matrix<std::complex<float> >, 3 > () ();

    std::cout << "std::complex<double>" << std::endl;
    test_my_matrix<numerics::compressed_matrix<std::complex<double> >, 3 > () ();
#endif
}



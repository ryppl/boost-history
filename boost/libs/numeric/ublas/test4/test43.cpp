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
            // Banded times banded isn't banded
			std::cout << "prod (m1, m2) = " << numerics::prod (m1, m2) << std::endl;
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
			M m1 (N, N, 1, 1), m2 (N, N, 1, 1), m3 (N, N, 1, 1);
            (*this) (m1, m2, m3);

#ifdef USE_RANGE
            numerics::matrix_range<M> mr1 (m1, 0, N, 0, N), mr2 (m2, 0, N, 0, N), mr3 (m3, 0, N, 0, N);
            (*this) (mr1, mr2, mr3);
#endif

#ifdef USE_SLICE
            numerics::matrix_slice<M> ms1 (m1, 0, 1, N, 0, 1, N), ms2 (m2, 0, 1, N, 0, 1, N), ms3 (m3, 0, 1, N, 0, 1, N);
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
    void operator () (int) const {
		try {
			M m1 (N, N, 1, 1), m2 (N, N, 1, 1), m3 (N, N, 1, 1);
            numerics::banded_adaptor<M> bam1 (m1, 1, 1), bam2 (m2, 1, 1), bam3 (m3, 1, 1);
            (*this) (bam1, bam2, bam3);

#ifdef USE_RANGE
            numerics::matrix_range<numerics::banded_adaptor<M> > mr1 (bam1, 0, N, 0, N), mr2 (bam2, 0, N, 0, N), mr3 (bam3, 0, N, 0, N);
            (*this) (mr1, mr2, mr3);
#endif

#ifdef USE_SLICE
            numerics::matrix_slice<numerics::banded_adaptor<M> > ms1 (bam1, 0, 1, N, 0, 1, N), ms2 (bam2, 0, 1, N, 0, 1, N), ms3 (bam3, 0, 1, N, 0, 1, N);
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

#ifdef USE_BOUNDED_ARRAY
    std::cout << "float, bounded_array" << std::endl;
    test_my_matrix<numerics::banded_matrix<float, numerics::row_major<>, numerics::bounded_array<float, 3 * 3> >, 3 > () ();
    test_my_matrix<numerics::banded_matrix<float, numerics::row_major<>, numerics::bounded_array<float, 3 * 3> >, 3 > () (0);

    std::cout << "double, bounded_array" << std::endl;
    test_my_matrix<numerics::banded_matrix<double, numerics::row_major<>, numerics::bounded_array<double, 3 * 3> >, 3 > () ();
    test_my_matrix<numerics::banded_matrix<double, numerics::row_major<>, numerics::bounded_array<double, 3 * 3> >, 3 > () (0);

    std::cout << "std::complex<float>, bounded_array" << std::endl;
    test_my_matrix<numerics::banded_matrix<std::complex<float>, numerics::row_major<>, numerics::bounded_array<std::complex<float>, 3 * 3> >, 3 > () ();
    test_my_matrix<numerics::banded_matrix<std::complex<float>, numerics::row_major<>, numerics::bounded_array<std::complex<float>, 3 * 3> >, 3 > () (0);

    std::cout << "std::complex<double>, bounded_array" << std::endl;
    test_my_matrix<numerics::banded_matrix<std::complex<double>, numerics::row_major<>, numerics::bounded_array<std::complex<double>, 3 * 3> >, 3 > () ();
    test_my_matrix<numerics::banded_matrix<std::complex<double>, numerics::row_major<>, numerics::bounded_array<std::complex<double>, 3 * 3> >, 3 > () (0);
#endif

#ifdef USE_UNBOUNDED_ARRAY
    std::cout << "float, unbounded_array" << std::endl;
    test_my_matrix<numerics::banded_matrix<float, numerics::row_major<>, numerics::unbounded_array<float> >, 3 > () ();
    test_my_matrix<numerics::banded_matrix<float, numerics::row_major<>, numerics::unbounded_array<float> >, 3 > () (0);

    std::cout << "double, unbounded_array" << std::endl;
    test_my_matrix<numerics::banded_matrix<double, numerics::row_major<>, numerics::unbounded_array<double> >, 3 > () ();
    test_my_matrix<numerics::banded_matrix<double, numerics::row_major<>, numerics::unbounded_array<double> >, 3 > () (0);

    std::cout << "std::complex<float>, unbounded_array" << std::endl;
    test_my_matrix<numerics::banded_matrix<std::complex<float>, numerics::row_major<>, numerics::unbounded_array<std::complex<float> > >, 3 > () ();
    test_my_matrix<numerics::banded_matrix<std::complex<float>, numerics::row_major<>, numerics::unbounded_array<std::complex<float> > >, 3 > () (0);

    std::cout << "std::complex<double>, unbounded_array" << std::endl;
    test_my_matrix<numerics::banded_matrix<std::complex<double>, numerics::row_major<>, numerics::unbounded_array<std::complex<double> > >, 3 > () ();
    test_my_matrix<numerics::banded_matrix<std::complex<double>, numerics::row_major<>, numerics::unbounded_array<std::complex<double> > >, 3 > () (0);
#endif

#ifdef USE_STD_VECTOR
    std::cout << "float, std::vector" << std::endl;
    test_my_matrix<numerics::banded_matrix<float, numerics::row_major<>, std::vector<float> >, 3 > () ();
    test_my_matrix<numerics::banded_matrix<float, numerics::row_major<>, std::vector<float> >, 3 > () (0);

    std::cout << "double, std::vector" << std::endl;
    test_my_matrix<numerics::banded_matrix<double, numerics::row_major<>, std::vector<double> >, 3 > () ();
    test_my_matrix<numerics::banded_matrix<double, numerics::row_major<>, std::vector<double> >, 3 > () (0);

    std::cout << "std::complex<float>, std::vector" << std::endl;
    test_my_matrix<numerics::banded_matrix<std::complex<float>, numerics::row_major<>, std::vector<std::complex<float> > >, 3 > () ();
    test_my_matrix<numerics::banded_matrix<std::complex<float>, numerics::row_major<>, std::vector<std::complex<float> > >, 3 > () (0);

    std::cout << "std::complex<double>, std::vector" << std::endl;
    test_my_matrix<numerics::banded_matrix<std::complex<double>, numerics::row_major<>, std::vector<std::complex<double> > >, 3 > () ();
    test_my_matrix<numerics::banded_matrix<std::complex<double>, numerics::row_major<>, std::vector<std::complex<double> > >, 3 > () (0);
#endif
}



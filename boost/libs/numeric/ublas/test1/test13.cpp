#include <iostream>

#include "../config.h"
#include "../vector.h"
#include "../matrix.h"
#include "../io.h"

#include "test1.h"

// Test matrix expression templates 
template<class M, numerics::size_type N>
struct test_my_matrix {
	typedef typename M::value_type value_type;

    void operator () () const {
		try {
            value_type t;
			M m1 (N, N), m2 (N, N), m3 (N, N);

            // Copy and swap
			initialize_matrix (m1);
			initialize_matrix (m2);
            numerics::fast_copy (m1, m2);
			std::cout << "fast_copy (m1, m2) = " << m1 << std::endl;
            numerics::safe_copy (m1, m2);
			std::cout << "safe_copy (m1, m2) = " << m1 << std::endl;
            numerics::fast_swap (m1, m2);
			std::cout << "fast_swap (m1, m2) = " << m1 << " " << m2 << std::endl;
            numerics::safe_swap (m1, m2);
			std::cout << "safe_swap (m1, m2) = " << m1 << " " << m2 << std::endl;

			// Unary matrix operations resulting in a matrix
			initialize_matrix (m1);
			m2 = - m1;
			std::cout << "- m1 = " << m2 << std::endl;

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

			// Matrix multiplication
			initialize_matrix (m1);
			initialize_matrix (m2);
            m3 = numerics::prod (m1, m2);
			std::cout << "prod (m1, m2) = " << m3 << std::endl;

			// Slicing
			t = N;
			initialize_matrix (m1);
			numerics::slice s1 (1, 0, 1), s2 (1, 0, 1);
			numerics::matrix_slice<M> ms (m1, s1, s2);
			std::cout << "matrix_slice (m1, slice (1, 0, 1), slice (1, 0, 1)) = " << ms << std::endl;
			ms *= value_type (1.);
			std::cout << "ms *= 1. = " << ms << std::endl;
			ms *= t;
			std::cout << "ms *= N = " << ms << std::endl;
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

    std::cout << "float, bounded_array" << std::endl;
    test_my_matrix<numerics::matrix<float, numerics::bounded_array<float, 3 * 3> >, 3 > () ();

    std::cout << "double, bounded_array" << std::endl;
    test_my_matrix<numerics::matrix<double, numerics::bounded_array<double, 3 * 3> >, 3 > () ();

    std::cout << "std::complex<float>, bounded_array" << std::endl;
    test_my_matrix<numerics::matrix<std::complex<float>, numerics::bounded_array<std::complex<float>, 3 * 3> >, 3 > () ();

    std::cout << "std::complex<float>, bounded_array" << std::endl;
    test_my_matrix<numerics::matrix<std::complex<double>, numerics::bounded_array<std::complex<double>, 3 * 3> >, 3 > () ();

    std::cout << "float, unbounded_array" << std::endl;
    test_my_matrix<numerics::matrix<float, numerics::unbounded_array<float> >, 3 > () ();

    std::cout << "double, unbounded_array" << std::endl;
    test_my_matrix<numerics::matrix<double, numerics::unbounded_array<double> >, 3 > () ();

    std::cout << "std::complex<float>, unbounded_array" << std::endl;
    test_my_matrix<numerics::matrix<std::complex<float>, numerics::unbounded_array<std::complex<float> > >, 3 > () ();

    std::cout << "std::complex<float>, unbounded_array" << std::endl;
    test_my_matrix<numerics::matrix<std::complex<double>, numerics::unbounded_array<std::complex<double> > >, 3 > () ();
}



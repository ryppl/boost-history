#include <iostream>

#include "../config.h"
#include "../vector.h"
#include "../matrix.h"
#include "../io.h"

#include "test1.h"

// Test matrix & vector expression templates 
template<class V, class M, numerics::size_type N>
struct test_my_matrix_vector {
    typedef typename V::value_type value_type;

    void operator () () const {
		try {
            value_type t;
			V v1 (N), v2 (N), v3 (N);
			M m1 (N, N), m2 (N, N), m3 (N, N);

			// Rows and columns
			initialize_matrix (m1);
			for (numerics::size_type i = 0; i < N; ++ i) {
				v1 = numerics::row (m1, i);
				std::cout << "row (m, " << i << ") = " << v1 << std::endl;
				v1 = numerics::column (m1, i);
				std::cout << "column (m, " << i << ") = " << v1 << std::endl;
			}

			// Outer product
			initialize_vector (v1);
			initialize_vector (v2);
			m1 = numerics::outer_prod (v1, v2);
			std::cout << "outer_prod (v1, v2) = " << m1 << std::endl;

			// Matrix vector product 
			initialize_matrix (m1);
			initialize_vector (v1);
            v2 = numerics::prod (m1, v1);
			std::cout << "prod (m1, v1) = " << v2 << std::endl;

			// Slicing
			t = N;
			initialize_matrix (m1);
			numerics::slice s1 (1, 0, 1), s2 (1, 0, 1);
			numerics::matrix_vector_slice<M> mvs (m1, s1, s2);
			std::cout << "matrix_vector_slice (m1, slice (1, 0, 1), slice (1, 0, 1)) = " << mvs << std::endl;
			mvs *= value_type (1.);
			std::cout << "mvs *= 1. = " << mvs << std::endl;
			mvs *= t;
			std::cout << "mvs *= N = " << mvs << std::endl;
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

    std::cout << "float, bounded_array" << std::endl;
    test_my_matrix_vector<numerics::vector<float, numerics::bounded_array<float, 3> >,
                          numerics::matrix<float, numerics::bounded_array<float, 3 * 3> >, 3 > () ();

    std::cout << "double, bounded_array" << std::endl;
    test_my_matrix_vector<numerics::vector<double, numerics::bounded_array<double, 3> >,
                          numerics::matrix<double, numerics::bounded_array<double, 3 * 3> >, 3 > () ();

    std::cout << "std::complex<float>, bounded_array" << std::endl;
    test_my_matrix_vector<numerics::vector<std::complex<float>, numerics::bounded_array<std::complex<float>, 3> >,
                          numerics::matrix<std::complex<float>, numerics::bounded_array<std::complex<float>, 3 * 3> >, 3 > () ();

    std::cout << "std::complex<float>, bounded_array" << std::endl;
    test_my_matrix_vector<numerics::vector<std::complex<double>, numerics::bounded_array<std::complex<double>, 3> >,
                          numerics::matrix<std::complex<double>, numerics::bounded_array<std::complex<double>, 3 * 3> >, 3 > () ();

    std::cout << "float, unbounded_array" << std::endl;
    test_my_matrix_vector<numerics::vector<float, numerics::unbounded_array<float> >,
                          numerics::matrix<float, numerics::unbounded_array<float> >, 3 > () ();

    std::cout << "double, unbounded_array" << std::endl;
    test_my_matrix_vector<numerics::vector<double, numerics::unbounded_array<double> >,
                          numerics::matrix<double, numerics::unbounded_array<double> >, 3 > () ();

    std::cout << "std::complex<float>, unbounded_array" << std::endl;
    test_my_matrix_vector<numerics::vector<std::complex<float>, numerics::unbounded_array<std::complex<float> > >,
                          numerics::matrix<std::complex<float>, numerics::unbounded_array<std::complex<float> > >, 3 > () ();

    std::cout << "std::complex<float>, unbounded_array" << std::endl;
    test_my_matrix_vector<numerics::vector<std::complex<double>, numerics::unbounded_array<std::complex<double> > >,
                          numerics::matrix<std::complex<double>, numerics::unbounded_array<std::complex<double> > >, 3 > () ();
}


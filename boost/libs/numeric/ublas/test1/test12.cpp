#ifdef USE_MSVC

#pragma warning (disable: 4355)
#pragma warning (disable: 4503)
#pragma warning (disable: 4786)

#endif

#include <iostream>

#include "../config.h"
#include "../vector.h"
#include "../matrix.h"
#include "../io.h"

#include "test1.h"

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

            numerics::matrix_row<M> mr1 (m1, 0), mr2 (m1, 1);
            (*this) (mr1, mr2, m1);

            numerics::matrix_column<M> mc1 (m1, 0), mc2 (m1, 1);
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


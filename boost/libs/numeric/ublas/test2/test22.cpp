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

#include "../blas.h"

#include "test2.h"

template<class V, class M, int N>
void test_blas_2<V, M, N>::operator () () {
	try {
		V v1 (N), v2 (N);
		M m (N, N);

		// _t_mv
		initialize_vector (v1);
		initialize_matrix (m);
		numerics::blas_2::tmv (v1, m);
		std::cout << "tmv (v1, m) = " << v1 << std::endl;
		initialize_vector (v1);
		initialize_matrix (m);
		numerics::blas_2::tmv (v1, numerics::trans (m));
		std::cout << "tmv (v1, trans (m)) = " << v1 << std::endl;
#ifdef USE_STD_COMPLEX
		initialize_vector (v1);
		initialize_matrix (m);
		numerics::blas_2::tmv (v1, numerics::herm (m));
		std::cout << "tmv (v1, herm (m)) = " << v1 << std::endl;
#endif

		// _t_sv
		initialize_vector (v1);
		initialize_vector (v2);
		initialize_lower_triangular (m);
		numerics::blas_2::tsv (v1, m, numerics::lower_triangular_tag ());
        std::cout << "tsv (v1, m) = " << v1 << " " << numerics::prod (m, v1) - v2 << std::endl;
		initialize_vector (v1);
		initialize_vector (v2);
		initialize_upper_triangular (m);
		numerics::blas_2::tsv (v1, numerics::trans (m), numerics::lower_triangular_tag ());
        std::cout << "tsv (v1, trans (m)) = " << v1 << " " << numerics::prod (numerics::trans (m), v1) - v2 << std::endl;
#ifdef USE_STD_COMPLEX
		initialize_vector (v1);
		initialize_vector (v2);
		initialize_upper_triangular (m);
		numerics::blas_2::tsv (v1, numerics::herm (m), numerics::lower_triangular_tag ());
        std::cout << "tsv (v1, herm (m)) = " << v1 << " " << numerics::prod (numerics::herm (m), v1) - v2 << std::endl;
#endif
		initialize_vector (v1);
		initialize_vector (v2);
		initialize_upper_triangular (m);
		numerics::blas_2::tsv (v1, m, numerics::upper_triangular_tag ());
        std::cout << "tsv (v1, m) = " << v1 << " " << numerics::prod (m, v1) - v2 << std::endl;
		initialize_vector (v1);
		initialize_vector (v2);
		initialize_lower_triangular (m);
		numerics::blas_2::tsv (v1, numerics::trans (m), numerics::upper_triangular_tag ());
        std::cout << "tsv (v1, trans (m)) = " << v1 << " " << numerics::prod (numerics::trans (m), v1) - v2 << std::endl;
#ifdef USE_STD_COMPLEX
		initialize_vector (v1);
		initialize_vector (v2);
		initialize_lower_triangular (m);
		numerics::blas_2::tsv (v1, numerics::herm (m), numerics::upper_triangular_tag ());
        std::cout << "tsv (v1, herm (m)) = " << v1 << " " << numerics::prod (numerics::herm (m), v1) - v2 << std::endl;
#endif

		// _g_mv
		// _s_mv
		// _h_mv
		initialize_vector (v1);
		initialize_vector (v2);
		initialize_matrix (m);
		numerics::blas_2::gmv (v1, value_type (1), value_type (1), m, v2);
		std::cout << "gmv (v1, 1, 1, m, v2) = " << v1 << std::endl;
		numerics::blas_2::gmv (v1, value_type (1), value_type (1), numerics::trans (m), v2);
		std::cout << "gmv (v1, 1, 1, trans (m), v2) = " << v1 << std::endl;
#ifdef USE_STD_COMPLEX
		numerics::blas_2::gmv (v1, value_type (1), value_type (1), numerics::herm (m), v2);
		std::cout << "gmv (v1, 1, 1, herm (m), v2) = " << v1 << std::endl;
#endif

// FIXME: MSVC ICE
#if ! defined (USE_MSVC) || ! defined (_DEBUG)
		// _g_r
		// _g_ru
		// _g_rc
		initialize_vector (v1);
		initialize_vector (v2);
		initialize_matrix (m);
		numerics::blas_2::gr (m, value_type (1), v1, v2);
		std::cout << "gr (m, 1, v1, v2) = " << m << std::endl;
		numerics::blas_2::gr (m, value_type (1), v1, numerics::conj (v2));
		std::cout << "gr (m, 1, v1, conj (v2)) = " << m << std::endl;

		// _s_r
		initialize_vector (v1);
		initialize_matrix (m);
		numerics::blas_2::sr (m, value_type (1), v1);
		std::cout << "sr (m, 1, v1) = " << m << std::endl;

#ifdef USE_STD_COMPLEX
		// _h_r
		initialize_vector (v1);
		initialize_matrix (m);
		numerics::blas_2::hr (m, value_type (1), v1);
		std::cout << "hr (m, 1, v1) = " << m << std::endl;
#endif

		// _s_r2
		initialize_vector (v1);
		initialize_vector (v2);
		initialize_matrix (m);
		numerics::blas_2::sr2 (m, value_type (1), v1, v2);
		std::cout << "sr2 (m, 1, v1, v2) = " << m << std::endl;

#ifdef USE_STD_COMPLEX
		// _h_r2
		initialize_vector (v1);
		initialize_vector (v2);
		initialize_matrix (m);
		numerics::blas_2::hr2 (m, value_type (1), v1, v2);
		std::cout << "hr2 (m, 1, v1, v2) = " << m << std::endl;
#endif
#endif
	}
	catch (std::exception &e) {
		std::cout << e.what () << std::endl;
	}
	catch (...) {
		std::cout << "unknown exception" << std::endl;
	}
}

template struct test_blas_2<numerics::vector<float>, numerics::matrix<float>, 3>;
template struct test_blas_2<numerics::vector<double>, numerics::matrix<double>, 3>;

#ifdef USE_STD_COMPLEX
template struct test_blas_2<numerics::vector<std::complex<float> >, numerics::matrix<std::complex<float> >, 3>;
template struct test_blas_2<numerics::vector<std::complex<double> >, numerics::matrix<std::complex<double> >, 3>;
#endif


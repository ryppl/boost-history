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

#include "../blas.h"
#else
#include <boost/numeric/ublas/config.h>
#include <boost/numeric/ublas/vector.h>
#include <boost/numeric/ublas/matrix.h>
#include <boost/numeric/ublas/triangular.h>
#include <boost/numeric/ublas/io.h>

#include <boost/numeric/ublas/blas.h>
#endif

#include "test2.h"

template<class M, int N>
void test_blas_3<M, N>::operator () () {
	try {
		M m1 (N, N), m2 (N, N), m3 (N, N);

		// _t_mm
		initialize_matrix (m1);
		initialize_matrix (m2);
		numerics::blas_3::tmm (m1, value_type (1), m2, m1);
		std::cout << "tmm (m1, 1, m2, m1) = " << m1 << std::endl;
		initialize_matrix (m1);
		initialize_matrix (m2);
		numerics::blas_3::tmm (m1, value_type (1), m2, numerics::trans (m1));
		std::cout << "tmm (m1, 1, m2, trans (m1)) = " << m1 << std::endl;
		initialize_matrix (m1);
		initialize_matrix (m2);
		numerics::blas_3::tmm (m1, value_type (1), numerics::trans (m2), m1);
		std::cout << "tmm (m1, 1, trans (m2), m1) = " << m1 << std::endl;
		initialize_matrix (m1);
		initialize_matrix (m2);
		numerics::blas_3::tmm (m1, value_type (1), numerics::trans (m2), numerics::trans (m1));
		std::cout << "tmm (m1, 1, trans (m2), trans (m1)) = " << m1 << std::endl;
#ifdef USE_STD_COMPLEX
		initialize_matrix (m1);
		initialize_matrix (m2);
		numerics::blas_3::tmm (m1, value_type (1), m2, numerics::herm (m1));
		std::cout << "tmm (m1, 1, m2, herm (m1)) = " << m1 << std::endl;
		initialize_matrix (m1);
		initialize_matrix (m2);
		numerics::blas_3::tmm (m1, value_type (1), numerics::herm (m2), m1);
		std::cout << "tmm (m1, 1, herm (m2), m1) = " << m1 << std::endl;
		initialize_matrix (m1);
		initialize_matrix (m2);
		numerics::blas_3::tmm (m1, value_type (1), numerics::trans (m2), numerics::herm (m1));
		std::cout << "tmm (m1, 1, trans (m2), herm (m1)) = " << m1 << std::endl;
		initialize_matrix (m1);
		initialize_matrix (m2);
		numerics::blas_3::tmm (m1, value_type (1), numerics::herm (m2), numerics::trans (m1));
		std::cout << "tmm (m1, 1, herm (m2), trans (m1)) = " << m1 << std::endl;
		initialize_matrix (m1);
		initialize_matrix (m2);
		numerics::blas_3::tmm (m1, value_type (1), numerics::herm (m2), numerics::herm (m1));
		std::cout << "tmm (m1, 1, herm (m2), herm (m1)) = " << m1 << std::endl;
#endif

		// _t_sm
		initialize_matrix (m1);
        initialize_matrix (m2, numerics::lower_tag ());
		initialize_matrix (m3);
		numerics::blas_3::tsm (m1, value_type (1), m2, numerics::lower_tag ());
        std::cout << "tsm (m1, 1, m2) = " << m1 << " " << numerics::prod (m2, m1) - value_type (1) * m3 << std::endl;			
		initialize_matrix (m1);
        initialize_matrix (m2, numerics::upper_tag ());
		numerics::blas_3::tsm (m1, value_type (1), numerics::trans (m2), numerics::lower_tag ());
		std::cout << "tsm (m1, 1, trans (m2)) = " << m1 << " " << numerics::prod (numerics::trans (m2), m1) - value_type (1) * m3 << std::endl;			
#ifdef USE_STD_COMPLEX
		initialize_matrix (m1);
        initialize_matrix (m2, numerics::upper_tag ());
		numerics::blas_3::tsm (m1, value_type (1), numerics::herm (m2), numerics::lower_tag ());
		std::cout << "tsm (m1, 1, herm (m2)) = " << m1 << " " << numerics::prod (numerics::herm (m2), m1) - value_type (1) * m3 << std::endl;			
#endif
		initialize_matrix (m1);
        initialize_matrix (m2, numerics::upper_tag ());
		numerics::blas_3::tsm (m1, value_type (1), m2, numerics::upper_tag ());
		std::cout << "tsm (m1, 1, m2) = " << m1 << " " << numerics::prod (m2, m1) - value_type (1) * m3 << std::endl;			
		initialize_matrix (m1);
        initialize_matrix (m2, numerics::lower_tag ());
		numerics::blas_3::tsm (m1, value_type (1), numerics::trans (m2), numerics::upper_tag ());
		std::cout << "tsm (m1, 1, trans (m2)) = " << m1 << " " << numerics::prod (numerics::trans (m2), m1) - value_type (1) * m3 << std::endl;			
#ifdef USE_STD_COMPLEX
		initialize_matrix (m1);
        initialize_matrix (m2, numerics::lower_tag ());
		numerics::blas_3::tsm (m1, value_type (1), numerics::herm (m2), numerics::upper_tag ());
		std::cout << "tsm (m1, 1, herm (m2)) = " << m1 << " " << numerics::prod (numerics::herm (m2), m1) - value_type (1) * m3 << std::endl;
#endif

		// _g_mm
		// _s_mm
		// _h_mm
		initialize_matrix (m1);
		initialize_matrix (m2);
		initialize_matrix (m3);
		numerics::blas_3::gmm (m1, value_type (1), value_type (1), m2, m3);
		std::cout << "gmm (m1, 1, 1, m2, m3) = " << m1 << std::endl;
		initialize_matrix (m1);
		initialize_matrix (m2);
		initialize_matrix (m3);
		numerics::blas_3::gmm (m1, value_type (1), value_type (1), numerics::trans (m2), m3);
		std::cout << "gmm (m1, 1, 1, trans (m2), m3) = " << m1 << std::endl;
		initialize_matrix (m1);
		initialize_matrix (m2);
		initialize_matrix (m3);
		numerics::blas_3::gmm (m1, value_type (1), value_type (1), m2, numerics::trans (m3));
		std::cout << "gmm (m1, 1, 1, m2, trans (m3)) = " << m1 << std::endl;
		initialize_matrix (m1);
		initialize_matrix (m2);
		initialize_matrix (m3);
		numerics::blas_3::gmm (m1, value_type (1), value_type (1), numerics::trans (m2), numerics::trans (m3));
		std::cout << "gmm (m1, 1, 1, trans (m2), trans (m3)) = " << m1 << std::endl;
#ifdef USE_STD_COMPLEX
		initialize_matrix (m1);
		initialize_matrix (m2);
		initialize_matrix (m3);
		numerics::blas_3::gmm (m1, value_type (1), value_type (1), numerics::herm (m2), m3);
		std::cout << "gmm (m1, 1, 1, herm (m2), m3) = " << m1 << std::endl;
		initialize_matrix (m1);
		initialize_matrix (m2);
		initialize_matrix (m3);
		numerics::blas_3::gmm (m1, value_type (1), value_type (1), m2, numerics::herm (m3));
		std::cout << "gmm (m1, 1, 1, m2, herm (m3)) = " << m1 << std::endl;
		initialize_matrix (m1);
		initialize_matrix (m2);
		initialize_matrix (m3);
		numerics::blas_3::gmm (m1, value_type (1), value_type (1), numerics::herm (m2), numerics::trans (m3));
		std::cout << "gmm (m1, 1, 1, herm (m2), trans (m3)) = " << m1 << std::endl;
		initialize_matrix (m1);
		initialize_matrix (m2);
		initialize_matrix (m3);
		numerics::blas_3::gmm (m1, value_type (1), value_type (1), numerics::trans (m2), numerics::herm (m3));
		std::cout << "gmm (m1, 1, 1, trans (m2), herm (m3)) = " << m1 << std::endl;
		initialize_matrix (m1);
		initialize_matrix (m2);
		initialize_matrix (m3);
		numerics::blas_3::gmm (m1, value_type (1), value_type (1), numerics::herm (m2), numerics::herm (m3));
		std::cout << "gmm (m1, 1, 1, herm (m2), herm (m3)) = " << m1 << std::endl;
#endif

		// s_rk
		initialize_matrix (m1);
		initialize_matrix (m2);
		numerics::blas_3::srk (m1, value_type (1), value_type (1), m2);
		std::cout << "srk (m1, 1, 1, m2) = " << m1 << std::endl;
		initialize_matrix (m1);
		initialize_matrix (m2);
		numerics::blas_3::srk (m1, value_type (1), value_type (1), numerics::trans (m2));
		std::cout << "srk (m1, 1, 1, trans (m2)) = " << m1 << std::endl;

#ifdef USE_STD_COMPLEX
		// h_rk
		initialize_matrix (m1);
		initialize_matrix (m2);
		numerics::blas_3::hrk (m1, value_type (1), value_type (1), m2);
		std::cout << "hrk (m1, 1, 1, m2) = " << m1 << std::endl;
		initialize_matrix (m1);
		initialize_matrix (m2);
		numerics::blas_3::hrk (m1, value_type (1), value_type (1), numerics::herm (m2));
		std::cout << "hrk (m1, 1, 1, herm (m2)) = " << m1 << std::endl;
#endif

		// s_r2k
		initialize_matrix (m1);
		initialize_matrix (m2);
		initialize_matrix (m3);
		numerics::blas_3::sr2k (m1, value_type (1), value_type (1), m2, m3);
		std::cout << "sr2k (m1, 1, 1, m2, m3) = " << m1 << std::endl;
		initialize_matrix (m1);
		initialize_matrix (m2);
		initialize_matrix (m3);
		numerics::blas_3::sr2k (m1, value_type (1), value_type (1), numerics::trans (m2), numerics::trans (m3));
		std::cout << "sr2k (m1, 1, 1, trans (m2), trans (m3)) = " << m1 << std::endl;

#ifdef USE_STD_COMPLEX
		// h_r2k
		initialize_matrix (m1);
		initialize_matrix (m2);
		initialize_matrix (m3);
		numerics::blas_3::hr2k (m1, value_type (1), value_type (1), m2, m3);
		std::cout << "hr2k (m1, 1, 1, m2, m3) = " << m1 << std::endl;
		initialize_matrix (m1);
		initialize_matrix (m2);
		initialize_matrix (m3);
		numerics::blas_3::hr2k (m1, value_type (1), value_type (1), numerics::herm (m2), numerics::herm (m3));
		std::cout << "hsr2k (m1, 1, 1, herm (m2), herm (m3)) = " << m1 << std::endl;
#endif
	}
	catch (std::exception &e) {
		std::cout << e.what () << std::endl;
	}
	catch (...) {
		std::cout << "unknown exception" << std::endl;
	}
}

template struct test_blas_3<numerics::matrix<float>, 3>;
template struct test_blas_3<numerics::matrix<double>, 3>;

#ifdef USE_STD_COMPLEX
template struct test_blas_3<numerics::matrix<std::complex<float> >, 3>;
template struct test_blas_3<numerics::matrix<std::complex<double> >, 3>;
#endif

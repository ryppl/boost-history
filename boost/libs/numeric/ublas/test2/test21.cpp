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

template<class V, int N>
void test_blas_1<V, N>::operator () () {
	try {
		value_type t;
		norm_type n;
		V v1 (N), v2 (N);

		// _asum
		initialize_vector (v1);
		n = numerics::blas_1::asum (v1);
		std::cout << "asum (v1) = " << n << std::endl;

		// _amax
		initialize_vector (v1);
		n = numerics::blas_1::amax (v1);
		std::cout << "amax (v1) = " << n << std::endl;

		// _nrm2
		initialize_vector (v1);
		n = numerics::blas_1::nrm2 (v1);
		std::cout << "nrm2 (v1) = " << n << std::endl;

		// _dot
		// _dotu
		// _dotc
		initialize_vector (v1);
		initialize_vector (v2);
		t = numerics::blas_1::dot (v1, v2);
		std::cout << "dot (v1, v2) = " << t << std::endl;
		t = numerics::blas_1::dot (numerics::conj (v1), v2);
		std::cout << "dot (conj (v1), v2) = " << t << std::endl;

		// _copy
		initialize_vector (v2);
		numerics::blas_1::copy (v1, v2);
		std::cout << "copy (v1, v2) = " << v1 << std::endl;

		// _swap 
		initialize_vector (v1);
		initialize_vector (v2);
		numerics::blas_1::swap (v1, v2);
		std::cout << "swap (v1, v2) = " << v1 << " " << v2 << std::endl;

		// _scal
		// csscal
		// zdscal
		initialize_vector (v1);
		numerics::blas_1::scal (v1, value_type (1));
		std::cout << "scal (v1, 1) = " << v1 << std::endl;

		// _axpy
		initialize_vector (v1);
		initialize_vector (v2);
		numerics::blas_1::axpy (v1, value_type (1), v2);
		std::cout << "axpy (v1, 1, v2) = " << v1 << std::endl;

		// _rot
		initialize_vector (v1);
		initialize_vector (v2);
		numerics::blas_1::rot (value_type (1), v1, value_type (1), v2);
		std::cout << "rot (1, v1, 1, v2) = " << v1 << " " << v2 << std::endl;
	}
    catch (std::exception &e) {
		std::cout << e.what () << std::endl;
	}
	catch (...) {
		std::cout << "unknown exception" << std::endl;
	}
}

template struct test_blas_1<numerics::vector<float>, 3>;
template struct test_blas_1<numerics::vector<double>, 3>;
template struct test_blas_1<numerics::vector<std::complex<float> >, 3>;
template struct test_blas_1<numerics::vector<std::complex<double> >, 3>;

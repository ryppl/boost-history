#ifdef USE_MSVC

#pragma warning (disable: 4355)
#pragma warning (disable: 4503)
#pragma warning (disable: 4786)

#endif

#include <iostream>

#include "../config.h"
#include "../vector.h"
#include "../matrix.h"
#include "../triangular.h"
#include "../io.h"

#include "../blas.h"

#include "test2.h"

int main () {
	std::cout << "test_blas_1" << std::endl;

	std::cout << "float" << std::endl;
    test_blas_1<numerics::vector<float>, 3> () ();

	std::cout << "double" << std::endl;
    test_blas_1<numerics::vector<double>, 3> () ();

#ifdef USE_STD_COMPLEX
	std::cout << "std::complex<float>" << std::endl;
    test_blas_1<numerics::vector<std::complex<float> >, 3> () ();

	std::cout << "std::complex<double>" << std::endl;
    test_blas_1<numerics::vector<std::complex<double> >, 3> () ();
#endif

	std::cout << "test_blas_2" << std::endl;

	std::cout << "float" << std::endl;
    test_blas_2<numerics::vector<float>, numerics::matrix<float>, 3> () ();

	std::cout << "double" << std::endl;
    test_blas_2<numerics::vector<double>, numerics::matrix<double>, 3> () ();

#ifdef USE_STD_COMPLEX
	std::cout << "std::complex<float>" << std::endl;
    test_blas_2<numerics::vector<std::complex<float> >, numerics::matrix<std::complex<float> >, 3> () ();

	std::cout << "std::complex<double>" << std::endl;
    test_blas_2<numerics::vector<std::complex<double> >, numerics::matrix<std::complex<double> >, 3> () ();
#endif

	std::cout << "float" << std::endl;
    test_blas_3<numerics::matrix<float>, 3> () ();

	std::cout << "double" << std::endl;
    test_blas_3<numerics::matrix<double>, 3> () ();

#ifdef USE_STD_COMPLEX
	std::cout << "std::complex<float>" << std::endl;
    test_blas_3<numerics::matrix<std::complex<float> >, 3> () ();

	std::cout << "std::complex<double>" << std::endl;
    test_blas_3<numerics::matrix<std::complex<double> >, 3> () ();
#endif
    return 0;
}


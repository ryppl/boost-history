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
#else
#include <boost/numeric/ublas/config.h>
#include <boost/numeric/ublas/vector.h>
#include <boost/numeric/ublas/matrix.h>
#include <boost/numeric/ublas/triangular.h>
#include <boost/numeric/ublas/io.h>
#endif

#include "test5.h"

// Test vector expression templates
template<class V, int N>
struct test_my_vector {
    typedef typename V::value_type value_type;
    typedef typename V::size_type size_type;
    typedef typename numerics::type_traits<value_type>::norm_type norm_type;

    template<class VP>
    void operator () (VP &v1, VP &v2, VP &v3) const {
		try {            
            value_type t;
            size_type i;
            norm_type n;

            // Copy and swap
			initialize_vector (v1);
			initialize_vector (v2);
            v1 = v2;
			std::cout << "v1 = v2 = " << v1 << std::endl;
            v1.assign_temporary (v2);
			std::cout << "v1.assign_temporary (v2) = " << v1 << std::endl;
            v1.swap (v2);
			std::cout << "v1.swap (v2) = " << v1 << " " << v2 << std::endl;

			// Unary vector operations resulting in a vector
			initialize_vector (v1);
			v2 = - v1;
			std::cout << "- v1 = " << v2 << std::endl;
            v2 = numerics::conj (v1);
			std::cout << "conj (v1) = " << v2 << std::endl;

			// Binary vector operations resulting in a vector
			initialize_vector (v1);
			initialize_vector (v2);
			v3 = v1 + v2;
			std::cout << "v1 + v2 = " << v3 << std::endl;

			v3 = v1 - v2;
			std::cout << "v1 - v2 = " << v3 << std::endl;

			// Scaling a vector
			t = N;
			initialize_vector (v1);
			v2 = value_type (1.) * v1;
			std::cout << "1. * v1 = " << v2 << std::endl;
			v2 = t * v1;
			std::cout << "N * v1 = " << v2 << std::endl;

			// Some assignments
			initialize_vector (v1);
			initialize_vector (v2);
#ifdef NUMERICS_USE_ET
			v2 += v1;
			std::cout << "v2 += v1 = " << v2 << std::endl;
			v2 -= v1;
			std::cout << "v2 -= v1 = " << v2 << std::endl;
#else
			v2 = v2 + v1;
			std::cout << "v2 += v1 = " << v2 << std::endl;
			v2 = v2 - v1;
			std::cout << "v2 -= v1 = " << v2 << std::endl;
#endif
			v1 *= value_type (1.);
			std::cout << "v1 *= 1. = " << v1 << std::endl;
			v1 *= t;
			std::cout << "v1 *= N = " << v1 << std::endl;

			// Unary vector operations resulting in a scalar
			initialize_vector (v1);
			t = numerics::sum (v1);
			std::cout << "sum (v1) = " << t << std::endl;
			n = numerics::norm_1 (v1);
			std::cout << "norm_1 (v1) = " << n << std::endl;
			n = numerics::norm_2 (v1);
			std::cout << "norm_2 (v1) = " << n << std::endl;
			n = numerics::norm_inf (v1);
			std::cout << "norm_inf (v1) = " << n << std::endl;

            i = numerics::index_norm_inf (v1);
			std::cout << "index_norm_inf (v1) = " << i << std::endl;

			// Binary vector operations resulting in a scalar
			initialize_vector (v1);
			initialize_vector (v2);
			t = numerics::inner_prod (v1, v2);
			std::cout << "inner_prod (v1, v2) = " << t << std::endl;
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
			V v1 (N), v2 (N), v3 (N);
            (*this) (v1, v2, v3);

#ifdef USE_RANGE
            numerics::vector_range<V> vr1 (v1, 0, N), vr2 (v2, 0, N), vr3 (v3, 0, N);
            (*this) (vr1, vr2, vr3);
#endif

#ifdef USE_SLICE
            numerics::vector_slice<V> vs1 (v1, 0, 1, N), vs2 (v2, 0, 1, N), vs3 (v3, 0, 1, N);
            (*this) (vs1, vs2, vs3);
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

// Test vector
void test_vector () {
    std::cout << "test_vector" << std::endl;

#ifdef USE_BOUNDED_ARRAY
    std::cout << "float, bounded_array" << std::endl;
    test_my_vector<numerics::vector<float, numerics::forward, numerics::bounded_array<float, 3> >, 3 > () ();

    std::cout << "double, bounded_array" << std::endl;
    test_my_vector<numerics::vector<double, numerics::forward, numerics::bounded_array<double, 3> >, 3 > () ();

    std::cout << "std::complex<float>, bounded_array" << std::endl;
    test_my_vector<numerics::vector<std::complex<float>, numerics::forward, numerics::bounded_array<std::complex<float>, 3> >, 3 > () ();

    std::cout << "std::complex<double>, bounded_array" << std::endl;
    test_my_vector<numerics::vector<std::complex<double>, numerics::forward, numerics::bounded_array<std::complex<double>, 3> >, 3 > () ();
#endif

#ifdef USE_UNBOUNDED_ARRAY
    std::cout << "float, unbounded_array" << std::endl;
    test_my_vector<numerics::vector<float, numerics::forward, numerics::unbounded_array<float> >, 3 > () ();

    std::cout << "double, unbounded_array" << std::endl;
    test_my_vector<numerics::vector<double, numerics::forward, numerics::unbounded_array<double> >, 3 > () ();

    std::cout << "std::complex<float>, unbounded_array" << std::endl;
    test_my_vector<numerics::vector<std::complex<float>, numerics::forward, numerics::unbounded_array<std::complex<float> > >, 3 > () ();

    std::cout << "std::complex<double>, unbounded_array" << std::endl;
    test_my_vector<numerics::vector<std::complex<double>, numerics::forward, numerics::unbounded_array<std::complex<double> > >, 3 > () ();
#endif

#ifdef USE_STD_VECTOR
    std::cout << "float, std::vector" << std::endl;
    test_my_vector<numerics::vector<float, numerics::forward, std::vector<float> >, 3 > () ();

    std::cout << "double, std::vector" << std::endl;
    test_my_vector<numerics::vector<double, numerics::forward, std::vector<double> >, 3 > () ();

    std::cout << "std::complex<float>, std::vector" << std::endl;
    test_my_vector<numerics::vector<std::complex<float>, numerics::forward, std::vector<std::complex<float> > >, 3 > () ();

    std::cout << "std::complex<double>, std::vector" << std::endl;
    test_my_vector<numerics::vector<std::complex<double>, numerics::forward, std::vector<std::complex<double> > >, 3 > () ();
#endif
}


#include <iostream>

#include "../config.h"
#include "../vector.h"
#include "../matrix.h"
#include "../io.h"

#include "test1.h"

// Test vector expression templates 
template<class V, numerics::size_type N>
struct test_my_vector {
    typedef typename V::value_type value_type;
    typedef typename numerics::type_traits<value_type>::norm_type norm_type;

    void operator () () const {
		try {            
            value_type t;
            norm_type n;
			V v1 (N), v2 (N), v3 (N);

            // Copy and swap
			initialize_vector (v1);
			initialize_vector (v2);
            numerics::fast_copy (v1, v2);
			std::cout << "fast_copy (v1, v2) = " << v1 << std::endl;
            numerics::safe_copy (v1, v2);
			std::cout << "safe_copy (v1, v2) = " << v1 << std::endl;
            numerics::fast_swap (v1, v2);
			std::cout << "fast_swap (v1, v2) = " << v1 << " " << v2 << std::endl;
            numerics::safe_swap (v1, v2);
			std::cout << "safe_swap (v1, v2) = " << v1 << " " << v2 << std::endl;

			// Unary vector operations resulting in a vector
			initialize_vector (v1);
			v2 = - v1;
			std::cout << "- v1 = " << v2 << std::endl;

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

			// Binary vector operations resulting in a scalar
			initialize_vector (v1);
			initialize_vector (v2);
			t = numerics::inner_prod (v1, v2);
			std::cout << "inner_prod (v1, v2) = " << t << std::endl;

			// Slicing
			t = N;
			initialize_vector (v1);
			numerics::slice s (1, 0, 1);
			numerics::vector_slice<V> vs (v1, s);
			std::cout << "vector_slice (v1, slice (1, 0, 1)) = " << vs << std::endl;
			vs *= value_type (1.);
			std::cout << "vs *= 1. = " << vs << std::endl;
			vs *= t;
			std::cout << "vs *= N = " << vs << std::endl;
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

    std::cout << "float, bounded_array" << std::endl;
    test_my_vector<numerics::vector<float, numerics::bounded_array<float, 3> >, 3 > () ();

    std::cout << "double, bounded_array" << std::endl;
    test_my_vector<numerics::vector<double, numerics::bounded_array<double, 3> >, 3 > () ();

    std::cout << "std::complex<float>, bounded_array" << std::endl;
    test_my_vector<numerics::vector<std::complex<float>, numerics::bounded_array<std::complex<float>, 3> >, 3 > () ();

    std::cout << "std::complex<float>, bounded_array" << std::endl;
    test_my_vector<numerics::vector<std::complex<double>, numerics::bounded_array<std::complex<double>, 3> >, 3 > () ();

    std::cout << "float, unbounded_array" << std::endl;
    test_my_vector<numerics::vector<float, numerics::unbounded_array<float> >, 3 > () ();

    std::cout << "double, unbounded_array" << std::endl;
    test_my_vector<numerics::vector<double, numerics::unbounded_array<double> >, 3 > () ();

    std::cout << "std::complex<float>, unbounded_array" << std::endl;
    test_my_vector<numerics::vector<std::complex<float>, numerics::unbounded_array<std::complex<float> > >, 3 > () ();

    std::cout << "std::complex<float>, unbounded_array" << std::endl;
    test_my_vector<numerics::vector<std::complex<double>, numerics::unbounded_array<std::complex<double> > >, 3 > () ();
}


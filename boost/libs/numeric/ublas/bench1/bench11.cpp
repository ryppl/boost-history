#include <iostream>

#include "../config.h"
#include "../vector.h"
#include "../matrix.h"

#include "../timer.h"

#include "bench1.h"

template<class T, numerics::size_type N, numerics::size_type R>
struct bench_c_inner_prod {
    void operator () () const {
		try {
			numerics::timer t;
			c_vector_traits<T, N>::type v1, v2;
			for (int i = 0; i < R; ++ i) {
				initialize_c_vector<T, N> () (v1);
				initialize_c_vector<T, N> () (v2);
				T s (0);
				for (int j = 0; j < N; ++ j) {
					s += v1 [j] * v2 [j];
				}
				sink_scalar (s);
			}
			std::cout << "elapsed: " << t.elapsed () << " s" << std::endl;
		}
        catch (std::exception &e) {
			std::cout << e.what () << std::endl;
		}
		catch (...) {
			std::cout << "unknown exception" << std::endl;
		}
    }
};
template<class T, class V, numerics::size_type N, numerics::size_type R>
struct bench_my_inner_prod {
    void operator () () const {
		try {
			numerics::timer t;
			V v1 (N), v2 (N);
			for (int i = 0; i < R; ++ i) {
				initialize_vector (v1);
				initialize_vector (v2);
				T s (numerics::inner_prod (v1, v2));
				sink_scalar (s);
			}
			std::cout << "elapsed: " << t.elapsed () << " s" << std::endl;
		}
		catch (std::exception &e) {
			std::cout << e.what () << std::endl;
		}
		catch (...) {
			std::cout << "unknown exception" << std::endl;
		}
    }
};
template<class T, class V, numerics::size_type N, numerics::size_type R>
struct bench_cpp_inner_prod {
    void operator () () const {
		try {
			numerics::timer t;
			V v1 (N), v2 (N);
			for (int i = 0; i < R; ++ i) {
				initialize_vector (v1);
				initialize_vector (v2);
				T s ((v1 * v2).sum ());
				sink_scalar (s);
			}
			std::cout << "elapsed: " << t.elapsed () << " s" << std::endl;
		}
		catch (std::exception &e) {
			std::cout << e.what () << std::endl;
		}
		catch (...) {
			std::cout << "unknown exception" << std::endl;
		}
    }
};

template<class T, numerics::size_type N, numerics::size_type R>
struct bench_c_vector_add {
    void operator () () const {
		try {
			numerics::timer t;
			c_vector_traits<T, N>::type v1, v2, v3;
			for (int i = 0; i < R; ++ i) {
				initialize_c_vector<T, N> () (v1);
				initialize_c_vector<T, N> () (v2);
				for (int j = 0; j < N; ++ j) {
					v3 [j] = - (v1 [j] + v2 [j]);
				}
				sink_c_vector<T, N> () (v3);
			}
			std::cout << "elapsed: " << t.elapsed () << " s" << std::endl;
		}
		catch (std::exception &e) {
			std::cout << e.what () << std::endl;
		}
		catch (...) {
			std::cout << "unknown exception" << std::endl;
		}
    }
};
template<class V, numerics::size_type N, numerics::size_type R>
struct bench_my_vector_add {
    void operator () (bool safe) const {
		try {
			numerics::timer t;
			V v1 (N), v2 (N), v3 (N);
			for (int i = 0; i < R; ++ i) {
				initialize_vector (v1);
				initialize_vector (v2);
				if (safe)
					v3 = - (v1 + v2);
				else
					v3 ^= - (v1 + v2);
				sink_vector (v3);
			}
			std::cout << "elapsed: " << t.elapsed () << " s" << std::endl;
		}
		catch (std::exception &e) {
			std::cout << e.what () << std::endl;
		}
		catch (...) {
			std::cout << "unknown exception" << std::endl;
		}
    }
};
template<class V, numerics::size_type N, numerics::size_type R>
struct bench_cpp_vector_add {
    void operator () () const {
		try {
			numerics::timer t;
			V v1 (N), v2 (N), v3 (N);
			for (int i = 0; i < R; ++ i) {
				initialize_vector (v1);
				initialize_vector (v2);
				v3 = - (v1 + v2);
				sink_vector (v3);
			}
			std::cout << "elapsed: " << t.elapsed () << " s" << std::endl;
		}
		catch (std::exception &e) {
			std::cout << e.what () << std::endl;
		}
		catch (...) {
			std::cout << "unknown exception" << std::endl;
		}
    }
};

// Benchmark O (n)
template<class T, numerics::size_type N, numerics::size_type R>
void bench_1<T, N, R>::operator () () {
	std::cout << "bench_1" << std::endl;

	std::cout << "inner_prod " << N << std::endl;

	std::cout << "C array" << std::endl;
	bench_c_inner_prod<T, N, R> () ();

	std::cout << "vector<bounded_array>" << std::endl;
	bench_my_inner_prod<T, numerics::vector<T, numerics::bounded_array<T, N> >, N, R> () ();

	std::cout << "vector<unbounded_array>" << std::endl;
	bench_my_inner_prod<T, numerics::vector<T, numerics::unbounded_array<T> >, N, R> () ();

#ifndef NUMERICS_USE_ITERATOR
	std::cout << "vector<std::valarray>" << std::endl;
	bench_my_inner_prod<T, numerics::vector<T, std::valarray<T> >, N, R> () ();

	std::cout << "vector<std::vector>" << std::endl;
	bench_my_inner_prod<T, numerics::vector<T, std::vector<T> >, N, R> () ();
#endif

#ifdef USE_VALARRAY
	std::cout << "std::valarray" << std::endl;
	bench_cpp_inner_prod<T, std::valarray<T>, N, R> () ();
#endif

	std::cout << "add " << N << std::endl;

	std::cout << "C array" << std::endl;
	bench_c_vector_add<T, N, R> () ();

	std::cout << "vector<bounded_array> safe" << std::endl;
	bench_my_vector_add<numerics::vector<T, numerics::bounded_array<T, N> >, N, R> () (true);

	std::cout << "vector<bounded_array>" << std::endl;
	bench_my_vector_add<numerics::vector<T, numerics::bounded_array<T, N> >, N, R> () (false);

	std::cout << "vector<unbounded_array> safe" << std::endl;
	bench_my_vector_add<numerics::vector<T, numerics::unbounded_array<T> >, N, R> () (true);

	std::cout << "vector<unbounded_array>" << std::endl;
	bench_my_vector_add<numerics::vector<T, numerics::unbounded_array<T> >, N, R> () (false);

#ifndef NUMERICS_USE_ITERATOR
	std::cout << "vector<std::valarray> safe" << std::endl;
	bench_my_vector_add<numerics::vector<T, std::valarray<T> >, N, R> () (true);

	std::cout << "vector<std::valarray>" << std::endl;
	bench_my_vector_add<numerics::vector<T, std::valarray<T> >, N, R> () (false);

	std::cout << "vector<std::vector> safe" << std::endl;
	bench_my_vector_add<numerics::vector<T, std::vector<T> >, N, R> () (true);

	std::cout << "vector<std::vector>" << std::endl;
	bench_my_vector_add<numerics::vector<T, std::vector<T> >, N, R> () (false);
#endif

#ifdef USE_VALARRAY
	std::cout << "std::valarray" << std::endl;
	bench_cpp_vector_add<std::valarray<T>, N, R> () ();
#endif
}

template struct bench_1<float, 3, 1000000 * SCALE>;
template struct bench_1<float, 10, 300000 * SCALE>;
template struct bench_1<float, 30, 100000 * SCALE>;
template struct bench_1<float, 100, 30000 * SCALE>;

template struct bench_1<double, 3, 1000000 * SCALE>;
template struct bench_1<double, 10, 300000 * SCALE>;
template struct bench_1<double, 30, 100000 * SCALE>;
template struct bench_1<double, 100, 30000 * SCALE>;

template struct bench_1<std::complex<float>, 3, 1000000 * SCALE>;
template struct bench_1<std::complex<float>, 10, 300000 * SCALE>;
template struct bench_1<std::complex<float>, 30, 100000 * SCALE>;
template struct bench_1<std::complex<float>, 100, 30000 * SCALE>;

template struct bench_1<std::complex<double>, 3, 1000000 * SCALE>;
template struct bench_1<std::complex<double>, 10, 300000 * SCALE>;
template struct bench_1<std::complex<double>, 30, 100000 * SCALE>;
template struct bench_1<std::complex<double>, 100, 30000 * SCALE>;

#include <iostream>

#include "../config.h"
#include "../vector.h"
#include "../matrix.h"

#include "../timer.h"

#include "bench1.h"

template<class T, numerics::size_type N, numerics::size_type R>
struct bench_c_matrix_prod {
    void operator () () const {
		try {
			numerics::timer t;
			c_matrix_traits<T, N, N>::type m1, m2, m3;
			for (int i = 0; i < R; ++ i) {
				initialize_c_matrix<T, N, N> () (m1);
				initialize_c_matrix<T, N, N> () (m2);
				for (int j = 0; j < N; ++ j) {
					for (int k = 0; k < N; ++ k) {
						m3 [j] [k] = 0;
						for (int l = 0; l < N; ++ l) {
							m3 [j] [k] += m1 [j] [l] * m2 [l] [k];
						}
					}
				}
				sink_c_matrix<T, N, N> () (m3);
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
template<class M, numerics::size_type N, numerics::size_type R>
struct bench_my_matrix_prod {
    void operator () (bool safe) const {
		try {
			numerics::timer t;
			M m1 (N, N), m2 (N, N), m3 (N, N);
			for (int i = 0; i < R; ++ i) {
				initialize_matrix (m1);
				initialize_matrix (m2);
				if (safe)
					m3 = numerics::prod (m1, m2);
				else
                    m3 ^= numerics::prod (m1, m2);
				sink_matrix (m3);
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
template<class M, numerics::size_type N, numerics::size_type R>
struct bench_cpp_matrix_prod {
    void operator () () const {
		try {
			numerics::timer t;
			M m1 (N * N), m2 (N * N), m3 (N * N);
			for (int i = 0; i < R; ++ i) {
				initialize_vector (m1);
				initialize_vector (m2);
				for (int j = 0; j < N; ++ j) {
                    std::valarray<typename M::value_type> row (m1 [std::slice (N * j, N, 1)]);
					for (int k = 0; k < N; ++ k) {
                        std::valarray<typename M::value_type> column (m2 [std::slice (k, N, N)]);
						m3 [N * j + k] = (row * column).sum ();
					}
				}
				sink_vector (m3);
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

// Benchmark O (n ^ 3)
template<class T, numerics::size_type N, numerics::size_type R>
void bench_3<T, N, R>::operator () () {
	std::cout << "bench_3" << std::endl;

	std::cout << "prod " << N << std::endl;

	std::cout << "C array" << std::endl;
	bench_c_matrix_prod<T, N, R> () ();

	std::cout << "matrix<bounded_array> safe" << std::endl;
	bench_my_matrix_prod<numerics::matrix<T, numerics::bounded_array<T, N * N> >, N, R> () (true);

	std::cout << "matrix<bounded_array>" << std::endl;
	bench_my_matrix_prod<numerics::matrix<T, numerics::bounded_array<T, N * N> >, N, R> () (false);

	std::cout << "matrix<unbounded_array> safe" << std::endl;
	bench_my_matrix_prod<numerics::matrix<T, numerics::unbounded_array<T> >, N, R> () (true);

	std::cout << "matrix<unbounded_array>" << std::endl;
	bench_my_matrix_prod<numerics::matrix<T, numerics::unbounded_array<T> >, N, R> () (false);

#ifndef NUMERICS_USE_ITERATOR
	std::cout << "matrix<std::valarray> safe" << std::endl;
	bench_my_matrix_prod<numerics::matrix<T, std::valarray<T> >, N, R> () (true);

	std::cout << "matrix<std::valarray>" << std::endl;
	bench_my_matrix_prod<numerics::matrix<T, std::valarray<T> >, N, R> () (false);

	std::cout << "matrix<std::vector> safe" << std::endl;
	bench_my_matrix_prod<numerics::matrix<T, std::vector<T> >, N, R> () (true);

	std::cout << "matrix<std::vector>" << std::endl;
	bench_my_matrix_prod<numerics::matrix<T, std::vector<T> >, N, R> () (false);
#endif

#ifdef USE_VALARRAY
	std::cout << "std::valarray" << std::endl;
	bench_cpp_matrix_prod<std::valarray<T>, N, R> () ();
#endif
}

template struct bench_3<float, 3, 100000 * SCALE>;
template struct bench_3<float, 10, 3000 * SCALE>;
template struct bench_3<float, 30, 100 * SCALE>;
template struct bench_3<float, 100, 3 * SCALE>;

template struct bench_3<double, 3, 100000 * SCALE>;
template struct bench_3<double, 10, 3000 * SCALE>;
template struct bench_3<double, 30, 100 * SCALE>;
template struct bench_3<double, 100, 3 * SCALE>;

template struct bench_3<std::complex<float>, 3, 100000 * SCALE>;
template struct bench_3<std::complex<float>, 10, 3000 * SCALE>;
template struct bench_3<std::complex<float>, 30, 100 * SCALE>;
template struct bench_3<std::complex<float>, 100, 3 * SCALE>;

template struct bench_3<std::complex<double>, 3, 100000 * SCALE>;
template struct bench_3<std::complex<double>, 10, 3000 * SCALE>;
template struct bench_3<std::complex<double>, 30, 100 * SCALE>;
template struct bench_3<std::complex<double>, 100, 3 * SCALE>;

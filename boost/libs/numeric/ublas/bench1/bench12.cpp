#include <iostream>

#include "../config.h"
#include "../vector.h"
#include "../matrix.h"

#include "../timer.h"

#include "bench1.h"

template<class T, numerics::size_type N, numerics::size_type R>
struct bench_c_matrix_vector_prod {
    void operator () () const {
		try {
			numerics::timer t;
			c_matrix_traits<T, N, N>::type m;
			c_vector_traits<T, N>::type v1, v2;
			for (int i = 0; i < R; ++ i) {
				initialize_c_matrix<T, N, N> () (m);
				initialize_c_vector<T, N> () (v1);
				for (int j = 0; j < N; ++ j) {
					v2 [j] = 0;
					for (int k = 0; k < N; ++ k) {
						v2 [j] += m [j] [k] * v1 [k];
					}
				}
				sink_c_vector<T, N> () (v2);
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
template<class M, class V, numerics::size_type N, numerics::size_type R>
struct bench_my_matrix_vector_prod {
    void operator () (bool safe) const {
		try {
			numerics::timer t;
			M m (N, N);
			V v1 (N), v2 (N);
			for (int i = 0; i < R; ++ i) {
				initialize_matrix (m);
				initialize_vector (v1);
				if (safe)
                    v2 = numerics::prod (m, v1);
				else
					v2 ^= numerics::prod (m, v1);
				sink_vector (v2);
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
template<class M, class V, numerics::size_type N, numerics::size_type R>
struct bench_cpp_matrix_vector_prod {
    void operator () () const {
		try {
			numerics::timer t;
			M m (N * N);
			V v1 (N), v2 (N);
			for (int i = 0; i < R; ++ i) {
				initialize_vector (m);
				initialize_vector (v1);
				for (int j = 0; j < N; ++ j) {
					std::valarray<m.value_type> row (m [std::slice (N * j, N, 1)]);
					v2 [j] = (row * v1).sum ();
				}
				sink_vector (v2);
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
struct bench_c_matrix_add {
    void operator () () const {
		try {
			numerics::timer t;
			c_matrix_traits<T, N, N>::type m1, m2, m3;
			for (int i = 0; i < R; ++ i) {
				initialize_c_matrix<T, N, N> () (m1);
				initialize_c_matrix<T, N, N> () (m2);
				for (int j = 0; j < N; ++ j) {
					for (int k = 0; k < N; ++ k) {
						m3 [j] [k] = - (m1 [j] [k] + m2 [j] [k]);
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
struct bench_my_matrix_add {
    void operator () (bool safe) const {
		try {
			numerics::timer t;
			M m1 (N, N), m2 (N, N), m3 (N, N);
			for (int i = 0; i < R; ++ i) {
				initialize_matrix (m1);
				initialize_matrix (m2);
				if (safe) 
					m3 = - (m1 + m2);
				else
					m3 ^= - (m1 + m2);
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
struct bench_cpp_matrix_add {
    void operator () () const {
		try {
			numerics::timer t;
			M m1 (N * N), m2 (N * N), m3 (N * N);
			for (int i = 0; i < R; ++ i) {
				initialize_vector (m1);
				initialize_vector (m2);
				m3 = - (m1 + m2);
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

// Benchmark O (n ^ 2)
template<class T, numerics::size_type N, numerics::size_type R>
void bench_2<T, N, R>::operator () () {
	std::cout << "bench_2" << std::endl;

	std::cout << "prod " << N << std::endl;

	std::cout << "C array" << std::endl;
	bench_c_matrix_vector_prod<T, N, R> () ();

	std::cout << "vector<bounded_array> safe" << std::endl;
	bench_my_matrix_vector_prod<numerics::matrix<T, numerics::bounded_array<T, N * N> >,
								numerics::vector<T, numerics::bounded_array<T, N> >, N, R> () (true);

	std::cout << "vector<bounded_array>" << std::endl;
	bench_my_matrix_vector_prod<numerics::matrix<T, numerics::bounded_array<T, N * N> >, 
								numerics::vector<T, numerics::bounded_array<T, N> >, N, R> () (false);

	std::cout << "vector<unbounded_array> safe" << std::endl;
	bench_my_matrix_vector_prod<numerics::matrix<T, numerics::unbounded_array<T> >, 
								numerics::vector<T, numerics::unbounded_array<T> >, N, R> () (true);

	std::cout << "vector<unbounded_array>" << std::endl;
	bench_my_matrix_vector_prod<numerics::matrix<T, numerics::unbounded_array<T> >,
								numerics::vector<T, numerics::unbounded_array<T> >, N, R> () (false);

#ifndef NUMERICS_USE_ITERATOR
	std::cout << "vector<std::valarray> safe" << std::endl;
	bench_my_matrix_vector_prod<numerics::matrix<T, std::valarray<T> >, 
								numerics::vector<T, std::valarray<T> >, N, R> () (true);

	std::cout << "vector<std::valarray>" << std::endl;
	bench_my_matrix_vector_prod<numerics::matrix<T, std::valarray<T> >,
								numerics::vector<T, std::valarray<T> >, N, R> () (false);

	std::cout << "vector<std::vector> safe" << std::endl;
	bench_my_matrix_vector_prod<numerics::matrix<T, std::vector<T> >,
								numerics::vector<T, std::vector<T> >, N, R> () (true);

	std::cout << "vector<std::vector>" << std::endl;
	bench_my_matrix_vector_prod<numerics::matrix<T, std::vector<T> >,
								numerics::vector<T, std::vector<T> >, N, R> () (false);
#endif

#ifdef USE_VALARRAY
	std::cout << "std::valarray" << std::endl;
	bench_cpp_matrix_vector_prod<std::valarray<T>, std::valarray<T>, N, R> () ();
#endif

	std::cout << "add " << N << std::endl;

	std::cout << "C array" << std::endl;
	bench_c_matrix_add<T, N, R> () ();

	std::cout << "matrix<bounded_array> safe" << std::endl;
	bench_my_matrix_add<numerics::matrix<T, numerics::bounded_array<T, N * N> >, N, R> () (true);

	std::cout << "matrix<bounded_array>" << std::endl;
	bench_my_matrix_add<numerics::matrix<T, numerics::bounded_array<T, N * N> >, N, R> () (false);

	std::cout << "matrix<unbounded_array> safe" << std::endl;
	bench_my_matrix_add<numerics::matrix<T, numerics::unbounded_array<T> >, N, R> () (true);

	std::cout << "matrix<unbounded_array>" << std::endl;
	bench_my_matrix_add<numerics::matrix<T, numerics::unbounded_array<T> >, N, R> () (false);

#ifndef NUMERICS_USE_ITERATOR
	std::cout << "matrix<std::valarray> safe" << std::endl;
	bench_my_matrix_add<numerics::matrix<T, std::valarray<T> >, N, R> () (true);

	std::cout << "matrix<std::valarray>" << std::endl;
	bench_my_matrix_add<numerics::matrix<T, std::valarray<T> >, N, R> () (false);

	std::cout << "matrix<std::vector> safe" << std::endl;
	bench_my_matrix_add<numerics::matrix<T, std::vector<T> >, N, R> () (true);

	std::cout << "matrix<std::vector>" << std::endl;
	bench_my_matrix_add<numerics::matrix<T, std::vector<T> >, N, R> () (false);
#endif

#ifdef USE_VALARRAY
	std::cout << "std::valarray" << std::endl;
	bench_cpp_matrix_add<std::valarray<T>, N, R> () ();
#endif
}

template struct bench_2<float, 3, 300000 * SCALE>;
template struct bench_2<float, 10, 30000 * SCALE>;
template struct bench_2<float, 30, 3000 * SCALE>;
template struct bench_2<float, 100, 300 * SCALE>;

template struct bench_2<double, 3, 300000 * SCALE>;
template struct bench_2<double, 10, 30000 * SCALE>;
template struct bench_2<double, 30, 3000 * SCALE>;
template struct bench_2<double, 100, 300 * SCALE>;

template struct bench_2<std::complex<float>, 3, 300000 * SCALE>;
template struct bench_2<std::complex<float>, 10, 30000 * SCALE>;
template struct bench_2<std::complex<float>, 30, 3000 * SCALE>;
template struct bench_2<std::complex<float>, 100, 300 * SCALE>;

template struct bench_2<std::complex<double>, 3, 300000 * SCALE>;
template struct bench_2<std::complex<double>, 10, 30000 * SCALE>;
template struct bench_2<std::complex<double>, 30, 3000 * SCALE>;
template struct bench_2<std::complex<double>, 100, 300 * SCALE>;


#ifdef USE_MSVC

#pragma warning (disable: 4355)
#pragma warning (disable: 4503)
#pragma warning (disable: 4786)

#endif

#include <iostream>
#include <string>

#ifdef NUMERICS_PRIVATE
#include "../config.h"
#include "../vector.h"
#include "../matrix.h"

#include "../timer.h"
#else
#include <boost/numeric/ublas/config.h>
#include <boost/numeric/ublas/vector.h>
#include <boost/numeric/ublas/matrix.h>

#include <boost/numeric/ublas/timer.h>
#endif

#include "bench1.h"

template<class T, int N>
struct bench_c_outer_prod {
    typedef T value_type;

    void operator () (int runs) const {
		try {
			static typename c_matrix_traits<T, N, N>::type m;
			static typename c_vector_traits<T, N>::type v1, v2;
			initialize_c_vector<T, N> () (v1);
			initialize_c_vector<T, N> () (v2);
			numerics::timer t;
			for (int i = 0; i < runs; ++ i) {
				for (int j = 0; j < N; ++ j) {
					for (int k = 0; k < N; ++ k) {
						m [j] [k] = - v1 [j] * v2 [k];
					}
				}
//				sink_c_matrix<T, N, N> () (m);
			}
            footer<value_type> () (N * N, N * N, runs, t.elapsed ());
		}
		catch (std::exception &e) {
			std::cout << e.what () << std::endl;
		}
		catch (...) {
			std::cout << "unknown exception" << std::endl;
		}
    }
};
template<class M, class V, int N>
struct bench_my_outer_prod {
    typedef typename M::value_type value_type;

    void operator () (int runs, safe_tag) const {
		try {
			static M m (N, N);
			static V v1 (N), v2 (N);
			initialize_vector (v1);
			initialize_vector (v2);
			numerics::timer t;
			for (int i = 0; i < runs; ++ i) {
                m = - numerics::outer_prod (v1, v2);
//				sink_matrix (m);
			}
            footer<value_type> () (N * N, N * N, runs, t.elapsed ());
		}
		catch (std::exception &e) {
			std::cout << e.what () << std::endl;
		}
		catch (...) {
			std::cout << "unknown exception" << std::endl;
		}
    }
    void operator () (int runs, fast_tag) const {
		try {
			static M m (N, N);
			static V v1 (N), v2 (N);
			initialize_matrix (m);
			initialize_vector (v1);
			numerics::timer t;
			for (int i = 0; i < runs; ++ i) {
                m.assign (- numerics::outer_prod (v1, v2));
//				sink_matrix (m);
			}
            footer<value_type> () (N * N, N * N, runs, t.elapsed ());
		}
		catch (std::exception &e) {
			std::cout << e.what () << std::endl;
		}
		catch (...) {
			std::cout << "unknown exception" << std::endl;
		}
    }
};
template<class M, class V, int N>
struct bench_cpp_outer_prod {
    typedef typename M::value_type value_type;

    void operator () (int runs) const {
		try {
			static M m (N * N);
			static V v1 (N), v2 (N);
			initialize_vector (v1);
			initialize_vector (v2);
			numerics::timer t;
			for (int i = 0; i < runs; ++ i) {
				for (int j = 0; j < N; ++ j) {
					for (int k = 0; k < N; ++ k) {
						m3 [N * j + k] = - v1 [j] * v2 [k];
					}
				}
//				sink_vector (m);
			}
            footer<value_type> () (N * N, N * N, runs, t.elapsed ());
		}
		catch (std::exception &e) {
			std::cout << e.what () << std::endl;
		}
		catch (...) {
			std::cout << "unknown exception" << std::endl;
		}
    }
};

template<class T, int N>
struct bench_c_matrix_vector_prod {
    typedef T value_type;

    void operator () (int runs) const {
		try {
			static typename c_matrix_traits<T, N, N>::type m;
			static typename c_vector_traits<T, N>::type v1, v2;
			initialize_c_matrix<T, N, N> () (m);
			initialize_c_vector<T, N> () (v1);
			numerics::timer t;
			for (int i = 0; i < runs; ++ i) {
				for (int j = 0; j < N; ++ j) {
					v2 [j] = 0;
					for (int k = 0; k < N; ++ k) {
						v2 [j] += m [j] [k] * v1 [k];
					}
				}
//				sink_c_vector<T, N> () (v2);
			}
            footer<value_type> () (N * N, N * (N - 1), runs, t.elapsed ());
		}
		catch (std::exception &e) {
			std::cout << e.what () << std::endl;
		}
		catch (...) {
			std::cout << "unknown exception" << std::endl;
		}
    }
};
template<class M, class V, int N>
struct bench_my_matrix_vector_prod {
    typedef typename M::value_type value_type;

    void operator () (int runs, safe_tag) const {
		try {
			static M m (N, N);
			static V v1 (N), v2 (N);
			initialize_matrix (m);
			initialize_vector (v1);
			numerics::timer t;
			for (int i = 0; i < runs; ++ i) {
                v2 = numerics::prod (m, v1);
//				sink_vector (v2);
			}
            footer<value_type> () (N * N, N * (N - 1), runs, t.elapsed ());
		}
		catch (std::exception &e) {
			std::cout << e.what () << std::endl;
		}
		catch (...) {
			std::cout << "unknown exception" << std::endl;
		}
    }
    void operator () (int runs, fast_tag) const {
		try {
			static M m (N, N);
			static V v1 (N), v2 (N);
			initialize_matrix (m);
			initialize_vector (v1);
			numerics::timer t;
			for (int i = 0; i < runs; ++ i) {
    			v2.assign (numerics::prod (m, v1));
//				sink_vector (v2);
			}
            footer<value_type> () (N * N, N * (N - 1), runs, t.elapsed ());
		}
		catch (std::exception &e) {
			std::cout << e.what () << std::endl;
		}
		catch (...) {
			std::cout << "unknown exception" << std::endl;
		}
    }
};
template<class M, class V, int N>
struct bench_cpp_matrix_vector_prod {
    typedef typename M::value_type value_type;

    void operator () (int runs) const {
		try {
			static M m (N * N);
			static V v1 (N), v2 (N);
			initialize_vector (m);
			initialize_vector (v1);
			numerics::timer t;
			for (int i = 0; i < runs; ++ i) {
				for (int j = 0; j < N; ++ j) {
                    std::valarray<value_type> row (m [std::slice (N * j, N, 1)]);
					v2 [j] = (row * v1).sum ();
				}
//				sink_vector (v2);
			}
            footer<value_type> () (N * N, N * (N - 1), runs, t.elapsed ());
		}
		catch (std::exception &e) {
			std::cout << e.what () << std::endl;
		}
		catch (...) {
			std::cout << "unknown exception" << std::endl;
		}
    }
};

template<class T, int N>
struct bench_c_matrix_add {
    typedef T value_type;

    void operator () (int runs) const {
		try {
			static typename c_matrix_traits<T, N, N>::type m1, m2, m3;
			initialize_c_matrix<T, N, N> () (m1);
			initialize_c_matrix<T, N, N> () (m2);
			numerics::timer t;
			for (int i = 0; i < runs; ++ i) {
				for (int j = 0; j < N; ++ j) {
					for (int k = 0; k < N; ++ k) {
						m3 [j] [k] = - (m1 [j] [k] + m2 [j] [k]);
					}
				}
//				sink_c_matrix<T, N, N> () (m3);
			}
            footer<value_type> () (0, 2 * N * N, runs, t.elapsed ());
		}
		catch (std::exception &e) {
			std::cout << e.what () << std::endl;
		}
		catch (...) {
			std::cout << "unknown exception" << std::endl;
		}
    }
};
template<class M, int N>
struct bench_my_matrix_add {
    typedef typename M::value_type value_type;

    void operator () (int runs, safe_tag) const {
		try {
			static M m1 (N, N), m2 (N, N), m3 (N, N);
			initialize_matrix (m1);
			initialize_matrix (m2);
			numerics::timer t;
			for (int i = 0; i < runs; ++ i) {
    			m3 = - (m1 + m2);
//				sink_matrix (m3);
			}
            footer<value_type> () (0, 2 * N * N, runs, t.elapsed ());
		}
		catch (std::exception &e) {
			std::cout << e.what () << std::endl;
		}
		catch (...) {
			std::cout << "unknown exception" << std::endl;
		}
    }
    void operator () (int runs, fast_tag) const {
		try {
			static M m1 (N, N), m2 (N, N), m3 (N, N);
			initialize_matrix (m1);
			initialize_matrix (m2);
			numerics::timer t;
			for (int i = 0; i < runs; ++ i) {
				m3.assign (- (m1 + m2));
//				sink_matrix (m3);
			}
            footer<value_type> () (0, 2 * N * N, runs, t.elapsed ());
		}
		catch (std::exception &e) {
			std::cout << e.what () << std::endl;
		}
		catch (...) {
			std::cout << "unknown exception" << std::endl;
		}
    }
};
template<class M, int N>
struct bench_cpp_matrix_add {
    typedef typename M::value_type value_type;

    void operator () (int runs) const {
		try {
			static M m1 (N * N), m2 (N * N), m3 (N * N);
			initialize_vector (m1);
			initialize_vector (m2);
			numerics::timer t;
			for (int i = 0; i < runs; ++ i) {
				m3 = - (m1 + m2);
//				sink_vector (m3);
			}
            footer<value_type> () (0, 2 * N * N, runs, t.elapsed ());
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
template<class T, int N>
void bench_2<T, N>::operator () (int runs) {
	header ("bench_2");

    header ("outer_prod");

	header ("C array");
	bench_c_outer_prod<T, N> () (runs);

#ifdef USE_C_ARRAY
    header ("c_matrix, c_vector safe");
    bench_my_outer_prod<numerics::c_matrix<T, N, N>,
						numerics::c_vector<T, N>, N> () (runs, safe_tag ());

    header ("c_matrix, c_vector fast");
    bench_my_outer_prod<numerics::c_matrix<T, N, N>,
						numerics::c_vector<T, N>, N> () (runs, fast_tag ());
#endif

#ifdef USE_BOUNDED_ARRAY
    header ("matrix<bounded_array>, vector<bounded_array> safe");
    bench_my_outer_prod<numerics::matrix<T, numerics::row_major, numerics::bounded_array<T, N * N> >,
						numerics::vector<T, numerics::bounded_array<T, N> >, N> () (runs, safe_tag ());

    header ("matrix<bounded_array>, vector<bounded_array> fast");
	bench_my_outer_prod<numerics::matrix<T, numerics::row_major, numerics::bounded_array<T, N * N> >, 
						numerics::vector<T, numerics::bounded_array<T, N> >, N> () (runs, fast_tag ());
#endif

#ifdef USE_UNBOUNDED_ARRAY
    header ("matrix<unbounded_array>, vector<unbounded_array> safe");
	bench_my_outer_prod<numerics::matrix<T, numerics::row_major, numerics::unbounded_array<T> >, 
						numerics::vector<T, numerics::unbounded_array<T> >, N> () (runs, safe_tag ());

    header ("matrix<unbounded_array>, vector<unbounded_array> fast");
	bench_my_outer_prod<numerics::matrix<T, numerics::row_major, numerics::unbounded_array<T> >,
						numerics::vector<T, numerics::unbounded_array<T> >, N> () (runs, fast_tag ());
#endif

#ifdef USE_STD_VALARRAY
    header ("matrix<std::valarray>, vector<std::valarray> safe");
	bench_my_outer_prod<numerics::matrix<T, numerics::row_major, std::valarray<T> >, 
						numerics::vector<T, std::valarray<T> >, N> () (runs, safe_tag ());

    header ("matrix<std::valarray>, vector<std::valarray> fast");
	bench_my_outer_prod<numerics::matrix<T, numerics::row_major, std::valarray<T> >,
						numerics::vector<T, std::valarray<T> >, N> () (runs, fast_tag ());
#endif

#ifdef USE_STD_VECTOR
    header ("matrix<std::vector>, vector<std::vector> safe");
	bench_my_outer_prod<numerics::matrix<T, numerics::row_major, std::vector<T> >,
						numerics::vector<T, std::vector<T> >, N> () (runs, safe_tag ());

    header ("matrix<std::vector>, vector<std::vector> fast");
	bench_my_outer_prod<numerics::matrix<T, numerics::row_major, std::vector<T> >,
			            numerics::vector<T, std::vector<T> >, N> () (runs, fast_tag ());
#endif

#ifdef USE_STD_VALARRAY
    header ("std::valarray");
	bench_cpp_outer_prod<std::valarray<T>, std::valarray<T>, N> () (runs);
#endif

    header ("prod (matrix, vector)");

	header ("C array");
	bench_c_matrix_vector_prod<T, N> () (runs);

#ifdef USE_C_ARRAY
    header ("c_matrix, c_vector safe");
	bench_my_matrix_vector_prod<numerics::c_matrix<T, N, N>,
								numerics::c_vector<T, N>, N> () (runs, safe_tag ());

    header ("c_matrix, c_vector fast");
	bench_my_matrix_vector_prod<numerics::c_matrix<T, N, N>,
								numerics::c_vector<T, N>, N> () (runs, fast_tag ());
#endif

#ifdef USE_BOUNDED_ARRAY
    header ("matrix<bounded_array>, vector<bounded_array> safe");
	bench_my_matrix_vector_prod<numerics::matrix<T, numerics::row_major, numerics::bounded_array<T, N * N> >,
								numerics::vector<T, numerics::bounded_array<T, N> >, N> () (runs, safe_tag ());

    header ("matrix<bounded_array>, vector<bounded_array> fast");
	bench_my_matrix_vector_prod<numerics::matrix<T, numerics::row_major, numerics::bounded_array<T, N * N> >, 
								numerics::vector<T, numerics::bounded_array<T, N> >, N> () (runs, fast_tag ());
#endif

#ifdef USE_UNBOUNDED_ARRAY
    header ("matrix<unbounded_array>, vector<unbounded_array> safe");
	bench_my_matrix_vector_prod<numerics::matrix<T, numerics::row_major, numerics::unbounded_array<T> >, 
								numerics::vector<T, numerics::unbounded_array<T> >, N> () (runs, safe_tag ());

    header ("matrix<unbounded_array>, vector<unbounded_array> fast");
	bench_my_matrix_vector_prod<numerics::matrix<T, numerics::row_major, numerics::unbounded_array<T> >,
								numerics::vector<T, numerics::unbounded_array<T> >, N> () (runs, fast_tag ());
#endif

#ifdef USE_STD_VALARRAY
    header ("matrix<std::valarray>, vector<std::valarray> safe");
	bench_my_matrix_vector_prod<numerics::matrix<T, numerics::row_major, std::valarray<T> >, 
								numerics::vector<T, std::valarray<T> >, N> () (runs, safe_tag ());

    header ("matrix<std::valarray>, vector<std::valarray> fast");
	bench_my_matrix_vector_prod<numerics::matrix<T, numerics::row_major, std::valarray<T> >,
								numerics::vector<T, std::valarray<T> >, N> () (runs, fast_tag ());
#endif

#ifdef USE_STD_VECTOR
    header ("matrix<std::vector>, vector<std::vector> safe");
	bench_my_matrix_vector_prod<numerics::matrix<T, numerics::row_major, std::vector<T> >,
								numerics::vector<T, std::vector<T> >, N> () (runs, safe_tag ());

    header ("matrix<std::vector>, vector<std::vector> fast");
	bench_my_matrix_vector_prod<numerics::matrix<T, numerics::row_major, std::vector<T> >,
								numerics::vector<T, std::vector<T> >, N> () (runs, fast_tag ());
#endif

#ifdef USE_STD_VALARRAY
    header ("std::valarray");
	bench_cpp_matrix_vector_prod<std::valarray<T>, std::valarray<T>, N> () (runs);
#endif

    header ("matrix + matrix");

	header ("C array");
	bench_c_matrix_add<T, N> () (runs);

#ifdef USE_C_ARRAY
	header ("c_matrix safe");
	bench_my_matrix_add<numerics::c_matrix<T, N, N>, N> () (runs, safe_tag ());

    header ("c_matrix fast");
	bench_my_matrix_add<numerics::c_matrix<T, N, N>, N> () (runs, fast_tag ());
#endif

#ifdef USE_BOUNDED_ARRAY
    header ("matrix<bounded_array> safe");
	bench_my_matrix_add<numerics::matrix<T, numerics::row_major, numerics::bounded_array<T, N * N> >, N> () (runs, safe_tag ());

	header ("matrix<bounded_array> fast");
	bench_my_matrix_add<numerics::matrix<T, numerics::row_major, numerics::bounded_array<T, N * N> >, N> () (runs, fast_tag ());
#endif

#ifdef USE_UNBOUNDED_ARRAY
	header ("matrix<unbounded_array> safe");
	bench_my_matrix_add<numerics::matrix<T, numerics::row_major, numerics::unbounded_array<T> >, N> () (runs, safe_tag ());

	header ("matrix<unbounded_array> fast");
	bench_my_matrix_add<numerics::matrix<T, numerics::row_major, numerics::unbounded_array<T> >, N> () (runs, fast_tag ());
#endif

#ifdef USE_STD_VALARRAY
	header ("matrix<std::valarray> safe");
	bench_my_matrix_add<numerics::matrix<T, numerics::row_major, std::valarray<T> >, N> () (runs, safe_tag ());

	header ("matrix<std::valarray> fast");
	bench_my_matrix_add<numerics::matrix<T, numerics::row_major, std::valarray<T> >, N> () (runs, fast_tag ());
#endif

#ifdef USE_STD_VECTOR
	header ("matrix<std::vector> safe");
	bench_my_matrix_add<numerics::matrix<T, numerics::row_major, std::vector<T> >, N> () (runs, safe_tag ());

	header ("matrix<std::vector> fast");
	bench_my_matrix_add<numerics::matrix<T, numerics::row_major, std::vector<T> >, N> () (runs, fast_tag ());
#endif

#ifdef USE_STD_VALARRAY
	header ("std::valarray");
	bench_cpp_matrix_add<std::valarray<T>, N> () (runs);
#endif
}

template struct bench_2<float, 3>;
template struct bench_2<float, 10>;
template struct bench_2<float, 30>;
template struct bench_2<float, 100>;

template struct bench_2<double, 3>;
template struct bench_2<double, 10>;
template struct bench_2<double, 30>;
template struct bench_2<double, 100>;

#ifdef USE_STD_COMPLEX

template struct bench_2<std::complex<float>, 3>;
template struct bench_2<std::complex<float>, 10>;
template struct bench_2<std::complex<float>, 30>;
template struct bench_2<std::complex<float>, 100>;

template struct bench_2<std::complex<double>, 3>;
template struct bench_2<std::complex<double>, 10>;
template struct bench_2<std::complex<double>, 30>;
template struct bench_2<std::complex<double>, 100>;

#endif



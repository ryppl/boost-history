#ifndef BENCH1_H
#define BENCH1_H

void header (std::string text);

template<class T>
struct footer {
    void operator () (int multiplies, int plus, int runs, double elapsed) {
        std::cout << "elapsed: " << elapsed << " s, " 
                  << (multiplies * numerics::type_traits<T>::multiplies_complexity + 
                      plus * numerics::type_traits<T>::plus_complexity) * runs / 
                     (1024 * 1024 * elapsed) << " mflops" << std::endl;
        std::cerr << "elapsed: " << elapsed << " s, " 
                  << (multiplies * numerics::type_traits<T>::multiplies_complexity + 
                      plus * numerics::type_traits<T>::plus_complexity) * runs / 
                     (1024 * 1024 * elapsed) << " mflops" << std::endl;
    }
};

template<class T, int N> 
struct c_vector_traits {
    typedef T type [N];
};
template<class T, int N, int M> 
struct c_matrix_traits {
    typedef T type [N] [M];
};

template<class T, int N> 
struct initialize_c_vector  {
#ifdef USE_MSVC
    NUMERICS_INLINE
	void operator () (c_vector_traits<T, N>::type v) {
#else 
	void operator () (c_vector_traits<T, N>::type &v) {
#endif 
		for (int i = 0; i < N; ++ i) 
            v [i] = rand ();
//			v [i] = 0;
		}
};
template<class V>
NUMERICS_INLINE
void initialize_vector (V &v) {
    int size = v.size ();
    for (int i = 0; i < size; ++ i) 
        v [i] = rand ();
//      v [i] = 0;
}

template<class T, int N, int M> 
struct initialize_c_matrix  {
#ifdef USE_MSVC
    NUMERICS_INLINE
	void operator () (c_matrix_traits<T, N, M>::type m) {
#else 
	void operator () (c_matrix_traits<T, N, M>::type &m) {
#endif 
		for (int i = 0; i < N; ++ i) 
			for (int j = 0; j < M; ++ j) 
		        m [i] [j] = rand ();
//				m [i] [j] = 0;
	}
};
template<class M>
NUMERICS_INLINE
void initialize_matrix (M &m) {
    int size1 = m.size1 ();
    int size2 = m.size2 ();
    for (int i = 0; i < size1; ++ i) 
        for (int j = 0; j < size2; ++ j) 
            m (i, j) = rand ();
//          m (i, j) = 0;
}

template<class T>
NUMERICS_INLINE
void sink_scalar (const T &s) {
    static T g_s = s;
}

template<class T, int N>
struct sink_c_vector {
#ifdef USE_MSVC
    NUMERICS_INLINE
	void operator () (const c_vector_traits<T, N>::type v) {
#else 
	void operator () (const c_vector_traits<T, N>::type &v) {
#endif 
		static c_vector_traits<T, N>::type g_v;
		for (int i = 0; i < N; ++ i)
			g_v [i] = v [i];
	}
};
template<class V>
NUMERICS_INLINE
void sink_vector (const V &v) {
    static V g_v (v);
}

template<class T, int N, int M>
struct sink_c_matrix {
#ifdef USE_MSVC
    NUMERICS_INLINE
	void operator () (const c_matrix_traits<T, N, M>::type m) {
#else 
	void operator () (const c_matrix_traits<T, N, M>::type &m) {
#endif 
    static c_matrix_traits<T, N, M>::type g_m;
    for (int i = 0; i < N; ++ i)
        for (int j = 0; j < M; ++ j)
            g_m [i] [j] = m [i] [j];
	}
};
template<class M>
NUMERICS_INLINE
void sink_matrix (const M &m) {
    static M g_m (m);
}

template<class T>
struct peak {
	void operator () (int runs);
};

template<class T, int N>
struct bench_1 {
	void operator () (int runs);
};

template<class T, int N>
struct bench_2 {
	void operator () (int runs);
};

template<class T, int N>
struct bench_3 {
	void operator () (int runs);
};

struct safe_tag {};
struct fast_tag {};

// #define USE_STD_COMPLEX

#define USE_C_ARRAY
// #define USE_BOUNDED_ARRAY
#define USE_UNBOUNDED_ARRAY
// #define USE_STD_VALARRAY
#define USE_STD_VECTOR

#endif 



#ifndef BENCH1_H
#define BENCH1_H

#define SCALE 1

template<class T, numerics::size_type N> 
struct c_vector_traits {
    typedef T type [N];
};
template<class T, numerics::size_type N, numerics::size_type M> 
struct c_matrix_traits {
    typedef T type [N] [M];
};

template<class T, numerics::size_type N> 
struct initialize_c_vector  {
#ifdef USE_MSVC
    NUMERICS_INLINE
	void operator () (c_vector_traits<T, N>::type v) {
#else // USE_MSVC
	void operator () (c_vector_traits<T, N>::type &v) {
#endif // USE_MSVC
		for (int i = 0; i < N; ++ i) 
//			  v [i] = rand ();
			v [i] = 0;
		}
};
template<class V>
NUMERICS_INLINE
void initialize_vector (V &v) {
    int size = v.size ();
    for (int i = 0; i < size; ++ i) 
//        v [i] = rand ();
        v [i] = 0;
}

template<class T, numerics::size_type N, numerics::size_type M> 
struct initialize_c_matrix  {
#ifdef USE_MSVC
    NUMERICS_INLINE
	void operator () (c_matrix_traits<T, N, M>::type m) {
#else // USE_MSVC
	void operator () (c_matrix_traits<T, N, M>::type &m) {
#endif // USE_MSVC
		for (int i = 0; i < N; ++ i) 
			for (int j = 0; j < M; ++ j) 
//				  m [i] [j] = rand ();
				m [i] [j] = 0;
	}
};
template<class M>
NUMERICS_INLINE
void initialize_matrix (M &m) {
    int size1 = m.size1 ();
    int size2 = m.size2 ();
    for (int i = 0; i < size1; ++ i) 
        for (int j = 0; j < size2; ++ j) 
//            m (i, j) = rand ();
            m (i, j) = 0;
}

template<class T>
NUMERICS_INLINE
void sink_scalar (const T &s) {
    static T g_s = s;
}

template<class T, numerics::size_type N>
struct sink_c_vector {
#ifdef USE_MSVC
    NUMERICS_INLINE
	void operator () (const c_vector_traits<T, N>::type v) {
#else // USE_MSVC
	void operator () (const c_vector_traits<T, N>::type &v) {
#endif // USE_MSVC
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

template<class T, numerics::size_type N, numerics::size_type M>
struct sink_c_matrix {
#ifdef USE_MSVC
    NUMERICS_INLINE
	void operator () (const c_matrix_traits<T, N, M>::type m) {
#else // USE_MSVC
	void operator () (const c_matrix_traits<T, N, M>::type &m) {
#endif // USE_MSVC
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

template<class T, numerics::size_type N, numerics::size_type R>
struct bench_1 {
	void operator () ();
};

template<class T, numerics::size_type N, numerics::size_type R>
struct bench_2 {
	void operator () ();
};

template<class T, numerics::size_type N, numerics::size_type R>
struct bench_3 {
	void operator () ();
};

#endif // BENCH1_H
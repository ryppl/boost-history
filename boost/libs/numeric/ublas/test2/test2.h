#ifndef TEST2_H
#define TEST2_H

template<class V>
void initialize_vector (V &v) {
    int size = v.size ();
    for (int i = 0; i < size; ++ i) 
        v [i] = i + 1;
}

template<class M>
void initialize_matrix (M &m) {
    int size1 = m.size1 ();
    int size2 = m.size2 ();
    for (int i = 0; i < size1; ++ i) 
        for (int j = 0; j < size2; ++ j) 
            m (i, j) = i * size1 + j + 1;
}

template<class M>
void initialize_lower_triangular (M &m) {
    int size1 = m.size1 ();
    int size2 = m.size2 ();
    for (int i = 0; i < size1; ++ i) {
		int j = 0;
        for (; j <= i; ++ j) 
            m (i, j) = i * size1 + j + 1;
        for (; j < size2; ++ j) 
            m (i, j) = 0;
	}
}
template<class M>
void initialize_upper_triangular (M &m) {
    int size1 = m.size1 ();
    int size2 = m.size2 ();
    for (int i = 0; i < size1; ++ i) {
		int j = 0;
        for (; j < i; ++ j) 
            m (i, j) = 0;
        for (; j < size2; ++ j) 
            m (i, j) = i * size1 + j + 1;
	}
}

template<class V, int N>
struct test_blas_1 {
    typedef typename V::value_type value_type;
    typedef typename numerics::type_traits<value_type>::norm_type norm_type;

	void operator () ();
};

template<class V, class M, int N>
struct test_blas_2 {
    typedef typename V::value_type value_type;

	void operator () ();
};

template<class M, int N>
struct test_blas_3 {
    typedef typename M::value_type value_type;

	void operator () ();
};

// #define USE_STD_COMPLEX

#endif 



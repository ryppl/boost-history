#ifndef TEST1_H
#define TEST1_H

template<class V>
void initialize_vector (V &v) {
    int size = v.size ();
    for (int i = 0; i < size; ++ i) 
        v [i] = i + 1;
}

template<class M>
void initialize_matrix (M &m, numerics::lower_tag) {
    int size1 = m.size1 ();
//    int size2 = m.size2 ();
    for (int i = 0; i < size1; ++ i) {
		int j = 0;
        for (; j <= i; ++ j) 
            m (i, j) = i * size1 + j + 1;
//        for (; j < size2; ++ j) 
//            m (i, j) = 0;
	}
}
template<class M>
void initialize_matrix (M &m, numerics::upper_tag) {
    int size1 = m.size1 ();
    int size2 = m.size2 ();
    for (int i = 0; i < size1; ++ i) {
		int j = 0;
//        for (; j < i; ++ j) 
//            m (i, j) = 0;
        for (; j < size2; ++ j) 
            m (i, j) = i * size1 + j + 1;
	}
}
template<class M>
void initialize_matrix (M &m) {
    int size1 = m.size1 ();
    int size2 = m.size2 ();
    for (int i = 0; i < size1; ++ i) 
        for (int j = 0; j < size2; ++ j) 
            m (i, j) = i * size1 + j + 1;
}


void test_vector ();

void test_matrix_vector ();

void test_matrix ();

// FIXME: Borland out of memory!
#ifndef USE_BCC
#define USE_RANGE
// #define USE_SLICE
#endif

// #define USE_BOUNDED_ARRAY
#define USE_UNBOUNDED_ARRAY
// #define USE_STD_VECTOR

#endif 



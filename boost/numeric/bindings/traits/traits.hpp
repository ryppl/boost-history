// (-*- C++ -*- header)  

/*
 * 
 * Copyright (c) Kresimir Fresl and Toon Knapen 2002 
 *
 * Permission to copy, modify, use and distribute this software 
 * for any non-commercial or commercial purpose is granted provided 
 * that this license appear on all copies of the software source code.
 *
 * Authors assume no responsibility whatsoever for its use and makes 
 * no guarantees about its quality, correctness or reliability.
 *
 * First author acknowledges the support of the Faculty of Civil 
 * Engineering, University of Zagreb, Croatia.
 *
 */

#include <cstddef> 

namespace boost { namespace numeric { namespace bindings { namespace traits {

  // vector_traits<> base:
  // .. implementation -- do not use it directly
  template <typename V, typename T = typename V::value_type>
  struct default_vector_traits {
    typedef T value_type; 
    typedef T* pointer; 
    // assumption: iterator==pointer
    // .. e.g. ublas::(un)bounded_array 
    static pointer storage (V& v) { return v.begin(); }
    static int size (V& v) { return v.size(); } 
    static int stride (V& v) { return 1; } 
  }; 

  template <typename V, typename T>
  struct default_vector_traits<V const, T> {
    typedef T value_type; 
    typedef T const* pointer; 
    static pointer storage (V const& v) { return v.begin(); }
    static int size (V const& v) { return v.size(); } 
    static int stride (V const& v) { return 1; } 
  }; 

  // vector_traits<> generic version: 
  template <typename V>
  struct vector_traits : default_vector_traits<V> {}; 


  // matrix structure tags:
  struct general_t {}; 
  struct symmetric_t {}; 
  struct symmetric_packed_t {}; 
  struct hermitian_t {}; 
  struct hermitian_packed_t {}; 
  // triangular, banded etc. 
  struct unknown_structure_t {}; 

  // matrix_traits<> generic version: 
  template <typename M>
  struct matrix_traits {
    // typedefs:
    //   matrix_structure 
    //   value_type
    //   pointer
    //   ordering_type
    // static functions:
    //   pointer storage()
    //   int storage_size()
    //   int size1()
    //   int size2()
    //   int leading_dimension()  // not all matrix types 
    // symmetric/hermitian typedefs:
    //   uplo_type 
  }; 


  // storage ordering tags: 
  struct row_major_t {};
  struct column_major_t {};

  // upper/lower tags:
  struct upper_t {};
  struct lower_t {};


  // free accessor functions: 
  
  template <typename VectorT>
  inline 
  typename vector_traits<VectorT>::pointer vector_storage (VectorT& v) { 
    return vector_traits<VectorT>::storage (v); 
  }

  template <typename VectorT>
  inline
  int vector_size (VectorT& v) { 
    return vector_traits<VectorT>::size (v); 
  }

  template <typename VectorT>
  inline
  int vector_stride (VectorT& v) { 
    return vector_traits<VectorT>::stride (v); 
  }

  template <typename MatrixT>
  inline
  typename matrix_traits<MatrixT>::pointer matrix_storage (MatrixT& m) { 
    return matrix_traits<MatrixT>::storage (m); 
  }
  
  template <typename MatrixT>
  inline
  int matrix_size1 (MatrixT& m) { 
    return matrix_traits<MatrixT>::size1 (m); 
  }
  
  template <typename MatrixT>
  inline
  int matrix_size2 (MatrixT& m) { 
    return matrix_traits<MatrixT>::size2 (m); 
  }
  
  template <typename MatrixT>
  inline
  int matrix_storage_size (MatrixT& m) { 
    return matrix_traits<MatrixT>::storage_size (m); 
  }
  
  template <typename MatrixT>
  inline
  int matrix_stride1 (MatrixT& m) { 
    return matrix_traits<MatrixT>::stride1 (m); 
  }
  
  template <typename MatrixT>
  inline
  int matrix_stride2 (MatrixT& m) { 
    return matrix_traits<MatrixT>::stride2 (m); 
  }
  
  template <typename MatrixT>
  inline
  int leading_dimension (MatrixT& m) { 
    return matrix_traits<MatrixT>::leading_dimension (m); 
  }
  
}}}}  

#endif 

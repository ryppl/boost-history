//  Permission to copy, use, modify, sell and
//  distribute this software is granted provided this copyright notice appears
//  in all copies. This software is provided "as is" without express or implied
//  warranty, and with no claim as to its suitability for any purpose.
//  Copyright Toon Knapen

#include <boost/numeric/bindings/amos/amos.hpp>
#include <boost/numeric/bindings/amos/amos.h>
#include <algorithm>

namespace boost { namespace numeric { namespace bindings { namespace amos {

  namespace detail {

    template < typename strided_iterator, typename iterator1 >
    void swap(strided_iterator begin, strided_iterator end, int stride, iterator1 it1)
    { for( ; begin != end ; begin += stride, ++it1 ) std::swap( *begin, *it1 ) ; }

    template <>
    void besi< float >(const float * z, const float * fnu, const int * kode, const int * n, float* cy, int * nz, int * error)
    { AMOS_CBESI( z, fnu, kode, n, cy, nz, error ) ; }
    
    template <>
    void besi< double >(const double * z, const double * fnu, const int * kode, const int * n, double* cy, int * nz, int * error)
    { 
      AMOS_ZBESI( z, z + *n, fnu, kode, n, cy, cy + *n, nz, error ) ; 
      swap((double*)(z  + 1), (double*)(z  + *n - 1), 2, (double*)(z  + *n) ) ;
      swap((double*)(cy + 1), (double*)(cy + *n - 1), 2, (double*)(cy + *n) ) ;
    }
    
    template <>
    void besj< float >(const float * z, const float * fnu, const int * kode, const int * n, float* cy, int * nz, int * error)
    { AMOS_CBESJ( z, fnu, kode, n, cy, nz, error ) ; }
    
    template <>
    void besj< double >(const double * z, const double * fnu, const int * kode, const int * n, double* cy, int * nz, int * error)
    { 
      AMOS_ZBESJ( z, z+1, fnu, kode, n, cy, cy + 1, nz, error ) ; 
      swap((double*)(z  + 1), (double*)(z  + *n - 1), 2, (double*)(z  + *n) ) ;
      swap((double*)(cy + 1), (double*)(cy + *n - 1), 2, (double*)(cy + *n) ) ;
    }
    
    template <>
    void besy< float >(const float * z, const float * fnu, const int * kode, const int * n, float* cy, int * nz, float * wrk, int * error)
    { AMOS_CBESY( z, fnu, kode, n, cy, nz, wrk, error ) ; }
    
    template <>
    void besy< double >(const double * z, const double * fnu, const int * kode, const int * n, double* cy, int * nz, double * wrk, int * error)
    { 
      AMOS_ZBESY( z, z+1, fnu, kode, n, cy, cy + 1, nz, wrk, wrk + 1, error ) ; 
      swap((double*)(z  + 1), (double*)(z  + *n - 1), 2, (double*)(z  + *n) ) ;
      swap((double*)(cy + 1), (double*)(cy + *n - 1), 2, (double*)(cy + *n) ) ;
    }
    
  }

}}}}

//  Permission to copy, use, modify, sell and
//  distribute this software is granted provided this copyright notice appears
//  in all copies. This software is provided "as is" without express or implied
//  warranty, and with no claim as to its suitability for any purpose.
//  Copyright Toon Knapen

#ifndef boost_numeric_bindings_amos_amos_hpp
#define boost_numeric_bindings_amos_amos_hpp

#ifndef __cplusplus
#error "include directly amos_h"
#endif

#include <boost/numeric/bindings/amos/amos.h>
#include <boost/numeric/bindings/traits/value_traits.hpp>
#include <boost/numeric/bindings/traits/traits.hpp>
#include <boost/numeric/bindings/traits/value_traits.hpp>

namespace boost { namespace numeric { namespace bindings { namespace amos {

  namespace detail
  {
    using namespace ::boost::numeric::bindings::traits ;
    //
    // [CZ]BESI
    //
    template < typename value_type > 
    void besi(const typename value_traits< value_type >::value_type * z, 
              const typename value_traits< value_type >::value_type * fnu, 
              const int                                             * kode, 
              const int                                             * n, 
              typename value_traits< value_type >::value_type       * cy, 
              int                                                   * nz, 
              int                                                   * error)
    { assert( 0 ) ; } // no bindings for this value_type

    template <>
    void besi< float >(const fcomplex * z, const fcomplex * fnu, const int * kode, const int * n, fcomplex* cy, int * nz, int * error) ;

    template <>
    void besi< double >(const dcomplex * z, const dcomplex * fnu, const int * kode, const int * n, dcomplex* cy, int * nz, int * error) ;

    template <>
    void besi< std::complex< float > >(const fcomplex * z, const fcomplex * fnu, const int * kode, const int * n, fcomplex* cy, int * nz, int * error) ;

    template <>
    void besi< std::complex< double > >(const dcomplex * z, const dcomplex * fnu, const int * kode, const int * n, dcomplex* cy, int * nz, int * error) ;

    //
    // [CZ]BESJ
    //
    template < typename value_type > 
    void besj(const typename value_traits< value_type >::value_type * z, 
              const typename value_traits< value_type >::value_type * fnu, 
              const int                                             * kode, 
              const int                                             * n, 
              typename value_traits< value_type >::value_type       * cy, 
              int                                                   * nz, 
              int                                                   * error)
    { assert( 0 ) ; } // no bindings for this value_type

    template <>
    void besj< float >(const fcomplex * z, const fcomplex * fnu, const int * kode, const int * n, fcomplex* cy, int * nz, int * error) ;
    
    template <>
    void besj< double >(const dcomplex * z, const dcomplex * fnu, const int * kode, const int * n, dcomplex* cy, int * nz, int * error) ;

    template <>
    void besj< std::complex< float > >(const fcomplex * z, const fcomplex * fnu, const int * kode, const int * n, fcomplex* cy, int * nz, int * error) ;
    
    template <>
    void besj< std::complex< double > >(const dcomplex * z, const dcomplex * fnu, const int * kode, const int * n, dcomplex* cy, int * nz, int * error) ;


    //
    // [CZ]BESY
    //
    template < typename value_type > // actually bind_type
    void besy(const typename value_traits< value_type >::value_type * z, 
              const typename value_traits< value_type >::value_type * fnu, 
              const int                                             * kode, 
              const int                                             * n, 
              typename value_traits< value_type >::value_type       * cy, 
              int                                                   * nz, 
              typename value_traits< value_type >::value_type       * wrk, 
              int                                                   * error)
    { assert( 0 ) ; } // no bindings for this value_type

    template <>
    void besy< float >(const float * z, const float * fnu, const int * kode, const int * n, float* cy, int * nz, float * wrk, int * error) ;
    
    template <>
    void besy< double >(const double * z, const double * fnu, const int * kode, const int * n, double* cy, int * nz, double * wrk, int * error) ;

    template <>
    void besy< std::complex< float > >(const float * z, const float * fnu, const int * kode, const int * n, float* cy, int * nz, float * wrk, int * error) ;
    
    template <>
    void besy< std::complex< double > >(const double * z, const double * fnu, const int * kode, const int * n, double* cy, int * nz, double * wrk, int * error) ;
  }

  template < typename vector_type >
  int besi(const typename vector_type::value_type&                                             z,   // std::complex< float > or std::complex< double >
           const typename traits::value_traits< typename vector_type::value_type >::value_type fnu, // float or double
           int                                                                                 kode, 
           vector_type&                                                                        cy, 
           int&                                                                                nz) 
  {
    typedef typename vector_type::value_type                                        value_type ;
    typedef typename traits::value_traits< value_type >::value_type                 bind_type ;
    typedef typename boost::numeric::bindings::traits::vector_traits< vector_type > vector_traits ;
    int n = vector_traits::size( cy ) ;
    const value_type * cy_ptr = vector_traits::storage( cy ) ;
    int error = 0 ;
    detail::besi< value_type >( (const bind_type*)&z, (const bind_type*)&fnu, &kode, &n, (bind_type*)cy_ptr, &nz, &error ) ;
    return error ;
  }

  template < typename vector_type >
  int besj(const typename vector_type::value_type&                                              z, 
           const typename traits::value_traits< typename vector_type::value_type >::value_type  fnu, 
           int                                                                                  kode, 
           vector_type&                                                                         cy, 
           int&                                                                                 nz) 
  {
    typedef typename vector_type::value_type                           value_type ;
    typedef typename traits::value_traits< value_type >::value_type    bind_type ;
    typedef typename traits::vector_traits< vector_type >              vector_traits ;
    int n = vector_traits::size( cy ) ;
    const value_type * cy_ptr = vector_traits::storage( cy ) ;
    int error = 0 ;
    detail::besj< value_type >( (const bind_type*)&z, (const bind_type*)&fnu, &kode, &n, (bind_type*)cy_ptr, &nz, &error ) ;
    return error ;
  }

  template < typename vector_type >
  int besy(const typename vector_type::value_type&                                             z, 
           const typename traits::value_traits< typename vector_type::value_type >::value_type fnu, 
           int                                                                                 kode, 
           vector_type&                                                                        cy, 
           int&                                                                                nz) 
  {
    typedef typename vector_type::value_type                          value_type ;
    typedef typename traits::value_traits< value_type >::value_type   bind_type ;
    typedef typename traits::vector_traits< vector_type >             vector_traits ;
    int n = vector_traits::size( cy ) ;
    const value_type * cy_ptr = vector_traits::storage( cy ) ;
    value_type * cwrk = new value_type[n];
    int error = 0 ;
    detail::besy< value_type >( (const bind_type*)&z, (const bind_type*)&fnu, &kode, &n, (bind_type*)cy_ptr, &nz, cwrk, &error ) ;
    delete[] cwrk ;
    return error ;
  }

}}}}

#endif // boost_numeric_bindings_amos_amos_hpp

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

namespace boost { namespace numeric { namespace bindings { namespace amos {

  namespace detail
  {
    template < typename value_type > // actually bind_type
    void besi(const value_type * z, const value_type * fnu, const int * kode, const int * n, value_type* cy, int * nz, int * error)
    {}

    template < typename value_type > // actually bind_type
    void besj(const value_type * z, const value_type * fnu, const int * kode, const int * n, value_type* cy, int * nz, int * error)
    {}

    template < typename value_type > // actually bind_type
    void besy(const value_type * z, const value_type * fnu, const int * kode, const int * n, value_type* cy, int * nz, value_type * wrk, int * error)
    {}
  }

  template < typename vector_type >
  int besi(const vector_type& z, const typename boost::numeric::bindings::traits::value_traits< typename vector_type::value_type >::value_type fnu, int kode, vector_type& cy, int& nz) 
  {
    typedef typename vector_type::value_type                                                   value_type ;
    typedef typename boost::numeric::bindings::traits::value_traits< value_type >::value_type  bind_type ;
    typedef typename boost::numeric::bindings::traits::vector_traits< const vector_type >      const_vector_traits ;
    typedef typename boost::numeric::bindings::traits::vector_traits<       vector_type >      vector_traits ;
    int n = const_vector_traits::size( z ) ;
    const value_type * z_ptr  = const_vector_traits::storage( z ) ;
    const value_type * cy_ptr = vector_traits::storage( cy ) ;
    int error = 0 ;
    detail::besi( (const bind_type*)z_ptr, (const bind_type*)&fnu, &kode, &n, (bind_type*)cy_ptr, &nz, &error ) ;
    return error ;
  }

  template < typename vector_type >
  int besj(const vector_type& z, const typename boost::numeric::bindings::traits::value_traits< typename vector_type::value_type >::value_type fnu, int kode, vector_type& cy, int& nz) 
  {
    typedef typename vector_type::value_type                                               value_type ;
    typedef typename boost::numeric::bindings::traits::value_traits< value_type >::value_type  bind_type ;
    typedef typename boost::numeric::bindings::traits::vector_traits< const vector_type >  const_vector_traits ;
    typedef typename boost::numeric::bindings::traits::vector_traits< vector_type >        vector_traits ;
    int n = const_vector_traits::size( z ) ;
    const value_type * z_ptr  = const_vector_traits::storage( z ) ;
    const value_type * cy_ptr = vector_traits::storage( cy ) ;
    int error = 0 ;
    detail::besj( (const bind_type*)z_ptr, (const bind_type*)&fnu, &kode, &n, (bind_type*)cy_ptr, &nz, &error ) ;
    return error ;
  }

  template < typename vector_type >
  int besy(const vector_type& z, const typename boost::numeric::bindings::traits::value_traits< typename vector_type::value_type >::value_type fnu, int kode, vector_type& cy, int& nz) 
  {
    typedef typename vector_type::value_type                                               value_type ;
    typedef typename boost::numeric::bindings::traits::value_traits< value_type >::value_type  bind_type ;
    typedef typename boost::numeric::bindings::traits::vector_traits< const vector_type >  const_vector_traits ;
    typedef typename boost::numeric::bindings::traits::vector_traits< vector_type >        vector_traits ;
    int n = const_vector_traits::size( z ) ;
    const value_type * z_ptr  = const_vector_traits::storage( z ) ;
    const value_type * cy_ptr = vector_traits::storage( cy ) ;
    value_type * cwrk = new value_type[n];
    int error = 0 ;
    detail::besy( (const bind_type*)z_ptr, (const bind_type*)&fnu, &kode, &n, (bind_type*)cy_ptr, &nz, cwrk, &error ) ;
    delete[] cwrk ;
    return error ;
  }

}}}}

#endif // boost_numeric_bindings_amos_amos_hpp

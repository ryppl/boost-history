#ifndef boost_numeric_bindings_lapack_traits_hpp
#define boost_numeric_bindings_lapack_traits_hpp

#include <boost/numeric/bindings/traits/type.hpp>
#include <boost/numeric/bindings/traits/value_traits.hpp>

namespace boost { namespace numeric { namespace bindings { namespace lapack {

  template < typename T >
  struct traits
  {
    typedef typename value_traits< T >::value_type bind_type ;

    typedef void (*getrf_type)(int* m, int* n, bind_type* a, int* lda, int* ipiv, int* info) ;
    typedef void (*getrs_type)(char* trans, int *n, int* nrhs, bind_type* a, int* lda, int* ipiv, bind_type* b, int* ldb, int* info);

    static getrf_type getrf ;
    static getrs_type getrs ;
  };

  extern double* work_buffer ;
  extern int work_buffer_size ;

  extern double* rwork_buffer ;
  extern int rwork_buffer_size ;

  extern int* bwork_buffer ;
  extern int bwork_buffer_size ;

}}}}

#endif // boost_numeric_bindings_lapack_traits_hpp

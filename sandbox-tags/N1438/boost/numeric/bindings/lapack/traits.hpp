#ifndef boost_numeric_bindings_lapack_traits_hpp
#define boost_numeric_bindings_lapack_traits_hpp

#include <boost/numeric/bindings/traits/type.hpp>
#include <boost/numeric/bindings/traits/value_traits.hpp>

namespace boost { namespace numeric { namespace bindings { namespace lapack {

  template < typename T >
  struct traits
  {
    typedef typename ::boost::numeric::bindings::traits::value_traits< T >::value_type bind_type ;

    typedef void (*getrf_type)(const int* m, const int* n, bind_type* a, const int* lda, int* ipiv, int* info) ;
    typedef void (*getrs_type)(const char* trans, const int *n, const int* nrhs, const bind_type* a, const int* lda, const int* ipiv, bind_type* b, const int* ldb, int* info);

    typedef void (*sytrf_type)(const char* m, const int* n, bind_type* a, const int* lda, int* ipiv, bind_type* work, const int* lwork, int* info) ;
    typedef void (*sytrs_type)(const char* trans, const int *n, const int* nrhs, const bind_type* a, const int* lda, const int* ipiv, bind_type* b, const int* ldb, int* info);

    static getrf_type getrf ;
    static getrs_type getrs ;

    static sytrf_type sytrf ;
    static sytrs_type sytrs ;
  };

  extern double* work_buffer ;
  extern int work_buffer_size ;

  extern double* rwork_buffer ;
  extern int rwork_buffer_size ;

  extern int* bwork_buffer ;
  extern int bwork_buffer_size ;

}}}}

#endif // boost_numeric_bindings_lapack_traits_hpp

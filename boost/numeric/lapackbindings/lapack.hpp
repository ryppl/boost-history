#ifndef boost_numeric_lapackbindings_lapack_hpp
#define boost_numeric_lapackbindings_lapack_hpp

#include <boost/numeric/blasbindings/type.hpp>
#include <boost/numeric/blasbindings/valuetraits.hpp>

namespace boost { namespace numeric { namespace lapackbindings {

  template < typename T >
  struct lapack
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

}}}

#endif // boost_numeric_lapackbindings_lapack_hpp

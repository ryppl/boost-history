// Copyright blablabla

#include <boost/numeric/bindings/lapack/traits.hpp>
#include <boost/numeric/bindings/lapack/lapack.h>

namespace boost { namespace numeric { namespace bindings { namespace lapack {

  double* work_buffer = NULL;
  int work_buffer_size = 0;

  double* rwork_buffer = NULL;
  int rwork_buffer_size = 0;

  int* bwork_buffer = NULL;
  int bwork_buffer_size = 0;

  template<> traits< double         >::getrf_type  traits< double         >::getrf = LAPACK_DGETRF ;
  template<> traits< double_complex >::getrf_type  traits< double_complex >::getrf = LAPACK_ZGETRF ;

  template<> traits< double         >::getrs_type  traits< double         >::getrs = LAPACK_DGETRS ;
  template<> traits< double_complex >::getrs_type  traits< double_complex >::getrs = LAPACK_ZGETRS ;

}}}}

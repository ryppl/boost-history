// Copyright blablabla

#include <boost/numeric/lapackbindings/lapack.hpp>
#include <boost/numeric/lapackbindings/lapackbindings.hpp>
#include <boost/numeric/lapackbindings/lapack.h>

namespace boost { namespace numeric { namespace lapackbindings {

  double* work_buffer = NULL;
  int work_buffer_size = 0;

  double* rwork_buffer = NULL;
  int rwork_buffer_size = 0;

  int* bwork_buffer = NULL;
  int bwork_buffer_size = 0;

  template<> lapack< double         >::getrf_type  lapack< double         >::getrf = LAPACK_DGETRF ;
  template<> lapack< double_complex >::getrf_type  lapack< double_complex >::getrf = LAPACK_ZGETRF ;

  template<> lapack< double         >::getrs_type  lapack< double         >::getrs = LAPACK_DGETRS ;
  template<> lapack< double_complex >::getrs_type  lapack< double_complex >::getrs = LAPACK_ZGETRS ;

}}}

// Copyright blablabla

#include <boost/numeric/ublas/lapack.hpp>

namespace boost { namespace numerics {
  double* work_buffer = NULL;
  int work_buffer_size = 0;
  int* bwork_buffer = NULL;
  int bwork_buffer_size = 0;

  lapack_traits< double >::getrf_type  lapack_traits< double >::getrf = LAPACK_DGETRF ;
  lapack_traits< double >::getrs_type  lapack_traits< double >::getrs = LAPACK_DGETRS ;
  // lapack_traits< double >::sytrs_type  lapack_traits< double >::sytrs = LAPACK_DSYTRS ;
  lapack_traits< double >::geev_type  lapack_traits< double >::geev = LAPACK_DGEEV ;
  lapack_traits< double >::gees_type  lapack_traits< double >::gees = LAPACK_DGEES ;
  
  lapack_traits< std::complex< double > >::getrf_type  lapack_traits< std::complex< double > >::getrf = LAPACK_ZGETRF ;
  lapack_traits< std::complex< double > >::getrs_type  lapack_traits< std::complex< double > >::getrs = LAPACK_ZGETRS ;
  // lapack_traits< std::complex< double > >::sytrs_type  lapack_traits< std::complex< double > >::sytrs = LAPACK_ZSYTRS ;
  lapack_traits< std::complex< double > >::geev_type  lapack_traits< std::complex< double > >::geev = LAPACK_ZGEEV ;
  lapack_traits< std::complex< double > >::gees_type  lapack_traits< std::complex< double > >::gees = LAPACK_ZGEES ;

}}

// Copyright blablabla

#include <boost/numeric/ublas/lapack.hpp>

namespace boost { namespace numerics {
  lapack_traits< double >::getrf_type  lapack_traits< double >::getrf = LAPACK_DGETRF ;
  lapack_traits< double >::getrs_type  lapack_traits< double >::getrs = LAPACK_DGETRS ;
  // lapack_traits< double >::sytrs_type  lapack_traits< double >::sytrs = LAPACK_DSYTRS ;
  
  lapack_traits< std::complex< double > >::getrf_type  lapack_traits< std::complex< double > >::getrf = LAPACK_ZGETRF ;
  lapack_traits< std::complex< double > >::getrs_type  lapack_traits< std::complex< double > >::getrs = LAPACK_ZGETRS ;
  // lapack_traits< std::complex< double > >::sytrs_type  lapack_traits< std::complex< double > >::sytrs = LAPACK_ZSYTRS ;
}}

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
  template<> traits< boost::numeric::bindings::traits::double_complex >::getrf_type  traits< boost::numeric::bindings::traits::double_complex >::getrf = LAPACK_ZGETRF ;

  template<> traits< double         >::getrs_type  traits< double         >::getrs = LAPACK_DGETRS ;
  template<> traits< boost::numeric::bindings::traits::double_complex >::getrs_type  traits< boost::numeric::bindings::traits::double_complex >::getrs = LAPACK_ZGETRS ;
 
  template<> traits< double         >::sytrf_type  traits< double         >::sytrf = LAPACK_DSYTRF ;
  template<> traits< boost::numeric::bindings::traits::double_complex >::sytrf_type  traits< boost::numeric::bindings::traits::double_complex >::sytrf = LAPACK_ZSYTRF ;

  template<> traits< double         >::sytrs_type  traits< double         >::sytrs = LAPACK_DSYTRS ;
  template<> traits< boost::numeric::bindings::traits::double_complex >::sytrs_type  traits< boost::numeric::bindings::traits::double_complex >::sytrs = LAPACK_ZSYTRS ;
 
  template<> traits< double         >::potrf_type  traits< double         >::potrf = LAPACK_DPOTRF ;
  template<> traits< boost::numeric::bindings::traits::double_complex >::potrf_type  traits< boost::numeric::bindings::traits::double_complex >::potrf = LAPACK_ZPOTRF ;

  template<> traits< double         >::potrs_type  traits< double         >::potrs = LAPACK_DPOTRS ;
  template<> traits< boost::numeric::bindings::traits::double_complex >::potrs_type  traits< boost::numeric::bindings::traits::double_complex >::potrs = LAPACK_ZPOTRS ;
}}}}

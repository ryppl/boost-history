// Copyright blablabla

#include <boost/numeric/ublas/lapack.hpp>

namespace boost { namespace numerics {
  double* work_buffer = NULL;
  int work_buffer_size = 0;
  double* rwork_buffer = NULL;
  int rwork_buffer_size = 0;
  int* bwork_buffer = NULL;
  int bwork_buffer_size = 0;

  lapack_traits< double >::getrf_type  lapack_traits< double >::getrf = LAPACK_DGETRF ;
  lapack_traits< double >::getrs_type  lapack_traits< double >::getrs = LAPACK_DGETRS ;
  // lapack_traits< double >::sytrs_type  lapack_traits< double >::sytrs = LAPACK_DSYTRS ;
  
  lapack_traits< std::complex< double > >::getrf_type  lapack_traits< std::complex< double > >::getrf = LAPACK_ZGETRF ;
  lapack_traits< std::complex< double > >::getrs_type  lapack_traits< std::complex< double > >::getrs = LAPACK_ZGETRS ;
  // lapack_traits< std::complex< double > >::sytrs_type  lapack_traits< std::complex< double > >::sytrs = LAPACK_ZSYTRS ;

  template <>
  int geev< double >(char jobvl, char jobvr, matrix< double, column_major >& a, vector< std::complex< double > >& w, matrix< double, column_major >& vl, matrix< double, column_major >& vr) 
  {
    static const int buffer_size_ratio = 2 ;
    assert( jobvl == 'N' || jobvl == 'V' );
    assert( jobvr == 'N' || jobvr == 'V' );
    assert( a.size1() == a.size2() );
    assert( w.size() >= a.size1() );
    int n = a.size2();
    int lda = a.size1();
    double* wr = (double*)(&w[0]);
    double* wi = wr + n ; 
    int ldvl = vl.size1();
    int ldvr = vr.size1();
    int lwork = buffer_size_ratio * 3 * n;
    if ( lwork > work_buffer_size ) {
      delete[] work_buffer ;
      work_buffer = new double[ lwork ];
      work_buffer_size = lwork ;
    }
    int info = 0;
    LAPACK_DGEEV( &jobvl, &jobvr, &n, &(a.data()[0]), &lda, wr, wi, &(vl.data()[0]), &ldvl, &(vr.data()[0]), &ldvr, work_buffer, &work_buffer_size, &info );
    double* wr_it = wr + 1; 
    double* wi_it = wi ;
    for ( ; wr_it < wi ; wr_it += 2, wi_it += 2 ) { std::swap( *wr_it, *wi_it ); }
    return info;
  }

  template <>
  int geev< std::complex< double > >(char jobvl, char jobvr, matrix< std::complex< double >, column_major >& a, vector< std::complex< double > >& w, matrix< std::complex< double >, column_major >& vl, matrix< std::complex< double >, column_major >& vr) 
  {
    static const int buffer_size_ratio = 2 ;
    assert( jobvl == 'N' || jobvl == 'V' );
    assert( jobvr == 'N' || jobvr == 'V' );
    assert( a.size1() == a.size2() );
    assert( w.size() >= a.size1() );
    int n = a.size2();
    int lda = a.size1();
    int ldvl = vl.size1();
    int ldvr = vr.size1();
    int lwork = buffer_size_ratio * 2 * 2 * n;
    if ( lwork > work_buffer_size ) {
      delete[] work_buffer ;
      work_buffer = new double[ lwork ];
      work_buffer_size = lwork ;
    }
    int rwork = 2 * n;
    if ( rwork > rwork_buffer_size ) {
      delete[] rwork_buffer;
      rwork_buffer = new double[ rwork ];
    }
    int info = 0;
    LAPACK_ZGEEV( &jobvl, &jobvr, &n, &(a.data()[0]), &lda, &(w[0]), &(vl.data()[0]), &ldvl, &(vr.data()[0]), &ldvr, (std::complex<double>*)work_buffer, &work_buffer_size, rwork_buffer, &info );
    return info;
  }

  int fortran_select()
  { return 0; }

  template <>
  int gees< double >(char jobvl, char sort, matrix< double, column_major >& a, int& sdim, vector< std::complex< double > >& w, matrix< double, column_major >& vs) 
  {
    const int ratio = 2 ;
    assert( a.size1() == a.size2() );
    int n = a.size2();
    int lda = a.size1();
    double* wr = (double*)(&w[0]);
    double* wi = wr + n ; 
    int ldvs = vs.size2();
    if ( work_buffer_size < ratio * 3 * n ) {
      work_buffer_size = ratio * 3 * n;
      delete[] work_buffer;
      work_buffer = new double[ work_buffer_size ];
    }
    if ( bwork_buffer_size < n ) {
      bwork_buffer_size = n;
      delete[] bwork_buffer;
      bwork_buffer = new int[ bwork_buffer_size ];
    }
    int info = 0;
    LAPACK_DGEES( &jobvl, &sort, (fortran_function_type)fortran_select, &n, &(a.data()[0]), &lda, &sdim, wr, wi, &(vs.data()[0]), &ldvs, work_buffer, &work_buffer_size, bwork_buffer, &info );
    double* wr_it = wr + 1; 
    double* wi_it = wi ;
    for ( ; wr_it < wi ; wr_it += 2, wi_it += 2 ) { std::swap( *wr_it, *wi_it ); }
    return info;
  }

  template <>
  int gees< std::complex< double > >(char jobvl, char sort, matrix< std::complex< double >, column_major >& a, int& sdim, vector< std::complex< double > >& w, matrix< std::complex< double >, column_major >& vs) 
  {
    const int ratio = 2 ;
    assert( a.size1() == a.size2() );
    int n = a.size2();
    int lda = a.size1();
    int ldvs = vs.size2();
    if ( work_buffer_size < ratio * 2 * 2 * n ) {
      work_buffer_size = ratio * 2 * 2 * n;
      delete[] work_buffer;
      work_buffer = new double[ work_buffer_size ];
    }
    if ( rwork_buffer_size < n ) {
      rwork_buffer_size = n;
      delete[] rwork_buffer ;
      rwork_buffer = new double[ rwork_buffer_size ];
    }
    if ( bwork_buffer_size < n ) {
      bwork_buffer_size = n;
      delete[] bwork_buffer;
      bwork_buffer = new int[ bwork_buffer_size ];
    }
    int info = 0;
    LAPACK_ZGEES( &jobvl, &sort, (fortran_function_type)fortran_select, &n, &(a.data()[0]), &lda, &sdim, &w[0], &(vs.data()[0]), &ldvs, (std::complex<double>*)work_buffer, &work_buffer_size, rwork_buffer, bwork_buffer, &info );
    return info;
  }
}}

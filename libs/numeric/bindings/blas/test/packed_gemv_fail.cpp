//  Permission to copy, use, modify, sell and
//  distribute this software is granted provided this copyright notice appears
//  in all copies. This software is provided "as is" without express or implied
//  warranty, and with no claim as to its suitability for any purpose.
//  Copyright Toon Knapen

// test wether the compiler correctly refuses to perform
// a gemv on a non-dense matrix

#include <boost/numeric/bindings/blas/blas2.hpp>
#include <boost/numeric/bindings/traits/ublas_vector.hpp>
#include <boost/numeric/bindings/traits/ublas_matrix.hpp>

int main()
{
  boost::numeric::ublas::vector< double > v(1) ;
  boost::numeric::ublas::symmetric_matrix< double > a(1,1) ;
  boost::numeric::bindings::blas::gemv( a, v, v ) ;
  return 0 ;
}

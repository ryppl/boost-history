//
//  Copyright Toon Knapen, Karl Meerbergen
//  Copyright Thomas Klimpel 2008
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//

#include "ublas_heev.hpp"

#include <boost/numeric/bindings/lapack/heevx.hpp>
#include <boost/numeric/bindings/traits/ublas_matrix.hpp>
#include <boost/numeric/bindings/traits/ublas_vector.hpp>
#include <boost/numeric/bindings/traits/ublas_hermitian.hpp>
#include <boost/numeric/ublas/matrix_proxy.hpp>
#include <boost/numeric/ublas/vector_proxy.hpp>
#include <boost/numeric/ublas/io.hpp>

#include <iostream>


namespace ublas = boost::numeric::ublas;
namespace lapack = boost::numeric::bindings::lapack;


template <typename T, typename W, typename UPLO>
int do_memory_uplo(int n, W& workspace ) {
   typedef typename boost::numeric::bindings::traits::type_traits<T>::real_type real_type ;

   typedef ublas::matrix<T, ublas::column_major>     matrix_type ;
   typedef ublas::vector<real_type>                  vector_type ;

   typedef ublas::hermitian_adaptor<matrix_type, UPLO> hermitian_type;

   // Set matrix
   matrix_type a( n, n ); a.clear();
   vector_type e1( n );
   vector_type e2( n );

   fill( a );
   matrix_type a2( a );
   matrix_type z( a );

   // Compute Schur decomposition.
   integer_t m;
   ublas::vector<integer_t> ifail(n);
   
   hermitian_type h_a( a );
   lapack::heevx( 'V', 'A', h_a, real_type(0.0), real_type(1.0), 2, n-1, real_type(1e-28), m,
                  e1, z, ifail, workspace ) ;

   if (check_residual( a2, e1, z )) return 255 ;

   hermitian_type h_a2( a2 );
   lapack::heevx( 'N', 'A', h_a2, real_type(0.0), real_type(1.0), 2, n-1, real_type(1e-28), m,
                  e2, z, ifail, workspace ) ;
   if (norm_2( e1 - e2 ) > n * norm_2( e1 ) * std::numeric_limits< real_type >::epsilon()) return 255 ;

   // Test for a matrix range
   fill( a ); a2.assign( a );

   typedef ublas::matrix_range< matrix_type > matrix_range ;
   typedef ublas::hermitian_adaptor<matrix_range, UPLO> hermitian_range_type;

   ublas::range r(1,n-1) ;
   matrix_range a_r( a, r, r );
   matrix_range z_r( z, r, r );
   ublas::vector_range< vector_type> e_r( e1, r );
   ublas::vector<integer_t> ifail_r(n-2);

   hermitian_range_type h_a_r( a_r );
   lapack::heevx( 'V', 'A', h_a_r, real_type(0.0), real_type(1.0), 2, n-1, real_type(1e-28), m,
                  e_r, z_r, ifail_r, workspace );

   matrix_range a2_r( a2, r, r );
   if (check_residual( a2_r, e_r, z_r )) return 255 ;

   return 0 ;
} // do_memory_uplo()


template <typename T, typename W>
int do_memory_type(int n, W workspace) {
   std::cout << "  upper\n" ;
   if (do_memory_uplo<T,W,ublas::upper>(n, workspace)) return 255 ;
   std::cout << "  lower\n" ;
   if (do_memory_uplo<T,W,ublas::lower>(n, workspace)) return 255 ;
   return 0 ;
}


template <typename T>
struct Workspace {
   typedef ublas::vector< T >                                               array_type ;
   typedef ublas::vector< integer_t >                                       int_array_type ;
   typedef lapack::detail::workspace1< array_type >                         first_type;
   typedef lapack::detail::workspace1< int_array_type >                     second_type;
   typedef std::pair<first_type, second_type>                               type ;

   Workspace(size_t n)
   : work_( 8*n )
   , iwork_( 5*n )
   {}

   type operator() () {
      return type( first_type( work_ ), second_type( iwork_ ) );
   }

   array_type work_ ;
   int_array_type     iwork_ ;
};

template <typename T>
struct Workspace< std::complex<T> > {
   typedef ublas::vector< std::complex<T> >                                 complex_array_type ;
   typedef ublas::vector< T >                                               real_array_type ;
   typedef ublas::vector< integer_t >                                       int_array_type ;
   typedef lapack::detail::workspace2< complex_array_type,real_array_type > first_type;
   typedef lapack::detail::workspace1< int_array_type >                     second_type;
   typedef std::pair<first_type, second_type>                               type ;

   Workspace(size_t n)
   : work_( 2*n )
   , rwork_( 7*n )
   , iwork_( 5*n )
   {}

   type operator() () {
      return type( first_type( work_, rwork_ ), second_type( iwork_ ) );
   }

   complex_array_type work_ ;
   real_array_type    rwork_ ;
   int_array_type     iwork_ ;
};


template <typename T>
int do_value_type() {
   const int n = 8 ;

   std::cout << " optimal workspace\n";
   if (do_memory_type<T,lapack::optimal_workspace>( n, lapack::optimal_workspace() ) ) return 255 ;

   std::cout << " minimal workspace\n";
   if (do_memory_type<T,lapack::minimal_workspace>( n, lapack::minimal_workspace() ) ) return 255 ;

   std::cout << " workspace array\n";
   Workspace<T> work( n );
   do_memory_type<T,typename Workspace<T>::type >( n, work() );
   return 0;
} // do_value_type()


int main() {
   // Run tests for different value_types
   std::cout << "float\n" ;
   if (do_value_type< float >()) return 255;

   std::cout << "double\n" ;
   if (do_value_type< double >()) return 255;

   std::cout << "complex<float>\n" ;
   if (do_value_type< std::complex<float> >()) return 255;

   std::cout << "complex<double>\n" ;
   if (do_value_type< std::complex<double> >()) return 255;

   std::cout << "Regression test succeeded\n" ;
   return 0;
}


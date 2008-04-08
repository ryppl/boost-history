/*
 * 
 * Copyright (c) Kresimir Fresl 2002 
 *
 * Permission to copy, modify, use and distribute this software 
 * for any non-commercial or commercial purpose is granted provided 
 * that this license appear on all copies of the software source code.
 *
 * Author assumes no responsibility whatsoever for its use and makes 
 * no guarantees about its quality, correctness or reliability.
 *
 * Author acknowledges the support of the Faculty of Civil Engineering, 
 * University of Zagreb, Croatia.
 *
 */

#ifndef BOOST_NUMERIC_BINDINGS_TRAITS_DETAIL_UBLAS_ORDERING_H
#define BOOST_NUMERIC_BINDINGS_TRAITS_DETAIL_UBLAS_ORDERING_H

#include <boost/numeric/ublas/fwd.hpp> 

namespace boost { namespace numeric { namespace bindings { namespace traits {

  namespace detail {

    template <typename StOrdTag>
    struct ublas_ordering {};
    
    template<> 
    struct ublas_ordering<boost::numeric::ublas::row_major_tag> {
      typedef row_major_t                        type; 
      typedef boost::numeric::ublas::row_major   functor_type; 

      template <typename M>
      static int leading_dimension( M const& m ) {
        return m.size2() ;
      }

      template <typename M>
      static int stride1( M const& m ) {
        return m.size2() ;
      }

      template <typename M>
      static int stride2( M const& m ) {
        return 1 ;
      }
    };
    
    template<> 
    struct ublas_ordering<boost::numeric::ublas::column_major_tag> {
      typedef column_major_t                        type; 
      typedef boost::numeric::ublas::column_major   functor_type; 

      template <typename M>
      static int leading_dimension( M const& m ) {
        return m.size1() ;
      }

      template <typename M>
      static int stride1( M const& m ) {
        return 1 ;
      }

      template <typename M>
      static int stride2( M const& m ) {
        return m.size1() ;
      }
    };

    template <typename StOrdTag>
    struct ublas_banded_ordering {};
    
    template<> 
    struct ublas_banded_ordering<boost::numeric::ublas::row_major_tag> {

      template <typename M>
      static int leading_dimension( M const& m ) {
        return m.lower() + m.upper() + 1 ;
      }

      template <typename M>
      static int stride1( M const& m ) {
        return 1 ;
      }

      template <typename M>
      static int stride2( M const& m ) {
        return leading_dimension(m)-1 ;
      }
    };
    
    template<> 
    struct ublas_banded_ordering<boost::numeric::ublas::column_major_tag> {

      template <typename M>
      static int leading_dimension( M const& m ) {
        return m.size2() ;
      }

      template <typename M>
      static int stride1( M const& m ) {
        return leading_dimension(m) ;
      }

      template <typename M>
      static int stride2( M const& m ) {
        return 1-leading_dimension(m) ;
      }
    };
  }

}}}}

#endif 

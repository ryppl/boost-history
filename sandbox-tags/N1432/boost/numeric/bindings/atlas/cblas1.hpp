// (-*- C++ -*- header)  

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

#ifndef BOOST_NUMERIC_CBLAS_LEVEL_1_HPP
#define BOOST_NUMERIC_CBLAS_LEVEL_1_HPP

#include <cassert>

#include <boost/numeric/bindings/traits/traits.hpp>
#include <boost/numeric/bindings/atlas/cblas1_overloads.hpp>

#ifdef BOOST_BINDINGS_CHECK_TYPES
#  include <boost/type_traits/same_traits.hpp>
#  include <boost/static_assert.hpp>
#endif 

namespace boost { namespace numeric { namespace bindings { 

  namespace atlas {

    namespace detail {

      template <typename Real> 
      struct type_traits { 
	typedef Real type; 
	typedef Real real_type; 
      };
      template <typename Real> 
      struct type_traits<std::complex<Real> > { 
	typedef std::complex<Real> type; 
	typedef Real real_type; 
      };

    }

    // x_i <- alpha for all i
    template <typename T, typename Vct> 
    inline 
    void set (T const alpha, Vct& x) {
      typedef traits::vector_traits<Vct> vtraits;
      detail::set (vtraits::size (x),
		   alpha, vtraits::storage (x), vtraits::stride (x)); 
    }

    // y <- x
    template <typename VctX, typename VctY>
    inline 
    void copy (VctX const& x, VctY& y) {
      typedef traits::vector_traits<VctX const> xvtraits;
      typedef traits::vector_traits<VctY> yvtraits;
      assert (yvtraits::size (y) >= xvtraits::size (x));
      detail::copy (xvtraits::size (x),
		    xvtraits::storage (x), xvtraits::stride (x), 
		    yvtraits::storage (y), yvtraits::stride (y)); 
    }

    // x <-> y
    template <typename VctX, typename VctY>
    inline 
    void swap (VctX& x, VctY& y) {
      typedef traits::vector_traits<VctX> xvtraits;
      typedef traits::vector_traits<VctY> yvtraits;
      assert (yvtraits::size (y) >= xvtraits::size (x));
      detail::swap (xvtraits::size (x),
		    xvtraits::storage (x), xvtraits::stride (x), 
		    yvtraits::storage (y), yvtraits::stride (y)); 
    }

    // x <- alpha * x
    template <typename T, typename Vct> 
    inline 
    void scal (T const alpha, Vct& x) {
      typedef traits::vector_traits<Vct> vtraits;
#ifdef BOOST_BINDINGS_CHECK_TYPES
    BOOST_STATIC_ASSERT(
      (boost::is_same<T, typename vtraits::value_type>::value
       || 
       boost::is_same<T, 
         typename detail::type_traits<typename vtraits::value_type>::real_type
       >::value
       ));
#endif 
      detail::scal (vtraits::size (x),
		    alpha, vtraits::storage (x), vtraits::stride (x)); 
    }

    // y <- alpha * x + y
    template <typename T, typename VctX, typename VctY>
    inline 
    void axpy (T const alpha, VctX const& x, VctY& y) {
      typedef traits::vector_traits<VctX const> xvtraits;
      typedef traits::vector_traits<VctY> yvtraits;
      assert (yvtraits::size (y) >= xvtraits::size (x));
      detail::axpy (xvtraits::size (x),
		    alpha, xvtraits::storage (x), xvtraits::stride (x), 
		    yvtraits::storage (y), yvtraits::stride (y)); 
    }

    // y <- alpha * x + beta * y
    template <typename T, typename VctX, typename VctY>
    inline 
    void axpby (T const alpha, VctX const& x, 
		T const beta, VctY& y) { 
      typedef traits::vector_traits<VctX const> xvtraits;
      typedef traits::vector_traits<VctY> yvtraits;
      assert (yvtraits::size (y) >= xvtraits::size (x));
      detail::axpby (xvtraits::size (x),
		     alpha, xvtraits::storage (x), xvtraits::stride (x), 
		     beta, yvtraits::storage (y), yvtraits::stride (y)); 
    }

    // dot <- x^T * y 
    // .. real & complex types
    template <typename VctX, typename VctY>
    inline 
    typename traits::vector_traits<VctX>::value_type 
    dot (VctX const& x, VctY const& y) {
      typedef traits::vector_traits<VctX const> xvtraits;
      typedef traits::vector_traits<VctY const> yvtraits;
      assert (yvtraits::size (y) >= xvtraits::size (x));
      return detail::dot (xvtraits::size (x),
			  xvtraits::storage (x), xvtraits::stride (x), 
			  yvtraits::storage (y), yvtraits::stride (y)); 
    }

    // dot <- x^T * y 
    // .. float only -- with double accumulation
    template <typename VctX, typename VctY>
    inline 
    double dsdot (VctX const& x, VctY const& y) {
      typedef traits::vector_traits<VctX const> xvtraits;
      typedef traits::vector_traits<VctY const> yvtraits;
      assert (yvtraits::size (y) >= xvtraits::size (x));
      return cblas_dsdot (xvtraits::size (x),
			  xvtraits::storage (x), xvtraits::stride (x), 
			  yvtraits::storage (y), yvtraits::stride (y)); 
    }

    // apdot <- alpha + x^T * y    
    // .. float only -- computation uses double precision 
    template <typename VctX, typename VctY>
    inline 
    float sdsdot (float alpha, VctX const& x, VctY const& y) {
      typedef traits::vector_traits<VctX const> xvtraits;
      typedef traits::vector_traits<VctY const> yvtraits;
      assert (yvtraits::size (y) >= xvtraits::size (x));
      return cblas_sdsdot (xvtraits::size (x), alpha, 
			   xvtraits::storage (x), xvtraits::stride (x), 
			   yvtraits::storage (y), yvtraits::stride (y)); 
    }

    // dotu <- x^T * y 
    // .. complex types only
    // .. function
    template <typename VctX, typename VctY>
    inline 
    typename traits::vector_traits<VctX>::value_type 
    dotu (VctX const& x, VctY const& y) {
      typedef traits::vector_traits<VctX const> xvtraits;
      typedef traits::vector_traits<VctY const> yvtraits;
      assert (yvtraits::size (y) >= xvtraits::size (x));
      typename xvtraits::value_type val;
      detail::dotu (xvtraits::size (x),
		    xvtraits::storage (x), xvtraits::stride (x), 
		    yvtraits::storage (y), yvtraits::stride (y),
		    &val);
      return val; 
    }
    // .. procedure 
    template <typename VctX, typename VctY>
    inline 
    void dotu (VctX const& x, VctY const& y, 
	       typename traits::vector_traits<VctX>::value_type& val) {
      typedef traits::vector_traits<VctX const> xvtraits;
      typedef traits::vector_traits<VctY const> yvtraits;
      assert (yvtraits::size (y) >= xvtraits::size (x));
      detail::dotu (xvtraits::size (x),
		    xvtraits::storage (x), xvtraits::stride (x), 
		    yvtraits::storage (y), yvtraits::stride (y),
		    &val);
    }

    // dotc <- x^H * y 
    // .. complex types only
    // .. function
    template <typename VctX, typename VctY>
    inline 
    typename traits::vector_traits<VctX>::value_type 
    dotc (VctX const& x, VctY const& y) {
      typedef traits::vector_traits<VctX const> xvtraits;
      typedef traits::vector_traits<VctY const> yvtraits;
      assert (yvtraits::size (y) >= xvtraits::size (x));
      typename xvtraits::value_type val;
      detail::dotc (xvtraits::size (x),
		    xvtraits::storage (x), xvtraits::stride (x), 
		    yvtraits::storage (y), yvtraits::stride (y),
		    &val);
      return val; 
    }
    // .. procedure 
    template <typename VctX, typename VctY>
    inline 
    void dotc (VctX const& x, VctY const& y, 
	       typename traits::vector_traits<VctX>::value_type& val) {
      typedef traits::vector_traits<VctX const> xvtraits;
      typedef traits::vector_traits<VctY const> yvtraits;
      assert (yvtraits::size (y) >= xvtraits::size (x));
      detail::dotc (xvtraits::size (x),
		    xvtraits::storage (x), xvtraits::stride (x), 
		    yvtraits::storage (y), yvtraits::stride (y),
		    &val);
    }

    // nrm2 <- ||x||_2
    template <typename Vct> 
    inline 
    typename detail::type_traits<
      typename traits::vector_traits<Vct const>::value_type 
    >::real_type 
    nrm2 (Vct const& x) {
      typedef traits::vector_traits<Vct const> vtraits;
      return detail::nrm2 (vtraits::size (x),
			   vtraits::storage (x), vtraits::stride (x)); 
    }

    // asum <- ||re (x)|| + ||im (x)||
    template <typename Vct> 
    inline 
    typename detail::type_traits<
      typename traits::vector_traits<Vct const>::value_type 
    >::real_type 
    asum (Vct const& x) {
      typedef traits::vector_traits<Vct const> vtraits;
      return detail::asum (vtraits::size (x),
			   vtraits::storage (x), vtraits::stride (x)); 
    }

    // iamax <- 1st i: max (|re (x_i)| + |im (x_i)|)
    template <typename Vct> 
    inline 
    CBLAS_INDEX iamax (Vct const& x) {
      typedef traits::vector_traits<Vct const> vtraits;
      return detail::iamax (vtraits::size (x),
			    vtraits::storage (x), vtraits::stride (x)); 
    }

    // TO DO: plane rotations 

  } // namespace atlas

}}} 

#endif // BOOST_NUMERIC_CBLAS_LEVEL_1_HPP

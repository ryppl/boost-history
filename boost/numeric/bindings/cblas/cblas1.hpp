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
 * Zagreb, Croatia.
 *
 */

#ifndef BOOST_NUMERIC_CBLAS_LEVEL_1_HPP
#define BOOST_NUMERIC_CBLAS_LEVEL_1_HPP

#include <cassert>

#include <boost/numeric/bindings/traits/traits.hpp>
#include <boost/numeric/bindings/cblas/cblas1_overloads.hpp>

#ifdef BOOST_BINDINGS_CBLAS_CHECK_TYPES
#  include <boost/type_traits/same_traits.hpp>
#  include <boost/static_assert.hpp>
#endif 

namespace boost { namespace numeric { namespace bindings { 

  namespace cblas {

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

#ifdef ATLAS_BINDINGS

    // x_i <- alpha for all i
    template <typename T, typename vector_t> 
    inline 
    void set (T const alpha, vector_t& x) {
      typedef traits::vector_traits<vector_t> vtraits;
      detail::set (vtraits::size (x),
		   alpha, vtraits::storage (x), vtraits::stride (x)); 
    }

#endif 

    // y <- x
    template <typename vector_x_t, typename vector_y_t>
    inline 
    void copy (vector_x_t const& x, vector_y_t& y) {
      typedef traits::vector_traits<vector_x_t const> xvtraits;
      typedef traits::vector_traits<vector_y_t> yvtraits;
      assert (yvtraits::size (y) >= xvtraits::size (x));
      detail::copy (xvtraits::size (x),
		    xvtraits::storage (x), xvtraits::stride (x), 
		    yvtraits::storage (y), yvtraits::stride (y)); 
    }

    // x <-> y
    template <typename vector_x_t, typename vector_y_t>
    inline 
    void swap (vector_x_t& x, vector_y_t& y) {
      typedef traits::vector_traits<vector_x_t> xvtraits;
      typedef traits::vector_traits<vector_y_t> yvtraits;
      assert (yvtraits::size (y) >= xvtraits::size (x));
      detail::swap (xvtraits::size (x),
		    xvtraits::storage (x), xvtraits::stride (x), 
		    yvtraits::storage (y), yvtraits::stride (y)); 
    }

    // x <- alpha * x
    template <typename T, typename vector_t> 
    inline 
    void scal (T const alpha, vector_t& x) {
      typedef traits::vector_traits<vector_t> vtraits;
#ifdef BOOST_BINDINGS_CBLAS_CHECK_TYPES
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
    template <typename T, typename vector_x_t, typename vector_y_t>
    inline 
    void axpy (T const alpha, vector_x_t const& x, vector_y_t& y) {
      typedef traits::vector_traits<vector_x_t const> xvtraits;
      typedef traits::vector_traits<vector_y_t> yvtraits;
      assert (yvtraits::size (y) >= xvtraits::size (x));
      detail::axpy (xvtraits::size (x),
		    alpha, xvtraits::storage (x), xvtraits::stride (x), 
		    yvtraits::storage (y), yvtraits::stride (y)); 
    }

#ifdef ATLAS_BINDINGS

    // y <- alpha * x + beta * y
    template <typename T, typename vector_x_t, typename vector_y_t>
    inline 
    void axpby (T const alpha, vector_x_t const& x, 
		T const beta, vector_y_t& y) { 
      typedef traits::vector_traits<vector_x_t const> xvtraits;
      typedef traits::vector_traits<vector_y_t> yvtraits;
      assert (yvtraits::size (y) >= xvtraits::size (x));
      detail::axpby (xvtraits::size (x),
		     alpha, xvtraits::storage (x), xvtraits::stride (x), 
		     beta, yvtraits::storage (y), yvtraits::stride (y)); 
    }

#endif 

    // dot <- x^T * y 
    // .. real & complex types
    template <typename vector_x_t, typename vector_y_t>
    inline 
    typename traits::vector_traits<vector_x_t>::value_type 
    dot (vector_x_t const& x, vector_y_t const& y) {
      typedef traits::vector_traits<vector_x_t const> xvtraits;
      typedef traits::vector_traits<vector_y_t const> yvtraits;
      assert (yvtraits::size (y) >= xvtraits::size (x));
      return detail::dot (xvtraits::size (x),
			  xvtraits::storage (x), xvtraits::stride (x), 
			  yvtraits::storage (y), yvtraits::stride (y)); 
    }

    // dot <- x^T * y 
    // .. float only -- with double accumulation
    template <typename vector_x_t, typename vector_y_t>
    inline 
    double dsdot (vector_x_t const& x, vector_y_t const& y) {
      typedef traits::vector_traits<vector_x_t const> xvtraits;
      typedef traits::vector_traits<vector_y_t const> yvtraits;
      assert (yvtraits::size (y) >= xvtraits::size (x));
      return cblas_dsdot (xvtraits::size (x),
			  xvtraits::storage (x), xvtraits::stride (x), 
			  yvtraits::storage (y), yvtraits::stride (y)); 
    }

    // apdot <- alpha + x^T * y    
    // .. float only -- computation uses double precision 
    template <typename vector_x_t, typename vector_y_t>
    inline 
    float sdsdot (float alpha, vector_x_t const& x, vector_y_t const& y) {
      typedef traits::vector_traits<vector_x_t const> xvtraits;
      typedef traits::vector_traits<vector_y_t const> yvtraits;
      assert (yvtraits::size (y) >= xvtraits::size (x));
      return cblas_sdsdot (xvtraits::size (x), alpha, 
			   xvtraits::storage (x), xvtraits::stride (x), 
			   yvtraits::storage (y), yvtraits::stride (y)); 
    }

    // dotu <- x^T * y 
    // .. complex types only
    // .. function
    template <typename vector_x_t, typename vector_y_t>
    inline 
    typename traits::vector_traits<vector_x_t>::value_type 
    dotu (vector_x_t const& x, vector_y_t const& y) {
      typedef traits::vector_traits<vector_x_t const> xvtraits;
      typedef traits::vector_traits<vector_y_t const> yvtraits;
      assert (yvtraits::size (y) >= xvtraits::size (x));
      typename xvtraits::value_type val;
      detail::dotu (xvtraits::size (x),
		    xvtraits::storage (x), xvtraits::stride (x), 
		    yvtraits::storage (y), yvtraits::stride (y),
		    &val);
      return val; 
    }
    // .. procedure 
    template <typename vector_x_t, typename vector_y_t>
    inline 
    void dotu (vector_x_t const& x, vector_y_t const& y, 
	       typename traits::vector_traits<vector_x_t>::value_type& val) {
      typedef traits::vector_traits<vector_x_t const> xvtraits;
      typedef traits::vector_traits<vector_y_t const> yvtraits;
      assert (yvtraits::size (y) >= xvtraits::size (x));
      detail::dotu (xvtraits::size (x),
		    xvtraits::storage (x), xvtraits::stride (x), 
		    yvtraits::storage (y), yvtraits::stride (y),
		    &val);
    }

    // dotc <- x^H * y 
    // .. complex types only
    // .. function
    template <typename vector_x_t, typename vector_y_t>
    inline 
    typename traits::vector_traits<vector_x_t>::value_type 
    dotc (vector_x_t const& x, vector_y_t const& y) {
      typedef traits::vector_traits<vector_x_t const> xvtraits;
      typedef traits::vector_traits<vector_y_t const> yvtraits;
      assert (yvtraits::size (y) >= xvtraits::size (x));
      typename xvtraits::value_type val;
      detail::dotc (xvtraits::size (x),
		    xvtraits::storage (x), xvtraits::stride (x), 
		    yvtraits::storage (y), yvtraits::stride (y),
		    &val);
      return val; 
    }
    // .. procedure 
    template <typename vector_x_t, typename vector_y_t>
    inline 
    void dotc (vector_x_t const& x, vector_y_t const& y, 
	       typename traits::vector_traits<vector_x_t>::value_type& val) {
      typedef traits::vector_traits<vector_x_t const> xvtraits;
      typedef traits::vector_traits<vector_y_t const> yvtraits;
      assert (yvtraits::size (y) >= xvtraits::size (x));
      detail::dotc (xvtraits::size (x),
		    xvtraits::storage (x), xvtraits::stride (x), 
		    yvtraits::storage (y), yvtraits::stride (y),
		    &val);
    }

    // nrm2 <- ||x||_2
    template <typename vector_t> 
    inline 
    typename detail::type_traits<
      typename traits::vector_traits<vector_t const>::value_type 
    >::real_type 
    nrm2 (vector_t const& x) {
      typedef traits::vector_traits<vector_t const> vtraits;
      return detail::nrm2 (vtraits::size (x),
			   vtraits::storage (x), vtraits::stride (x)); 
    }

    // asum <- ||re (x)|| + ||im (x)||
    template <typename vector_t> 
    inline 
    typename detail::type_traits<
      typename traits::vector_traits<vector_t const>::value_type 
    >::real_type 
    asum (vector_t const& x) {
      typedef traits::vector_traits<vector_t const> vtraits;
      return detail::asum (vtraits::size (x),
			   vtraits::storage (x), vtraits::stride (x)); 
    }

    // iamax <- 1st i: max (|re (x_i)| + |im (x_i)|)
    template <typename vector_t> 
    inline 
    CBLAS_INDEX iamax (vector_t const& x) {
      typedef traits::vector_traits<vector_t const> vtraits;
      return detail::iamax (vtraits::size (x),
			    vtraits::storage (x), vtraits::stride (x)); 
    }

    // plane rotations 

  } // namespace cblas

}}} 

#endif // BOOST_NUMERIC_CBLAS_LEVEL_1_HPP

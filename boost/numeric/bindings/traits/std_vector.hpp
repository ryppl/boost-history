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

#ifndef BOOST_NUMERIC_BINDINGS_TRAITS_STD_VECTOR_H
#define BOOST_NUMERIC_BINDINGS_TRAITS_STD_VECTOR_H

#include <boost/numeric/bindings/traits/vector_traits.hpp>

#ifndef BOOST_NUMERIC_BINDINGS_POOR_MANS_TRAITS 

#include <vector>

namespace boost { namespace numeric { namespace bindings { namespace traits {

  // std::vector<>
  template <typename T, typename Alloc>
  struct vector_traits<std::vector<T, Alloc> > 
  : default_vector_traits<std::vector<T, Alloc> >
  {
    // g++ 3.2 doesn't accept, como does:
    //   using typename 
    //   default_vector_traits<std::vector<T, Alloc> >::pointer; 
    // therefore: 
    //   typedef typename 
    //   default_vector_traits<std::vector<T, Alloc> >::pointer pointer; 
    // but `T*' is probably the best solution (certainly the simplest)
    static T* storage (std::vector<T, Alloc>& v) {
      return &v.front(); 
    }
  }; 

  template <typename T, typename Alloc>
  struct vector_traits<std::vector<T, Alloc> const> 
  : default_vector_traits<std::vector<T, Alloc> const>
  {
    static T const* storage (std::vector<T, Alloc> const& v) {
      return &v.front(); 
    }
  }; 

}}}}  

#endif // BOOST_NUMERIC_BINDINGS_POOR_MANS_TRAITS 

#endif // BOOST_NUMERIC_BINDINGS_TRAITS_STD_VECTOR_H


#ifndef UTILS_H
#define UTILS_H

#include <cstddef>
#include <iostream>
#include <boost/numeric/bindings/traits/traits.hpp>

template <typename V>
void init_v (V& v, double s = 1., double a = 0.) {
  std::size_t sz 
    = boost::numeric::bindings::traits::vector_traits<V>::size (v);
  for(std::size_t i = 0; i < sz; ++i ) 
    v[i] = s * (i + a); 
}

template <typename V>
void print_v (V const& v, char const* ch = "") {
  std::cout << ch << ": "; 
  std::size_t sz 
    = boost::numeric::bindings::traits::vector_traits<V const>::size (v);
  for (std::size_t i = 0; i < sz; ++i)
    std::cout << v[i] << " ";
  std::cout << std::endl; 
}

template <typename M>
void init_m (M& m, double s = 1., double a = 0.) {
  std::size_t sz1 
    = boost::numeric::bindings::traits::matrix_traits<M>::size1 (m);
  std::size_t sz2
    = boost::numeric::bindings::traits::matrix_traits<M>::size2 (m);
  for(std::size_t i = 0; i < sz1; ++i ) 
    for(std::size_t j = 0; j < sz2; ++j ) 
      m (i, j) = s * (10 * (i + a) + (j + a)); 
}

template <typename M>
void print_m (M const& m, char const* ch = "") {
  std::cout << ch << ":\n"; 
  std::size_t sz1 
    = boost::numeric::bindings::traits::matrix_traits<M const>::size1 (m);
  std::size_t sz2
    = boost::numeric::bindings::traits::matrix_traits<M const>::size2 (m);
  for(std::size_t i = 0 ; i < sz1 ; ++i ) {
    for(std::size_t j = 0 ; j < sz2 ; ++j ) 
      std::cout << m (i, j) << " ";
    std::cout << std::endl; 
  }
  std::cout << std::endl; 
}

#endif

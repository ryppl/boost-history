// (-*- C++ -*- header)  

#ifndef BOOST_BINDINGS_TRAITS_BOOST_ARRAY_H
#define BOOST_BINDINGS_TRAITS_BOOST_ARRAY_H

#include <boost/numeric/bindings/traits/traits.hpp>
#include <boost/array.hpp>

// we don't need this at all -- generic version do the right thing

// namespace boost { namespace numeric { namespace bindings {

//   // boost::array<>
//   template <typename T, std::size_t N> 
//   struct vector_traits<boost::array<T, N> >
//   : default_vector_traits<boost::array<T, N> > 
//   {
//     static T* storage (boost::array<T, N>& ba) { return &ba.front(); }
//   };

//   template <typename T, std::size_t N> 
//   struct vector_traits<boost::array<T, N> const>
//   : default_vector_traits<boost::array<T, N> const> 
//   {
//     static T const* storage (boost::array<T, N> const& ba) {
//       return &ba.front(); 
//     }
//   };

// }}}

#endif 

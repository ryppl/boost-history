/* Copyright Joaquín M López Muñoz 2003-2004. Use, modification, and distribution
 * are subject to the Boost Software License, Version 1.0. (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * See http://www.boost.org/libs/indexed_set for library home page.
 */

#ifndef BOOST_INDEXED_SET_AUTO_SPACE_HPP
#define BOOST_INDEXED_SET_AUTO_SPACE_HPP

#include <boost/indexed_set/allocator.hpp>
#include <boost/noncopyable.hpp>
#include <memory>

namespace boost{

namespace indexed_sets{

namespace detail{

/* auto_space provides uninitialized space suitably to store
 * a given number of elements of a given type.
 */

template<typename T,typename Allocator=std::allocator<T> >
struct auto_space:private noncopyable
{
  explicit auto_space(const Allocator& al=Allocator(),std::size_t n=1):
    al_(al),n_(n),data_(allocator::allocate<T>(al_,n_))
  {}

  ~auto_space()
  {
    allocator::deallocate(al_,data_,n_);
  }

  T* data()const{return data_;}
    
private:
  typename allocator::rebind<
    Allocator,T>::type        al_;
  std::size_t                 n_;
  T*                          data_;
};

} /* namespace indexed_sets::detail */

} /* namespace indexed_sets */

} /* namespace boost */

#endif

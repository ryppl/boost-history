/* Copyright Joaqu�n M L�pez Mu�oz 2003. Use, modification, and distribution
 * are subject to the Boost Software License, Version 1.0. (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * See http://www.boost.org/libs/indexed_set for library home page.
 */

#ifndef BOOST_INDEXED_SET_HEADER_HOLDER_HPP
#define BOOST_INDEXED_SET_HEADER_HOLDER_HPP

#include <boost/noncopyable.hpp>
#include <boost/utility/base_from_member.hpp>

namespace boost{

namespace indexed_sets{

namespace detail{

/* An utility class derived from base_from_member used to hold
 * a pointer to the header node. The base from member idiom is used
 * because index classes, which are superclasses of indexed_set,
 * need this header in construction time.
 * The allocation is made by the allocator of the indexed_set
 * class --hence, this allocator needs also be stored resorting
 * to the base from member trick.
 */

template<typename NodeType,typename Final>
struct header_holder:base_from_member<NodeType*>,private noncopyable
{
  header_holder():super(final().allocate_node()){}
  ~header_holder(){final().deallocate_node(super::member);}

private:
  typedef base_from_member<NodeType*> super;
  Final& final(){return *static_cast<Final*>(this);}
};



} /* namespace indexed_sets::detail */

} /* namespace indexed_sets */

} /* namespace boost */

#endif

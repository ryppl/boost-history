/* Copyright Joaqu�n M L�pez Mu�oz 2003. Use, modification, and distribution
 * are subject to the Boost Software License, Version 1.0. (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * See http://www.boost.org/libs/indexed_set for library home page.
 */

#ifndef BOOST_INDEXED_SET_INDEX_BASE_HPP
#define BOOST_INDEXED_SET_INDEX_BASE_HPP

#include <boost/config.hpp> /* keep it first to prevent some nasty warnings in MSVC */
#include <boost/call_traits.hpp>
#include <boost/detail/workaround.hpp>
#include <boost/indexed_set/node_type.hpp>
#include <boost/indexed_set_fwd.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/tuple/tuple.hpp>
#include <utility>

namespace boost{

namespace indexed_sets{

namespace detail{

/* The role of this class is threefold:
 *   - tops the linear hierarchy of indices.
 *   - terminates some cascading backbone function calls (insert_, etc.),
 *   - grants access to the backbone functions of the final indexed_set class
 *     (for access restriction reasons, these cannot be called directly
 *     from the index classes.)
 */

template<typename Value,typename IndexSpecifierList,typename Allocator>
class index_base
{
protected:
  typedef index_node_base<Value>                          node_type;
  typedef typename indexed_set_node_type<
    Value,IndexSpecifierList,Allocator>::type             final_node_type;
  typedef indexed_set<Value,IndexSpecifierList,Allocator> final_type;
  typedef tuples::null_type                               ctor_args_list;
  typedef mpl::vector0<>                                  index_type_list;
  typedef mpl::vector0<>                                  iterator_type_list;
  typedef mpl::vector0<>                                  const_iterator_type_list;

private:
  typedef typename call_traits<Value>::param_type value_param_type;

protected:
  explicit index_base(const ctor_args_list&,const Allocator&){}

  std::pair<node_type*,bool> insert_(value_param_type v,node_type* x)
  {
    detail::allocator::construct(&x->value,v);
    return std::pair<node_type*,bool>(x,true);
  }

  std::pair<node_type*,bool> insert_(value_param_type v,node_type* position,node_type* x)
  {
    detail::allocator::construct(&x->value,v);
    return std::pair<node_type*,bool>(x,true);
  }

  void erase_(node_type* x)
  {
    detail::allocator::destroy(&x->value);
  }

  void swap_(index_base<Value,IndexSpecifierList,Allocator>&){}

  bool update_(value_param_type v,node_type* x)
  {
    x->value=v;
    return true;
  }

#if !defined(BOOST_NO_MEMBER_TEMPLATES)||defined(BOOST_MSVC6_MEMBER_TEMPLATES)
  bool modify_(node_type* x){return true;}
#endif

#if defined(BOOST_INDEXED_SET_ENABLE_INVARIANT_CHECKING)
  /* invariant stuff */

  bool invariant_()const{return true;}
#endif

  /* access to backbone memfuns of Final class */

  final_type&       final(){return *static_cast<final_type*>(this);}
  const final_type& final()const{return *static_cast<const final_type*>(this);}

  final_node_type* final_header()const{return final().header();}

  bool        final_empty_()const{return final().empty_();}
  std::size_t final_size_()const{return final().size_();}
  std::size_t final_max_size_()const{final.max_size_();}

  std::pair<final_node_type*,bool> final_insert_(value_param_type x)
    {return final().insert_(x);}
  std::pair<final_node_type*,bool> final_insert_(value_param_type x,final_node_type* position)
    {return final().insert_(x,position);}

  void final_erase_(final_node_type* x){final().erase_(x);}
  void final_swap_(final_type& x){final().swap_(x);}
  bool final_update_(value_param_type k,final_node_type* x){return final().update_(k,x);}

#if !defined(BOOST_NO_MEMBER_TEMPLATES)||defined(BOOST_MSVC6_MEMBER_TEMPLATES)
  template<typename Modifier>
  bool final_modify_(Modifier mod,final_node_type* x){return final().modify_(mod,x);}
#endif

#if defined(BOOST_INDEXED_SET_ENABLE_INVARIANT_CHECKING)
  void final_check_invariant_()const{final().check_invariant_();}
#endif
};

} /* namespace indexed_sets::detail */

} /* namespace indexed_sets */

} /* namespace boost */

#endif

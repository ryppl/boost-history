/* Multiply indexed container.
 *
 * Copyright Joaquín M López Muñoz 2003-2004. Use, modification, and distribution
 * are subject to the Boost Software License, Version 1.0. (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * See http://www.boost.org/libs/indexed_set for library home page.
 */

#ifndef BOOST_INDEXED_SET_HPP
#define BOOST_INDEXED_SET_HPP

#include <boost/config.hpp> /* keep it first to prevent some nasty warnings in MSVC */
#include <algorithm>
#include <boost/detail/workaround.hpp>
#include <boost/mpl/at.hpp>
#include <boost/mpl/contains.hpp>
#include <boost/mpl/find_if.hpp>
#include <boost/mpl/int.hpp>
#include <boost/mpl/size.hpp>
#include <boost/indexed_set_fwd.hpp>
#include <boost/indexed_set/access_specifier.hpp>
#include <boost/indexed_set/allocator.hpp>
#include <boost/indexed_set/converter.hpp>
#include <boost/indexed_set/def_ctor_tuple_cons.hpp>
#include <boost/indexed_set/header_holder.hpp>
#include <boost/indexed_set/has_tag.hpp>
#include <boost/indexed_set/index.hpp>
#include <boost/indexed_set/member.hpp>
#include <boost/indexed_set/mem_fun.hpp>
#include <boost/indexed_set/base_type.hpp>
#include <boost/indexed_set/prevent_eti.hpp>
#include <boost/indexed_set/scope_guard.hpp>
#include <boost/indexed_set/try_catch.hpp>
#include <boost/static_assert.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/utility/base_from_member.hpp>

#if defined(BOOST_INDEXED_SET_ENABLE_INVARIANT_CHECKING)
#include <boost/indexed_set/invariant_assert.hpp>
#define BOOST_INDEXED_SET_CHECK_INVARIANT \
  detail::scope_guard BOOST_JOIN(check_invariant_,__LINE__)=\
    detail::make_obj_guard(*this,&indexed_set::check_invariant_);\
  BOOST_JOIN(check_invariant_,__LINE__).touch();
#else
#define BOOST_INDEXED_SET_CHECK_INVARIANT
#endif

namespace boost{

namespace indexed_sets{

template<typename Value,typename IndexSpecifierList,typename Allocator>
class indexed_set:
  private base_from_member<
    typename detail::allocator::rebind<
      Allocator,
      typename detail::indexed_set_node_type<Value,IndexSpecifierList,Allocator>::type
    >::type>,
  BOOST_INDEXED_SET_PRIVATE_IF_MEMBER_TEMPLATE_FRIENDS detail::header_holder<
    typename detail::indexed_set_node_type<Value,IndexSpecifierList,Allocator>::type,
    indexed_set<Value,IndexSpecifierList,Allocator> >,
  public detail::indexed_set_base_type<Value,IndexSpecifierList,Allocator>::type
{
private:
#if !defined(BOOST_NO_MEMBER_TEMPLATE_FRIENDS)
  template <typename,typename,typename,typename> friend class  detail::index_base;
  template <typename,typename>                   friend class  detail::header_holder;
  template <typename,typename>                   friend class  detail::converter;
#endif

  typedef typename detail::indexed_set_base_type<
      Value,IndexSpecifierList,Allocator>::type   super;
  typedef base_from_member<
    typename detail::allocator::rebind<
      Allocator,
      typename super::node_type
    >::type>                                      bfm_allocator;
  typedef detail::header_holder<
    typename super::node_type,
    indexed_set>                                  bfm_header;

public:
  /* All types are inherited from super, a few are explicitly
   * brought forward here to save us some typename's.
   */

#if defined(BOOST_MSVC)
  typedef 
    detail::default_constructible_tuple_cons<
      typename super::ctor_args_list>              ctor_args_list;
#else
  typedef typename super::ctor_args_list           ctor_args_list;
#endif

  typedef typename IndexSpecifierList::type        index_specifier_type_list;
  typedef typename super::index_type_list          index_type_list;
  typedef typename super::iterator_type_list       iterator_type_list;
  typedef typename super::const_iterator_type_list const_iterator_type_list;
  typedef typename super::value_type               value_type;
  typedef typename super::final_allocator_type     allocator_type;
  typedef typename super::iterator                 iterator;
  typedef typename super::const_iterator           const_iterator;

  /* global project() needs to see this publicly */

  typedef typename super::node_type     node_type;

  /* construct/copy/destroy */

  explicit indexed_set(
    const ctor_args_list& args_list=ctor_args_list(),
    const allocator_type& al=allocator_type()):
    bfm_allocator(al),
    super(args_list,bfm_allocator::member),
    node_count(0)
  {
    BOOST_INDEXED_SET_CHECK_INVARIANT;
  }    

#if !defined(BOOST_NO_MEMBER_TEMPLATES)||defined(BOOST_MSVC6_MEMBER_TEMPLATES)
  template<typename InputIterator>
  indexed_set(
    InputIterator first,InputIterator last,
    const ctor_args_list& args_list=ctor_args_list(),
    const allocator_type& al=allocator_type()):
    bfm_allocator(al),
    super(args_list,bfm_allocator::member),
    node_count(0)
  {
    BOOST_INDEXED_SET_CHECK_INVARIANT;
    BOOST_INDEXED_SET_TRY{
      iterator hint=super::end();
      for(;first!=last;++first){
        hint=super::make_iterator(insert_(*first,hint.get_node()).first);
      }
    }
    BOOST_INDEXED_SET_CATCH(...){
      clean_up();
      BOOST_INDEXED_SET_RETHROW;
    }
  }
#endif

  indexed_set(const indexed_set<Value,IndexSpecifierList,Allocator>& x):
    bfm_allocator(x.bfm_allocator::member),
    super(x),
    node_count(0)
  {
    BOOST_INDEXED_SET_CHECK_INVARIANT;
    copy_map_type map(bfm_allocator::member,x.size(),x.header(),header());
    for(const_iterator it=x.begin();it!=x.end();++it)map.clone(it.get_node());
    super::copy_(x,map);
    map.release();
    node_count=x.size();
  }

  ~indexed_set()
  {
    BOOST_INDEXED_SET_CHECK_INVARIANT;
    clean_up();
  }

  indexed_set<Value,IndexSpecifierList,Allocator>& operator=(
    const indexed_set<Value,IndexSpecifierList,Allocator>& x)
  {
    BOOST_INDEXED_SET_CHECK_INVARIANT;
    indexed_set<Value,IndexSpecifierList,Allocator> tmp(x);
    swap(tmp);
    return *this;
  }

  allocator_type get_allocator()const
  {
    return allocator_type(bfm_allocator::member);
  }

  /* retrieval of indices by number */

#if !defined(BOOST_NO_MEMBER_TEMPLATES)
  template<int N>
  struct nth_index_type
  {
    BOOST_STATIC_ASSERT(N>=0&&N<mpl::size<index_type_list>::type::value);
    typedef typename mpl::at_c<index_type_list,N>::type type;
  };

  template<int N>
  typename nth_index_type<N>::type& get(BOOST_EXPLICIT_TEMPLATE_NON_TYPE(int,N))
  {
    BOOST_STATIC_ASSERT(N>=0&&N<mpl::size<index_type_list>::type::value);
    return *this;
  }

  template<int N>
  const typename nth_index_type<N>::type& get(BOOST_EXPLICIT_TEMPLATE_NON_TYPE(int,N))const
  {
    BOOST_STATIC_ASSERT(N>=0&&N<mpl::size<index_type_list>::type::value);
    return *this;
  }
#endif

  /* retrieval of indices by tag */

#if !defined(BOOST_NO_MEMBER_TEMPLATES)
  template<typename Tag>
  struct index_type
  {
    typedef typename mpl::find_if<
      index_type_list,
      detail::has_tag<Tag>
    >::type                                    iter;

    BOOST_STATIC_CONSTANT(bool,index_found=!(is_same<iter,mpl::end<index_type_list> >::value));
    BOOST_STATIC_ASSERT(index_found);

    typedef typename iter::type                type;
  };

  template<typename Tag>
  typename index_type<Tag>::type& get(BOOST_EXPLICIT_TEMPLATE_TYPE(Tag))
  {
    return *this;
  }

  template<typename Tag>
  const typename index_type<Tag>::type& get(BOOST_EXPLICIT_TEMPLATE_TYPE(Tag))const
  {
    return *this;
  }
#endif

  /* projection of iterators by number */

#if !defined(BOOST_NO_MEMBER_TEMPLATES)
  template<int N>
  struct nth_iterator_type
  {
    typedef typename nth_index_type<N>::type::iterator type;
  };

  template<int N>
  struct nth_const_iterator_type
  {
    typedef typename nth_index_type<N>::type::const_iterator type;
  };

  template<int N,typename IteratorType>
  typename nth_iterator_type<N>::type project(
    IteratorType it
    BOOST_APPEND_EXPLICIT_TEMPLATE_NON_TYPE(int,N))
  {
    typedef typename nth_index_type<N>::type index_type;

    BOOST_STATIC_ASSERT((mpl::contains<iterator_type_list,IteratorType>::value));

    BOOST_INDEXED_SET_CHECK_VALID_ITERATOR(it);
    BOOST_INDEXED_SET_CHECK_IS_OWNER(
      it,static_cast<typename IteratorType::container_type&>(*this));

    return index_type::make_iterator(static_cast<node_type*>(it.get_node()));
  }

  template<int N,typename IteratorType>
  typename nth_const_iterator_type<N>::type project(
    IteratorType it
    BOOST_APPEND_EXPLICIT_TEMPLATE_NON_TYPE(int,N))const
  {
    typedef typename nth_index_type<N>::type index_type;

    BOOST_STATIC_ASSERT((
      mpl::contains<iterator_type_list,IteratorType>::value||
      mpl::contains<const_iterator_type_list,IteratorType>::value));

    BOOST_INDEXED_SET_CHECK_VALID_ITERATOR(it);
    BOOST_INDEXED_SET_CHECK_IS_OWNER(
      it,static_cast<const typename IteratorType::container_type&>(*this));
    return index_type::make_iterator(static_cast<node_type*>(it.get_node()));
  }
#endif

  /* projection of iterators by tag */

#if !defined(BOOST_NO_MEMBER_TEMPLATES)
  template<typename Tag>
  struct iterator_type
  {
    typedef typename index_type<Tag>::type::iterator type;
  };

  template<typename Tag>
  struct const_iterator_type
  {
    typedef typename index_type<Tag>::type::const_iterator type;
  };

  template<typename Tag,typename IteratorType>
  typename iterator_type<Tag>::type project(
    IteratorType it
    BOOST_APPEND_EXPLICIT_TEMPLATE_TYPE(Tag))
  {
    typedef typename index_type<Tag>::type index_type;

    BOOST_STATIC_ASSERT((mpl::contains<iterator_type_list,IteratorType>::value));

    BOOST_INDEXED_SET_CHECK_VALID_ITERATOR(it);
    BOOST_INDEXED_SET_CHECK_IS_OWNER(
      it,static_cast<typename IteratorType::container_type&>(*this));
    return index_type::make_iterator(static_cast<node_type*>(it.get_node()));
  }

  template<typename Tag,typename IteratorType>
  typename const_iterator_type<Tag>::type project(
    IteratorType it
    BOOST_APPEND_EXPLICIT_TEMPLATE_TYPE(Tag))const
  {
    typedef typename index_type<Tag>::type index_type;

    BOOST_STATIC_ASSERT((
      mpl::contains<iterator_type_list,IteratorType>::value||
      mpl::contains<const_iterator_type_list,IteratorType>::value));

    BOOST_INDEXED_SET_CHECK_VALID_ITERATOR(it);
    BOOST_INDEXED_SET_CHECK_IS_OWNER(
      it,static_cast<const typename IteratorType::container_type&>(*this));
    return index_type::make_iterator(static_cast<node_type*>(it.get_node()));
  }
#endif

BOOST_INDEXED_SET_PROTECTED_IF_MEMBER_TEMPLATE_FRIENDS:
  typedef typename super::copy_map_type copy_map_type;

  node_type* header()const
  {
    return bfm_header::member;
  }

  node_type* allocate_node()
  {
    return detail::allocator::allocate<node_type>(bfm_allocator::member,1);
  }

  void deallocate_node(node_type* x)
  {
    detail::allocator::deallocate(bfm_allocator::member,x,1);
  }

  bool empty_()const
  {
    return node_count==0;
  }

  std::size_t size_()const
  {
    return node_count;
  }

  std::size_t max_size_()const
  {
    return static_cast<std::size_t >(-1);
  }

  std::pair<node_type*,bool> insert_(const Value& v)
  {
    node_type* x=allocate_node();
    BOOST_INDEXED_SET_TRY{
      node_type* res=super::insert_(v,x);
      if(res==x){
        ++node_count;
        return std::pair<node_type*,bool>(res,true);
      }
      else{
        deallocate_node(x);
        return std::pair<node_type*,bool>(res,false);
      }
    }
    BOOST_INDEXED_SET_CATCH(...){
      deallocate_node(x);
      BOOST_INDEXED_SET_RETHROW;
    }
  }

  std::pair<node_type*,bool> insert_(const Value& v,node_type* position)
  {
    node_type* x=allocate_node();
    BOOST_INDEXED_SET_TRY{
      node_type* res=super::insert_(v,position,x);
      if(res==x){
        ++node_count;
        return std::pair<node_type*,bool>(res,true);
      }
      else{
        deallocate_node(x);
        return std::pair<node_type*,bool>(res,false);
      }
    }
    BOOST_INDEXED_SET_CATCH(...){
      deallocate_node(x);
      BOOST_INDEXED_SET_RETHROW;
    }
  }

  void erase_(node_type* x)
  {
    super::erase_(x);
    deallocate_node(x);
    --node_count;
  }

  void swap_(indexed_set<Value,IndexSpecifierList,Allocator>& x)
  {
    std::swap(bfm_header::member,x.bfm_header::member);
    super::swap_(x);
    std::swap(node_count,x.node_count);
  }

  bool update_(const Value& k,node_type* x)
  {
    return super::update_(k,x);
  }

#if !defined(BOOST_NO_MEMBER_TEMPLATES)||defined(BOOST_MSVC6_MEMBER_TEMPLATES)
  template<typename Modifier>
  bool modify_(Modifier mod,node_type* x)
  {
    mod(const_cast<value_type&>(x->value));

    BOOST_INDEXED_SET_TRY{
      if(!super::modify_(x)){
        deallocate_node(x);
        --node_count;
        return false;
      }
      else return true;
    }
    BOOST_INDEXED_SET_CATCH(...){
      deallocate_node(x);
      --node_count;
      BOOST_INDEXED_SET_RETHROW;
    }
  }
#endif

#if defined(BOOST_INDEXED_SET_ENABLE_INVARIANT_CHECKING)
  /* invariant stuff */

  bool invariant_()const
  {
    return super::invariant_();
  }

  void check_invariant_()const
  {
    BOOST_INDEXED_SET_INVARIANT_ASSERT(invariant_());
  }
#endif

private:
  void clean_up()
  {
    for(iterator it=super::begin();it!=super::end();)erase_(it++.get_node());
  }

  std::size_t node_count;
};

/*  specialized algorithms */

template<typename Value,typename IndexSpecifierList,typename Allocator>
void swap(
  indexed_set<Value,IndexSpecifierList,Allocator>& x,
  indexed_set<Value,IndexSpecifierList,Allocator>& y)
{
  x.swap(y);
}

/* retrieval of indices by number */

template<typename IndexedSet,int N>
struct nth_index_type
{
  BOOST_STATIC_CONSTANT(
    int,M=mpl::size<typename IndexedSet::index_type_list>::type::value);
  BOOST_STATIC_ASSERT(N>=0&&N<M);
  typedef typename mpl::at_c<
    typename IndexedSet::index_type_list,N>::type type;
};

template<int N,typename Value,typename IndexSpecifierList,typename Allocator>
typename nth_index_type<indexed_set<Value,IndexSpecifierList,Allocator>,N>::type&
get(
  indexed_set<Value,IndexSpecifierList,Allocator>& m
  BOOST_APPEND_EXPLICIT_TEMPLATE_NON_TYPE(int,N))
{
  typedef indexed_set<
    Value,IndexSpecifierList,Allocator>    indexed_set_type;
  typedef typename nth_index_type<
    indexed_set<
      Value,IndexSpecifierList,Allocator>,
    N
  >::type                                  index_type;

  BOOST_STATIC_ASSERT(N>=0&&
    N<mpl::size<BOOST_DEDUCED_TYPENAME indexed_set_type::index_type_list>::type::value);

  return detail::converter<indexed_set_type,index_type>::index(m);
}

template<int N,typename Value,typename IndexSpecifierList,typename Allocator>
const typename nth_index_type<indexed_set<Value,IndexSpecifierList,Allocator>,N>::type&
get(
  const indexed_set<Value,IndexSpecifierList,Allocator>& m
  BOOST_APPEND_EXPLICIT_TEMPLATE_NON_TYPE(int,N))
{
  typedef indexed_set<
    Value,IndexSpecifierList,Allocator>    indexed_set_type;
  typedef typename nth_index_type<
    indexed_set<
      Value,IndexSpecifierList,Allocator>,
    N
  >::type                                  index_type;

  BOOST_STATIC_ASSERT(N>=0&&
    N<mpl::size<BOOST_DEDUCED_TYPENAME indexed_set_type::index_type_list>::type::value);

  return detail::converter<indexed_set_type,index_type>::index(m);
}

/* retrieval of indices by tag */

template<typename IndexedSet,typename Tag>
struct index_type
{
  typedef typename IndexedSet::index_type_list index_type_list;

  typedef typename mpl::find_if<
    index_type_list,
    detail::has_tag<Tag>
  >::type                                      iter;

  BOOST_STATIC_CONSTANT(bool,index_found=!(is_same<iter,mpl::end<index_type_list> >::value));
  BOOST_STATIC_ASSERT(index_found);

  typedef typename iter::type                  type;
};

template<typename Tag,typename Value,typename IndexSpecifierList,typename Allocator>
typename index_type<indexed_set<Value,IndexSpecifierList,Allocator>,Tag>::type&
get(
  indexed_set<Value,IndexSpecifierList,Allocator>& m
  BOOST_APPEND_EXPLICIT_TEMPLATE_TYPE(Tag))
{
  typedef indexed_set<
    Value,IndexSpecifierList,Allocator>    indexed_set_type;
  typedef typename index_type<
    indexed_set<
      Value,IndexSpecifierList,Allocator>,
    Tag
  >::type                                  index_type;

  return detail::converter<indexed_set_type,index_type>::index(m);
}

template<typename Tag,typename Value,typename IndexSpecifierList,typename Allocator>
const typename index_type<indexed_set<Value,IndexSpecifierList,Allocator>,Tag>::type&
get(
  const indexed_set<Value,IndexSpecifierList,Allocator>& m
  BOOST_APPEND_EXPLICIT_TEMPLATE_TYPE(Tag))
{
  typedef indexed_set<
    Value,IndexSpecifierList,Allocator>    indexed_set_type;
  typedef typename index_type<
    indexed_set<
      Value,IndexSpecifierList,Allocator>,
    Tag
  >::type                                  index_type;

  return detail::converter<indexed_set_type,index_type>::index(m);
}

/* projection of iterators by number */

template<typename IndexedSet,int N>
struct nth_iterator_type
{
  typedef typename detail::prevent_eti<
    nth_index_type<IndexedSet,N>,
    typename nth_index_type<IndexedSet,N>::type>::type::iterator type;
};

template<typename IndexedSet,int N>
struct nth_const_iterator_type
{
  typedef typename detail::prevent_eti<
    nth_index_type<IndexedSet,N>,
    typename nth_index_type<IndexedSet,N>::type>::type::const_iterator type;
};

template<
  int N,typename IteratorType,
  typename Value,typename IndexSpecifierList,typename Allocator>
typename nth_iterator_type<indexed_set<Value,IndexSpecifierList,Allocator>,N>::type
project(
  indexed_set<Value,IndexSpecifierList,Allocator>& m,
  IteratorType it
  BOOST_APPEND_EXPLICIT_TEMPLATE_NON_TYPE(int,N))
{
  typedef indexed_set<Value,IndexSpecifierList,Allocator>   indexed_set_type;
  typedef typename nth_index_type<indexed_set_type,N>::type index_type;

#if !defined(BOOST_MSVC)||!(BOOST_MSVC<1300) /* this ain't work in MSVC++ 6.0 */
  BOOST_STATIC_ASSERT((
    mpl::contains<
      BOOST_DEDUCED_TYPENAME indexed_set_type::iterator_type_list,
      IteratorType>::value));
#endif

  BOOST_INDEXED_SET_CHECK_VALID_ITERATOR(it);

#if defined(BOOST_INDEXED_SET_ENABLE_SAFE_MODE)
  typedef detail::converter<
    indexed_set_type,
    BOOST_DEDUCED_TYPENAME IteratorType::container_type> converter;
  BOOST_INDEXED_SET_CHECK_IS_OWNER(it,converter::index(m));
#endif

  return detail::converter<indexed_set_type,index_type>::iterator(
    m,static_cast<typename indexed_set_type::node_type*>(it.get_node()));
}

template<
  int N,typename IteratorType,
  typename Value,typename IndexSpecifierList,typename Allocator>
typename nth_const_iterator_type<indexed_set<Value,IndexSpecifierList,Allocator>,N>::type
project(
  const indexed_set<Value,IndexSpecifierList,Allocator>& m,
  IteratorType it
  BOOST_APPEND_EXPLICIT_TEMPLATE_NON_TYPE(int,N))
{
  typedef indexed_set<Value,IndexSpecifierList,Allocator>   indexed_set_type;
  typedef typename nth_index_type<indexed_set_type,N>::type index_type;

#if !defined(BOOST_MSVC)||!(BOOST_MSVC<1300) /* this ain't work in MSVC++ 6.0 */
  BOOST_STATIC_ASSERT((
    mpl::contains<
      BOOST_DEDUCED_TYPENAME indexed_set_type::iterator_type_list,
      IteratorType>::value||
    mpl::contains<
      BOOST_DEDUCED_TYPENAME indexed_set_type::const_iterator_type_list,
      IteratorType>::value));
#endif

  BOOST_INDEXED_SET_CHECK_VALID_ITERATOR(it);

#if defined(BOOST_INDEXED_SET_ENABLE_SAFE_MODE)
  typedef detail::converter<
    indexed_set_type,
    BOOST_DEDUCED_TYPENAME IteratorType::container_type> converter;
  BOOST_INDEXED_SET_CHECK_IS_OWNER(it,converter::index(m));
#endif

  return detail::converter<indexed_set_type,index_type>::const_iterator(
    m,static_cast<typename indexed_set_type::node_type*>(it.get_node()));
}

/* projection of iterators by tag */

template<typename IndexedSet,typename Tag>
struct iterator_type
{
  typedef typename index_type<IndexedSet,Tag>::type::iterator type;
};

template<typename IndexedSet,typename Tag>
struct const_iterator_type
{
  typedef typename index_type<IndexedSet,Tag>::type::const_iterator type;
};

template<
  typename Tag,typename IteratorType,
  typename Value,typename IndexSpecifierList,typename Allocator>
typename iterator_type<indexed_set<Value,IndexSpecifierList,Allocator>,Tag>::type
project(
  indexed_set<Value,IndexSpecifierList,Allocator>& m,
  IteratorType it
  BOOST_APPEND_EXPLICIT_TEMPLATE_TYPE(Tag))
{
  typedef indexed_set<Value,IndexSpecifierList,Allocator> indexed_set_type;
  typedef typename index_type<indexed_set_type,Tag>::type index_type;

#if !defined(BOOST_MSVC)||!(BOOST_MSVC<1300) /* this ain't work in MSVC++ 6.0 */
  BOOST_STATIC_ASSERT((
    mpl::contains<
      BOOST_DEDUCED_TYPENAME indexed_set_type::iterator_type_list,
      IteratorType>::value));
#endif

  BOOST_INDEXED_SET_CHECK_VALID_ITERATOR(it);

#if defined(BOOST_INDEXED_SET_ENABLE_SAFE_MODE)
  typedef detail::converter<
    indexed_set_type,
    BOOST_DEDUCED_TYPENAME IteratorType::container_type> converter;
  BOOST_INDEXED_SET_CHECK_IS_OWNER(it,converter::index(m));
#endif

  return detail::converter<indexed_set_type,index_type>::iterator(
    m,static_cast<typename indexed_set_type::node_type*>(it.get_node()));
}

template<
  typename Tag,typename IteratorType,
  typename Value,typename IndexSpecifierList,typename Allocator>
typename const_iterator_type<indexed_set<Value,IndexSpecifierList,Allocator>,Tag>::type
project(
  const indexed_set<Value,IndexSpecifierList,Allocator>& m,
  IteratorType it
  BOOST_APPEND_EXPLICIT_TEMPLATE_TYPE(Tag))
{
  typedef indexed_set<Value,IndexSpecifierList,Allocator> indexed_set_type;
  typedef typename index_type<indexed_set_type,Tag>::type index_type;

#if !defined(BOOST_MSVC)||!(BOOST_MSVC<1300) /* this ain't work in MSVC++ 6.0 */
  BOOST_STATIC_ASSERT((
    mpl::contains<
      BOOST_DEDUCED_TYPENAME indexed_set_type::iterator_type_list,
      IteratorType>::value||
    mpl::contains<
      BOOST_DEDUCED_TYPENAME indexed_set_type::const_iterator_type_list,
      IteratorType>::value));
#endif

  BOOST_INDEXED_SET_CHECK_VALID_ITERATOR(it);

#if defined(BOOST_INDEXED_SET_ENABLE_SAFE_MODE)
  typedef detail::converter<
    indexed_set_type,
    BOOST_DEDUCED_TYPENAME IteratorType::container_type> converter;
  BOOST_INDEXED_SET_CHECK_IS_OWNER(it,converter::index(m));
#endif

  return detail::converter<indexed_set_type,index_type>::const_iterator(
    m,static_cast<typename indexed_set_type::node_type*>(it.get_node()));
}

/* Comparison. Simple forward to first index. */

template<typename Value,typename IndexSpecifierList,typename Allocator>
bool operator==(
  const indexed_set<Value,IndexSpecifierList,Allocator>& x,
  const indexed_set<Value,IndexSpecifierList,Allocator>& y)
{
  return get<0>(x)==get<0>(y);
}

template<typename Value,typename IndexSpecifierList,typename Allocator>
bool operator<(
  const indexed_set<Value,IndexSpecifierList,Allocator>& x,
  const indexed_set<Value,IndexSpecifierList,Allocator>& y)
{
  return get<0>(x)<get<0>(y);
}

template<typename Value,typename IndexSpecifierList,typename Allocator>
bool operator!=(
  const indexed_set<Value,IndexSpecifierList,Allocator>& x,
  const indexed_set<Value,IndexSpecifierList,Allocator>& y)
{
  return get<0>(x)!=get<0>(y);
}

template<typename Value,typename IndexSpecifierList,typename Allocator>
bool operator>(
  const indexed_set<Value,IndexSpecifierList,Allocator>& x,
  const indexed_set<Value,IndexSpecifierList,Allocator>& y)
{
  return get<0>(x)>get<0>(y);
}

template<typename Value,typename IndexSpecifierList,typename Allocator>
bool operator>=(
  const indexed_set<Value,IndexSpecifierList,Allocator>& x,
  const indexed_set<Value,IndexSpecifierList,Allocator>& y)
{
  return get<0>(x)>=get<0>(y);
}

template<typename Value,typename IndexSpecifierList,typename Allocator>
bool operator<=(
  const indexed_set<Value,IndexSpecifierList,Allocator>& x,
  const indexed_set<Value,IndexSpecifierList,Allocator>& y)
{
  return get<0>(x)<=get<0>(y);
}

} /* namespace indexed_sets */

/* Associated global functions are promoted to namespace boost, except
 * comparison operators and swap, which are meant to be Koenig looked-up.
 */

using indexed_sets::get;
using indexed_sets::project;

} /* namespace boost */

#undef BOOST_INDEXED_SET_CHECK_INVARIANT

#endif

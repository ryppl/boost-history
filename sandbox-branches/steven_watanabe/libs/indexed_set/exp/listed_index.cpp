#include <boost/indexed_set.hpp>
#include <algorithm>
#include <iostream>
#include <iterator>

namespace boost_indexed_set_ext{

namespace detail{

template<typename Super>
struct listed_index_node:Super
{
  listed_index_node* prior;
  listed_index_node* next;
};

template<typename Node>
class listed_index_iterator:
  public boost::bidirectional_iterator_helper<
    listed_index_iterator<Node>,
    const typename Node::value_type>
{
public:
  listed_index_iterator(){}
  listed_index_iterator(Node* node_):node(node_){}

  const typename Node::value_type& operator*()const
  {
    return node->value;
  }

  listed_index_iterator& operator++()
  {
    node=node->next;
    return *this;
  }

  listed_index_iterator& operator--()
  {
    node=node->prior;
    return *this;
  }

  friend bool operator==(const listed_index_iterator& x,const listed_index_iterator& y)
  {
    return x.node==y.node;
  }

  /* get_node is not to be used by the user */

  Node* get_node()const{return node;}

private:
  Node* node;
};

template<typename Allocator,typename Super,typename TagList>
class listed_index:BOOST_INDEXED_SET_PROTECTED_IF_MEMBER_TEMPLATE_FRIENDS Super
{ 
protected:
  typedef listed_index_node<typename Super::node_type> node_type;

public:
  /* types */

  typedef typename node_type::value_type             value_type;
  typedef boost::tuples::null_type                   ctor_args;
  typedef Allocator                                  allocator_type;
  typedef typename Allocator::reference              reference;
  typedef typename Allocator::const_reference        const_reference;
  typedef listed_index_iterator<node_type>           iterator;
  typedef listed_index_iterator<node_type>           const_iterator;
  typedef std::size_t                                size_type;      
  typedef std::ptrdiff_t                             difference_type;
  typedef typename Allocator::pointer                pointer;
  typedef typename Allocator::const_pointer          const_pointer;
  typedef typename
    boost::reverse_iterator_generator<
      iterator>::type                                reverse_iterator;
  typedef typename
    boost::reverse_iterator_generator<
      const_iterator>::type                          const_reverse_iterator;
  typedef typename TagList::type                     tag_list;

protected:
  typedef typename Super::final_node_type            final_node_type;
  typedef boost::tuples::cons<
    ctor_args, 
    typename Super::ctor_args_list>                  ctor_args_list;
  typedef typename boost::mpl::push_front<
    typename Super::index_type_list,
    listed_index>::type                              index_type_list;
  typedef typename boost::mpl::push_front<
    typename Super::iterator_type_list,
    iterator>::type                                  iterator_type_list;
  typedef typename boost::mpl::push_front<
    typename Super::const_iterator_type_list,
    const_iterator>::type                            const_iterator_type_list;

private:
  typedef typename boost::call_traits<
    value_type>::param_type                          value_param_type;

public:

  /* construct/copy/destroy
   * Default and copy ctors are in the protected section as indices are
   * not supposed to be created on their own. No range ctor either.
   */

  listed_index<Allocator,Super,TagList>& operator=(
    const listed_index<Allocator,Super,TagList>& x)
  {
    final()=x.final();
    return *this;
  }

  allocator_type get_allocator()const
  {
    return final().get_allocator();
  }

  /* iterators */

  iterator               begin(){return make_iterator(header()->next);}
  const_iterator         begin()const{return make_iterator(header()->next);}
  iterator               end(){return make_iterator(header());}
  const_iterator         end()const{return make_iterator(header());}
  reverse_iterator       rbegin(){return make_reverse_iterator(end());}
  const_reverse_iterator rbegin()const{return make_reverse_iterator(end());}
  reverse_iterator       rend(){return make_reverse_iterator(begin());}
  const_reverse_iterator rend()const{return make_reverse_iterator(begin());}

  /* capacity */

  bool      empty()const{return final_empty_();}
  size_type size()const{return final_size_();}
  size_type max_size()const{return final_max_size_();}

  /* access */

  reference       front(){return *begin();}
  const_reference front()const{return *begin();}
  reference       back(){return *--end();}
  const_reference back()const{return *--end();}

  /* modifiers */

  void push_front(value_param_type x){insert(begin(),x);}
  void pop_front(){erase(begin());}
  void push_back(value_param_type x){insert(end(),x);}
  void pop_back(){erase(end());}

  std::pair<iterator,bool> insert(iterator position,value_param_type x)
  {
    std::pair<final_node_type*,bool> p=final_insert_(x);
    if(p.second)relink(position.get_node(),p.first);
    return std::pair<iterator,bool>(make_iterator(p.first),p.second);
  }

  void erase(iterator position)
  {
    final_erase_(static_cast<final_node_type*>(position.get_node()));
  }
  
  void erase(iterator first,iterator last)
  {
    while(first!=last){
      erase(first++);
    }
  }

  bool update(iterator position,value_param_type x)
  {
    return final_update_(x,static_cast<final_node_type*>(position.get_node()));
  }

#if !defined(BOOST_NO_MEMBER_TEMPLATES)||defined(BOOST_MSVC6_MEMBER_TEMPLATES)
  template<typename Modifier>
  bool modify(iterator position,Modifier mod)
  {
    return final_modify_(mod,static_cast<final_node_type*>(position.get_node()));
  }

#endif

  void swap(listed_index<Allocator,Super,TagList>& x)
  {
    final_swap_(x.final());
  }

  void clear()
  {
    erase(begin(),end());
  }

  /* Create an iterator given a node pointer. This should be protected,
   * but it is not simple to do so in a reasonable manner as it gets called
   * by global project().
   */

  iterator       make_iterator(node_type* node){return iterator(node);}
  const_iterator make_iterator(node_type* node)const{return const_iterator(node);}

protected:
  listed_index(const ctor_args_list& args_list,const Allocator& al):
    Super(args_list.get_tail(),al)
  {
    empty_initialize();
  }

  listed_index(const listed_index<Allocator,Super,TagList>& x):
    Super(x)
  {
    empty_initialize();
  }

  std::pair<node_type*,bool> insert_(value_param_type v,node_type* x)
  {
    link(x);
    boost::indexed_sets::detail::scope_guard undo=
      boost::indexed_sets::detail::make_obj_guard(*this,&listed_index::unlink,x);

    typedef typename Super::node_type super_node_type;
    std::pair<super_node_type*,bool> p=Super::insert_(v,x);
    if(p.second)undo.dismiss();

    return std::pair<node_type*,bool>(static_cast<node_type*>(p.first),p.second);
  }

  std::pair<node_type*,bool> insert_(value_param_type v,node_type* position,node_type* x)
  {
    link(x);
    boost::indexed_sets::detail::scope_guard undo=
      boost::indexed_sets::detail::make_obj_guard(*this,&listed_index::unlink,x);

    typedef typename Super::node_type super_node_type;
    std::pair<super_node_type*,bool> p=Super::insert_(v,position,x);
    if(p.second)undo.dismiss();

    return std::pair<node_type*,bool>(static_cast<node_type*>(p.first),p.second);
  }

  void erase_(node_type* x)
  {
    unlink(x);
    Super::erase_(x);
  }

  void swap_(listed_index<Allocator,Super,TagList>& x)
  {
    Super::swap_(x);
  }

  bool update_(value_param_type v,node_type* x)
  {
    return Super::update_(v,x);
  }

#if !defined(BOOST_NO_MEMBER_TEMPLATES)||defined(BOOST_MSVC6_MEMBER_TEMPLATES)
  bool modify_(node_type* x)
  {
    detail::scope_guard local_eraser=detail::make_obj_guard(
      *this,&listed_index::unlink,x);
    if(Super::modify_(x)){
      local_eraser.dismiss();
      return true;
    }
    return false;
  }
#endif

#if defined(BOOST_INDEXED_SET_ENABLE_INVARIANT_CHECKING)
  /* invariant stuff */

  bool invariant_()const
  {
    return Super::invariant_();
  }
#endif

private:
  node_type* header()const{return final_header();}

  void empty_initialize()
  {
    header()->prior=header()->next=header();
  }

  void link(node_type* x)
  {
    x->prior=header()->prior;
    x->next=header();
    x->prior->next=x->next->prior=x;
  };

  void unlink(node_type* x)
  {
    x->prior->next=x->next;
    x->next->prior=x->prior;
  }

  void relink(node_type* position,node_type* x)
  {
    unlink(x);
    x->prior=position->prior;
    x->next=position;
    x->prior->next=x->next->prior=x;
  }
};

} /* namespace boost_indexed_set_ext::detail */

template <typename TagList=tag<> >
struct listed
{
  template<typename Super>
  struct node_type
  {
    typedef detail::listed_index_node<Super> type;
  };

  template<typename Allocator,typename Super>
  struct index_class_type
  {
    typedef detail::listed_index<Allocator,Super,TagList> type;
  };

  BOOST_STATIC_ASSERT(boost::indexed_sets::detail::is_tag<TagList>::value);
};

} /* namespace boost_indexed_set_ext */

/* comparison operators and global swap not implemented yet */

using namespace boost::indexed_sets;

int main()
{
  typedef indexed_set<
    int,
    index_list<
      unique<identity<int> >,
      boost_indexed_set_ext::listed<>
    >
  > listed_set;
  typedef nth_index_type<listed_set,1>::type listed_index;

  listed_set    ls;
  listed_index& li=get<1>(ls);

  li.push_front(0);                       /* 0        */
  li.push_front(4);                       /* 40       */
  ls.insert(2);                           /* 402      */
  ls.insert(5);                           /* 4025     */
  li.push_front(3);                       /* 34025    */
  li.push_back(6);                        /* 340256   */
  li.push_back(1);                        /* 3402561  */
  li.insert(project<1>(ls,ls.find(2)),8); /* 34082561 */

  ls.erase(6);                            /* 3408251  */

  std::copy(ls.begin(),ls.end(),std::ostream_iterator<int>(std::cout));
  std::cout<<std::endl;
  std::copy(li.begin(),li.end(),std::ostream_iterator<int>(std::cout));

  return 0;
}

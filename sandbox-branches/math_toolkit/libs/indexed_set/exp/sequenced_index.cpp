#include <boost/indexed_set.hpp>
#include <algorithm>
#include <iostream>
#include <iterator>

namespace boost{

namespace indexed_sets{

namespace detail{

template<typename Super>
struct sequenced_index_node:Super
{
  sequenced_index_node* prior;
  sequenced_index_node* next;
};

template<typename Node>
class sequenced_index_iterator:
  public boost::bidirectional_iterator_helper<
    sequenced_index_iterator<Node>,
    const typename Node::value_type>
{
public:
  sequenced_index_iterator(){}
  sequenced_index_iterator(Node* node_):node(node_){}

  const typename Node::value_type& operator*()const
  {
    return node->value;
  }

  sequenced_index_iterator& operator++()
  {
    node=node->next;
    return *this;
  }

  sequenced_index_iterator& operator--()
  {
    node=node->prior;
    return *this;
  }

  friend bool operator==(const sequenced_index_iterator& x,const sequenced_index_iterator& y)
  {
    return x.node==y.node;
  }

  /* get_node is not to be used by the user */

  Node* get_node()const{return node;}

private:
  Node* node;
};

template<typename Allocator,typename Super,typename TagList>
class sequenced_index:BOOST_INDEXED_SET_PROTECTED_IF_MEMBER_TEMPLATE_FRIENDS Super
{ 
protected:
  typedef sequenced_index_node<typename Super::node_type> node_type;

public:
  /* types */

  typedef typename node_type::value_type             value_type;
  typedef tuples::null_type                          ctor_args;
  typedef Allocator                                  allocator_type;
  typedef typename Allocator::reference              reference;
  typedef typename Allocator::const_reference        const_reference;
  typedef sequenced_index_iterator<node_type>        iterator;
  typedef sequenced_index_iterator<node_type>        const_iterator;
  typedef std::size_t                                size_type;      
  typedef std::ptrdiff_t                             difference_type;
  typedef typename Allocator::pointer                pointer;
  typedef typename Allocator::const_pointer          const_pointer;
  typedef typename
    reverse_iterator_generator<iterator>::type       reverse_iterator;
  typedef typename
    reverse_iterator_generator<const_iterator>::type const_reverse_iterator;
  typedef typename TagList::type                     tag_list;

protected:
  typedef typename Super::final_node_type              final_node_type;
  typedef tuples::cons<
    ctor_args, 
    typename Super::ctor_args_list>                    ctor_args_list;
  typedef typename mpl::push_front<
    typename Super::index_type_list,
    sequenced_index>::type                             index_type_list;
  typedef typename mpl::push_front<
    typename Super::iterator_type_list,iterator>::type iterator_type_list;
  typedef typename mpl::push_front<
    typename Super::const_iterator_type_list,
    const_iterator>::type                              const_iterator_type_list;

private:
  typedef typename call_traits<value_type>::param_type value_param_type;

public:

  /* construct/copy/destroy
   * Default and copy ctors are in the protected section as indices are
   * not supposed to be created on their own. No range ctor either.
   */

  sequenced_index<Allocator,Super,TagList>& operator=(
    const sequenced_index<Allocator,Super,TagList>& x)
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

  void swap(sequenced_index<Allocator,Super,TagList>& x)
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
  sequenced_index(const ctor_args_list& args_list,const Allocator& al):
    Super(args_list.get_tail(),al)
  {
    empty_initialize();
  }

  sequenced_index(const sequenced_index<Allocator,Super,TagList>& x):
    Super(x)
  {
    empty_initialize();
  }

  std::pair<node_type*,bool> insert_(value_param_type v,node_type* x)
  {
    link(x);
    detail::scope_guard undo=detail::make_obj_guard(*this,&sequenced_index::unlink,x);

    typedef typename Super::node_type super_node_type;
    std::pair<super_node_type*,bool> p=Super::insert_(v,x);
    if(p.second)undo.dismiss();

    return std::pair<node_type*,bool>(static_cast<node_type*>(p.first),p.second);
  }

  std::pair<node_type*,bool> insert_(value_param_type v,node_type* position,node_type* x)
  {
    link(x);
    detail::scope_guard undo=detail::make_obj_guard(*this,&sequenced_index::unlink,x);

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

  void swap_(sequenced_index<Allocator,Super,TagList>& x)
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
      *this,&sequenced_index::unlink,x);
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

} /* namespace indexed_sets::detail */

template <typename TagList=tag<> >
struct sequenced
{
  template<typename Super>
  struct node_type
  {
    typedef detail::sequenced_index_node<Super> type;
  };

  template<typename Allocator,typename Super>
  struct index_class_type
  {
    typedef detail::sequenced_index<Allocator,Super,TagList> type;
  };

  BOOST_STATIC_ASSERT(detail::is_tag<TagList>::value);
};

} /* namespace indexed_sets */

} /* namespace boost */

/* comparison operators and global swap not implemented yet */

using namespace boost::indexed_sets;

int main()
{
  typedef indexed_set<
    int,
    index_list<
      unique<identity<int> >,
      sequenced<>
    >
  > sequenced_set;
  typedef nth_index_type<sequenced_set,1>::type sequenced_index;

  sequenced_set    ss;
  sequenced_index& si=get<1>(ss);

  si.push_front(0);                       /* 0        */
  si.push_front(4);                       /* 40       */
  ss.insert(2);                           /* 402      */
  ss.insert(5);                           /* 4025     */
  si.push_front(3);                       /* 34025    */
  si.push_back(6);                        /* 340256   */
  si.push_back(1);                        /* 3402561  */
  si.insert(project<1>(ss,ss.find(2)),8); /* 34082561 */

  ss.erase(6);                            /* 3408251  */

  std::copy(ss.begin(),ss.end(),std::ostream_iterator<int>(std::cout));
  std::cout<<std::endl;
  std::copy(si.begin(),si.end(),std::ostream_iterator<int>(std::cout));

  return 0;
}

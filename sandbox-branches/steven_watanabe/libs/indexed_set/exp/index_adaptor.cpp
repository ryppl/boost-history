#include <boost/indexed_set.hpp>
#include <boost/mpl/pop_front.hpp>
#include <boost/signal.hpp>
#include <iostream>

namespace boost{

namespace indexed_set_ext{

namespace detail{

template<typename Super>
class index_adaptor:BOOST_INDEXED_SET_PROTECTED_IF_MEMBER_TEMPLATE_FRIENDS Super
{ 
protected:
  /* types */

  typedef Super::node_type                      node_type;
  typedef typename mpl::push_front<
    typename mpl::pop_front<
      typename Super::index_type_list
    >::type,
    index_adaptor>::type                        index_type_list;
  typedef typename Super::final_allocator_type  final_allocator_type;
  typedef typename Super::ctor_args_list        ctor_args_list;
  typedef typename Super::copy_map_type         copy_map_type;
  typedef typename node_type::value_type        value_type;

private:
  typedef typename call_traits<value_type>::param_type value_param_type;

public:
  signal1<void,value_param_type> on_insert;

BOOST_INDEXED_SET_PROTECTED_IF_MEMBER_TEMPLATE_FRIENDS:
  index_adaptor(const ctor_args_list& args_list,const final_allocator_type& al):
    Super(args_list,al)
  {
  }

  index_adaptor(const index_adaptor<Super>& x):
    Super(x)
  {
  }

  void copy_(const index_adaptor<Super>& x,const copy_map_type& map)
  {
    Super::copy_(x,map);
  }

  node_type* insert_(value_param_type v,node_type* x)
  {
    on_insert(v);
    return Super::insert_(v,x);
  }

  node_type* insert_(value_param_type v,node_type* position,node_type* x)
  {
    return Super::insert_(v,position,x);
  }

  void erase_(node_type* x)
  {
    Super::erase_(x);
  }

  void swap_(index_adaptor<Super>& x)
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
    return Super::modify_(x);
  }
#endif

#if defined(BOOST_INDEXED_SET_ENABLE_INVARIANT_CHECKING)
  bool invariant_()const
  {
    return Super::invariant_();
  }
#endif
};

} /* namespace indexed_set_ext::detail */

template <typename IndexSpecifier>
struct notifying
{
  template<typename Super>
  struct node_type
  {
    //typedef typename IndexSpecifier::node_type<Super>::type type;

    typedef typename indexed_sets::detail::msvc_index_specifier<IndexSpecifier>::
      template result_node_type<Super>::type type;

  };

  template<typename Super>
  struct index_class_type
  {
    typedef typename indexed_sets::detail::msvc_index_specifier<IndexSpecifier>::
      template result_index_type<Super>::type   adapted_index;

    typedef detail::index_adaptor<adapted_index> type;
  };
};

} /* namespace boost::indexed_set_ext */

} /* namespace boost */

/* comparison operators and global swap not implemented yet */

using namespace boost::indexed_sets;
using namespace boost::indexed_set_ext;

struct insert_log
{
  void operator()(int x)
  {
    std::clog<<"insert: "<<x<<std::endl;
  }
};

int main()
{
  typedef indexed_set<
    int,
    index_list<
      notifying<unique<identity<int> > >, // notifying index
      non_unique<identity<int> >
    >
  > indexed_t;

  indexed_t t;

  t.on_insert.connect(insert_log()); // on_insert is the signal associated to insertions

  t.insert(0);
  t.insert(1);

  return 0;
}

// output:
//   insert: 0
//   insert: 1

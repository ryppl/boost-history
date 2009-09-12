//Purpose:
//  Prototype of using multiple inheiritance to avoid 
//  recursive tuple implementation.
//
//  Also, use package_range_c instead of make_indexes from
//  [n2080].
//
//References:
//
//  [std.c++]
      /*
      Post:
        http://groups.google.com/group/comp.std.c++/msg/6449d909fd3d5cdc
      Headers:
        From: Douglas Gregor <doug.gregor@gmail.com>
        Newsgroups: comp.std.c++
        Subject: Re: Variadic Templates in C++0x need some additional 
          features to come closer to fulfilling their promise
        Date: Mon, 12 Jan 2009 19:07:31 CST
       */
//       
//  [n2080]
//    Section A.3 Function Object Binders,
//    p. 16 of: 
//      http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2006/n2080.pdf       
//

#include <boost/mpl/package_range_c.hpp>
//^above is replacement for make_indexes in [n2080]
#include  <boost/type_traits/remove_reference.hpp>

using namespace boost;

//From [std.c++]
//-{
 template<typename Key, typename Value>
 struct _Tuple_element {
   Value value;
 };


 template<typename Keys, typename... Elements>
 struct _Tuple_impl;

 template<typename Index, Index... Indices, typename... Elements>
 struct _Tuple_impl<mpl::package_c<Index,Indices...>, Elements...>
   : public _Tuple_element<mpl::integral_c<Index, Indices>, Elements>... 
 { /* and so on */ 
 };

template<typename Index, typename... Elements>
struct tuple 
 : public _Tuple_impl
   < typename mpl::package_range_c
     < Index
     , Index(0)
     , Index(sizeof...(Elements))
     >::type
   , Elements...
   > 
{
    template<Index I, typename T>
        static 
      T&
    at_elem(_Tuple_element<mpl::integral_c<Index,I>,T>& a_elem)
    {
        return a_elem.value;
    }
        
    template<Index I, typename T>
        static 
      T const&
    at_elem(_Tuple_element<mpl::integral_c<Index,I>,T>const& a_elem)
    {
        return a_elem.value;
    }
        
    template<Index I>
      struct
    at_type
    {
     private:
            static
          tuple
        our_tuple
        ;
     public:
            typedef
          typename remove_reference
          < decltype
            ( tuple::at_elem<I>(our_tuple)
            )
          >::type
        type
        ;
    };
};

template<typename Index, Index I, typename... Elements>
  typename tuple<Index, Elements...>::template at_type<I>::type&
at_index( tuple<Index, Elements...>& a_tuple )
{
    return tuple<Index, Elements...>::template at_elem<I>(a_tuple);
}
template<typename Index, Index I, typename... Elements>
  typename tuple<Index, Elements...>::template at_type<I>::type const&
at_index( tuple<Index, Elements...>const& a_tuple )
{
    return tuple<Index, Elements...>::template at_elem<I>(a_tuple);
}

//-}
   
 
struct type_base
{
 public:
    unsigned instance(void)const{ return my_instance;}
 protected:
    type_base(void)
      : my_instance(our_instances++)
      {}
    type_base(type_base const& a_self)
      : my_instance(a_self.my_instance)
      {}
    void operator=(type_base const& a_self)
      {
          my_instance= a_self.my_instance;
      }
 private:    
    static unsigned our_instances;
    unsigned my_instance;
    
};
unsigned type_base::our_instances=0;

template<typename Index, Index I> 
struct type_i
: type_base
{
};

  enum
tuple_index
{ ti_0
, ti_1
, ti_2
}
;
    typedef
  tuple
  < tuple_index
  , type_i<tuple_index,ti_0>
  , type_i<tuple_index,ti_1>
  , type_i<tuple_index,ti_0>
  >
type3_type
;
#include <boost/test/minimal.hpp>
#include <boost/mpl/assert.hpp>
#include <boost/type_traits/is_same.hpp>

type3_type t3;

void simple_test(void)
{
    type_i<tuple_index,ti_0> val0=type3_type::at_elem<ti_0>(t3);
    type_i<tuple_index,ti_1> val1=type3_type::at_elem<ti_1>(t3);
    type_i<tuple_index,ti_0> val2=type3_type::at_elem<ti_2>(t3);
    BOOST_CHECK(val0.instance() == 0);
    BOOST_CHECK(val1.instance() == 1);
    BOOST_CHECK(val2.instance() == 2);
}    

void at_type_test(void)
{
    BOOST_MPL_ASSERT((is_same<type3_type::at_type<ti_0>::type,type_i<tuple_index,ti_0> >));
    BOOST_MPL_ASSERT((is_same<type3_type::at_type<ti_1>::type,type_i<tuple_index,ti_1> >));
    BOOST_MPL_ASSERT((is_same<type3_type::at_type<ti_2>::type,type_i<tuple_index,ti_0> >));
}    

void at_index_test(void)
{
    type_i<tuple_index,ti_0> val0=at_index<tuple_index,ti_0>(t3);
    type_i<tuple_index,ti_1> val1=at_index<tuple_index,ti_1>(t3);
    type_i<tuple_index,ti_0> val2=at_index<tuple_index,ti_2>(t3);
    BOOST_CHECK(val0.instance() == 0);
    BOOST_CHECK(val1.instance() == 1);
    BOOST_CHECK(val2.instance() == 2);
}    

  int
test_main(int,char**)
{
    simple_test();
    at_index_test();
    return 0;
}

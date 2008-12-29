/////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Olaf Krzikalla 2004-2006.
// (C) Copyright Ion Gaztanaga  2006-2007.
//
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/intrusive for documentation.
//
/////////////////////////////////////////////////////////////////////////////
#include <boost/intrusive/detail/config_begin.hpp>
#include <boost/intrusive/splay_set.hpp>
#include <boost/intrusive/detail/pointer_to_other.hpp>
#include "itestvalue.hpp"
#include "smart_ptr.hpp"
#include "generic_multiset_test.hpp"

namespace boost { namespace intrusive { namespace test {

#if !defined (BOOST_INTRUSIVE_VARIADIC_TEMPLATES)
template<class T, class O1, class O2, class O3, class O4>
#else
template<class T, class ...Options>
#endif
struct has_const_overloads<boost::intrusive::splay_multiset<
#if !defined (BOOST_INTRUSIVE_VARIADIC_TEMPLATES)
T, O1, O2, O3, O4
#else
T, Options...
#endif
>
>
{
   static const bool value = false;
};

#if !defined (BOOST_INTRUSIVE_VARIADIC_TEMPLATES)
template<class T, class O1, class O2, class O3, class O4>
#else
template<class T, class ...Options>
#endif
struct has_splay<boost::intrusive::splay_multiset<T, 
   #if !defined (BOOST_INTRUSIVE_VARIADIC_TEMPLATES)
   O1, O2, O3, O4
   #else
   Options...
   #endif
> >
{
   static const bool value = true;
};

#if !defined (BOOST_INTRUSIVE_VARIADIC_TEMPLATES)
template<class T, class O1, class O2, class O3, class O4>
#else
template<class T, class ...Options>
#endif
struct has_rebalance<boost::intrusive::splay_multiset<T, 
   #if !defined (BOOST_INTRUSIVE_VARIADIC_TEMPLATES)
   O1, O2, O3, O4
   #else
   Options...
   #endif
> >
{
   static const bool value = true;
};

}}}

template< class ValueType
        , class Option1 = boost::intrusive::none
        , class Option2 = boost::intrusive::none
        , class Option3 = boost::intrusive::none
        >
struct GetContainer
{
   typedef boost::intrusive::splay_multiset
      < ValueType
      , Option1
      , Option2
      , Option3
      > type;
};

template<class VoidPointer, bool constant_time_size>
class test_main_template
{
   public:
   int operator()()
   {
      using namespace boost::intrusive;
      typedef testvalue<VoidPointer, constant_time_size> value_type;

      test::test_generic_multiset < typename detail::get_base_value_traits
                  < value_type
                  , typename value_type::splay_set_base_hook_t
                  >::type
               , GetContainer
               >::test_all();
      test::test_generic_multiset < typename detail::get_member_value_traits
                  < value_type
                  , member_hook< value_type
                              , typename value_type::splay_set_member_hook_t
                              , &value_type::splay_set_node_
                              >
                  >::type
               , GetContainer
               >::test_all();
      return 0;
   }
};

template<class VoidPointer>
class test_main_template<VoidPointer, false>
{
   public:
   int operator()()
   {
      using namespace boost::intrusive;
      typedef testvalue<VoidPointer, false> value_type;

      test::test_generic_multiset < typename detail::get_base_value_traits
                  < value_type
                  , typename value_type::splay_set_base_hook_t
                  >::type
               , GetContainer
               >::test_all();

      test::test_generic_multiset < typename detail::get_member_value_traits
                  < value_type
                  , member_hook< value_type
                              , typename value_type::splay_set_member_hook_t
                              , &value_type::splay_set_node_
                              >
                  >::type
               , GetContainer
               >::test_all();

      test::test_generic_multiset < typename detail::get_base_value_traits
                  < value_type
                  , typename value_type::splay_set_auto_base_hook_t
                  >::type
               , GetContainer
               >::test_all();

      test::test_generic_multiset < typename detail::get_member_value_traits
                  < value_type
                  , member_hook< value_type
                              , typename value_type::splay_set_auto_member_hook_t
                              , &value_type::splay_set_auto_node_
                              >
                  >::type
               , GetContainer
               >::test_all();
      return 0;
   }
};

int main( int, char* [] ) 
{
   test_main_template<void*, false>()();
   test_main_template<boost::intrusive::smart_ptr<void>, false>()();
   test_main_template<void*, true>()();
   test_main_template<boost::intrusive::smart_ptr<void>, true>()();
   return boost::report_errors();
}

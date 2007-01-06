/////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Olaf Krzikalla 2004-2007.
// (C) Copyright Ion Gaztañaga  2006.
//
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/intrusive for documentation.
//
/////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_INTRUSIVE_ITESTVALUE
#define BOOST_INTRUSIVE_ITESTVALUE

#ifdef _MSC_VER
//#pragma warning(disable : 4584)
#endif

#include <iostream>
#include <boost/intrusive/iset.hpp>
#include <boost/intrusive/ilist.hpp>
#include <boost/intrusive/islist.hpp>
#include <boost/intrusive/ihashset.hpp>
#include <boost/intrusive/detail/pointer_to_other.hpp>
#include <boost/functional/hash.hpp>
#include "smart_ptr.hpp"

namespace boost{
namespace intrusive{

template<class VoidPointer, bool ConstantTimeSize>
struct testvalue
   :  boost::intrusive::iset_base_hook<0, true, VoidPointer>
   ,  boost::intrusive::iset_auto_base_hook<0, VoidPointer>
   ,  boost::intrusive::ihashset_base_hook<0, true, VoidPointer>
   ,  boost::intrusive::ihashset_auto_base_hook<0, VoidPointer>
   ,  boost::intrusive::ilist_base_hook<0, true, VoidPointer>
   ,  boost::intrusive::ilist_auto_base_hook<0, VoidPointer>
   ,  boost::intrusive::islist_base_hook<0, true, VoidPointer>
   ,  boost::intrusive::islist_auto_base_hook<0, VoidPointer>
{

   typedef boost::intrusive::iset_base_hook<0, true, VoidPointer> set_base_hook;
   typedef boost::intrusive::iset_member_hook<testvalue, true, VoidPointer> set_member_hook;
   typedef boost::intrusive::iset_auto_base_hook<0, VoidPointer> set_auto_base_hook;
   typedef boost::intrusive::iset_auto_member_hook<testvalue, VoidPointer> set_auto_member_hook;

   typedef boost::intrusive::ihashset_base_hook<0, true, VoidPointer> hashset_base_hook;
   typedef boost::intrusive::ihashset_member_hook<testvalue, true, VoidPointer> hashset_member_hook;
   typedef boost::intrusive::ihashset_auto_base_hook<0, VoidPointer> hashset_auto_base_hook;
   typedef boost::intrusive::ihashset_auto_member_hook<testvalue, VoidPointer> hashset_auto_member_hook;

   typedef boost::intrusive::ilist_base_hook<0, true, VoidPointer> list_base_hook;
   typedef boost::intrusive::ilist_member_hook<testvalue, true, VoidPointer> list_member_hook;
   typedef boost::intrusive::ilist_auto_base_hook<0, VoidPointer> list_auto_base_hook;
   typedef boost::intrusive::ilist_auto_member_hook<testvalue, VoidPointer> list_auto_member_hook;

   typedef boost::intrusive::islist_base_hook<0, true, VoidPointer> slist_base_hook;
   typedef boost::intrusive::islist_member_hook<testvalue, true, VoidPointer> slist_member_hook;
   typedef boost::intrusive::islist_auto_base_hook<0, VoidPointer> slist_auto_base_hook;
   typedef boost::intrusive::islist_auto_member_hook<testvalue, VoidPointer> slist_auto_member_hook;


   typedef typename boost::intrusive::detail::pointer_to_other
      <VoidPointer, testvalue>::type pointer;

   //Set members
   //pointer set_parent_, set_left_, set_right_;
   //int set_color_;
   set_member_hook set_node_;
   set_auto_member_hook set_auto_node_;
   hashset_member_hook hashset_node_;
   hashset_auto_member_hook hashset_auto_node_;

   //List members
   //pointer list_next_, list_prev_;
   list_member_hook list_node_;
   list_auto_member_hook list_auto_node_;

   //Slist members
   //pointer slist_next_;
   slist_member_hook slist_node_;
   slist_auto_member_hook slist_auto_node_;

   int value_;

   enum{  constant_time_size = ConstantTimeSize  };

   testvalue()
   {  /*init_nodes();  */}

   testvalue (const testvalue& src)
      :  value_ (src.value_)
   {  /*init_nodes();  */}

   // testvalue is used in std::vector and thus prev and next
   // have to be handled appropriately when copied:

   testvalue & operator= (const testvalue& src)
   {
      set_base_hook::operator=(src);
      set_member_hook::operator=(src.set_node_);
      set_auto_base_hook::operator=(src);
      set_auto_member_hook::operator=(src.set_auto_node_);

      hashset_base_hook::operator=(src);
      hashset_member_hook::operator=(src.hashset_node_);
      hashset_auto_base_hook::operator=(src);
      hashset_auto_member_hook::operator=(src.hashset_auto_node_);

      list_base_hook::operator=(src);
      list_member_hook::operator=(src.list_node_);
      list_auto_base_hook::operator=(src);
      list_auto_member_hook::operator=(src.list_auto_node_);

      slist_base_hook::operator=(src);;
      slist_member_hook::operator=(src.slist_node_);
      slist_auto_base_hook::operator=(src);;
      slist_auto_member_hook::operator=(src.slist_auto_node_);
      value_ = src.value_;
      return *this;
   }

   void swap_nodes(testvalue &other)
   {
      //Set has no swapping

      //...

      //List
      list_base_hook::swap_nodes(other);
      list_node_.swap_nodes(other.list_node_);
      list_auto_base_hook::swap_nodes(other);
      list_auto_node_.swap_nodes(other.list_auto_node_);

/*
      typedef boost::intrusive::ilist_ptr_to_member
            < testvalue        , pointer
            , &testvalue::list_prev_, &testvalue::list_next_> list_member_ptr_value_traits;
      typedef boost::intrusive::ilist<list_member_ptr_value_traits, constant_time_size> member_ptr_list;
      list_member_ptr_value_traits::swap_nodes(*this, other);
*/
      //Slist
      slist_base_hook::swap_nodes(other);
      slist_node_.swap_nodes(other.slist_node_);
      slist_auto_base_hook::swap_nodes(other);
      slist_auto_node_.swap_nodes(other.slist_auto_node_);

/*
      typedef boost::intrusive::islist_ptr_to_member
            < testvalue        , pointer
            , &testvalue::slist_next_> slist_member_ptr_value_traits;
      typedef boost::intrusive::islist<slist_member_ptr_value_traits, constant_time_size> member_ptr_slist;
      slist_member_ptr_value_traits::swap_nodes(*this, other);
*/
   }

   ~testvalue()
   {}

   bool operator< (const testvalue &other) const
   {  return value_ < other.value_;  }

   bool operator==(const testvalue &other) const
   {  return value_ == other.value_;  }

   private:
/*
   void init_nodes()
   {
      //Set
      typedef boost::intrusive::iset_ptr_to_member_hook
         <testvalue, pointer, int
         ,&testvalue::set_color_, 0, 1
         ,&testvalue::set_parent_, &testvalue::set_left_, &testvalue::set_right_>
         set_member_ptr_value_traits;
      //List
      typedef boost::intrusive::ilist_ptr_to_member
            < testvalue        
            , pointer
            , &testvalue::list_prev_
            , &testvalue::list_next_> list_member_ptr_value_traits;

      //Slist
      typedef boost::intrusive::islist_ptr_to_member
            < testvalue        
            , pointer
            , &testvalue::slist_next_> slist_member_ptr_value_traits;

      this->set_base_hook::init_node();
      this->set_node_.init_node();
      set_member_ptr_value_traits::init_node(*this);
      this->list_base_hook::init_node();
      this->list_node_.init_node();
      list_member_ptr_value_traits::init_node(*this);
      this->slist_base_hook::init_node();
      this->slist_node_.init_node();
      slist_member_ptr_value_traits::init_node(*this);
   }
*/
};

template<class VoidPointer, bool ConstantTimeSize>
std::size_t hash_value(const testvalue<VoidPointer, ConstantTimeSize> &t)
{
   boost::hash<int> hasher;
   return hasher(t.value_);
}

template<class VoidPointer, bool constant_time_size>
std::ostream& operator<<
   (std::ostream& s, const testvalue<VoidPointer, constant_time_size>& t)
{  return s << t.value_;   }

struct even_odd
{
   template<class VoidPointer, bool constant_time_size>
   bool operator()
      (const testvalue<VoidPointer, constant_time_size>& v1
      ,const testvalue<VoidPointer, constant_time_size>& v2) const
   {
      if ((v1.value_ & 1) == (v2.value_ & 1))
         return v1.value_ < v2.value_;
      else
         return v2.value_ & 1;
   }  
};

typedef testvalue<void *, false>          value_r_f;
typedef testvalue<smart_ptr<void>, false> value_s_f;
typedef testvalue<void *, true>           value_r_t;
typedef testvalue<smart_ptr<void>, true>  value_s_t;

//Typedefs
typedef value_r_f::set_base_hook::
   value_traits<value_r_f >                              set_base_raw;

typedef value_r_f::set_member_hook::
      value_traits<&value_r_f::set_node_>                set_member_raw;

typedef value_r_f::set_auto_base_hook::
   value_traits<value_r_f>               set_auto_base_raw;

typedef value_r_f::set_auto_member_hook::
   value_traits<&value_r_f::set_auto_node_>          set_auto_member_raw;

/*
typedef iset_ptr_to_member_hook
   < value_r_f 
   , value_r_f *
   , int 
   , &value_r_f::set_color_
   , 0
   , 1
   , &value_r_f::set_parent_
   , &value_r_f::set_left_
   , &value_r_f::set_right_>                             set_ptr_to_member_raw;
*/
typedef value_s_f::set_base_hook::
   value_traits<value_s_f >                   set_base_smart;

typedef value_s_f::set_member_hook::
      value_traits<&value_s_f::set_node_>         set_member_smart;

typedef value_s_f::set_auto_base_hook::
   value_traits<value_s_f>               set_auto_base_smart;

typedef value_s_f::set_auto_member_hook::
      value_traits<&value_s_f::set_auto_node_>                    set_auto_member_smart;

/*
typedef iset_ptr_to_member_hook
   < value_s_f
   , smart_ptr<value_s_f >
   , int 
   , &value_s_f::set_color_
   , 0
   , 1
   , &value_s_f::set_parent_
   , &value_s_f::set_left_
   , &value_s_f::set_right_>                  set_ptr_to_member_smart;
*/
typedef value_r_t::set_base_hook::
   value_traits<value_r_t >                              set_base_raw_t;

typedef value_r_t::set_member_hook::
      value_traits<&value_r_t::set_node_>                    set_member_raw_t;
/*
typedef iset_ptr_to_member_hook
   < value_r_t 
   , value_r_t *
   , int 
   , &value_r_t::set_color_
   , 0
   , 1
   , &value_r_t::set_parent_
   , &value_r_t::set_left_
   , &value_r_t::set_right_>                             set_ptr_to_member_raw_t;
*/
typedef value_s_t::set_base_hook::
   value_traits<value_s_t >                   set_base_smart_t;

typedef value_s_t::set_member_hook::
      value_traits<&value_s_t::set_node_>         set_member_smart_t;
/*
typedef iset_ptr_to_member_hook
   < value_s_t
   , smart_ptr<value_s_t >
   , int 
   , &value_s_t::set_color_
   , 0
   , 1
   , &value_s_t::set_parent_
   , &value_s_t::set_left_
   , &value_s_t::set_right_>                  set_ptr_to_member_smart_t;
*/

//Typedefs
typedef value_r_f::hashset_base_hook::
   value_traits<value_r_f >                              hashset_base_raw;

typedef value_r_f::hashset_member_hook::
      value_traits<&value_r_f::hashset_node_>                hashset_member_raw;

typedef value_r_f::hashset_auto_base_hook::
   value_traits<value_r_f>               hashset_auto_base_raw;

typedef value_r_f::hashset_auto_member_hook::
   value_traits<&value_r_f::hashset_auto_node_>          hashset_auto_member_raw;

/*
typedef ihashset_ptr_to_member_hook
   < value_r_f 
   , value_r_f *
   , int 
   , &value_r_f::hashset_color_
   , 0
   , 1
   , &value_r_f::hashset_parent_
   , &value_r_f::hashset_left_
   , &value_r_f::hashset_right_>                             hashset_ptr_to_member_raw;
*/
typedef value_s_f::hashset_base_hook::
   value_traits<value_s_f >                   hashset_base_smart;

typedef value_s_f::hashset_member_hook::
      value_traits<&value_s_f::hashset_node_>         hashset_member_smart;

typedef value_s_f::hashset_auto_base_hook::
   value_traits<value_s_f>               hashset_auto_base_smart;

typedef value_s_f::hashset_auto_member_hook::
      value_traits<&value_s_f::hashset_auto_node_>                    hashset_auto_member_smart;

/*
typedef ihashset_ptr_to_member_hook
   < value_s_f
   , smart_ptr<value_s_f >
   , int 
   , &value_s_f::hashset_color_
   , 0
   , 1
   , &value_s_f::hashset_parent_
   , &value_s_f::hashset_left_
   , &value_s_f::hashset_right_>                  hashset_ptr_to_member_smart;
*/
typedef value_r_t::hashset_base_hook::
   value_traits<value_r_t >                              hashset_base_raw_t;

typedef value_r_t::hashset_member_hook::
      value_traits<&value_r_t::hashset_node_>                    hashset_member_raw_t;
/*
typedef ihashset_ptr_to_member_hook
   < value_r_t 
   , value_r_t *
   , int 
   , &value_r_t::hashset_color_
   , 0
   , 1
   , &value_r_t::hashset_parent_
   , &value_r_t::hashset_left_
   , &value_r_t::hashset_right_>                             hashset_ptr_to_member_raw_t;
*/
typedef value_s_t::hashset_base_hook::
   value_traits<value_s_t >                   hashset_base_smart_t;

typedef value_s_t::hashset_member_hook::
      value_traits<&value_s_t::hashset_node_>         hashset_member_smart_t;
/*
typedef ihashset_ptr_to_member_hook
   < value_s_t
   , smart_ptr<value_s_t >
   , int 
   , &value_s_t::hashset_color_
   , 0
   , 1
   , &value_s_t::hashset_parent_
   , &value_s_t::hashset_left_
   , &value_s_t::hashset_right_>                  hashset_ptr_to_member_smart_t;
*/


//Explicit instantiations
typedef value_r_f::list_base_hook::
   value_traits<value_r_f>               list_base_raw;

typedef value_r_f::list_member_hook::
   value_traits<&value_r_f::list_node_>          list_member_raw;

typedef value_r_f::list_auto_base_hook::
   value_traits<value_r_f>               list_auto_base_raw;

typedef value_r_f::list_auto_member_hook::
   value_traits<&value_r_f::list_auto_node_>          list_auto_member_raw;
/*
typedef boost::intrusive::ilist_ptr_to_member
      < value_r_f        , value_r_f *
      , &value_r_f::list_prev_, &value_r_f::list_next_>            list_ptr_to_member_raw;
*/
typedef value_s_f::list_base_hook::
   value_traits<value_s_f>               list_base_smart;

typedef value_s_f::list_member_hook::
      value_traits<&value_s_f::list_node_>                    list_member_smart;

typedef value_s_f::list_auto_base_hook::
   value_traits<value_s_f>               list_auto_base_smart;

typedef value_s_f::list_auto_member_hook::
      value_traits<&value_s_f::list_auto_node_>                    list_auto_member_smart;

/*
typedef boost::intrusive::ilist_ptr_to_member
      < value_s_f
      , smart_ptr<value_s_f>
      , &value_s_f::list_prev_
      , &value_s_f::list_next_>                               list_ptr_to_member_smart;
*/

typedef value_r_t::list_base_hook::
   value_traits<value_r_t>               list_base_raw_t;

typedef value_r_t::list_member_hook::
   value_traits<&value_r_t::list_node_>  list_member_raw_t;

/*
typedef boost::intrusive::ilist_ptr_to_member
      < value_r_t        , value_r_t *
      , &value_r_t::list_prev_, &value_r_t::list_next_>            list_ptr_to_member_raw_t;
*/
typedef value_s_t::list_base_hook::
      value_traits<value_s_t>               list_base_smart_t;

typedef value_s_t::list_member_hook::
      value_traits<&value_s_t::list_node_>                    list_member_smart_t;

/*
typedef boost::intrusive::ilist_ptr_to_member
      < value_s_t
      , smart_ptr<value_s_t>
      , &value_s_t::list_prev_
      , &value_s_t::list_next_>                    list_ptr_to_member_smart_t;
*/
//Explicit instantiations
typedef value_r_f::slist_base_hook::
   value_traits<value_r_f>               slist_base_raw;

typedef value_r_f::slist_member_hook::
   value_traits<&value_r_f::slist_node_>          slist_member_raw;

typedef value_r_f::slist_auto_base_hook::
   value_traits<value_r_f>               slist_auto_base_raw;

typedef value_r_f::slist_auto_member_hook::
   value_traits<&value_r_f::slist_auto_node_>          slist_auto_member_raw;
/*
typedef boost::intrusive::islist_ptr_to_member
      < value_r_f        , value_r_f *
      , &value_r_f::slist_next_>            slist_ptr_to_member_raw;
*/
typedef value_s_f::slist_base_hook::
   value_traits<value_s_f>               slist_base_smart;

typedef value_s_f::slist_member_hook::
      value_traits<&value_s_f::slist_node_>                    slist_member_smart;

typedef value_s_f::slist_auto_base_hook::
   value_traits<value_s_f>               slist_auto_base_smart;

typedef value_s_f::slist_auto_member_hook::
      value_traits<&value_s_f::slist_auto_node_>                    slist_auto_member_smart;

/*
typedef boost::intrusive::islist_ptr_to_member
      < value_s_f
      , smart_ptr<value_s_f>
      , &value_s_f::slist_next_>                               slist_ptr_to_member_smart;
*/

typedef value_r_t::slist_base_hook::
   value_traits<value_r_t>               slist_base_raw_t;

typedef value_r_t::slist_member_hook::
   value_traits<&value_r_t::slist_node_>          slist_member_raw_t;
/*
typedef boost::intrusive::islist_ptr_to_member
      < value_r_t        , value_r_t *
      ,&value_r_t::slist_next_>            slist_ptr_to_member_raw_t;
*/
typedef value_s_t::slist_base_hook::
      value_traits<value_s_t>               slist_base_smart_t;

typedef value_s_t::slist_member_hook::
      value_traits<&value_s_t::slist_node_>                    slist_member_smart_t;
/*
typedef boost::intrusive::islist_ptr_to_member
      < value_s_t
      , smart_ptr<value_s_t>
      , &value_s_t::slist_next_>                    slist_ptr_to_member_smart_t;
*/
}  //namespace boost{
}  //namespace intrusive{

#endif

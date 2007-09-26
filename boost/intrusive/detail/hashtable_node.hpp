/////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Ion Gaztanaga  2007
//
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/intrusive for documentation.
//
/////////////////////////////////////////////////////////////////////////////

#ifndef BOOST_INTRUSIVE_HASHTABLE_NODE_HPP
#define BOOST_INTRUSIVE_HASHTABLE_NODE_HPP

#include <boost/intrusive/detail/config_begin.hpp>
#include <iterator>
#include <boost/intrusive/detail/assert.hpp>
#include <boost/intrusive/detail/pointer_to_other.hpp>
#include <boost/intrusive/circular_list_algorithms.hpp>
#include <boost/intrusive/detail/mpl.hpp>
#include <boost/intrusive/detail/utilities.hpp>
#include <boost/intrusive/detail/slist_node.hpp> //remove-me
#include <cstddef>

namespace boost {
namespace intrusive {
namespace detail {

template<int Dummy = 0>
struct prime_list_holder
{
   static const std::size_t prime_list[];
   static const std::size_t prime_list_size;
};

template<int Dummy>
const std::size_t prime_list_holder<Dummy>::prime_list[] = {
   53ul, 97ul, 193ul, 389ul, 769ul,
   1543ul, 3079ul, 6151ul, 12289ul, 24593ul,
   49157ul, 98317ul, 196613ul, 393241ul, 786433ul,
   1572869ul, 3145739ul, 6291469ul, 12582917ul, 25165843ul,
   50331653ul, 100663319ul, 201326611ul, 402653189ul, 805306457ul,
   1610612741ul, 3221225473ul, 4294967291ul };

template<int Dummy>
const std::size_t prime_list_holder<Dummy>::prime_list_size
   = sizeof(prime_list)/sizeof(std::size_t);

template <class Slist>
struct bucket_impl : public Slist
{
   bucket_impl()
   {}

   bucket_impl(const bucket_impl &)
   {}

   ~bucket_impl()
   {
      //This bucket is still being used!
      BOOST_INTRUSIVE_INVARIANT_ASSERT(Slist::empty());
   }

   bucket_impl &operator=(const bucket_impl&)
   {
      //This bucket is still in use!
      BOOST_INTRUSIVE_INVARIANT_ASSERT(Slist::empty());
      //Slist::clear();
   }

   static typename Slist::difference_type get_bucket_num
      ( typename Slist::const_iterator it
      , const bucket_impl<Slist> &first_bucket
      , const bucket_impl<Slist> &last_bucket)
   {
      typename Slist::const_iterator
         first(first_bucket.cend()), last(last_bucket.cend());

      //The end node is embedded in the singly linked list:
      //iterate until we reach it.
      while(!(first.pointed_node() <= it.pointed_node() &&
              it.pointed_node() <= last.pointed_node())){
         ++it;
      }
      //Now get the bucket_impl from the iterator
      const bucket_impl &b = static_cast<const bucket_impl&>
         (Slist::container_from_end_iterator(it));

      //Now just calculate the index b has in the bucket array
      return &b - &first_bucket;
   }
};

template<class Slist>
struct bucket_traits_impl
{
   /// @cond
   typedef typename boost::pointer_to_other
      < typename Slist::pointer, bucket_impl<Slist> >::type bucket_ptr;
   typedef typename Slist::size_type size_type;
   /// @endcond

   bucket_traits_impl(bucket_ptr buckets, size_type len)
      :  buckets_(buckets), buckets_len_(len)
   {}

   bucket_ptr bucket_begin() const
   {  return buckets_;  }

   size_type  bucket_count() const
   {  return buckets_len_;  }

   private:
   bucket_ptr  buckets_;
   size_type   buckets_len_;
};

template<class Container, bool IsConst>
class hashtable_iterator
   :  public std::iterator
         < std::forward_iterator_tag
         , typename detail::add_const_if_c
            <typename Container::value_type, IsConst>::type
         >
{
   typedef typename Container::real_value_traits               real_value_traits;
   typedef typename Container::siterator                       siterator;
   typedef typename Container::const_siterator                 const_siterator;
   typedef typename Container::bucket_type                     bucket_type;
   typedef typename boost::pointer_to_other
      < typename Container::pointer, const Container>::type    const_cont_ptr;
   typedef typename Container::size_type                       size_type;

   public:
   typedef typename detail::add_const_if_c
      <typename Container::value_type, IsConst>::type          value_type;

   hashtable_iterator ()
   {}

   explicit hashtable_iterator(siterator ptr, const Container *cont)
      :  slist_it_ (ptr),   cont_ (cont)
   {}

   hashtable_iterator(const hashtable_iterator<Container, false> &other)
      :  slist_it_(other.slist_it()), cont_(other.get_container())
   {}

   const siterator &slist_it() const
   { return slist_it_; }

   public:
   hashtable_iterator& operator++() 
   {  this->increment();   return *this;   }
   
   hashtable_iterator operator++(int)
   {
      hashtable_iterator result (*this);
      this->increment();
      return result;
   }

   friend bool operator== (const hashtable_iterator& i, const hashtable_iterator& i2)
   { return i.slist_it_ == i2.slist_it_; }

   friend bool operator!= (const hashtable_iterator& i, const hashtable_iterator& i2)
   { return !(i == i2); }

   value_type& operator*() const
   { return *this->operator ->(); }

   value_type* operator->() const
   { return detail::get_pointer(this->get_real_value_traits()->to_value_ptr(slist_it_.pointed_node())); }

   const Container *get_container() const
   {  return detail::get_pointer(cont_);  }

   const real_value_traits *get_real_value_traits() const
   {  return &this->get_container()->get_real_value_traits();  }

   private:
   void increment()
   {
      const Container *cont =  detail::get_pointer(cont_);
      bucket_type* buckets = detail::get_pointer(cont->bucket_pointer());
      size_type   buckets_len    = cont->bucket_count();
      const_siterator first(buckets[0].cend());
      const_siterator last (buckets[buckets_len].cend());

      ++slist_it_;
      if(first.pointed_node()    <= slist_it_.pointed_node() && 
         slist_it_.pointed_node()<= last.pointed_node()      ){
         size_type n_bucket = (size_type)
               bucket_type::get_bucket_num(slist_it_, buckets[0], buckets[buckets_len]);
         do{
            if (++n_bucket == buckets_len){
               slist_it_ = buckets->end();
               break;
            }
            slist_it_ = buckets[n_bucket].begin();
         }
         while (slist_it_ == buckets[n_bucket].end());
      }
   }

   siterator      slist_it_;
   const_cont_ptr cont_;
};

}  //namespace detail {
}  //namespace intrusive {
}  //namespace boost {

#endif

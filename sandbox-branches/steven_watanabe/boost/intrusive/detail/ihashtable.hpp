/////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Ion Gaztañaga  2006.
//
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/intrusive for documentation.
//
/////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_INTRUSIVE_IHASHTABLE_HPP
#define BOOST_INTRUSIVE_IHASHTABLE_HPP

#include <boost/intrusive/detail/config_begin.hpp>
//std C++
#include <functional>
#include <iterator>
#include <utility>
#include <algorithm>
//boost
#include <boost/utility.hpp>
#include <boost/assert.hpp>
#include <boost/static_assert.hpp>
#include <boost/functional/hash.hpp>
//General intrusive utilities
#include <boost/intrusive/detail/pointer_type.hpp>
#include <boost/intrusive/detail/pointer_to_other.hpp>
#include <boost/intrusive/value_traits_type.hpp>
//Implementation utilities
#include <boost/intrusive/ihashset_hook.hpp>
//#include <boost/intrusive/detail/hashtable_node.hpp>
//#include <boost/intrusive/hashtable_algorithms.hpp>
#include <boost/intrusive/islist.hpp>

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

//! The class template ihashtable is an intrusive red-black tree container, that
//! is used to construct intrusive set and tree containers. The no-throw 
//! guarantee holds only, if the Equal object 
//! doesn't throw.
template< class ValueTraits
        , class Hash             = boost::hash<typename ValueTraits::value_type>
        , class Equal            = std::equal_to<typename ValueTraits::value_type>
        , bool  ConstantTimeSize = false
        , class SizeType         = std::size_t
        >
class ihashtable
   :  private detail::size_holder<ConstantTimeSize, SizeType>
{
   private:
   template<class T, class Self, class NodeTraits>
   class hashtable_iterator;

   typedef islist<ValueTraits, false, SizeType> islist_impl;

   struct bucket_type_impl
      :  private islist_impl
   {
      friend class ihashtable<ValueTraits, Hash, Equal, ConstantTimeSize, SizeType>;

      bucket_type_impl()
      {}

      bucket_type_impl(const bucket_type_impl &)
      {}

      bucket_type_impl &operator=(const bucket_type_impl&)
      {  islist_impl::clear();   }
   };

   static islist_impl &islist_from_bucket(bucket_type_impl &b)
   {  return static_cast<islist_impl&>(b);   }

   static const islist_impl &islist_from_bucket(const bucket_type_impl &b)
   {  return static_cast<const islist_impl&>(b);   }

   typedef ihashtable<ValueTraits, Hash, Equal
                     ,ConstantTimeSize, SizeType>           this_type; 
   typedef typename ValueTraits::node_traits                node_traits;
   typedef detail::size_holder<ConstantTimeSize, SizeType>  size_traits;

   //noncopyable
   ihashtable (const ihashtable&);
   ihashtable operator =(const ihashtable&);

   public:
   typedef typename ValueTraits::value_type        value_type;
   typedef typename detail::pointer_to_other
      <typename node_traits::void_pointer
      ,value_type>::type                           pointer;
   typedef typename detail::pointer_to_other
      <pointer, const value_type>::type            const_pointer;
   typedef value_type&                             reference;
   typedef const value_type&                       const_reference;
   typedef typename size_traits::size_type         size_type;
   typedef typename std::iterator_traits
      <pointer>::difference_type                   difference_type;
   typedef value_type                              key_type;
   typedef Hash                                    hasher;
   typedef Equal                                   key_equal;
   typedef bucket_type_impl                        bucket_type;
   typedef typename detail::pointer_to_other
      <pointer, bucket_type>::type                 bucket_ptr;
   typedef typename islist_impl::iterator          local_iterator;
   typedef typename islist_impl::const_iterator    const_local_iterator;

   private:
   typedef typename node_traits::node              node;
   typedef typename detail::pointer_to_other
      <pointer, node>::type                        node_ptr;
   typedef typename detail::pointer_to_other
      <node_ptr, const node>::type                 const_node_ptr;
   //typedef hashtable_algorithms<node_traits>       sequence_algorithms;
   typedef slist_algorithms<node_traits>           sequence_algorithms;
   enum { safemode_or_autounlink  = 
            (int)ValueTraits::type == (int)auto_unlink_value_traits   ||
            (int)ValueTraits::type == (int)safe_mode_value_traits     };

   //Constant-time size is incompatible with auto-unlink hooks!
   BOOST_STATIC_ASSERT(!(ConstantTimeSize && ((int)ValueTraits::type == (int)auto_unlink_value_traits)));

   template<class KeyValueEqual>
   struct key_node_ptr_equal
      :  private KeyValueEqual
   {
      key_node_ptr_equal(KeyValueEqual kcomp)
         :  KeyValueEqual(kcomp)
      {}

      template<class KeyType>
      bool operator()(node_ptr node, const KeyType &key) const
      {  return KeyValueEqual::operator()(*ValueTraits::to_value_ptr(node), key); }

      template<class KeyType>
      bool operator()(const KeyType &key, node_ptr node) const
      {  return KeyValueEqual::operator()(key, *ValueTraits::to_value_ptr(node)); }

      bool operator()(node_ptr node1, node_ptr node2) const
      {
         return KeyValueEqual::operator()
            (*ValueTraits::to_value_ptr(node1), *ValueTraits::to_value_ptr(node2)); 
      }
   };

   //User scattered inheritance to get EBO in all compilers
   struct bucket_info_t
      :  public Hash
   {
      bucket_info_t( const Hash &hasher
                   , bucket_ptr buckets
                   , size_type buckets_len)
         : Hash(hasher), buckets_(buckets), buckets_len_(buckets_len)
      {}

      bucket_ptr  buckets_;
      size_type   buckets_len_;
   } ;

   struct members
      :  public Equal
   {

      bucket_info_t bucket_info_;
      members( bucket_ptr buckets
             , size_type buckets_len
             , const Hash &hasher
             , const Equal &equal)
         :  Equal(equal), bucket_info_(hasher, buckets, buckets_len)
      {}
   } members_;

   typedef typename detail::pointer_to_other
      <typename node_traits::void_pointer
      ,bucket_info_t>::type                         bucket_info_ptr;

   typedef typename detail::pointer_to_other
      <typename node_traits::void_pointer
      ,const bucket_info_t>::type                   const_bucket_info_ptr;

   const Hash &hash() const
   {  return members_.bucket_info_;  }

   Hash &hash()
   {  return members_.bucket_info_;  }

   const Equal &eq() const
   {  return members_;  }

   Equal &eq()
   {  return members_;  }

   const bucket_ptr &buckets() const
   {  return members_.bucket_info_.buckets_;  }

   bucket_ptr &buckets()
   {  return members_.bucket_info_.buckets_;  }

   const size_type &buckets_len() const
   {  return members_.bucket_info_.buckets_len_;  }

   size_type &buckets_len()
   {  return members_.bucket_info_.buckets_len_;  }

   bucket_info_ptr bucket_info()
   {  return bucket_info_ptr(&members_.bucket_info_);  }

   const_bucket_info_ptr bucket_info() const
   {  return const_bucket_info_ptr(&members_.bucket_info_);  }

   template<class F>
   struct value_to_node_cloner
      :  private F
   {
      value_to_node_cloner(F f)
         :  F(f)
      {}
      
      node_ptr operator()(node_ptr p) const
      {  return ValueTraits::to_node_ptr(*F::operator()(*ValueTraits::to_value_ptr(p))); }
   };

   template<class F>
   struct value_to_node_destroyer
      :  private F
   {
      value_to_node_destroyer(F f)
         :  F(f)
      {}

      void operator()(node_ptr p) const
      {  F::operator()(ValueTraits::to_value_ptr(p));   }
   };

   static node_ptr uncast(const_node_ptr ptr)
   {
      using boost::get_pointer;
      return node_ptr(const_cast<node*>(get_pointer(ptr)));
   }

   static bucket_info_ptr uncast(const_bucket_info_ptr ptr)
   {
      using boost::get_pointer;
      return bucket_info_ptr(const_cast<bucket_info_t*>(get_pointer(ptr)));
   }

   template<class T, class Self, class NodeTraits>
   class hashtable_iterator
      :  public std::iterator<std::forward_iterator_tag, T>
   {
      protected:
      explicit hashtable_iterator
         (local_iterator ptr, bucket_info_ptr bucket_info, size_type n_bucket)
         :  local_it_ (ptr),   bucket_info_ (bucket_info),   n_bucket_ (n_bucket)
      {}

      public:
      hashtable_iterator ()
      {}

      Self& operator++()
      {
         using boost::get_pointer;
         ++local_it_;
         bucket_info_t *info   = get_pointer(bucket_info_);
         bucket_type *buckets  = get_pointer(info->buckets_);
			while (local_it_ == islist_from_bucket(buckets[n_bucket_]).end()){
            size_type buckets_len  = info->buckets_len_;
				if (++n_bucket_ == buckets_len){
					local_it_ = invalid_local_it(*info);
               break;
            }
            local_it_ = islist_from_bucket(buckets[n_bucket_]).begin();
			}
         return static_cast<Self&> (*this);
      }

      Self operator++(int)
      {
         Self result(local_it_, bucket_info_, n_bucket_);
         ++(*this);
         return result;
      }

      bool operator== (const Self& i) const
      { return local_it_ == i.local(); }

      bool operator!= (const Self& i) const
      { return !operator== (i); }

      Self &set_internals
         (local_iterator ptr, bucket_info_ptr bucket_info, size_type n_bucket)
      {
         local_it_ = ptr;  bucket_info_ = bucket_info;  n_bucket_ = n_bucket;
         return static_cast<Self&>(*this);
      }

      local_iterator local() const
      { return local_it_; }

      bucket_info_ptr bucket_info() const
      { return bucket_info_; }

      size_type bucket_num() const
      { return n_bucket_; }

      private:
      local_iterator    local_it_;
      bucket_info_ptr   bucket_info_;
      size_type         n_bucket_;
   };

   public:

   class iterator
      :  public hashtable_iterator <value_type, iterator, node_traits>
    {
      private:
      typedef typename ihashtable<ValueTraits, Hash, Equal, ConstantTimeSize, SizeType>::value_type   private_vt;
      typedef typename ihashtable<ValueTraits, Hash, Equal, ConstantTimeSize, SizeType>::pointer      private_pointer;
      typedef typename ihashtable<ValueTraits, Hash, Equal, ConstantTimeSize, SizeType>::reference    private_reference;
      typedef hashtable_iterator<private_vt, iterator, node_traits>   inherited;

      public:
      iterator()
      {}

      private_pointer operator->() const
      { return &*this->local(); }

      private_reference operator*() const
      { return *this->local(); }

      private:
      explicit iterator
         (local_iterator local_it, bucket_info_ptr bucket_info, size_type n_bucket)
         :  inherited(local_it, bucket_info, n_bucket)
      {}

      friend struct std::pair<iterator, iterator>;

      friend class ihashtable<ValueTraits, Hash, Equal, ConstantTimeSize, SizeType>; 
      friend class hashtable_iterator<private_vt, iterator, node_traits>;
   };

   class const_iterator
      :  public hashtable_iterator<const value_type, const_iterator, node_traits>
   {
      private:
      typedef const typename ihashtable<ValueTraits, Hash, Equal, ConstantTimeSize, SizeType>::value_type private_vt;
      typedef typename ihashtable<ValueTraits, Hash, Equal, ConstantTimeSize, SizeType>::const_pointer    private_pointer;
      typedef typename ihashtable<ValueTraits, Hash, Equal, ConstantTimeSize, SizeType>::const_reference  private_reference;
      typedef hashtable_iterator<private_vt, const_iterator, node_traits>   inherited;

      public:
      const_iterator()
      {}

      const_iterator(const iterator& it)
         :  inherited (it.local(), it.bucket_info(), it.bucket_num())
      {}

      const_iterator & operator=(const typename ihashtable::iterator& it)
      {  return inherited::set_internals(it.local(), it.bucket_info(), it.bucket_num());  }

      private_pointer operator->() const
      { return &*this->local(); }

      private_reference operator*() const
      { return *this->local(); }

      private:
      explicit const_iterator
         (local_iterator ptr, bucket_info_ptr bucket_info, size_type n_bucket)
         :  inherited(ptr, bucket_info, n_bucket)
      {}

      friend struct std::pair<const_iterator, const_iterator>;
      friend class ihashtable<ValueTraits, Hash, Equal, ConstantTimeSize, SizeType>; 
      friend class hashtable_iterator<private_vt, const_iterator, node_traits>;
   };

   //typedef typename sequence_algorithms::insert_commit_data insert_commit_data;
   struct insert_commit_data
   {
      local_iterator prev_pos;
      size_type   bucket_num;
   };

   ihashtable( bucket_ptr buckets
             , size_type buckets_len
             , const Hash & hasher = Hash()
             , const Equal &equal = Equal()) 
      :  members_(buckets, buckets_len, hasher, equal)
   {
      BOOST_ASSERT(buckets_len != 0);
      priv_clear_buckets();
      size_traits::set_size(size_type(0));
   }

   ~ihashtable() 
   {  this->clear(); }

   iterator begin()
   {
      size_type bucket_num;
      local_iterator local_it = priv_begin(bucket_num);
      return iterator( local_it
                     , this->bucket_info()
                     , bucket_num);
   }

   const_iterator begin() const
   {
      size_type bucket_num;
      local_iterator local_it = priv_begin(bucket_num);
      return const_iterator( local_it
                           , uncast(this->bucket_info())
                           , bucket_num);
   }

   iterator end()
   {
      using boost::get_pointer;
      bucket_info_t *info = get_pointer(this->bucket_info());
      return iterator(invalid_local_it(*info), 0, info->buckets_len_);
   }

   const_iterator end() const
   {
      using boost::get_pointer;
      const bucket_info_t *info = get_pointer(this->bucket_info());
      return const_iterator(invalid_local_it(*info), 0, info->buckets_len_);
   }

   hasher hash_function() const
   {  return this->hash();  }

   key_equal key_eq() const
   {  return this->eq();   }

   bool empty() const
   {
      if(ConstantTimeSize){
         return !size();
      }
      else{
         using boost::get_pointer;
         size_type buckets_len = this->buckets_len();
         const bucket_type *b = get_pointer(this->buckets());
         for (size_type n = 0; n < buckets_len; ++n, ++b){
            if(!b->empty()){
               return false;
            }
         }
         return true;
      }
   }

   size_type size() const
   {
      if(ConstantTimeSize)
         return size_traits::get_size();
      else{
         size_type len = 0;
         using boost::get_pointer;
         size_type buckets_len = this->buckets_len();
         const bucket_type *b = get_pointer(this->buckets());
         for (size_type n = 0; n < buckets_len; ++n, ++b){
            len += b->size();
         }
         return len;
      }
   }

   void swap(ihashtable& other)
   {
      std::swap(this->eq(), other.eq());
      std::swap(this->hash(), other.hash());
      std::swap(this->buckets(), other.buckets());
      std::swap(this->buckets_len(), other.buckets_len());
      if(ConstantTimeSize){
         size_type backup = size_traits::get_size();
         size_traits::set_size(other.get_size());
         other.set_size(backup);
      }
   }

   iterator insert_equal(value_type& val)
   {
      size_type bucket_num;
      local_iterator it = priv_find(val, this->hash(), this->eq(), bucket_num);
      bucket_type &b = this->buckets()[bucket_num];
      if(it == invalid_local_it(*this->bucket_info())){
         it = b.before_begin();
      }
      size_traits::increment();
      return iterator(b.insert_after(it, val), this->bucket_info(), bucket_num);
   }

   template<class Iterator>
   void insert_equal(Iterator b, Iterator e)
   {
      for (; b != e; ++b)
         this->insert_equal(*b);
   }

   std::pair<iterator, bool> insert_unique(value_type& val)
   {
      insert_commit_data commit_data;
      std::pair<iterator, bool> ret = insert_unique_check(val, commit_data);
      if(!ret.second)
         return ret;
      return std::pair<iterator, bool> (insert_unique_commit(val, commit_data), true);
   }

   template<class Iterator>
   void insert_unique(Iterator b, Iterator e)
   {
      for (; b != e; ++b)
         this->insert_unique(*b);
   }

   std::pair<iterator, bool> insert_unique_check
      (const value_type &value, insert_commit_data &commit_data)
   {  return insert_unique_check(value, this->hash(), this->eq(), commit_data); }

   template<class KeyType, class KeyHasher, class KeyValueEqual>
   std::pair<iterator, bool> insert_unique_check
      ( const KeyType &key
      , const KeyHasher &hasher
      , const KeyValueEqual &key_value_eq
      , insert_commit_data &commit_data)
   {
      commit_data.prev_pos = 
         priv_find(key, hasher, key_value_eq, commit_data.bucket_num);
      bool success = commit_data.prev_pos == invalid_local_it(*this->bucket_info());
      if(success){
         commit_data.prev_pos = this->buckets()[commit_data.bucket_num].before_begin();
      }
      return std::pair<iterator, bool>
         (iterator(commit_data.prev_pos, this->bucket_info(), commit_data.bucket_num)
         ,success);
   }

   iterator insert_unique_commit(value_type &val, const insert_commit_data &commit_data)
   {
      bucket_type &b = this->buckets()[commit_data.bucket_num];
      size_traits::increment();
      return iterator( b.insert_after(commit_data.prev_pos, val)
                     , this->bucket_info()
                     , commit_data.bucket_num);
   }

   void erase(iterator i)
   {
      local_iterator to_erase(i.local());
      size_type n_bucket = i.bucket_num();
      bucket_type &b = this->buckets()[n_bucket];
      b.erase(to_erase);
      size_traits::decrement();
   }

   iterator erase(iterator b, iterator e)
   {  size_type n;   return private_erase(b, e, n);   }

   size_type erase(const value_type &value)
   {
      key_node_ptr_equal<key_equal> key_node_eq(this->eq());
      std::pair<iterator,iterator> p = this->equal_range(value);
      size_type n;
      private_erase(p.first, p.second, n);
      return n;
   }

   template<class KeyType, class KeyHasher, class KeyValueEqual>
   size_type erase(const KeyType& key, const KeyHasher &hasher, const KeyValueEqual &equal)
   {
      std::pair<iterator,iterator> p = this->equal_range(key, hasher, equal);
      size_type n;
      private_erase(p.first, p.second, n);
      return n;
   }

   template<class Destroyer>
   iterator erase(iterator i, Destroyer destroyer)
   {
      local_iterator to_erase(i.local());
      size_type n_bucket = i.bucket_num();
      bucket_type &b = this->buckets()[n_bucket];
      b.erase_after(b.previous(to_erase), destroyer);
      size_traits::decrement();
   }

   template<class Destroyer>
   iterator erase(iterator b, iterator e, Destroyer destroyer)
   {  size_type n;   return private_erase(b, e, n, destroyer);   }

   template<class Destroyer>
   size_type erase(const value_type &value, Destroyer destroyer)
   {
      std::pair<iterator,iterator> p = this->equal_range(value);
      size_type n;
      private_erase(p.first, p.second, n, destroyer);
      return n;
   }

   template<class KeyType, class KeyHasher, class KeyValueEqual, class Destroyer>
   size_type erase(const KeyType& key, const KeyHasher &hasher
                  ,const KeyValueEqual &equal, Destroyer destroyer)
   {
      std::pair<iterator,iterator> p = this->equal_range(key, hasher, equal);
      size_type n;
      private_erase(p.first, p.second, n, destroyer);
      return n;
   }

   void clear()
   {
      if(safemode_or_autounlink){
         priv_clear_buckets();
      }
      size_traits::set_size(size_type(0));
   }

   template<class Destroyer>
   void clear(Destroyer destroyer)
   {
      iterator beg(this->begin());
      iterator end(this->end());
      while(beg != end){
         iterator to_erase(beg);
         ++beg;
         node_ptr p(beg.local());
         this->erase(to_erase);
         destroyer(ValueTraits::to_value_ptr(p));
      }
   }

   size_type count(const value_type &value) const
   {
      std::pair<const_iterator, const_iterator> ret = this->equal_range(value);
      return std::distance(ret.first, ret.second);
   }

   template<class KeyType, class KeyHasher, class KeyValueEqual>
   size_type count(const KeyType &key, const KeyHasher &hasher, const KeyValueEqual &equal) const
   {
      std::pair<const_iterator, const_iterator> ret = this->equal_range(key, hasher, equal);
      return std::distance(ret.first, ret.second);
   }

   iterator find(const value_type &value)
   {  return find(value, this->hash(), this->eq());   }

   template<class KeyType, class KeyHasher, class KeyValueEqual>
   iterator find(const KeyType &key, const KeyHasher &hasher, const KeyValueEqual &equal)
   {
      size_type bucket_n;
      local_iterator local_it = priv_find(key, hasher, equal, bucket_n);
      return iterator( local_it
                      , this->bucket_info()
                      , bucket_n);
   }

   const_iterator find(const value_type &value) const
   {  return find(value, this->hash(), this->eq());   }

   template<class KeyType, class KeyHasher, class KeyValueEqual>
   const_iterator find
      (const KeyType &key, const KeyHasher &hasher, const KeyValueEqual &equal) const
   {
      size_type bucket_n;
      local_iterator local_it = priv_find(key, hasher, equal, bucket_n);
      return const_iterator( local_it
                           , uncast(this->bucket_info())
                           , bucket_n);
   }

   std::pair<iterator,iterator> equal_range(const value_type &value)
   {  return this->equal_range(value, this->hash(), this->eq());  }

   template<class KeyType, class KeyHasher, class KeyValueEqual>
   std::pair<iterator,iterator> equal_range
      (const KeyType &key, const KeyHasher &hasher, const KeyValueEqual &equal)
   {
      size_type bucket_n1, bucket_n2;
      std::pair<local_iterator, local_iterator> ret
         = priv_equal_range(key, hasher, equal, bucket_n1, bucket_n2);
      return std::pair<iterator, iterator>
         (  iterator( ret.first, this->bucket_info(), bucket_n1)
         ,  iterator( ret.second, this->bucket_info(), bucket_n2) );
   }

   std::pair<const_iterator, const_iterator>
      equal_range(const value_type &value) const
   {  return this->equal_range(value, this->hash(), this->eq());  }

   template<class KeyType, class KeyHasher, class KeyValueEqual>
   std::pair<const_iterator,const_iterator> equal_range
      (const KeyType &key, const KeyHasher &hasher, const KeyValueEqual &equal) const
   {
      size_type bucket_n1, bucket_n2;
      std::pair<local_iterator, local_iterator> ret
         = priv_equal_range(key, hasher, equal, bucket_n1, bucket_n2);
      return std::pair<const_iterator, const_iterator>
         (  const_iterator( ret.first, uncast(this->bucket_info()), bucket_n1)
         ,  const_iterator( ret.second, uncast(this->bucket_info()), bucket_n2)  );
   }

   size_type bucket_count() const
   {  return this->buckets_len();   }

   size_type bucket_size(size_type n)
   {  return this->buckets()[n].size();   }

   size_type bucket(const key_type& k)
   {  return this->bucket(k, this->hash());   }

   template<class KeyType, class KeyHasher>
   size_type bucket(const KeyType& k, const KeyHasher &hasher)
   {  return hasher(k) % this->buckets_len();   }

   bucket_ptr bucket_pointer() const
   {  return this->buckets();   }

   local_iterator begin(size_type n)
   {  return this->buckets()[n].begin();  }

   const_local_iterator begin(size_type n) const
   {  return const_cast<const bucket_type&>(this->buckets()[n]).begin();  }

   local_iterator end(size_type n)
   {  return this->buckets()[n].end();  }

   const_local_iterator end(size_type n) const
   {  return const_cast<const bucket_type&>(this->buckets()[n]).end();  }

   void rehash(bucket_ptr new_buckets, size_type new_buckets_len)
   {
      bucket_ptr old_buckets     = this->buckets();
      size_type  old_buckets_len = this->buckets_len();

      try{
         size_type n = 0;
         bool same_buffer = old_buckets == new_buckets;
         //If we are shrinking the bucket array, just rehash the last nodes
         if(same_buffer && (old_buckets_len > new_buckets_len)){
            n = new_buckets_len;
         }

         //Iterate through nodes
         for(; n < old_buckets_len; ++n){
            bucket_type &old_bucket = old_buckets[n];
            local_iterator before_i(old_bucket.before_begin());
            local_iterator end(old_bucket.end());
            local_iterator i(old_bucket.begin());
            for(;i != end; ++i){
               size_type new_n = this->hash()(*i) % new_buckets_len;
               //If this is a buffer expansion don't move if it's not necessary
               if(same_buffer && new_n == n){
                  ++before_i;
               }
               else{
                  bucket_type &new_b = new_buckets[new_n];
                  new_b.splice_after(new_b.before_begin(), old_bucket, before_i);
                  i = before_i;
               }
            }
         }

         this->buckets()      = new_buckets;
         this->buckets_len()  = new_buckets_len;
      }
      catch(...){
         for(size_type n = 0; n < new_buckets_len; ++n){
            new_buckets[n].clear();
            old_buckets[n].clear();
         }
         size_traits::set_size(size_type(0));
         throw;
      }
   }

   iterator current(value_type& v)
   {
      return iterator( bucket_type::current(v)
                     , this->bucket_info()
                     , this->hash()(v) % this->buckets_len());
   }

   const_iterator current(const value_type& v) const
   {
      return const_iterator( bucket_type::current(const_cast<value_type&>(v))
                     , uncast(this->bucket_info())
                     , this->hash()(v) % this->buckets_len());
   }

   static local_iterator current_local(value_type& v)
   {  return bucket_type::current(v);  }

   static const_local_iterator current_local(const value_type& v)
   {  return bucket_type::current(v);  }

   // no throw
   static size_type suggested_upper_bucket_count(size_type n)
   {
      const std::size_t *primes     = &prime_list_holder<0>::prime_list[0];
      const std::size_t *primes_end = primes + prime_list_holder<0>::prime_list_size;
      size_type const* bound =
            std::lower_bound(primes, primes_end, n);
      if(bound == primes_end)
            bound--;
      return size_type(*bound);
   }

   // no throw
   static size_type suggested_lower_bucket_count(size_type n)
   {
      const std::size_t *primes     = &prime_list_holder<0>::prime_list[0];
      const std::size_t *primes_end = primes + prime_list_holder<0>::prime_list_size;
      size_type const* bound =
            std::upper_bound(primes, primes_end, n);
      if(bound != primes_end)
            bound--;
      return size_type(*bound);
   }

   private:

   static local_iterator invalid_local_it(const bucket_info_t &b)
   {  return b.buckets_->end();  }

   local_iterator priv_begin(size_type &bucket_num) const
   {
      size_type buckets_len = this->buckets_len();
      for (bucket_num = 0; bucket_num < buckets_len; ++bucket_num){
         bucket_type &b = this->buckets()[bucket_num];
         if(!b.empty())
            return b.begin();
      }
      return invalid_local_it(*this->bucket_info());
   }

   void priv_clear_buckets()
   {  priv_clear_buckets(this->buckets(), this->buckets_len());  }

   static void priv_clear_buckets(bucket_ptr buckets_ptr, size_type buckets_len)
   {
      for(size_type i = 0; i < buckets_len; ++i, ++buckets_ptr){
         buckets_ptr->clear();
      }
   }
   template<class KeyType, class KeyHasher, class KeyValueEqual>
   local_iterator priv_find
      ( const KeyType &key, const KeyHasher &hasher
      , const KeyValueEqual &equal, size_type &bucket_number) const
   {
      size_type b_len(this->buckets_len());
      bucket_number = hasher(key) % b_len;
      if(ConstantTimeSize && this->empty()){
         return invalid_local_it(*this->bucket_info());
      }
      bucket_type &b = this->buckets()[bucket_number];
      local_iterator it = b.begin();

      while(it != b.end()){
         if(equal(key, *it)){
            return it;
         }
         ++it;
      }

      return invalid_local_it(*this->bucket_info());
   }

   template<class KeyType, class KeyHasher, class KeyValueEqual>
   std::pair<local_iterator, local_iterator> priv_equal_range
      ( const KeyType &key, const KeyHasher &hasher
      , const KeyValueEqual &equal
      , size_type &bucket_number_first
      , size_type &bucket_number_second) const
   {
      //Let's see if the element is present
      std::pair<local_iterator, local_iterator> to_return
         ( priv_find(key, hasher, equal, bucket_number_first)
         , invalid_local_it(*this->bucket_info()));
      if(to_return.first == to_return.second)
         return to_return;

      //If it's present, find the first that it's not equal in
      //the same bucket
      bucket_type &b = this->buckets()[bucket_number_first];
      local_iterator it = to_return.first;
      ++it;

      while(it != b.end()){
         if(!equal(key, *it)){
            to_return.second = it;
            return to_return;
         }
         ++it;
      }
   
      //If we reached the end, find the first, non-empty bucket
      for(bucket_number_second = bucket_number_first+1
         ; bucket_number_second != this->buckets_len()
         ; ++bucket_number_second){
         bucket_type &b = this->buckets()[bucket_number_second];
         if(!b.empty()){
            to_return.second = b.begin();
            return to_return;
         }
      }

      //Otherwise, return the end node
      to_return.second = invalid_local_it(*this->bucket_info());
      return to_return;
   }

   template<class Destroyer>
   iterator private_erase(iterator b, iterator e, size_type &n, Destroyer destroyer)
   {
      for(n = 0; b != e; ++n)
        this->erase(b++, destroyer);
      return b;
   }

   iterator private_erase(iterator b, iterator e, size_type &n)
   {
      for(n = 0; b != e; ++n)
        this->erase(b++);
      return b;
   }
};

} //namespace detail
} //namespace intrusive 
} //namespace boost 

#include <boost/intrusive/detail/config_end.hpp>

#endif //BOOST_INTRUSIVE_IHASHTABLE_HPP

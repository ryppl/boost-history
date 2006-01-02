//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Ion Gaztañaga 2005. Distributed under the Boost
// Software License, Version 1.0. (See accompanying file
// LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/shmem for documentation.
//
//////////////////////////////////////////////////////////////////////////////

#ifndef BOOST_INTERSEGMENT_PTR_HPP
#define BOOST_INTERSEGMENT_PTR_HPP

#if (defined _MSC_VER) && (_MSC_VER >= 1200)
#  pragma once
#endif

#include <boost/shmem/detail/workaround.hpp>
#include <boost/shmem/detail/config_begin.hpp>
#include <boost/shmem/shmem_fwd.hpp>
#include <boost/shmem/detail/utilities.hpp>
#include <boost/shmem/detail/cast_tags.hpp>
#include <boost/assert.hpp>
#include <boost/shmem/sync/lock.hpp>
#include <boost/type_traits/has_trivial_constructor.hpp>
#include <boost/type_traits/has_trivial_destructor.hpp>
#include <boost/shmem/sync/shared_mutex.hpp>
#include <boost/shmem/containers/flat_map.hpp>
#include <boost/detail/no_exceptions_support.hpp>

/*!\file
   Describes a smart pointer that stores the offset between this pointer and
   target pointee, called intersegment_ptr. This file defines some variations of 
   this pointer. 
*/
namespace boost {

namespace shmem {

/*!Configures intersegment_ptr with the capability to address:
   2^(sizeof(std::size_t)*8/2) segment groups
   2^(sizeof(std::size_t)*8/2) segments per group.
   2^(sizeof(std::size_t)*8/2)-1 bytes maximum per segment.
   The mapping is implemented through flat_maps synchronized with mutexes.*/
template <class Mutex>
struct flat_map_intersegment
{
   typedef flat_map_intersegment<Mutex>   self_t;
   typedef std::size_t                    offset_t;
   typedef std::size_t                    segment_t;

   /*!Returns true if object represents null pointer*/
   bool is_null() const
      {  return m_offset == ~offset_t(0); }

   /*!Sets the object to represent the null pointer*/
   void set_null()
      {  m_offset = ~offset_t(0);  }

   /*!Sets the object internals to represent the address pointed by ptr*/
   void set_from_pointer(const void *ptr)
   {
      if(!ptr){
         set_null();
      }
      else{
         segment_t id1, id2;
         offset_t  offset1, offset2;
         {
            //------------------------------------------------------------------
            boost::shmem::scoped_lock<typename mappings_t::mutex_t> lock(s_map);
            //------------------------------------------------------------------
            get_segment_and_offset(ptr,  id1, offset1);
            get_segment_and_offset(this, id2, offset2);
         }
         m_distance  = id1 - id2;
         m_offset    = offset1;
      }
   }

   /*!Sets the object internals to represent the address pointed 
      by another flat_map_intersegment*/
   void set_from_other(const self_t &other)
   {
      if(other.is_null()){
         set_null();
      }
      else{
         segment_t id1, id2;
         {
            //------------------------------------------------------------------
            boost::shmem::scoped_lock<typename mappings_t::mutex_t> lock(s_map);
            //------------------------------------------------------------------
            offset_t  off;
            get_segment_and_offset(&other, id1, off);
            get_segment_and_offset(this, id2, off);
         }
         m_distance  = id1 + other.m_distance - id2;
         m_offset    = other.m_offset;
      }
   }

   /*!Obtains the address pointed by the object*/
   void *get_pointer() const
   {
      segment_t   id1;
      offset_t    off;
      void *      base_addr = 0;
      if(is_null()){
         return base_addr;
      }
      else{
         {
            //------------------------------------------------------------------
            boost::shmem::scoped_lock<typename mappings_t::mutex_t> lock(s_map);
            //------------------------------------------------------------------
            get_segment_and_offset(this, id1, off);
            segment_t target_segment = segment_t(id1+m_distance);
            base_addr = get_base_address(target_segment);
         }
         return detail::char_ptr_cast(base_addr) + m_offset;
      }
   }

   /*!Swaps two objects efficiently*/
   void swap(self_t &other)
   {
      //This specialized swap avoids some costly operations
      segment_t id1, id2, id;
      offset_t  off;
      {
         //------------------------------------------------------------------
         boost::shmem::scoped_lock<typename mappings_t::mutex_t> lock(s_map);
         //------------------------------------------------------------------
         get_segment_and_offset(&other, id1, off);
         get_segment_and_offset(this,   id2, off);
      }
      //Swap offset value
      off = m_offset; m_offset = other.m_offset; other.m_offset = off;
      //Calculate new segment distance. Let overflow do its work
      id = m_distance;
      m_distance         = id1 + other.m_distance - id2;
      other.m_distance   = id2 + id - id1;
   }

   /*!Increments internal offset*/
   void inc_offset(offset_t bytes)
      {  m_offset += bytes;   }

   /*!Decrements internal offset*/
   void dec_offset(offset_t bytes)
      {  m_offset -= bytes;   }

   /*!Calculates the distance between two basic_intersegment_ptr-s.
      This only works with two basic_intersegment_ptr pointing
      to the same segment. Otherwise undefined*/
   offset_t diff(const self_t &other) const
      {  return m_offset - other.m_offset;   }

   /*!Returns true if both point to the same object*/
   bool equal(const self_t &y) const
   {  
      if(m_offset != y.m_offset){
         return false;
      }
      else{
         segment_t id1, id2;
         offset_t  off;
         {
            //------------------------------------------------------------------
            boost::shmem::scoped_lock<typename mappings_t::mutex_t> lock(s_map);
            //------------------------------------------------------------------
            get_segment_and_offset(this, id1, off);
            get_segment_and_offset(&y,   id2, off);
         }
         return segment_t(id1 + m_distance) == segment_t(id2 + y.m_distance);
      }
   }

   /*!Returns true if *this is less than other.
      This only works with two basic_intersegment_ptr pointing
      to the same segment group. Otherwise undefined. Never throws*/
   bool less(const self_t &y) const
   {  
      segment_t id1, id2;
      offset_t  off;
      {
         //------------------------------------------------------------------
         boost::shmem::scoped_lock<typename mappings_t::mutex_t> lock(s_map);
         //------------------------------------------------------------------
         get_segment_and_offset(this, id1, off);
         get_segment_and_offset(&y,   id2, off);
      }
      id1 = segment_t(id1 + m_distance);
      id2 = segment_t(id2 + y.m_distance);
      return (id1 < id2) || (id1 == id2 && m_offset < y.m_offset);
   }

   //--------------------------------------------------------------------------

   static void get_group_and_id(void *addr, std::size_t &group, std::size_t &id)
   {  
      {
         //------------------------------------------------------------------
         boost::shmem::scoped_lock<typename mappings_t::mutex_t>     lock(s_map);
         //------------------------------------------------------------------
         get_segment_and_offset(addr, group, id);
      }
      group = group >>  s_shift;
   }

   /*!Returns the polymorphic multi-segment creator associated with a
      segment-group. Returns 0 if not found or an error occurs.*/
   static void* find_group_data(std::size_t group)
   {
      typedef typename mappings_t::group_to_data_t::iterator it_t;
      //------------------------------------------------------------------
      boost::shmem::scoped_lock<typename mappings_t::mutex_t>     lock(s_map);
      //------------------------------------------------------------------
      it_t it(s_map.group_to_data.find(group));
      if(it == s_map.group_to_data.end()){
         return 0;
      }
      return it->second;
   }

   /*!Reserves a segment-group, installs the associated polymorphic
      segment-creator, and the segment passed as (base, size) as
      the segment id = 0 of the new group. Returns the group that will
      be associated with this segment. Returns 0 if there are no available
      groups or an error occurs.*/
   static std::size_t new_group(void *group_data)
   {
      typedef typename mappings_t::group_to_data_t::iterator   it_t;
      typedef typename mappings_t::group_to_data_t::value_type value_type;
      typedef std::pair<it_t, bool>                            insert_ret;
      std::size_t group = 0;

      BOOST_TRY{
         //------------------------------------------------------------------
         boost::shmem::scoped_lock<typename mappings_t::mutex_t> lock(s_map);
         //------------------------------------------------------------------
         //Check if there are too much groups
         if(s_map.group_to_data.size() >= (s_max_value - 1) ){
            return 0;
         }
         //Check if there are no registered groups or the first one
         //is not group 1, assign id 1 to the new group.
         else if(s_map.group_to_data.empty() ||
                  s_map.group_to_data.begin()->first != 1){
            group = 1;
         }
         //If not, take a not used number
         else{
            it_t  it1(s_map.group_to_data.begin()), 
                  it2(it1),
                  itend(s_map.group_to_data.end());

            for(++it2; it2 != itend; ++it1, ++it2){
               if(it2->first - it1->first > 1){
                  break;
               }
            }
            group = it1->first+1;
         }
         insert_ret ret = 
            s_map.group_to_data.insert(value_type(group, group_data));
         //This insertion must succeed
         assert(ret.second);
         if(!ret.second)   return false;
         return group;
      }
      BOOST_CATCH(const std::bad_alloc &){}
      BOOST_CATCH_END
      return 0;
   }

   /*!Erases the mapping between a group and the associated polymorphic pointer.
      Returns false if the group is not found or there is an error*/
   static bool delete_group(std::size_t group)
   {
      //------------------------------------------------------------------
      boost::shmem::scoped_lock<typename mappings_t::mutex_t>  lock(s_map);
      //------------------------------------------------------------------
      typedef typename mappings_t::segment_to_ptr_t::iterator  it_t;

      //Find the range of the group
      it_t first_of_group(s_map.segment_to_ptr.lower_bound(group << s_shift));
      it_t one_past_group(s_map.segment_to_ptr.lower_bound((group+1)<< s_shift));

      //Erase all info related to the group
      for(it_t it(first_of_group); it != one_past_group; ++it){
         //Erase entry from addr-segment
         if(!s_map.ptr_to_segment.erase(it->second)){
            //This can't be possible since both indexes should be synchronized
            assert(false);
            return false;
         }
      }
      //Erase all info from groups
      s_map.segment_to_ptr.erase(first_of_group, one_past_group);

      //Erase the group info
      bool ret = s_map.group_to_data.erase(group) == 1;
      assert(ret);
      if(!ret) return false;
      //Erase all mappings of this group
      return true;
   }

   /*!Associates a segment defined by group/id with a base address and size.
      Returns false if the group is not found or there is an error*/
   static bool insert_mapping(std::size_t group,   std::size_t id,
                              void *base_address,  std::size_t size)
   {
      //Check limits
      if(group > s_max_value || id > s_max_value){
         return false;
      }

      BOOST_TRY{
         typedef typename mappings_t::ptr_to_segment_t::value_type   ptr_to_segment_val_t;
         typedef typename mappings_t::segment_to_ptr_t::value_type   segment_to_ptr_val_t;
         typedef typename mappings_t::ptr_to_segment_t::iterator     ptr_to_segment_it_t;
         typedef typename mappings_t::segment_to_ptr_t::iterator     segment_to_ptr_it_t;
         typedef std::pair<ptr_to_segment_it_t, bool>       ptr_to_segment_ret_t;
         typedef std::pair<segment_to_ptr_it_t, bool>       segment_to_ptr_ret_t;

         //Compose segment identification
         segment_t segment = (group << s_shift) | id;
         typename mappings_t:: segment_info_t segment_info;
         segment_info.segment = segment;
         segment_info.size    = size;

         {  
            //------------------------------------------------------------------
            boost::shmem::scoped_lock<typename mappings_t::mutex_t>     lock(s_map);
            //------------------------------------------------------------------
            //This can throw
            ptr_to_segment_ret_t ptr_to_segment_ret = 
               s_map.ptr_to_segment.insert(ptr_to_segment_val_t(base_address, segment_info));

            if(!ptr_to_segment_ret.second)
               return false;

            //Node eraser will erase the node if an exception occurs
            detail::value_eraser<typename mappings_t::ptr_to_segment_t> 
               value_eraser(s_map.ptr_to_segment, ptr_to_segment_ret.first);

            //This can throw
            segment_to_ptr_ret_t segment_to_ptr_ret = 
               s_map.segment_to_ptr.insert(segment_to_ptr_val_t(segment, base_address));

            if(!segment_to_ptr_ret.second){
               //This should never occur, since both maps must have
               //the same elements indexed by different key
               assert(!segment_to_ptr_ret.second);
               return false;
            }
            //Insertion ok, release value_eraser
            value_eraser.release(); 
            return true;                 
         }
      }
      BOOST_CATCH(...){
         return false;
      }
      BOOST_CATCH_END
   }

   static bool erase_mapping(void *base_address)
   {
      typedef typename mappings_t::ptr_to_segment_t::iterator  ptr_to_segment_it_t;
      typedef typename mappings_t::segment_to_ptr_t::iterator  segment_to_ptr_it_t;

      {  //------------------------------------------------------------------
         boost::shmem::scoped_lock<typename mappings_t::mutex_t>     lock(s_map);
         //------------------------------------------------------------------
         ptr_to_segment_it_t ptr_to_segment_it = s_map.ptr_to_segment.find(base_address);
         if(ptr_to_segment_it == s_map.ptr_to_segment.end()){
            //This group/id is not an valid entry
            assert(ptr_to_segment_it == s_map.ptr_to_segment.end());
            return false;
         }
         //Obtain segment
         segment_t segment = ptr_to_segment_it->second.segment;
         //Erase node from map
         s_map.ptr_to_segment.erase(ptr_to_segment_it);
         //Erase entry in the second map
         if(!s_map.segment_to_ptr.erase(segment)){
            //This can't be possible since both indexes should be synchronized
            assert(false);
            return false;
         }
      }
      return true;
   }

   static bool erase_mapping(std::size_t group, std::size_t id)
   {
      //Check limits
      if(group > s_max_value || id > s_max_value){
         return false;
      }

      typedef typename mappings_t::ptr_to_segment_t::iterator ptr_to_segment_it_t;
      typedef typename mappings_t::segment_to_ptr_t::iterator  segment_to_ptr_it_t;

      //Compose segment identification
      segment_t segment = (group << s_shift) | id;
      {  //------------------------------------------------------------------
         boost::shmem::scoped_lock<typename mappings_t::mutex_t>     lock(s_map);
         //------------------------------------------------------------------
         segment_to_ptr_it_t segment_to_ptr_it = s_map.segment_to_ptr.find(segment);
         if(segment_to_ptr_it == s_map.segment_to_ptr.end()){
            //This group/id is not an valid entry
            assert(segment_to_ptr_it != s_map.segment_to_ptr.end());
            return false;
         }
         //Obtain address
         void *address = segment_to_ptr_it->second;
         //Erase node from map
         s_map.segment_to_ptr.erase(segment_to_ptr_it);
         //Erase entry in the second map
         if(!s_map.ptr_to_segment.erase(address)){
            //This can't be possible since both indexes should be synchronized
            assert(false);
            return false;
         }
      }
      return true;
   }

   private:
   /*!Half of the bits are for group id and the
      other half for the index inside the group
      unsigned    group : sizeof(std::size_t)/2;
      unsigned    index : sizeof(std::size_t)/2;*/
   segment_t   m_distance;
   offset_t    m_offset;

   struct mappings_t : Mutex
   {
      struct segment_info_t
      {
         std::size_t size;
         segment_t   segment;
      };

      /*!Mutex to preserve integrity in multi-threaded enviroments*/
      typedef Mutex        mutex_t;
      /*!Maps base addresses and segment information 
         (size and segment group and id)*/
      typedef boost::shmem::flat_map
         <const void * 
         ,segment_info_t
         ,std::less<const void *> >          ptr_to_segment_t;
      /*!Maps segment group/id with base addresses*/
      typedef boost::shmem::flat_map
         <segment_t, void *>                 segment_to_ptr_t;
      /*!Maps segment group with a polymorphic multi-segment creator
         that knows how to create new segments*/
      typedef boost::shmem::flat_map
         <segment_t, void*>                  group_to_data_t;
      
      ptr_to_segment_t     ptr_to_segment;
      segment_to_ptr_t     segment_to_ptr;
      group_to_data_t   group_to_data;

      ~mappings_t()
      {
         //Check that all mappings have been erased
         assert(ptr_to_segment.empty());
         assert(segment_to_ptr.empty());
         assert(group_to_data.empty());
      }
   };

   //Static members
   static mappings_t        s_map;
   static const std::size_t s_shift     = sizeof(std::size_t)*8/2;
   static const std::size_t s_max_value = std::size_t(1) << s_shift;

   private:

   /*!Returns the segment and offset of an address*/
   static void get_segment_and_offset(const void *ptr, segment_t &segment, offset_t &offset)
   {
      if(s_map.ptr_to_segment.empty()){
         segment = 0;
         offset  = detail::char_ptr_cast(ptr) - detail::char_ptr_cast();
      }
      //Find the first base address greater than ptr
      typename mappings_t::ptr_to_segment_t::iterator it 
         = s_map.ptr_to_segment.upper_bound(ptr);
      if(it == s_map.ptr_to_segment.begin()){
         segment = 0;
         offset  = detail::char_ptr_cast(ptr) - detail::char_ptr_cast();
      }
      //Go to the previous one
      --it;
      char *      segment_base = detail::char_ptr_cast(it->first);
      std::size_t segment_size = it->second.size;
      
      if(segment_base <= detail::char_ptr_cast(ptr) &&
         (segment_base + segment_size) >= detail::char_ptr_cast(ptr)){
         segment = it->second.segment;
         offset  = detail::char_ptr_cast(ptr) - segment_base;
      }
      else{
         segment = 0;
         offset  = detail::char_ptr_cast(ptr) - detail::char_ptr_cast();
      }
   }

   /*!Returns the base address of a segment*/
   static void *get_base_address(segment_t segment)
   {
      typename mappings_t::segment_to_ptr_t::iterator it 
         = s_map.segment_to_ptr.find(segment);
      
      if(it == s_map.segment_to_ptr.end()){
         return 0;
      }
      else{
         return it->second;
      }
   }
};

/*!Static map-group associated with flat_map_intersegment<>*/
template <class Mutex>
typename flat_map_intersegment<Mutex>::mappings_t 
   flat_map_intersegment<Mutex>::s_map;

/*!Static constant that shows the number of bits to shift to obtain the 
   group part of segment_t type*/
template <class Mutex>
const std::size_t flat_map_intersegment<Mutex>::s_shift;

/*!Static constant that shows the number of bits to shift to obtain the 
   group part of segment_t type*/
template <class Mutex>
const std::size_t flat_map_intersegment<Mutex>::s_max_value;

/*!
   A smart pointer that can point to a pointee that resides in another memory 
   memory mapped segment.
*/
template <class T, class PT>
class intersegment_ptr : public PT
{
   typedef boost::shmem::workaround::random_it<T>  random_it_t;
   typedef intersegment_ptr<T, PT>                 self_t;
   typedef PT                                      base_t;
   typedef typename random_it_t::const_pointer     const_pointer_t;
   typedef typename random_it_t::const_reference   const_reference_t;

   void unspecified_bool_type_func() const {}
   typedef void (self_t::*unspecified_bool_type)() const;

 public:
   typedef typename random_it_t::pointer           pointer;
   typedef typename random_it_t::reference         reference;
   typedef typename random_it_t::value_type        value_type;
   typedef typename random_it_t::difference_type   difference_type;
   typedef typename random_it_t::iterator_category iterator_category;

 public:   //Public Functions

   /*!Obtains intersegment_ptr <OtherType> from intersegment_ptr*/
   template<class OtherType>
   struct rebind
   {   
      typedef intersegment_ptr<OtherType, PT> type;
   };

   /*!Default Constructor. Never throws*/
   intersegment_ptr(){}

   /*!Constructor from raw pointer (allows "0" pointer conversion). Never throws.*/
   intersegment_ptr(pointer ptr) {  base_t::set_from_pointer(ptr); }

   /*!Constructor from other pointer. Never throws.*/
   template <class U>
   intersegment_ptr(U *ptr) {  base_t::set_from_pointer(pointer(ptr)); }

   /*!Constructor from other intersegment_ptr */
   intersegment_ptr(const intersegment_ptr& ptr) 
      {  base_t::set_from_other(ptr);   }

   /*!Constructor from other intersegment_ptr. If pointers of pointee types are 
      convertible, intersegment_ptrs will be convertibles. Never throws.*/
   template<class T2>
   intersegment_ptr(const intersegment_ptr<T2, PT> &ptr) 
      {  base_t::set_from_other(ptr); }

   /*!Emulates static_cast operator. Never throws.  */
   template<class U>
   intersegment_ptr(const intersegment_ptr<U, PT> &r, detail::static_cast_tag)
      //{  base_t::set_from_pointer(static_cast<T*>(r.get())); }
   {  
      if(r.is_null()){
         base_t::set_from_pointer(0);
      }
      else{
         //Some dirty tricks to safe segment operations.
         //Calculate pointer adjustment and adjust offset.
         pointer ptr = reinterpret_cast<pointer>(this);
         std::ptrdiff_t difference = detail::char_ptr_cast(static_cast<T*>(ptr)) -
                                     detail::char_ptr_cast(ptr);
         base_t::set_from_other(r);
         base_t::inc_offset(difference*sizeof(T));
      }
   }

   /*!Emulates const_cast operator. Never throws.*/
   template<class U>
   intersegment_ptr(const intersegment_ptr<U, PT> &r, detail::const_cast_tag)
      //{  base_t::set_from_pointer(const_cast<T*>(r.get())); }
      {  
//         (void)const_cast<T*>((U*)0);  
         pointer a = const_cast<pointer>((U*)0);
//         (void)const_cast<T*>((U*)0);  
         base_t::set_from_other(r); 
      }

   /*!Emulates dynamic_cast operator. Never throws.*/
   template<class U>
   intersegment_ptr(const intersegment_ptr<U, PT> &r, detail::dynamic_cast_tag)
      {  base_t::set_from_pointer(dynamic_cast<T*>(r.get())); }

   /*!Emulates reinterpret_cast operator. Never throws.*/
   template<class U>
   intersegment_ptr(const intersegment_ptr<U, PT> &r, detail::reinterpret_cast_tag)
      {  base_t::set_from_pointer(reinterpret_cast<T*>(r.get())); }

   /*!Obtains raw pointer from offset. Never throws.*/
   pointer get()const
      {  return (pointer)base_t::get_pointer();   }

   /*!Pointer-like -> operator. It can return 0 pointer. Never throws.*/
   pointer operator->() const           
      {  return self_t::get(); }

   /*!Dereferencing operator, if it is a null intersegment_ptr behavior 
         is undefined. Never throws.*/
   reference operator* () const           
      {  return *(self_t::get());   }

   /*!Indexing operator. Never throws.*/
   reference operator[](std::ptrdiff_t idx) const   
      {  return self_t::get()[idx];  }

   /*!Assignment from pointer (saves extra conversion). Never throws.*/
   intersegment_ptr& operator= (pointer from)
      {  base_t::set_from_pointer(from); return *this;  }

   /*!Assignment from other intersegment_ptr. Never throws.*/
   intersegment_ptr& operator= (const intersegment_ptr &ptr)
      {  base_t::set_from_other(ptr);  return *this;  }

   /*!Assignment from related intersegment_ptr. If pointers of pointee types 
         are assignable, intersegment_ptrs will be assignable. Never throws.*/
   template <class T2>
   intersegment_ptr& operator= (const intersegment_ptr<T2, PT> & ptr)
      {  base_t::set_from_other(ptr); return *this;   }
 
   /*!intersegment_ptr + std::ptrdiff_t. Never throws.*/
   intersegment_ptr operator+ (std::ptrdiff_t idx) const   
   {  
      intersegment_ptr result (*this);
      result.inc_offset(idx*sizeof(T));
      return result;
   }

   /*!std::ptrdiff_t + intersegment_ptr. Never throws.*/
   friend intersegment_ptr operator+ (std::ptrdiff_t idx, const intersegment_ptr &ptr)   
   {  
      intersegment_ptr result (ptr);
      result.inc_offset(idx*sizeof(T));
      return result;
   }

   /*!intersegment_ptr - std::ptrdiff_t. Never throws.*/
   intersegment_ptr operator- (std::ptrdiff_t idx) const   
   {  
      intersegment_ptr result (*this);
      result.dec_offset(idx*sizeof(T));
      return result;
   }

   /*!intersegment_ptr += std::ptrdiff_t. Never throws.*/
   intersegment_ptr &operator+= (std::ptrdiff_t offset)
      {  base_t::inc_offset(offset*sizeof(T));  return *this;  }

   /*!intersegment_ptr -= std::ptrdiff_t. Never throws.*/
   intersegment_ptr &operator-= (std::ptrdiff_t offset)
      {  base_t::dec_offset(offset*sizeof(T));  return *this;  }

   /*!++intersegment_ptr. Never throws.*/
   intersegment_ptr& operator++ (void)
      {  base_t::inc_offset(sizeof(T));   return *this;  }
 
   /*!intersegment_ptr++. Never throws.*/
   intersegment_ptr operator++ (int)
      {  intersegment_ptr temp(*this); ++*this; return temp; }

   /*!--intersegment_ptr. Never throws.*/
   intersegment_ptr& operator-- (void)
      {  base_t::dec_offset(sizeof(T));   return *this;  }

   /*!intersegment_ptr--. Never throws.*/
   intersegment_ptr operator-- (int)
      {  intersegment_ptr temp(*this); --*this; return temp; }

   /*!Safe bool conversion operator. Never throws.*/
   operator unspecified_bool_type() const  
      {  return base_t::is_null()? 0 : &self_t::unspecified_bool_type_func;   }

   /*!Not operator. Not needed in theory, but improves portability. 
      Never throws.*/
   bool operator! () const
      {  return base_t::is_null();   }

   /*!Swaps two intersegment_ptr-s. More efficient than std::swap.
      Never throws.*/
   void swap(intersegment_ptr &other)
      {  base_t::swap(other);   }

   /*!Compares the equality of two intersegment_ptr-s.
      Never throws.*/
   bool equal(const intersegment_ptr &other) const
      {  return base_t::equal(other);   }

   /*!Returns true if *this is less than other.
      This only works with two basic_intersegment_ptr pointing
      to the same segment group. Otherwise undefined. Never throws*/
   bool less(const intersegment_ptr &other) const
      {  return base_t::less(other);   }

   /*!Calculates the distance between two intersegment_ptr-s.
      This only works with two basic_intersegment_ptr pointing
      to the same segment. Otherwise undefined*/
   bool diff(const intersegment_ptr &other) const
      {  return base_t::less(other);   }
};

/*!intersegment_ptr<T1> == intersegment_ptr<T2, PT>. Never throws.*/
template<class T1, class T2, class PT>
inline bool operator== (const intersegment_ptr<T1, PT> &pt1, 
                        const intersegment_ptr<T2, PT> &pt2)
{  return pt1.equal(pt2);  }

/*!intersegment_ptr<T1> != intersegment_ptr<T2, PT>. Never throws.*/
template<class T1, class T2, class PT>
inline bool operator!= (const intersegment_ptr<T1, PT> &pt1, 
                        const intersegment_ptr<T2, PT> &pt2)
{  return !pt1.equal(pt2);  }

/*!intersegment_ptr<T1> < intersegment_ptr<T2, PT>. Never throws.*/
template<class T1, class T2, class PT>
inline bool operator< (const intersegment_ptr<T1, PT> &pt1, 
                       const intersegment_ptr<T2, PT> &pt2)
{  return pt1.less(pt2);   }

/*!intersegment_ptr<T1> <= intersegment_ptr<T2, PT>. Never throws.*/
template<class T1, class T2, class PT>
inline bool operator<= (const intersegment_ptr<T1, PT> &pt1, 
                        const intersegment_ptr<T2, PT> &pt2)
{  return !(pt1 > pt2);  }

/*!intersegment_ptr<T1> > intersegment_ptr<T2, PT>. Never throws.*/
template<class T1, class T2, class PT>
inline bool operator> (const intersegment_ptr<T1, PT> &pt1, 
                       const intersegment_ptr<T2, PT> &pt2)
{  return (pt2 < pt1);  }

/*!intersegment_ptr<T1> >= intersegment_ptr<T2, PT>. Never throws.*/
template<class T1, class T2, class PT>
inline bool operator>= (const intersegment_ptr<T1, PT> &pt1, 
                        const intersegment_ptr<T2, PT> &pt2)
{  return !(pt1 < pt2);  }

/*!operator<< */
template<class E, class T, class U, class PT> 
inline std::basic_ostream<E, T> & operator<< 
   (std::basic_ostream<E, T> & os, const intersegment_ptr<U, PT> & p)
{  return os << p.get();   }

/*!operator>> */
template<class E, class T, class U, class PT> 
inline std::basic_istream<E, T> & operator>> 
   (std::basic_istream<E, T> & os, intersegment_ptr<U, PT> & p)
{  U * tmp; return os >> tmp; p = tmp;   }

/*!std::ptrdiff_t + intersegment_ptr. 
   The result is another pointer of the same segment */
template<class T, class PT>
inline intersegment_ptr<T, PT> operator+(std::ptrdiff_t diff, intersegment_ptr<T, PT>& right)
{  return right + diff;  }

/*!intersegment_ptr - intersegment_ptr. 
   This only works with two intersegment_ptr-s that point to the
   same segment*/
template <class T, class T2, class PT>
inline std::ptrdiff_t operator- (const intersegment_ptr<T, PT> &pt, 
                          const intersegment_ptr<T2,PT> &pt2)
   {  return pt.diff(pt2);  }

/*! swap specialization */
template<class T, class PT>
inline void swap (boost::shmem::intersegment_ptr<T, PT> &pt, 
                  boost::shmem::intersegment_ptr<T, PT> &pt2)
{  pt.swap(pt2);  }

/*!Trait class to detect if an smart pointer has 
   multi-segment addressing capabilities.*/
template <class T, class PT>
struct is_multisegment_ptr
   <boost::shmem::intersegment_ptr<T, PT> >
{
   enum {   value = true   };
};

}  //namespace shmem {

/*!get_pointer() enables boost::mem_fn to recognize intersegment_ptr. 
   Never throws.*/
template<class T, class PT>
inline T * get_pointer(boost::shmem::intersegment_ptr<T, PT> const & p)
   {  return p.get();   }

/*!Simulation of static_cast between pointers. Never throws.*/
template<class T, class U, class PT> 
inline boost::shmem::intersegment_ptr<T, PT> static_pointer_cast(const boost::shmem::intersegment_ptr<U, PT> &r)
   {  return boost::shmem::intersegment_ptr<T, PT>(r, boost::shmem::detail::static_cast_tag());  }

/*!Simulation of const_cast between pointers. Never throws.*/
template<class T, class U, class PT> 
inline boost::shmem::intersegment_ptr<T, PT> const_pointer_cast(const boost::shmem::intersegment_ptr<U, PT> &r)
   {  return boost::shmem::intersegment_ptr<T, PT>(r, boost::shmem::detail::const_cast_tag());  }

/*!Simulation of dynamic_cast between pointers. Never throws.*/
template<class T, class U, class PT> 
inline boost::shmem::intersegment_ptr<T, PT> dynamic_pointer_cast(const boost::shmem::intersegment_ptr<U, PT> &r)
   {  return boost::shmem::intersegment_ptr<T, PT>(r, boost::shmem::detail::dynamic_cast_tag());  }

/*!Simulation of reinterpret_cast between pointers. Never throws.*/
template<class T, class U, class PT> 
inline boost::shmem::intersegment_ptr<T, PT> reinterpret_pointer_cast(const boost::shmem::intersegment_ptr<U, PT> &r)
   {  return boost::shmem::intersegment_ptr<T, PT>(r, boost::shmem::detail::reinterpret_cast_tag());  }

/*!has_trivial_constructor<> == true_type specialization for optimizations*/
template <class T, class PT>
struct has_trivial_constructor
   < boost::shmem::intersegment_ptr<T, PT> > 
   : public true_type{};

/*!has_trivial_destructor<> == true_type specialization for optimizations*/
template <class T, class PT>
struct has_trivial_destructor
   < boost::shmem::intersegment_ptr<T, PT> > 
   : public true_type{};

}  //namespace boost {

#include <boost/shmem/detail/config_end.hpp>

#endif //#ifndef BOOST_INTERSEGMENT_PTR_HPP


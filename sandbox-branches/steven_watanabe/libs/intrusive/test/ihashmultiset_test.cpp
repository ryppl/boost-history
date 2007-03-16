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

#include <boost/intrusive/ihashset.hpp>
#include <boost/intrusive/detail/pointer_to_other.hpp>
#include "itestvalue.hpp"
#include "smart_ptr.hpp"

#include <vector>

// Boost.Test
#include "boost/test/included/test_exec_monitor.hpp"

using namespace boost::intrusive;

static const std::size_t BucketSize = 11;

template<class ValueTraits>
struct test_hashmultiset
{
   typedef typename ValueTraits::value_type value_type;
   static void test_all (std::vector<value_type>& values);
   static void test_sort(std::vector<value_type>& values);
   static void test_insert(std::vector<value_type>& values);
   static void test_swap(std::vector<value_type>& values);
   static void test_rehash(std::vector<value_type>& values);
   static void test_find(std::vector<value_type>& values);
   static void test_impl();
};

template<class ValueTraits>
void test_hashmultiset<ValueTraits>::test_all (std::vector<typename ValueTraits::value_type>& values)
{
   test_sort(values);
   test_insert(values);
   test_swap(values);
   test_rehash(values);
   test_find(values);
   test_impl();
}

//test case due to an error in tree implementation:
template<class ValueTraits>
void test_hashmultiset<ValueTraits>::test_impl()
{
   typedef typename ValueTraits::value_type testvalue_t;
   std::vector<testvalue_t> values (5);
   for (int i = 0; i < 5; ++i)
      values[i].value_ = i; 
   typedef boost::intrusive::ihashmultiset
      <ValueTraits
      ,boost::hash<typename ValueTraits::value_type>
      ,std::equal_to<typename ValueTraits::value_type>
      ,ValueTraits::value_type::constant_time_size, std::size_t 
      > hashmultiset_type;
   typename hashmultiset_type::bucket_type buckets [BucketSize];
   hashmultiset_type testset(buckets, BucketSize);

   for (int i = 0; i < 5; ++i)
      testset.insert (values[i]);

   testset.erase (testset.current (values[0]));
   testset.erase (testset.current (values[1]));
   testset.insert (values[1]);
     
   testset.erase (testset.current (values[2]));
   testset.erase (testset.current (values[3]));
}

//test: constructor, iterator, clear, reverse_iterator, front, back, size:
template<class ValueTraits>
void test_hashmultiset<ValueTraits>::test_sort(std::vector<typename ValueTraits::value_type>& values)
{
   typedef typename ValueTraits::value_type testvalue_t;
   boost::test_tools::output_test_stream test_seq;

   typedef boost::intrusive::ihashmultiset
      <ValueTraits
      ,boost::hash<typename ValueTraits::value_type>
      ,std::equal_to<typename ValueTraits::value_type>
      ,ValueTraits::value_type::constant_time_size, std::size_t 
      > hashmultiset_type;
   typename hashmultiset_type::bucket_type buckets [BucketSize];
   hashmultiset_type testset1(buckets, BucketSize, values.begin(), values.end());

   std::copy (testset1.begin(), testset1.end(), 
               std::ostream_iterator<testvalue_t> (test_seq));
   BOOST_CHECK (test_seq.is_equal ("122345"));
     
   testset1.clear();
   BOOST_CHECK (testset1.empty());
/*
   typedef boost::intrusive::ihashmultiset
      <ValueTraits
      ,even_odd
      ,ValueTraits::value_type::constant_time_size, std::size_t 
      > hashmultiset_type2;
   typename hashmultiset_type2::bucket_type buckets2 [BucketSize];
   hashmultiset_type2 testset2(buckets2, BucketSize, &values[0], &values[0] + 6);
   std::copy (testset2.rbegin(), testset2.rend(), 
               std::ostream_iterator<testvalue_t> (test_seq));
   BOOST_CHECK (test_seq.is_equal ("531422"));
   //  BOOST_CHECK (testset2.front().value_ == 2);
   BOOST_CHECK (testset2.begin()->value_ == 2);
   //  BOOST_CHECK (testset2.back().value_ == 5);
   BOOST_CHECK (testset2.rbegin()->value_ == 5);
*/
}  
  
//test: insert, const_iterator, const_reverse_iterator, erase, current:
template<class ValueTraits>
void test_hashmultiset<ValueTraits>::test_insert(std::vector<typename ValueTraits::value_type>& values)
{
   typedef typename ValueTraits::value_type testvalue_t;
   boost::test_tools::output_test_stream test_seq;

   typedef boost::intrusive::ihashmultiset
      <ValueTraits
      ,boost::hash<typename ValueTraits::value_type>
      ,std::equal_to<typename ValueTraits::value_type>
      ,ValueTraits::value_type::constant_time_size, std::size_t 
      > hashmultiset_type;
   typename hashmultiset_type::bucket_type buckets [BucketSize];
   hashmultiset_type testset(buckets, BucketSize);

   testset.insert(&values[0] + 2, &values[0] + 5);

   const hashmultiset_type& const_testset = testset;
   std::copy (const_testset.begin(), const_testset.end(), 
               std::ostream_iterator<testvalue_t> (test_seq));
   BOOST_CHECK (test_seq.is_equal ("145"));

   BOOST_DEDUCED_TYPENAME hashmultiset_type::iterator i = testset.begin();
   BOOST_CHECK (i->value_ == 1);

   i = testset.insert (values[0]);
   BOOST_CHECK (&*i == &values[0]);
     
//   std::copy (const_testset.rbegin(), const_testset.rend(), 
//               std::ostream_iterator<testvalue_t> (test_seq));
//   BOOST_CHECK (test_seq.is_equal ("5431"));

   i = testset.current (values[2]);
   BOOST_CHECK (&*i == &values[2]);
   testset.erase(i);

   //test post-increment:
   for (BOOST_DEDUCED_TYPENAME hashmultiset_type::const_iterator i = const_testset.begin(),
         e = const_testset.end(); i != e; i++)
      test_seq << *i;
   BOOST_CHECK (test_seq.is_equal ("135"));
}  

//test: insert (seq-version), swap, erase (seq-version), size:
template<class ValueTraits>
void test_hashmultiset<ValueTraits>::test_swap(std::vector<typename ValueTraits::value_type>& values)
{
   typedef typename ValueTraits::value_type testvalue_t;
   boost::test_tools::output_test_stream test_seq;
   typedef boost::intrusive::ihashmultiset
      <ValueTraits
      ,boost::hash<typename ValueTraits::value_type>
      ,std::equal_to<typename ValueTraits::value_type>
      ,ValueTraits::value_type::constant_time_size, std::size_t 
      > hashmultiset_type;
   typename hashmultiset_type::bucket_type buckets [BucketSize];
   hashmultiset_type testset1(buckets, BucketSize, &values[0], &values[0] + 2);

   typename hashmultiset_type::bucket_type buckets2 [BucketSize];
   hashmultiset_type testset2(buckets2, BucketSize);

   testset2.insert (&values[0] + 2, &values[0] + 6);
   testset1.swap (testset2);
   
   std::copy (testset1.begin(), testset1.end(), 
               std::ostream_iterator<testvalue_t> (test_seq));
   BOOST_CHECK (test_seq.is_equal ("1245"));
   //test post-increment:
   for (BOOST_DEDUCED_TYPENAME hashmultiset_type::iterator i = testset2.begin(),
         e = testset2.end(); i != e; i++)
      test_seq << *i;
   BOOST_CHECK (test_seq.is_equal ("23"));

   testset1.erase (testset1.current(values[5]), testset1.end());
   BOOST_CHECK (testset1.size() == 1);
   //  BOOST_CHECK (&testset1.front() == &values[3]);
   BOOST_CHECK (&*testset1.begin() == &values[3]);
}  

//test: insert (seq-version), rehash:
template<class ValueTraits>
void test_hashmultiset<ValueTraits>::test_rehash(std::vector<typename ValueTraits::value_type>& values)
{
   typedef typename ValueTraits::value_type testvalue_t;
   boost::test_tools::output_test_stream test_seq;

   typedef boost::intrusive::ihashmultiset
      <ValueTraits
      ,boost::hash<typename ValueTraits::value_type>
      ,std::equal_to<typename ValueTraits::value_type>
      ,ValueTraits::value_type::constant_time_size, std::size_t 
      > hashmultiset_type;

   typename hashmultiset_type::bucket_type buckets1 [BucketSize];
   hashmultiset_type testset1(buckets1, BucketSize, &values[0], &values[0] + 6);
   BOOST_CHECK (testset1.size() == 6);
   std::copy (testset1.begin(), testset1.end(), 
               std::ostream_iterator<testvalue_t> (test_seq));
   BOOST_CHECK (test_seq.is_equal ("122345"));

   typename hashmultiset_type::bucket_type buckets2 [2];
   testset1.rehash(buckets2, 2);
   BOOST_CHECK (testset1.size() == 6);
   std::copy (testset1.begin(), testset1.end(), 
               std::ostream_iterator<testvalue_t> (test_seq));
   BOOST_CHECK (test_seq.is_equal ("422531"));


   typename hashmultiset_type::bucket_type buckets3 [BucketSize*2];
   testset1.rehash(buckets3, BucketSize*2);
   BOOST_CHECK (testset1.size() == 6);
   std::copy (testset1.begin(), testset1.end(), 
               std::ostream_iterator<testvalue_t> (test_seq));
   BOOST_CHECK (test_seq.is_equal ("122345"));

   //Now rehash reducing the buckets
   testset1.rehash(buckets3, 2);
   BOOST_CHECK (testset1.size() == 6);
   std::copy (testset1.begin(), testset1.end(), 
               std::ostream_iterator<testvalue_t> (test_seq));
   BOOST_CHECK (test_seq.is_equal ("422531"));

   //Now rehash increasing the buckets
   testset1.rehash(buckets3, BucketSize*2);
   BOOST_CHECK (testset1.size() == 6);
   std::copy (testset1.begin(), testset1.end(), 
               std::ostream_iterator<testvalue_t> (test_seq));
   BOOST_CHECK (test_seq.is_equal ("122345"));
}  

//test: find, equal_range (lower_bound, upper_bound):
template<class ValueTraits>
void test_hashmultiset<ValueTraits>::test_find(std::vector<typename ValueTraits::value_type>& values)
{
   typedef typename ValueTraits::value_type testvalue_t;
   boost::test_tools::output_test_stream test_seq;
   typedef boost::intrusive::ihashmultiset
      <ValueTraits
      ,boost::hash<typename ValueTraits::value_type>
      ,std::equal_to<typename ValueTraits::value_type>
      ,ValueTraits::value_type::constant_time_size, std::size_t 
      > hashmultiset_type;
   typename hashmultiset_type::bucket_type buckets[BucketSize];
   hashmultiset_type testset(buckets, BucketSize, values.begin(), values.end());

   typedef BOOST_DEDUCED_TYPENAME hashmultiset_type::iterator iterator;

   testvalue_t cmp_val;
   cmp_val.value_ = 2;
   iterator i = testset.find (cmp_val);
   BOOST_CHECK (i->value_ == 2);
   BOOST_CHECK ((++i)->value_ == 2);
   std::pair<iterator,iterator> range = testset.equal_range (cmp_val);
     
   BOOST_CHECK (range.first->value_ == 2);
   BOOST_CHECK (range.second->value_ == 3);
   BOOST_CHECK (std::distance (range.first, range.second) == 2);

   cmp_val.value_ = 7;
   BOOST_CHECK (testset.find (cmp_val) == testset.end());
} 

template<class VoidPointer, bool constant_time_size>
class test_main_template
{
   public:
   int operator()()
   {
      typedef testvalue<VoidPointer, constant_time_size> testvalue_t;
      static const int random_init[6] = { 3, 2, 4, 1, 5, 2 };
      std::vector<testvalue<VoidPointer, constant_time_size> > data (6);
      for (int i = 0; i < 6; ++i)
         data[i].value_ = random_init[i]; 

      test_hashmultiset <typename testvalue_t::hashset_base_hook::template
         value_traits<testvalue_t> >::test_all(data);

      test_hashmultiset <typename testvalue_t::hashset_member_hook::template
            value_traits<&testvalue_t::hashset_node_> >::test_all(data);

/*
   test_hashmultiset <boost::intrusive::iset_ptr_to_member
      < testvalue_t        , typename testvalue_t::pointer
      , &testvalue_t::hashset_prev_, &testvalue_t::hashset_next_> >::test_all(data);
*/
      return 0;
   }
};

template<class VoidPointer>
class test_main_template<VoidPointer, false>
{
   public:
   int operator()()
   {
      typedef testvalue<VoidPointer, false> testvalue_t;
      static const int random_init[6] = { 3, 2, 4, 1, 5, 2 };
      std::vector<testvalue<VoidPointer, false> > data (6);
      for (int i = 0; i < 6; ++i)
         data[i].value_ = random_init[i]; 

      test_hashmultiset <typename testvalue_t::hashset_base_hook::template
         value_traits<testvalue_t> >::test_all(data);

      test_hashmultiset <typename testvalue_t::hashset_member_hook::template
            value_traits<&testvalue_t::hashset_node_> >::test_all(data);

   /*
      test_hashmultiset <boost::intrusive::iset_ptr_to_member
         < testvalue_t        , typename testvalue_t::pointer
         , &testvalue_t::hashset_prev_, &testvalue_t::hashset_next_> >::test_all(data);
   */

      test_hashmultiset <typename testvalue_t::hashset_auto_base_hook::template
         value_traits<testvalue_t> >::test_all(data);

      test_hashmultiset <typename testvalue_t::hashset_auto_member_hook::template
            value_traits<&testvalue_t::hashset_auto_node_> >::test_all(data);
      return 0;
   }
};

//Explicit instantiations of non-counted classes
template class boost::intrusive::ihashmultiset<hashset_base_raw>;
template class boost::intrusive::ihashmultiset<hashset_member_raw>;
template class boost::intrusive::ihashmultiset<hashset_auto_base_raw>;
template class boost::intrusive::ihashmultiset<hashset_auto_member_raw>;
//template class boost::intrusive::ihashmultiset<hashset_ptr_to_member_raw>;
template class boost::intrusive::ihashmultiset<hashset_base_smart>;
template class boost::intrusive::ihashmultiset<hashset_member_smart>;
template class boost::intrusive::ihashmultiset<hashset_auto_base_smart>;
template class boost::intrusive::ihashmultiset<hashset_auto_member_smart>;
//template class boost::intrusive::ihashmultiset<hashset_ptr_to_member_smart>;

//Explicit instantiation of counted classes
template class boost::intrusive::ihashmultiset
   <hashset_base_raw_t
   , boost::hash<hashset_base_raw_t::value_type>
   , std::equal_to<hashset_base_raw_t::value_type>, true>;
template class boost::intrusive::ihashmultiset
   <hashset_member_raw_t, 
boost::hash<hashset_base_raw_t::value_type>,
std::equal_to<hashset_member_raw_t::value_type>, true>;
//template class boost::intrusive::ihashmultiset
//   <hashset_ptr_to_member_raw_t, 
//boost::hash<hashset_base_raw_t::value_type>
//std::equal_to<hashset_ptr_to_member_raw_t::value_type>, true>;
template class boost::intrusive::ihashmultiset
   <hashset_base_smart_t, 
   boost::hash<hashset_base_smart_t::value_type>,
   std::equal_to<hashset_base_smart_t::value_type>, true>;
template class boost::intrusive::ihashmultiset
   <hashset_member_smart_t,
   boost::hash<hashset_member_smart_t::value_type>,
   std::equal_to<hashset_member_smart_t::value_type>, true>;
//template class boost::intrusive::ihashmultiset
//   <hashset_ptr_to_member_smart_t, 
//boost::hash<hashset_base_raw_t::value_type>
//std::equal_to<hashset_ptr_to_member_smart_t::value_type>, true>;

int test_main( int, char* [] ) 
{
   test_main_template<void*, false>()();
   test_main_template<boost::intrusive::smart_ptr<void>, false>()();
   test_main_template<void*, true>()();
   test_main_template<boost::intrusive::smart_ptr<void>, true>()();
   return 0;
}

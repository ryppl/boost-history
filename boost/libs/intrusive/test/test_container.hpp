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

#ifndef BOOST_INTRUSIVE_TEST_CONTAINER_HPP
#define BOOST_INTRUSIVE_TEST_CONTAINER_HPP

#include <boost/detail/lightweight_test.hpp>

namespace boost {
namespace intrusive {
namespace test {

template< class Container >
void test_container( Container & c )
{
   typedef typename Container::value_type       value_type;
   typedef typename Container::iterator         iterator;
   typedef typename Container::const_iterator   const_iterator;
   typedef typename Container::reference        reference;
   typedef typename Container::const_reference  const_reference;
   typedef typename Container::pointer          pointer;
   typedef typename Container::const_pointer    const_pointer;
   typedef typename Container::difference_type  difference_type;
   typedef typename Container::size_type        size_type;
   typedef typename Container::difference_type  difference_type;
   typedef typename Container::size_type        size_type;
   typedef typename Container::value_traits     value_traits;

   const size_type num_elem = c.size();
   BOOST_TEST( c.empty() == (num_elem == 0) );
   {
      iterator it(c.begin()), itend(c.end());
      size_type i;
      for(i = 0; i < num_elem; ++i){
         ++it;
      }
      BOOST_TEST( it == c.end() );
      BOOST_TEST( c.size() == i );
   }

   //Check iterator conversion
   BOOST_TEST( const_iterator(c.begin()) == c.cbegin() );
   {
      const_iterator it(c.cbegin()), itend(c.cend());
      size_type i;
      for(i = 0; i < num_elem; ++i){
         ++it;
      }
      BOOST_TEST( it == c.cend() );
      BOOST_TEST( c.size() == i );
   }
}


template< class Container, class Data >
void test_sequence_container(Container & c, Data & d)
{
   assert( d.size() > 2 );

   c.clear();

   BOOST_TEST( c.size() == 0 );
   BOOST_TEST( c.empty() );

   c.insert( c.begin(), *d.begin() );
   c.insert( c.end(), *(++d.begin()) );

   BOOST_TEST( c.size() == 2 );
   BOOST_TEST( !c.empty() );

   c.erase( c.begin() );

   BOOST_TEST( c.size() == 1 );

   c.insert( c.begin(), *(++++d.begin()) );

   c.erase( c.begin(), c.end() );

   BOOST_TEST( c.empty() );

   c.insert( c.begin(), *d.begin() );

   BOOST_TEST( c.size() == 1 );

   BOOST_TEST( c.begin() != c.end() );

   c.erase( c.begin() );

   c.assign(d.begin(), d.end());

   BOOST_TEST( c.size() == d.size() );

   c.clear();

   BOOST_TEST( c.size() == 0 );
   BOOST_TEST( c.empty() );
}

template< class Container, class Data >
void test_common_unordered_and_associative_container(Container & c, Data & d)
{
   typedef typename Container::size_type  size_type;

   assert( d.size() > 2 );

   c.clear();
   c.insert(d.begin(), d.end());

   for( typename Data::const_iterator di = d.begin(), de = d.end();
      di != de; ++di )
   {
      BOOST_TEST( c.find(*di) != c.end() );
   }

   typename Data::const_iterator da =   d.begin();
   typename Data::const_iterator db = ++d.begin();

   size_type old_size = c.size();

   c.erase(*da);

   BOOST_TEST( c.size() == old_size-1 );

   BOOST_TEST( c.count(*da) == 0 );
   BOOST_TEST( c.count(*db) != 0 );

   BOOST_TEST( c.find(*da) == c.end() );
   BOOST_TEST( c.find(*db) != c.end() );

   BOOST_TEST( c.equal_range(*db).first != c.end() );

   c.clear();

   BOOST_TEST( c.equal_range(*da).first == c.end() );
}

template< class Container, class Data >
void test_associative_container_invariants(Container & c, Data & d)
{
   typedef typename Container::const_iterator const_iterator;
   for( typename Data::const_iterator di = d.begin(), de = d.end();
      di != de; ++di)
   {
      const_iterator ci = c.find(*di);
      BOOST_TEST( ci != c.end() );
      BOOST_TEST( ! c.value_comp()(*ci, *di) );
      const_iterator cil = c.lower_bound(*di);
      const_iterator ciu = c.upper_bound(*di);
      std::pair<const_iterator, const_iterator> er = c.equal_range(*di);
      BOOST_TEST( cil == er.first );
      BOOST_TEST( ciu == er.second );
      if(ciu != c.end()){
         BOOST_TEST( c.value_comp()(*cil, *ciu) );
      }
      if(c.count(*di) > 1){
         const_iterator ci_next = cil; ++ci_next;
         for( ; ci_next != ciu; ++cil, ++ci_next){
            BOOST_TEST( !c.value_comp()(*ci_next, *cil) );
         }
      }
   }
}

template< class Container, class Data >
void test_associative_container(Container & c, Data & d)
{
   typedef typename Container::const_iterator const_iterator;
   assert( d.size() > 2 );

   c.clear();
   c.insert(d.begin(),d.end());

   test_associative_container_invariants(c, d);

   const Container & cr = c;

   test_associative_container_invariants(cr, d);
}

template< class Container, class Data >
void test_unordered_associative_container_invariants(Container & c, Data & d)
{
   typedef typename Container::size_type size_type;
   typedef typename Container::const_iterator const_iterator;

   for( typename Data::const_iterator di = d.begin(), de = d.end() ;
      di != de ; ++di ){
      const_iterator i = c.find(*di);
      size_type nb = c.bucket(*i);
      size_type bucket_elem = std::distance(c.begin(nb), c.end(nb));
      BOOST_TEST( bucket_elem ==  c.bucket_size(nb) );
      BOOST_TEST( &*c.local_iterator_to(*c.find(*di)) == &*i );
      std::pair<const_iterator, const_iterator> er = c.equal_range(*di);
      size_type cnt = std::distance(er.first, er.second);
      BOOST_TEST( cnt == c.count(*di));
      if(cnt > 1)
      for(const_iterator n = er.first, i = n++, e = er.second; n != e; ++i, ++n){
         BOOST_TEST( c.key_eq()(*i, *n) );
         BOOST_TEST( c.hash_function()(*i) == c.hash_function()(*n) );
      }
   }

   size_type blen = c.bucket_count();
   size_type total_objects = 0;
   for(size_type i = 0; i < blen; ++i){
      total_objects += c.bucket_size(i);
   }
   BOOST_TEST( total_objects ==  c.size() );
}

template< class Container, class Data >
void test_unordered_associative_container(Container & c, Data & d)
{
   c.clear();
   c.insert( d.begin(), d.end() );

   test_unordered_associative_container_invariants(c, d);

   const Container & cr = c;

   test_unordered_associative_container_invariants(cr, d);
}

template< class Container, class Data >
void test_unique_container(Container & c, Data & d)
{
   typedef typename Container::value_type value_type;
   c.clear();
   c.insert(d.begin(),d.end());
   typename Container::size_type old_size = c.size();
   value_type v(*d.begin());
   c.insert(v);
   BOOST_TEST( c.size() == old_size );
   c.clear();
}

template< class Container, class Data >
void test_non_unique_container(Container & c, Data & d)
{
   typedef typename Container::value_type value_type;
   c.clear();
   c.insert(d.begin(),d.end());
   typename Container::size_type old_size = c.size();
   value_type v(*d.begin());
   c.insert(v);
   BOOST_TEST( c.size() == (old_size+1) );
   c.clear();
}

}}}

#endif   //#ifndef BOOST_INTRUSIVE_TEST_CONTAINER_HPP

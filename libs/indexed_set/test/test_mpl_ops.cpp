/* Boost.IndexedSet test for MPL operations.
 *
 * Copyright Joaquín M López Muñoz 2003-2004. Use, modification, and distribution
 * are subject to the Boost Software License, Version 1.0. (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * See http://www.boost.org/libs/indexed_set for library home page.
 */

#include "test_mpl_ops.hpp"

#include <boost/config.hpp> /* keep it first to prevent some nasty warnings in MSVC */
#include "pre_indexed_set.hpp"
#include <boost/indexed_set.hpp>
#include <boost/indexed_set/sequenced_index.hpp>
#include <boost/mpl/at.hpp>
#include <boost/mpl/list.hpp>

using namespace boost::indexed_sets;

void test_mpl_ops()
{
  typedef indexed_set<
    int,
    index_list<
      unique<identity<int> >,
      non_unique<identity<int> >
     >
  >                           indexed_t1;

  BOOST_STATIC_ASSERT((boost::is_same<
    boost::mpl::at_c<indexed_t1::index_specifier_type_list,0>::type,
    unique<identity<int> > >::value));
  BOOST_STATIC_ASSERT((boost::is_same<
    boost::mpl::at_c<indexed_t1::index_specifier_type_list,1>::type,
    non_unique<identity<int> > >::value));
  BOOST_STATIC_ASSERT((boost::is_same<
    boost::mpl::at_c<indexed_t1::index_type_list,0>::type,
    nth_index_type<indexed_t1,0>::type>::value));
  BOOST_STATIC_ASSERT((boost::is_same<
    boost::mpl::at_c<indexed_t1::index_type_list,1>::type,
    nth_index_type<indexed_t1,1>::type>::value));
  BOOST_STATIC_ASSERT((boost::is_same<
    boost::mpl::at_c<indexed_t1::iterator_type_list,0>::type,
    nth_iterator_type<indexed_t1,0>::type>::value));
  BOOST_STATIC_ASSERT((boost::is_same<
    boost::mpl::at_c<indexed_t1::iterator_type_list,1>::type,
    nth_iterator_type<indexed_t1,1>::type>::value));
  BOOST_STATIC_ASSERT((boost::is_same<
    boost::mpl::at_c<indexed_t1::const_iterator_type_list,0>::type,
    nth_const_iterator_type<indexed_t1,0>::type>::value));
  BOOST_STATIC_ASSERT((boost::is_same<
    boost::mpl::at_c<indexed_t1::const_iterator_type_list,1>::type,
    nth_const_iterator_type<indexed_t1,1>::type>::value));

  typedef boost::mpl::push_front<
    indexed_t1::index_specifier_type_list,
    sequenced<>
  >::type                     index_list_t;

  typedef indexed_set<
    int,
    mpl_index_list<
      index_list_t
    >
  >                           indexed_t2;

  BOOST_STATIC_ASSERT((boost::is_same<
    boost::mpl::at_c<indexed_t2::index_specifier_type_list,0>::type,
    sequenced<> >::value));
  BOOST_STATIC_ASSERT((boost::is_same<
    boost::mpl::at_c<indexed_t2::index_specifier_type_list,1>::type,
    boost::mpl::at_c<indexed_t1::index_specifier_type_list,0>::type>::value));
  BOOST_STATIC_ASSERT((boost::is_same<
    boost::mpl::at_c<indexed_t2::index_specifier_type_list,2>::type,
    boost::mpl::at_c<indexed_t1::index_specifier_type_list,1>::type>::value));


  typedef indexed_set<
    int,
    mpl_index_list<
      boost::mpl::list<
        unique<identity<int> >,
        non_unique<identity<int> >
      >
    >
  >                           indexed_t3;

  BOOST_STATIC_ASSERT((boost::is_same<
    boost::mpl::at_c<indexed_t3::index_specifier_type_list,0>::type,
    boost::mpl::at_c<indexed_t1::index_specifier_type_list,0>::type>::value));
  BOOST_STATIC_ASSERT((boost::is_same<
    boost::mpl::at_c<indexed_t3::index_specifier_type_list,1>::type,
    boost::mpl::at_c<indexed_t1::index_specifier_type_list,1>::type>::value));
}

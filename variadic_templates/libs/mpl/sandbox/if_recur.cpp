
// Copyright Larry Evans 2009
//
// Distributed under the Boost Software License, Version 1.0. 
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/mpl for documentation.

// $Id$
// $Date: 2009/04/19 14:39:38 $
// $Revision: 1.35 $

#include <boost/mpl/if_recur.hpp>

#include <boost/mpl/begin_end.hpp>
#include <boost/mpl/range_c.hpp>
#include <boost/mpl/int.hpp>
#include <boost/mpl/arg.hpp>
#include <boost/mpl/apply.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/push_front.hpp>
#include <boost/mpl/next.hpp>
#include <boost/mpl/deref.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/vector_c.hpp>
#include <boost/mpl/equal.hpp>
#include <boost/mpl/less.hpp>
#include <boost/type_traits/is_same.hpp>

#include <boost/mpl/aux_/test.hpp>

using namespace mpl;

  template
  < class Iter
  , class Container
  >
  struct
iter_container
{
    typedef iter_container type;
    typedef Iter iter;
    typedef Container container;
};

  template
  < class IterContainer
  >
  struct
down_next
{
        typedef
      iter_container
      < typename next<typename IterContainer::iter>::type
      , typename IterContainer::container
      >   
    type
    ;
};
  template
  < class IterContainer
  >
  struct
down_next_push
{
        typedef typename
      IterContainer::iter
    iter
    ;
        typedef
      iter_container
      < typename next<iter>::type
      , typename push_front
        < typename IterContainer::container
        , typename deref<iter>::type
        >::type
      >   
    type
    ;
};

  template
  < class StateNow
  , class ContainerUp
  >
  struct
up_push
{
        typedef
      typename push_front
      < ContainerUp
      , typename deref<typename StateNow::iter>::type
      >::type
    type
    ;
};
  template
  < class StateNow
  , class IterEnd
  >
  struct
not_end
/**@brief
 *  not at end iterator?
 */
: not_
  < is_same
    < typename StateNow::iter
    , IterEnd
    >
  >
{
};
  template
  < class IterContainer
  >
  struct
else_container
/**@brief
 *  just return the container, since
 *  on the way up the recursion, the
 *  up_push operation, if present,
 *  will just need the container and
 *  has no need for the iterator.
 */
{
      typedef
    typename IterContainer::container
  type
  ;
};    

    typedef
  range_c
  < int
  , 9
  , 13
  >
numbers
;
    typedef
  begin<numbers>::type
beg_numb
;  
    typedef
  end<numbers>::type
end_numb
;
    typedef
  iter_container<beg_numb,vector<> >
beg_vector
;  
#if 1
      template
      < class StateNow
      , class StateUp
      >
    struct if_recur_up_default
    {
            typedef
          StateUp
        type
        ;
    };
    
    typedef
  if_recur
  < identity<beg_vector>
  , not_end<arg<1>,end_numb>
  , down_next_push<arg<1> >
  , if_recur_up_default<arg<1>,arg<2> >
  , else_container<arg<1> >
  >::type
result_down_push
;
    typedef
  vector_c<int,12,11,10,9>
expected_down_push
;
BOOST_MPL_ASSERT((equal<result_down_push,expected_down_push>));
#endif 
#if 1
    typedef
  if_recur
  < identity<beg_vector>
  , not_end<arg<1>,end_numb>
  , down_next_push<arg<1> >
  , up_push<arg<1>,arg<2> >
  , else_container<arg<1> >
  >::type
result_down_up_push
;
    typedef
  vector_c<int,9,10,11,12,12,11,10,9>
expected_down_up_push
;
BOOST_MPL_ASSERT((equal<result_down_up_push,expected_down_up_push>));
#endif 
#if 1
    typedef
  if_recur
  < identity<beg_vector>
  , not_end<arg<1>,end_numb>
  , down_next<arg<1> >
  , up_push<arg<1>,arg<2> >
  , else_container<arg<1> >
  >::type
result_up_push
;
    typedef
  vector_c<int,9,10,11,12>
expected_up_push
;
BOOST_MPL_ASSERT((equal<result_up_push,expected_up_push>));
#endif 
#if 1
  template
  < class StateNow
  , class IterEnd
  , class LessCriteria
  >
  struct
fwdpred_short_circuit
: and_
  < not_end<StateNow,IterEnd>
  , less
    < typename deref<typename StateNow::iter>::type
    , LessCriteria
    >
  >
{
};
    typedef
  begin<expected_down_up_push>::type
beg_short_circuit
;
    typedef
  end<expected_down_up_push>::type
end_short_circuit
;
    typedef
  iter_container<beg_short_circuit,vector<> >
beg_vector_short_circuit
;  
    typedef
  if_recur
  < identity<beg_vector_short_circuit>
  , fwdpred_short_circuit
    < arg<1>
    , end_short_circuit
    , int_<11>
    >
  , down_next<arg<1> >
  , up_push<arg<1>,arg<2> >
  , else_container<arg<1> >
  >::type
result_short_circuit
;
    typedef
  vector_c<int,9,10>
expected_short_circuit
;
BOOST_MPL_ASSERT((equal<result_short_circuit,expected_short_circuit>));
#endif 

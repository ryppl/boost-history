//  boost/rank_list/detail/forward_decl.hpp

//  Copyright (c) Universidad de Alcala 2006
//  Use, modification, and distribution is subject to the Boost Software
//  License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

//  See library home page at http://www.boost.org/libs/rank_list

/*
  detail/forward_decl.hpp
  -----------------------

  Forward declarations
  
  See accompanying documentation for more details.

                                    November, 2006
                                    Martin Knoblauch Revuelta
                                    Universidad de Alcala (Spain)
                                    martin at aut.uah.es
*/

#ifndef _BOOST_RANK_LIST_FORWARD_DECL_HPP_
#define _BOOST_RANK_LIST_FORWARD_DECL_HPP_

#ifndef _BOOST_RANK_LIST_HPP_
#error "Don't include this file. Include rank_list.hpp instead."
#endif

namespace boost
{
  namespace rank_lists
  {

    template<class T, class A,
             class W, class P>              // The only visible class
    class rank_list;                        // is rank_list<T,A,W,P>

    namespace detail
    {

      template<class T, class A,
               class W, class P>            // Links and counters
      class rank_list_node_tree_fields;     // of a tree node

      template<class T, class A,
               class W, class P>            // A tree node, including
      class rank_list_node;                 // its payload value_type

      template<class T, class A,
               class W, class P>            // A list of nodes to
      class rollback_list;                  // complete or delete

      template<class T, class A,
               class W, class P,
               class Ref, class Ptr>
      class rank_list_iterator;             // Normal iterator

      template<class T, class A,
               class W, class P,
               class Ref, class Ptr>
      class rank_list_rev_iter;             // Reverse iterator

      class allocator_returned_null;
      class index_out_of_bounds;
      class invalid_op_with_end;            // Exceptions
      class lesser_and_greater;

      template<class Ptr>
      class null_data_provider;
      template<class Ptr, class IT>  // Functors used internally
      class iter_data_provider;      // for encapsulating different
      template<class Ptr, class IT>  // sources of value_type objects
      class range_data_provider;     // under the same interface
      template<class Ptr>
      class copy_data_provider;

      class empty_number;     // Default parameter for W (no NPSV)
                            // and P (no stable_sort)
                            
    }  // namespace detail
  }  // namespace rank_lists
}  // namespace boost

#endif


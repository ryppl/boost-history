#ifndef LIBS_POLICY_PTR_TEST_SP_TYPES_STD_HPP
#define LIBS_POLICY_PTR_TEST_SP_TYPES_STD_HPP
#include "boost/policy_ptr/std_ptr.hpp"
#include "boost/policy_ptr/policy/std.hpp"
#define SHARED_GRAPH
#ifdef SHARED_GRAPH
//#define USE_CONTAINER_INTERN
#ifndef USE_CONTAINER_INTERN
#include "boost/fields_visitor/container_extern/single.hpp"
#else
#include "boost/fields_visitor/container_intern/vector.hpp"
#include "boost/fields_visitor/container_intern/field_iterator_of_container_intern_single.hpp"
#endif
#include "boost/policy_ptr/detail/shared_count_hook_sp_counted_accepting.hpp"
#include "boost/policy_ptr/detail/shared_count_hook_sp_counted_tagged.hpp"
#endif
#include <vector>
#include <boost/mpl/vector.hpp>
#include "source_auto_ptr.hpp"

namespace boost
{

    enum
  sp_std_numerals
  { std_shared_dag
#ifdef SHARED_GRAPH  
  , std_shared_graph_accepting
  , std_shared_graph_tagged
#endif
  , num_sp
  };
  
    template
    < sp_std_numerals
    >
  struct sp_types_std
  ;
  
    template
    <
    >
  struct sp_types_std
    < std_shared_dag
    >
  {
          static
        sp_std_numerals const
      our_std_numeral
        =std_shared_dag
      ;
        template<class Referent>
        struct
      source
      {
              typedef 
            source_auto_ptr<Referent>
          type
          ;
      };
      
        template<class Referent>
        struct
      scalar
      {
              typedef typename
            std_ptr
              < Referent
              >
              ::shared
          type
          ;
      };
      
        template<class Value>
        struct
      container
      {
              typedef 
            std::vector
            < Value
            >
          type
          ;
      };
      
  };//end sp_types_std<std_shared_dag> specialization
  
#ifdef SHARED_GRAPH  
    template
    <
    >
  struct sp_types_std
    < std_shared_graph_accepting
    >
  {
          static
        sp_std_numerals const
      our_std_numeral
        =std_shared_graph_accepting
      ;
        template<class Referent>
        struct
      source
      {
              typedef 
            source_auto_ptr<Referent>
          type
          ;
      };
      
        template<class Referent>
        struct
      scalar
      {
              typedef typename
            std_ptr
              < Referent
              >
              ::shared_accepting
          type
          ;
      };
      
        template<class Value>
        struct
      container
      {
              typedef 
        #ifndef USE_CONTAINER_INTERN
            typename fields_visitor::container_extern::single
            < std::vector
            , Value
            , mpl::vector<policy_ptr::detail::shared_count_visitor_abs>
            >
        #else
            typename fields_visitor::container_intern::vector
            < Value
            , fields_visitor::container_intern::allocator
              < Value
              , mpl::vector<policy_ptr::detail::shared_count_visitor_abs>
              >
            >
        #endif      
          type
          ;
      };
      
  };//end sp_types_std<std_shared_graph_accepting> specialization
  
    template
    <
    >
  struct sp_types_std
    < std_shared_graph_tagged
    >
  {
          static
        sp_std_numerals const
      our_std_numeral
        =std_shared_graph_tagged
      ;
        template<class Referent>
        struct
      source
      {
              typedef 
            source_auto_ptr<Referent>
          type
          ;
      };
      
        template<class Referent>
        struct
      scalar
      {
              typedef typename
            std_ptr
              < Referent
              >
              ::shared_tagged
          type
          ;
      };
      
        template<class Value>
        struct
      container
      {
              typedef 
            std::vector
            < Value
            >
          type
          ;
      };
      
  };//end sp_types_std<std_shared_graph_tagged> specialization
  
#endif //SHARED_GRAPH
}//exit boost namespace    
//----------------------------------------
//ChangeLog:
//  2005-06-15: Larry Evans
//    WHAT:
//      Renamed include files from:
//        shared_count_sp_counted_*.hpp
//      to:
//        shared_count_hook_sp_counted_*.hpp
//    WHY:
//      Part of changes described in ChangeLog of:
//        boost/policy_ptr/detail/shared_count.hpp
#endif //include guard

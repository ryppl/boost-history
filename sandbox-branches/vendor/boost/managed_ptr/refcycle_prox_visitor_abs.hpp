//prox_visitor(arg to prox_children) to handle just refcycle_counted_curry_prox_visitor<this_type>::owner_void
#ifndef BOOST_MANAGED_PTR_REFCYCLE_PROX_VISITOR_ABS_HPP_LJE20031122
#define BOOST_MANAGED_PTR_REFCYCLE_PROX_VISITOR_ABS_HPP_LJE20031122
  
#include "boost/managed_ptr/refcycle_counted_curry_prox_visitor.fpp"
#include "boost/managed_ptr/prox_children.hpp"
#include "boost/managed_ptr/smart_ptr.hpp"
namespace boost{namespace managed_ptr{

  struct 
refcycle_prox_visitor_abs
//Purpose:
//  An instance of ProxVisitor arg to
//  prox_children<...> template.
//  This instance only handles 1 type of
//  ownership policy:
//    refcycle_counted_curry_prox_visitor<this_type>::owner_void
{
 public:

        typedef 
      refcycle_prox_visitor_abs 
    this_type
    ;
        typedef
      refcycle_counted_curry_prox_visitor<this_type>::owner_void
    void_owner_type
    ;
        typedef
      prox_children<this_type>::prox_iterator_acceptor_abs
    prox_iter_acc_type
    ;
        typedef 
      prox_children<this_type>::detail_iterators::prox_iterator_con_described 
    prox_iter_desc_type
    ;
    unsigned my_max_depth;
    unsigned my_cur_depth;
    
    refcycle_prox_visitor_abs
    ( unsigned a_max_depth=20
    )
    //Purpose:
    //  A smart_ptr graph traverser.
    //  Subclasses will perform specific actions
    //  on the visited smart_ptr's.
    ;
      void 
    accept_each
    ( prox_iter_acc_type& a_iter
    )
    //Purpose:
    //  accept *this for each element in a_iter
    ;
    
    template
      < typename Value
      >
      void 
    visit_prox
    ( Value& a_val
    )
    //Purpose:
    //  Traverses the children of a_val 
    //  until a owner<ReferentPtr>& is visited by one of the visit_owner template methods
    ;
    
#include "boost/managed_ptr/BOOST_SMART_POINTER_PARAMS_POLICIES_prefix.hpp"
    template
      < typename Referent
      , BOOST_SMART_POINTER_PARAMETERS
      >
      void 
    visit_prox
    ( smart_ptr
        < Referent
        , BOOST_SMART_POINTER_POLICIES
        >& a_sp
    )
    {
            typedef
          smart_ptr
            < Referent
            , BOOST_SMART_POINTER_POLICIES
            >
        smart_ptr_type;
      #ifdef TRACE_SCOPE_HPP
        utility::trace_scope ts("visit_prox(smart_ptr<>)");
      #endif
        visit_prox(static_cast<typename smart_ptr_type::ownership_policy&>(a_sp)); 
    }
#include "boost/managed_ptr/BOOST_SMART_POINTER_PARAMS_POLICIES_suffix.hpp"

    template
      < typename Referent
      >
      void 
    visit_prox
    ( refcycle_counted_curry_prox_visitor<this_type>::owner<Referent*>& a_typed_owner
    )
    {
      #ifdef TRACE_SCOPE_HPP
        utility::trace_scope ts("visit_prox(curry_prox)");
      #endif
        visit_prox(static_cast<void_owner_type&>(a_typed_owner));
    }

      void
    visit_prox
    ( void_owner_type& a_void_owner
    )
    //Purpose:
    //  Simply forwards the argument to visit_children method with
    //  the iterator over children of a_void_owner.
    ;
    
        virtual 
      void 
    visit_children
    ( void_owner_type& a_void_owner
    , prox_iter_acc_type& l_iter 
      //^iterator over the children of a_esp
    )
    //Purpose:
    //  A work-around for not having templated virtual functions.
    //  Subclasses perform specific actions on the a_esp and it's
    //  children, which is available via l_iter.
    =0
    ;
      
}
;//end refcycle_prox_visitor_abs

    namespace rm_nondeduced
    //Purpose:
    //  These classes overcome a restriction on specialization
    //  of nested template classes in nondeduced contexts.
    //Reference:
    //  1) c++ standard 14.8.2.4 "Deducing template arguments from a type"
    //     paragraph 4.
    //  2) http://groups.google.com/groups?hl=en&lr=&ie=UTF-8&selm=10422s0n4gtdaac%40corp.supernews.com
    {

      template<typename Referent>
      struct nester_of
      < refcycle_counted_curry_prox_visitor<refcycle_prox_visitor_abs>::owner<Referent*>
      >
      {
          typedef refcycle_counted_curry_prox_visitor<refcycle_prox_visitor_abs> type;
      };
      
    } //exit rm_nondeduced namespace
}}//exit boost::managed_ptr namespace
#endif

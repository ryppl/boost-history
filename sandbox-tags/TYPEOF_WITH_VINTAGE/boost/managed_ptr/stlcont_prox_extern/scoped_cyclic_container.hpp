#ifndef BOOST_MANAGED_PTR_STLCONT_PROX_EXTERN_VECTOR_HPP_LJE20040108
#define BOOST_MANAGED_PTR_STLCONT_PROX_EXTERN_VECTOR_HPP_LJE20040108
#include "boost/managed_ptr/prox_children.hpp"
#include "boost/managed_ptr/refcycle_prox_visitor_abs.hpp"
#include <memory>
//#define SCOPED_CYCLIC_TRACE
#ifdef SCOPED_CYCLIC_TRACE
#include "boost/utility/object_tracked.hpp"
#endif
namespace boost
{
namespace managed_ptr
{
namespace stlcont_prox_extern
{
template
  < template
      < typename //Element
      , typename //Allocator with default value
      >
    class Container
  , typename Element
  , typename ProxVisitor=refcycle_prox_visitor_abs
  , typename Allocator=std::allocator<Element>
  >
  class
scoped_cyclic_container
#ifdef SCOPED_CYCLIC_TRACE
  : public utility::object_tracked
  , public Container<Element,Allocator>
#else
  : public Container<Element,Allocator>
#endif
  , private prox_children<ProxVisitor>::builder_materials::make_recorder
  {
  public:
        typedef
      Element
    element_type
      ;
        typedef
      scoped_cyclic_container
        < Container
        , element_type
        , ProxVisitor
        , Allocator
        >
    my_type
      ;
        typedef
      Container<element_type, Allocator>
    container_type
      ;
        typedef
      typename prox_children<ProxVisitor>::builder_materials::make_recorder
    recorder_type
      ;
    ~scoped_cyclic_container(void)
      {
      #ifdef SCOPED_CYCLIC_TRACE
      ; mout()<<"scoped_cyclic_container::TOR-:id="<<id_get()<<":"<<"\n"
      #endif
      ;}
    scoped_cyclic_container(void)
      : recorder_type(this)
      {
      #ifdef SCOPED_CYCLIC_TRACE
      ; utility::obj_id*oid=this
      ; mout()<<"scoped_cyclic_container::TOR+(void)"<<":oid="<<oid<<":id="<<id_get()<<"\n"
      #endif
      ;}
    scoped_cyclic_container(unsigned const a_size)
      : container_type(a_size)
      , recorder_type(this)
      {
      #ifdef SCOPED_CYCLIC_TRACE
      ; utility::obj_id*oid=this
      ; mout()<<"scoped_cyclic_container::TOR+(unsigned)"<<":oid="<<oid<<":id="<<id_get()<<"\n"
      #endif
      ;}
  }
  ;//end scoped_cyclic_container<ProxVisitor, Container, Element> general template
  
}//exit stlcont_prox_extern namespace
template
  < typename ProxVisitor
  >//for prox_children
    template
      < template
          < typename //Element
          , typename //Allocator
          >
        class Container
      , typename Element
      , typename Allocator
      >//for prox_iterator_of
      struct 
prox_children
  < ProxVisitor
  >
  ::      
  detail_iterators
    ::
    prox_iterator_of
      < stlcont_prox_extern::scoped_cyclic_container
        < Container
        , Element
        , ProxVisitor
        , Allocator
        >
      >
      {
      public:
            typedef
          ProxVisitor
        prox_visitor
          ;
            typedef
          iterator_default::range
          < typename Container
            < Element
            , Allocator
            >::iterator
          >
        iter_type
          ;    
            typedef        
          stlcont_prox_extern::scoped_cyclic_container
          < Container
          , Element
          , prox_visitor
          , Allocator
          >
        subj_type
          ;
        prox_iterator_of
          ( subj_type* a_subj
          )
          : m_iter(a_subj->begin(),a_subj->end())
          {
            #ifdef TRACE_SCOPE_HPP
              utility::trace_scope ts("prox_iterator_of<scoped_cyclic_container>");
              mout()<<":subj_id="<<a_subj->id_get()<<")\n";
            #endif
          }
          void
        accept(prox_visitor& a_visitor)const
          { 
          ; Element& a_elem = m_iter.deref()
          ; a_visitor.visit_prox(a_elem)
          ;}
          void
        increment(void)
          { m_iter.increment();
          } 
          std::size_t
        size(void)const
          { return m_iter.size();
          } 
          bool
        empty(void)const
          { return m_iter.empty();
          } 
      private:
          iter_type
        m_iter
          ;
      };//end prox_iterator_of<scoped_cyclic_container<...> > specialization

}//exit managed_ptr namespace
}//exit boost namespace
//////////////////////////////////////////////////////////////////////////////
// ChangeLog:
//   2004-01-22: Larry Evans
//     WHAT:
//       changed prox_iterator_of<scoped_cyclic_container<...> > to use
//       "aggregation" (member variable m_iter) instead of inheritance   
//       (by : public iterator_default::range<...> ) to contain
//       iterator_default::range<...>, even though this required
//       several forwarding functions.
//     WHY:
//       Both como4.3.3 and intel 8.0 fail to compile with inheritance.
//   2004-01-09: Larry Evans
//     WHAT:
//       "merged":
//          http://www.yahoogroups.com/files/boost/shared_cyclic_ptr/
//          shared_cyclic_ptr.zip (/boost/shared_cyclic_ptr/shared_cyclic_ptr.hpp
//         :template class scoped_cyclic_ptr) 
//       with:
//          http://www.yahoogroups.com/files/boost/shared_cyclic_ptr/
//          stl_container.cpp:mod::vector
//          < Subject
//          , mod::allocator
//            < Subject
//            , mod::yes_prox_scan
//            >
//          >
//////////////////////////////////////////////////////////////////////////////
#endif

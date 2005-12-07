#ifndef BOOST_FIELDS_VISITOR_CONTAINER_EXTERN_SINGLE_HPP_LJE20041124
#define BOOST_FIELDS_VISITOR_CONTAINER_EXTERN_SINGLE_HPP_LJE20041124
//  (C) Copyright Larry Evans 2004.
//
//  Permission to copy, use, modify, sell and distribute this software
//  is granted provided this copyright notice appears in all copies.
//  This software is provided "as is" without express or implied
//  warranty, and with no claim as to its suitability for any purpose.
//
#ifdef CONTAINER_SINGLE_OBJ_ID
#include "boost/utility/obj_id.hpp"
#include "boost/utility/trace_scope.hpp"
#endif
#include "boost/fields_visitor/registrar_heirarchy.hpp"
#include <memory>
namespace boost
{
namespace fields_visitor
{
namespace container_extern
{
/**@brief
 *  Enable visiting a record's fields where
 *  record is a templated container with
 *  a single template parameter describing it's
 *  contained fields.
 */
template
  < template
      < typename //Element
      , typename //Allocator with default value
      >
    class Container
  , typename Element
  , typename VisitorSequence
  , typename Allocator=std::allocator<Element>
  >
  class
single
  : public Container<Element,Allocator>
#ifdef CONTAINER_SINGLE_OBJ_ID
  , public utility::obj_id
#endif
  , public registrar_heirarchy<VisitorSequence>
{
 public:
        typedef
      Element
    element_type
    ;
        typedef
      single
        < Container
        , element_type
        , VisitorSequence
        , Allocator
        >
    my_type
    ;
        typedef
      Container<element_type, Allocator>
    container_type
    ;
        typedef
      registrar_heirarchy<VisitorSequence>
    registrar_type
    ;
    ~single(void)
    {
        #ifdef CONTAINER_SINGLE_OBJ_ID
        utility::trace_scope ts("-single(void)");
        mout()<<"single::TOR-:id="<<id_get()<<":"<<"\n";
        #endif
    }
    single
      ( void
      )
      : registrar_type(this)
    {
        #ifdef CONTAINER_SINGLE_OBJ_ID
        utility::trace_scope ts("+single(void)");
        mout()<<"single::TOR+:id="<<id_get()<<":"<<"\n";
        #endif
    }
    single
      ( unsigned const a_size
      )
      : container_type(a_size)
      , registrar_type(this)
    {
        #ifdef CONTAINER_SINGLE_OBJ_ID
        utility::trace_scope ts("+single(unsigned)");
        mout()<<"single::TOR+:id="<<id_get()<<":size="<<a_size<<"\n";
        #endif
    }
};//end single<FieldsVisitor, Container, Element> general template
  
}//exit container_extern namespace

template
  < typename VisitorSequence
  , typename FieldsVisitor
  , template
      < typename //Element
      , typename //Allocator
      >
      class Container
  , typename Element
  , typename Allocator
  >
struct field_iterator_of
  < FieldsVisitor
  , container_extern::single
    < Container
    , Element
    , VisitorSequence
    , Allocator
    >
  >
{
 public:
        typedef
      FieldsVisitor
    fields_visitor_type
    ;
        typedef
      iter_range
      < Container
        < Element
        , Allocator
        >
      >
    iter_type
    ;    
        typedef        
      container_extern::single
      < Container
      , Element
      , VisitorSequence
      , Allocator
      >
    record_type
    ;
    field_iterator_of
      ( record_type* a_record
      )
      : m_iter(*a_record)
    {
    }
      void
    accept(fields_visitor_type& a_visitor)const
    { 
        Element& a_elem = *m_iter;
        a_visitor.visit_field(a_elem);
    }
      void
    increment(void)
    { 
        ++m_iter;
    } 
      std::size_t
    size(void)const
    { 
        field_iterator_of l_cpy(*this);
        std::size_t l_siz=0;
        for(; !l_cpy.empty(); ++l_siz, l_cpy.increment())
        return l_siz;
    } 
      bool
    empty(void)const
    { 
        return m_iter.empty();
    }
    
private:
      iter_type
    m_iter
    ;
};//end field_iterator_of<FieldsVisitor,container_extern::single<...> > specialization

}//exit fields_visitor namespace
}//exit boost namespace
//////////////////////////////////////////////////////////////////////////////
// ChangeLog:
//   2005-08-17: Larry Evans
//     WHAT:
//       replaced rangelib::crange with new iter_range.
//       and made corresponding change to empty (by using iter_type::empty).
//     WHY:
//       ease transition to using boost::range::iterator_range.
//   2004-11-14: Larry Evans
//     WHAT:
//       copied and renamed from ../../managed_ptr/stlcont_prox_extern
//       /scoped_cyclic_container.hpp
//     WHY:
//       For same reason cited in ../field_visitor.hpp ChangeLog entry
//       for 2004-10-25.
//////////////////////////////////////////////////////////////////////////////
#endif

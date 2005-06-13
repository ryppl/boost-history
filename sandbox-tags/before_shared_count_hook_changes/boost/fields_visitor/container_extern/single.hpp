#ifndef BOOST_FIELDS_VISITOR_CONTAINER_EXTERN_SINGLE_HPP_LJE20041124
#define BOOST_FIELDS_VISITOR_CONTAINER_EXTERN_SINGLE_HPP_LJE20041124
//  (C) Copyright Larry Evans 2004.
//
//  Permission to copy, use, modify, sell and distribute this software
//  is granted provided this copyright notice appears in all copies.
//  This software is provided "as is" without express or implied
//  warranty, and with no claim as to its suitability for any purpose.
//
#include "boost/fields_visitor/fields_visitor.hpp"
#include <memory>
//#define CONTAINER_SINGLE_OBJ_ID
#ifdef CONTAINER_SINGLE_OBJ_ID
#include "boost/utility/obj_id.hpp"
#endif
namespace boost
{
namespace fields_visitor
{
namespace container_extern
{
/**@brief
 *  Enable visiting a record's fields where
 *  record is a templated container containing
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
  , typename FieldsVisitor
  , typename Allocator=std::allocator<Element>
  >
  class
single
#ifdef CONTAINER_SINGLE_OBJ_ID
  : public utility::obj_id
  , public Container<Element,Allocator>
#else
  : public Container<Element,Allocator>
#endif
  , private field_selector<FieldsVisitor>
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
        , FieldsVisitor
        , Allocator
        >
    my_type
    ;
        typedef
      Container<element_type, Allocator>
    container_type
    ;
        typedef
      field_selector<FieldsVisitor>
    selector_type
    ;
    ~single(void)
    {
        #ifdef CONTAINER_SINGLE_OBJ_ID
        mout()<<"single::TOR-:id="<<id_get()<<":"<<"\n";
        #endif
    }
    single(void)
      : selector_type(this)
    {
    }
    single(unsigned const a_size)
      : container_type(a_size)
      , selector_type(this)
    {
    }
      
};//end single<FieldsVisitor, Container, Element> general template
  
}//exit container_extern namespace

template
  < typename FieldsVisitor
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
    , FieldsVisitor
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
      rangelib::crange
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
      , fields_visitor_type
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
        return !(m_iter.operator bool());
    }
    
private:
      iter_type
    m_iter
    ;
};//end field_iterator_of<FieldsVisitor,single<...> > specialization

}//exit fields_visitor namespace
}//exit boost namespace
//////////////////////////////////////////////////////////////////////////////
// ChangeLog:
//   2004-11-14: Larry Evans
//     WHAT:
//       copied and renamed from ../../managed_ptr/stlcont_prox_extern
//       /scoped_cyclic_container.hpp
//     WHY:
//       For same reason cited in ../field_visitor.hpp ChangeLog entry
//       for 2004-10-25.
//////////////////////////////////////////////////////////////////////////////
#endif

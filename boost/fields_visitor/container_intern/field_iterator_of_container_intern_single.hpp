/**@file
 *  Prototype of how to modify stl containers to use
 *  "custom pointer objects" pointing to the "root" of
 *  the container's data; thereby enabling the
 *  fields_visitor library to iterate over the container's
 *  data.
 */
#ifndef BOOST_FIELDS_VISITOR_FIELD_ITERATOR_OF_CONTAINER_INTERN_SINGLE_HPP_INCLUDED
#define BOOST_FIELDS_VISITOR_FIELD_ITERATOR_OF_CONTAINER_INTERN_SINGLE_HPP_INCLUDED
//--------------------------------------------------------------------
//  (C) Copyright Larry Evans 2005.
//
//  Permission to copy, use, modify, sell and distribute this software
//  is granted provided this copyright notice appears in all copies.
//  This software is provided "as is" without express or implied
//  warranty, and with no claim as to its suitability for any purpose.
//--------------------------------------------------------------------

#include "boost/fields_visitor/container_intern/prox_indirect_registrar_heirarchy.hpp"
namespace boost
{
namespace fields_visitor
{
  template
  < typename VisitorSequence 
    //^ Sequence of fields_visitor's for use as arg to ../registrar_heirarchy.hpp.
  , typename FieldsVisitor
  , template
      < typename //Element
      , typename //Allocator
      >
      class Single //the modified stl container
  , typename Element
  >
struct field_iterator_of
  < FieldsVisitor
  , Single
    < Element
    , container_intern::allocator 
      < Element
      , VisitorSequence 
        //^ enables iteration over Single's data, if != empty_registrar
        //via specialization of prox_indirect, as in:
        //   prox_indirect_registrar_heirarchy.hpp
      >
    >
  >
{
 public:
        typedef
      FieldsVisitor
    fields_visitor_type
    ;
        typedef
      container_intern::allocator<Element,VisitorSequence>
    alloc_type
    ;
        typedef
      rangelib::crange
      < Single
        < Element
        , alloc_type
        >
      >
    iter_type
    ;    
        typedef        
      Single
      < Element
      , alloc_type
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
};//end field_iterator_of<FieldsVisitor,container_intern::vector<...> > specialization

}//exit fields_visitor namespace
}//exit boost namespace

#endif

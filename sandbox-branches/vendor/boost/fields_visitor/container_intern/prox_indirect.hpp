#ifndef BOOST_FIELDS_VISITOR_CONTAINER_INTERN_PROX_INDIRECT_HPP_INCLUDED
#define BOOST_FIELDS_VISITOR_CONTAINER_INTERN_PROX_INDIRECT_HPP_INCLUDED
/**@file
 *  Prototype of how to modify stl containers to use 
 *  fields_visitor library.
 */
//--------------------------------------------------------------------
//  (C) Copyright Larry Evans 2005.
//
//  Permission to copy, use, modify, sell and distribute this software
//  is granted provided this copyright notice appears in all copies.
//  This software is provided "as is" without express or implied
//  warranty, and with no claim as to its suitability for any purpose.
//--------------------------------------------------------------------
#include "boost/fields_visitor/empty_registrar.hpp"
namespace boost
{
namespace fields_visitor
{
namespace container_intern
{
  template
    < typename Subject
    >
  struct prox_typed
    {
          typedef
        Subject
      subj_type
        ;
          typedef
        prox_typed<subj_type>
      my_type
        ;
      prox_typed
        ( subj_type* a_subj=0
        )
        : m_subj(a_subj)
        {}
        my_type const&
      subj_set(subj_type* a_subj=0)
        { 
        ; m_subj = a_subj
        ; return *this
        ;}
        my_type const&
      operator=(my_type const& a_my_type)
        { 
        ; return subj_set(a_my_type.m_subj)
        ;}
        subj_type*
      subj_mut(void)
        { return m_subj
        ;}
        subj_type const*
      subj_con(void)const
        { return m_subj
        ;}
        bool
      is_null(void)const
        { return m_subj == static_cast<subj_type*>(0)
        ;}
    private:
        subj_type*
      m_subj
        ;
    };//end prox_typed struct

    template
      < typename Value
      , typename VisitorSequence=registrar_heirarchy_detail::empty_registrar
      >
    struct prox_indirect
      ;//end prox_indirect general template
      
    template
      < typename Value
      >
    struct prox_indirect
      < Value
      , registrar_heirarchy_detail::empty_registrar
      >
      : public prox_typed<Value>
      {
          prox_indirect(Value* a_value, void* a_container=0)
            : prox_typed<Value>(a_value)
            {}
      };//end prox_indirect general template
      
    template
      < typename Value
      , typename VisitorSequence=registrar_heirarchy_detail::empty_registrar
      >
    struct allocator
      : public std::allocator<Value>
      //Purpose:
      //  replacement for std::allocator<Value>
      {
            typedef
          prox_indirect<Value,VisitorSequence>
        root_pointer
          ;
      };//end allocator
      
}//exit container_intern namespace
}//exit fields_visitor namespace
}//exit boost namespace

//ChangeLog:
//  2005-06-19 Larry Evans
//    WHAT:
//      1) Removed all code after allocator declaration.
//      2) Renamed file to prox_indirect.hpp.
//      3) Removed all #includes except for that containing std::allocator.
//      4) Enclose everything in new namespace.
//    WHY:
//      1) Not needed to define prox_indirect.
//      2) To reflect what will be specialized.
//      3) Minimize dependencies.
//      4) Indicate code is part of fields_visitor.
//  2005-06-15: Larry Evans
//    WHAT:
//      copied from:
/*
boost/sand-box/lje/libs/managed_ptr.old/prototypes/stl_container/stl_container.cpp
 */
//    WHY:
//      to reuse, after modification, for policy_ptr.

#endif

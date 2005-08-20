#ifndef BOOST_FIELDS_VISITOR_CONTAINER_INTERN_PROX_INDIRECT_REGISTRAR_HEIRARCHY_HPP_INCLUDED
#define BOOST_FIELDS_VISITOR_CONTAINER_INTERN_PROX_INDIRECT_REGISTRAR_HEIRARCHY_HPP_INCLUDED
/**@file
 */

//  (C) Copyright Larry Evans 2005.
//
//  Permission to copy, use, modify, sell and distribute this software
//  is granted provided this copyright notice appears in all copies.
//  This software is provided "as is" without express or implied
//  warranty, and with no claim as to its suitability for any purpose.
#include "boost/fields_visitor/container_intern/prox_indirect.hpp"
#include "boost/fields_visitor/registrar_heirarchy.hpp"
//--------------------------------------------------------------------
//  (C) Copyright Larry Evans 2005.
//
//  Permission to copy, use, modify, sell and distribute this software
//  is granted provided this copyright notice appears in all copies.
//  This software is provided "as is" without express or implied
//  warranty, and with no claim as to its suitability for any purpose.
//--------------------------------------------------------------------
namespace boost
{
namespace fields_visitor
{
namespace container_intern
{
    template
      < typename Value
      , typename VisitorSequence
      >
    struct prox_indirect
      : public prox_typed<Value>
      , public registrar_heirarchy<VisitorSequence>
      {
          template
            < typename Container
            >
          prox_indirect(Value* a_value, Container* a_container)
            : prox_typed<Value>(a_value)
            , registrar_heirarchy<VisitorSequence>(a_container)
            {}
      };//end prox_indirect specialization
      
}//exit container_intern namespace
}//exit fields_visitor namespace
}//exit boost namespace

//ChangeLog:
//  2005-06-19 Larry Evans
//    WHAT:
//      modified from prox_indirect.hpp.
//    WHY:
//      to enable fields_visitor lib to traverse Container which
//      is arg to CTOR.
#endif

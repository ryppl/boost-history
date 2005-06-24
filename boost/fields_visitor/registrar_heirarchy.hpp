//defines template allowing "field" to be visited by sequence of visitors.
#ifndef BOOST_FIELDS_VISITOR_REGISTRAR_HEIRARCHY_LJE20050318
#define BOOST_FIELDS_VISITOR_REGISTRAR_HEIRARCHY_LJE20050318
//  (C) Copyright Larry Evans 2005.
//
//  Permission to copy, use, modify, sell and distribute this software
//  is granted provided this copyright notice appears in all copies.
//  This software is provided "as is" without express or implied
//  warranty, and with no claim as to its suitability for any purpose.
//
#include <boost/mpl/inherit_linearly.hpp>
#include <boost/mpl/arg.hpp>
#include "boost/fields_visitor/fields_visitor.hpp"
#include "boost/fields_visitor/empty_registrar.hpp"

namespace boost
{
namespace fields_visitor
{

namespace registrar_heirarchy_detail
{
  template
  < typename ThisVisitor
  , typename OtherVisitors
  >
  struct
partial_registrars
  : public OtherVisitors
  , public boost::fields_visitor::field_registrar<ThisVisitor>
/**@brief
 *  Appends one element, field_registrar<ThisVisitor>,
 *  to the heirarchy of registrars mentioned in comments
 *  to registrar_heirarchy.  As mentioned in comments
 *  to field_registrar, the registrar CTOR argument,
 *  a SelectedField*, is used to "register" this SelectedField
 *  in a "descriptor" of the "record" to which this SelectedField 
 *  belongs.
 *
 *  Unfortunately, this means there's a separate descriptor
 *  for each "ThisVisitor" for the "record" :<
 */
{
        typedef
      boost::fields_visitor::field_registrar<ThisVisitor>
    registrar_type
    ;
      template<typename SelectedField>
    partial_registrars
      ( SelectedField* a_field
      )
      : OtherVisitors(a_field)
      , registrar_type(a_field)
    {
      #if 0 && defined( CONTAINER_SINGLE_OBJ_ID)
        debug_utility::trace_scope ts("partial_registrar(SelectedField*a_field)");
        mout()<<":id_get="<<a_field->id_get()<<":visitor="<<ThisVisitor::our_visitor_id<<"\n";
      #endif
    }
};


}//exit registrar_heirarchy_detail

template<typename VisitorSequence>
struct registrar_heirarchy
/**@brief
 *  Allow an mpl sequence of visitors to be used
 *  as template arg to a corresponding sequence of "registrar"
 *  super-classes of some derived class, SelectedField,
 *  and pass SelectedField* to each such registrar CTOR in the
 *  heirarchy.
 */
  : mpl::inherit_linearly
    < VisitorSequence
    , registrar_heirarchy_detail::partial_registrars
      < boost::mpl::arg<2>
      , boost::mpl::arg<1> 
      >
    , registrar_heirarchy_detail::empty_registrar
    >::type
{
        typedef
      typename boost::mpl::inherit_linearly
      < VisitorSequence
      , registrar_heirarchy_detail::partial_registrars
        < boost::mpl::arg<2>
        , boost::mpl::arg<1> 
        >
      , registrar_heirarchy_detail::empty_registrar
      >::type
    super_type
    ;
      template<typename SelectedField>
    registrar_heirarchy(SelectedField*a_field)
      : super_type(a_field)
    {
      #if 0 && defined(CONTAINER_SINGLE_OBJ_ID)
        debug_utility::trace_scope ts("registrar_heirarchy(SelectedField*a_field)");
        mout()<<":id_get="<<a_field->id_get()<<"\n";
      #endif
    }
};//end registrar_heirarchy

}//exit fields_visitor namespace
}//exit boost namespace
#endif

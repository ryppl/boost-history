//forward declaration of operators for calculating the layout of a composite.
#ifndef BOOST_COMPOSITE_STORAGE_LAYOUT_OPERATORS_FWD_HPP_INCLUDED
#define BOOST_COMPOSITE_STORAGE_LAYOUT_OPERATORS_FWD_HPP_INCLUDED
//  (C) Copyright Larry Evans 2010.
//
//  Permission to copy, use, modify, sell and distribute this software
//  is granted provided this copyright notice appears in all copies.
//  This software is provided "as is" without express or implied
//  warranty, and with no claim as to its suitability for any purpose.
//
#include <boost/composite_storage/tags.hpp>

namespace boost
{
namespace composite_storage
{
namespace layout
{
  template
  < class CompositeTag
  >
  struct 
operators
/**@brief
 *  Member templates:
 *    push_back
 *    layout0
 *  are used to calculate various "layout" traits of a composite.
 *  These traits include at least:
 *    the size
 *  and possibly:
 *    the offsets and alignments
 *  of the composite's components.
 *  Also a layout contains member functions:
 *    inject:
 *      for creating or modifying a component.
 *    project:
 *      for retrieving a component.
 */
{
      template
      < typename IndexUndefined //index for accessing this layout.
      >
      struct
    layout0
    /**@brief
     *  Nullary operator:
     *    The layout of a composite with 0 components.
     */
    ;
      template
      < typename HeadLayout   //layout of some Composite
      , typename TailComponent//Type to be appended to some Composite
      >
      struct      
    push_back
    /**@brief
     *  Binary operator:
     *    Calculates the layout of composite with component, TailComponent,
     *    appended to a Composite with layout, HeadLayout, and return
     *    layout result in push_back<HeadLayout,TailComponent>::type.
     */
    ;
};

}//exit detail namespace
}//exit composite_storage namespace
}//exit boost namespace
#endif

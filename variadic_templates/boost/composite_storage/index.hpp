//ustility for associating a single component with its index or key.
#ifndef BOOST_COMPOSITE_STORAGE_INDEX_HPP_INCLUDED
#define BOOST_COMPOSITE_STORAGE_INDEX_HPP_INCLUDED
//  (C) Copyright Larry Evans 2010.
//
//  Permission to copy, use, modify, sell and distribute this software
//  is granted provided this copyright notice appears in all copies.
//  This software is provided "as is" without express or implied
//  warranty, and with no claim as to its suitability for any purpose.
//
//====================================================================
#include <boost/composite_storage/index_component.hpp>
namespace boost
{
namespace composite_storage
{
      template
      < typename IndexType
      , IndexType IndexValue
      >
    struct index
    /**@brief
     *  See operator=.
     */
    {
        template
        < typename Component
        >
          index_component
          < IndexType
          , IndexValue
          , Component
          >
        operator=(Component a_comp)const
          /**@brief
           *  Produce an association between IndexValue and a_comp.
           */
          {
            return index_component<IndexType,IndexValue,Component>(a_comp);
          }
    };
    
}//exit composite_storage namespace
}//exit boost namespace

#endif

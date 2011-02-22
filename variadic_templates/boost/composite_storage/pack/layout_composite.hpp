//operators for calculating the layout of one_of_maybe composite.
#ifndef BOOST_COMPOSITE_STORAGE_PACK_LAYOUT_COMPOSITE_HPP_INCLUDED
#define BOOST_COMPOSITE_STORAGE_PACK_LAYOUT_COMPOSITE_HPP_INCLUDED
//  (C) Copyright Larry Evans 2010.
//
//  Permission to copy, use, modify, sell and distribute this software
//  is granted provided this copyright notice appears in all copies.
//  This software is provided "as is" without express or implied
//  warranty, and with no claim as to its suitability for any purpose.
//====================================================================
#include <boost/composite_storage/layout/operators_fwd.hpp>
#include <boost/composite_storage/enum_base.hpp>
#include <boost/mpl/fold_assoc_pack.hpp>
#include <boost/mpl/next.hpp>
#include <boost/type_traits/remove_reference.hpp>

namespace boost
{
namespace composite_storage
{
namespace pack
{
  template
  < class CompositeTag
  , class Index0
  , typename... Components
  >
struct layout_composite
{
        typedef
      layout::operators<CompositeTag>
    layout_ops
    ;
        typedef
      typename Index0::value_type
    index_type
    ;
        typedef typename
      enum_base<Index0>::type
    index_base
    ;
        typedef
      mpl::integral_c<index_base,index_base(Index0::value)-1>
    index_undefined
    ;
        typedef
      typename mpl::fold_assoc_pack
      < mpl::assoc_left
      , layout_ops::template push_back
      , typename layout_ops::template layout0<index_undefined>
      , Components...
      >::type
    scanned
    ;
        typedef
      typename mpl::next<typename scanned::index_part>::type
    index_end
    ;
      template
      < index_type IndexValu
      >
    struct result_type
    {
            typedef
          mpl::integral_c<index_base,IndexValu>
        index_value
        ;
            typedef
          decltype
          ( scanned::project
            ( index_value()
            , static_cast<char*>(0)
            )
          )
        ref
        ;
            typedef
              typename
          remove_reference<ref>::type
        type
        ;
    };
};

}//exit pack
}//exit composite_storage namespace
}//exit boost namespace
#endif


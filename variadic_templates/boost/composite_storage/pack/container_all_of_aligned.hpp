//container template  containing all components in template arg list, all of which are aligned.
#ifndef BOOST_COMPOSITE_STORAGE_CONTAINER_ALL_OF_ALIGNED_HPP_INCLUDED
#define BOOST_COMPOSITE_STORAGE_CONTAINER_ALL_OF_ALIGNED_HPP_INCLUDED
//  (C) Copyright Larry Evans 2010.
//
//  Permission to copy, use, modify, sell and distribute this software
//  is granted provided this copyright notice appears in all copies.
//  This software is provided "as is" without express or implied
//  warranty, and with no claim as to its suitability for any purpose.
//====================================================================
#include <boost/composite_storage/layout/operators_all_of_aligned.hpp>
#include <boost/composite_storage/pack/layout_composite.hpp>
#include <boost/composite_storage/pack/container_fwd.hpp>
#include <boost/composite_storage/methods/all_of.hpp>
#include <boost/composite_storage/buffers/layout_buf.hpp>

namespace boost
{
namespace composite_storage
{
namespace pack
{
  template
  < class Index0
  , typename... Components
  >
  struct
container
  < tags::all_of_aligned
  , Index0
  , Components...
  >
: buffers::layout_buf
  < layout_composite
    < tags::all_of_aligned
    , Index0
    , Components...
    >
  >
{
        typedef
      layout_composite
      < tags::all_of_aligned
      , Index0
      , Components...
      >
    layout_comp
    ;
        typedef
      typename layout_comp::scanned
    scanned
    ;
        typedef
      typename layout_comp::index_base
    index_base
    ;
        typedef
      typename layout_comp::index_undefined
    index_undefined
    ;
        typedef
      typename layout_comp::index_type
    index_type
    ;
        typedef
      methods::all_of<scanned,Index0>
    methods_all
    ;
 public:
    container(void)
    {
        char*to_buf=this->address();
        methods_all::ctor_default_all(to_buf);
    }
    container( container& from)
    {
        char      *to_buf=this->address();
        char const*fr_buf=from.address();
          methods_all::
        ctor_copy_all
        ( to_buf
        , fr_buf
        );
    }
    container( container&& from)
    {
        char      *to_buf=this->address();
        char      *fr_buf=from.address();
          methods_all::
        ctor_copy_all
        ( to_buf
        , fr_buf
        );
    }
      container const&
    operator=(container const& from)
    {
        char      *to_buf=this->address();
        char const*fr_buf=from.address();
          methods_all::
        assign_all
        ( to_buf
        , fr_buf
        );
        return *this;
    }
      container const&
    operator=(container&& from)
    {
        char      *to_buf=this->address();
        buffers::rval_ref_buf fr_buf(from.address());
          methods_all::
        assign_all
        ( to_buf
        , fr_buf
        );
        return *this;
    }
    ~container(void)
    {
        scanned::destroy( this->address());
    }   
 
      template
      < index_type IndexValu
      >
    struct result_type
    : layout_comp::template result_type<IndexValu>
    {
    };
    
      template
      < index_type IndexValu
      >
      typename result_type<IndexValu>::type const&
    project(void)const
    {
        mpl::integral_c<index_base,IndexValu> index;
        return scanned::project(index,this->address());
    }        
      template
      < index_type IndexValu
      >
      typename result_type<IndexValu>::type &
    project(void)
    {
        mpl::integral_c<index_base,IndexValu> index;
        return scanned::project(index,this->address());
    }        
};

}//exit pack namespace
}//exit composite_storage namespace
}//exit boost namespace

#endif

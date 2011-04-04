//container template provided all public functions but constructors.
#ifndef BOOST_COMPOSITE_STORAGE_CTOR_PROTECTED_ALL_OF_ALIGNED_HPP_INCLUDED
#define BOOST_COMPOSITE_STORAGE_CTOR_PROTECTED_ALL_OF_ALIGNED_HPP_INCLUDED
//  (C) Copyright Larry Evans 2011.
//
//  Permission to copy, use, modify, sell and distribute this software
//  is granted provided this copyright notice appears in all copies.
//  This software is provided "as is" without express or implied
//  warranty, and with no claim as to its suitability for any purpose.
//====================================================================
#include <boost/composite_storage/layout/operators_all_of_aligned.hpp>
#include <boost/composite_storage/pack/layout_composite.hpp>
#include <boost/composite_storage/pack/ctor_protected_fwd.hpp>
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
ctor_protected
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
      methods::layout_indices
      < scanned
      , Index0
      >
    layout_indices
    ;
        typedef
      methods::all_of
      < layout_indices
      >
    methods_all
    ;
      ctor_protected const&
    operator=(ctor_protected const& from)
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
      ctor_protected const&
    operator=(ctor_protected&& from)
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
    ~ctor_protected(void)
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
    
 protected:
      template
      < index_type IndexValu
      , typename... Args
      >
      void
    inject(Args&&... args)
    {
        mpl::integral_c<index_base,IndexValu> index;
        scanned::inject(index,this->address(),args...);
    }
    
    ctor_protected(void)
    {}
    ctor_protected( ctor_protected const& from)
    {}
};

}//exit pack namespace
}//exit composite_storage namespace
}//exit boost namespace

#endif

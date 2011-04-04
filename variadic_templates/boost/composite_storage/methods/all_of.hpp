//methods common to composites with all_of* tags.
#ifndef BOOST_COMPOSITE_STORAGE_METHODS_ALL_OF_HPP_INCLUDED
#define BOOST_COMPOSITE_STORAGE_METHODS_ALL_OF_HPP_INCLUDED
//  (C) Copyright Larry Evans 2010.
//
//  Permission to copy, use, modify, sell and distribute this software
//  is granted provided this copyright notice appears in all copies.
//  This software is provided "as is" without express or implied
//  warranty, and with no claim as to its suitability for any purpose.
//
//====================================================================
#include <boost/mpl/range_c.hpp>
#include <boost/mpl/next.hpp>
#include <boost/mpl/for_each.hpp>
#include <boost/composite_storage/enum_base.hpp>

namespace boost
{
namespace composite_storage
{
namespace methods
{

  template
  < typename Layout
  , typename IndexBegin
  >
  struct
layout_indices
: Layout
{

        typedef
      IndexBegin
    index_begin
    ;
        typedef
      typename mpl::next<typename Layout::index_part>::type
    index_end
    ;
        typedef typename
      enum_base<IndexBegin>::type
    index_type
    ;
        typedef typename
      mpl::range_c
      < index_type
      , index_begin::value
      , index_end::value
      >::type
    indices
    ;
};

  template
  < typename Layout
  >
  struct
all_of
{
        typedef
      typename Layout::indices
    indices
    ;
      template
      < typename FrBuffer
      >
      struct
    ctor_default_one
    {

          FrBuffer
        my_buffer
        ;
        ctor_default_one(FrBuffer a_buffer)
        : my_buffer(a_buffer)
        {}
          template
          < class Index
          >
          void
        operator()(Index index)const
        {
            Layout::inject_default(index,my_buffer);
        }
         
    };
      template
      < typename FrBuffer
      >
        static
      void
    ctor_default_all(FrBuffer buffer)
    //Call default construct for all components.
    {
        ctor_default_one<FrBuffer> constructor(buffer);
        mpl::for_each<indices>(constructor);
    };
      template
      < typename FrBuffer
      >
      struct
    ctor_copy_one
    {

          char*
        my_to_buffer
        ;
          FrBuffer
        my_fr_buffer
        ;
        ctor_copy_one(char* a_to_buffer, FrBuffer a_fr_buffer)
        : my_to_buffer(a_to_buffer)
        , my_fr_buffer(a_fr_buffer)
        {}
          template
          < class Index
          >
          void
        operator()(Index index)const
        {
            Layout::inject(index,my_to_buffer,Layout::project(index,my_fr_buffer));
        }
         
    };
      template
      < typename FrBuffer
      >
        static
      void
    ctor_copy_all(char* to_buffer, FrBuffer fr_buffer)
    //Call copy construct for all components in to_buffer
    //from corresponding components in fr_buffer.
    {
        ctor_copy_one<FrBuffer> constructor(to_buffer, fr_buffer);
        mpl::for_each<indices>(constructor);
    };
      template
      < typename FrBuffer
      >
      struct
    assign_one
    {

          char*
        my_to_buffer
        ;
          FrBuffer
        my_fr_buffer
        ;
        assign_one(char* a_to_buffer, FrBuffer a_fr_buffer)
        : my_to_buffer(a_to_buffer)
        , my_fr_buffer(a_fr_buffer)
        {}
          template
          < class Index
          >
          void
        operator()(Index index)const
        {
            Layout::project(index,my_to_buffer)
              = Layout::project
                ( index
                , my_fr_buffer
                );
        }
         
    };
      template
      < typename FrBuffer
      >
        static
      void
    assign_all(char*to_buffer, FrBuffer fr_buffer)
    //Call assignment operator for all components in to_buffer
    //from corresponding components in fr_buffer.
    {
        assign_one<FrBuffer> constructor(to_buffer, fr_buffer);
        mpl::for_each<indices>(constructor);
    };
};

}//exit methods namespace

}//exit composite_storage namespace
}//exit boost namespace
#endif

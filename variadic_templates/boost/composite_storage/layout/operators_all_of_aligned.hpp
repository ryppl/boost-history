//operators for calculating the layout of all_of_aligned composite.
#ifndef BOOST_COMPOSITE_STORAGE_LAYOUT_OPERATORS_ALL_OF_ALIGNED_HPP_INCLUDED
#define BOOST_COMPOSITE_STORAGE_LAYOUT_OPERATORS_ALL_OF_ALIGNED_HPP_INCLUDED
//  (C) Copyright Larry Evans 2010.
//
//  Permission to copy, use, modify, sell and distribute this software
//  is granted provided this copyright notice appears in all copies.
//  This software is provided "as is" without express or implied
//  warranty, and with no claim as to its suitability for any purpose.
//
#include <boost/composite_storage/layout/operators_fwd.hpp>
#include <boost/composite_storage/tags.hpp>
#include <boost/composite_storage/layout/layout_of.hpp>
#include <boost/composite_storage/buffers/rval_ref_buf.hpp>
#include <boost/composite_storage/alignment/compose.hpp>
#include <boost/composite_storage/alignment/aligned_offset.hpp>
#include <boost/composite_storage/special_components.hpp>
#include <boost/mpl/next.hpp>

#include <memory>

namespace boost
{
namespace composite_storage
{
namespace layout
{
    
  template
  <// class CompositeTag
  >
struct operators< tags::all_of_aligned>
: components_aligned_yes
{
  
      template
      < typename HeadComposed
      , typename TailLayout
      >
    struct compose_layout
    {
            static
          std::size_t const
        offset
        =alignment::aligned_offset
         < HeadComposed::size
         , TailLayout::align
         >::value
        ;
            static
          std::size_t const
        size
        =offset
        +TailLayout::size
        ;
            static
          std::size_t const
        align
        =alignment::compose
         < HeadComposed::align
         , TailLayout::align
         >::value
        ;
    };
        
      template
      < typename IndexUndefined
      >
    struct layout0
    {
            typedef
          IndexUndefined
        index_part
        ;
            typedef
          layout_data<>
        comp_part
        ;
            static
          void
        inject(void)
        {
        }
            static
          void
        inject_default(void)
        {
        }
            static
          void
        project(void)
        {
        }
            static
          void
        destroy(char*buffer_composite)
        {
        }
    };
    
      template
      < typename HeadLayout
      , typename TailComponent
      >
    struct push_back
    {
          struct
        type
        : private HeadLayout
        {
                typedef
              HeadLayout
            head_layout
            ;
                typedef
              typename mpl::next<typename HeadLayout::index_part>::type
            index_part
            ;
                typedef
              compose_layout
              < typename HeadLayout::comp_part
              , layout_of<TailComponent>
              >
            comp_part
            ;
              using HeadLayout::
            inject
            ;
                static
              void
            inject
              ( index_part index_arg
              , char*buffer_composite
              , TailComponent&& tail_component
              )
            {
                void*tail_buffer=buffer_composite+comp_part::offset;
                new(tail_buffer) TailComponent(std::forward<TailComponent>(tail_component));
            }
                static
              void
            inject
              ( index_part index_arg
              , char*buffer_composite
              , TailComponent const& tail_component
              )
            {
                void*tail_buffer=buffer_composite+comp_part::offset;
                new(tail_buffer) TailComponent(tail_component);
            }
              using HeadLayout::
            inject_default
            ;
                static
              void
            inject_default
              ( index_part index_arg
              , char*buffer_composite
              )
            
            {
                TailComponent tail_component;
                inject(index_arg,buffer_composite,std::move(tail_component));
            }
              using HeadLayout::
            project
            ;
                static
              TailComponent const&
            project(index_part index_arg, char const*buffer_composite)
            {
                void const*tail_buffer=buffer_composite+comp_part::offset;
                TailComponent const*tail_ptr=static_cast<TailComponent const*>(tail_buffer);
                return *tail_ptr;
            }
                static
              TailComponent&
            project(index_part index_arg, char*buffer_composite)
            {
                void*tail_buffer=buffer_composite+comp_part::offset;
                TailComponent*tail_ptr=static_cast<TailComponent*>(tail_buffer);
                return *tail_ptr;
            }
                static
              TailComponent&&
            project(index_part index_arg, buffers::rval_ref_buf buffer_composite)
            {
                void*tail_buffer=buffer_composite.my_buf+comp_part::offset;
                TailComponent*tail_ptr=static_cast<TailComponent*>(tail_buffer);
                return std::move(*tail_ptr);
            }
                static
              void
            destroy( char*buffer_composite)
            {
                TailComponent&tail_ref=project( index_part(), buffer_composite);
                tail_ref.~TailComponent();
                HeadLayout::destroy(buffer_composite);
            }
            
        };//end type struct
    };//end push_back struct
    
      template
      < typename HeadLayout
      >
    struct push_back< HeadLayout, void>
    {
            typedef
          void
        TailComponent
        ;
          struct
        type
        : private HeadLayout
        {
                typedef
              HeadLayout
            head_layout
            ;
                typedef
              typename mpl::next<typename HeadLayout::index_part>::type
            index_part
            ;
              using HeadLayout::
            comp_part
            ;
              using HeadLayout::
            inject
            ;
              using HeadLayout::
            inject_default
            ;
                 static
              void
            inject_default
              ( index_part index_arg
              , char*buffer_composite
              )
            
            {
            }
             using HeadLayout::
            project
            ;
                static
              void
            destroy( char*buffer_composite)
            {
                HeadLayout::destroy(buffer_composite);
            }
            
        };//end type struct
    };//end push_back<.,void> struct
};//end operators struct

}//exit layout namespace
}//exit composite_storage namespace
}//exit boost namespace
#endif


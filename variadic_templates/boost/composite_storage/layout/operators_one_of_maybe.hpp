//operators for calculating the layout of one_of_maybe composite.
#ifndef BOOST_COMPOSITE_STORAGE_DETAIL_LAYOUT_OPERATORS_ONE_OF_MAYBE_HPP_INCLUDED
#define BOOST_COMPOSITE_STORAGE_DETAIL_LAYOUT_OPERATORS_ONE_OF_MAYBE_HPP_INCLUDED
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
struct operators< tags::one_of_maybe>
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
        size
        = HeadComposed::size
        > TailLayout::size
        ? HeadComposed::size
        : TailLayout::size
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
          special_components::nothing
        tail_type
        ;
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
        inject_default( index_part index_arg, char*buffer_composite)
        {
        }
            static
          void
        inject( index_part index_arg, char*buffer_composite, tail_type const&)
        {
        }
            static
          special_components::nothing const&
        project(index_part index_arg, char const*buffer_composite)
        {
            return tail_type::_();
        }
            static
          special_components::nothing&
        project(index_part index_arg, char*buffer_composite)
        {
            return tail_type::_();
        }
            static
          special_components::nothing&
        project(index_part index_arg, buffers::rval_ref_buf buffer_composite)
        {
            return tail_type::_();
        }
            static
          void
        destroyer( index_part index_arg, char*buffer_composite)
        {
        }
    };
    
      template
      < typename HeadLayout
      , typename TailComponent
      >
    struct push_back
    {
        struct type
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
              template
              < typename TailConvertible
              , int Dummy=0
              >
              struct
            inject_non_empty
            {
                    static
                  void
                _
                  ( char*buffer_composite
                  )
                {
                    new(buffer_composite) TailComponent;
                }
                    static
                  void
                _
                  ( char*buffer_composite
                  , TailConvertible const& tail_component
                  )
                {
                    new(buffer_composite) TailComponent(tail_component);
                }
                    static
                  void
                _
                  ( char*buffer_composite
                  ,   TailConvertible&& tail_component
                  )
                {
                    new(buffer_composite) TailComponent(std::forward<TailConvertible>(tail_component));
                }
            };

              template
              < int Dummy
              >
              struct
            inject_non_empty
              < special_components::empty
              , Dummy
              >
            {
                    static
                  void
                _
                  ( char*buffer_composite
                  )
                {
                }
                    static
                  void
                _
                  ( char*buffer_composite
                  , special_components::empty
                  )
                {
                }
            };
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
                inject_non_empty<TailComponent>::_(buffer_composite);
            }

              using HeadLayout::
            inject
            ;
                static
              void
            inject
              ( index_part index_arg
              , char*buffer_composite
              , TailComponent const& tail_component
              )
            {
                inject_non_empty<TailComponent>::_
                ( buffer_composite
                , tail_component
                );
            }
                static
              void
            inject
              ( index_part index_arg
              , char*buffer_composite
              , TailComponent&& tail_component
              )
            {
                inject_non_empty<TailComponent>::_
                ( buffer_composite
                , std::forward<TailComponent>(tail_component)
                );
            }
       
              using HeadLayout::
            project
            ;
                static
              TailComponent const&
            project(index_part index_arg, char const*buffer_composite)
            {
                void const*tail_buffer=buffer_composite;
                TailComponent const*tail_ptr=static_cast<TailComponent const*>(tail_buffer);
                return *tail_ptr;
            }
                static
              TailComponent&
            project(index_part index_arg, char*buffer_composite)
            {
                void*tail_buffer=buffer_composite;
                TailComponent*tail_ptr=static_cast<TailComponent*>(tail_buffer);
                return *tail_ptr;
            }
                static
              TailComponent&
            project(index_part index_arg, buffers::rval_ref_buf buffer_composite)
            {
                void*tail_buffer=buffer_composite.my_buf;
                TailComponent*tail_ptr=static_cast<TailComponent*>(tail_buffer);
                return *tail_ptr;
            }
              using HeadLayout::
            destroyer
            ;
              template     
              < typename TailConvertible
              , int Dummy=0
              >
              struct
            destroy_non_empty
            {
                    static
                  void
                _
                  ( void*buffer_composite
                  )
                {
                    TailComponent*tail_ptr=static_cast<TailComponent*>(buffer_composite);
                    tail_ptr->~TailComponent();
                }
            };

              template
              < int Dummy
              >
              struct
            destroy_non_empty
              < special_components::empty
              , Dummy
              >
            {
                    static
                  void
                _
                  ( void*buffer_composite
                  )
                {
                }
            };
                static
              void
            destroyer( index_part index_arg, char*buffer_composite)
            {
                destroy_non_empty<TailComponent>::_( buffer_composite);
            }
        };
    };
    
};

}//exit layout namespace
}//exit composite_storage namespace
}//exit boost namespace
#endif


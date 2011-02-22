//container template  containing, at most, one of components in template arg list.
#ifndef BOOST_COMPOSITE_STORAGE_CONTAINER_ONE_OF_MAYBE_HPP_INCLUDED
#define BOOST_COMPOSITE_STORAGE_CONTAINER_ONE_OF_MAYBE_HPP_INCLUDED
//  (C) Copyright Larry Evans 2010.
//
//  Permission to copy, use, modify, sell and distribute this software
//  is granted provided this copyright notice appears in all copies.
//  This software is provided "as is" without express or implied
//  warranty, and with no claim as to its suitability for any purpose.
//====================================================================
#include <boost/composite_storage/functor_indexed.hpp>

namespace boost
{
namespace composite_storage
{
namespace detail_one_of_maybe
{

//[functor_indexed_usage

  template
  < typename Layout
  , typename FrBuffer
  >
struct assign_copy_visitor
: functor_indexed::layout_visitor<Layout>
{
        typedef
      typename functor_indexed::layout_visitor<Layout>::case_type
    case_type
    ;
        typedef
      void
    result_type
    ;
      template
      < case_type CaseValu
      >
    result_type operator()( mpl::integral_c<case_type,CaseValu> index) const 
    {
        Layout::scanned::inject
          ( index
          , my_buffer_0
          , std::move
            (  Layout::scanned::project
               ( index
               , my_buffer_1
               )
            )
          );
    }
    
    assign_copy_visitor
      ( char*    composite_buffer_0
      , FrBuffer composite_buffer_1
      )
    : my_buffer_0(composite_buffer_0)
    , my_buffer_1(composite_buffer_1)
    {}
 private:
      char*
    my_buffer_0
    ;
      FrBuffer
    my_buffer_1
    ;
};

  template
  < typename Layout
  >
struct equal_visitor
: functor_indexed::layout_visitor<Layout>
{
        typedef
      typename functor_indexed::layout_visitor<Layout>::case_type
    case_type
    ;
        typedef
      bool
    result_type
    ;
      template
      < case_type CaseValu
      >
    result_type operator()( mpl::integral_c<case_type,CaseValu> index) const 
    {
        bool result =
            Layout::scanned::project(index, my_buffer_0)
          ==Layout::scanned::project(index, my_buffer_1) ;
        return result ;
    }
    
    equal_visitor
      ( char const* composite_buffer_0
      , char const* composite_buffer_1
      )
    : my_buffer_0( composite_buffer_0)
    , my_buffer_1( composite_buffer_1)
    {}
 private:
      char const*
    my_buffer_0
    ;
      char const*
    my_buffer_1
    ;
};

  template
  < typename Layout
  >
struct destroy_visitor
: functor_indexed::layout_visitor<Layout>
{
        typedef
      typename functor_indexed::layout_visitor<Layout>::cases
    cases
    ;
        typedef
      typename cases::value_type
    case_type
    ;
        typedef
      void
    result_type
    ;
      template
      < case_type CaseValu
      >
    result_type operator()( mpl::integral_c<case_type,CaseValu> index) const 
    {
        Layout::scanned::destroyer(index,my_buffer);
    }
    
    destroy_visitor
      ( char* composite_buffer
      )
    : my_buffer(composite_buffer)
    {}
 private:
      char*
    my_buffer
    ;
};

//]functor_indexed_usage

}//exit detail_one_of_maybe namespace
}//exit composite_storage namespace
}//exit boost namespace

#include <boost/composite_storage/layout/operators_one_of_maybe.hpp>
#include <boost/composite_storage/layout/operators_all_of_aligned.hpp>
#include <boost/composite_storage/pack/layout_composite.hpp>
#include <boost/mpl/if.hpp>
#include <boost/composite_storage/pack/container_fwd.hpp>
#include <boost/composite_storage/buffers/char_buf.hpp>
#include <boost/fusion/support/pair.hpp>

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
  < tags::one_of_maybe
  , Index0
  , Components...
  >
: layout_composite
  < tags::one_of_maybe
  , Index0
  , Components...
  >
{
        typedef
      layout_composite
      < tags::one_of_maybe
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
        typename
      layout::operators
      < tags::all_of_aligned
      >::template push_back
      < scanned
      , index_base
      >::type
    scanned_index
    ;
        typedef
      typename layout_comp::index_undefined
    index_undefined
    ;
        typedef
      typename layout_comp::index_type
    index_type
    ;
 private:
        typedef
      buffers::char_buf
      < scanned_index::comp_part::size
      , scanned_index::comp_part::align
      >
    buffer_type
    ;
      buffer_type
    buffer
    ;
      void
    destroy(void)
    {
            typedef
          detail_one_of_maybe::destroy_visitor<layout_comp> 
        visitor_type
        ;
          visitor_type 
        buffer_visitor(buffer.address());
        functor_indexed::apply(buffer_visitor,which());
    }
      void
    assign_copy( container const& from)
    {
            typedef
          detail_one_of_maybe::assign_copy_visitor<layout_comp, char const*> 
        visitor_type
        ;
          visitor_type
        buffer_visitor(buffer.address(), from.buffer.address());
        functor_indexed::apply(buffer_visitor,from.which());
        which_put(from.which());
    }  
      void
    assign_copy( container&& from)
    {
            typedef
          detail_one_of_maybe::assign_copy_visitor
          < layout_comp
          , buffers::rval_ref_buf
          >
        visitor_type
        ;
        buffers::rval_ref_buf fr_buffer(from.buffer.address());
          visitor_type
        buffer_visitor(buffer.address(), fr_buffer);
        functor_indexed::apply(buffer_visitor,from.which());
        which_put(from.which());
    }  
      void
    which_put(index_base new_which)
    {
        typename scanned_index::index_part which_index;
        index_base& result=scanned_index::project(which_index,buffer.address());
        result=new_which;
    }
 public:
      index_base
    which(void)const
    {
        typename scanned_index::index_part which_index;
        index_base const result=scanned_index::project(which_index,buffer.address());
        return result;
    }
      template
      < index_type IndexValu
      >
        static
      mpl::integral_c< index_type, IndexValu>
    our_index(void)
    {
        return mpl::integral_c< index_type, IndexValu>();
    }
      template
      < index_type IndexValu
      >
      mpl::integral_c< index_type, IndexValu>
    index(void)const
    {
        return container::our_index<IndexValu>();
    }
    container(void)
    {
        which_put(index_undefined::value);
    }
    container( container const& from)
    {
        assign_copy(from);
    }
    container( container&& from)
    {
        assign_copy(move(from));
    }
      container const&
    operator=( container const& from)
    {
        destroy();
        assign_copy(from);
        return *this;
    }   
      container const&
    operator=( container&& from)
    {
        destroy();
        assign_copy(std::move(from));
        return *this;
    }   
      template
      < index_type IndexValu
      , typename Component
      >
    container
      ( mpl::integral_c<index_type,IndexValu>
      , Component index_component
      )
    {
        mpl::integral_c<index_base,IndexValu> index;
        scanned::inject( index, buffer.address(), std::forward<Component>(index_component));
        which_put(IndexValu);
    }
      template
      < index_type IndexValu
      , typename Component
      >
    struct index_component
    : fusion::pair
      < mpl::integral_c< index_type, IndexValu>
      , Component
      >
    {
            typedef
          fusion::pair
          < mpl::integral_c< index_type, IndexValu>
          , Component
          >
        super_type
        ;
            typedef
          typename utility::remove_cv_ref
          < Component
          >::type
        comp_type
        ;
        index_component(void)
        : super_type(comp_type())
        {}
        index_component(Component a_comp)
        : super_type(a_comp)
        {}
            static
          index_component
        _(void)
        {
            return index_component();
        }
    };
      template
      < index_type IndexValu
      , typename Component
      >
    container
      ( index_component<IndexValu,Component const&> index_component
      )
    {
        mpl::integral_c<index_base,IndexValu> index;
        scanned::inject
          ( index
          , buffer.address()
          , index_component.second
          );
        which_put(IndexValu);
    }
      template
      < index_type IndexValu
      , typename Component
      >
    container
      ( index_component<IndexValu,Component&&> index_component
      )
    {
        mpl::integral_c<index_base,IndexValu> index;
        scanned::inject
          ( index
          , buffer.address()
          , std::move(index_component.second)
          );
        which_put(IndexValu);
    }
      template
      < index_type IndexValu
      , typename Component
      >
      container const&
    operator=
      ( index_component<IndexValu,Component const&> index_component
      )
    {
        destroy();
        mpl::integral_c<index_base,IndexValu> index;
        scanned::inject
          ( index
          , buffer.address()
          , index_component.second
          );
        which_put(IndexValu);
        return *this;
    }
      template
      < index_type IndexValu
      , typename Component
      >
      container const&
    operator=
      ( index_component<IndexValu,Component&&> index_component
      )
    {
        destroy();
        mpl::integral_c<index_base,IndexValu> index;
        scanned::inject
          ( index
          , buffer.address()
          , std::move(index_component.second)
          );
        which_put(IndexValu);
        return *this;
    }
    
    ~container(void)
    {
        destroy();
    }
      template
      < index_type IndexValu
      >
    struct result_type
    : mpl::if_c
      < index_base(IndexValu)==index_undefined::value
      , special_components::nothing
      , typename layout_comp::template result_type<IndexValu>::type
      >
    {
    };
    
      template
      < index_type IndexValu
      , typename TailConvertible
      >
      void
    inject
      ( TailConvertible tail_component
      )
    {
        destroy();
        mpl::integral_c<index_base,IndexValu> index;
        scanned::inject(index,buffer.address(),std::move(tail_component));
        which_put(IndexValu);
    }        
      template
      < index_type IndexValu
      >
      void
    inject
      ( void
      )
    {
        destroy();
        mpl::integral_c<index_base,IndexValu> index;
        scanned::inject_default(index,buffer.address());
        which_put(IndexValu);
    }        
      template
      < index_type IndexValu
      >
      typename result_type<IndexValu>::type const&
    project(void)const
      /**@brief
       *  Requires:
       *    IndexValue == which()
       */
    {
        mpl::integral_c<index_base,IndexValu> index;
#ifdef MULTIPLE_DISPATCH_DEBUG
        std::cout<<__FILE__<<":project-yes-const<"<<IndexValu<<">()\n";
#endif
        return scanned::project(index,buffer.address());
    }        
      template
      < index_type IndexValu
      >
      typename result_type<IndexValu>::type&
    project(void)
      /**@brief
       *  Requires:
       *    IndexValue == which()
       */
    {
        mpl::integral_c<index_base,IndexValu> index;
#ifdef MULTIPLE_DISPATCH_DEBUG
        std::cout<<__FILE__<<":project-not-const<"<<IndexValu<<">()\n";
#endif
        return scanned::project(index,buffer.address());
    }
      bool
    operator==( container const& from)const
    {
        if(which() != from.which()) return false;
            typedef
          detail_one_of_maybe::equal_visitor<layout_comp> 
        visitor_type
        ;
          visitor_type
        buffer_visitor(buffer.address(), from.buffer.address());
        bool is_equal=functor_indexed::apply(buffer_visitor,which());
        return is_equal;
    }
};

}//exit pack namespace
}//exit composite_storage namespace
}//exit boost namespace
#endif

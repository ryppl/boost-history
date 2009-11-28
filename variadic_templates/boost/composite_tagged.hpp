#ifndef BOOST_COMPOSITE_TAGGED_INCLUDED//if#BOOST_COMPOSITE_TAGGED_INCLUDED
#define BOOST_COMPOSITE_TAGGED_INCLUDED
//  (C) Copyright Larry Evans 2009.
//
//  Permission to copy, use, modify, sell and distribute this software
//  is granted provided this copyright notice appears in all copies.
//  This software is provided "as is" without express or implied
//  warranty, and with no claim as to its suitability for any purpose.
//

#include <cstddef>
#include <boost/math/common_factor_ct.hpp>
#include <boost/aligned_storage.hpp>
#include <boost/mpl/for_each.hpp>

#include <boost/mpl/fold_assoc_pack.hpp>
#include <boost/mpl/package_range_c.hpp>
#include <boost/mpl/mk_indexed_pairs.hpp>

#include <iostream>

namespace boost
{
namespace composite_tags
{
    struct one_of         
      //composite contains an instance of 'one of' template arguments.
      ; 
    struct all_of_aligned 
      //composite contains instances of 'all of' template arguments, properly 'aligned'.
      ; 
    struct all_of_packed  
      //composite contains instances of 'all of' template arguments, 'packed' together.
      ; 

};

  template
  < class CompositeTag//possibly something from namespace composite_tags.
  , class IndexType
  , typename... Components
  >
  struct
composite_tagged
;

namespace detail_composite_tagged//general
{

    typedef
  int
index_type
;
    static
  index_type const
index_undefined
=-1
;  
  template
  < index_type Index=index_undefined
  >
  struct
index_wrap
/**@brief
 *  Used as argument to overloaded functions.
 *  Instances of this type are used to select 
 *  which function to call.
 */
{
};

  template
  < std::size_t Left
  , std::size_t Right
  >
  struct
compatible_alignment
/**@brief
 *  "Metafunction" returning alignment which is compatible with the 
 *   metafunction argument alignments, Left and Right.
 *
 *  The real reason why this template was created instead of directly using
 *  static_lcm was to provide the documentation shown below which justifies
 *  static_lcm use.
 */
{
        static
      std::size_t const
    value
    = ::boost::math::static_lcm
      < (unsigned long)Left
      , (unsigned long)Right
      >::value
      //The use of static_lcm is based on the statement:
      //
      //  N is the least common multiple of all Alignments
      //
      //in paragraph 1 on page 10 of:
      //
      //  http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2007/n2165.pdf
      //
      //AFAICT, the reason lcm is used instead of just taking the
      //maximum:
      //
      //  Left>Right?Left:Right
      //
      //is to account for "Extended alignments" mentioned in paragraph 3 
      //on page 3 of the n2165 reference mentioned above.  OTOH, if only
      //"Fundamental alignments" are used (which, I assume, are the
      //"static alignments;powers of 2" in paragraph 4 on page 5 of:
      //
      //  http://www.open-std.org/JTC1/SC22/WG21/docs/papers/2006/n2140.pdf
      //
      //) then simply using max instead of lcm would work.
    ;
};

  template
  < std::size_t Size
  , std::size_t Align
  >
  struct
aligned_char_storage
: private ::boost::aligned_storage<Size,Align>
{
        typedef
      ::boost::aligned_storage<Size,Align>
    super_type
    ;
      char const*
    address(void)const
    {
        void const*p=this->super_type::address();
        return static_cast<char const*>(p);
    }
      char*
    address(void)
    {
        void*p=this->super_type::address();
        return static_cast<char*>(p);
    }
    
};
  
  template
  < class CompositeTag
  >
  struct 
layout_make
/**@brief
 *  Member template:
 *    push_back
 *  is used to calculate various "layout" traits of a composite.
 *  These traits include at least the size
 *  and possibly the offsets and alignments
 *  of the composite's components.
 *  Also a layout contains injection and projection 
 *  functions for creating components of the composite
 *  in a memory buffer and retrieving the components
 *  from that memory buffer.
 */
{
      template
      < typename HeadLayout   //layout of some Composite
      , typename TailComponent//Type to be appended to some Composite
      >
      struct      
    push_back
    /**@brief
     *  Calculate the layout of composite with component, TailComponent,
     *  appended to a Composite with layout, HeadLayout, and return
     *  layout result in push_back<HeadLayout,TailComponent>::type.
     */
    ;
      struct
    layout0
    /**@brief
     *  layout of a composite with 0 components.
     */
    ;
};

}//exit detail_composite_tagged//general

namespace detail_composite_tagged//composite_tags::one_of
{
  template
  <
  >
  struct 
layout_make
  < composite_tags::one_of
  >
{
      struct
    layout0
    {
            static
          index_type const
        index_part
        =index_undefined
        ;
            static
          std::size_t const
        size_part
        =0
        ;
            static
          std::size_t const
        align_part
        =1
        ;
            static
          void
        result(index_wrap<index_part>)
        ;
            static
          void
        inject(index_wrap<index_part> index_arg, char*buffer_composite,...)
        {
        }
            static
          void const*
        project(index_wrap<index_part> index_arg, char const*buffer_composite)
        {
            return 0;
        }
            static
          void*
        project(index_wrap<index_part> index_arg, char*buffer_composite)
        {
            return 0;
        }
    };
      template
      < typename HeadLayout
      , typename TailComponent
      >
      struct      
    push_back
    {
          struct
        type
        : private HeadLayout
        {
                static
              index_type const
            index_part
            =HeadLayout::index_part+1
            ;
                static
              std::size_t const
            size_part
            =HeadLayout::size_part+sizeof(TailComponent)
            ;
                static
              std::size_t const
            align_part
            =compatible_alignment
             < HeadLayout::align_part
             , ::boost::alignment_of<TailComponent>::value
             >::value
            ;
              using HeadLayout::
            result
            ;
                static
              TailComponent
            result(index_wrap<index_part>)
            ;
              using HeadLayout::
            inject
            ;
                static
              void
            inject
              ( index_wrap<index_part> index_arg
              , char*buffer_composite
              , TailComponent const& tail_component
              )
            {
                char*buffer_tail=buffer_composite;
                new(buffer_tail) TailComponent(tail_component);
            }
              using HeadLayout::
            project
            ;
                static
              TailComponent const*
            project(index_wrap<index_part> index_arg, char const*buffer_composite)
            {
                void const*buffer_tail=buffer_composite;
                TailComponent const*p=static_cast<TailComponent const*>(buffer_tail);
                return p;
            }
                static
              TailComponent*
            project(index_wrap<index_part> index_arg, char*buffer_composite)
            {
                void*buffer_tail=buffer_composite;
                TailComponent*p=static_cast<TailComponent*>(buffer_tail);
                return p;
            }
        };
    };
    
};

}//exit detail_composite_tagged//composite_tags::one_of

  template
  < class IndexType
  , typename... Components
  >
  struct
composite_tagged
  < composite_tags::one_of
  , IndexType
  , Components...
  >
{
        typedef
      detail_composite_tagged::layout_make<composite_tags::one_of>
    appender_type
    ;
        typedef
      detail_composite_tagged::index_type
    index_type
    ;
        typedef
      typename mpl::fold_assoc_pack
      < mpl::assoc_left
      , appender_type::template push_back
      , typename appender_type::layout0
      , Components...
      >::type
    layout_type
    ;
 private:    
      detail_composite_tagged::aligned_char_storage
      < layout_type::size_part
      , layout_type::align_part
      >
    buffer
    ;
      index_type
    my_index
    ;
 public:
      index_type
    which(void)const
    {
        return my_index;
    }
    composite_tagged(void)
    : my_index(detail_composite_tagged::index_undefined)
    {}   
      template
      < index_type Index
      >
      struct
    result_type
    {
            typedef
          decltype
          ( layout_type::result
            ( detail_composite_tagged::index_wrap<Index>()
            )
          )
        type
        ;
    };
    
      template
      < IndexType indexValu
      >
      void
    inject
      ( typename result_type<indexValu>::type const& tail_component
      )
    {
        detail_composite_tagged::index_wrap<indexValu> index;
        layout_type::inject(index,buffer.address(),tail_component);
        my_index=indexValu;
    }        
      template
      < IndexType indexValu
      >
      typename result_type<indexValu>::type const*
    project(void)const
    {
        if(indexValu == my_index)
        {
            detail_composite_tagged::index_wrap<indexValu> index;
            return layout_type::project(index,buffer.address());
        }
        else
        {
            return 0;
        }
    }        
      template
      < IndexType indexValu
      >
      typename result_type<indexValu>::type*
    project(void)
    {
        if(indexValu == my_index)
        {
            detail_composite_tagged::index_wrap<indexValu> index;
            return layout_type::project(index,buffer.address());
        }
        else
        {
            return 0;
        }
    }        
};

#if 1 //if#composite_tags::all_of_packed
namespace  detail_composite_tagged//composite_tags::all_of_packed
{
  template
  <// class CompositeTag
  >
  struct 
layout_make
  < composite_tags::all_of_packed
  >
{
      struct      
    layout0
    {
            static
          index_type const
        index_part
        =index_undefined
        ;
            static
          std::size_t const
        size_part
        =0
        ;
            static
          std::size_t
        offset(index_wrap<index_part+1>)
        {
            return size_part;
        }
            static
          void
        result(index_wrap<index_part>)
        ;
            static
          void
        inject(index_wrap<index_part> index_arg, char*buffer_composite,...)
        {
        }
            static
          void
        project(index_wrap<index_part> index_arg, char const*buffer_composite)
        {
        }
            static
          void
        project(index_wrap<index_part> index_arg, char*buffer_composite)
        {
        }
    };
      template
      < typename HeadLayout
      , typename TailComponent
      >
      struct      
    push_back
    {
          struct
        type
        : private HeadLayout
        {
                static
              index_type const
            index_part
            =HeadLayout::index_part+1
            ;
                static
              std::size_t const
            size_part
            =HeadLayout::size_part+sizeof(TailComponent)
            ;
              using HeadLayout::
            offset
            ;
                  static
              std::size_t
            offset(index_wrap<index_part+1>)
            {
                return size_part;
            }
              using HeadLayout::
            result
            ;
                static
              TailComponent
            result(index_wrap<index_part>)
            ;
              using HeadLayout::
            inject
            ;
                static
              void
            inject
              ( index_wrap<index_part> index_arg
              , char*buffer_composite
              , TailComponent const& tail_component
              )
            
            {
                void*buffer_tail=buffer_composite+HeadLayout::size_part;
                new(buffer_tail) TailComponent(tail_component);
            }
              using HeadLayout::
            project
            ;
                static
              TailComponent const&
            project(index_wrap<index_part> index_arg, char const*buffer_composite)
            {
                void const*buffer_tail=buffer_composite+HeadLayout::size_part;
                TailComponent const*p=static_cast<TailComponent const*>(buffer_tail);
                return *p;
            }
                static
              TailComponent&
            project(index_wrap<index_part> index_arg, char*buffer_composite)
            {
                void*buffer_tail=buffer_composite+HeadLayout::size_part;
                TailComponent*p=static_cast<TailComponent*>(buffer_tail);
                return *p;
            }
            
        };//end type struct
    };//end push_back struct
};//end layout_make struct
    
  template
  < typename Layout
  , typename... Components
  >
  struct
construct_all
{
        typedef typename
      mpl::package_range_c
      < int
      , 0
      , sizeof...(Components)
      >::type
    indices
    ;
        typedef typename
      mpl::mk_indexed_pairs
      < indices
      , Components...
      >::type
    ndx_pairs
    ;
      struct
    initialize_one
    {

          char*
        my_buffer
        ;
        initialize_one(char* a_buffer)
        : my_buffer(a_buffer)
        {}
          template
          < class IndexPair
          >
          void
        operator()(IndexPair)const
        {
            int const index_part=IndexPair::first::value;
            index_wrap<index_part> index;
            typedef typename IndexPair::second value_type;
            value_type value_val;
            Layout::inject(index,my_buffer,value_val);
        }
         
    };
        static
      void
    initialize_all(char*buffer)
    {
        initialize_one initializer(buffer);
        mpl::for_each<ndx_pairs>(initializer);
    };
};
  
}//exit detail_composite_tagged//composite_tags::all_of_packed

  template
  < class IndexType
  , typename... Components
  >
  struct
composite_tagged
  < composite_tags::all_of_packed
  , IndexType
  , Components...
  >
{
        typedef
      detail_composite_tagged::layout_make<composite_tags::all_of_packed>
    appender_type
    ;
        typedef
      detail_composite_tagged::index_type
    index_type
    ;
        typedef
      typename mpl::fold_assoc_pack
      < mpl::assoc_left
      , appender_type::template push_back
      , typename appender_type::layout0
      , Components...
      >::type
    layout_type
    ;
 private:
      char
    buffer[layout_type::size_part]
    ;
 public:
    composite_tagged(void)
    {
        detail_composite_tagged::construct_all<layout_type,Components...>::initialize_all(buffer);
    }
 
      template
      < index_type IndexValu
      >
      struct
    result_type
    {
            typedef
          decltype
          ( layout_type::result
            ( detail_composite_tagged::index_wrap<IndexValu>()
            )
          )
        type
        ;
    };
      template
      < IndexType IndexValu
      >
      typename result_type<IndexValu>::type const&
    project(void)const
    {
        detail_composite_tagged::index_wrap<IndexValu> index;
        return layout_type::project(index,buffer);
    }        
      template
      < IndexType IndexValu
      >
      typename result_type<IndexValu>::type &
    project(void)
    {
        detail_composite_tagged::index_wrap<IndexValu> index;
        return layout_type::project(index,buffer);
    }        
};
#endif //endif#composite_tags::all_of_packed

#if 1 //if#composite_tags::all_of_aligned
namespace detail_composite_tagged//composite_tags::all_of_aligned
{
  template
  <// class CompositeTag
  >
  struct 
layout_make
  < composite_tags::all_of_aligned
  >
{
      template
      < std::size_t Offset
      , std::size_t Alignment
      >
      struct
    next_aligned_offset
    {
            static
          std::size_t const
        remainder
        = Offset%Alignment
        ;
            static
          std::size_t const
        value
        = remainder == 0
        ? Offset
        : Offset+(Alignment-remainder)
        //value is minimum value > Offset such that
        //value%Alignment == 0
        ;
        
    };
  
      struct      
    layout0
    {
            static
          index_type const
        index_part
        =index_undefined
        ;
            static
          std::size_t const
        size_part
        =0
        ;
            static
          std::size_t const
        align_part
        =1
        ;
            static
          std::size_t
        offset(index_wrap<index_part+1>)
        {
            return size_part;
        }
            static
          void
        result(index_wrap<index_part>)
        ;
            static
          void
        inject(index_wrap<index_part> index_arg, char*buffer_composite,...)
        {
        }
            static
          void
        project(index_wrap<index_part> index_arg, char const*buffer_composite)
        {
        }
            static
          void
        project(index_wrap<index_part> index_arg, char*buffer_composite)
        {
        }
    };
    
      template
      < typename HeadLayout
      , typename TailComponent
      >
      struct      
    push_back
    {
          struct
        type
        : private HeadLayout
        {
                static
              index_type const
            index_part
            =HeadLayout::index_part+1
            ;
                static
              std::size_t const
            align_one
            = ::boost::alignment_of<TailComponent>::value
            ;
                static
              std::size_t const
            align_part
            = compatible_alignment
              < HeadLayout::align_part
              , align_one
              >::value
            ;
                static
              std::size_t const
            size_part
            = next_aligned_offset
              <   HeadLayout::size_part 
                + sizeof(TailComponent)
              , align_one
              >::value
            ;
            
              using HeadLayout::
            offset
            ;
                  static
              std::size_t
            offset(index_wrap<index_part+1>)
            {
                return size_part;
            }
              using HeadLayout::
            result
            ;
                static
              TailComponent
            result(index_wrap<index_part>)
            ;
              using HeadLayout::
            inject
            ;
                static
              void
            inject
              ( index_wrap<index_part> index_arg
              , char*buffer_composite
              , TailComponent const& tail_component
              )
            {
                void*buffer_tail=buffer_composite+HeadLayout::size_part;
                new(buffer_tail) TailComponent(tail_component);
            }
              using HeadLayout::
            project
            ;
                static
              TailComponent const&
            project(index_wrap<index_part> index_arg, char const*buffer_composite)
            {
                void const*buffer_tail=buffer_composite+HeadLayout::size_part;
                TailComponent const*p=static_cast<TailComponent const*>(buffer_tail);
                return *p;
            }
                static
              TailComponent&
            project(index_wrap<index_part> index_arg, char*buffer_composite)
            {
                void*buffer_tail=buffer_composite+HeadLayout::size_part;
                TailComponent*p=static_cast<TailComponent*>(buffer_tail);
                return *p;
            }
            
        };//end type struct
    };//end push_back struct
};//end layout_make struct
  
}//exit detail_composite_tagged namespace//composite_tags::all_of_aligned

  template
  < class IndexType
  , typename... Components
  >
  struct
composite_tagged
  < composite_tags::all_of_aligned
  , IndexType
  , Components...
  >
{
        typedef
      detail_composite_tagged::layout_make<composite_tags::all_of_aligned>
    appender_type
    ;
        typedef
      detail_composite_tagged::index_type
    index_type
    ;
        typedef
      typename mpl::fold_assoc_pack
      < mpl::assoc_left
      , appender_type::template push_back
      , typename appender_type::layout0
      , Components...
      >::type
    layout_type
    ;
 private:
      detail_composite_tagged::aligned_char_storage
      < layout_type::size_part
      , layout_type::align_part
      >
    buffer
    ;
 public:
    composite_tagged(void)
    {
        char*memory=buffer.address();
        detail_composite_tagged::construct_all<layout_type,Components...>::initialize_all(memory);
    }
 
      template
      < index_type Index
      >
      struct
    result_type
    {
            typedef
          decltype
          ( layout_type::result
            ( detail_composite_tagged::index_wrap<Index>()
            )
          )
        type
        ;
    };
      template
      < IndexType IndexValu
      >
      typename result_type<IndexValu>::type const&
    project(void)const
    {
        detail_composite_tagged::index_wrap<IndexValu> index;
        return layout_type::project(index,buffer.address());
    }        
      template
      < IndexType IndexValu
      >
      typename result_type<IndexValu>::type &
    project(void)
    {
        detail_composite_tagged::index_wrap<IndexValu> index;
        return layout_type::project(index,buffer.address());
    }        
};
#endif //endif#composite_tags::all_of_aligned

}//exit boost namespace
#endif //endif#BOOST_COMPOSITE_TAGGED_INCLUDED

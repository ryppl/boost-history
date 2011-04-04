//
#ifndef BOOST_COMPOSITE_STORAGE_INDEXED_CTOR_ARGS_ALL_OF_ALIGNED_HPP_INCLUDED
#define BOOST_COMPOSITE_STORAGE_INDEXED_CTOR_ARGS_ALL_OF_ALIGNED_HPP_INCLUDED
//  (C) Copyright Larry Evans 2011.
//
//  Permission to copy, use, modify, sell and distribute this software
//  is granted provided this copyright notice appears in all copies.
//  This software is provided "as is" without express or implied
//  warranty, and with no claim as to its suitability for any purpose.
//====================================================================
#include <boost/composite_storage/pack/ctor_protected_all_of_aligned.hpp>
#include <boost/composite_storage/index_component.hpp>

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
indexed_ctor_args_all_of_aligned
: ctor_protected
  < tags::all_of_aligned
  , Index0
  , Components...
  >
  /**@brief
   *  container all_of_aligned with
   *  constructor which allows initialization
   *  of selective components using 
   *  non-default and possibly non-copy constructor.
   */
{

        typedef
      ctor_protected
      < tags::all_of_aligned
      , Index0
      , Components...
      >
    ctor_prot
    ;
        typedef
      typename ctor_prot::methods_all
    methods_all
    ;
        typedef
      typename ctor_prot::index_type
    index_type
    ;
        typedef
      typename ctor_prot::index_base
    index_base
    ;
        typedef
      typename ctor_prot::layout_indices
    layout_indices
    ;
 public:
    indexed_ctor_args_all_of_aligned(void)
    {
        char*to_buf=this->address();
        methods_all::ctor_default_all(to_buf);
    }
    indexed_ctor_args_all_of_aligned( indexed_ctor_args_all_of_aligned& from)
    {
        char      *to_buf=this->address();
        char const*fr_buf=from.address();
          methods_all::
        ctor_copy_all
        ( to_buf
        , fr_buf
        );
    }
    indexed_ctor_args_all_of_aligned( indexed_ctor_args_all_of_aligned&& from)
    {
        char      *to_buf=this->address();
        char      *fr_buf=from.address();
          methods_all::
        ctor_copy_all
        ( to_buf
        , fr_buf
        );
    }
    
      template
      < typename... Args
      >
    struct args_ftor
    ;
      template
      < index_type... Keys
      , typename... CtorTypes
      >
    struct args_ftor
      < index_component<index_type,Keys,CtorTypes>...
      >
    : index_component<index_type,Keys,CtorTypes>...
    /**brief
     *  A functor for injecting every component in to
     *  the composite when used with mpl::for_each.
     */
    {
     private:
          char*
        my_buffer
          /**@brief
           * the composite's buffer for storing its components.
           */
          ;
          
          struct 
        not_in_args
          /**@brief
           *  "Flag" type signaling one of the index_type's
           *  was not found in Keys...
           */
          {};
          
          template
          < index_type Key
          , typename CtorType
          >
            static
          index_component<index_type,Key,CtorType>const*
        get_our
          ( index_component<index_type,Key,CtorType>const* a_arg
          )
          /**@brief
           *  Returns the argument associated with key, Key, and
           *  passed to the CTOR and stored in one of the superclasses.
           */
          {
            return a_arg;
          }
            
          template
          < index_type Key
          >
            static
          not_in_args*
        get_our
          ( ...
          )
          /**@brief
           *  NO argument with Key was passed to the CTOR. 
           *  Signal that with special result type.
           */
          {
            return 0;
          }
            
          template
          < index_type Key
          , typename CtorType
          >
          void
        inject
          ( index_component<index_type,Key,CtorType>const* a_arg
          )const
          /**@brief
           *  a_arg was passed to CTOR with key==Key.
           *  Inject it into my_buffer (also passed to CTOR).
           */
          {
              mpl::integral_c<index_base,index_base(Key)> a_index;
              layout_indices::inject(a_index,my_buffer,a_arg->my_comp);
          }
          
          template
          < index_type Key
          >
          void
        inject
          ( not_in_args*
          )const
          /**@brief
           *  No arg passed to CTOR wkty key==Key.
           *  Inject default value into my_buffer (also passed to CTOR).
           */
          {
              mpl::integral_c<index_base,index_base(Key)> a_index;
              layout_indices::inject_default(a_index,my_buffer);
          }
          
     public:
          template
          < index_base IndexValue
          >
          void
        operator()
          ( mpl::integral_c<index_base,IndexValue> a_index
          )const
          /**@brief
           *  If the arg with IndexValue was pass to CTOR,
           *    inject that into my_buffer at IndexValue;
           *  Otherwise:
           *    inject the default value into my_buffer at IndexValue.
           */
          {
              index_type const index_value=index_type(IndexValue);
              auto arg_ptr=get_our<index_value>(this);
              inject<index_value>(arg_ptr);
          }
          
        args_ftor
          ( char* this_buffer //The buffer into which ctor_args are to be injected.
          ,   index_component
              < index_type
              , Keys
              , CtorTypes
              >const&...
            ctor_args //the args to be injected into this_buffer at given key in Keys.
          )
          : index_component<index_type,Keys,CtorTypes>(ctor_args)...
          , my_buffer(this_buffer)
          {}
    };
    
      template
      < index_type... Keys
      , typename... CtorTypes
      >
    indexed_ctor_args_all_of_aligned
      (   index_component
          < index_type
          , Keys
          , CtorTypes
          >const&...
        ctor_args
      )
      {
            args_ftor
            < index_component
              < index_type
              , Keys
              , CtorTypes
              >...
            >
          f_args
            ( this->address()
            , ctor_args...
            )
          ;
          mpl::for_each<typename layout_indices::indices>(f_args);
      }
};

}//exit pack namespace
}//exit composite_storage namespace
}//exit boost namespace

#endif

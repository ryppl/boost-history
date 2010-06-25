//
#ifndef BOOST_COMPOSITE_STORAGE_PACK_MULTIPLE_DISPATCH_REIFY_APPLY_HPP_INCLUDED
#define BOOST_COMPOSITE_STORAGE_PACK_MULTIPLE_DISPATCH_REIFY_APPLY_HPP_INCLUDED
//  (C) Copyright Larry Evans 2010.
//
//  Permission to copy, use, modify, sell and distribute this software
//  is granted provided this copyright notice appears in all copies.
//  This software is provided "as is" without express or implied
//  warranty, and with no claim as to its suitability for any purpose.
//====================================================================
#include <boost/mpl/assert.hpp>
#include <boost/composite_storage/pack/multiple_dispatch/replace_source_with_target_ptr.hpp>
#include <boost/composite_storage/pack/multiple_dispatch/apply_unpack.hpp>

namespace boost
{
namespace composite_storage
{
namespace pack
{
namespace multiple_dispatch
{
  template
  < template
    < typename ReifyApply //this type
    , typename ArgsConcreteAbstract //container concrete followed by abstract args.
    >class Reifier //converter from abstract to concrete arg.
  , typename Functor //Functor on concrete args.
  >
struct reify_apply_impl
  /**@brief
   *  A functor, whose operator()(ArgsConcreteAbstract*p_args),
   *  recursively applies Reifier to each "abstract" argument in *p_args,
   *  accummulating the resulting "concrete" arguments in same p_args, 
   *  until no more abstract arguments are available, and then
   *  applies Functor to the accummulated "concrete" arguments.
   */
{
    typedef typename Functor::result_type result_type;
    typedef reify_apply_impl<Reifier,Functor> this_type;
    Functor& my_functor;
    reify_apply_impl(Functor& a_functor)
    : my_functor(a_functor)
    {}
      template
      < typename ArgsConcrete
      , typename HeadAbstract
      , typename... TailAbstract
      >
      result_type 
    operator()
      ( ptrs_target_source
        < ArgsConcrete
        , HeadAbstract
        , TailAbstract...
        >* tar_src_p
      )const
      /**@brief
       *  Applies conversion, Reifier, to HeadAbstract argument
       *  in *tar_src_p, converting it to a concrete argument
       *  appended to the end of ArgsConcrete, which replaces
       *  the HeadAbstract argument.
       */
    {
            typedef
          ptrs_target_source
          < ArgsConcrete
          , HeadAbstract
          , TailAbstract...
          >
        ptrs_tar_src_type;
          Reifier
          < this_type
          , ptrs_tar_src_type
          >
        reifier
          ( *this
          , tar_src_p
          );
        return reifier();
    }

      template
      < typename... ArgsConcrete
      >
      result_type 
    operator()
      ( ptrs_target_source
        < mpl::package
          < ArgsConcrete...
          >
        >* tar_src_p
      )const
      /**@brief
       *  When there are no more abstract arguments
       *  ( presumably after a number of calls to
       *    the previous member function.
       *  ) apply my_functor to *tar_src_p.
       */
    {
            apply_unpack
            < typename mpl::package_range_c
              < unsigned
              , 0
              , sizeof...(ArgsConcrete)
              >::type
            >
          uapp;
          return uapp(my_functor,*tar_src_p);
    }

};    

  template
  < template
    < typename ReifyApply
    , typename ArgsConcreteAbstract
    >class Reifier
  , typename Functor
  , typename... ArgsAbstract
  >
  typename Functor::result_type
reify_apply
  ( Functor & a_functor
  , ArgsAbstract const&... a_args_abstract
  )
  /**@brief
   *  Applies a Reifier to each "abstract" argument in
   *  a_args_abstract... to produce a container of 
   *  concrete arguments, which are then passed to a_functor.
   */  
{
      auto
    ptrs_target_src_v(mk_ptrs_source(a_args_abstract...))
    //creates container of pointers to a_args_abstract...
    ;
    reify_apply_impl<Reifier,Functor> rai(a_functor); 
    return rai(&ptrs_target_src_v);
}

}//exit namespace multiple_dispatch
}//exit namespace pack

}//exit composite_storage namespace
}//exit boost namespace
#endif

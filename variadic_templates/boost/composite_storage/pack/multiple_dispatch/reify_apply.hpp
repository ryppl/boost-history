//
#ifndef BOOST_COMPOSITE_STORAGE_PACK_MULTIPLE_DISPATCH_REIFY_APPLY_HPP_INCLUDED
#define BOOST_COMPOSITE_STORAGE_PACK_MULTIPLE_DISPATCH_REIFY_APPLY_HPP_INCLUDED
//#include <boost/mpl/assert.hpp>
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
    < typename ReifyApply
    , typename ArgsConcreteAbstract
    >class Reifier
  , typename Functor
  >
struct reify_apply_impl
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
        #ifdef MULTIPLE_DISPATCH_DEBUG
          std::cout
            <<"package<ArgsConcrete...>="
            <<utility::demangled_type_name<mpl::package<ArgsConcrete...> >()
            <<"\n";
          return result_type();
        #else
            apply_unpack
            < typename mpl::package_range_c
              < unsigned
              , 0
              , sizeof...(ArgsConcrete)
              >::type
            >
          uapp;
          return uapp(my_functor,*tar_src_p);
        #endif
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
  , ArgsAbstract&&... a_args_abstract
  )
  /**@brief
   *  Applies Reifier to each argument in a_args_abstract 
   *  to produce a list of concrete arguments, which 
   *  are then passed to a_functor.
   */  
{
      typename ptrs_target0_source
      < ArgsAbstract...
      >::type
    ptrs_target_src_v(a_args_abstract...)
    //creates container of void pointers to a_args_abstract...
    ;
    reify_apply_impl<Reifier,Functor> rai(a_functor); 
  #ifdef MULTIPLE_DISPATCH_DEBUG
    std::cout
      <<"reify_apply::ptrs_target_src_v="
      <<utility::demangled_type_name(ptrs_target_src_v)
      <<"\n";
  #endif
    return rai(&ptrs_target_src_v);
}

}//exit namespace multiple_dispatch
}//exit namespace pack

}//exit composite_storage namespace
}//exit boost namespace
#endif

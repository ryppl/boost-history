//
#ifndef BOOST_COMPOSITE_STORAGE_PACK_MULTIPLE_DISPATCH_APPLY_UNPACK_HPP_INCLUDED
#define BOOST_COMPOSITE_STORAGE_PACK_MULTIPLE_DISPATCH_APPLY_UNPACK_HPP_INCLUDED
//  (C) Copyright Larry Evans 2010.
//
//  Permission to copy, use, modify, sell and distribute this software
//  is granted provided this copyright notice appears in all copies.
//  This software is provided "as is" without express or implied
//  warranty, and with no claim as to its suitability for any purpose.
//

#include <boost/mpl/package_range_c.hpp>
#include <boost/type_traits/remove_cv.hpp>
#include <boost/function_types/can_be_called.hpp>
#include <exception>

namespace boost
{
namespace composite_storage
{
namespace pack
{
namespace multiple_dispatch
{
  template
  < typename Functor
  >
struct functor_result_type
{
        typedef
      typename remove_cv<Functor>::type
    functor_type
    ;
        typedef
      typename functor_type::result_type
    type
    ;
};  

  template
  < typename Functor
  , typename... Args
  >
  typename functor_result_type<Functor>::type
apply_ftor_callable_args
  ( mpl::bool_<true>
  , Functor& ftor
  , Args const&... args
  )
{
    return ftor(args...);
};

  template<typename FunctorArgs>
struct bad_functor_args
  : public std::exception
{
    virtual const char * what() const throw()
    {
        return "boost::composite_storage::pack::multiple_dispatch::bad_functor_args: "
               "Functor not defined for Args.";
    }
};    
  template
  < typename Functor
  , typename... Args
  >
  typename functor_result_type<Functor>::type
apply_ftor_callable_args
  ( mpl::bool_<false>
  , Functor& ftor
  , Args const&... args
  )
{
    throw bad_functor_args<Functor(Args const&...)>();
    return typename functor_result_type<Functor>::type();
};

  template
  < typename Functor
  , typename... Args
  >
  typename functor_result_type<Functor>::type
apply_ftor_check_args
  ( Functor& ftor
  , Args const&... args
  )
  /**@brief
   *  If ftor is callable with args..., does so.
   *  If not, then issues error message and returns
   *  default value.
   */
{
        typedef
      typename function_types::can_be_called<Functor(Args const&...)>::type
    is_ftor_args_callable;
    return apply_ftor_callable_args
      ( is_ftor_args_callable()
      , ftor
      , args...
      );
};

  template
  < typename Indices
  >
struct apply_unpack
;
  template
  < unsigned... Indices
      //0...Arity-1, where Arity is arity of Functor arg to this->operator()(_,_).
  >
struct apply_unpack
  < mpl::package_c<unsigned, Indices...>
  >
{
      template
      < typename Functor
      , typename ArgsPacked
      >
      typename functor_result_type<Functor>::type
    operator()
      ( Functor& a_functor
      , ArgsPacked const& a_args
      )
    {
        return 
        #ifndef APPLY_UNPACK_DEMO_UNCHECK_ARGS
          apply_ftor_check_args
          ( a_functor
          , a_args.template project<Indices>()...
          )
        #else
          //This branch of #if...#endif can cause compile errors
          //about "no match for for call to 'SomeFunCall'"
          //where SomdFunCall is some function name and
          //parameter type list.
          a_functor
          ( a_args.template project<Indices>()...
          )
        #endif
          ;
    }
};

}//exit multiple_dispatch namespace
}//exit pack namespace
}//exit composite_storage namespace
}//exit boost namespace
#endif

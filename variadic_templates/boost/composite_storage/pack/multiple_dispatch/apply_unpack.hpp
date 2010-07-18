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
#include <exception>
#ifndef APPLY_UNPACK_USER_CHECKED_ARGS
  #include <boost/function_types/can_be_called.hpp>
#endif  

namespace boost
{
namespace composite_storage
{
namespace pack
{
namespace multiple_dispatch
{
  template <typename Functor>
  struct functor_result_type
  {
      typedef typename remove_cv<Functor>::type functor_type;
      typedef typename functor_type::result_type type;
  };  
  
  template<typename FunctorArgs>
  struct bad_functor_args
    : public std::exception
  {
      virtual const char * what() const throw()
      {
          return "boost::composite_storage::pack::multiple_dispatch"
                 "::bad_functor_args:\n"
                 "      Functor not defined for Args.";
      }
  };
      
#ifdef APPLY_UNPACK_USER_CHECKED_ARGS
    template
    < typename Functor
    , typename ResultType
    >
  struct functor_bad_args
  {
      typedef ResultType result_type;
      
      template<typename... Args>
      result_type operator()(Args&... args) throw()
      {
          throw bad_functor_args<Functor(Args&...)>();
          return result_type();
      }
      template<typename... Args>
      result_type operator()(Args&... args)const throw()
      {
          throw bad_functor_args<Functor const(Args&...)>();
          return result_type();
      }
  }; 

#else
  template <typename Functor, typename... Args>
    typename functor_result_type<Functor>::type
  apply_ftor_callable_args( mpl::bool_<true>, Functor& ftor
    , Args const&... args)
  {
      return ftor(args...);
  };
  
  template <typename Functor, typename... Args>
    typename functor_result_type<Functor>::type
  apply_ftor_callable_args( mpl::bool_<false>, Functor& ftor
    , Args const&... args
    )
  {
      throw bad_functor_args<Functor(Args const&...)>();
      return typename functor_result_type<Functor>::type();
  };
  
  template <typename Functor, typename... Args>
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
      typedef typename function_types
        ::can_be_called<Functor(Args const&...)>::type
      is_ftor_args_callable;
      return apply_ftor_callable_args( is_ftor_args_callable()
        , ftor, args...);
  };
#endif  
  
  template <typename Indices>
  struct apply_unpack;
  
  template <unsigned... Indices>
  struct apply_unpack< mpl::package_c<unsigned, Indices...> >
  {
      template <typename Functor, typename ArgsPacked>
        typename functor_result_type<Functor>::type
      operator()( Functor& a_functor, ArgsPacked const& a_args)
      {
          return 
          #ifndef APPLY_UNPACK_USER_CHECKED_ARGS
            apply_ftor_check_args( a_functor
              , a_args.template project<Indices>()...);
          #else
            //This branch of #if...#endif may cause compile errors
            //about "no match for for call to 'SomeFunCall'"
            //where SomeFunCall is some function name and
            //parameter type list.
            //
            //To avoid these errors, derive Functor from
            //functor_bad_args *and* put a using declaration
            //in functor:
            //
            //   using super_type::operator();
            //
            //where super_type is typedef'ed to the functor_bad_args
            //super type of Functor.
            //
            a_functor( a_args.template project<Indices>()...);
          #endif
      }
  };

}//exit multiple_dispatch namespace
}//exit pack namespace
}//exit composite_storage namespace
}//exit boost namespace
#endif

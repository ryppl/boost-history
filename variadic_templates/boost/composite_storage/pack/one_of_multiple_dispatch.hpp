//
#ifndef BOOST_COMPOSITE_STORAGE_ONE_OF_MULTIPLE_DISPATCH_HPP_INCLUDED
#define BOOST_COMPOSITE_STORAGE_ONE_OF_MULTIPLE_DISPATCH_HPP_INCLUDED
//  (C) Copyright Larry Evans 2010.
//
//  Permission to copy, use, modify, sell and distribute this software
//  is granted provided this copyright notice appears in all copies.
//  This software is provided "as is" without express or implied
//  warranty, and with no claim as to its suitability for any purpose.
//====================================================================
#include <boost/assert.hpp>
#include <boost/mpl/assert.hpp>
#include <boost/composite_storage/pack/apply_unpack.hpp>

namespace boost
{
namespace composite_storage
{
namespace pack
{
namespace multiple_dispatch
{
//Starting from:
//  [apply_dispatched
//to:
//  ]apply_dispatched
//the code (except for the line containing arg_ptrs_buf_value)
//coped from:
//  /home/evansl/prog_dev/boost-svn/ro/sandbox/variadic_templates
//    /libs/composite_storage/sandbox/apply_dispatched.cpp.~1.23~:148-442.

//[apply_dispatched
  template
  < typename Functor
  , typename ArgsConcrete
  , typename ArgsAbstract
  >
      typename 
    Functor
  ::result_type 
apply_dispatched
  ( Functor & a_functor
  , ArgsConcrete const& a_args_concrete
  , ArgsAbstract const& a_args_abstract
  )
;
  template
  < typename Functor
  , typename ArgsConcrete
  , typename HeadAbstract
  , typename ArgsAbstract
  >
struct apply_vec
;
  template
  < typename Functor
  , typename ArgsConcrete
  , typename HeadAbstract
  , typename... ArgsAbstract
  >
struct apply_vec
  < Functor
  , ArgsConcrete
  , HeadAbstract
  , arg_ptrs<args_abstract,ArgsAbstract...>
  >
{
    Functor & my_functor;
    ArgsConcrete const& my_args_concrete;
    HeadAbstract const& my_head_abstract;
    typedef arg_ptrs<args_abstract,ArgsAbstract...> args_abstract_t;
    args_abstract_t const& my_args_abstract;
    apply_vec
      ( Functor & a_functor
      , ArgsConcrete const& a_args_concrete
      , HeadAbstract const& a_head_abstract
      , args_abstract_t const& a_args_abstract
      )
    : my_functor(a_functor)
    , my_args_concrete(a_args_concrete)
    , my_head_abstract(a_head_abstract)
    , my_args_abstract(a_args_abstract)
    {}
        typedef
      typename Functor::result_type
    result_type
    ;
        typedef
      typename HeadAbstract::index_base
    index_base
    ;
      template
      < index_base IndexBaseValue
      >
      result_type
    at_index_compile_time
      ( void
      )
    {
        BOOST_ASSERT((my_head_abstract.which() == IndexBaseValue));
        typedef typename HeadAbstract::index_type index_type;
        index_type const index_value=index_type(IndexBaseValue);
        typedef typename HeadAbstract::template result_type<index_value>::type head_concrete_t;
        head_concrete_t const*head_concrete_p=my_head_abstract.template project<index_value>();
        BOOST_ASSERT((head_concrete_p != (head_concrete_t const*)0));
        typedef typename args_push<ArgsConcrete,head_concrete_t>::type next_args_concrete_t;
        next_args_concrete_t next_args_concrete_v(my_args_concrete,head_concrete_p);
        BOOST_MPL_ASSERT_RELATION(next_args_concrete_t::size,==,ArgsConcrete::size+1);
        return apply_dispatched
          ( my_functor
          , next_args_concrete_v
          , my_args_abstract
          );
    }
        typedef
      result_type
    (apply_vec::*conc_fun)
      ( void
      )
    ;
        static
      unsigned const
    size_conc
    =HeadAbstract::index_end::value
    -HeadAbstract::index_undefined::value
    ;
        typedef
      conc_fun
    vec_fun[size_conc]
    ;
      template
      < index_base... Indices
      >
        static
      vec_fun const&
    at_indices_fun
      ( void
      )
    {
            static 
          vec_fun const
        v=
        { &apply_vec::at_index_compile_time<Indices>...
        };
        return v;
    }
      template
      < typename Indices
      >
    struct at_indices_fwd
    ;
      template
      < index_base... Indices
      >
    struct at_indices_fwd
      < mpl::package_c<index_base,Indices...>
      >
    {
            static
          vec_fun const&
        _(void)
        {
            return apply_vec::at_indices_fun<Indices...>();
        }
    };
      result_type
    at_index_run_time
      ( typename HeadAbstract::index_base a_index
      )
    {
            typedef 
          typename mpl::package_range_c
          < index_base
          , HeadAbstract::index_undefined::value
          , HeadAbstract::index_end::value
          >::type 
        indices;
        typedef at_indices_fwd<indices> fwd;
        unsigned const index_unsigned=a_index-HeadAbstract::index_undefined::value;
        conc_fun at_fun=fwd::_()[index_unsigned];
        return (this->*at_fun)();
    }
    
};

  template
  < typename Functor
  , typename ArgsConcrete
  , typename HeadAbstract
  , typename... ArgsAbstract
  >
      typename 
    Functor
  ::result_type 
apply_dispatched
  ( Functor & a_functor
  , ArgsConcrete const& a_args_concrete
  , arg_ptrs<args_abstract,HeadAbstract,ArgsAbstract...> const& a_args_abstract
  )
{
    typedef arg_ptrs<args_abstract,ArgsAbstract...> next_args_abstract_t;
    HeadAbstract const&a_head_abstract=a_args_abstract.get_head();
    typename HeadAbstract::index_base index_value=a_head_abstract.which();
      apply_vec
      < Functor
      , ArgsConcrete
      , HeadAbstract
      , next_args_abstract_t
      >
    av
      ( a_functor
      , a_args_concrete
      , a_head_abstract
      , a_args_abstract.get_tail()
      );
    return av.at_index_run_time(index_value);
}

  template
  < typename Functor
  , typename ArgsConcrete
  >
      typename 
    Functor
  ::result_type 
apply_dispatched
  ( Functor & a_functor
  , ArgsConcrete const& a_args_concrete
  , arg_ptrs<args_abstract> const& a_args_abstract
  )
{
        apply_unpack
        < typename boost::mpl::package_range_c
          < unsigned
          , 0
          , ArgsConcrete::size
          >::type
        >
      uapp
      ;
      return uapp(a_functor,a_args_concrete);
}

  template
  < typename Functor
  , typename... ArgsAbstract
  >
  typename Functor::result_type
apply_abstract
  ( Functor & a_functor
  , ArgsAbstract const&... a_args_abstract
  )
{
    unsigned const size_args=sizeof...(ArgsAbstract);
    arg_ptr_type arg_ptrs_buf_concrete[size_args];
    arg_ptrs<args_concrete> arg_ptrs_concrete(arg_ptrs_buf_concrete);
    arg_ptr_type arg_ptrs_buf_abstract[size_args];
    arg_ptrs<args_abstract,ArgsAbstract...> arg_ptrs_abstract(arg_ptrs_buf_abstract, a_args_abstract...);
    BOOST_MPL_ASSERT_RELATION((arg_ptrs<args_abstract,ArgsAbstract...>::size),==,size_args);
    return apply_dispatched
      ( a_functor
      , arg_ptrs_concrete
      , arg_ptrs_abstract
      );
}

  template
  < template
    < typename ReifyApply
    , typename ArgsConcrete
    , typename HeadAbstract
    , typename ArgsAbstract
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
      , typename ArgsAbstract
      >
      result_type 
    operator()
      ( ArgsConcrete const& a_args_concrete
      , ArgsAbstract const& a_args_abstract
      )const
    {
          Reifier
          < this_type
          , ArgsConcrete
          , typename ArgsAbstract::head_type
          , typename ArgsAbstract::tail_type
          >
        reifier
          ( *this
          , a_args_concrete
          , a_args_abstract.get_head()
          , a_args_abstract.get_tail()
          );
        return reifier();
    }

      template
      < typename ArgsConcrete
      >
      result_type 
    operator()
      ( ArgsConcrete const& a_args_concrete
      , arg_ptrs<args_abstract> const& a_args_abstract
      )const
    {
            apply_unpack
            < typename boost::mpl::package_range_c
              < unsigned
              , 0
              , ArgsConcrete::size
              >::type
            >
          uapp
          ;
          return uapp(my_functor,a_args_concrete);
    }

};    

  template
  < template
    < typename ReifyApply
    , typename ArgsConcrete
    , typename HeadAbstract
    , typename ArgsAbstract
    >class Reifier
  , typename Functor
  , typename... ArgsAbstract
  >
  typename Functor::result_type
reify_apply
  ( Functor & a_functor
  , ArgsAbstract const&... a_args_abstract
  )
{
    unsigned const size_args=sizeof...(ArgsAbstract);
    
    arg_ptr_type arg_ptrs_buf_concrete[size_args];
    typedef arg_ptrs<args_concrete> arg_ptrs_concrete_t;
    arg_ptrs_concrete_t arg_ptrs_concrete_v(arg_ptrs_buf_concrete);
    
    arg_ptr_type arg_ptrs_buf_abstract[size_args];
    typedef arg_ptrs<args_abstract,ArgsAbstract...> arg_ptrs_abstract_t;
    arg_ptrs_abstract_t arg_ptrs_abstract_v(arg_ptrs_buf_abstract, a_args_abstract...);
    
    BOOST_MPL_ASSERT_RELATION(arg_ptrs_abstract_t::size,==,size_args);
   
    reify_apply_impl<Reifier,Functor> ra(a_functor); 
    return ra(arg_ptrs_concrete_v,arg_ptrs_abstract_v);
}
//]apply_dispatched

}//exit namespace multiple_dispatch
}//exit namespace pack

}//exit composite_storage namespace
}//exit boost namespace
#endif

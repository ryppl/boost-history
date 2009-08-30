//variadic template version of unpack_args.hpp
#ifndef BOOST_MPL_UNPACK_ARGS_HPP_VARIADIC_TEMPLATES
#define BOOST_MPL_UNPACK_ARGS_HPP_VARIADIC_TEMPLATES
#include <boost/mpl/apply.hpp>
#include <boost/mpl/at.hpp>
#include <boost/mpl/size.hpp>
#include <boost/mpl/aux_/lambda_spec.hpp>
#include <boost/mpl/package_range_c.hpp>

namespace boost { namespace mpl {
namespace aux {

  template
  < typename Indices
  , typename F
  , typename Args
  >
struct unpack_args_impl;
;
  template
  < long... Indices
  , typename F
  , typename Args
  >
struct unpack_args_impl
  < package_c<long,Indices...>
  , F
  , Args
  > 
: apply
  < F
  , typename at_c<Args,Indices>::type...
  >
{
};

}

  template
  < typename F
  >
struct unpack_args
{
      template
      < typename Args 
      > 
    struct apply
    : aux::unpack_args_impl
      < typename package_range_c
        < long
        , long(0)
        , long(size<Args>::value) 
        >::type
      , F
      , Args
      >
    {
    };
};

BOOST_MPL_AUX_PASS_THROUGH_LAMBDA_SPEC(1, unpack_args)

}}

#endif

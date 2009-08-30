#ifndef BOOST_MPL_APPLY_HPP_VARIADIC_TEMPLATES
#define BOOST_MPL_APPLY_HPP_VARIADIC_TEMPLATES
#include <boost/mpl/apply_wrap.hpp>
#include <boost/mpl/placeholders.hpp>
//MAINTENANCE_NOTE: 2007-01-29: Larry Evans
//  Since nothing from placeholders.hpp is used below,
//  there's no apparent need to #include it above.
//  However, some other files (e.g. mpl/aux_/insert_impl.hpp)
//  use it and are included (in some instances) after
//  this one (e.g. buy mpl/aux_/reverse_fold_impl.hpp) so,
//  the easiest workaround is to #include it here although
//  a better one would be to just included it where needed. 
#include <boost/mpl/lambda.hpp>
namespace boost
{
namespace mpl
{
      template
      < class F
      , typename... Args
      >
      struct
    apply
      : apply_wrap
        < typename lambda<F>::type
        , Args...
        >
    {
    };
    
      template
      < class F
      , typename Arg0
      >
      struct
    apply1
      : apply
        < F
        , Arg0
        >
    {
    };
    
      template
      < class F
      , typename Arg0
      , typename Arg1
      >
      struct
    apply2
      : apply
        < F
        , Arg0
        , Arg1
        >
    {
    };
    
}//exit mpl namespace
}//exit boost namespace
#endif

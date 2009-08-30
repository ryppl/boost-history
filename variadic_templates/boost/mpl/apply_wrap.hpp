#ifndef BOOST_MPL_APPLY_WRAP_HPP_VARIADIC_TEMPLATES
#define BOOST_MPL_APPLY_WRAP_HPP_VARIADIC_TEMPLATES
#include <boost/mpl/aux_/has_apply.hpp>
//http://www.boost.org/libs/mpl/doc/refmanual/apply-wrap.html
//@@@TODO: Rm the following #include after all preprocessing includes
//  (i.e. those in 
//     <boost/mpl/CONTAINER/aux_/preprocessed/...
//   where CONTAINER is, .e.g. list or vector or...
//  )
//  have been replaced with non-preprocessed versions.
#include <boost/mpl/aux_/config/use_preprocessed.hpp>

namespace boost
{
namespace mpl
{
      template
      < class F
      , typename... Args
      >
      struct
    apply_wrap
    {
            typedef
          typename F::template apply<Args...>::type
        type
        ;
    };
    namespace detail_apply_wrap
    {
          template
          < class F
          , typename has_apply_ = typename aux::has_apply<F>::type
          >
          struct
        apply_wrap0
          : F::template apply<>
        {
        };
        
          template
          < class F
          >
          struct
        apply_wrap0
          < F
          , true_
          >
          : F::apply
        {
        };
        
    }//exit detail_apply_wrap
      template
      < class F
      >
      struct
    apply_wrap
      < F
      >
      : detail_apply_wrap::apply_wrap0<F>
    {
    };
    
}//exit mpl namespace
}//exit boost namespace
#endif

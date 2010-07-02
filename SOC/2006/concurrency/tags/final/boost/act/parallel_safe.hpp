//  Copyright (c) 2006, Matthew Calabrese
//
//  This code may be used under either of the following two licences:
//
//    Permission is hereby granted, free of charge, to any person
//    obtaining a copy of this software and associated documentation
//    files (the "Software"), to deal in the Software without
//    restriction, including without limitation the rights to use,
//    copy, modify, merge, publish, distribute, sublicense, and/or
//    sell copies of the Software, and to permit persons to whom the
//    Software is furnished to do so, subject to the following
//    conditions:
//
//    The above copyright notice and this permission notice shall be
//    included in all copies or substantial portions of the Software.
//
//    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
//    EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
//    OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
//    NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
//    HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
//    WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
//    FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
//    OTHER DEALINGS IN THE SOFTWARE. OF SUCH DAMAGE.
//
//  Or:
//
//    Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//    http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_ACT_PARALLEL_SAFE_HPP
#define BOOST_ACT_PARALLEL_SAFE_HPP

#include <boost/mpl/if.hpp>
#include <boost/mpl/empty_base.hpp>
#include <boost/mpl/apply.hpp>
#include <boost/mpl/always.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/type_traits/is_function.hpp>
#include <boost/type_traits/remove_pointer.hpp>
#include <boost/type_traits/function_traits.hpp>

#include <boost/preprocessor/repetition/enum_params.hpp>
#include <boost/preprocessor/repetition/enum_binary_params.hpp>
#include <boost/preprocessor/repetition/repeat_from_to.hpp>

#include "config/max_active_call_params.hpp"

namespace boost
{
namespace act
{

namespace detail
{
  template< typename FunType
          , typename Enabler = void
          >
  struct void_or_result_type
  {
    typedef void type;
  };

  template< typename FunType >
  struct void_or_result_type< FunType
                            , typename ::boost::mpl::apply
                              <
                                ::boost::mpl::always< void >
                              , typename FunType::result_type
                              >
                              ::type
                            >
  {
    typedef typename FunType::result_type type;
  };

  template< typename FunType >
  struct void_or_result_type
         <
           FunType
         , typename ::boost::enable_if
           <
             ::boost::is_function
             <
               typename ::boost::remove_pointer< FunType >::type
             >
           >
           ::type
         >
  {
  private:
    typedef typename ::boost::remove_pointer< FunType >::type fun_type;
  public:
    typedef typename ::boost::function_traits< fun_type >::result_type type;
  };
}

struct parallel_safe
{
};

template< bool Condition >
struct parallel_safe_if_c
  : ::boost::mpl::if_c< Condition
                      , parallel_safe
                      , ::boost::mpl::empty_base
                      >
                      ::type
{
};

template< typename Condition >
struct parallel_safe_if
  : parallel_safe_if_c< Condition::value >
{
};

template< typename FunType
        , typename ResultType
            = typename detail::void_or_result_type< FunType >::type
        >
class parallel_safe_fun
  : parallel_safe
{
public:
  typedef ResultType result_type;

  explicit parallel_safe_fun( FunType const& fun_init )
    : fun_m( fun_init )
  {
  }

  result_type operator ()()
  {
    return fun_m();
  } 

  result_type operator ()() const
  {
    return fun_m();
  }

#define BOOST_ACT_DETAIL_PARALLEL_SAFE_FUN_OPERATOR( z, num_params, qualifier )\
  template< BOOST_PP_ENUM_PARAMS_Z( z, num_params, typename Param ) >          \
  result_type operator ()                                                      \
    ( BOOST_PP_ENUM_BINARY_PARAMS_Z( z, num_params, Param, & arg ) ) qualifier \
  {                                                                            \
    return fun_m( BOOST_PP_ENUM_PARAMS_Z( z, num_params, arg ) );              \
  } 

#define BOOST_ACT_DETAIL_NO_QUALIFIER 

  BOOST_PP_REPEAT_FROM_TO( 1, BOOST_ACT_MAX_ACTIVE_CALL_PARAMS
                         , BOOST_ACT_DETAIL_PARALLEL_SAFE_FUN_OPERATOR
                         , BOOST_ACT_DETAIL_NO_QUALIFIER
                         )

  BOOST_PP_REPEAT_FROM_TO( 1, BOOST_ACT_MAX_ACTIVE_CALL_PARAMS
                         , BOOST_ACT_DETAIL_PARALLEL_SAFE_FUN_OPERATOR
                         , const
                         )
private:
  FunType fun_m;
};

// ToDo: Make compatible with lambda
//       Optimize away copies (change param type, special case for fun refs)
template< typename FunType >
parallel_safe_fun< FunType > make_parallel_safe_fun( FunType fun )
{
  return parallel_safe_fun< FunType >( fun );
}

// ToDo: Make compatible with lambda
//       Optimize away copies (change param type, special case for fun refs)
template< typename ResultType, typename FunType >
parallel_safe_fun< FunType, ResultType > make_parallel_safe_fun( FunType fun )
{
  return parallel_safe_fun< FunType, ResultType >( fun );
}

// ToDo: Make compatible with lambda
//       Optimize away copies (change param type, special case for fun refs)
template< bool IsSafe, typename FunType >
typename ::boost::enable_if_c
<
  IsSafe
, parallel_safe_fun< FunType >
>
make_parallel_safe_fun_if_c( FunType fun )
{
  return parallel_safe_fun< FunType >( fun );
}

// ToDo: Make compatible with lambda
//       Optimize away copies (change param type, special case for fun refs)
template< bool IsSafe, typename FunType >
typename ::boost::disable_if_c
<
  IsSafe
, FunType
>
make_parallel_safe_fun_if_c( FunType fun, void** dummy = 0 ) // Dummy for GCC workaround
{
  return fun;
}

// ToDo: Make compatible with lambda
//       Optimize away copies (change param type, special case for fun refs)
template< typename IsSafe, typename FunType >
typename ::boost::enable_if
<
  IsSafe
, parallel_safe_fun< FunType >
>
make_parallel_safe_fun_if( FunType fun )
{
  return parallel_safe_fun< FunType >( fun );
}

// ToDo: Make compatible with lambda
//       Optimize away copies (change param type, special case for fun refs)
template< typename IsSafe, typename FunType >
typename ::boost::disable_if
<
  IsSafe
, FunType
>
make_parallel_safe_fun_if( FunType fun, void** dummy = 0 ) // Dummy for GCC workaround
{
  return fun;
}

}
}

#endif
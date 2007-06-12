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

#ifndef BOOST_ACT_ACTION_ACTION_NO_DEFAULT_HPP
#define BOOST_ACT_ACTION_ACTION_NO_DEFAULT_HPP

#include <boost/operators.hpp>
#include <boost/preprocessor/repetition/enum_trailing_params.hpp>
#include <boost/tuple/tuple.hpp>
#include <boost/preprocessor/repetition/enum_params.hpp>
#include <boost/mpl/deque.hpp>
#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/repetition/enum.hpp>
#include <boost/preprocessor/repetition/repeat.hpp>
#include <boost/preprocessor/repetition/enum_trailing_binary_params.hpp>

#include "../act_fwd.hpp"
#include "../active/active_no_default.hpp"
#include "../config/max_active_call_params.hpp"
#include "../type_traits/remove_active.hpp"

#include "../detail/active_copier.hpp"
#include "../detail/params_to_stored_params.hpp"
#include "../detail/tuple_from_sequence.hpp"
#include "../detail/tie.hpp"
#include "../detail/active_function_caller.hpp"
#include "../detail/queued_function_caller.hpp"

#include "../detail/as_target.hpp"
#include "../detail/as_function_impl.hpp"

#include "../detail/function_package.hpp"

namespace boost
{
namespace act
{

// ToDo: Make target object type for storing a ref to an active object

// ToDo: Make copy constructor
template< typename RepresentedType
        , typename ActModel
        >
class action
  : ActModel::template action_impl< RepresentedType >
  , public dereferenceable // ToDo: Make not public
     <
       action< RepresentedType, ActModel >
     , typename add_active< RepresentedType, ActModel >::type*
     >
{
  template< typename OtherRepresentedType
          , typename OtherActModel
          >
  friend class action;
private:
  typedef typename ActModel::template action_impl
            < RepresentedType > impl_type;
private:
  typedef typename add_active< RepresentedType, ActModel >::type active_type;
public: // ToDo: Make private
  impl_type const& impl() const
  {
    return *this;
  }

  impl_type& impl()
  {
    return *this;
  }
public:
  action()
  {
  }
public:
  action( action const& source )
    : impl_type( source.impl()
               , detail::make_function_package< RepresentedType, ActModel
                                              , mpl::deque<>
                                              >
                   ( detail::active_copier< RepresentedType >(), detail::tie() )
               )
  {
  }
public:
  template< typename SourceType >
  action( SourceType const& source
        , typename enable_if
          <
            mpl::and_
            <
              is_active< SourceType const >
            , is_convertible
              <
                typename remove_active< SourceType const >::type
              , RepresentedType
              >
            >
          >
          ::type** dummy = 0
        )
    : impl_type( source.impl()
               , detail::make_function_package< RepresentedType, ActModel
                                              , mpl::deque<>
                                              >
                   ( detail::active_copier< RepresentedType >(), detail::tie() )
               )
  {
  }
public:
#define BOOST_ACT_DETAIL_ACTION_CONSTRUCTOR( z, num_params, dummy )            \
  template< typename FunctionType                                              \
            BOOST_PP_ENUM_TRAILING_PARAMS_Z( z, num_params, typename Param )   \
          >                                                                    \
  explicit action                                                              \
  (                                                                            \
    detail::as_function_impl_type< FunctionType > function                     \
    BOOST_PP_ENUM_TRAILING_BINARY_PARAMS_Z( z, num_params, Param, const& arg ) \
  )                                                                            \
    : impl_type( detail::make_function_package                                 \
                 <                                                             \
                   RepresentedType, ActModel                                   \
                 , mpl::deque                                                  \
                     < BOOST_PP_ENUM_PARAMS_Z( z, num_params, const Param ) >  \
                 >                                                             \
                 (                                                             \
                   function.target                                             \
                 , detail::tie( BOOST_PP_ENUM_PARAMS_Z( z, num_params, arg ) ) \
                 )                                                             \
               )                                                               \
  {                                                                            \
  }

  BOOST_PP_REPEAT( BOOST_ACT_MAX_ACTIVE_CALL_PARAMS
                 , BOOST_ACT_DETAIL_ACTION_CONSTRUCTOR
                 , BOOST_PP_NIL
                 )

  #undef BOOST_ACT_DETAIL_ACTION_CONSTRUCTOR

#define BOOST_ACT_DETAIL_ACTION_QUEUED_CONSTRUCTOR( z, num_params, dummy )     \
  template< typename ObjectType                                                \
          , typename FunctionType                                              \
            BOOST_PP_ENUM_TRAILING_PARAMS_Z( z, num_params, typename Param )   \
          >                                                                    \
  action                                                                       \
  (                                                                            \
    detail::as_target_impl_type< ObjectType > target                           \
  , FunctionType const& function                                               \
    BOOST_PP_ENUM_TRAILING_BINARY_PARAMS_Z( z, num_params, Param, const& arg ) \
  )                                                                            \
    : impl_type( target.target.impl()                                          \
               , detail::make_function_package                                 \
                 <                                                             \
                   RepresentedType, ActModel                                   \
                 , mpl::deque                                                  \
                     < BOOST_PP_ENUM_PARAMS_Z( z, num_params, const Param ) >  \
                 >                                                             \
                 (                                                             \
                   function                                                    \
                 , detail::tie( BOOST_PP_ENUM_PARAMS_Z( z, num_params, arg ) ) \
                 )                                                             \
               )                                                               \
  {                                                                            \
  }

  BOOST_PP_REPEAT( BOOST_ACT_MAX_ACTIVE_CALL_PARAMS
                 , BOOST_ACT_DETAIL_ACTION_QUEUED_CONSTRUCTOR
                 , BOOST_PP_NIL
                 )

  #undef BOOST_ACT_DETAIL_ACTION_QUEUED_CONSTRUCTOR

public:
  bool is_complete() const
  {
    return impl_type::is_complete();
  }

  void wait() const
  {
    impl_type::wait();
  }

  active_type& operator *() const
  {
    return impl().active_object();
  }

  using dereferenceable
        <
          action< RepresentedType, ActModel >
        , active_type*
        >
        ::operator ->;
};

}
}

#endif

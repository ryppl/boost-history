//  Copyright (c) 2006, Matthew Calabrese
//  All rights reserved.
//  Distributed under the New BSD License
//  (See accompanying file ACT_BSD_LICENSE.txt)

#ifndef BOOST_ACT_ACTION_ACTION_HPP
#define BOOST_ACT_ACTION_ACTION_HPP

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
#include "../config/max_active_call_params.hpp"
#include "../type_traits/remove_active.hpp"

#include "../detail/active_ref.hpp"
#include "../detail/active_copier.hpp"
#include "../detail/params_to_stored_params.hpp"
#include "../detail/tuple_from_sequence.hpp"
#include "../detail/tie.hpp"
#include "../detail/active_function_caller.hpp"
#include "../detail/queued_function_caller.hpp"

#include "../detail/as_target.hpp"
#include "../detail/as_function_impl.hpp"

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
  , public detail::active_ref< action< RepresentedType, ActModel > // ToDo: Make not public
                      , RepresentedType
                      , ActModel
                      >
  , public ::boost::dereferenceable // ToDo: Make not public
     <
       action< RepresentedType, ActModel >
     , detail::active_ref< action< RepresentedType, ActModel >
                         , RepresentedType
                         , ActModel
                         >*
     >
{
  template< typename OtherRepresentedType
          , typename OtherActModel
          >
  friend class action;
private:
  typedef typename ActModel::template action_impl
            < RepresentedType > impl_type;

  typedef detail::active_ref< action< RepresentedType, ActModel >
                            , RepresentedType
                            , ActModel
                            >
                            active_ref_base_type;
private:
  template< typename ParamSequence
          , typename FunctionType
          , typename TupleType
          >
  class function_package
  {
  public:
    typedef RepresentedType result_type;
  public:
    function_package( FunctionType const& function_init
                    , TupleType const& arguments_init
                    )
      : function_m( function_init )
      , arguments_m( arguments_init )
    {
    }
  public:
    class stored_function_type
    {
    public:
      typedef RepresentedType result_type;
    private:
      typedef typename detail::params_to_stored_params
                < ActModel, ParamSequence >::type
                  stored_params_sequence;

      typedef typename detail::tuple_from_sequence
                < stored_params_sequence >::type
                  stored_params_type;
    public:
      stored_function_type( FunctionType const& function_init
                          , TupleType const& arguments_init
                          )
        : function_m( function_init )
        , arguments_m( arguments_init )
      {
      }
    public:
      RepresentedType operator ()() const
      {
        return detail::active_function_caller< result_type >
                 ( function_m, arguments_m );
      }

      template< typename TargetType >
      RepresentedType operator ()( TargetType& target ) const
      {
        return detail::queued_function_caller< result_type >
                 ( target, function_m, arguments_m );
      }
    private:
      FunctionType function_m;
      stored_params_type arguments_m;
    };
  public:
    stored_function_type store_function() const
    {
      return stored_function_type( function_m, arguments_m );
    }
  private:
    FunctionType const& function_m;
    TupleType const& arguments_m;
  };

  template< typename ParamSequence
          , typename FunctionType
          , typename ArgumentsType
          >
  static function_package< ParamSequence, FunctionType, ArgumentsType >
  make_function_package( FunctionType const& function
                       , ArgumentsType const& arguments
                       )
  {
    return function_package< ParamSequence, FunctionType, ArgumentsType >
             ( function, arguments );
  }
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
               , make_function_package< ::boost::mpl::deque<> >
                   ( detail::active_copier< RepresentedType >(), detail::tie() )
               )
  {
  }
public:
  template< typename SourceType >
  action( SourceType const& source
        , typename ::boost::enable_if
          <
            ::boost::mpl::and_
            <
              is_active< SourceType const >
            , ::boost::is_convertible
              <
                typename remove_active< SourceType const >::type
              , RepresentedType
              >
            >
          >
          ::type** dummy = 0
        )
    : impl_type( source.impl()
               , make_function_package< ::boost::mpl::deque<> >
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
    : impl_type( make_function_package                                         \
                 <                                                             \
                   ::boost::mpl::deque                                         \
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
               , make_function_package                                         \
                 <                                                             \
                   ::boost::mpl::deque                                         \
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
public:
  bool is_complete() const
  {
    return impl_type::is_complete();
  }

  void wait() const
  {
    impl_type::wait();
  }

  active_ref_base_type& operator *() const
  {
    return const_cast< active_ref_base_type& >
             ( static_cast< active_ref_base_type const& >( *this ) );
  }

  using ::boost::dereferenceable
        <
          action< RepresentedType, ActModel >
        , active_ref_base_type*
        >
        ::operator ->;
};

}
}

#include "../config/default_act_model.hpp"

#endif

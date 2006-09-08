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

#ifndef BOOST_ACT_DETAIL_COMPLEX_RESULT_OF_HPP
#define BOOST_ACT_DETAIL_COMPLEX_RESULT_OF_HPP

namespace boost
{
namespace act
{
namespace detail
{

template< typename Type >
struct static_encapsulator
{
  static Type instance;
};

struct invalid_result_of;

template< typename Helper, typename FunctionType, typename Enabler = void >
struct complex_result_of_impl;

#define BOOST_ACT_DETAIL_STATIC_ENCAPSULATOR_INSTANCE( z, index, fun_type )    \
static_encapsulator< BOOST_PP_CAT                                              \
                     ( BOOST_PP_CAT( function_traits< fun_type >::arg          \
                                   , BOOST_PP_INC( index )                     \
                                   )                                           \
                     , _type                                                   \
                     )                                                         \
                   >                                                           \
                   ::instance

#define BOOST_ACT_DETAIL_COMPLEX_RESULT_OF_MACRO( z, fun_index, num_params )   \
+ static_cast< ::std::size_t >                                                 \
(                                                                              \
     sizeof( complex_result_of_impl::BOOST_PP_CAT( fun, fun_index )            \
             ( BOOST_PP_CAT( BOOST_PP_ENUM_, z )                               \
               ( num_params                                                    \
               , BOOST_ACT_DETAIL_STATIC_ENCAPSULATOR_INSTANCE                 \
               , ParamSequence                                                 \
               )                                                               \
             )                                                                 \
           )                                                                   \
  == complex_result_of_impl::is_callable                                       \
)

#define BOOST_ACT_DETAIL_COMPLEX_T_RESULT_OF_MACRO( z, fun_index, num_params ) \
+ static_cast< ::std::size_t >                                                 \
(                                                                              \
     sizeof( complex_result_of_impl::BOOST_PP_CAT( fun_template, fun_index )   \
             ( BOOST_PP_CAT( BOOST_PP_ENUM_, z )                               \
               ( num_params                                                    \
               , BOOST_ACT_DETAIL_STATIC_ENCAPSULATOR_INSTANCE                 \
               , ParamSequence                                                 \
               )                                                               \
             )                                                                 \
           )                                                                   \
  == complex_result_of_impl::is_callable                                       \
)

#define BOOST_ACT_COMPLEX_RAW_RESULT_OF_SPEC( z, num_params, dummy )           \
template< typename Helper, typename FunctionType >                             \
struct complex_raw_result_of_impl                                              \
       <                                                                       \
         Helper, FunctionType                                                  \
       , typename enable_if                                                    \
         < mpl::equal_to                                                       \
           <                                                                   \
             mpl::size_t< function_traits< FunctionType >::arity >             \
           , mpl::size_t< num_params >                                         \
           >                                                                   \
         >                                                                     \
         ::type                                                                \
       >                                                                       \
{                                                                              \
private:                                                                       \
  typedef typename function_traits< FunctionType >::result_type function_type; \
                                                                               \
  typedef typename give_cv_to< function_type, Helper >                         \
            ::type qualified_helper_type;                                      \
private:                                                                       \
  static ::std::size_t const overload_index                                    \
           = sizeof( static_encapsulator< qualified_helper_type >::instance.fun\
                       BOOST_PP_CAT( BOOST_PP_ENUM_, z )                       \
                       ( num_params                                            \
                       , BOOST_ACT_DETAIL_STATIC_ENCAPSULATOR_INSTANCE         \
                       , FunctionType                                          \
                       )                                                       \
                   ) - 1;                                                      \
public:                                                                        \
  typedef typename mpl::at_c< typename Helper::result_sequence                 \
                            , overload_index                                   \
                            >                                                  \
                            ::type type;                                       \
};

template< typename Helper, typename FunctionType >
struct complex_result_of_impl
  : mpl::if_
    <
      is_same
      <
        typename complex_raw_result_of_impl
        <
          detail_result_helper
        , FunctionType
        >
        ::type
      , invalid_result_of
      >
    , mpl::empty_base
    , complex_raw_result_of_impl
      <
        detail_result_helper
      , FunctionType
      >
    >
    ::type
{
  BOOST_MPL_ASSERT_MSG
  ( ( is_same
      <
        typename complex_raw_result_of_impl
        <
          detail_result_helper
        , FunctionType
        >
        ::type
      , invalid_result_of
      >
      ::value
    )
  , INVALID_PARAMETERS_FOR_FUNCTION_CALL
  , ()
  )
};

/*
template< typename Base, typename ParamSequence >
struct complex_result_of_impl
       <
         Base, ParamSequence
       , typename enable_if< mpl::equal_to
                             <
                               mpl::size< ParamSequence >
                             , mpl::size_t< num_params >
                             >
                           >
                           ::type
       >
{
private:
  static ::std::size_t const num_matches
    = 0 BOOST_PP_CAT( BOOST_PP_REPEAT_, z )
        (
          num_params
        , BOOST_ACT_DETAIL_COMPLEX_RESULT_OF_MACRO
        , num_params
        );
public:
  // ToDo: Assert if index is 0
  typedef typename mpl::at_c
          <
            typename complex_result_of_impl::return_type_sequence
          , overload_index
          >
          ::type type;
};
*/
}
}
}

#define BOOST_ACT_DETAIL_IS_AMBIGUOUS_RESULT( name, num, overloads )           \
struct BOOST_PP_CAT( BOOST_PP_CAT( detail_, name ), _base )                    \
{                                                                              \
  static ::std::size_t const num_overloads = num                               \
                             invalid_result_of_index = 1,                      \
                             is_uncallable = 1,                                \
                             is_callable = 2;                                  \
                                                                               \
  static char (&fun(...))[invalid_result_of_index];                            \
  static char (&fun_template(...))[invalid_result_of_index];                   \
                                                                               \
  /* ToDo: use num+2 or invalid_result_of_index depending on template or not */\
  static char (&BOOST_PP_CAT(fun,num)(...))[is_uncallable];\
  static char (&BOOST_PP_CAT(fun,num)(/* ToDo: Params here */))[is_callable];\
\
  static char (&BOOST_PP_CAT(fun_template,num)(...))[is_uncallable];\
  static char (&BOOST_PP_CAT(fun_template,num)(/* ToDo: Params here */))[is_callable];\
\
  /* ToDo: Put non-template overloads here, different size result for each */\
  /* ToDo: Put template overloads here, different size result for each */\
\
  /* ToDo: Make sequence of result types */\
};

#define BOOST_ACT_DETAIL_DECLARE_RESULT_HELPER_FUN( z, index, overloads )      \
template< BOOST_ACT_DETAIL_ENUM_PLIST(BOOST_ACT_SIMPLE_PLIST(t_params)) >      \
static char (&fun( BOOST_ACT_DETAIL_ENUM_PLIST                                 \
                  ( BOOST_ACT_SIMPLE_PLIST( params ) )                         \
                 ) qualifier                                                   \
            )[index+2];

#define BOOST_ACT_DETAIL_RESULT( num, overloads )                              \
template< typename FunctionType >                                              \
struct result                                                                  \
{                                                                              \
private:                                                                       \
  struct helper                                                                \
  {                                                                            \
    static ::std::size_t const invalid_result_of_index = 1,                    \
                                                                               \
    static char (&fun(...))[invalid_result_of_index];                          \
                                                                               \
    BOOST_PP_REPEAT( num                                                       \
                   , BOOST_ACT_DETAIL_DECLARE_RESULT_HELPER_FUN                \
                   , overloads                                                 \
                   )                                                           \
                                                                               \
    typedef ::boost::mpl::deque< ::boost::act::detail::invalid_result_of       \
                                 BOOST_PP_ENUM_TRAILING                        \
                                 ( num                                         \
                                 , BOOST_ACT_DETAIL_RETURN_FROM_OVERLOAD       \
                                 , overloads                                   \
                                 )                                             \
                               >                                               \
                               result_sequence;                                \
  };                                                                           \
public:                                                                        \
  typedef typename ::boost::act::detail::complex_result_of_impl< helper        \
                                                               , FunctionType  \
                                                               >               \
                                                               ::type          \
};

};


#endif

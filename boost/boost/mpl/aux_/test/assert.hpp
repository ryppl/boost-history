
#ifndef BOOST_MPL_AUX_TEST_ASSERT_HPP_INCLUDED
#define BOOST_MPL_AUX_TEST_ASSERT_HPP_INCLUDED

// Copyright (c) Aleksey Gurtovoy 2002-2004
//
// Distributed under the Boost Software License, Version 1.0. 
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/mpl for documentation.

// $Source$
// $Date$
// $Revision$

#include <boost/mpl/not.hpp>
#include <boost/mpl/long.hpp>
#include <boost/mpl/aux_/yes_no.hpp>
#include <boost/mpl/aux_/na.hpp>
#include <boost/mpl/aux_/value_wknd.hpp>
#include <boost/mpl/aux_/adl_barrier.hpp>
#include <boost/mpl/aux_/config/ctps.hpp>
#include <boost/mpl/aux_/config/msvc.hpp>
#include <boost/mpl/aux_/config/gcc.hpp>
#include <boost/mpl/aux_/config/workaround.hpp>

#include <boost/preprocessor/tuple/rem.hpp>
#include <boost/preprocessor/control/expr_if.hpp>
#include <boost/preprocessor/logical/not.hpp>
#include <boost/preprocessor/cat.hpp>

#if BOOST_WORKAROUND(__BORLANDC__, < 0x600) || (BOOST_MPL_CFG_GCC != 0)
#   define BOOST_MPL_CFG_ASSERT_USE_RELATION_NAMES
#endif



//#define MPL_ASSERT( expr ) BOOST_STATIC_ASSERT( expr )
#define MPL_ASSERT_EQUAL(arity, tuple) assert_equal< BOOST_PP_TUPLE_REM(arity)tuple >()
#define MPL_ASSERT_NOT_EQUAL(arity, tuple) assert_not_equal< BOOST_PP_TUPLE_REM(arity)tuple >()
#define MPL_ASSERT_SAME(arity, tuple) assert_same< BOOST_PP_TUPLE_REM(arity)tuple >()
#define MPL_ASSERT_NOT_SAME(arity, tuple) assert_not_same< BOOST_PP_TUPLE_REM(arity)tuple >()

#if !defined(BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION)
#   define AUX_ASSERT_DEF( param_type, suffix, test_equality ) \
template< param_type x, param_type y > struct assert_##suffix \
BOOST_PP_EXPR_IF( BOOST_PP_NOT(test_equality), {} ); \
template< param_type x > struct assert_##suffix<x,x> \
BOOST_PP_EXPR_IF( test_equality, {} ); \
/**/
#elif BOOST_WORKAROUND(BOOST_MSVC, <= 1300)
#   define AUX_ASSERT_DEF( param_type, suffix, test_equality ) \
template< param_type x > struct assert_##suffix##_impl \
{ \
    template< param_type y > struct result_; \
    template<> struct result_<x> \
    { BOOST_PP_EXPR_IF( BOOST_PP_NOT(test_equality), private: virtual ~result_<x>() = 0; ) }; \
    template< param_type y > struct result_ \
    { BOOST_PP_EXPR_IF( test_equality, private: virtual ~result_<y>() = 0;  ) }; \
}; \
\
template< param_type x, param_type y > struct assert_##suffix \
    : assert_##suffix##_impl<x>::template result_<y> {}; \
/**/
#else
#   define AUX_ASSERT_DEF( param_type, suffix, test_equality ) \
template< param_type x > struct assert_##suffix##_impl \
{ \
    template< param_type y > struct result_ \
    BOOST_PP_EXPR_IF( BOOST_PP_NOT(test_equality), {} ); \
}; \
template< param_type x > struct assert_##suffix##_impl<x>::result_<x> \
BOOST_PP_EXPR_IF( test_equality, {} ); \
\
template< param_type x, param_type y > struct assert_##suffix \
    : assert_##suffix##_impl<x>::template result_<y> {}; \
/**/
#endif

namespace {
AUX_ASSERT_DEF(long, equal, 1)
AUX_ASSERT_DEF(long, not_equal, 0)
AUX_ASSERT_DEF(typename, same, 1)
AUX_ASSERT_DEF(typename, not_same, 0)
}

#undef AUX_ASSERT_DEF







BOOST_MPL_AUX_ADL_BARRIER_NAMESPACE_OPEN

struct failed {};

template< bool C >  struct assert        { typedef void* type; };
template<>          struct assert<false> { typedef assert type; };

template< bool C >
int assertion_failed( typename assert<C>::type );

struct assert_
{
    template< typename T1, typename T2 = na, typename T3 = na, typename T4 = na > struct types {};
    enum relations { arg, equal, not_equal, greater, greater_equal, less, less_equal };
};


#if !defined(BOOST_MPL_CFG_ASSERT_USE_RELATION_NAMES)

#   if !BOOST_WORKAROUND(BOOST_MSVC, <= 1300) && !BOOST_WORKAROUND(__MWERKS__, BOOST_TESTED_AT(0x3003))
template< long x, long y > bool operator==(long_<x>, long_<y>);
template< long x, long y > bool operator!=(long_<x>, long_<y>);
template< long x, long y > bool operator>(long_<x>, long_<y>);
template< long x, long y > bool operator>=(long_<x>, long_<y>);
template< long x, long y > bool operator<(long_<x>, long_<y>);
template< long x, long y > bool operator<=(long_<x>, long_<y>);

template< long x, long y, bool (*)(long_<x>, long_<y>) > struct assert_relation {};
#   else
bool operator==(failed, failed);
bool operator!=(failed, failed);
bool operator>(failed, failed);
bool operator>=(failed, failed);
bool operator<(failed, failed);
bool operator<=(failed, failed);

template< long x, long y, bool (*)(failed, failed) > struct assert_relation {};
#   endif

#else // BOOST_MPL_CFG_ASSERT_USE_RELATION_NAMES

aux::weighted_tag<1>::type operator==(assert_::relations, assert_::relations);
aux::weighted_tag<2>::type operator!=(assert_::relations, assert_::relations);
aux::weighted_tag<3>::type operator>(assert_::relations, assert_::relations);
aux::weighted_tag<4>::type operator>=(assert_::relations, assert_::relations);
aux::weighted_tag<5>::type operator<(assert_::relations, assert_::relations);
aux::weighted_tag<6>::type operator<=(assert_::relations, assert_::relations);

template< assert_::relations r, long x, long y > struct assert_relation {};

#endif 


#if !BOOST_WORKAROUND(__MWERKS__, BOOST_TESTED_AT(0x3003)) \
    && !BOOST_WORKAROUND(__BORLANDC__, < 0x600)
template< bool > struct assert_arg_pred_impl { typedef int type; };
template<> struct assert_arg_pred_impl<true> { typedef void* type; };

template< typename P > struct assert_arg_pred
{
    typedef typename P::type p_type;
    typedef typename assert_arg_pred_impl< p_type::value >::type type;
};

template< typename P > struct assert_arg_pred_not
{
    typedef typename P::type p_type;
    enum { p = !p_type::value };
    typedef typename assert_arg_pred_impl<p>::type type;
};

template< typename Pred >
failed ************ (Pred::************ 
      assert_arg( void (*)(Pred), typename assert_arg_pred<Pred>::type )
    );

template< typename Pred >
failed ************ (boost::mpl::not_<Pred>::************ 
      assert_not_arg( void (*)(Pred), typename assert_arg_pred_not<Pred>::type )
    );

template< typename Pred >
assert<false>
assert_arg( void (*)(Pred), typename assert_arg_pred_not<Pred>::type );

template< typename Pred >
assert<false>
assert_not_arg( void (*)(Pred), typename assert_arg_pred<Pred>::type );


#else // BOOST_WORKAROUND(__MWERKS__, BOOST_TESTED_AT(0x3003)) 
    //  || BOOST_WORKAROUND(__BORLANDC__, < 0x600)
        
template< bool c, typename Pred > struct assert_arg_type_impl
{
    typedef failed      ************ Pred::* mwcw83_wknd;
    typedef mwcw83_wknd ************* type;
};

template< typename Pred > struct assert_arg_type_impl<true,Pred>
{
    typedef assert<false> type;
};

template< typename Pred > struct assert_arg_type
    : assert_arg_type_impl<BOOST_MPL_AUX_VALUE_WKND(Pred::type)::value,Pred>
{
};

template< typename Pred >
typename assert_arg_type<Pred>::type 
assert_arg(void (*)(Pred), int);

template< typename Pred >
typename assert_arg_type< boost::mpl::not_<Pred> >::type 
assert_not_arg(void (*)(Pred), int);

template< long x, long y, bool (*r)(failed, failed) >
typename assert_arg_type_impl< false,assert_relation<x,y,r> >::type
assert_rel_arg( assert_relation<x,y,r> );

#endif 

BOOST_MPL_AUX_ADL_BARRIER_NAMESPACE_CLOSE


// MPL_ASSERT((pred<x,...>))

#define MPL_ASSERT(pred) \
enum { \
    BOOST_PP_CAT(mpl_assertion_in_line_,__LINE__) = sizeof( \
          boost::mpl::assertion_failed<false>( \
              boost::mpl::assert_arg( (void (*) pred)0, 1 ) \
            ) \
        ) \
}\
/**/

// MPL_ASSERT_NOT((pred<x,...>))

#define MPL_ASSERT_NOT(pred) \
enum { \
    BOOST_PP_CAT(mpl_assertion_in_line_,__LINE__) = sizeof( \
          boost::mpl::assertion_failed<false>( \
              boost::mpl::assert_not_arg( (void (*) pred)0, 1 ) \
            ) \
        ) \
}\
/**/

// MPL_ASSERT_RELATION(x, ==|!=|<=|<|>=|>, y)

#if defined(BOOST_MPL_CFG_ASSERT_USE_RELATION_NAMES)
#   define MPL_ASSERT_RELATION(x, rel, y) \
enum { \
      BOOST_PP_CAT(mpl_assert_rel,__LINE__) = sizeof(boost::mpl::assert_::arg rel boost::mpl::assert_::arg) \
    , BOOST_PP_CAT(mpl_assertion_in_line_,__LINE__) = sizeof( \
        boost::mpl::assertion_failed<(x rel y)>( \
            (boost::mpl::failed ************ ( boost::mpl::assert_relation< \
                  boost::mpl::assert_::relations(BOOST_PP_CAT(mpl_assert_rel,__LINE__)) \
                , x \
                , y \
                >::************)) 0 ) \
        ) \
}\
/**/
#elif BOOST_WORKAROUND(__MWERKS__, BOOST_TESTED_AT(0x3003))
#   define MPL_ASSERT_RELATION(x, rel, y) \
enum { \
    BOOST_PP_CAT(mpl_assertion_in_line_,__LINE__) = sizeof( \
        boost::mpl::assertion_failed<(x rel y)>( \
            boost::mpl::assert_rel_arg( boost::mpl::assert_relation<x,y,(&boost::mpl::operator rel)>() ) ) \
        ) \
}\
/**/
#else
#   define MPL_ASSERT_RELATION(x, rel, y) \
enum { \
    BOOST_PP_CAT(mpl_assertion_in_line_,__LINE__) = sizeof( \
        boost::mpl::assertion_failed<(x rel y)>( (boost::mpl::failed ************ ( \
            boost::mpl::assert_relation<x,y,(&boost::mpl::operator rel) >::************))0 ) \
        ) \
}\
/**/
#endif


// MPL_ASSERT_MSG( (pred<x,...>::value), USER_PROVIDED_MESSAGE, (types<x,...>) ) 

#   define MPL_ASSERT_MSG( c, msg, types ) \
    struct msg; \
    typedef struct BOOST_PP_CAT(msg,__LINE__) : boost::mpl::assert_ \
    { \
        typedef boost::mpl::failed **************** (msg::**************** arg_type) types; \
        arg_type assert_arg() const { return 0; } \
    } BOOST_PP_CAT(mpl_assert_arg,__LINE__); \
    enum { \
        BOOST_PP_CAT(mpl_assertion_in_line_,__LINE__) = sizeof( \
              boost::mpl::assertion_failed<(c)>( BOOST_PP_CAT(mpl_assert_arg,__LINE__)().assert_arg() ) \
            ) \
    }\
/**/

#endif // BOOST_MPL_AUX_TEST_ASSERT_HPP_INCLUDED

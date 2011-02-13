#if !defined(TT_INTROSPECTION_VM_DETAIL_HPP)
#define TT_INTROSPECTION_VM_DETAIL_HPP

#include <boost/config.hpp>

#if !defined(BOOST_NO_VARIADIC_MACROS)

#include <boost/mpl/has_xxx.hpp>
#include <boost/mpl/identity.hpp>
#include <boost/preprocessor/arithmetic/add.hpp>
#include <boost/preprocessor/cat.hpp>
#include <boost/variadic_macro_data/vmd.hpp>
#include "dtti.hpp"

#if !defined(BOOST_MPL_CFG_NO_HAS_XXX_TEMPLATE)
#if !BOOST_WORKAROUND(BOOST_MSVC, <= 1400)

#define TTI_VM_DETAIL_TRAIT_HAS_TEMPLATE_CHECK_PARAMS(trait,name,...) \
  TTI_DETAIL_HAS_MEMBER_WITH_FUNCTION_SFINAE \
    (  \
      ( BOOST_PP_ADD(BOOST_VMD_DATA_SIZE(__VA_ARGS__),4), ( trait, name, 1, false, __VA_ARGS__ ) )  \
    )  \
/**/

#define TTI_VM_DETAIL_MTFC_TRAIT_HAS_TEMPLATE_CHECK_PARAMS(trait,name,...) \
  struct trait \
    { \
    TTI_DETAIL_HAS_MEMBER_WITH_FUNCTION_SFINAE \
      (  \
        ( BOOST_PP_ADD(BOOST_VMD_DATA_SIZE(__VA_ARGS__),4), ( apply, name, 1, false, __VA_ARGS__ ) )  \
      )  \
    }; \
/**/

#else // !!BOOST_WORKAROUND(BOOST_MSVC, <= 1400)

#define TTI_VM_DETAIL_TRAIT_HAS_TEMPLATE_CHECK_PARAMS(trait,name,...) \
  TTI_DETAIL_HAS_MEMBER_WITH_TEMPLATE_SFINAE \
    ( \
      ( BOOST_PP_ADD(BOOST_VMD_DATA_SIZE(__VA_ARGS__),4), ( trait, name, 1, false, __VA_ARGS__ ) )  \
    ) \
/**/

#define TTI_VM_DETAIL_MTFC_TRAIT_HAS_TEMPLATE_CHECK_PARAMS(trait,name,...) \
  TTI_DETAIL_MTFC_HAS_MEMBER_WITH_TEMPLATE_SFINAE \
    ( \
      ( BOOST_PP_ADD(BOOST_VMD_DATA_SIZE(__VA_ARGS__),4), ( trait, name, 1, false, __VA_ARGS__ ) )  \
    ) \
/**/

#endif // !BOOST_WORKAROUND(BOOST_MSVC, <= 1400)
#else // !!defined(BOOST_MPL_CFG_NO_HAS_XXX_TEMPLATE)

#define TTI_VM_DETAIL_TRAIT_HAS_TEMPLATE_CHECK_PARAMS(trait,name,...) \
  TTI_DETAIL_SAME(trait,name) \
/**/

#define TTI_VM_DETAIL_MTFC_TRAIT_HAS_TEMPLATE_CHECK_PARAMS(trait,name,...) \
  struct trait \
    { \
    TTI_DETAIL_SAME(apply,name) \
    }; \
/**/

#endif // !defined(BOOST_MPL_CFG_NO_HAS_XXX_TEMPLATE)
#endif // !defined(BOOST_NO_VARIADIC_MACROS)
#endif // TT_INTROSPECTION_VM_DETAIL_HPP

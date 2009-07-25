#ifndef BOOST_MPL_USTRING_HPP
#define BOOST_MPL_USTRING_HPP

#include <boost/mpl/vector_c.hpp>

#include <boost/preprocessor/repetition/enum_params.hpp>
#include <boost/preprocessor/repetition/enum_binary_params.hpp>
#include <boost/preprocessor/facilities/intercept.hpp>

#include <boost/cuchar.hpp>

namespace boost
{
namespace mpl
{

#define BOOST_MPL_USTRING_MAX_PARAMS BOOST_PP_SUB(BOOST_MPL_LIMIT_VECTOR_SIZE, 1)

template<BOOST_PP_ENUM_BINARY_PARAMS(BOOST_MPL_USTRING_MAX_PARAMS, char16 cp, = 0 BOOST_PP_INTERCEPT)>
struct u16string
  : vector_c<char16, BOOST_PP_ENUM_PARAMS(BOOST_MPL_USTRING_MAX_PARAMS, cp)>
{
};

template<BOOST_PP_ENUM_BINARY_PARAMS(BOOST_MPL_USTRING_MAX_PARAMS, char32 cp, = 0 BOOST_PP_INTERCEPT)>
struct u32string
  : vector_c<char32, BOOST_PP_ENUM_PARAMS(BOOST_MPL_USTRING_MAX_PARAMS, cp)>
{
};

} // namespace mpl
} // namespace boost

#endif

#if !defined(BOOST_PP_IS_ITERATING)
# error Boost.ref - do not include this file!
#endif

#if BOOST_PP_ITERATION() > 0

template<BOOST_PP_ENUM_PARAMS(BOOST_PP_ITERATION(),typename T)>
BOOST_DEDUCED_TYPENAME ::boost::result_of<T(BOOST_PP_ENUM_PARAMS(BOOST_PP_ITERATION(),T))>::type
operator()(BOOST_PP_ENUM_BINARY_PARAMS(BOOST_PP_ITERATION(),T,& a)) const
{
  return get()(BOOST_PP_ENUM_PARAMS(BOOST_PP_ITERATION(),a));
}
#endif

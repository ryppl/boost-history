#if !defined(BOOST_PP_IS_ITERATING)
# error Boost result_of - do not include this file!
#endif

template<typename F BOOST_PP_COMMA_IF(BOOST_PP_ITERATION())
         BOOST_PP_ENUM_PARAMS(BOOST_PP_ITERATION(),typename T)>
struct result_of<F(BOOST_PP_ENUM_PARAMS(BOOST_PP_ITERATION(),T))>
  : detail::result_of<F, F(BOOST_PP_ENUM_PARAMS(BOOST_PP_ITERATION(),T))> {};

namespace detail {

template<typename R,  typename FArgs BOOST_PP_COMMA_IF(BOOST_PP_ITERATION())
         BOOST_PP_ENUM_PARAMS(BOOST_PP_ITERATION(),typename T)>
struct result_of<R (*)(BOOST_PP_ENUM_PARAMS(BOOST_PP_ITERATION(),T)), FArgs>
{
  typedef R type;
};

#if BOOST_PP_ITERATION() > 1
template<typename R, typename FArgs BOOST_PP_COMMA_IF(BOOST_PP_ITERATION())
         BOOST_PP_ENUM_PARAMS(BOOST_PP_ITERATION(),typename T)>
struct result_of<R (T0::*)
                     (BOOST_PP_ENUM_SHIFTED_PARAMS(BOOST_PP_ITERATION(),T)),
                 FArgs>
{
  typedef R type;
};
#endif

}

#define n BOOST_PP_ITERATION()

template<typename RETURN, typename CLASS BOOST_PP_COMMA_IF(n) BOOST_PP_ENUM_PARAMS(n, typename A)>
struct function_type_helper<RETURN (CLASS::*)(BOOST_PP_ENUM_PARAMS(n, A))>
{
  typedef function_type<RETURN, mpl::vector<BOOST_PP_ENUM_PARAMS(n, A)>, mpl::false_> function_type;
};

template<typename RETURN, typename CLASS BOOST_PP_COMMA_IF(n) BOOST_PP_ENUM_PARAMS(n, typename A)>
struct function_type_helper<RETURN (CLASS::*)(BOOST_PP_ENUM_PARAMS(n, A)) const>
{
  typedef function_type<RETURN, mpl::vector<BOOST_PP_ENUM_PARAMS(n, A)>, mpl::true_> function_type;
};

template<typename RETURN BOOST_PP_COMMA_IF(n) BOOST_PP_ENUM_PARAMS(n, typename A)>
struct function_type_helper<RETURN (*)(BOOST_PP_ENUM_PARAMS(n, A))>
{
  typedef function_type<RETURN, mpl::vector<BOOST_PP_ENUM_PARAMS(n, A)>, mpl::false_> function_type;
};

#undef n

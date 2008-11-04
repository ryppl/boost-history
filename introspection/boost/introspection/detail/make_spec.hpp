#define n BOOST_PP_ITERATION()

template<BOOST_PP_ENUM_PARAMS(n, typename A)>
object make(const std::string& class_name, BOOST_PP_ENUM_BINARY_PARAMS(n, A, a))
{
  const class_type* type = user_defined_type::by_name<class_type>(class_name);
  return object(type, type->static_function<
    typename function_type_helper<void* (*)(BOOST_PP_ENUM_PARAMS(n, A))>::function_type >("")(BOOST_PP_ENUM_PARAMS(n, a)));
}

#undef n

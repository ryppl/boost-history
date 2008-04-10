/*
 * Copyright Jeremy Pack 2007
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 *
 * See http://www.boost.org/ for latest version.
 */


// No header guard - this file is intended to be included multiple times.

# define N BOOST_PP_ITERATION()


  template <class ParamFirst BOOST_PP_COMMA_IF(N)
  BOOST_PP_ENUM_PARAMS(N, class Param)>
void reflect_constructor() {
  add_constructor<ParamFirst BOOST_PP_COMMA_IF(N)
  BOOST_PP_ENUM_PARAMS(N, Param)>(&impl::construct_instance<T, ParamFirst
                                  BOOST_PP_COMMA_IF(N)
                                  BOOST_PP_ENUM_PARAMS(N, Param)>);
}
template <class ReturnValue BOOST_PP_COMMA_IF(N)
          BOOST_PP_ENUM_PARAMS(N, class Param)>
void reflect(ReturnValue (T::*func)(BOOST_PP_ENUM_PARAMS(N, Param)),
             Info info) {
  function_info f(reflections::type_info_handler<TypeInfo,
                  ReturnValue (*)(BOOST_PP_ENUM_PARAMS(N, Param))>
                    ::get_class_type(), info);
  ReturnValue (*f2)(void *, MemberFunctionPtr BOOST_PP_COMMA_IF(N)
      BOOST_PP_ENUM_PARAMS(N, Param)) =
      &impl::call_member<T, ReturnValue BOOST_PP_COMMA_IF(N)
                   BOOST_PP_ENUM_PARAMS(N, Param)>;
  std::pair<MemberFunctionPtr, FunctionPtr>
    p(reinterpret_cast<MemberFunctionPtr>(func),
      reinterpret_cast<FunctionPtr>(f2));
  std::pair<function_info, std::pair<MemberFunctionPtr, FunctionPtr> >
    p2(f, p);
  reflection_->functions_.insert(p2);
}
template <class ParamFirst BOOST_PP_COMMA_IF(N)
BOOST_PP_ENUM_PARAMS(N, class Param)>
void add_constructor(instance (*func)(ParamFirst BOOST_PP_COMMA_IF(N)
                                      BOOST_PP_ENUM_PARAMS(N, Param))) {
  reflection_->constructors_.insert(std::make_pair<TypeInfo, FunctionPtr>(
      reflections::type_info_handler
      <TypeInfo, instance (*)(ParamFirst BOOST_PP_COMMA_IF(N)
                              BOOST_PP_ENUM_PARAMS(N, Param))>
        ::get_class_type(),
      reinterpret_cast<FunctionPtr>(func)));
}

#undef N
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
reflector& constructor() {
  instance (*ctor_func)(
    ParamFirst BOOST_PP_COMMA_IF(N) BOOST_PP_ENUM_PARAMS(N, Param))
      (&impl::construct_instance<T, ParamFirst
                                 BOOST_PP_COMMA_IF(N)
                                 BOOST_PP_ENUM_PARAMS(N, Param)>);
  reflection_->constructors_.insert(std::make_pair<TypeInfo, impl::FunctionPtr>(
      reflections::type_info_handler
      <TypeInfo, instance (*)(ParamFirst BOOST_PP_COMMA_IF(N)
                              BOOST_PP_ENUM_PARAMS(N, Param))>
        ::get_class_type(),
      reinterpret_cast<impl::FunctionPtr>(ctor_func)));
  return *this;
}

template <class ReturnValue BOOST_PP_COMMA_IF(N)
          BOOST_PP_ENUM_PARAMS(N, class Param)>
reflector& function(ReturnValue (T::*func)(BOOST_PP_ENUM_PARAMS(N, Param)),
                    Info info) {
  function_info f(reflections::type_info_handler<TypeInfo,
                  ReturnValue (*)(BOOST_PP_ENUM_PARAMS(N, Param))>
                    ::get_class_type(), info);
  ReturnValue (*f2)(void *, impl::MemberFunctionPtr BOOST_PP_COMMA_IF(N)
      BOOST_PP_ENUM_PARAMS(N, Param)) =
      &impl::call_member<T, ReturnValue BOOST_PP_COMMA_IF(N)
                   BOOST_PP_ENUM_PARAMS(N, Param)>;
  std::pair<impl::MemberFunctionPtr, impl::FunctionPtr>
    p(reinterpret_cast<impl::MemberFunctionPtr>(func),
      reinterpret_cast<impl::FunctionPtr>(f2));
  std::pair<function_info, std::pair<impl::MemberFunctionPtr, impl::FunctionPtr> >
    p2(f, p);
  reflection_->functions_.insert(p2);
  return *this;
}


#undef N
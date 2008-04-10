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

// Versions with included info about parameters
// An auxiliary macro to add a single parameter to a list
// of parameter information.
#define BOOST_REFLECTION_PUSH_PARAMETER_INFO_SINGLE(f, N) \
(f).parameter_info_.push_back(BOOST_PP_CAT(i, N));

// An auxiliary macro to add a series of parameters to a list
// of parameter information.
#define BOOST_REFLECTION_PUSH_PARAMETER_INFO(f, N) \
BOOST_PP_IF(N, BOOST_REFLECTION_PUSH_PARAMETER_INFO_SINGLE(f, BOOST_PP_DEC(N)),) \
BOOST_PP_IF(BOOST_PP_DEC(N), BOOST_REFLECTION_PUSH_PARAMETER_INFO(f, BOOST_PP_DEC(N)),)

// Reflect a constructor with the given signature.
public:
template <class ParamFirst BOOST_PP_COMMA_IF(N)
  BOOST_PP_ENUM_PARAMS(N, class Param)>
void reflect_constructor(BOOST_PP_ENUM_PARAMS(N, ParameterInfo i)) {
  add_constructor<ParamFirst BOOST_PP_COMMA_IF(N)
  BOOST_PP_ENUM_PARAMS(N, Param)>(&impl::construct_instance<T, ParamFirst
                                  BOOST_PP_COMMA_IF(N)
                                  BOOST_PP_ENUM_PARAMS(N, Param)>
                                  BOOST_PP_COMMA_IF(N)
                                  BOOST_PP_ENUM_PARAMS(N, i));
}

template <class ReturnValue BOOST_PP_COMMA_IF(N)
          BOOST_PP_ENUM_PARAMS(N, class Param)>
void reflect(ReturnValue (T::*func)(BOOST_PP_ENUM_PARAMS(N, Param)),
             Info info, ParameterInfo i_return BOOST_PP_COMMA_IF(N)
             BOOST_PP_ENUM_PARAMS(N, ParameterInfo i)) {
  function_info f(reflections::type_info_handler<TypeInfo,
                  ReturnValue (*)(BOOST_PP_ENUM_PARAMS(N, Param))>
                    ::get_class_type(), info);
  BOOST_REFLECTION_PUSH_PARAMETER_INFO(f, N); 
  f.parameter_info_.push_back(i_return);
  ReturnValue (*f2)(void *, MemberFunctionPtr BOOST_PP_COMMA_IF(N)
      BOOST_PP_ENUM_PARAMS(N, Param)) =
      &impl::call_member<T, ReturnValue BOOST_PP_COMMA_IF(N)
                   BOOST_PP_ENUM_PARAMS(N, Param)>;
  std::pair<MemberFunctionPtr, FunctionPtr>
    in_pair(reinterpret_cast<MemberFunctionPtr>(func),
      reinterpret_cast<FunctionPtr>(f2));
  std::pair<function_info, std::pair<MemberFunctionPtr, FunctionPtr> >
    out_pair(f, in_pair);
  reflection_->functions_.insert(out_pair);
}

template <class ParamFirst BOOST_PP_COMMA_IF(N) BOOST_PP_ENUM_PARAMS(N, class Param)>
void reflect(void (T::*func)(ParamFirst p_first BOOST_PP_COMMA_IF(N) BOOST_PP_ENUM_PARAMS(N, Param)),
             Info info, ParameterInfo i_first BOOST_PP_COMMA_IF(N)
             BOOST_PP_ENUM_PARAMS(N, ParameterInfo i)) {
  function_info f(reflections::type_info_handler<TypeInfo,
                  void (*)(BOOST_PP_ENUM_PARAMS(N, Param))>
                    ::get_class_type(), info);
  f.parameter_info_.push_back(i_first);
  BOOST_REFLECTION_PUSH_PARAMETER_INFO(f, N); 
  void (*f2)(void *, MemberFunctionPtr BOOST_PP_COMMA_IF(N)
      BOOST_PP_ENUM_PARAMS(N, Param)) =
      &impl::call_member<T, void BOOST_PP_COMMA_IF(N)
                   BOOST_PP_ENUM_PARAMS(N, Param)>;
  std::pair<MemberFunctionPtr, FunctionPtr>
    in_pair(reinterpret_cast<MemberFunctionPtr>(func),
      reinterpret_cast<FunctionPtr>(f2));
  std::pair<function_info, std::pair<MemberFunctionPtr, FunctionPtr> >
    out_pair(f, in_pair);
  reflection_->functions_.insert(out_pair);
}

template <class ParamFirst BOOST_PP_COMMA_IF(N)
BOOST_PP_ENUM_PARAMS(N, class Param)>
void add_constructor(instance (*func)(ParamFirst BOOST_PP_COMMA_IF(N)
                                      BOOST_PP_ENUM_PARAMS(N, Param))
                                      BOOST_PP_COMMA_IF(N)
                                      BOOST_PP_ENUM_PARAMS(N, ParameterInfo i)) {
  constructor_info f(reflections::type_info_handler
      <TypeInfo, instance (*)(ParamFirst BOOST_PP_COMMA_IF(N)
                              BOOST_PP_ENUM_PARAMS(N, Param))>
        ::get_class_type());
  BOOST_REFLECTION_PUSH_PARAMETER_INFO(f, N); 
  reflection_->constructors_.insert(std::make_pair<TypeInfo, FunctionPtr>(
      f, reinterpret_cast<FunctionPtr>(func)));
}

#undef N
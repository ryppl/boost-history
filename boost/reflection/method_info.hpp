/*
 * Boost.Reflection / first prototype - stored information for methods
 *
 * (C) Copyright Mariano G. Consoni 2007
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 *
 * See http://www.boost.org/ for latest version.
 */

#ifndef BOOST_EXTENSION_METHOD_INFO_HPP
#define BOOST_EXTENSION_METHOD_INFO_HPP

#include <boost/function.hpp>

#include "parameter_map.hpp"



namespace boost { 
  namespace extension {

#ifdef BOOST_EXTENSION_USE_PP

#ifndef BOOST_EXTENSION_REFLECTION_PARAMS
#define BOOST_EXTENSION_REFLECTION_PARAMS(z, n, dummy) \
  BOOST_PP_CAT(MethodParam,n),BOOST_PP_CAT(MethodParamID,n)
#endif

#ifndef BOOST_EXTENSION_REFLECTION_CLASS_PARAMS
#define BOOST_EXTENSION_REFLECTION_CLASS_PARAMS(z, n, dummy) \
  BOOST_PP_CAT(class MethodParam,n),BOOST_PP_CAT(class MethodParamID,n)
#endif

    template<class Implementation, class MethodID, \
             class MethodReturnValue, \
             BOOST_PP_ENUM_PARAMS_WITH_A_DEFAULT(                        \
             BOOST_PP_ADD(BOOST_PP_INC(BOOST_EXTENSION_MAX_FUNCTOR_PARAMS) \
                       ,BOOST_PP_INC(BOOST_EXTENSION_MAX_FUNCTOR_PARAMS)), \
             class P, void)                        \
    > class method_info;

#define BOOST_EXTENSION_METHOD_INFO(Z, N, _) \
template<class Implementation, class MethodID, class MethodReturnValue \
  BOOST_PP_COMMA_IF(N) \
  BOOST_PP_ENUM(N, BOOST_EXTENSION_REFLECTION_CLASS_PARAMS, dummy) > \
class method_info<Implementation, MethodID, MethodReturnValue \
  BOOST_PP_COMMA_IF(N) \
  BOOST_PP_ENUM(N, BOOST_EXTENSION_REFLECTION_PARAMS, dummy) > \
  { \
    public: \
          typedef BOOST_PP_CAT(boost::function,BOOST_PP_INC(N)) \
                  <MethodReturnValue,                                  \
              Implementation *                                          \
              BOOST_PP_COMMA_IF(N) \
              BOOST_PP_ENUM_PARAMS(N, MethodParam) > \
          method_type;                                \
\
      method_info(MethodID id, method_type f) : id_(id), f_(f) {} \
\
      MethodReturnValue call(Implementation *imp_ptr \
                             BOOST_PP_COMMA_IF(N) \
                             BOOST_PP_ENUM_BINARY_PARAMS(N, MethodParam, p))  \
      { \
              return f_(imp_ptr BOOST_PP_COMMA_IF(N) \
                        BOOST_PP_ENUM_PARAMS(N, p));        \
      } \
\
      MethodID get_id(void) const { return id_; } \
\
    private: \
        MethodID id_; \
        method_type f_; \
    }; \
/**/


BOOST_PP_REPEAT(BOOST_PP_INC(BOOST_EXTENSION_MAX_FUNCTOR_PARAMS), \
                             BOOST_EXTENSION_METHOD_INFO, _)


#undef BOOST_EXTENSION_METHOD_INFO

#else

    template<class Implementation, class MethodID,
             class MethodReturnValue,
             class MethodParam0 = void, class MethodParamID0 = void, 
             class MethodParam1 = void, class MethodParamID1 = void,
             class MethodParam2 = void, class MethodParamID2 = void>
    class method_info;


    template<class Implementation, class MethodID,
             class MethodReturnValue,
             class MethodParam0, class MethodParamID0,
             class MethodParam1, class MethodParamID1>
    class method_info<Implementation, MethodID,
                      MethodReturnValue,
                      MethodParam0, MethodParamID0,
                      MethodParam1, MethodParamID1>
    {
    public:
      typedef boost::function3<MethodReturnValue, Implementation *, 
                               MethodParam0, MethodParam1> method_type;

      method_info(MethodID id, method_type f) : id_(id), f_(f) {}

      MethodReturnValue call(Implementation *imp_ptr, MethodParam0 p0, 
                             MethodParam1 p1)
      {
        return f_(imp_ptr, p0, p1);
      }

      MethodID get_id(void) const { return id_; }

    private:
        MethodID id_;
        method_type f_;
    };


    template<class Implementation, class MethodID,
             class MethodReturnValue,
             class MethodParam0, class MethodParamID0>
    class method_info<Implementation, MethodID, MethodReturnValue,
                      MethodParam0, MethodParamID0>
    {
    public:
      typedef boost::function2<MethodReturnValue, Implementation *, 
                               MethodParam0> method_type;

      method_info(MethodID id, method_type f) : id_(id), f_(f) {}

      MethodReturnValue call(Implementation *imp_ptr, MethodParam0 p0)
      {
        return f_(imp_ptr, p0);
      }

      MethodID get_id(void) const { return id_; }

    private:
        MethodID id_;
        method_type f_;
    };



    template<class Implementation, class MethodID, class MethodReturnValue>
    class method_info<Implementation, MethodID, MethodReturnValue>
    {
    public:
      typedef boost::function1<MethodReturnValue, Implementation *> 
                                                        method_type;

      method_info(MethodID id, method_type f) : id_(id), f_(f) {}

      MethodReturnValue call(Implementation *imp_ptr)
      {
        return f_(imp_ptr);
      }

      MethodID get_id(void) const { return id_; }

    private:
        MethodID id_;
        method_type f_;
    };

#endif // BOOST_EXTENSION_USE_PP

  } // extension
} // boost

#endif // BOOST_EXTENSION_METHOD_INFO_HPP

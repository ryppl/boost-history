/*
 * Boost.Reflection / main header
 *
 * (C) Copyright Mariano G. Consoni 2007
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 *
 * See http://www.boost.org/ for latest version.
 */

// TODO:
//  . implement calls with parameter maps
//  . fix FIXMEs

#ifndef BOOST_EXTENSION_REFLECTION_HPP
#define BOOST_EXTENSION_REFLECTION_HPP

#include <boost/function.hpp>

#include <list>


#ifdef BOOST_EXTENSION_USE_PP

#include <boost/preprocessor/arithmetic/inc.hpp>
#include <boost/preprocessor/if.hpp>
#include <boost/preprocessor/punctuation/comma_if.hpp>
#include <boost/preprocessor/repetition.hpp>

#ifndef BOOST_EXTENSION_MAX_FUNCTOR_PARAMS
#define BOOST_EXTENSION_MAX_FUNCTOR_PARAMS 2
#endif

#endif

#include "method_info.hpp"
#include "parameter_map.hpp"



namespace boost { 
  namespace extension {

    template<class Implementation, class Info>
    class reflection
    {
    public:
      reflection(Info info) : info_(info) {}

      class generic_method_container
      {
      public:
        virtual ~generic_method_container(){}
      };


      template <class MethodID, class MethodReturnValue,
                class MethodParam0 = void, class MethodParamID0 = void, 
                class MethodParam1 = void, class MethodParamID1 = void>

      class method_container
        : public std::list<method_info<Implementation, MethodID,
                                       MethodReturnValue, 
                                       MethodParam0, MethodParamID0, 
                                       MethodParam1, MethodParamID1> >,
          public generic_method_container 
      {
          public:
            method_container() {}
            virtual ~method_container(){}
      };


      typedef std::list<generic_method_container *> MethodList;
      MethodList methods_;

#ifdef BOOST_EXTENSION_USE_PP


#ifndef BOOST_EXTENSION_REFLECTION_PARAMS
#define BOOST_EXTENSION_REFLECTION_PARAMS(z, n, dummy) \
  BOOST_PP_CAT(MethodParam,n),BOOST_PP_CAT(MethodParamID,n)
#endif

#ifndef BOOST_EXTENSION_REFLECTION_CLASS_PARAMS
#define BOOST_EXTENSION_REFLECTION_CLASS_PARAMS(z, n, dummy) \
  BOOST_PP_CAT(class MethodParam,n),BOOST_PP_CAT(class MethodParamID,n)
#endif

#define BOOST_EXTENSION_REFLECTION_FIND_METHOD(Z, N, _) \
template<class MethodID, class MethodReturnValue \
  BOOST_PP_COMMA_IF(N) \
  BOOST_PP_ENUM(N, BOOST_EXTENSION_REFLECTION_CLASS_PARAMS, dummy) > \
typename reflection::MethodList::iterator find_method(MethodID id) \
{ \
  typename MethodList::iterator it = methods_.begin(); \
\
  for(; it != methods_.end(); ++it) { \
    method_container<MethodID, MethodReturnValue \
      BOOST_PP_COMMA_IF(N) \
      BOOST_PP_ENUM(N, BOOST_EXTENSION_REFLECTION_PARAMS, dummy) \
    > *mc = \
      dynamic_cast< method_container<MethodID, MethodReturnValue \
      BOOST_PP_COMMA_IF(N) \
      BOOST_PP_ENUM(N, BOOST_EXTENSION_REFLECTION_PARAMS, dummy) \
        > * >(*it); \
      if(mc == NULL) { \
        continue; \
      } \
\
      if(mc->begin()->get_id() == id) { \
        return it; \
      } \
  } \
  return methods_.end(); \
}
/**/


#define BOOST_EXTENSION_REFLECTION_GET_METHOD(Z, N, _) \
template<class MethodID, class MethodReturnValue \
  BOOST_PP_COMMA_IF(N) \
  BOOST_PP_ENUM(N, BOOST_EXTENSION_REFLECTION_CLASS_PARAMS, dummy) > \
std::list<method_info<Implementation, MethodID, MethodReturnValue \
      BOOST_PP_COMMA_IF(N) \
      BOOST_PP_ENUM(N, BOOST_EXTENSION_REFLECTION_PARAMS, dummy) > > & \
get(MethodID id) \
{ \
  typename MethodList::iterator it = \
    find_method<MethodID, MethodReturnValue                                \
      BOOST_PP_COMMA_IF(N) \
      BOOST_PP_ENUM(N, BOOST_EXTENSION_REFLECTION_PARAMS, dummy) > (id); \
\
  if(it == methods_.end()) { \
    method_container<MethodID, MethodReturnValue \
      BOOST_PP_COMMA_IF(N) \
      BOOST_PP_ENUM(N, BOOST_EXTENSION_REFLECTION_PARAMS, dummy) > \
      * ret = new method_container<MethodID, MethodReturnValue \
      BOOST_PP_COMMA_IF(N) \
      BOOST_PP_ENUM(N, BOOST_EXTENSION_REFLECTION_PARAMS, dummy) > \
        (); \
    methods_.push_back(ret); \
    return *ret; \
  } else { \
    return static_cast<method_container<MethodID, MethodReturnValue \
      BOOST_PP_COMMA_IF(N) \
      BOOST_PP_ENUM(N, BOOST_EXTENSION_REFLECTION_PARAMS, dummy) > \
                  &> (*(*it)); \
  } \
}
/**/


#define BOOST_EXTENSION_REFLECTION_ADD_METHOD(Z, N, _) \
template<class MethodID, class MethodReturnValue \
  BOOST_PP_COMMA_IF(N) \
  BOOST_PP_ENUM(N, BOOST_EXTENSION_REFLECTION_CLASS_PARAMS, dummy) > \
void add( BOOST_PP_CAT(boost::function, BOOST_PP_ADD(N,1) ) \
          <MethodReturnValue, Implementation *                            \
        BOOST_PP_COMMA_IF(N) BOOST_PP_ENUM_PARAMS(N, MethodParam) > \
        method, MethodID method_id BOOST_PP_COMMA_IF(N) \
        BOOST_PP_ENUM_BINARY_PARAMS(N, MethodParamID, method_param_id) ) \
{ \
  typedef std::list<method_info<Implementation, MethodID, MethodReturnValue \
      BOOST_PP_COMMA_IF(N) \
      BOOST_PP_ENUM(N, BOOST_EXTENSION_REFLECTION_PARAMS, dummy) > > \
     ListType; \
\
  ListType & s = this->get<MethodID, MethodReturnValue \
      BOOST_PP_COMMA_IF(N) \
      BOOST_PP_ENUM(N, BOOST_EXTENSION_REFLECTION_PARAMS, dummy) > \
     (method_id); \
\
  method_info<Implementation, MethodID, MethodReturnValue \
      BOOST_PP_COMMA_IF(N) \
      BOOST_PP_ENUM(N, BOOST_EXTENSION_REFLECTION_PARAMS, dummy) > \
     m(method_id, method); \
  s.push_back(m); \
}
/**/

#define BOOST_EXTENSION_REFLECTION_CALL_METHOD(Z, N, _) \
template<class MethodID, class MethodReturnValue \
  BOOST_PP_COMMA_IF(N) \
  BOOST_PP_ENUM(N, BOOST_EXTENSION_REFLECTION_CLASS_PARAMS, dummy) > \
MethodReturnValue call(Implementation *imp_ptr, MethodID method_id \
BOOST_PP_COMMA_IF(N) \
BOOST_PP_ENUM_BINARY_PARAMS(N, MethodParam, p) ) \
{ \
  typename MethodList::iterator it = find_method<MethodID, MethodReturnValue \
      BOOST_PP_COMMA_IF(N) \
      BOOST_PP_ENUM(N, BOOST_EXTENSION_REFLECTION_PARAMS, dummy) > \
      (method_id); \
\
  method_container<MethodID, MethodReturnValue \
      BOOST_PP_COMMA_IF(N) \
      BOOST_PP_ENUM(N, BOOST_EXTENSION_REFLECTION_PARAMS, dummy) > \
      &s = \
      static_cast<method_container<MethodID, MethodReturnValue \
      BOOST_PP_COMMA_IF(N) \
      BOOST_PP_ENUM(N, BOOST_EXTENSION_REFLECTION_PARAMS, dummy) > \
      &>(*(*it)); \
\
  if(s.begin() == s.end()) { \
    std::cerr << "Not Found: " << method_id << std::endl; \
    std::exit(-1); \
  } \
\
  method_info<Implementation, MethodID, MethodReturnValue \
      BOOST_PP_COMMA_IF(N) \
      BOOST_PP_ENUM(N, BOOST_EXTENSION_REFLECTION_PARAMS, dummy) > \
      mi = *(s.begin()); \
\
  return mi.call(imp_ptr BOOST_PP_COMMA_IF(N) \
                 BOOST_PP_ENUM_PARAMS(N, p));        \
}
/**/


BOOST_PP_REPEAT(BOOST_PP_INC(BOOST_EXTENSION_MAX_FUNCTOR_PARAMS), \
                             BOOST_EXTENSION_REFLECTION_FIND_METHOD, _)


BOOST_PP_REPEAT(BOOST_PP_INC(BOOST_EXTENSION_MAX_FUNCTOR_PARAMS), \
                             BOOST_EXTENSION_REFLECTION_GET_METHOD, _)


BOOST_PP_REPEAT(BOOST_PP_INC(BOOST_EXTENSION_MAX_FUNCTOR_PARAMS), \
                             BOOST_EXTENSION_REFLECTION_ADD_METHOD, _)


BOOST_PP_REPEAT(BOOST_PP_INC(BOOST_EXTENSION_MAX_FUNCTOR_PARAMS), \
                             BOOST_EXTENSION_REFLECTION_CALL_METHOD, _)


#undef BOOST_EXTENSION_REFLECTION_FIND_METHOD
#undef BOOST_EXTENSION_REFLECTION_GET_METHOD
#undef BOOST_EXTENSION_REFLECTION_ADD_METHOD
#undef BOOST_EXTENSION_REFLECTION_CALL_METHOD

#else
      template<class MethodID, class MethodReturnValue, 
               class MethodParam1, class MethodParam1ID>
      typename reflection::MethodList::iterator find_method(MethodID id)
      {
        typename MethodList::iterator it = methods_.begin();

        for(; it != methods_.end(); ++it) {
          method_container<MethodID, MethodReturnValue, 
            MethodParam1, MethodParam1ID> *mc = 
            dynamic_cast< method_container<MethodID, MethodReturnValue,
            MethodParam1, MethodParam1ID> *>(*it);

          if(mc == NULL) {
            // if the cast failed it means that this element of the list
            // it isn't of the expected type, so we skip it.
            continue;
          }

          // now we check if the id is correct (knowing that the parameters
          // types are correct)
           if(mc->begin()->get_id() == id) {
             return it;
           }
        }
        return methods_.end();
      }


      template<class MethodID, class MethodReturnValue>
      typename reflection::MethodList::iterator find_method(MethodID id)
      {
        typename MethodList::iterator it = methods_.begin();

        for(; it != methods_.end(); ++it) {
          method_container<MethodID, MethodReturnValue> *mc = 
            dynamic_cast< method_container<MethodID, 
                        MethodReturnValue> *>(*it);

          if(mc == NULL) {
            // if the cast failed it means that this element of the list
            // it isn't of the expected type, so we skip it.
            continue;
          }

          // now we check if the id is correct (knowing that the parameters
          // types are correct)
           if(mc->begin()->get_id() == id) {
             return it;
           }
        }

        return methods_.end();
      }




      template<class MethodID, class MethodReturnValue>
      std::list<method_info<Implementation, MethodID, MethodReturnValue> > &
      get(MethodID id)
      {
        typename MethodList::iterator it = 
          find_method<MethodID, MethodReturnValue>(id);
      
         if(it == methods_.end()) {
          // FIXME: free
          method_container<MethodID, MethodReturnValue> * ret = new 
            method_container<MethodID, MethodReturnValue>();

          methods_.push_back(ret);
          return *ret;
         } else {
           // Change to dynamic if this fails
           return static_cast<method_container<MethodID, MethodReturnValue> &>
             (*(*it));
         }
      }


      template<class MethodID, class MethodReturnValue, 
               class MethodParam1, class MethodParam1ID>
      std::list<method_info<Implementation, MethodID, 
                MethodReturnValue, MethodParam1, MethodParam1ID> > &
      get(MethodID id)
      {
        typename MethodList::iterator it = 
          find_method<MethodID, MethodReturnValue, 
          MethodParam1, MethodParam1ID>(id);
      
         if(it == methods_.end()) {
          // FIXME: free
          method_container<MethodID, MethodReturnValue,
            MethodParam1, MethodParam1ID> * ret = new 
            method_container<MethodID, MethodReturnValue,
            MethodParam1, MethodParam1ID>();

          methods_.push_back(ret);
          return *ret;
         } else {
           // Change to dynamic if this fails
          return static_cast<method_container<MethodID, MethodReturnValue,
            MethodParam1, MethodParam1ID> &> (*(*it));
         }
      }


      template<class MethodID, class MethodReturnValue, class MethodParam1, 
               class MethodParam1ID>
      void add(boost::function2<MethodReturnValue, Implementation *, 
               MethodParam1> method, MethodID method_id, 
               MethodParam1ID method_param1_id)
      {
         typedef std::list<method_info<Implementation, MethodID, 
          MethodReturnValue, 
           MethodParam1, MethodParam1ID> > ListType;

         ListType & s = this->get<MethodID, MethodReturnValue, MethodParam1, 
          MethodParam1ID>(method_id);

        method_info<Implementation, MethodID, MethodReturnValue, 
          MethodParam1, MethodParam1ID> m(method_id, method);
         s.push_back(m);
      }


      template<class MethodID, class MethodReturnValue>
      void add(boost::function1<MethodReturnValue, Implementation *> method, 
               MethodID method_id)
      {
         typedef std::list<method_info<Implementation, MethodID,
          MethodReturnValue> > ListType;

         ListType & s = this->get<MethodID, MethodReturnValue>(method_id);

        method_info<Implementation, MethodID, MethodReturnValue> m(method_id, 
                                                                   method);
         s.push_back(m);
      }


      template<class MethodID, class MethodReturnValue, class MethodParam1, 
               class MethodParam1ID>
      MethodReturnValue call(Implementation *imp_ptr, MethodID method_id, 
                             MethodParam1 p1)
      {
        typename MethodList::iterator it = find_method<MethodID, 
          MethodReturnValue, MethodParam1, MethodParam1ID>(method_id);

        method_container<MethodID, MethodReturnValue, MethodParam1, 
                      MethodParam1ID> &s = 
          static_cast<method_container<MethodID, MethodReturnValue, 
          MethodParam1, MethodParam1ID> &>(*(*it));

        // FIXME: see if it is useful having a list here instead of the
        // plain method_info.
        if(s.begin() == s.end()) {
          std::cerr << "Not Found: " << method_id << std::endl;
          std::exit(-1);
        }
        method_info<Implementation, MethodID, MethodReturnValue, MethodParam1, 
          MethodParam1ID> mi = *(s.begin());

        return mi.call(imp_ptr, p1);
      }


      template<class MethodID, class MethodReturnValue>
      MethodReturnValue call(Implementation *imp_ptr, MethodID method_id)
      {
        typename MethodList::iterator it = find_method<MethodID, 
          MethodReturnValue>(method_id);

        method_container<MethodID, MethodReturnValue> &s = 
          static_cast<method_container<MethodID, MethodReturnValue> &>(*(*it));

        // FIXME: see if it is useful having a list here instead of the
        // plain method_info.
        if(s.begin() == s.end()) {
          std::cerr << "Not Found: " << method_id << std::endl;
          std::exit(-1);
        }
        method_info<Implementation, MethodID, MethodReturnValue> mi = 
          *(s.begin());

         return mi.call(imp_ptr);
      }
#endif // BOOST_EXTENSION_USE_PP

      // FIXME: boost.any return value
      template<class MethodID>
      void call(MethodID method_id, const parameter_map &pm) const
      {
        // find the corresponding method
        // iterate each parameter and search for it in the parameter map
      }

      Info &get_info(void) { return info_; }

    private:
      Info info_;
    };

  } // extension
} // boost

#endif // BOOST_EXTENSION_REFLECTION_HPP

//  (C) Copyright David Abrahams 2000. Permission to copy, use, modify, sell and
//  distribute this software is granted provided this copyright notice appears
//  in all copies. This software is provided "as is" without express or implied
//  warranty, and with no claim as to its suitability for any purpose.
//
//  The author gratefully acknowleges the support of Dragon Systems, Inc., in
//  producing this work.
//
//  This file was generated for %d-argument constructors by gen_init_function.python

#ifndef INIT_FUNCTION_DWA052000_H_
# define INIT_FUNCTION_DWA052000_H_

# include "pyconfig.h"
# include "functions.h"
# include "signatures.h"
# include <typeinfo>

namespace python {

namespace detail {

  // parameter_traits - so far, this is a way to pass a const T& when we can be
  // sure T is not a reference type, and a raw T otherwise. This should be
  // rolled into boost::call_traits. Ordinarily, parameter_traits would be
  // written:
  //
  //    template <class T> struct parameter_traits
  //    {
  //        typedef const T& const_reference;
  //    };
  //
  //    template <class T> struct parameter_traits<T&>
  //    {
  //        typedef T& const_reference;
  //    };
  //
  //    template <> struct parameter_traits<void>
  //    {
  //        typedef void const_reference;
  //    };
  //
  // ...but since we can't partially specialize on reference types, we need this
  // long-winded but equivalent incantation.

  // const_ref_selector -- an implementation detail of parameter_traits (below). This uses
  // the usual "poor man's partial specialization" hack for MSVC. 
  template <bool is_ref>
  struct const_ref_selector
  {
      template <class T>
      struct const_ref
      {
          typedef const T& type;
      };
  };

  template <>
  struct const_ref_selector<true>
  {
      template <class T>
      struct const_ref
      {
          typedef T type;
      };
  };

# ifdef BOOST_MSVC
#  pragma warning(push)
#  pragma warning(disable: 4181)
# endif // BOOST_MSVC
  template <class T>
  struct parameter_traits
  {
   private:
      typedef const_ref_selector<boost::is_reference<T>::value> selector;
   public:
      typedef typename selector::template const_ref<T>::type const_reference;
  };
# ifdef BOOST_MSVC
#  pragma warning(pop)
# endif // BOOST_MSVC

  // Full spcialization for void
  template <>
  struct parameter_traits<void>
  {
      typedef void const_reference;
  };

  template <class T>
  class reference_parameter
  {
      typedef typename parameter_traits<T>::const_reference const_reference;
   public:
      reference_parameter(const_reference value)
          : value(value) {}
      operator const_reference() { return value; }
   private:
      const_reference value;
  };

class extension_instance;
class instance_holder_base;

class init;
template <class T> struct init0;
template <class T, class A1> struct init1;
template <class T, class A1, class A2> struct init2;
template <class T, class A1, class A2, class A3> struct init3;
template <class T, class A1, class A2, class A3, class A4> struct init4;
template <class T, class A1, class A2, class A3, class A4, class A5> struct init5;

template <class T>
struct init_function
{
    static init* create(signature0) {
        return new init0<T>;
    }

    template <class A1>
    static init* create(signature1<A1>) {
        return new init1<T,
                       detail::parameter_traits<A1>::const_reference>;
    }

    template <class A1, class A2>
    static init* create(signature2<A1, A2>) {
        return new init2<T,
                       detail::parameter_traits<A1>::const_reference,
                       detail::parameter_traits<A2>::const_reference>;
    }

    template <class A1, class A2, class A3>
    static init* create(signature3<A1, A2, A3>) {
        return new init3<T,
                       detail::parameter_traits<A1>::const_reference,
                       detail::parameter_traits<A2>::const_reference,
                       detail::parameter_traits<A3>::const_reference>;
    }

    template <class A1, class A2, class A3, class A4>
    static init* create(signature4<A1, A2, A3, A4>) {
        return new init4<T,
                       detail::parameter_traits<A1>::const_reference,
                       detail::parameter_traits<A2>::const_reference,
                       detail::parameter_traits<A3>::const_reference,
                       detail::parameter_traits<A4>::const_reference>;
    }

    template <class A1, class A2, class A3, class A4, class A5>
    static init* create(signature5<A1, A2, A3, A4, A5>) {
        return new init5<T,
                       detail::parameter_traits<A1>::const_reference,
                       detail::parameter_traits<A2>::const_reference,
                       detail::parameter_traits<A3>::const_reference,
                       detail::parameter_traits<A4>::const_reference,
                       detail::parameter_traits<A5>::const_reference>;
    }
};

class init : public function
{
private: // override function hook
    PyObject* do_call(PyObject* args, PyObject* keywords) const;
private:
    virtual instance_holder_base* create_holder(extension_instance* self, PyObject* tail_args, PyObject* keywords) const = 0;
    string arguments_as_string(tuple arguments) const;
};


template <class T>
struct init0 : init
{
    virtual instance_holder_base* create_holder(extension_instance* self, PyObject* args, PyObject* /*keywords*/) const
    {
        if (!PyArg_ParseTuple(args, const_cast<char*>("")))
            throw argument_error();
        return new T(self
            );
    }
    
    PyObject* description() const
    { 
        return function_signature((void (*)())0); 
    }
    
    string function_name() const
    { 
        string result(get_python_type_name(python::type<T>()));
        result += ".__init__";
        return result; 
    }
};

template <class T, class A1>
struct init1 : init
{
    virtual instance_holder_base* create_holder(extension_instance* self, PyObject* args, PyObject* /*keywords*/) const
    {
        PyObject* a1;
        if (!PyArg_ParseTuple(args, const_cast<char*>("O"), &a1))
            throw argument_error();
        return new T(self,
            python::detail::reference_parameter<A1>(from_python(a1, type<A1>()))
            );
    }
    
    PyObject* description() const
    { 
        return function_signature((void (*)(A1))0); 
    }
    
    string function_name() const
    { 
        string result(get_python_type_name(python::type<T>()));
        result += ".__init__";
        return result; 
    }
};

template <class T, class A1, class A2>
struct init2 : init
{
    virtual instance_holder_base* create_holder(extension_instance* self, PyObject* args, PyObject* /*keywords*/) const
    {
        PyObject* a1;
        PyObject* a2;
        if (!PyArg_ParseTuple(args, const_cast<char*>("OO"), &a1, &a2))
            throw argument_error();
        return new T(self,
            python::detail::reference_parameter<A1>(from_python(a1, type<A1>())),
            python::detail::reference_parameter<A2>(from_python(a2, type<A2>()))
            );
    }
    
    PyObject* description() const
    { 
        return function_signature((void (*)(A1, A2))0); 
    }
    
    string function_name() const
    { 
        string result(get_python_type_name(python::type<T>()));
        result += ".__init__";
        return result; 
    }
};

template <class T, class A1, class A2, class A3>
struct init3 : init
{
    virtual instance_holder_base* create_holder(extension_instance* self, PyObject* args, PyObject* /*keywords*/) const
    {
        PyObject* a1;
        PyObject* a2;
        PyObject* a3;
        if (!PyArg_ParseTuple(args, const_cast<char*>("OOO"), &a1, &a2, &a3))
            throw argument_error();
        return new T(self,
            python::detail::reference_parameter<A1>(from_python(a1, type<A1>())),
            python::detail::reference_parameter<A2>(from_python(a2, type<A2>())),
            python::detail::reference_parameter<A3>(from_python(a3, type<A3>()))
            );
    }
    
    PyObject* description() const
    { 
        return function_signature((void (*)(A1, A2, A3))0); 
    }
    
    string function_name() const
    { 
        string result(get_python_type_name(python::type<T>()));
        result += ".__init__";
        return result; 
    }
};

template <class T, class A1, class A2, class A3, class A4>
struct init4 : init
{
    virtual instance_holder_base* create_holder(extension_instance* self, PyObject* args, PyObject* /*keywords*/) const
    {
        PyObject* a1;
        PyObject* a2;
        PyObject* a3;
        PyObject* a4;
        if (!PyArg_ParseTuple(args, const_cast<char*>("OOOO"), &a1, &a2, &a3, &a4))
            throw argument_error();
        return new T(self,
            python::detail::reference_parameter<A1>(from_python(a1, type<A1>())),
            python::detail::reference_parameter<A2>(from_python(a2, type<A2>())),
            python::detail::reference_parameter<A3>(from_python(a3, type<A3>())),
            python::detail::reference_parameter<A4>(from_python(a4, type<A4>()))
            );
    }
    
    PyObject* description() const
    { 
        return function_signature((void (*)(A1, A2, A3, A4))0); 
    }
    
    string function_name() const
    { 
        string result(get_python_type_name(python::type<T>()));
        result += ".__init__";
        return result; 
    }
};

template <class T, class A1, class A2, class A3, class A4, class A5>
struct init5 : init
{
    virtual instance_holder_base* create_holder(extension_instance* self, PyObject* args, PyObject* /*keywords*/) const
    {
        PyObject* a1;
        PyObject* a2;
        PyObject* a3;
        PyObject* a4;
        PyObject* a5;
        if (!PyArg_ParseTuple(args, const_cast<char*>("OOOOO"), &a1, &a2, &a3, &a4, &a5))
            throw argument_error();
        return new T(self,
            python::detail::reference_parameter<A1>(from_python(a1, type<A1>())),
            python::detail::reference_parameter<A2>(from_python(a2, type<A2>())),
            python::detail::reference_parameter<A3>(from_python(a3, type<A3>())),
            python::detail::reference_parameter<A4>(from_python(a4, type<A4>())),
            python::detail::reference_parameter<A5>(from_python(a5, type<A5>()))
            );
    }
    
    PyObject* description() const
    { 
        return function_signature((void (*)(A1, A2, A3, A4, A5))0); 
    }
    
    string function_name() const
    { 
        string result(get_python_type_name(python::type<T>()));
        result += ".__init__";
        return result; 
    }
};

}} // namespace python::detail

#endif // INIT_FUNCTION_DWA052000_H_


//  (C) Copyright David Abrahams 2000. Permission to copy, use, modify, sell and
//  distribute this software is granted provided this copyright notice appears
//  in all copies. This software is provided "as is" without express or implied
//  warranty, and with no claim as to its suitability for any purpose.
//
//  The author gratefully acknowleges the support of Dragon Systems, Inc., in
//  producing this work.

#ifndef FUNCTIONS_DWA051400_H_
# define FUNCTIONS_DWA051400_H_

# include "pyconfig.h"
# include "wrap_python.h"
# include "pyptr.h"
# include "signatures.h"
# include "caller.h"
# include <boost/call_traits.hpp>
# include "objects.h"
# include "base_object.h"
# include <typeinfo>
# include <vector>

namespace py { namespace detail {

// forward declaration
class ExtensionInstance;


// Function --
//      the common base class for all overloadable function and method objects
//      supplied by the library.
class Function : public PythonObject
{
 public:
    Function();
    // Function objects are reasonably rare, so we guess we can afford a virtual table.
    // This cuts down on the number of distinct type objects which need to be defined.
    virtual ~Function() {}

    PyObject* call(PyObject* args, PyObject* keywords) const;
    static void add_to_namespace(PyPtr<Function> f, const char* name, PyObject* dict);
    
 private:
    virtual PyObject* do_call(PyObject* args, PyObject* keywords) const = 0;
    virtual const char* description() const = 0;
 private:
    struct TypeObject;
 private:
    PyPtr<Function> m_overloads;
};

// WrappedFunctionPointer<> --
//      A single function or member function pointer wrapped and presented to
//      Python as a callable object.
//
//   Template parameters:
//      R - the return type of the function pointer
//      F - the complete type of the wrapped function pointer
template <class R, class F>
struct WrappedFunctionPointer : Function
{
	typedef F PtrFun; // pointer-to--function or pointer-to-member-function
	
	WrappedFunctionPointer(PtrFun pf)
        : m_pf(pf) {}

 private:
	PyObject* do_call(PyObject* args, PyObject* keywords) const
        { return Caller<R>::call(m_pf, args, keywords); }
    
    const char* description() const
        { return typeid(F).name(); }
    
 private:
	const PtrFun m_pf;
};

// raw_arguments_function
//      A function that passes the Python argument tuple and keyword dictionary 
//      verbatim to C++ (useful for customized argument parsing and variable
//      argument lists)
template <class Ret, class Args, class Keywords>
struct raw_arguments_function : Function
{
	typedef Ret (*PtrFun)(Args, Keywords); 
	
	raw_arguments_function(PtrFun pf)
        : m_pf(pf) {}

 private:
	PyObject* do_call(PyObject* args, PyObject* keywords) const
    { 
        Ptr dict(keywords ? 
                 Ptr(keywords, Ptr::new_ref) :
                 Ptr(PyDict_New()));
            
        return to_python(
            (*m_pf)(from_python(args, py::Type<Args>()),
                    from_python(dict.get(), py::Type<Keywords>()))); 
    }
    
    const char* description() const
        { return typeid(PtrFun).name(); }
    
 private:
	const PtrFun m_pf;
};

// virtual_function<> --
//      A virtual function with a default implementation wrapped and presented
//      to Python as a callable object.
//
//   Template parameters:
//      T - the type of the target class
//      R - the return type of the function pointer
//      V - the virtual function pointer being wrapped
//          (should be of the form R(T::*)(<args>), or R (*)(T, <args>))
//      D - a function which takes a T&, const T&, T*, or const T* first
//          parameter and calls T::f on it /non-virtually/, where V
//          approximates &T::f.
template <class T, class R, class V, class D>
class virtual_function : public Function
{
 public:
	virtual_function(V virtual_function_ptr, D default_implementation)
        : m_virtual_function_ptr(virtual_function_ptr),
          m_default_implementation(default_implementation)
        {}

 private:
	PyObject* do_call(PyObject* args, PyObject* keywords) const;
    
    const char* description() const
        { return typeid(V).name(); }
    
 private:
	const V m_virtual_function_ptr;
    const D m_default_implementation;
};

// A helper function for new_member_function(), below.  Implements the core
// functionality once the return type has already been deduced. R is expected to
// be Type<X>, where X is the actual return type of pmf.
template <class F, class R>
Function* new_wrapped_function_aux(R, F pmf)
{
    // We can't just use "typename R::Type" below because MSVC (incorrectly) pukes.
    typedef typename R::Type ReturnType;
    return new WrappedFunctionPointer<ReturnType, F>(pmf);
}

// Create and return a new member function object wrapping the given
// pointer-to-member function
template <class F>
inline Function* new_wrapped_function(F pmf)
{
    // Deduce the return type and pass it off to the helper function above
	return new_wrapped_function_aux(return_value(pmf), pmf);
}

template <class R, class Args, class Keywords>
Function* new_raw_arguments_function(R (*pmf)(Args, Keywords))
{
    return new raw_arguments_function<R, Args, Keywords>(pmf);
}


// A helper function for new_virtual_function(), below.  Implements the core
// functionality once the return type has already been deduced. R is expected to
// be Type<X>, where X is the actual return type of V.
template <class T, class R, class V, class D>
inline Function* new_virtual_function_aux(
    Type<T>, R, V virtual_function_ptr, D default_implementation
    )
{
    // We can't just use "typename R::Type" below because MSVC (incorrectly) pukes.
    typedef typename R::Type ReturnType;
    return new virtual_function<T, ReturnType, V, D>(
        virtual_function_ptr, default_implementation);
}

// Create and return a new virtual_function object wrapping the given
// virtual_function_ptr and default_implementation
template <class T, class V, class D>
inline Function* new_virtual_function(
    Type<T>, V virtual_function_ptr, D default_implementation
    )
{
    // Deduce the return type and pass it off to the helper function above
    return new_virtual_function_aux(
        Type<T>(), return_value(virtual_function_ptr),
        virtual_function_ptr, default_implementation);
}

// A function with a bundled "bound target" object. This is what is produced by
// the expression a.b where a is an Instance or ExtensionInstance object and b
// is a callable object not found in the instance namespace but on its class or
// a base class.
class BoundFunction : public PythonObject
{
 public:
    static BoundFunction* create(const Ptr& target, const Ptr& fn);
    
    BoundFunction(const Ptr& target, const Ptr& fn);
    PyObject* call(PyObject*args, PyObject* keywords) const;
    PyObject* getattr(const char* name) const;
    
 private:
    struct TypeObject;
    friend struct TypeObject;
    
    Ptr m_target;
    Ptr m_unbound_function;
    
 private: // data members for allocation/deallocation optimization
    BoundFunction* m_free_list_link;

	static BoundFunction* free_list;
};

// Special functions designed to access data members of a wrapped C++ object.
template <class ClassType, class MemberType>
class GetterFunction : public Function
{
 public:
    typedef MemberType ClassType::* PointerToMember;
    
    GetterFunction(PointerToMember pm)
        : m_pm(pm) {}

 private:
    PyObject* do_call(PyObject* args, PyObject* keywords) const;
    
    const char* description() const
        { return typeid(MemberType (*)(const ClassType&)).name(); }
 private:
    PointerToMember m_pm;
};

template <class ClassType, class MemberType>
class SetterFunction : public Function
{
 public:
    typedef MemberType ClassType::* PointerToMember;
    
    SetterFunction(PointerToMember pm)
        : m_pm(pm) {}

 private:
    PyObject* do_call(PyObject* args, PyObject* keywords) const;
    
    const char* description() const
        { return typeid(void (*)(const ClassType&, const MemberType&)).name(); }
 private:
    PointerToMember m_pm;
};

template <class ClassType, class MemberType>
PyObject* GetterFunction<ClassType, MemberType>::do_call(
    PyObject* args, PyObject* /* keywords */) const
{
    PyObject* self;
    if (!PyArg_ParseTuple(args, const_cast<char*>("O"), &self))
        return 0;

    return to_python(
        from_python(self, Type<const ClassType*>())->*m_pm);
}

template <class ClassType, class MemberType>
PyObject* SetterFunction<ClassType, MemberType>::do_call(
    PyObject* args, PyObject* /* keywords */) const
{
    PyObject* self;
    PyObject* value;
    if (!PyArg_ParseTuple(args, const_cast<char*>("OO"), &self, &value))
        return 0;
    
    typedef typename boost::call_traits<MemberType>::const_reference ExtractType;
    from_python(self, Type<ClassType*>())->*m_pm
        = from_python(value, Type<ExtractType>());
    
    return none();
}

template <class T, class R, class V, class D>
PyObject* virtual_function<T,R,V,D>::do_call(PyObject* args, PyObject* keywords) const
{
    // If the target object is held by pointer, we must call through the virtual
    // function pointer to the most-derived override.
    PyObject* target = PyTuple_GetItem(args, 0);
    if (target != 0)
    {
        ExtensionInstance* self = get_extension_instance(target);
        if (self->wrapped_objects().size() == 1
            && !self->wrapped_objects()[0]->held_by_value())
        {
            return Caller<R>::call(m_virtual_function_ptr, args, keywords);
        }
    }
    return Caller<R>::call(m_default_implementation, args, keywords);
}
    
}} // namespace py::detail

#endif // FUNCTIONS_DWA051400_H_

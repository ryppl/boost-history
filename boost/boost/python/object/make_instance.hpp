// Copyright David Abrahams 2002. Permission to copy, use,
// modify, sell and distribute this software is granted provided this
// copyright notice appears in all copies. This software is provided
// "as is" without express or implied warranty, and with no claim as
// to its suitability for any purpose.
#ifndef MAKE_INSTANCE_DWA200296_HPP
# define MAKE_INSTANCE_DWA200296_HPP

# include <boost/python/object/instance.hpp>
# include <boost/python/converter/registered.hpp>
# include <boost/python/detail/decref_guard.hpp>

namespace boost { namespace python { namespace objects { 

template <class T, class Holder, class Derived>
struct make_instance_impl
{
    typedef objects::instance<Holder> instance_t;
        
    template <class Arg>
    static inline PyObject* execute(Arg& x)
    {
        BOOST_STATIC_ASSERT(is_class<T>::value);

        PyTypeObject* type = Derived::get_class_object(x);

        PyObject* raw_result = type->tp_alloc(
            type, objects::additional_instance_size<Holder>::value);
          
        if (raw_result != 0)
        {
            python::detail::decref_guard protect(raw_result);
            
            instance_t* instance = (instance_t*)raw_result;
            
            // construct the new C++ object and install the pointer
            // in the Python object.
            Derived::construct(&instance->storage, (PyObject*)instance, x)->install(raw_result);
              
            // Note the position of the internally-stored Holder,
            // for the sake of destruction
            instance->ob_size = offsetof(instance_t, storage);

            // Release ownership of the python object
            protect.cancel();
        }
        return raw_result;
    }
};
    

template <class T, class Holder>
struct make_instance
    : make_instance_impl<T, Holder, make_instance<T,Holder> >
{
    template <class U>
    static inline PyTypeObject* get_class_object(U&)
    {
        return converter::registered<T>::converters.get_class_object();
    }
    
    static inline Holder* construct(void* storage, PyObject* instance, reference_wrapper<T const> x)
    {
        return new (storage) Holder(instance, x);
    }
};
  

}}} // namespace boost::python::object

#endif // MAKE_INSTANCE_DWA200296_HPP

#ifndef CROSS_MODULE_HPP
# define CROSS_MODULE_HPP

# include <boost/python/class_builder.hpp>

namespace boost { namespace python {
    struct import_error : error_already_set {};
    struct export_error : error_already_set {};
}}

namespace boost { namespace python { namespace detail {

// Concept: throw exception if api_major is changed
//          show warning on stderr if api_minor is changed
const int export_converters_api_major = 4;
const int export_converters_api_minor = 1;
extern const char* converters_attribute_name;
void* import_converter_object(const std::string& module_name,
                              const std::string& py_class_name,
                              const std::string& attribute_name);
void check_export_converters_api(const int importing_major,
                                 const int importing_minor,
                                 const int imported_major,
                                 const int imported_minor);

}}}

// forward declaration
namespace boost { namespace python { namespace detail {
template <class T> class import_extension_class;
}}}

BOOST_PYTHON_BEGIN_CONVERSION_NAMESPACE

//QUESTIONMARK
//  This class is a look-alike of class python_extension_class_converters.
//  Is there a formal way to ensure that the siblings stay in sync?
template <class T>
class python_import_extension_class_converters
{
 public:

    friend python_import_extension_class_converters py_extension_class_converters(boost::python::type<T>, bool sig = false) {
        return python_import_extension_class_converters();
    }

    PyObject* to_python(const T& x) const {
        return boost::python::detail::import_extension_class<T>::get_converters()->to_python(x);
    }

    friend T* from_python(PyObject* p, boost::python::type<T*> t, bool sig = false) {
        return boost::python::detail::import_extension_class<T>::get_converters()->from_python_Ts(p, t);
    }
    friend const T* from_python(PyObject* p, boost::python::type<const T*> t, bool sig = false) {
        return boost::python::detail::import_extension_class<T>::get_converters()->from_python_cTs(p, t);
    }
    friend const T* from_python(PyObject* p, boost::python::type<const T*const&> t, bool sig = false) {
        return boost::python::detail::import_extension_class<T>::get_converters()->from_python_cTscr(p, t);
    }
    friend T* from_python(PyObject* p, boost::python::type<T* const&> t, bool sig = false) {
        return boost::python::detail::import_extension_class<T>::get_converters()->from_python_Tscr(p, t);
    }
    friend T& from_python(PyObject* p, boost::python::type<T&> t, bool sig = false) {
        return boost::python::detail::import_extension_class<T>::get_converters()->from_python_Tr(p, t);
    }
    friend const T& from_python(PyObject* p, boost::python::type<const T&> t, bool sig = false) {
        return boost::python::detail::import_extension_class<T>::get_converters()->from_python_cTr(p, t);
    }
    friend const T& from_python(PyObject* p, boost::python::type<T> t, bool sig = false) {
        return boost::python::detail::import_extension_class<T>::get_converters()->from_python_T(p, t);
    }

    friend std::auto_ptr<T>& from_python(PyObject* p, boost::python::type<std::auto_ptr<T>&> t, bool sig = false) {
        return boost::python::detail::import_extension_class<T>::get_converters()->from_python_aTr(p, t);
    }
    friend std::auto_ptr<T> from_python(PyObject* p, boost::python::type<std::auto_ptr<T> > t, bool sig = false) {
        return boost::python::detail::import_extension_class<T>::get_converters()->from_python_aT(p, t);
    }
    friend const std::auto_ptr<T>& from_python(PyObject* p, boost::python::type<const std::auto_ptr<T>&> t, bool sig = false) {
        return boost::python::detail::import_extension_class<T>::get_converters()->from_python_caTr(p, t);
    }
    friend PyObject* to_python(std::auto_ptr<T> x, bool sig = false) {
        return boost::python::detail::import_extension_class<T>::get_converters()->to_python(x);
    }

    friend boost::shared_ptr<T>& from_python(PyObject* p, boost::python::type<boost::shared_ptr<T>&> t, bool sig = false) {
        return boost::python::detail::import_extension_class<T>::get_converters()->from_python_sTr(p, t);
    }
    friend const boost::shared_ptr<T>& from_python(PyObject* p, boost::python::type<boost::shared_ptr<T> > t, bool sig = false) {
        return boost::python::detail::import_extension_class<T>::get_converters()->from_python_sT(p, t);
    }
    friend const boost::shared_ptr<T>& from_python(PyObject* p, boost::python::type<const boost::shared_ptr<T>&> t, bool sig = false) {
        return boost::python::detail::import_extension_class<T>::get_converters()->from_python_csTr(p, t);
    }
    friend PyObject* to_python(boost::shared_ptr<T> x, bool sig = false) {
        return boost::python::detail::import_extension_class<T>::get_converters()->to_python(x);
    }
};

BOOST_PYTHON_END_CONVERSION_NAMESPACE

namespace boost { namespace python {

BOOST_PYTHON_IMPORT_CONVERSION(python_import_extension_class_converters);

// A pointer to this class is exported/imported via the Python API.
// All functions are virtual. This is, what we really export/import
// is essentially just a pointer to a vtbl.
template <class T>
struct export_converter_object_base
{
    virtual int get_api_major() const { return detail::export_converters_api_major; }
    virtual int get_api_minor() const { return detail::export_converters_api_minor; }

    virtual PyObject* to_python(const T& x) = 0;

    virtual T* from_python_Ts(PyObject* p, boost::python::type<T*> t) = 0;
    virtual const T* from_python_cTs(PyObject* p, boost::python::type<const T*> t) = 0;
    virtual const T* from_python_cTscr(PyObject* p, boost::python::type<const T*const&> t) = 0;
    virtual T* from_python_Tscr(PyObject* p, boost::python::type<T* const&> t) = 0;
    virtual T& from_python_Tr(PyObject* p, boost::python::type<T&> t) = 0;
    virtual const T& from_python_cTr(PyObject* p, boost::python::type<const T&> t) = 0;
    virtual const T& from_python_T(PyObject* p, boost::python::type<T> t) = 0;

    virtual std::auto_ptr<T>& from_python_aTr(PyObject* p, boost::python::type<std::auto_ptr<T>&> t) = 0;
    virtual std::auto_ptr<T> from_python_aT(PyObject* p, boost::python::type<std::auto_ptr<T> > t) = 0;
    virtual const std::auto_ptr<T>& from_python_caTr(PyObject* p, boost::python::type<const std::auto_ptr<T>&> t) = 0;
    virtual PyObject* to_python(std::auto_ptr<T> x) = 0;

    virtual boost::shared_ptr<T>& from_python_sTr(PyObject* p, boost::python::type<boost::shared_ptr<T>&> t) = 0;
    virtual const boost::shared_ptr<T>& from_python_sT(PyObject* p, boost::python::type<boost::shared_ptr<T> > t) = 0;
    virtual const boost::shared_ptr<T>& from_python_csTr(PyObject* p, boost::python::type<const boost::shared_ptr<T>&> t) = 0;
    virtual PyObject* to_python(boost::shared_ptr<T> x) = 0;
};

// Converters to be used if T is not copyable.
template <class T>
struct export_converter_object_noncopyable : export_converter_object_base<T>
{
    virtual PyObject* to_python(const T& x) {
        PyErr_SetString(PyExc_RuntimeError,
          "to_python(const T&) converter not exported");
        throw import_error();
    }

    virtual T* from_python_Ts(PyObject* p, boost::python::type<T*> t) {
        return BOOST_PYTHON_CONVERSION::from_python(p, t);
    }
    virtual const T* from_python_cTs(PyObject* p, boost::python::type<const T*> t) {
        return BOOST_PYTHON_CONVERSION::from_python(p, t);
    }
    virtual const T* from_python_cTscr(PyObject* p, boost::python::type<const T*const&> t) {
        return BOOST_PYTHON_CONVERSION::from_python(p, t);
    }
    virtual T* from_python_Tscr(PyObject* p, boost::python::type<T* const&> t) {
        return BOOST_PYTHON_CONVERSION::from_python(p, t);
    }
    virtual T& from_python_Tr(PyObject* p, boost::python::type<T&> t) {
        return BOOST_PYTHON_CONVERSION::from_python(p, t);
    }
    virtual const T& from_python_cTr(PyObject* p, boost::python::type<const T&> t) {
        return BOOST_PYTHON_CONVERSION::from_python(p, t);
    }
    virtual const T& from_python_T(PyObject* p, boost::python::type<T> t) {
        return BOOST_PYTHON_CONVERSION::from_python(p, t);
    }

    virtual std::auto_ptr<T>& from_python_aTr(PyObject* p, boost::python::type<std::auto_ptr<T>&> t) {
        return BOOST_PYTHON_CONVERSION::from_python(p, t);
    }
    virtual std::auto_ptr<T> from_python_aT(PyObject* p, boost::python::type<std::auto_ptr<T> > t) {
        return BOOST_PYTHON_CONVERSION::from_python(p, t);
    }
    virtual const std::auto_ptr<T>& from_python_caTr(PyObject* p, boost::python::type<const std::auto_ptr<T>&> t) {
        return BOOST_PYTHON_CONVERSION::from_python(p, t);
    }
    virtual PyObject* to_python(std::auto_ptr<T> x) {
        return BOOST_PYTHON_CONVERSION::to_python(x);
    }

    virtual boost::shared_ptr<T>& from_python_sTr(PyObject* p, boost::python::type<boost::shared_ptr<T>&> t) {
        return BOOST_PYTHON_CONVERSION::from_python(p, t);
    }
    virtual const boost::shared_ptr<T>& from_python_sT(PyObject* p, boost::python::type<boost::shared_ptr<T> > t) {
        return BOOST_PYTHON_CONVERSION::from_python(p, t);
    }
    virtual const boost::shared_ptr<T>& from_python_csTr(PyObject* p, boost::python::type<const boost::shared_ptr<T>&> t) {
        return BOOST_PYTHON_CONVERSION::from_python(p, t);
    }
    virtual PyObject* to_python(boost::shared_ptr<T> x) {
        return BOOST_PYTHON_CONVERSION::to_python(x);
    }
};

// The addditional to_python() converter that can be used if T is copyable.
template <class T>
struct export_converter_object : export_converter_object_noncopyable<T>
{
    virtual PyObject* to_python(const T& x) {
        return BOOST_PYTHON_CONVERSION::py_extension_class_converters(boost::python::type<T>()).to_python(x);
    }
};

namespace detail {

//QUESTIONMARK
//  A stripped-down, modified version of class extension_class.
//  Would it make sense to establish a formal relationship
//  between the two classes?
template <class T>
class import_extension_class
    : public python_import_extension_class_converters<T>
{
 public:
    inline import_extension_class(const char* module, const char* py_class) {
      m_module = module;
      m_py_class = py_class;
    }

    static boost::python::export_converter_object_base<T>* get_converters();

  private:
    static std::string m_module;
    static std::string m_py_class;
    static boost::python::export_converter_object_base<T>* imported_converters;
};

template <class T> std::string import_extension_class<T>::m_module;
template <class T> std::string import_extension_class<T>::m_py_class;
template <class T>
boost::python::export_converter_object_base<T>*
import_extension_class<T>::imported_converters = 0;

template <class T>
boost::python::export_converter_object_base<T>*
import_extension_class<T>::get_converters() {
  if (imported_converters == 0) {
    void* cobject
      = import_converter_object(m_module, m_py_class,
                                converters_attribute_name);
    imported_converters
      = static_cast<boost::python::export_converter_object_base<T>*>(cobject);
    check_export_converters_api(
      export_converters_api_major,
      export_converters_api_minor,
      imported_converters->get_api_major(),
      imported_converters->get_api_minor());
  }
  return imported_converters;
}

}}} // namespace boost::python::detail

namespace boost { namespace python {

template <class T, class U>
void export_converters(class_builder<T, U>& cb)
{
    static export_converter_object<T> export_cvts;
    cb.add(
      ref(PyCObject_FromVoidPtr(reinterpret_cast<void*>(&export_cvts), NULL)),
      detail::converters_attribute_name);
}

template <class T, class U>
void export_converters_noncopyable(class_builder<T, U>& cb)
{
    static export_converter_object_noncopyable<T> export_cvts;
    cb.add(
      ref(PyCObject_FromVoidPtr(reinterpret_cast<void*>(&export_cvts), NULL)),
      detail::converters_attribute_name);
}

template <class T>
class import_converters
    : python_import_extension_class_converters<T>
{
 public:
    import_converters(const char* module, const char* py_class)
        : m_class(new detail::import_extension_class<T>(module, py_class))
    { }
 private:
    //QUESTIONMARK
    //reference<detail::import_extension_class<T> > m_class;
    boost::shared_ptr<detail::import_extension_class<T> > m_class;
};

}} // namespace boost::python

#endif // CROSS_MODULE_HPP

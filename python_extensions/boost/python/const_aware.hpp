#ifndef BOOST_PYTHON_CONST_AWARE_CLASS_HPP
#define BOOST_PYTHON_CONST_AWARE_CLASS_HPP

#include <boost/python.hpp>
#include <boost/function_types/parameter_types.hpp>
#include <boost/mpl/front.hpp>
#include <boost/type_traits/is_const.hpp>
#include <boost/type_traits/remove_reference.hpp>
#include <boost/static_assert.hpp>

namespace boost {
namespace python {

namespace const_aware_registry {

BOOST_PYTHON_DECL PyTypeObject * lookup(PyTypeObject * non_const);

BOOST_PYTHON_DECL void insert(object const & non_const, object const & const_);

} // namespace boost::python::const_aware_registry

namespace objects {

template <typename T, typename Holder>
struct make_const_ptr_instance
    : make_instance_impl<T, Holder, make_const_ptr_instance<T,Holder> >
{
    
    template <typename Arg>
    static inline Holder* construct(void* storage, PyObject*, Arg& x) {
        return new (storage) Holder(x);
    }

    template <typename Ptr>
    static inline PyTypeObject* get_class_object(Ptr const& x) {
        PyTypeObject * non_const = make_ptr_instance<T,Holder>::get_class_object(x);
        return const_aware_registry::lookup(non_const);
    }

#ifndef BOOST_PYTHON_NO_PY_SIGNATURES
    static inline PyTypeObject const* get_pytype()
    {
        return converter::registered<T>::converters.get_class_object();
    }
#endif
};

} // namespace boost::python::objects

namespace detail {

template <typename F>
struct is_const_method : public 
boost::is_const< 
    typename boost::remove_reference< 
        typename boost::mpl::front<
            boost::function_types::parameter_types<F>
            >::type
        >::type
    >
{};

struct make_const_reference_holder {

    template <typename T>
    static PyObject * execute(T* p) {
        typedef objects::pointer_holder<T*,T> holder_t;
        T* q = const_cast<T*>(p);
        return objects::make_const_ptr_instance<T, holder_t>::execute(q);
    }

};

} // namespace boost::python::detail

struct const_reference_existing_object {
    
    template <class T>
    struct apply
    {
        BOOST_STATIC_CONSTANT(
            bool, ok = is_pointer<T>::value || is_reference<T>::value);
        
        typedef typename mpl::if_c<
            ok
            , to_python_indirect<T, detail::make_const_reference_holder>
            , detail::reference_existing_object_requires_a_pointer_or_reference_return_type<T>
        >::type type;
    };

};

template <std::size_t owner_arg = 1, class BasePolicy_ = default_call_policies>
struct return_internal_const_reference
    : with_custodian_and_ward_postcall<0, owner_arg, BasePolicy_>
{
 private:
    BOOST_STATIC_CONSTANT(bool, legal = owner_arg > 0);
 public:
    typedef typename mpl::if_c<
        legal
        , const_reference_existing_object
        , detail::return_internal_reference_owner_arg_must_be_greater_than_zero<owner_arg>
    >::type result_converter;
};

template <
    typename W, 
    typename X1 = detail::not_specified,
    typename X2 = detail::not_specified,
    typename X3 = detail::not_specified
    >
class const_aware_class {
public:
    
    typedef typename objects::class_metadata<W,X1,X2,X3> metadata;
    typedef const_aware_class<W,X1,X2,X3> self;

    typedef class_<W,X1,X2,X3> const_class_t;
    typedef class_< W, typename metadata::held_type_arg, typename metadata::bases, boost::noncopyable
                    > non_const_class_t;

    const_class_t const_class;
    non_const_class_t non_const_class;

public: // constructors

    const_aware_class(char const * non_const_name, char const * const_name, char const* doc = 0) :
        const_class(const_name, doc), non_const_class(non_const_name, doc) { _register(); }

    const_aware_class(char const * non_const_name, char const * const_name, no_init_t n) :
        const_class(const_name, n), non_const_class(non_const_name, n) { _register(); }

    const_aware_class(char const * non_const_name, char const * const_name, char const * doc, no_init_t n) :
        const_class(const_name, doc, n), non_const_class(non_const_name, doc, n) { _register(); }

    template <typename DerivedT>
    const_aware_class(char const * non_const_name, char const * const_name, init_base<DerivedT> const & i) :
        const_class(const_name, i), non_const_class(non_const_name, i) { _register(); }

    template <typename DerivedT>
    const_aware_class(char const * non_const_name, char const * const_name, 
                      char const * doc, init_base<DerivedT> const & i) :
        const_class(const_name, doc, i), non_const_class(non_const_name, doc, i) { _register(); }

public: // member functions

    // Generic visitation
    template <class Derived>
    self& def(def_visitor<Derived> const& visitor)
    {
        visitor.visit(non_const_class);
        visitor.visit(const_class);
        return *this;
    }

    // Wrap a member function or a non-member function which can take
    // a T, T cv&, or T cv* as its first parameter, a callable
    // python object, or a generic visitor.
    template <class F>
    self& def(char const* name, F f)
    {
        this->def_impl(
            detail::unwrap_wrapper((W*)0)
          , name, f, detail::def_helper<char const*>(0), &f);
        return *this;
    }

    template <class A1, class A2>
    self& def(char const* name, A1 a1, A2 const& a2)
    {
        this->def_maybe_overloads(name, a1, a2, &a2);
        return *this;
    }

    template <class Fn, class A1, class A2>
    self& def(char const* name, Fn fn, A1 const& a1, A2 const& a2)
    {
        //  The arguments are definitely:
        //      def(name, function, policy, doc_string)
        //      def(name, function, doc_string, policy)

        this->def_impl(
            detail::unwrap_wrapper((W*)0)
          , name, fn
          , detail::def_helper<A1,A2>(a1,a2)
          , &fn);

        return *this;
    }

    template <class Fn, class A1, class A2, class A3>
    self& def(char const* name, Fn fn, A1 const& a1, A2 const& a2, A3 const& a3)
    {
        this->def_impl(
            detail::unwrap_wrapper((W*)0)
          , name, fn
          , detail::def_helper<A1,A2,A3>(a1,a2,a3)
          , &fn);

        return *this;
    }

public:

    template <typename D>
    self & def_readonly(char const* name, D const & d, char const* doc=0) {
        non_const_class.def_readonly(name, d, doc);
        const_class.def_readonly(name, d, doc);
        return *this;
    }

    template <typename D>
    self & def_readwrite(char const* name, D const & d, char const* doc=0) {
        non_const_class.def_readwrite(name, d, doc);
        const_class.def_readonly(name, d, doc);
        return *this;
    }

    template <typename D>
    self & def_readonly(char const* name, D & d, char const* doc=0) {
        non_const_class.def_readonly(name, d, doc);
        const_class.def_readonly(name, d, doc);
        return *this;
    }

    template <typename D>
    self & def_readwrite(char const* name, D & d, char const* doc=0) {
        non_const_class.def_readwrite(name, d, doc);
        const_class.def_readonly(name, d, doc);
        return *this;
    }

    template <class Get>
    self & add_property(char const* name, Get fget, char const* docstr = 0) {
        non_const_class.add_property(name, fget, docstr);
        const_class.add_property(name, fget, docstr);
        return *this;
    }

    template <class Get, class Set>
    self & add_property(char const* name, Get fget, Set fset, char const* docstr = 0) {
        non_const_class.add_property(name, fget, fset, docstr);
        const_class.add_property(name, fget, docstr);
        return *this;
    }
        
    template <class Get>
    self & add_static_property(char const* name, Get fget) {
        non_const_class.add_static_property(name, fget);
        const_class.add_static_property(name, fget);
        return *this;
    }

    template <class Get, class Set>
    self & add_static_property(char const* name, Get fget, Set fset) {
        non_const_class.add_static_property(name, fget, fset);
        const_class.add_static_property(name, fget, fset);
        return *this;
    }
        
    template <class U>
    self & setattr(char const* name, U const& x) {
        non_const_class.setattr(name, x);
        const_class.setattr(name, x);
        return *this;
    }
    
    self & enable_pickling() {
        non_const_class.enable_pickling();
        const_class.enable_pickling();
        return *this;
    }

    self & staticmethod(char const* name) {
        non_const_class.staticmethod(name);
        const_class.static_method(name);
        return *this;
    }

private:

    void _register() {
        const_aware_registry::insert(non_const_class, const_class);
    }

    //
    // These two overloads discriminate between def() as applied to a
    // generic visitor and everything else.
    //
    // @group def_impl {
    template <class T, class Helper, class LeafVisitor, class Visitor>
    inline void def_impl(
        T*
      , char const* name
      , LeafVisitor
      , Helper const& helper
      , def_visitor<Visitor> const* v
    )
    {
        v->visit(non_const_class, name, helper);
        v->visit(const_class, name, helper);
    }

    template <class T, class Fn, class Helper>
    inline void def_impl(
        T*
      , char const* name
      , Fn fn
      , Helper const& helper
      , ...
    )
    {
        object method = 
            make_function(
                fn
                , helper.policies()
                , helper.keywords()
                , detail::get_signature(fn, (T*)0)
            );
        objects::add_to_namespace(non_const_class, name, method, helper.doc());

        if (detail::is_const_method<Fn>::value) {
            objects::add_to_namespace(const_class, name, method, helper.doc());
        }

    }

    //
    // These two overloads handle the definition of default
    // implementation overloads for virtual functions. The second one
    // handles the case where no default implementation was specified.
    //
    // @group def_default {
    template <class Fn, class Helper>
    inline void def_default(
        char const* name
        , Fn
        , Helper const& helper
        , mpl::bool_<true>)
    {
        detail::error::virtual_function_default<W,Fn>::must_be_derived_class_member(
            helper.default_implementation());

        object default_method = make_function(
            helper.default_implementation(), helper.policies(), helper.keywords()
        );

        objects::add_to_namespace(non_const_class, name, default_method);

        if (detail::is_const_method<Fn>::value) {
            objects::add_to_namespace(const_class, name, default_method);
        }

    }
    
    template <class Fn, class Helper>
    inline void def_default(char const*, Fn, Helper const&, mpl::bool_<false>)
    { }
    // }
    
    //
    // These two overloads discriminate between def() as applied to
    // regular functions and def() as applied to the result of
    // BOOST_PYTHON_FUNCTION_OVERLOADS(). The final argument is used to
    // discriminate.
    //
    // @group def_maybe_overloads {
    template <class OverloadsT, class SigT>
    void def_maybe_overloads(
        char const* name
        , SigT sig
        , OverloadsT const& overloads
        , detail::overloads_base const*)

    {
        BOOST_STATIC_ASSERT(sizeof(SigT) < 0); // No support for overload macros (yet).
    }

    template <class Fn, class A1>
    void def_maybe_overloads(
        char const* name
        , Fn fn
        , A1 const& a1
        , ...)
    {
        this->def_impl(
            detail::unwrap_wrapper((W*)0)
          , name
          , fn
          , detail::def_helper<A1>(a1)
          , &fn
        );

    }
    // }
};

} // namespace boost::python
} // namespace boost


#endif // !BOOST_PYTHON_CONST_AWARE_CLASS_HPP

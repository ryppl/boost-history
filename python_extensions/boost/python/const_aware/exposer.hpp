// Copyright 2010 Jim Bosch
// Adapted from Boost.Python code, Copyright David Abrahams, 2004.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_PYTHON_CONST_AWARE_EXPOSER_HPP
#define BOOST_PYTHON_CONST_AWARE_EXPOSER_HPP

#include <boost/python.hpp>
#include <boost/python/const_aware/proxy_class.hpp>
#include <boost/python/const_aware/const_shared_ptr_to_python.hpp>
#include <boost/python/const_aware/shared_ptr_from_proxy.hpp>
#include <boost/function_types/parameter_types.hpp>
#include <boost/mpl/front.hpp>
#include <boost/type_traits/is_const.hpp>
#include <boost/type_traits/remove_reference.hpp>
#include <boost/static_assert.hpp>

namespace boost { namespace python { namespace const_aware {

template <typename F>
struct is_const_method
  :  boost::is_const< 
        typename boost::remove_reference< 
            typename boost::mpl::front<
                boost::function_types::parameter_types<F>
            >::type
        >::type
    >
{};

template <
    typename W, 
    typename X1 = detail::not_specified,
    typename X2 = detail::not_specified,
    typename X3 = detail::not_specified
    >
class exposer {
    typedef class_<W,X1,X2,X3> class_t;    
    typedef exposer<W,X1,X2,X3> self;
    typedef typename objects::class_metadata<W,X1,X2,X3> metadata;
    typedef typename metadata::wrapped wrapped;
    class_t m_class;
    proxy_class m_proxy;

public: // constructors

    exposer(class_t const & class_ref) : m_class(class_ref), m_proxy(m_class) {}

    // Construct with the class name, with or without docstring, and default __init__() function
    exposer(char const* name, char const* doc = 0) : m_class(name, doc), m_proxy(m_class) {}

    // Construct with class name, no docstring, and an uncallable __init__ function
    exposer(char const* name, no_init_t n) : m_class(name, n), m_proxy(m_class) {}

    // Construct with class name, docstring, and an uncallable __init__ function
    exposer(char const* name, char const* doc, no_init_t n) : m_class(name, doc, n), m_proxy(m_class) {}

    // Construct with class name and init<> function
    template <class DerivedT>
    exposer(char const* name, init_base<DerivedT> const& i) : m_class(name, i), m_proxy(m_class) {}

    // Construct with class name, docstring and init<> function
    template <class DerivedT>
    exposer(char const* name, char const* doc, init_base<DerivedT> const& i)
        : m_class(name, doc, i), m_proxy(m_class) {}

public: // miscellaneous

    class_t & main_class() { return m_class; }
    proxy_class & const_proxy() { return m_proxy; }

    self & enable_shared_ptr() {
        register_ptr_to_python< boost::shared_ptr<wrapped> >();
        const_aware::const_shared_ptr_to_python<wrapped>();
        converter::shared_ptr_from_python<wrapped const>();
        const_aware::shared_ptr_from_proxy<wrapped const>();
        return *this;
    }

    self & enable_pickling() {
        m_class.enable_pickling();
        return *this;
    }

    self & copy_method_to_proxy(char const * name) {
        m_proxy.use_method(name, m_class.attr(name));
        return *this;
    }

public: // member functions

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

public: // data members

    template <typename D>
    self & def_readonly(char const* name, D const & d, char const* doc=0) {
        m_class.def_readonly(name, d, doc);
        m_proxy.use_property(name, m_class.attr(name));
        return *this;
    }

    template <typename D>
    self & def_readwrite(char const* name, D const & d, char const* doc=0) {
        m_class.def_readwrite(name, d, doc);
        m_proxy.use_property(name, m_class.attr(name));
        return *this;
    }

    template <typename D>
    self & def_readonly(char const* name, D & d, char const* doc=0) {
        m_class.def_readonly(name, d, doc);
        m_proxy.use_property(name, m_class.attr(name));
        return *this;
    }

    template <typename D>
    self & def_readwrite(char const* name, D & d, char const* doc=0) {
        m_class.def_readwrite(name, d, doc);
        m_proxy.use_property(name, m_class.attr(name));
        return *this;
    }

    template <class Get>
    self & add_property(char const* name, Get fget, char const* docstr = 0) {
        m_class.add_property(name, fget, docstr);
        m_proxy.use_property(name, m_class.attr(name));
        return *this;
    }

    template <class Get, class Set>
    self & add_property(char const* name, Get fget, Set fset, char const* docstr = 0) {
        m_class.add_property(name, fget, fset, docstr);
        m_proxy.use_property(name, m_class.attr(name));
        return *this;
    }

    self & staticmethod(char const* name) {
        m_class.staticmethod(name);
        m_proxy.setattr(name, m_class.attr(name));
        return *this;
    }

private:

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
        BOOST_STATIC_ASSERT(sizeof(LeafVisitor) < 0); // No support for visitors (yet).
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
        objects::add_to_namespace(m_class, name, method, helper.doc());

        if (is_const_method<Fn>::value) {
            m_proxy.use_method(name, method);
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

        objects::add_to_namespace(m_class, name, default_method);

        if (is_const_method<Fn>::value) {
            m_proxy.use_method(name, default_method);
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

}}} // namespace boost::python::const_aware

#endif // !BOOST_PYTHON_CONST_AWARE_EXPOSER_HPP

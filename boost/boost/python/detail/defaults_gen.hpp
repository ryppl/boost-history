///////////////////////////////////////////////////////////////////////////////
//
// Copyright David Abrahams 2002. Permission to copy, use,
// modify, sell and distribute this software is granted provided this
// copyright notice appears in all copies. This software is provided
// "as is" without express or implied warranty, and with no claim as
// to its suitability for any purpose.
//
///////////////////////////////////////////////////////////////////////////////
#ifndef DEFAULTS_GEN_JDG20020807_HPP
#define DEFAULTS_GEN_JDG20020807_HPP

#include <boost/python/detail/preprocessor.hpp>
#include <boost/preprocessor/repeat.hpp>
#include <boost/preprocessor/enum.hpp>
#include <boost/preprocessor/enum_params.hpp>
#include <boost/preprocessor/tuple.hpp>
#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/arithmetic/sub.hpp>
#include <boost/preprocessor/arithmetic/add.hpp>
#include <boost/preprocessor/stringize.hpp>
#include <boost/preprocessor/inc.hpp>
#include <boost/preprocessor/empty.hpp>
#include <boost/preprocessor/comma_if.hpp>
#include <boost/config.hpp>
#include <boost/mpl/at.hpp>
#include <boost/mpl/front.hpp>

namespace boost { namespace python {

///////////////////////////////////////////////////////////////////////////////
//
//  func_stubs_base is used as a base class for all function stubs.
//
///////////////////////////////////////////////////////////////////////////////
    namespace detail
    {
        struct func_stubs_base {};
    }

template <class CallPoliciesT, class StubsT>
struct func_stubs_with_call_policies
: public detail::func_stubs_base
{
    typedef typename StubsT::nv_type    nv_type;
    typedef typename StubsT::v_type     v_type;

    func_stubs_with_call_policies(CallPoliciesT const& policies_, char const* doc_)
    : policies(policies_), doc(doc_) {}

    char const* doc_string() const
    { return doc; }

    CallPoliciesT
    call_policies() const
    { return policies; }

    CallPoliciesT policies;
    char const* doc;
};

}} // namespace boost::python



///////////////////////////////////////////////////////////////////////////////
#define BOOST_PYTHON_TYPEDEF_GEN(z, index, data)                                \
    typedef typename ::boost::mpl::at_c<                                        \
        BOOST_PP_ADD_D(1, index, data),                                         \
        SigT>::type BOOST_PP_CAT(T, index);                                     \

#define BOOST_PYTHON_FUNC_WRAPPER_GEN(z, index, data)                           \
    static RT BOOST_PP_CAT(func_, index) (                                      \
        BOOST_PYTHON_BINARY_ENUM(                                               \
            BOOST_PP_ADD_D(1, BOOST_PP_TUPLE_ELEM(3, 1, data), index), T, arg)) \
    {                                                                           \
        BOOST_PP_TUPLE_ELEM(3, 2, data)                                         \
        BOOST_PP_TUPLE_ELEM(3, 0, data)(                                        \
            BOOST_PP_ENUM_PARAMS(                                               \
                BOOST_PP_ADD_D(1, BOOST_PP_TUPLE_ELEM(3, 1, data), index),      \
                arg));                                                          \
    }

#define BOOST_PYTHON_GEN_FUNCTION(fname, fstubs_name, n_args, n_dflts, ret)     \
    struct fstubs_name {                                                        \
                                                                                \
        BOOST_STATIC_CONSTANT(int, n_funcs = BOOST_PP_INC(n_dflts));            \
        BOOST_STATIC_CONSTANT(int, max_args = n_funcs);                         \
                                                                                \
        template <typename SigT>                                                \
        struct gen {                                                            \
                                                                                \
            typedef typename ::boost::mpl::front<SigT>::type RT;                \
                                                                                \
            BOOST_PP_REPEAT_2ND(                                                \
                n_args,                                                         \
                BOOST_PYTHON_TYPEDEF_GEN,                                       \
                1)                                                              \
                                                                                \
            BOOST_PP_REPEAT_2ND(                                                \
                BOOST_PP_INC(n_dflts),                                          \
                BOOST_PYTHON_FUNC_WRAPPER_GEN,                                  \
                (fname, BOOST_PP_SUB_D(1, n_args, n_dflts), ret))               \
        };                                                                      \
    };                                                                          \

///////////////////////////////////////////////////////////////////////////////
#define BOOST_PYTHON_MEM_FUNC_WRAPPER_GEN(z, index, data)                       \
    static RT BOOST_PP_CAT(func_, index) (                                      \
        ClassT& obj BOOST_PP_COMMA_IF(                                          \
            BOOST_PP_ADD_D(1, BOOST_PP_TUPLE_ELEM(3, 1, data), index))          \
        BOOST_PYTHON_BINARY_ENUM(                                               \
            BOOST_PP_ADD_D(1, BOOST_PP_TUPLE_ELEM(3, 1, data), index), T, arg)  \
        )                                                                       \
    {                                                                           \
        BOOST_PP_TUPLE_ELEM(3, 2, data) obj.BOOST_PP_TUPLE_ELEM(3, 0, data)(    \
            BOOST_PP_ENUM_PARAMS(                                               \
                BOOST_PP_ADD_D(1, BOOST_PP_TUPLE_ELEM(3, 1, data), index), arg) \
        );                                                                      \
    }

#define BOOST_PYTHON_GEN_MEM_FUNCTION(fname, fstubs_name, n_args, n_dflts, ret) \
    struct fstubs_name {                                                        \
                                                                                \
        BOOST_STATIC_CONSTANT(int, n_funcs = BOOST_PP_INC(n_dflts));            \
        BOOST_STATIC_CONSTANT(int, max_args = n_funcs + 1);                     \
                                                                                \
        template <typename SigT>                                                \
        struct gen {                                                            \
                                                                                \
            typedef typename ::boost::mpl::front<SigT>::type RT;                \
            typedef typename ::boost::mpl::at_c<1, SigT>::type ClassT;          \
                                                                                \
            BOOST_PP_REPEAT_2ND(                                                \
                n_args,                                                         \
                BOOST_PYTHON_TYPEDEF_GEN,                                       \
                2)                                                              \
                                                                                \
            BOOST_PP_REPEAT_2ND(                                                \
                BOOST_PP_INC(n_dflts),                                          \
                BOOST_PYTHON_MEM_FUNC_WRAPPER_GEN,                              \
                (fname, BOOST_PP_SUB_D(1, n_args, n_dflts), ret))               \
        };                                                                      \
    };


///////////////////////////////////////////////////////////////////////////////
#if defined(BOOST_MSVC)

#define BOOST_PYTHON_GEN_FUNCTION_STUB(fname, fstubs_name, n_args, n_dflts)     \
    BOOST_PYTHON_GEN_FUNCTION                                                   \
        (fname, BOOST_PP_CAT(fstubs_name, _NV), n_args, n_dflts, return)        \
    BOOST_PYTHON_GEN_FUNCTION                                                   \
        (fname, BOOST_PP_CAT(fstubs_name, _V), n_args, n_dflts, ;)              \
    struct fstubs_name                                                          \
    : public boost::python::detail::func_stubs_base {                           \
                                                                                \
        typedef BOOST_PP_CAT(fstubs_name, _NV)  nv_type;                        \
        typedef BOOST_PP_CAT(fstubs_name, _V)   v_type;                         \
        typedef fstubs_name self_t;                                             \
                                                                                \
        fstubs_name(char const* doc_ = 0)                                       \
        : doc(doc_) {}                                                          \
                                                                                \
        char const* doc_string() const                                          \
        { return doc; }                                                         \
                                                                                \
        default_call_policies                                                   \
        call_policies() const                                                   \
        { return default_call_policies(); }                                     \
                                                                                \
        template <class CallPoliciesT>                                          \
        func_stubs_with_call_policies<CallPoliciesT, self_t>                    \
        operator[](CallPoliciesT const& policies) const                         \
        {                                                                       \
            return func_stubs_with_call_policies<CallPoliciesT, self_t>         \
                (policies, doc);                                                \
        }                                                                       \
                                                                                \
        char const* doc;                                                        \
    };                                                                          \

///////////////////////////////////////////////////////////////////////////////
#define BOOST_PYTHON_GEN_MEM_FUNCTION_STUB(fname, fstubs_name, n_args, n_dflts) \
    BOOST_PYTHON_GEN_MEM_FUNCTION                                               \
        (fname, BOOST_PP_CAT(fstubs_name, _NV), n_args, n_dflts, return)        \
    BOOST_PYTHON_GEN_MEM_FUNCTION                                               \
        (fname, BOOST_PP_CAT(fstubs_name, _V), n_args, n_dflts, ;)              \
    struct fstubs_name                                                          \
    : public boost::python::detail::func_stubs_base {                           \
                                                                                \
        typedef BOOST_PP_CAT(fstubs_name, _NV)  nv_type;                        \
        typedef BOOST_PP_CAT(fstubs_name, _V)   v_type;                         \
        typedef fstubs_name self_t;                                             \
                                                                                \
        fstubs_name(char const* doc_ = 0)                                       \
        : doc(doc_) {}                                                          \
                                                                                \
        char const* doc_string() const                                          \
        { return doc; }                                                         \
                                                                                \
        default_call_policies                                                   \
        call_policies() const                                                   \
        { return default_call_policies(); }                                     \
                                                                                \
        template <class CallPoliciesT>                                          \
        func_stubs_with_call_policies<CallPoliciesT, self_t>                    \
        operator[](CallPoliciesT const& policies) const                         \
        {                                                                       \
            return func_stubs_with_call_policies<CallPoliciesT, self_t>         \
                (policies, doc);                                                \
        }                                                                       \
                                                                                \
        char const* doc;                                                        \
    };                                                                          \

#else

///////////////////////////////////////////////////////////////////////////////
#define BOOST_PYTHON_GEN_FUNCTION_STUB(fname, fstubs_name, n_args, n_dflts)     \
    BOOST_PYTHON_GEN_FUNCTION                                                   \
        (fname, BOOST_PP_CAT(fstubs_name, _NV), n_args, n_dflts, return)        \
    struct fstubs_name                                                          \
    : public boost::python::detail::func_stubs_base {                           \
                                                                                \
        typedef BOOST_PP_CAT(fstubs_name, _NV)  nv_type;                        \
        typedef BOOST_PP_CAT(fstubs_name, _NV)  v_type;                         \
        typedef fstubs_name self_t;                                             \
                                                                                \
        fstubs_name(char const* doc_ = 0)                                       \
        : doc(doc_) {}                                                          \
                                                                                \
        char const* doc_string() const                                          \
        { return doc; }                                                         \
                                                                                \
        default_call_policies                                                   \
        call_policies() const                                                   \
        { return default_call_policies(); }                                     \
                                                                                \
        template <class CallPoliciesT>                                          \
        func_stubs_with_call_policies<CallPoliciesT, self_t>                    \
        operator[](CallPoliciesT const& policies) const                         \
        {                                                                       \
            return func_stubs_with_call_policies<CallPoliciesT, self_t>         \
                (policies, doc);                                                \
        }                                                                       \
                                                                                \
        char const* doc;                                                        \
    };                                                                          \

///////////////////////////////////////////////////////////////////////////////
#define BOOST_PYTHON_GEN_MEM_FUNCTION_STUB(fname, fstubs_name, n_args, n_dflts) \
    BOOST_PYTHON_GEN_MEM_FUNCTION                                               \
        (fname, BOOST_PP_CAT(fstubs_name, _NV), n_args, n_dflts, return)        \
    struct fstubs_name                                                          \
    : public boost::python::detail::func_stubs_base {                           \
                                                                                \
        typedef BOOST_PP_CAT(fstubs_name, _NV)  nv_type;                        \
        typedef BOOST_PP_CAT(fstubs_name, _NV)  v_type;                         \
        typedef fstubs_name self_t;                                             \
                                                                                \
        fstubs_name(char const* doc_ = 0)                                       \
        : doc(doc_) {}                                                          \
                                                                                \
        char const* doc_string() const                                          \
        { return doc; }                                                         \
                                                                                \
        default_call_policies                                                   \
        call_policies() const                                                   \
        { return default_call_policies(); }                                     \
                                                                                \
        template <class CallPoliciesT>                                          \
        func_stubs_with_call_policies<CallPoliciesT, self_t>                    \
        operator[](CallPoliciesT const& policies) const                         \
        {                                                                       \
            return func_stubs_with_call_policies<CallPoliciesT, self_t>         \
                (policies, doc);                                                \
        }                                                                       \
                                                                                \
        char const* doc;                                                        \
    };                                                                          \

#endif // defined(BOOST_MSVC)

///////////////////////////////////////////////////////////////////////////////
//
//  MAIN MACROS
//
//      Given generator_name, fname, min_args and max_args, These macros
//      generate function stubs that forward to a function or member function
//      named fname. max_args is the arity of the function or member function
//      fname. fname can have default arguments. min_args is the minimum
//      arity that fname can accept.
//
//      There are two versions:
//
//          1. BOOST_PYTHON_FUNCTION_OVERLOADS for free functions
//          2. BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS for member functions.
//
//      For instance, given a function:
//
//      int
//      foo(int a, char b = 1, unsigned c = 2, double d = 3)
//      {
//          return a + b + c + int(d);
//      }
//
//      The macro invocation:
//
//          BOOST_PYTHON_FUNCTION_OVERLOADS(foo_stubs, foo, 1, 4)
//
//      Generates this code:
//
//      struct foo_stubs_NV  {
//
//          static const int n_funcs = 4;
//          static const int max_args = n_funcs;
//
//          template <typename SigT>
//          struct gen {
//
//              typedef typename ::boost::mpl::at_c<0, SigT>::type RT;
//              typedef typename ::boost::mpl::at_c<1, SigT>::type T0;
//              typedef typename ::boost::mpl::at_c<2, SigT>::type T1;
//              typedef typename ::boost::mpl::at_c<3, SigT>::type T2;
//              typedef typename ::boost::mpl::at_c<4, SigT>::type T3;
//
//              static RT func_0(T0 arg0)
//              { return foo(arg0); }
//
//              static RT func_1(T0 arg0, T1 arg1)
//              { return foo(arg0, arg1); }
//
//              static RT func_2(T0 arg0, T1 arg1, T2 arg2)
//              { return foo(arg0, arg1, arg2); }
//
//              static RT func_3(T0 arg0, T1 arg1, T2 arg2, T3 arg3)
//              { return foo(arg0, arg1, arg2, arg3); }
//          };
//      };
//
//      struct foo_stubs
//      :   public boost::python::detail::func_stubs_base {
//
//          typedef foo_stubs_NV    nv_type;
//          typedef foo_stubs_NV    v_type;
//      };
//
//      The typedefs nv_type and v_type are used to handle compilers that
//      do not support void returns. The example above typedefs nv_type
//      and v_type to foo_stubs_NV. On compilers that do not support
//      void returns, there are two versions: foo_stubs_NV and foo_stubs_V.
//      The "V" version is almost identical to the "NV" version except
//      for the return type (void) and the lack of the return keyword.
//
///////////////////////////////////////////////////////////////////////////////
#define BOOST_PYTHON_FUNCTION_OVERLOADS(generator_name, fname, min_args, max_args)          \
    BOOST_PYTHON_GEN_FUNCTION_STUB(                                                         \
        fname,                                                                              \
        generator_name,                                                                     \
        max_args,                                                                           \
        BOOST_PP_SUB_D(1, max_args, min_args))

#define BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(generator_name, fname, min_args, max_args)   \
    BOOST_PYTHON_GEN_MEM_FUNCTION_STUB(                                                     \
        fname,                                                                              \
        generator_name,                                                                     \
        max_args,                                                                           \
        BOOST_PP_SUB_D(1, max_args, min_args))

// deprecated macro names (to be removed)
#define BOOST_PYTHON_FUNCTION_GENERATOR BOOST_PYTHON_FUNCTION_OVERLOADS
#define BOOST_PYTHON_MEM_FUN_GENERATOR BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS

///////////////////////////////////////////////////////////////////////////////
#endif // DEFAULTS_GEN_JDG20020807_HPP



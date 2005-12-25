///////////////////////////////////////////////////////////////////////////////
// foreach.hpp header file
//
// Copyright 2004 Eric Niebler.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef BOOST_FOREACH

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif

#include <cstddef>
#include <utility>  // for std::pair

#include <boost/config.hpp>
#include <boost/detail/workaround.hpp>

// Some compilers let us detect even const-qualified rvalues at compile-time
#if BOOST_WORKAROUND(BOOST_MSVC, >= 1310)                                                       \
 || BOOST_WORKAROUND(__GNUC__, >= 4)                                                            \
 || (BOOST_WORKAROUND(__GNUC__, == 3) && (__GNUC_MINOR__ >= 4))
# define BOOST_FOREACH_COMPILE_TIME_CONST_RVALUE_DETECTION
#else
// Some compilers allow temporaries to be bound to non-const references.
// These compilers make it impossible to for BOOST_FOREACH to detect
// temporaries and avoid reevaluation of the collection expression.
# if BOOST_WORKAROUND(BOOST_MSVC, <= 1300)                                                      \
  || BOOST_WORKAROUND(__BORLANDC__, BOOST_TESTED_AT(0x564))                                     \
  || (BOOST_WORKAROUND(BOOST_INTEL_CXX_VERSION, <= 700) && defined(_MSC_VER))                   \
  || BOOST_WORKAROUND(__SUNPRO_CC, BOOST_TESTED_AT(0x570))                                      \
  || BOOST_WORKAROUND(__DECCXX_VER, BOOST_TESTED_AT(60590042))
#  define BOOST_FOREACH_NO_RVALUE_DETECTION
# endif
// Some compilers do not correctly implement the lvalue/rvalue conversion
// rules of the ternary conditional operator.
# if defined(BOOST_FOREACH_NO_RVALUE_DETECTION)                                                 \
  || defined(BOOST_NO_SFINAE)                                                                   \
  || BOOST_WORKAROUND(BOOST_MSVC, BOOST_TESTED_AT(1400))                                        \
  || BOOST_WORKAROUND(BOOST_INTEL_WIN, <= 810)                                                  \
  || BOOST_WORKAROUND(__GNUC__, < 3)                                                            \
  || (BOOST_WORKAROUND(__GNUC__, == 3) && (__GNUC_MINOR__ <= 2))                                \
  || (BOOST_WORKAROUND(__GNUC__, == 3) && (__GNUC_MINOR__ <= 3) && defined(__APPLE_CC__))       \
  || BOOST_WORKAROUND(__IBMCPP__, BOOST_TESTED_AT(600))                                         \
  || BOOST_WORKAROUND(__MWERKS__, BOOST_TESTED_AT(0x3206))
#  define BOOST_FOREACH_NO_CONST_RVALUE_DETECTION
# else
#  define BOOST_FOREACH_RUN_TIME_CONST_RVALUE_DETECTION
# endif
#endif

#include <boost/mpl/if.hpp>
#include <boost/mpl/logical.hpp>
#include <boost/mpl/eval_if.hpp>
#include <boost/noncopyable.hpp>
#include <boost/range/end.hpp>
#include <boost/range/begin.hpp>
#include <boost/range/result_iterator.hpp>
#include <boost/type_traits/is_array.hpp>
#include <boost/type_traits/is_const.hpp>
#include <boost/type_traits/is_base_and_derived.hpp>
#include <boost/iterator/iterator_traits.hpp>
#include <boost/utility/addressof.hpp>

#ifdef BOOST_FOREACH_RUN_TIME_CONST_RVALUE_DETECTION
# include <new>
# include <boost/aligned_storage.hpp>
# include <boost/utility/enable_if.hpp>
# include <boost/type_traits/remove_const.hpp>
#endif

// This must be at global scope, hence the uglified name
enum boost_foreach_cheap_copy_argument_dependent_lookup_hack
{
    boost_foreach_cheap_copy_argument_dependent_lookup_hack_value
};

namespace boost
{

// forward declarations for iterator_range
template<typename T>
class iterator_range;

// forward declarations for sub_range
template<typename T>
class sub_range;

namespace foreach
{
    ///////////////////////////////////////////////////////////////////////////////
    // in_range
    //
    template<typename T>
    inline std::pair<T, T> in_range(T begin, T end)
    {
        return std::make_pair(begin, end);
    }

    ///////////////////////////////////////////////////////////////////////////////
    // boost::foreach::tag
    //
    typedef boost_foreach_cheap_copy_argument_dependent_lookup_hack tag;
    tag const adl = boost_foreach_cheap_copy_argument_dependent_lookup_hack_value;

    ///////////////////////////////////////////////////////////////////////////////
    // boost::foreach::has_cheap_copy
    //   Specialize this for user-defined collection types if they are inexpensive to copy.
    //   This tells BOOST_FOREACH it can avoid the rvalue/lvalue detection stuff.
    template<typename T>
    struct has_cheap_copy
        : boost::mpl::false_
    {
    };

    ///////////////////////////////////////////////////////////////////////////////
    // boost::foreach::is_noncopyable
    //   Specialize this for user-defined collection types if they cannot be copied.
    //   This also tells BOOST_FOREACH to avoid the rvalue/lvalue detection stuff.
    template<typename T>
    struct is_noncopyable
    #ifndef BOOST_BROKEN_IS_BASE_AND_DERIVED
        : boost::is_base_and_derived<boost::noncopyable, T>
    #else
        : boost::mpl::false_
    #endif
    {
    };

} // namespace foreach

} // namespace boost

///////////////////////////////////////////////////////////////////////////////
// boost_foreach_has_cheap_copy
//   Another customization point for the has_cheap_copy optimization,
//   this one works on legacy compilers. Overload boost_foreach_has_cheap_copy
//   at the global namespace for your type.
template<typename T>
inline boost::foreach::has_cheap_copy<T> *
boost_foreach_has_cheap_copy(T *&, ...) { return 0; }

template<typename T>
inline boost::mpl::true_ *
boost_foreach_has_cheap_copy(std::pair<T, T> *&, boost::foreach::tag) { return 0; }

template<typename T>
inline boost::mpl::true_ *
boost_foreach_has_cheap_copy(boost::iterator_range<T> *&, boost::foreach::tag) { return 0; }

template<typename T>
inline boost::mpl::true_ *
boost_foreach_has_cheap_copy(boost::sub_range<T> *&, boost::foreach::tag) { return 0; }

template<typename T>
inline boost::mpl::true_ *
boost_foreach_has_cheap_copy(T **, boost::foreach::tag) { return 0; }

template<typename T, std::size_t N>
inline boost::mpl::false_ *
boost_foreach_has_cheap_copy(T (*)[N], boost::foreach::tag) { return 0; }

///////////////////////////////////////////////////////////////////////////////
// boost_foreach_is_noncopyable
//   Another customization point for the is_noncopyable trait,
//   this one works on legacy compilers. Overload boost_foreach_is_noncopyable
//   at the global namespace for your type.
template<typename T>
inline boost::foreach::is_noncopyable<T> *
boost_foreach_is_noncopyable(T *&, ...) { return 0; }

namespace boost
{

namespace foreach_detail_
{

///////////////////////////////////////////////////////////////////////////////
// Define some utilities for assessing the properties of expressions
//
typedef char yes_type;
typedef char (&no_type)[2];
yes_type is_true(boost::mpl::true_ *);
no_type is_true(boost::mpl::false_ *);

// Extracts the desired property from the expression without evaluating it
#define BOOST_FOREACH_PROTECT(expr)                                                             \
    (static_cast<boost::mpl::bool_<1 == sizeof(boost::foreach_detail_::is_true(expr))> *>(0))

template<typename Bool1, typename Bool2>
inline boost::mpl::and_<Bool1, Bool2> *and_(Bool1 *, Bool2 *) { return 0; }

template<typename Bool1, typename Bool2, typename Bool3>
inline boost::mpl::and_<Bool1, Bool2, Bool3> *and_(Bool1 *, Bool2 *, Bool3 *) { return 0; }

template<typename Bool1, typename Bool2>
inline boost::mpl::or_<Bool1, Bool2> *or_(Bool1 *, Bool2 *) { return 0; }

template<typename Bool1, typename Bool2, typename Bool3>
inline boost::mpl::or_<Bool1, Bool2, Bool3> *or_(Bool1 *, Bool2 *, Bool3 *) { return 0; }

template<typename Bool>
inline boost::mpl::not_<Bool> *not_(Bool *) { return 0; }

template<typename T>
inline boost::mpl::false_ *is_rvalue(T &, int) { return 0; }

template<typename T>
inline boost::mpl::true_ *is_rvalue(T const &, ...) { return 0; }

template<typename T>
inline boost::is_array<T> *is_array(T const &) { return 0; }

template<typename T>
inline boost::is_const<T> *is_const(T &) { return 0; }

#ifndef BOOST_FOREACH_NO_RVALUE_DETECTION
template<typename T>
inline boost::mpl::true_ *is_const(T const &) { return 0; }
#endif

///////////////////////////////////////////////////////////////////////////////
// auto_any_t/auto_any
//  General utility for putting an object of any type into automatic storage
struct auto_any_base
{
    // auto_any_base must evaluate to false in boolean context so that
    // they can be declared in if() statements.
    operator bool() const
    {
        return false;
    }
};

template<typename T>
struct auto_any : auto_any_base
{
    auto_any(T const &t)
        : item(t)
    {
    }

    // temporaries of type auto_any will be bound to const auto_any_base
    // references, but we still want to be able to mutate the stored
    // data, so declare it as mutable.
    mutable T item;
};

typedef auto_any_base const &auto_any_t;

template<typename T, typename C>
inline BOOST_DEDUCED_TYPENAME boost::mpl::if_<C, T const, T>::type &auto_any_cast(auto_any_t a)
{
    return static_cast<auto_any<T> const &>(a).item;
}

typedef boost::mpl::true_ const_;

///////////////////////////////////////////////////////////////////////////////
// type2type
//
template<typename T, typename C = boost::mpl::false_>
struct type2type
    : boost::mpl::if_<C, T const, T>
{
};

template<typename T, typename C = boost::mpl::false_>
struct foreach_iterator
{
    typedef BOOST_DEDUCED_TYPENAME boost::mpl::eval_if<
        C
      , range_const_iterator<T>
      , range_iterator<T>
    >::type type;
};

template<typename T, typename C = boost::mpl::false_>
struct foreach_reference
    : iterator_reference<BOOST_DEDUCED_TYPENAME foreach_iterator<T, C>::type>
{
};

///////////////////////////////////////////////////////////////////////////////
// encode_type
//
template<typename T>
inline type2type<T> *encode_type(T &, boost::mpl::false_ *) { return 0; }

template<typename T>
inline type2type<T, const_> *encode_type(T const &, boost::mpl::true_ *) { return 0; }

///////////////////////////////////////////////////////////////////////////////
// set_false
//
inline bool set_false(bool &b) { return b = false; }

///////////////////////////////////////////////////////////////////////////////
// to_ptr
//
template<typename T>
inline T *&to_ptr(T const &)
{
    static T *t = 0;
    return t;
}

// Borland needs a little extra help with arrays
#if BOOST_WORKAROUND(__BORLANDC__, BOOST_TESTED_AT(0x564))
template<typename T,std::size_t N>
inline T (*to_ptr(T (&t)[N]))[N]  { return 0; }
#endif

///////////////////////////////////////////////////////////////////////////////
// derefof
//
template<typename T>
inline T &derefof(T *t)
{
    // This is a work-around for a compiler bug in Borland. If T* is a pointer to array type U(*)[N],
    // then dereferencing it results in a U* instead of U(&)[N]. The cast forces the issue.
    return reinterpret_cast<T &>(
        *const_cast<char *>(
            reinterpret_cast<char const volatile *>(t)
        )
    );
}

#ifdef BOOST_FOREACH_COMPILE_TIME_CONST_RVALUE_DETECTION
///////////////////////////////////////////////////////////////////////////////
// Detect at compile-time whether an expression yields an rvalue or
// an lvalue. This is rather non-standard, but some popular compilers
// accept it.
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// rvalue_probe
//
template<typename T>
struct rvalue_probe
{
    // can't ever return an array by value
    typedef BOOST_DEDUCED_TYPENAME boost::mpl::if_<boost::is_array<T>, int, T>::type rvalue;
    operator rvalue();
    operator T &() const;
};

template<typename T>
rvalue_probe<T> const make_probe(T const &t);

# define BOOST_FOREACH_IS_RVALUE(COL)                                                           \
    boost::foreach_detail_::and_(                                                               \
        boost::foreach_detail_::not_(boost::foreach_detail_::is_array(COL))                     \
      , BOOST_FOREACH_PROTECT(boost::foreach_detail_::is_rvalue(                                \
            (true ? boost::foreach_detail_::make_probe(COL) : (COL)), 0)))

#elif defined(BOOST_FOREACH_RUN_TIME_CONST_RVALUE_DETECTION)
///////////////////////////////////////////////////////////////////////////////
// Detect at run-time whether an expression yields an rvalue
// or an lvalue. This is 100% standard C++, but not all compilers
// accept it. Also, it causes FOREACH to break when used with non-
// copyable collection types.
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// rvalue_probe
//
struct rvalue_probe
{
    template<typename T>
    rvalue_probe(T const &t, bool &b)
        : ptemp(const_cast<BOOST_DEDUCED_TYPENAME boost::remove_const<T>::type *>(&t))
        , is_rvalue(b)
    {
    }

    template<typename U>
    operator U()
    {
        this->is_rvalue = true;
        return *static_cast<U *>(ptemp);
    }

    template<typename V>
    operator V &() const
    {
        return *static_cast<V *>(ptemp);
    }

private:
    void *ptemp;
    bool &is_rvalue;
};

///////////////////////////////////////////////////////////////////////////////
// simple_variant
//  holds either a T or a T*
template<typename T>
struct simple_variant
{
    simple_variant(T *t)
        : is_rvalue(false)
    {
        *static_cast<T **>(this->data.address()) = t;
    }

    simple_variant(T const &t)
        : is_rvalue(true)
    {
        ::new(this->data.address()) T(t);
    }

    simple_variant(simple_variant const &that)
        : is_rvalue(that.is_rvalue)
    {
        if(this->is_rvalue)
            ::new(this->data.address()) T(*that.get());
        else
            *static_cast<T **>(this->data.address()) = that.get();
    }

    ~simple_variant()
    {
        if(this->is_rvalue)
            this->get()->~T();
    }

    T *get() const
    {
        if(this->is_rvalue)
            return static_cast<T *>(this->data.address());
        else
            return *static_cast<T **>(this->data.address());
    }

private:
    enum size_type { size = sizeof(T) > sizeof(T*) ? sizeof(T) : sizeof(T*) };
    simple_variant &operator =(simple_variant const &); 
    bool const is_rvalue;
    mutable aligned_storage<size> data;
};

// If the collection is an array or is noncopyable, it must be an lvalue.
// If the collection is cheap-to-copy, treat it as an rvalue
template<typename LValue, typename IsCheap>
inline BOOST_DEDUCED_TYPENAME boost::enable_if<boost::mpl::or_<LValue, IsCheap>, IsCheap *>::type
should_copy_impl(LValue *, IsCheap *, bool *)
{
    return 0;
}

// Otherwise, we must determine at runtime whether it's an lvalue or rvalue
inline bool *
should_copy_impl(boost::mpl::false_ *, boost::mpl::false_ *, bool *is_rvalue)
{
    return is_rvalue;
}

#endif

///////////////////////////////////////////////////////////////////////////////
// contain
//
template<typename T>
inline auto_any<T> contain(T const &t, boost::mpl::true_ *) // rvalue
{
    return t;
}

template<typename T>
inline auto_any<T *> contain(T &t, boost::mpl::false_ *) // lvalue
{
    // Cannot seem to get sunpro to handle addressof() with array types.
    #if BOOST_WORKAROUND(__SUNPRO_CC, BOOST_TESTED_AT(0x570))
    return &t;
    #else
    return boost::addressof(t);
    #endif
}

#ifdef BOOST_FOREACH_RUN_TIME_CONST_RVALUE_DETECTION
template<typename T>
auto_any<simple_variant<T const> >
contain(T const &t, bool *rvalue)
{
    return *rvalue ? simple_variant<T const>(t) : simple_variant<T const>(&t);
}
#endif

/////////////////////////////////////////////////////////////////////////////
// begin
//
template<typename T, typename C>
inline auto_any<BOOST_DEDUCED_TYPENAME foreach_iterator<T, C>::type>
begin(auto_any_t col, type2type<T, C> *, boost::mpl::true_ *) // rvalue
{
    return boost::begin(auto_any_cast<T, C>(col));
}

template<typename T, typename C>
inline auto_any<BOOST_DEDUCED_TYPENAME foreach_iterator<T, C>::type>
begin(auto_any_t col, type2type<T, C> *, boost::mpl::false_ *) // lvalue
{
    typedef BOOST_DEDUCED_TYPENAME type2type<T, C>::type type;
    typedef BOOST_DEDUCED_TYPENAME foreach_iterator<T, C>::type iterator;
    return iterator(boost::begin(derefof(auto_any_cast<type *, boost::mpl::false_>(col))));
}

#ifdef BOOST_FOREACH_RUN_TIME_CONST_RVALUE_DETECTION
template<typename T>
auto_any<BOOST_DEDUCED_TYPENAME foreach_iterator<T, const_>::type>
begin(auto_any_t col, type2type<T, const_> *, bool *)
{
    return boost::begin(*auto_any_cast<simple_variant<T const>, boost::mpl::false_>(col).get());
}
#endif

///////////////////////////////////////////////////////////////////////////////
// end
//
template<typename T, typename C>
inline auto_any<BOOST_DEDUCED_TYPENAME foreach_iterator<T, C>::type>
end(auto_any_t col, type2type<T, C> *, boost::mpl::true_ *) // rvalue
{
    return boost::end(auto_any_cast<T, C>(col));
}

template<typename T, typename C>
inline auto_any<BOOST_DEDUCED_TYPENAME foreach_iterator<T, C>::type>
end(auto_any_t col, type2type<T, C> *, boost::mpl::false_ *) // lvalue
{
    typedef BOOST_DEDUCED_TYPENAME type2type<T, C>::type type;
    typedef BOOST_DEDUCED_TYPENAME foreach_iterator<T, C>::type iterator;
    return iterator(boost::end(derefof(auto_any_cast<type *, boost::mpl::false_>(col))));
}

#ifdef BOOST_FOREACH_RUN_TIME_CONST_RVALUE_DETECTION
template<typename T>
auto_any<BOOST_DEDUCED_TYPENAME foreach_iterator<T, const_>::type>
end(auto_any_t col, type2type<T, const_> *, bool *)
{
    return boost::end(*auto_any_cast<simple_variant<T const>, boost::mpl::false_>(col).get());
}
#endif

#ifndef BOOST_NO_FUNCTION_TEMPLATE_ORDERING
template<typename T, typename C>
inline auto_any<int>
end(auto_any_t col, type2type<T *, C> *, boost::mpl::true_ *) // null-terminated C-style strings
{
    return 0; // not used
}
#endif

///////////////////////////////////////////////////////////////////////////////
// done
//
template<typename T, typename C>
inline bool done(auto_any_t cur, auto_any_t end, type2type<T, C> *)
{
    typedef BOOST_DEDUCED_TYPENAME foreach_iterator<T, C>::type iter_t;
    return auto_any_cast<iter_t, boost::mpl::false_>(cur) == auto_any_cast<iter_t, boost::mpl::false_>(end);
}

#ifndef BOOST_NO_FUNCTION_TEMPLATE_ORDERING
template<typename T, typename C>
inline bool done(auto_any_t cur, auto_any_t, type2type<T *, C> *) // null-terminated C-style strings
{
    return ! *auto_any_cast<T *, boost::mpl::false_>(cur);
}
#endif

///////////////////////////////////////////////////////////////////////////////
// next
//
template<typename T, typename C>
inline void next(auto_any_t cur, type2type<T, C> *)
{
    typedef BOOST_DEDUCED_TYPENAME foreach_iterator<T, C>::type iter_t;
    ++auto_any_cast<iter_t, boost::mpl::false_>(cur);
}

///////////////////////////////////////////////////////////////////////////////
// deref
//
template<typename T, typename C>
inline BOOST_DEDUCED_TYPENAME foreach_reference<T, C>::type
deref(auto_any_t cur, type2type<T, C> *)
{
    typedef BOOST_DEDUCED_TYPENAME foreach_iterator<T, C>::type iter_t;
    return *auto_any_cast<iter_t, boost::mpl::false_>(cur);
}

} // namespace foreach_detail_
} // namespace boost

#ifdef BOOST_FOREACH_COMPILE_TIME_CONST_RVALUE_DETECTION
///////////////////////////////////////////////////////////////////////////////
// R-values and const R-values supported here with zero runtime overhead
///////////////////////////////////////////////////////////////////////////////

// A sneaky way to get the type of the collection without evaluating the expression
# define BOOST_FOREACH_TYPEOF(COL)                                                              \
    (true ? 0 : boost::foreach_detail_::encode_type(COL, boost::foreach_detail_::is_const(COL)))

// Evaluate the collection expression
# define BOOST_FOREACH_EVALUATE(COL)                                                            \
    (COL)

// No variable is needed to track the rvalue-ness of the collection expression
# define BOOST_FOREACH_HEADER()                                                                 \
    /**/

# define BOOST_FOREACH_SHOULD_COPY(COL)                                                         \
    (true ? 0 : boost::foreach_detail_::or_(                                                    \
        BOOST_FOREACH_IS_RVALUE(COL)                                                            \
      , boost_foreach_has_cheap_copy(                                                           \
            boost::foreach_detail_::to_ptr(COL)                                                 \
          , boost::foreach::adl)))

#elif defined(BOOST_FOREACH_RUN_TIME_CONST_RVALUE_DETECTION)
///////////////////////////////////////////////////////////////////////////////
// R-values and const R-values supported here
///////////////////////////////////////////////////////////////////////////////

// A sneaky way to get the type of the collection without evaluating the expression
# define BOOST_FOREACH_TYPEOF(COL)                                                              \
    (true ? 0 : boost::foreach_detail_::encode_type(COL, boost::foreach_detail_::is_const(COL)))

// Evaluate the collection expression, and detect if it is an lvalue or and rvalue
# define BOOST_FOREACH_EVALUATE(COL)                                                            \
    (true ? boost::foreach_detail_::rvalue_probe((COL), _foreach_is_rvalue) : (COL))

// Declare a variable to track the rvalue-ness of the collection expression
# define BOOST_FOREACH_HEADER()                                                                 \
    if (bool _foreach_is_rvalue = false) {} else

// The rvalue/lvalue-ness of the collection expression is determined dynamically, unless
// type type is an array or is noncopyable or is non-const, in which case we know it's an lvalue
# define BOOST_FOREACH_SHOULD_COPY(COL)                                                         \
    (boost::foreach_detail_::should_copy_impl(                                                  \
        true ? 0 : boost::foreach_detail_::or_(                                                 \
            boost::foreach_detail_::is_array(COL)                                               \
          , boost_foreach_is_noncopyable(                                                       \
                boost::foreach_detail_::to_ptr(COL)                                             \
              , boost::foreach::adl)                                                            \
          , boost::foreach_detail_::not_(boost::foreach_detail_::is_const(COL)))                \
      , true ? 0 : boost_foreach_has_cheap_copy(                                                \
            boost::foreach_detail_::to_ptr(COL)                                                 \
          , boost::foreach::adl)                                                                \
      , &_foreach_is_rvalue))

#elif !defined(BOOST_FOREACH_NO_RVALUE_DETECTION)
///////////////////////////////////////////////////////////////////////////////
// R-values supported here, const R-values NOT supported here
///////////////////////////////////////////////////////////////////////////////

// A sneaky way to get the type of the collection without evaluating the expression
# define BOOST_FOREACH_TYPEOF(COL)                                                              \
    (true ? 0 : boost::foreach_detail_::encode_type(COL, boost::foreach_detail_::is_const(COL)))

// Evaluate the collection expression
# define BOOST_FOREACH_EVALUATE(COL)                                                            \
    (COL)

// No variable is needed to track the rvalue-ness of the collection expression
# define BOOST_FOREACH_HEADER()                                                                 \
    /**/

// Determine whether the collection expression is an lvalue or an rvalue.
// NOTE: this gets the answer wrong for const rvalues.
# define BOOST_FOREACH_SHOULD_COPY(COL)                                                         \
    (true ? 0 : boost::foreach_detail_::or_(                                                    \
        boost::foreach_detail_::is_rvalue((COL), 0)                                             \
      , boost_foreach_has_cheap_copy(                                                           \
            boost::foreach_detail_::to_ptr(COL)                                                 \
          , boost::foreach::adl)))

#else
///////////////////////////////////////////////////////////////////////////////
// R-values NOT supported here
///////////////////////////////////////////////////////////////////////////////

// A sneaky way to get the type of the collection without evaluating the expression
# define BOOST_FOREACH_TYPEOF(COL)                                                              \
    (true ? 0 : boost::foreach_detail_::encode_type(COL, boost::foreach_detail_::is_const(COL)))

// Evaluate the collection expression
# define BOOST_FOREACH_EVALUATE(COL)                                                            \
    (COL)

// No variable is needed to track the rvalue-ness of the collection expression
# define BOOST_FOREACH_HEADER()                                                                 \
    /**/

// Can't use rvalues with BOOST_FOREACH (unless they have been flagged as cheap-to-copy)
# define BOOST_FOREACH_SHOULD_COPY(COL)                                                         \
    (true ? 0 : boost_foreach_has_cheap_copy(                                                   \
        boost::foreach_detail_::to_ptr(COL)                                                     \
      , boost::foreach::adl))

#endif


#define BOOST_FOREACH_CONTAIN(COL)                                                              \
    boost::foreach_detail_::contain(                                                            \
        BOOST_FOREACH_EVALUATE(COL)                                                             \
      , BOOST_FOREACH_SHOULD_COPY(COL))

#define BOOST_FOREACH_BEGIN(COL)                                                                \
    boost::foreach_detail_::begin(                                                              \
        _foreach_col                                                                            \
      , BOOST_FOREACH_TYPEOF(COL)                                                               \
      , BOOST_FOREACH_SHOULD_COPY(COL))

#define BOOST_FOREACH_END(COL)                                                                  \
    boost::foreach_detail_::end(                                                                \
        _foreach_col                                                                            \
      , BOOST_FOREACH_TYPEOF(COL)                                                               \
      , BOOST_FOREACH_SHOULD_COPY(COL))

#define BOOST_FOREACH_DONE(COL)                                                                 \
    boost::foreach_detail_::done(                                                               \
        _foreach_cur                                                                            \
      , _foreach_end                                                                            \
      , BOOST_FOREACH_TYPEOF(COL))

#define BOOST_FOREACH_NEXT(COL)                                                                 \
    boost::foreach_detail_::next(                                                               \
        _foreach_cur                                                                            \
      , BOOST_FOREACH_TYPEOF(COL))

#define BOOST_FOREACH_DEREF(COL)                                                                \
    boost::foreach_detail_::deref(                                                              \
        _foreach_cur                                                                            \
      , BOOST_FOREACH_TYPEOF(COL))

///////////////////////////////////////////////////////////////////////////////
// BOOST_FOREACH
//
//   For iterating over collections. Collections can be
//   arrays, null-terminated strings, or STL containers.
//   The loop variable can be a value or reference. For
//   example:
//
//   std::list<int> int_list(/*stuff*/);
//   BOOST_FOREACH(int &i, int_list)
//   {
//       /* 
//        * loop body goes here.
//        * i is a reference to the int in int_list.
//        */
//   }
//
//   Alternately, you can declare the loop variable first,
//   so you can access it after the loop finishes. Obviously,
//   if you do it this way, then the loop variable cannot be
//   a reference.
//
//   int i;
//   BOOST_FOREACH(i, int_list)
//       { ... }
//
#define BOOST_FOREACH(VAR, COL)                                                                 \
    BOOST_FOREACH_HEADER()                                                                      \
    if (boost::foreach_detail_::auto_any_t _foreach_col = BOOST_FOREACH_CONTAIN(COL)) {} else   \
    if (boost::foreach_detail_::auto_any_t _foreach_cur = BOOST_FOREACH_BEGIN(COL)) {} else     \
    if (boost::foreach_detail_::auto_any_t _foreach_end = BOOST_FOREACH_END(COL)) {} else       \
    for (bool _foreach_continue = true;                                                         \
              _foreach_continue && !BOOST_FOREACH_DONE(COL);                                    \
              _foreach_continue ? BOOST_FOREACH_NEXT(COL) : (void)0)                            \
        if  (boost::foreach_detail_::set_false(_foreach_continue)) {} else                      \
        for (VAR = BOOST_FOREACH_DEREF(COL); !_foreach_continue; _foreach_continue = true)

#endif

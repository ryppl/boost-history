//  Boost operators.hpp header file  ----------------------------------------//

//  (C) Copyright David Abrahams, Jeremy Siek, and Daryle Walker 1999-2001.
//  Permission to copy, use, modify, sell and distribute this software is
//  granted provided this copyright notice appears in all copies.  This
//  software is provided "as is" without express or implied warranty, and
//  with no claim as to its suitability for any purpose.

//  See http://www.boost.org for most recent version including documentation.

//  Revision History
//  21 Oct 02 Modified implementation of operators to allow compilers with a
//            correct named return value optimization (NRVO) to produce optimal
//            code.  (Daniel Frey)
//  02 Dec 01 Bug fixed in random_access_iteratable.  (Helmut Zeisel)
//  28 Sep 01 Factored out iterator operator groups.  (Daryle Walker)
//  27 Aug 01 'left' form for non commutative operators added;
//            additional classes for groups of related operators added;
//            workaround for empty base class optimization
//            bug of GCC 3.0 (Helmut Zeisel)
//  25 Jun 01 output_iterator_helper changes: removed default template 
//            parameters, added support for self-proxying, additional 
//            documentation and tests (Aleksey Gurtovoy)
//  29 May 01 Added operator classes for << and >>.  Added input and output
//            iterator helper classes.  Added classes to connect equality and
//            relational operators.  Added classes for groups of related
//            operators.  Reimplemented example operator and iterator helper
//            classes in terms of the new groups.  (Daryle Walker, with help
//            from Alexy Gurtovoy)
//  11 Feb 01 Fixed bugs in the iterator helpers which prevented explicitly
//            supplied arguments from actually being used (Dave Abrahams)
//  04 Jul 00 Fixed NO_OPERATORS_IN_NAMESPACE bugs, major cleanup and
//            refactoring of compiler workarounds, additional documentation
//            (Alexy Gurtovoy and Mark Rodgers with some help and prompting from
//            Dave Abrahams) 
//  28 Jun 00 General cleanup and integration of bugfixes from Mark Rodgers and
//            Jeremy Siek (Dave Abrahams)
//  20 Jun 00 Changes to accommodate Borland C++Builder 4 and Borland C++ 5.5
//            (Mark Rodgers)
//  20 Jun 00 Minor fixes to the prior revision (Aleksey Gurtovoy)
//  10 Jun 00 Support for the base class chaining technique was added
//            (Aleksey Gurtovoy). See documentation and the comments below 
//            for the details. 
//  12 Dec 99 Initial version with iterator operators (Jeremy Siek)
//  18 Nov 99 Change name "divideable" to "dividable", remove unnecessary
//            specializations of dividable, subtractable, modable (Ed Brey) 
//  17 Nov 99 Add comments (Beman Dawes)
//            Remove unnecessary specialization of operators<> (Ed Brey)
//  15 Nov 99 Fix less_than_comparable<T,U> second operand type for first two
//            operators.(Beman Dawes)
//  12 Nov 99 Add operators templates (Ed Brey)
//  11 Nov 99 Add single template parameter version for compilers without
//            partial specialization (Beman Dawes)
//  10 Nov 99 Initial version

// 10 Jun 00:
// An additional optional template parameter was added to most of 
// operator templates to support the base class chaining technique (see 
// documentation for the details). Unfortunately, a straightforward
// implementation of this change would have broken compatibility with the
// previous version of the library by making it impossible to use the same
// template name (e.g. 'addable') for both the 1- and 2-argument versions of
// an operator template. This implementation solves the backward-compatibility
// issue at the cost of some simplicity.
//
// One of the complications is an existence of special auxiliary class template
// 'is_chained_base<>' (see 'detail' namespace below), which is used
// to determine whether its template parameter is a library's operator template
// or not. You have to specialize 'is_chained_base<>' for each new 
// operator template you add to the library.
//
// However, most of the non-trivial implementation details are hidden behind 
// several local macros defined below, and as soon as you understand them,
// you understand the whole library implementation. 

#ifndef BOOST_OPERATORS_HPP
#define BOOST_OPERATORS_HPP

#include <boost/config.hpp>
#include <boost/iterator.hpp>

#if defined(__sgi) && !defined(__GNUC__)
#pragma set woff 1234
#endif

#if defined(BOOST_MSVC)
#   pragma warning( disable : 4284 ) // complaint about return type of 
#endif                               // operator-> not begin a UDT

namespace boost {
namespace detail {

// Helmut Zeisel, empty base class optimization bug with GCC 3.0.0
#if defined(__GNUC__) && __GNUC__==3 && __GNUC_MINOR__==0 && __GNU_PATCHLEVEL__==0
class empty_base {
  bool dummy; 
};
#else
class empty_base {};
#endif

} // namespace detail
} // namespace boost

// In this section we supply the xxxx1 and xxxx2 forms of the operator
// templates, which are explicitly targeted at the 1-type-argument and
// 2-type-argument operator forms, respectively. Some compilers get confused
// when inline friend functions are overloaded in namespaces other than the
// global namespace. When BOOST_NO_OPERATORS_IN_NAMESPACE is defined, all of
// these templates must go in the global namespace.

#ifndef BOOST_NO_OPERATORS_IN_NAMESPACE
namespace boost
{
#endif

//  Basic operator classes (contributed by Dave Abrahams) ------------------//

//  Note that friend functions defined in a class are implicitly inline.
//  See the C++ std, 11.4 [class.friend] paragraph 5

template <class T, class U, class B = ::boost::detail::empty_base>
struct less_than_comparable2 : B
{
     friend bool operator<=(const T& x, const U& y) { return !(x > y); }
     friend bool operator>=(const T& x, const U& y) { return !(x < y); }
     friend bool operator>(const U& x, const T& y)  { return y < x; }
     friend bool operator<(const U& x, const T& y)  { return y > x; }
     friend bool operator<=(const U& x, const T& y) { return !(y < x); }
     friend bool operator>=(const U& x, const T& y) { return !(y > x); }
};

template <class T, class B = ::boost::detail::empty_base>
struct less_than_comparable1 : B
{
     friend bool operator>(const T& x, const T& y)  { return y < x; }
     friend bool operator<=(const T& x, const T& y) { return !(y < x); }
     friend bool operator>=(const T& x, const T& y) { return !(x < y); }
};

template <class T, class U, class B = ::boost::detail::empty_base>
struct equality_comparable2 : B
{
     friend bool operator==(const U& y, const T& x) { return x == y; }
     friend bool operator!=(const U& y, const T& x) { return !(x == y); }
     friend bool operator!=(const T& y, const U& x) { return !(y == x); }
};

template <class T, class B = ::boost::detail::empty_base>
struct equality_comparable1 : B
{
     friend bool operator!=(const T& x, const T& y) { return !(x == y); }
};

//  NRVO-friendly implementation (contributed by Daniel Frey) ---------------//

#if defined(BOOST_HAS_NRVO) || defined(BOOST_FORCE_SYMMETRIC_OPERATORS)

// This is the optimal implementation for ISO/ANSI C++,
// but it requires the compiler to implement the NRVO.
// If the compiler has no NRVO, this is the best symmetric
// implementation available.

#define BOOST_BINARY_OPERATOR_COMMUTATIVE( NAME, OP )                         \
template <class T, class U, class B = ::boost::detail::empty_base>            \
struct NAME##2 : B                                                            \
{                                                                             \
  friend T operator OP( const T& lhs, const U& rhs )                          \
    { T nrv( lhs ); nrv OP##= rhs; return nrv; }                              \
  friend T operator OP( const U& lhs, const T& rhs )                          \
    { T nrv( rhs ); nrv OP##= lhs; return nrv; }                              \
};                                                                            \
                                                                              \
template <class T, class B = ::boost::detail::empty_base>                     \
struct NAME##1 : B                                                            \
{                                                                             \
  friend T operator OP( const T& lhs, const T& rhs )                          \
    { T nrv( lhs ); nrv OP##= rhs; return nrv; }                              \
};

#define BOOST_BINARY_OPERATOR_NON_COMMUTATIVE( NAME, OP )                     \
template <class T, class U, class B = ::boost::detail::empty_base>            \
struct NAME##2 : B                                                            \
{                                                                             \
  friend T operator OP( const T& lhs, const U& rhs )                          \
    { T nrv( lhs ); nrv OP##= rhs; return nrv; }                              \
};                                                                            \
                                                                              \
template <class T, class U, class B = ::boost::detail::empty_base>            \
struct NAME##2_left : B                                                       \
{                                                                             \
  friend T operator OP( const U& lhs, const T& rhs )                          \
    { T nrv( lhs ); nrv OP##= rhs; return nrv; }                              \
};                                                                            \
                                                                              \
template <class T, class B = ::boost::detail::empty_base>                     \
struct NAME##1 : B                                                            \
{                                                                             \
  friend T operator OP( const T& lhs, const T& rhs )                          \
    { T nrv( lhs ); nrv OP##= rhs; return nrv; }                              \
};

#else // defined(BOOST_HAS_NRVO) || defined(BOOST_FORCE_SYMMETRIC_OPERATORS)

// For compilers without NRVO the following code is optimal, but not symmetric!
// Note that the implementation of NAME##2_left only looks cool, but doesn't
// provide optimization opportunities to the compiler :)

#define BOOST_BINARY_OPERATOR_COMMUTATIVE( NAME, OP )                         \
template <class T, class U, class B = ::boost::detail::empty_base>            \
struct NAME##2 : B                                                            \
{                                                                             \
  friend T operator OP( T lhs, const U& rhs ) { return lhs OP##= rhs; }       \
  friend T operator OP( const U& lhs, T rhs ) { return rhs OP##= lhs; }       \
};                                                                            \
                                                                              \
template <class T, class B = ::boost::detail::empty_base>                     \
struct NAME##1 : B                                                            \
{                                                                             \
  friend T operator OP( T lhs, const T& rhs ) { return lhs OP##= rhs; }       \
};

#define BOOST_BINARY_OPERATOR_NON_COMMUTATIVE( NAME, OP )                     \
template <class T, class U, class B = ::boost::detail::empty_base>            \
struct NAME##2 : B                                                            \
{                                                                             \
  friend T operator OP( T lhs, const U& rhs ) { return lhs OP##= rhs; }       \
};                                                                            \
                                                                              \
template <class T, class U, class B = ::boost::detail::empty_base>            \
struct NAME##2_left : B                                                       \
{                                                                             \
  friend T operator OP( const U& lhs, const T& rhs )                          \
    { return T( lhs ) OP##= rhs; }                                            \
};                                                                            \
                                                                              \
template <class T, class B = ::boost::detail::empty_base>                     \
struct NAME##1 : B                                                            \
{                                                                             \
  friend T operator OP( T lhs, const T& rhs ) { return lhs OP##= rhs; }       \
};

#endif // defined(BOOST_HAS_NRVO) || defined(BOOST_FORCE_SYMMETRIC_OPERATORS)

BOOST_BINARY_OPERATOR_COMMUTATIVE( multipliable, * )
BOOST_BINARY_OPERATOR_COMMUTATIVE( addable, + )
BOOST_BINARY_OPERATOR_NON_COMMUTATIVE( subtractable, - )
BOOST_BINARY_OPERATOR_NON_COMMUTATIVE( dividable, / )
BOOST_BINARY_OPERATOR_NON_COMMUTATIVE( modable, % )
BOOST_BINARY_OPERATOR_COMMUTATIVE( xorable, ^ )
BOOST_BINARY_OPERATOR_COMMUTATIVE( andable, & )
BOOST_BINARY_OPERATOR_COMMUTATIVE( orable, | )

#undef BOOST_BINARY_OPERATOR_COMMUTATIVE
#undef BOOST_BINARY_OPERATOR_NON_COMMUTATIVE

//  incrementable and decrementable contributed by Jeremy Siek

template <class T, class B = ::boost::detail::empty_base>
struct incrementable : B
{
  friend T operator++(T& x, int)
  {
    incrementable_type nrv(x);
    ++x;
    return nrv;
  }
private: // The use of this typedef works around a Borland bug
  typedef T incrementable_type;
};

template <class T, class B = ::boost::detail::empty_base>
struct decrementable : B
{
  friend T operator--(T& x, int)
  {
    decrementable_type nrv(x);
    --x;
    return nrv;
  }
private: // The use of this typedef works around a Borland bug
  typedef T decrementable_type;
};

//  Iterator operator classes (contributed by Jeremy Siek) ------------------//

template <class T, class P, class B = ::boost::detail::empty_base>
struct dereferenceable : B
{
  P operator->() const
  { 
    return &*static_cast<const T&>(*this); 
  }
};

template <class T, class I, class R, class B = ::boost::detail::empty_base>
struct indexable : B
{
  R operator[](I n) const
  {
    return *(static_cast<const T&>(*this) + n);
  }
};

//  More operator classes (contributed by Daryle Walker) --------------------//
//  (NRVO-friendly implementation contributed by Daniel Frey) ---------------//

#if defined(BOOST_HAS_NRVO)

#define BOOST_BINARY_OPERATOR( NAME, OP )                                     \
template <class T, class U, class B = ::boost::detail::empty_base>            \
struct NAME##2 : B                                                            \
{                                                                             \
  friend T operator OP( const T& lhs, const U& rhs )                          \
    { T nrv( lhs ); nrv OP##= rhs; return nrv; }                              \
};                                                                            \
                                                                              \
template <class T, class B = ::boost::detail::empty_base>                     \
struct NAME##1 : B                                                            \
{                                                                             \
  friend T operator OP( const T& lhs, const T& rhs )                          \
    { T nrv( lhs ); nrv OP##= rhs; return nrv; }                              \
};

#else // defined(BOOST_HAS_NRVO)

#define BOOST_BINARY_OPERATOR( NAME, OP )                                     \
template <class T, class U, class B = ::boost::detail::empty_base>            \
struct NAME##2 : B                                                            \
{                                                                             \
  friend T operator OP( T lhs, const U& rhs ) { return lhs OP##= rhs; }       \
};                                                                            \
                                                                              \
template <class T, class B = ::boost::detail::empty_base>                     \
struct NAME##1 : B                                                            \
{                                                                             \
  friend T operator OP( T lhs, const T& rhs ) { return lhs OP##= rhs; }       \
};

#endif // defined(BOOST_HAS_NRVO)

BOOST_BINARY_OPERATOR( left_shiftable, << )
BOOST_BINARY_OPERATOR( right_shiftable, >> )

#undef BOOST_BINARY_OPERATOR

template <class T, class U, class B = ::boost::detail::empty_base>
struct equivalent2 : B
{
  friend bool operator==(const T& x, const U& y)
  {
    return !(x < y) && !(x > y);
  }
};

template <class T, class B = ::boost::detail::empty_base>
struct equivalent1 : B
{
  friend bool operator==(const T&x, const T&y)
  {
    return !(x < y) && !(y < x);
  }
};

template <class T, class U, class B = ::boost::detail::empty_base>
struct partially_ordered2 : B
{
  friend bool operator<=(const T& x, const U& y)
    { return (x < y) || (x == y); }
  friend bool operator>=(const T& x, const U& y)
    { return (x > y) || (x == y); }
  friend bool operator>(const U& x, const T& y)
    { return y < x; }
  friend bool operator<(const U& x, const T& y)
    { return y > x; }
  friend bool operator<=(const U& x, const T& y)
    { return (y > x) || (y == x); }
  friend bool operator>=(const U& x, const T& y)
    { return (y < x) || (y == x); }
};

template <class T, class B = ::boost::detail::empty_base>
struct partially_ordered1 : B
{
  friend bool operator>(const T& x, const T& y)
    { return y < x; }
  friend bool operator<=(const T& x, const T& y)
    { return (x < y) || (x == y); }
  friend bool operator>=(const T& x, const T& y)
    { return (y < x) || (x == y); }
};

//  Combined operator classes (contributed by Daryle Walker) ----------------//

template <class T, class U, class B = ::boost::detail::empty_base>
struct totally_ordered2
    : less_than_comparable2<T, U
    , equality_comparable2<T, U, B
      > > {};

template <class T, class B = ::boost::detail::empty_base>
struct totally_ordered1
    : less_than_comparable1<T
    , equality_comparable1<T, B
      > > {};

template <class T, class U, class B = ::boost::detail::empty_base>
struct additive2
    : addable2<T, U
    , subtractable2<T, U, B
      > > {};

template <class T, class B = ::boost::detail::empty_base>
struct additive1
    : addable1<T
    , subtractable1<T, B
      > > {};

template <class T, class U, class B = ::boost::detail::empty_base>
struct multiplicative2
    : multipliable2<T, U
    , dividable2<T, U, B
      > > {};

template <class T, class B = ::boost::detail::empty_base>
struct multiplicative1
    : multipliable1<T
    , dividable1<T, B
      > > {};

template <class T, class U, class B = ::boost::detail::empty_base>
struct integer_multiplicative2
    : multiplicative2<T, U
    , modable2<T, U, B
      > > {};

template <class T, class B = ::boost::detail::empty_base>
struct integer_multiplicative1
    : multiplicative1<T
    , modable1<T, B
      > > {};

template <class T, class U, class B = ::boost::detail::empty_base>
struct arithmetic2
    : additive2<T, U
    , multiplicative2<T, U, B
      > > {};

template <class T, class B = ::boost::detail::empty_base>
struct arithmetic1
    : additive1<T
    , multiplicative1<T, B
      > > {};

template <class T, class U, class B = ::boost::detail::empty_base>
struct integer_arithmetic2
    : additive2<T, U
    , integer_multiplicative2<T, U, B
      > > {};

template <class T, class B = ::boost::detail::empty_base>
struct integer_arithmetic1
    : additive1<T
    , integer_multiplicative1<T, B
      > > {};

template <class T, class U, class B = ::boost::detail::empty_base>
struct bitwise2
    : xorable2<T, U
    , andable2<T, U
    , orable2<T, U, B
      > > > {};

template <class T, class B = ::boost::detail::empty_base>
struct bitwise1
    : xorable1<T
    , andable1<T
    , orable1<T, B
      > > > {};

template <class T, class B = ::boost::detail::empty_base>
struct unit_steppable
    : incrementable<T
    , decrementable<T, B
      > > {};

template <class T, class U, class B = ::boost::detail::empty_base>
struct shiftable2
    : left_shiftable2<T, U
    , right_shiftable2<T, U, B
      > > {};

template <class T, class B = ::boost::detail::empty_base>
struct shiftable1
    : left_shiftable1<T
    , right_shiftable1<T, B
      > > {};

template <class T, class U, class B = ::boost::detail::empty_base>
struct ring_operators2
    : additive2<T, U
    , subtractable2_left<T, U
    , multipliable2<T, U, B
      > > > {};

template <class T, class B = ::boost::detail::empty_base>
struct ring_operators1
    : additive1<T
    , multipliable1<T, B
      > > {};

template <class T, class U, class B = ::boost::detail::empty_base>
struct ordered_ring_operators2
    : ring_operators2<T, U
    , totally_ordered2<T, U, B
      > > {};

template <class T, class B = ::boost::detail::empty_base>
struct ordered_ring_operators1
    : ring_operators1<T
    , totally_ordered1<T, B
      > > {};

template <class T, class U, class B = ::boost::detail::empty_base>
struct field_operators2
    : ring_operators2<T, U
    , dividable2<T, U
    , dividable2_left<T, U, B
      > > > {};

template <class T, class B = ::boost::detail::empty_base>
struct field_operators1
    : ring_operators1<T
    , dividable1<T, B
      > > {};

template <class T, class U, class B = ::boost::detail::empty_base>
struct ordered_field_operators2
    : field_operators2<T, U
    , totally_ordered2<T, U, B
      > > {};

template <class T, class B = ::boost::detail::empty_base>
struct ordered_field_operators1
    : field_operators1<T
    , totally_ordered1<T, B
      > > {};

template <class T, class U, class B = ::boost::detail::empty_base>
struct euclidian_ring_operators2
    : ring_operators2<T, U
    , dividable2<T, U
    , dividable2_left<T, U
    , modable2<T, U
    , modable2_left<T, U, B
      > > > > > {};

template <class T, class B = ::boost::detail::empty_base>
struct euclidian_ring_operators1
    : ring_operators1<T
    , dividable1<T
    , modable1<T, B
      > > > {};

template <class T, class U, class B = ::boost::detail::empty_base>
struct ordered_euclidian_ring_operators2
    : totally_ordered2<T, U
    , euclidian_ring_operators2<T, U, B
      > > {};

template <class T, class B = ::boost::detail::empty_base>
struct ordered_euclidian_ring_operators1
    : totally_ordered1<T
    , euclidian_ring_operators1<T, B
      > > {};
      
template <class T, class P, class B = ::boost::detail::empty_base>
struct input_iteratable
    : equality_comparable1<T
    , incrementable<T
    , dereferenceable<T, P, B
      > > > {};

template <class T, class B = ::boost::detail::empty_base>
struct output_iteratable
    : incrementable<T, B
      > {};

template <class T, class P, class B = ::boost::detail::empty_base>
struct forward_iteratable
    : input_iteratable<T, P, B
      > {};

template <class T, class P, class B = ::boost::detail::empty_base>
struct bidirectional_iteratable
    : forward_iteratable<T, P
    , decrementable<T, B
      > > {};

//  To avoid repeated derivation from equality_comparable,
//  which is an indirect base class of bidirectional_iterable,
//  random_access_iteratable must not be derived from totally_ordered1
//  but from less_than_comparable1 only. (Helmut Zeisel, 02-Dec-2001)
template <class T, class P, class D, class R, class B = ::boost::detail::empty_base>
struct random_access_iteratable
    : bidirectional_iteratable<T, P
    , less_than_comparable1<T
    , additive2<T, D
    , indexable<T, D, R, B
      > > > > {};

#ifndef BOOST_NO_OPERATORS_IN_NAMESPACE
} // namespace boost
#endif // BOOST_NO_OPERATORS_IN_NAMESPACE


// BOOST_IMPORT_TEMPLATE1 .. BOOST_IMPORT_TEMPLATE4 -
//
// When BOOST_NO_OPERATORS_IN_NAMESPACE is defined we need a way to import an
// operator template into the boost namespace. BOOST_IMPORT_TEMPLATE1 is used
// for one-argument forms of operator templates; BOOST_IMPORT_TEMPLATE2 for
// two-argument forms. Note that these macros expect to be invoked from within
// boost.

#ifndef BOOST_NO_OPERATORS_IN_NAMESPACE

  // The template is already in boost so we have nothing to do.
# define BOOST_IMPORT_TEMPLATE4(template_name)
# define BOOST_IMPORT_TEMPLATE3(template_name)
# define BOOST_IMPORT_TEMPLATE2(template_name)
# define BOOST_IMPORT_TEMPLATE1(template_name)

#else // BOOST_NO_OPERATORS_IN_NAMESPACE

#  ifndef BOOST_NO_USING_TEMPLATE

     // Bring the names in with a using-declaration
     // to avoid stressing the compiler.
#    define BOOST_IMPORT_TEMPLATE4(template_name) using ::template_name;
#    define BOOST_IMPORT_TEMPLATE3(template_name) using ::template_name;
#    define BOOST_IMPORT_TEMPLATE2(template_name) using ::template_name;
#    define BOOST_IMPORT_TEMPLATE1(template_name) using ::template_name;

#  else

     // Otherwise, because a Borland C++ 5.5 bug prevents a using declaration
     // from working, we are forced to use inheritance for that compiler.
#    define BOOST_IMPORT_TEMPLATE4(template_name)                                          \
     template <class T, class U, class V, class W, class B = ::boost::detail::empty_base>  \
     struct template_name : ::template_name<T, U, V, W, B> {};

#    define BOOST_IMPORT_TEMPLATE3(template_name)                                 \
     template <class T, class U, class V, class B = ::boost::detail::empty_base>  \
     struct template_name : ::template_name<T, U, V, B> {};

#    define BOOST_IMPORT_TEMPLATE2(template_name)                              \
     template <class T, class U, class B = ::boost::detail::empty_base>        \
     struct template_name : ::template_name<T, U, B> {};

#    define BOOST_IMPORT_TEMPLATE1(template_name)                              \
     template <class T, class B = ::boost::detail::empty_base>                 \
     struct template_name : ::template_name<T, B> {};

#  endif // BOOST_NO_USING_TEMPLATE

#endif // BOOST_NO_OPERATORS_IN_NAMESPACE

//
// Here's where we put it all together, defining the xxxx forms of the templates
// in namespace boost. We also define specializations of is_chained_base<> for
// the xxxx, xxxx1, and xxxx2 templates, importing them into boost:: as
// neccessary.
//
#ifndef BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION

// is_chained_base<> - a traits class used to distinguish whether an operator
// template argument is being used for base class chaining, or is specifying a
// 2nd argument type.

namespace boost {
// A type parameter is used instead of a plain bool because Borland's compiler
// didn't cope well with the more obvious non-type template parameter.
namespace detail {
  struct true_t {};
  struct false_t {};
} // namespace detail

// Unspecialized version assumes that most types are not being used for base
// class chaining. We specialize for the operator templates defined in this
// library.
template<class T> struct is_chained_base {
  typedef ::boost::detail::false_t value;
};

} // namespace boost

// Import a 4-type-argument operator template into boost (if neccessary) and
// provide a specialization of 'is_chained_base<>' for it.
# define BOOST_OPERATOR_TEMPLATE4(template_name4)                     \
  BOOST_IMPORT_TEMPLATE4(template_name4)                              \
  template<class T, class U, class V, class W, class B>               \
  struct is_chained_base< ::boost::template_name4<T, U, V, W, B> > {  \
    typedef ::boost::detail::true_t value;                            \
  };

// Import a 3-type-argument operator template into boost (if neccessary) and
// provide a specialization of 'is_chained_base<>' for it.
# define BOOST_OPERATOR_TEMPLATE3(template_name3)                     \
  BOOST_IMPORT_TEMPLATE3(template_name3)                              \
  template<class T, class U, class V, class B>                        \
  struct is_chained_base< ::boost::template_name3<T, U, V, B> > {     \
    typedef ::boost::detail::true_t value;                            \
  };

// Import a 2-type-argument operator template into boost (if neccessary) and
// provide a specialization of 'is_chained_base<>' for it.
# define BOOST_OPERATOR_TEMPLATE2(template_name2)                  \
  BOOST_IMPORT_TEMPLATE2(template_name2)                           \
  template<class T, class U, class B>                              \
  struct is_chained_base< ::boost::template_name2<T, U, B> > {     \
    typedef ::boost::detail::true_t value;                         \
  };

// Import a 1-type-argument operator template into boost (if neccessary) and
// provide a specialization of 'is_chained_base<>' for it.
# define BOOST_OPERATOR_TEMPLATE1(template_name1)                  \
  BOOST_IMPORT_TEMPLATE1(template_name1)                           \
  template<class T, class B>                                       \
  struct is_chained_base< ::boost::template_name1<T, B> > {        \
    typedef ::boost::detail::true_t value;                         \
  };

// BOOST_OPERATOR_TEMPLATE(template_name) defines template_name<> such that it
// can be used for specifying both 1-argument and 2-argument forms. Requires the
// existence of two previously defined class templates named '<template_name>1'
// and '<template_name>2' which must implement the corresponding 1- and 2-
// argument forms.
//
// The template type parameter O == is_chained_base<U>::value is used to
// distinguish whether the 2nd argument to <template_name> is being used for
// base class chaining from another boost operator template or is describing a
// 2nd operand type. O == true_t only when U is actually an another operator
// template from the library. Partial specialization is used to select an
// implementation in terms of either '<template_name>1' or '<template_name>2'.
//

# define BOOST_OPERATOR_TEMPLATE(template_name)                    \
template <class T                                                  \
         ,class U = T                                              \
         ,class B = ::boost::detail::empty_base                    \
         ,class O = typename is_chained_base<U>::value             \
         >                                                         \
struct template_name : template_name##2<T, U, B> {};               \
                                                                   \
template<class T, class U, class B>                                \
struct template_name<T, U, B, ::boost::detail::true_t>             \
  : template_name##1<T, U> {};                                     \
                                                                   \
template <class T, class B>                                        \
struct template_name<T, T, B, ::boost::detail::false_t>            \
  : template_name##1<T, B> {};                                     \
                                                                   \
template<class T, class U, class B, class O>                       \
struct is_chained_base< ::boost::template_name<T, U, B, O> > {     \
  typedef ::boost::detail::true_t value;                           \
};                                                                 \
                                                                   \
BOOST_OPERATOR_TEMPLATE2(template_name##2)                         \
BOOST_OPERATOR_TEMPLATE1(template_name##1)


#else // BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION

#  define BOOST_OPERATOR_TEMPLATE4(template_name4) \
        BOOST_IMPORT_TEMPLATE4(template_name4)
#  define BOOST_OPERATOR_TEMPLATE3(template_name3) \
        BOOST_IMPORT_TEMPLATE3(template_name3)
#  define BOOST_OPERATOR_TEMPLATE2(template_name2) \
        BOOST_IMPORT_TEMPLATE2(template_name2)
#  define BOOST_OPERATOR_TEMPLATE1(template_name1) \
        BOOST_IMPORT_TEMPLATE1(template_name1)

   // In this case we can only assume that template_name<> is equivalent to the
   // more commonly needed template_name1<> form.
#  define BOOST_OPERATOR_TEMPLATE(template_name)                   \
   template <class T, class B = ::boost::detail::empty_base>       \
   struct template_name : template_name##1<T, B> {};

#endif // BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION

namespace boost {
    
BOOST_OPERATOR_TEMPLATE(less_than_comparable)
BOOST_OPERATOR_TEMPLATE(equality_comparable)
BOOST_OPERATOR_TEMPLATE(multipliable)
BOOST_OPERATOR_TEMPLATE(addable)
BOOST_OPERATOR_TEMPLATE(subtractable)
BOOST_OPERATOR_TEMPLATE2(subtractable2_left)
BOOST_OPERATOR_TEMPLATE(dividable)
BOOST_OPERATOR_TEMPLATE2(dividable2_left)
BOOST_OPERATOR_TEMPLATE(modable)
BOOST_OPERATOR_TEMPLATE2(modable2_left)
BOOST_OPERATOR_TEMPLATE(xorable)
BOOST_OPERATOR_TEMPLATE(andable)
BOOST_OPERATOR_TEMPLATE(orable)

BOOST_OPERATOR_TEMPLATE1(incrementable)
BOOST_OPERATOR_TEMPLATE1(decrementable)

BOOST_OPERATOR_TEMPLATE2(dereferenceable)
BOOST_OPERATOR_TEMPLATE3(indexable)

BOOST_OPERATOR_TEMPLATE(left_shiftable)
BOOST_OPERATOR_TEMPLATE(right_shiftable)
BOOST_OPERATOR_TEMPLATE(equivalent)
BOOST_OPERATOR_TEMPLATE(partially_ordered)

BOOST_OPERATOR_TEMPLATE(totally_ordered)
BOOST_OPERATOR_TEMPLATE(additive)
BOOST_OPERATOR_TEMPLATE(multiplicative)
BOOST_OPERATOR_TEMPLATE(integer_multiplicative)
BOOST_OPERATOR_TEMPLATE(arithmetic)
BOOST_OPERATOR_TEMPLATE(integer_arithmetic)
BOOST_OPERATOR_TEMPLATE(bitwise)
BOOST_OPERATOR_TEMPLATE1(unit_steppable)
BOOST_OPERATOR_TEMPLATE(shiftable)
BOOST_OPERATOR_TEMPLATE(ring_operators)
BOOST_OPERATOR_TEMPLATE(ordered_ring_operators)
BOOST_OPERATOR_TEMPLATE(field_operators)
BOOST_OPERATOR_TEMPLATE(ordered_field_operators)
BOOST_OPERATOR_TEMPLATE(euclidian_ring_operators)
BOOST_OPERATOR_TEMPLATE(ordered_euclidian_ring_operators)
BOOST_OPERATOR_TEMPLATE2(input_iteratable)
BOOST_OPERATOR_TEMPLATE1(output_iteratable)
BOOST_OPERATOR_TEMPLATE2(forward_iteratable)
BOOST_OPERATOR_TEMPLATE2(bidirectional_iteratable)
BOOST_OPERATOR_TEMPLATE4(random_access_iteratable)

#undef BOOST_OPERATOR_TEMPLATE
#undef BOOST_OPERATOR_TEMPLATE4
#undef BOOST_OPERATOR_TEMPLATE3
#undef BOOST_OPERATOR_TEMPLATE2
#undef BOOST_OPERATOR_TEMPLATE1
#undef BOOST_IMPORT_TEMPLATE1
#undef BOOST_IMPORT_TEMPLATE2
#undef BOOST_IMPORT_TEMPLATE3
#undef BOOST_IMPORT_TEMPLATE4

// The following 'operators' classes can only be used portably if the derived class
// declares ALL of the required member operators.
template <class T, class U>
struct operators2
    : totally_ordered2<T,U
    , integer_arithmetic2<T,U
    , bitwise2<T,U
      > > > {};

#ifndef BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION
template <class T, class U = T>
struct operators : operators2<T, U> {};

template <class T> struct operators<T, T>
#else
template <class T> struct operators
#endif
    : totally_ordered<T
    , integer_arithmetic<T
    , bitwise<T
    , unit_steppable<T
      > > > > {};

//  Iterator helper classes (contributed by Jeremy Siek) -------------------//
//  (Input and output iterator helpers contributed by Daryle Walker) -------//
//  (Changed to use combined operator classes by Daryle Walker) ------------//
template <class T,
          class V,
          class D = std::ptrdiff_t,
          class P = V const *,
          class R = V const &>
struct input_iterator_helper
  : input_iteratable<T, P
  , boost::iterator<std::input_iterator_tag, V, D, P, R
    > > {};

template<class T>
struct output_iterator_helper
  : output_iteratable<T
  , boost::iterator<std::output_iterator_tag, void, void, void, void
  > >
{
  T& operator*()  { return static_cast<T&>(*this); }
  T& operator++() { return static_cast<T&>(*this); }
};

template <class T,
          class V,
          class D = std::ptrdiff_t,
          class P = V*,
          class R = V&>
struct forward_iterator_helper
  : forward_iteratable<T, P
  , boost::iterator<std::forward_iterator_tag, V, D, P, R
    > > {};

template <class T,
          class V,
          class D = std::ptrdiff_t,
          class P = V*,
          class R = V&>
struct bidirectional_iterator_helper
  : bidirectional_iteratable<T, P
  , boost::iterator<std::bidirectional_iterator_tag, V, D, P, R
    > > {};

template <class T,
          class V, 
          class D = std::ptrdiff_t,
          class P = V*,
          class R = V&>
struct random_access_iterator_helper
  : random_access_iteratable<T, P, D, R
  , boost::iterator<std::random_access_iterator_tag, V, D, P, R
    > >
{
  friend D requires_difference_operator(const T& x, const T& y) {
    return x - y;
  }
}; // random_access_iterator_helper

} // namespace boost

#if defined(__sgi) && !defined(__GNUC__)
#pragma reset woff 1234
#endif

#endif // BOOST_OPERATORS_HPP

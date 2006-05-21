
//   The following code was taken from tuple_basic.hpp/tuple_basic_no_partial_spec.hpp
//   with the kind permission of Jaakko Järvi.
//   The only modification done was to change namespaces.
//   Comments below are that of the orginal version. - RR

// Copyright (C) 1999, 2000 Jaakko Järvi (jaakko.jarvi@cs.utu.fi)
// Copyright (C) 2001 Doug Gregor (gregod@rpi.edu)
// Copyright (C) 2001 Gary Powell (gary.powell@sierra.com)
//
// Permission to copy, use, sell and distribute this software is granted
// provided this copyright notice appears in all copies. 
// Permission to modify the code and to distribute modified code is granted
// provided this copyright notice appears in all copies, and a notice 
// that the code was modified is included with the copyright notice.
//
// This software is provided "as is" without express or implied warranty, 
// and with no claim as to its suitability for any purpose.

// For more information, see http://www.boost.org or http://lambda.cs.utu.fi 


#ifndef BOOST_TUPPLE_DETAIL_TUPPLE_DETAIL_HPP
#define BOOST_TUPPLE_DETAIL_TUPPLE_DETAIL_HPP

#include <boost/type_traits/transform_traits.hpp>
#include <boost/type_traits/cv_traits.hpp>

#include <boost/ref.hpp>
using boost::ref;
using boost::cref;

namespace boost {
namespace tupple {

struct null_type {};

namespace detail {

      // Takes a pointer and routes all assignments to whatever it points to
      template<typename T>
      struct assign_to_pointee
      {
      public:
        explicit assign_to_pointee(T* p) : ptr(p) {}

        template<typename Other>
        assign_to_pointee& operator=(const Other& other)
        {
          *ptr = other;
          return *this;
        }

      private:
        T* ptr;
      };

      // Swallows any assignment
      struct swallow_assign
      {
        template<typename T>
        swallow_assign& operator=(const T&)
        {
          return *this;
        }
      };

    template <typename T> struct add_const_reference : add_reference<typename add_const<T>::type> {};
} // end of namespace detail


    // "ignore" allows tuple positions to be ignored when using "tie". 
    namespace {
#if (defined(BOOST_MSVC) && BOOST_MSVC <= 1300) || (defined(__DECCXX_VER) && __DECCXX_VER <= 60590031)
      static
#endif 
      detail::swallow_assign ignore;
    }


// access traits lifted from detail namespace to be part of the interface,
// (Joel de Guzman's suggestion). Rationale: get functions are part of the
// interface, so should the way to express their return types be.


template <class T> struct access_traits {
  typedef const T& const_type;
  typedef T& non_const_type;

  typedef const typename ::boost::remove_cv<T>::type& arg_type;
// used as the tuple constructors parameter types
// Rationale: non-reference tuple element types can be cv-qualified.
// It should be possible to initialize such types with temporaries,
// and when binding temporaries to references, the reference must
// be non-volatile and const. 8.5.3. (5)
};

#ifndef BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION
template <class T> struct access_traits<T&> {

  typedef T& const_type;
  typedef T& non_const_type;

  typedef T& arg_type;
};
#endif // BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION

// -- generate error template, referencing to non-existing members of this
// template is used to produce compilation errors intentionally
template<class T>
class generate_error;

// ---------------------------------------------------------------------------
// The call_traits for make_tuple
// Honours the reference_wrapper class.

// Must be instantiated with plain or const plain types (not with references)

// from template<class T> foo(const T& t) : make_tuple_traits<const T>::type
// from template<class T> foo(T& t) : make_tuple_traits<T>::type

// Conversions:
// T -> T,
// references -> compile_time_error
// reference_wrapper<T> -> T&
// const reference_wrapper<T> -> T&
// array -> const ref array


template<class T>
struct make_tuple_traits {
  typedef T type;

  // commented away, see below  (JJ)
  //  typedef typename IF<
  //  ::boost::is_function<T>::value,
  //  T&,
  //  T>::RET type;

};


// The is_function test was there originally for plain function types,
// which can't be stored as such (we must either store them as references or
// pointers). Such a type could be formed if make_tuple was called with a
// reference to a function.
// But this would mean that a const qualified function type was formed in
// the make_tuple function and hence make_tuple can't take a function
// reference as a parameter, and thus T can't be a function type.
// So is_function test was removed.
// (14.8.3. says that type deduction fails if a cv-qualified function type
// is created. (It only applies for the case of explicitly specifying template
// args, though?)) (JJ)
#ifndef BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION
template<class T> struct make_tuple_traits<T&> {
  typedef typename
     generate_error<T&>::do_not_use_with_reference_type error;
};

// Arrays can't be stored as plain types: convert them to references.
// All arrays are converted to const. This is because make_tuple takes its
// parameters as const T& and thus the knowledge of the potential
// non-constness of actual argument is lost.
template<class T, int n> struct make_tuple_traits <T[n]> {
  typedef const T (&type)[n];
};

template<class T, int n> struct make_tuple_traits<const T[n]> {
  typedef const T (&type)[n];
};

template<class T, int n> struct make_tuple_traits<volatile T[n]> {
  typedef const volatile T (&type)[n];
};

template<class T, int n> struct make_tuple_traits<const volatile T[n]> {
  typedef const volatile T (&type)[n];
};

template<class T> struct make_tuple_traits< ::boost::reference_wrapper<T> > {
  typedef T& type;
};

template<class T> struct make_tuple_traits< const ::boost::reference_wrapper<T> > {
  typedef T& type;
};
#endif // BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION

} // namespace tupple
} // namespace boost


#endif

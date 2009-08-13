//
//  basic_generic_pointer_test.cpp
//
//  Copyright (c) 2009 Frank Mori Hess
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//

#include <boost/config.hpp>
#include <boost/detail/lightweight_test.hpp>
#include <boost/generic_ptr/asserting.hpp>
#include <boost/generic_ptr/cloning.hpp>
#include <boost/generic_ptr/intrusive.hpp>
#include <boost/generic_ptr/monitor.hpp>
#include <boost/generic_ptr/nonnull.hpp>
#include <boost/generic_ptr/pointer_cast.hpp>
#include <boost/generic_ptr/pointer_traits.hpp>
#include <boost/generic_ptr/shared.hpp>
#include <boost/generic_ptr/throwing.hpp>

namespace bgp = boost::generic_ptr;

class Y
{
public:
  virtual ~Y() {}
};

class X: public Y
{
public:
  static const int return_value = 1;
  int f() { return return_value; }
};

void intrusive_ptr_add_ref(const Y *) {};
void intrusive_ptr_release(const Y *) {};

template<typename GenericPointer>
void member_access_test(GenericPointer &p)
{
  BOOST_TEST(p->f() == X::return_value);
}

template<typename GenericPointer>
void dereference_test(GenericPointer &p)
{
  BOOST_TEST((*p).f() == X::return_value);
}

template<typename GenericPointer>
void rebind_test(GenericPointer &p)
{
  typedef typename bgp::rebind
  <
    GenericPointer,
    const typename bgp::pointer_traits<GenericPointer>::value_type
  >::other other_type;
  BOOST_TEST(typeid(const typename bgp::pointer_traits<GenericPointer>::value_type) ==
    typeid(typename bgp::pointer_traits<other_type>::value_type));
}

template<typename GenericPointer>
void cast_test(GenericPointer &p)
{
  typedef typename bgp::rebind<GenericPointer, Y>::other pointer_to_y_type;

  pointer_to_y_type yp = bgp::static_pointer_cast<Y>(p);
  GenericPointer xp = bgp::static_pointer_cast
    <typename bgp::pointer_traits<GenericPointer>::value_type>(yp);

  yp = bgp::dynamic_pointer_cast<Y>(p);
  BOOST_TEST(bgp::get_plain_old_pointer(yp) != 0);
  xp = bgp::dynamic_pointer_cast
    <typename bgp::pointer_traits<GenericPointer>::value_type>(yp);
  BOOST_TEST(bgp::get_plain_old_pointer(xp) != 0);

  typedef typename bgp::rebind
  <
    GenericPointer,
    const typename bgp::pointer_traits<GenericPointer>::value_type
  >::other pointer_to_const_x_type;
  pointer_to_const_x_type xp_const = bgp::const_pointer_cast
    <const typename bgp::pointer_traits<GenericPointer>::value_type>(p);
  xp = bgp::const_pointer_cast
    <
      typename bgp::pointer_traits<GenericPointer>::value_type
    >(xp_const);
}

template<typename GenericPointer>
void conversion_to_base_test(GenericPointer &p, bool is_cloning_pointer = false)
{
  typedef typename bgp::rebind<GenericPointer, Y>::other pointer_to_base_type;
  pointer_to_base_type base_p(p);
  BOOST_TEST(p == base_p ||
    is_cloning_pointer);
  base_p = p;
  BOOST_TEST(p == base_p ||
    is_cloning_pointer);
}

template<typename GenericPointer>
void conversion_to_void_test(GenericPointer &p, bool is_cloning_pointer = false)
{
  typedef typename bgp::rebind<GenericPointer, void>::other pointer_to_void_type;
  pointer_to_void_type void_p(p);
  BOOST_TEST(p == void_p ||
    is_cloning_pointer);
  void_p = p;
  BOOST_TEST(p == void_p ||
    is_cloning_pointer);

  typedef typename bgp::rebind
  <
    GenericPointer,
    const typename bgp::pointer_traits<GenericPointer>::value_type
  >::other pointer_to_const_type;
  pointer_to_const_type cp(p);

  typedef typename bgp::rebind
  <
    GenericPointer,
    const void
  >::other pointer_to_const_void_type;
  pointer_to_const_void_type const_void_p(cp);
  BOOST_TEST(cp == const_void_p ||
    is_cloning_pointer);
  void_p = p;
  BOOST_TEST(cp == const_void_p ||
    is_cloning_pointer);
}

#ifndef BOOST_NO_SFINAE
template<typename GenericPointer>
class overloaded
{
public:
	typedef typename bgp::rebind<GenericPointer, Y>::other base_ptr;
	typedef typename bgp::rebind<GenericPointer, int>::other other_ptr;
	void f(const base_ptr &)
	{
	}
	void f(const other_ptr &)
	{
		BOOST_TEST(false);
	}
};
#endif // BOOST_NO_SFINAE

template<typename GenericPointer>
void overload_resolution_test(GenericPointer &p)
{
#ifndef BOOST_NO_SFINAE
	overloaded<GenericPointer> o;
	o.f(p);
#endif // BOOST_NO_SFINAE
}

int main()
{
  {
    X x;
    X *p = &x;
    rebind_test(p);
    cast_test(p);
    conversion_to_base_test(p);
    conversion_to_void_test(p);
    overload_resolution_test(p);
  }
  {
    X x;
    bgp::asserting<X*> p(&x);
    member_access_test(p);
    dereference_test(p);
    rebind_test(p);
    cast_test(p);
    conversion_to_base_test(p);
    conversion_to_void_test(p);
    overload_resolution_test(p);
  }
  {
    X x;
    bgp::throwing<X*> p(&x);
    member_access_test(p);
    dereference_test(p);
    rebind_test(p);
    cast_test(p);
    conversion_to_base_test(p);
    conversion_to_void_test(p);
    overload_resolution_test(p);
  }
  {
    X x;
    bgp::nonnull<X*> p(&x);
    member_access_test(p);
    dereference_test(p);
    rebind_test(p);
    cast_test(p);
    conversion_to_base_test(p);
    conversion_to_void_test(p);
    overload_resolution_test(p);
  }
  {
    X x;
    bgp::monitor<X*> p(&x);
    member_access_test(p);
    // dereference_test(p); // monitors don't support dereference
    rebind_test(p);
    cast_test(p);
    conversion_to_base_test(p);
    conversion_to_void_test(p);
    overload_resolution_test(p);
  }
  {
    bgp::shared<X*> p(new X());
    member_access_test(p);
    dereference_test(p);
    rebind_test(p);
    cast_test(p);
    conversion_to_base_test(p);
    conversion_to_void_test(p);
    overload_resolution_test(p);
  }
  {
    X x;
    bgp::intrusive<X*> p(&x);
    member_access_test(p);
    dereference_test(p);
    rebind_test(p);
    cast_test(p);
    conversion_to_base_test(p);
    // conversion_to_void_test(p); // intrusive doesn't support void pointers
    overload_resolution_test(p);
  }
  {
    bgp::cloning<X*> p(new X());
    member_access_test(p);
    dereference_test(p);
    rebind_test(p);
    cast_test(p);
    conversion_to_base_test(p, true);
    conversion_to_void_test(p, true);
    overload_resolution_test(p);
  }
  //TODO: add tests for external pointer types
  return boost::report_errors();
}

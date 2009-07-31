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
  typedef typename boost::generic_ptr::rebind
  <
    GenericPointer,
    const typename boost::generic_ptr::pointer_traits<GenericPointer>::value_type
  >::other other_type;
  BOOST_TEST(typeid(const typename boost::generic_ptr::pointer_traits<GenericPointer>::value_type) ==
    typeid(typename boost::generic_ptr::pointer_traits<other_type>::value_type));
}

template<typename GenericPointer>
void cast_test(GenericPointer &p)
{
  typedef typename boost::generic_ptr::rebind<GenericPointer, Y>::other pointer_to_y_type;

  pointer_to_y_type yp = boost::generic_ptr::static_pointer_cast<Y>(p);
  GenericPointer xp = boost::generic_ptr::static_pointer_cast
    <typename boost::generic_ptr::pointer_traits<GenericPointer>::value_type>(yp);

  yp = boost::generic_ptr::dynamic_pointer_cast<Y>(p);
  xp = boost::generic_ptr::dynamic_pointer_cast
    <typename boost::generic_ptr::pointer_traits<GenericPointer>::value_type>(yp);
  BOOST_TEST(boost::generic_ptr::get_plain_old_pointer(xp) != 0);

  typedef typename boost::generic_ptr::rebind
  <
    GenericPointer,
    const typename boost::generic_ptr::pointer_traits<GenericPointer>::value_type
  >::other pointer_to_const_x_type;
  pointer_to_const_x_type xp_const = boost::generic_ptr::const_pointer_cast
    <const typename boost::generic_ptr::pointer_traits<GenericPointer>::value_type>(p);
  xp = boost::generic_ptr::const_pointer_cast
    <
      typename boost::generic_ptr::pointer_traits<GenericPointer>::value_type
    >(xp_const);
}

template<typename GenericPointer>
void conversion_test(GenericPointer &p, bool is_cloning_pointer = false)
{
  // conversion to base pointer
  typedef typename boost::generic_ptr::rebind<GenericPointer, Y>::other pointer_to_base_type;
  pointer_to_base_type base_p(p);
  BOOST_TEST(p == base_p ||
    is_cloning_pointer);
  base_p = p;
  BOOST_TEST(p == base_p ||
    is_cloning_pointer);
}

int main()
{
  {
    X x;
    X *p = &x;
    rebind_test(p);
    cast_test(p);
    conversion_test(p);
  }
  {
    X x;
    boost::generic_ptr::asserting<X*> p(&x);
    member_access_test(p);
    dereference_test(p);
    rebind_test(p);
    cast_test(p);
    conversion_test(p);
  }
  {
    X x;
    boost::generic_ptr::throwing<X*> p(&x);
    member_access_test(p);
    dereference_test(p);
    rebind_test(p);
    cast_test(p);
    conversion_test(p);
  }
  {
    X x;
    boost::generic_ptr::nonnull<X*> p(&x);
    member_access_test(p);
    dereference_test(p);
    rebind_test(p);
    cast_test(p);
    conversion_test(p);
  }
  {
    X x;
    boost::generic_ptr::monitor<X*> p(&x);
    member_access_test(p);
    // dereference_test(p); // monitors don't support dereference
    rebind_test(p);
    cast_test(p);
    conversion_test(p);
  }
  {
    boost::generic_ptr::shared<X*> p(new X());
    member_access_test(p);
    dereference_test(p);
    rebind_test(p);
    cast_test(p);
    conversion_test(p);
  }
  {
    X x;
    boost::generic_ptr::intrusive<X*> p(&x);
    member_access_test(p);
    dereference_test(p);
    rebind_test(p);
    cast_test(p);
    conversion_test(p);
  }
  {
    boost::generic_ptr::cloning<X*> p(new X());
    member_access_test(p);
    dereference_test(p);
    rebind_test(p);
    cast_test(p);
    conversion_test(p, true);
  }
  return 0;
}

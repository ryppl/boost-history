//
//  cloning_test.cpp
//
//  Copyright (c) 2009 Frank Mori Hess
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//

#include <boost/config.hpp>
#include <boost/detail/lightweight_test.hpp>
#include <boost/generic_ptr/cloning.hpp>
#include <boost/generic_ptr/null_deleter.hpp>

class X_base
{
public:
  virtual ~X_base() {};
  int f() {return 0;}
};

class X: public X_base
{
public:
  static unsigned instances;
  X()
  {
    ++instances;
  }
  X(const X &other)
  {
    ++instances;
  }
  ~X()
  {
    --instances;
  }
  unsigned num_instances() const {return instances;}
};
unsigned X::instances = 0;

void clone_test()
{
  BOOST_TEST(X::instances == 0);
  boost::generic_ptr::cloning<X*> cp(new X());
  BOOST_TEST(X::instances == 1);
  boost::generic_ptr::cloning<X*> cp2(cp);
  BOOST_TEST(X::instances == 2);
  BOOST_TEST(cp != cp2);
  boost::generic_ptr::cloning<X*> cp3;
  BOOST_TEST(X::instances == 2);
  cp3 = cp2;
  BOOST_TEST(X::instances == 3);
  BOOST_TEST(cp3->num_instances() == X::instances);
  BOOST_TEST(cp2 != cp3);
}

void move_test()
{
#ifndef BOOST_NO_RVALUE_REFERENCES
  BOOST_TEST(X::instances == 0);
  X * plain_x = new X();
  boost::generic_ptr::cloning<X*> cp(plain_x);
  BOOST_TEST(X::instances == 1);
  BOOST_TEST(cp == plain_x);
  boost::generic_ptr::cloning<X*> cp2(std::move(cp));
  BOOST_TEST(X::instances == 1);
  BOOST_TEST(cp.get() == 0);
  BOOST_TEST(cp2 == plain_x);
  boost::generic_ptr::cloning<X*> cp3;
  cp3 = std::move(cp2);
  BOOST_TEST(X::instances == 1);
  BOOST_TEST(cp3 == plain_x);
#endif // BOOST_NO_RVALUE_REFERENCES
}

void no_slice_test()
{
  BOOST_TEST(X::instances == 0);
  boost::generic_ptr::cloning<X*> cp(new X());
  BOOST_TEST(X::instances == 1);
  boost::generic_ptr::cloning<X_base*> cp2(cp);
  BOOST_TEST(X::instances == 2);
  BOOST_TEST(cp != cp2);
  boost::generic_ptr::cloning<X_base*> cp3;
  BOOST_TEST(X::instances == 2);
  cp3 = cp;
  BOOST_TEST(X::instances == 3);
  BOOST_TEST(cp3->f() == 0);
  BOOST_TEST(cp != cp3);
}

void cast_test()
{
  BOOST_TEST(X::instances == 0);
  boost::generic_ptr::cloning<X_base*> cp(new X());
  BOOST_TEST(X::instances == 1);
  boost::generic_ptr::cloning<X*> cp2 = boost::generic_ptr::static_pointer_cast<X>(cp);
  BOOST_TEST(X::instances == 2);
  BOOST_TEST(cp != cp2);
  boost::generic_ptr::cloning<X_base*> cp3(boost::generic_ptr::static_pointer_cast<X_base>(cp2));
  BOOST_TEST(X::instances == 3);
  BOOST_TEST(cp2 != cp3);
}

void deleter_test()
{
  BOOST_TEST(X::instances == 0);
  X *x0 = new X();
  X *x1 = 0;
  {
    boost::generic_ptr::cloning<X*> cp0(x0, boost::generic_ptr::null_deleter());
    BOOST_TEST(X::instances == 1);
    {
      boost::generic_ptr::cloning<X*> cp1 = cp0;
      BOOST_TEST(X::instances == 2);
      x1 = get_plain_old_pointer(cp1);
    }
    BOOST_TEST(X::instances == 2);
  }
  BOOST_TEST(X::instances == 2);
  using boost::delete_clone;
  delete_clone(x0);
  delete_clone(x1);
  BOOST_TEST(X::instances == 0);
}

class null_cloner
{
public:
  template<typename GenericPointer>
  GenericPointer operator()(const GenericPointer &p)
  {
    return p;
  }
};

void custom_cloner_test()
{
  X *x0 = new X();
  BOOST_TEST(X::instances == 1);
  {
    boost::generic_ptr::cloning<X*> cp0(x0, boost::generic_ptr::null_deleter(), null_cloner());
    BOOST_TEST(X::instances == 1);
    boost::generic_ptr::cloning<X*> cp1 = cp0;
    BOOST_TEST(X::instances == 1);
  }
  BOOST_TEST(X::instances == 1);
  using boost::delete_clone;
  delete_clone(x0);
  BOOST_TEST(X::instances == 0);
}

int main()
{
  clone_test();
  move_test();
  no_slice_test();
  cast_test();
  deleter_test();
  custom_cloner_test();
  return boost::report_errors();
}

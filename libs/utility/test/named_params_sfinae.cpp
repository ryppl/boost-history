// Copyright David Abrahams, Daniel Wallin 2003. Use, modification and 
// distribution is subject to the Boost Software License, Version 1.0. 
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt

#include <boost/named_params.hpp>
#include <cassert>
#include <string>
#include <boost/type_traits/is_convertible.hpp>
#include <iostream>

namespace test
{
  using boost::keyword;
  using boost::keywords;
  using boost::named_param;

  struct name_t; keyword<name_t> name;
  struct value_t; keyword<value_t> value;
  
  struct f_keywords
      : keywords<
            named_param<
                name_t
              , boost::mpl::true_
              , boost::is_convertible<boost::mpl::_, std::string>
            >
          , named_param<
                value_t
              , boost::mpl::true_
              , boost::is_convertible<boost::mpl::_, float>
            >
      >
  {};

  template<class P>
  void f_impl(P const& p)
  {
      std::string s = p[name | "bar"];
      float v = p[value | 3.f];

      assert(s == "foo");
      assert(v == 3.f);
  }
 
  void f()
  {
      f_impl(f_keywords()());
  }
  
  template<class A0>
  void f(A0 const& a0, typename f_keywords::restrict<A0>::type = f_keywords())
  {
      f_impl(f_keywords()(a0));
  }

  template<class A0, class A1>
  void f(A0 const& a0, A1 const& a1)
  {
      f_impl(f_keywords()(a0, a1));
  }

} // namespace test

int main()
{
    using test::name;
    using test::value;    
    using test::f;

    f("foo");
    f("foo", 3.f);
    f(value = 3.f, name = "foo");

    return 0;
}


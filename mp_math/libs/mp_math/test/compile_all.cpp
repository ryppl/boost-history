// Copyright Kevin Sopp 2008.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

// Compile all functions in the presence of the cmath header

#include <cmath>
#include <boost/mp_math/mp_int.hpp>


// first explicitly instantiate the class template
template class boost::mp_math::mp_int<>;



// a different allocator type used in conjunction with the basic_string class
// template
template<typename T>
struct my_alloc : std::allocator<T>
{
  template<typename U> struct rebind { typedef my_alloc<U> other; };
};

typedef std::basic_string<
  char, std::char_traits<char>, my_alloc<char> > my_string;



template<class MpInt>
MpInt compile_constructors()
{
  const std::string  a("0x123456789abcdefABCDEF");
  const std::wstring b(L"012345676543210");
  const my_string    c("0xabcdefabcdef0001");

  // constructors from string
  MpInt u("0x123456789abcdef");
  MpInt v(L"0x123456789abcdef");
  MpInt w(a);
  MpInt x(a.begin(), a.end());
  MpInt y(b);
  MpInt z(c);

  const signed char sc = -100;
  const unsigned char uc = 200;
  const int i = -12000;
  const unsigned int ui = 22000;

  // constructors from integral type
  MpInt m(sc);
  MpInt n(uc);
  MpInt o(i);
  MpInt p(ui);

  // copy constructor
  MpInt copy(z);

  return u + v + w + x + y + z + copy + m + n + o + p;
}

template<class MpInt>
MpInt compile_assignment_ops()
{
  MpInt a = "0xfffffffffffffffffff";
  MpInt b = "0xabcd";
  MpInt c = 1000;
  MpInt d = "07777777777777777777000001";

  a = b;
  b = d;

  const my_string s("0xa123b566c78e9aafdf");
  c = s;

  d.assign("-12345678901234", std::ios_base::dec);

  return a + b + c + d;
}


int main(int, char**)
{
  
  typedef boost::mp_math::mp_int<> mp_int_type;
  
  mp_int_type a("0");

  a += compile_constructors<mp_int_type>();
  a += compile_assignment_ops<mp_int_type>();
  a = boost::mp_math::pow(a, 3);
  a = boost::mp_math::abs(a);

  return 0;
}


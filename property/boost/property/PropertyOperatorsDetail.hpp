#if !defined(PROPERTY_OPERATORS_DETAIL_HPP)
#define PROPERTY_OPERATORS_DETAIL_HPP

#include <boost/typeof/typeof.hpp>

namespace properties
{

  namespace detail
  {
  
  template <class T>
  struct unary_ro_plus
    {
    typedef BOOST_TYPEOF_TPL(+T()) type;
    };
  
  template <class T>
  struct unary_ro_minus
    {
    typedef BOOST_TYPEOF_TPL(-T()) type;
    };
  
  template <class T>
  struct unary_ro_negate
    {
    typedef BOOST_TYPEOF_TPL(~T()) type;
    };
  
  template <class T>
  struct unary_ro_not
    {
    typedef BOOST_TYPEOF_TPL(!T()) type;
    };
  
  template <class T,class U>
  struct binary_ro_multiply
    {
    typedef BOOST_TYPEOF_TPL(T() * U()) type;
    };
  
  template <class T,class U>
  struct binary_ro_divide
    {
    typedef BOOST_TYPEOF_TPL(T() / U()) type;
    };
  
  template <class T,class U>
  struct binary_ro_modulo
    {
    typedef BOOST_TYPEOF_TPL(T() % U()) type;
    };
  
  template <class T,class U>
  struct binary_ro_add
    {
    typedef BOOST_TYPEOF_TPL(T() + U()) type;
    };
  
  template <class T,class U>
  struct binary_ro_subtract
    {
    typedef BOOST_TYPEOF_TPL(T() - U()) type;
    };
  
  template <class T,class U>
  struct binary_ro_left_shift
    {
    typedef BOOST_TYPEOF_TPL(T() << U()) type;
    };
  
  template <class T,class U>
  struct binary_ro_right_shift
    {
    typedef BOOST_TYPEOF_TPL(T() >> U()) type;
    };
  
  template <class T,class U>
  struct binary_ro_and
    {
    typedef BOOST_TYPEOF_TPL(T() & U()) type;
    };
  
  template <class T,class U>
  struct binary_ro_or
    {
    typedef BOOST_TYPEOF_TPL(T() | U()) type;
    };
  
  template <class T,class U>
  struct binary_ro_xor
    {
    typedef BOOST_TYPEOF_TPL(T() ^ U()) type;
    };
  
  template <class T,class U>
  struct binary_ro_greater
    {
    typedef BOOST_TYPEOF_TPL(T() > U()) type;
    };
  
  template <class T,class U>
  struct binary_ro_less
    {
    typedef BOOST_TYPEOF_TPL(T() < U()) type;
    };
  
  template <class T,class U>
  struct binary_ro_greater_or_equal
    {
    typedef BOOST_TYPEOF_TPL(T() >= U()) type;
    };
  
  template <class T,class U>
  struct binary_ro_less_or_equal
    {
    typedef BOOST_TYPEOF_TPL(T() <= U()) type;
    };
  
  template <class T,class U>
  struct binary_ro_equal
    {
    typedef BOOST_TYPEOF_TPL(T() == U()) type;
    };
  
  template <class T,class U>
  struct binary_ro_not_equal
    {
    typedef BOOST_TYPEOF_TPL(T() != U()) type;
    };
  
  template <class T,class U>
  struct binary_ro_logical_and
    {
    typedef BOOST_TYPEOF_TPL(T() && U()) type;
    };
  
  template <class T,class U>
  struct binary_ro_logical_or
    {
    typedef BOOST_TYPEOF_TPL(T() || U()) type;
    };
  
  template <class T>
  struct binary_ro_multiply_same
    {
    typedef typename binary_ro_multiply<T,T>::type type;
    };
  
  template <class T>
  struct binary_ro_divide_same
    {
    typedef typename binary_ro_divide<T,T>::type type;
    };
  
  template <class T>
  struct binary_ro_modulo_same
    {
    typedef typename binary_ro_modulo<T,T>::type type;
    };
  
  template <class T>
  struct binary_ro_add_same
    {
    typedef typename binary_ro_add<T,T>::type type;
    };
  
  template <class T>
  struct binary_ro_subtract_same
    {
    typedef typename binary_ro_subtract<T,T>::type type;
    };
  
  template <class T>
  struct binary_ro_left_shift_same
    {
    typedef typename binary_ro_left_shift<T,T>::type type;
    };
  
  template <class T>
  struct binary_ro_right_shift_same
    {
    typedef typename binary_ro_right_shift<T,T>::type type;
    };
  
  template <class T>
  struct binary_ro_and_same
    {
    typedef typename binary_ro_and<T,T>::type type;
    };
  
  template <class T>
  struct binary_ro_or_same
    {
    typedef typename binary_ro_or<T,T>::type type;
    };
  
  template <class T>
  struct binary_ro_xor_same
    {
    typedef typename binary_ro_xor<T,T>::type type;
    };
  
  template <class T>
  struct binary_ro_greater_same
    {
    typedef typename binary_ro_greater<T,T>::type type;
    };
  
  template <class T>
  struct binary_ro_less_same
    {
    typedef typename binary_ro_less<T,T>::type type;
    };
  
  template <class T>
  struct binary_ro_greater_or_equal_same
    {
    typedef typename binary_ro_greater_or_equal<T,T>::type type;
    };
  
  template <class T>
  struct binary_ro_less_or_equal_same
    {
    typedef typename binary_ro_less_or_equal<T,T>::type type;
    };
  
  template <class T>
  struct binary_ro_equal_same
    {
    typedef typename binary_ro_equal<T,T>::type type;
    };
  
  template <class T>
  struct binary_ro_not_equal_same
    {
    typedef typename binary_ro_not_equal<T,T>::type type;
    };
  
  template <class T>
  struct binary_ro_logical_and_same
    {
    typedef typename binary_ro_logical_and<T,T>::type type;
    };
  
  template <class T>
  struct binary_ro_logical_or_same
    {
    typedef typename binary_ro_logical_or<T,T>::type type;
    };
  
  }
}

#endif

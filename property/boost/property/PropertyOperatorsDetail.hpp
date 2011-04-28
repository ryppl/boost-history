#if !defined(PROPERTY_OPERATORS_DETAIL_HPP)
#define PROPERTY_OPERATORS_DETAIL_HPP

#include <boost/typeof/typeof.hpp>

namespace properties
{

  namespace detail
  {
  
  template <class T>
  struct UnaryROPlus
    {
    typedef BOOST_TYPEOF_TPL(+T()) type;
    };
  
  template <class T>
  struct UnaryROMinus
    {
    typedef BOOST_TYPEOF_TPL(-T()) type;
    };
  
  template <class T>
  struct UnaryRONegate
    {
    typedef BOOST_TYPEOF_TPL(~T()) type;
    };
  
  template <class T>
  struct UnaryRONot
    {
    typedef BOOST_TYPEOF_TPL(!T()) type;
    };
  
  template <class T,class U>
  struct BinaryROMultiply
    {
    typedef BOOST_TYPEOF_TPL(T() * U()) type;
    };
  
  template <class T,class U>
  struct BinaryRODivide
    {
    typedef BOOST_TYPEOF_TPL(T() / U()) type;
    };
  
  template <class T,class U>
  struct BinaryROModulo
    {
    typedef BOOST_TYPEOF_TPL(T() % U()) type;
    };
  
  template <class T,class U>
  struct BinaryROAdd
    {
    typedef BOOST_TYPEOF_TPL(T() + U()) type;
    };
  
  template <class T,class U>
  struct BinaryROSubtract
    {
    typedef BOOST_TYPEOF_TPL(T() - U()) type;
    };
  
  template <class T,class U>
  struct BinaryROLeftShift
    {
    typedef BOOST_TYPEOF_TPL(T() << U()) type;
    };
  
  template <class T,class U>
  struct BinaryRORightShift
    {
    typedef BOOST_TYPEOF_TPL(T() >> U()) type;
    };
  
  template <class T,class U>
  struct BinaryROAnd
    {
    typedef BOOST_TYPEOF_TPL(T() & U()) type;
    };
  
  template <class T,class U>
  struct BinaryROOr
    {
    typedef BOOST_TYPEOF_TPL(T() | U()) type;
    };
  
  template <class T,class U>
  struct BinaryROXOr
    {
    typedef BOOST_TYPEOF_TPL(T() ^ U()) type;
    };
  
  template <class T,class U>
  struct BinaryROGreater
    {
    typedef BOOST_TYPEOF_TPL(T() > U()) type;
    };
  
  template <class T,class U>
  struct BinaryROLess
    {
    typedef BOOST_TYPEOF_TPL(T() < U()) type;
    };
  
  template <class T,class U>
  struct BinaryROGreaterOrEqual
    {
    typedef BOOST_TYPEOF_TPL(T() >= U()) type;
    };
  
  template <class T,class U>
  struct BinaryROLessOrEqual
    {
    typedef BOOST_TYPEOF_TPL(T() <= U()) type;
    };
  
  template <class T,class U>
  struct BinaryROEqual
    {
    typedef BOOST_TYPEOF_TPL(T() == U()) type;
    };
  
  template <class T,class U>
  struct BinaryRONotEqual
    {
    typedef BOOST_TYPEOF_TPL(T() != U()) type;
    };
  
  template <class T,class U>
  struct BinaryROLogicalAnd
    {
    typedef BOOST_TYPEOF_TPL(T() && U()) type;
    };
  
  template <class T,class U>
  struct BinaryROLogicalOr
    {
    typedef BOOST_TYPEOF_TPL(T() || U()) type;
    };
  
  template <class T>
  struct BinaryROMultiplySame
    {
    typedef typename BinaryROMultiply<T,T>::type type;
    };
  
  template <class T>
  struct BinaryRODivideSame
    {
    typedef typename BinaryRODivide<T,T>::type type;
    };
  
  template <class T>
  struct BinaryROModuloSame
    {
    typedef typename BinaryROModulo<T,T>::type type;
    };
  
  template <class T>
  struct BinaryROAddSame
    {
    typedef typename BinaryROAdd<T,T>::type type;
    };
  
  template <class T>
  struct BinaryROSubtractSame
    {
    typedef typename BinaryROSubtract<T,T>::type type;
    };
  
  template <class T>
  struct BinaryROLeftShiftSame
    {
    typedef typename BinaryROLeftShift<T,T>::type type;
    };
  
  template <class T>
  struct BinaryRORightShiftSame
    {
    typedef typename BinaryRORightShift<T,T>::type type;
    };
  
  template <class T>
  struct BinaryROAndSame
    {
    typedef typename BinaryROAnd<T,T>::type type;
    };
  
  template <class T>
  struct BinaryROOrSame
    {
    typedef typename BinaryROOr<T,T>::type type;
    };
  
  template <class T>
  struct BinaryROXOrSame
    {
    typedef typename BinaryROXOr<T,T>::type type;
    };
  
  template <class T>
  struct BinaryROGreaterSame
    {
    typedef typename BinaryROGreater<T,T>::type type;
    };
  
  template <class T>
  struct BinaryROLessSame
    {
    typedef typename BinaryROLess<T,T>::type type;
    };
  
  template <class T>
  struct BinaryROGreaterOrEqualSame
    {
    typedef typename BinaryROGreaterOrEqual<T,T>::type type;
    };
  
  template <class T>
  struct BinaryROLessOrEqualSame
    {
    typedef typename BinaryROLessOrEqual<T,T>::type type;
    };
  
  template <class T>
  struct BinaryROEqualSame
    {
    typedef typename BinaryROEqual<T,T>::type type;
    };
  
  template <class T>
  struct BinaryRONotEqualSame
    {
    typedef typename BinaryRONotEqual<T,T>::type type;
    };
  
  template <class T>
  struct BinaryROLogicalAndSame
    {
    typedef typename BinaryROLogicalAnd<T,T>::type type;
    };
  
  template <class T>
  struct BinaryROLogicalOrSame
    {
    typedef typename BinaryROLogicalOr<T,T>::type type;
    };
  
  }
}

#endif

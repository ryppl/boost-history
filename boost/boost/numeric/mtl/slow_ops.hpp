#ifndef SLOW_OPS_HPP
#define SLOW_OPS_HPP

#include <vector>

template <class T>
std::vector<T> operator*(T a, const std::vector<T>& x)
{
  std::vector<T> ret(x.size());
  for (typename std::vector<T>::size_type i = 0; i < x.size(); ++i)
    ret[i] = a * x[i];
  return ret;
}

template <class T>
std::vector<T> operator+(const std::vector<T>& x, const std::vector<T>& y)
{
  std::vector<T> ret(x.size());
  for (typename std::vector<T>::size_type i = 0; i < x.size(); ++i)
    ret[i] = x[i] + y[i];
  return ret;
}

template <class T>
std::vector<T>& operator+=(std::vector<T>& x, const std::vector<T>& y)
{
  for (typename std::vector<T>::size_type i = 0; i < x.size(); ++i)
    x[i] += y[i];
  return x;
}


#endif // SLOW_OPS_HPP

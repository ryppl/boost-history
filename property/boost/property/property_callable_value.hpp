#if !defined(PROPERTY_CALLABLE_VALUE_HPP)
#define PROPERTY_CALLABLE_VALUE_HPP

#include "PropertyInterface.h"
#include "PropertyTag.h"
#include "PropertyException.h"
#include "PropertyOperators.h"
#include <boost/function.hpp>
#include <boost/type_traits/is_const.hpp>
#include <boost/mpl/if.hpp>

#if defined(BOOST_MSVC)

#pragma warning (push)
#pragma warning (disable: 4250)

#endif

namespace properties
{

  template <class T, 
            class Access = typename boost::mpl::if_
                        <
                        boost::is_const<T>,
                        read_tag,
                        read_write_tag
                        >::type
           >
  class prop_callable;
  
  template <class T>
  class prop_callable<T,read_tag> : 
    public virtual i_property_read<T>
  {

    public:
  
    explicit prop_callable(boost::function<T ()> g) : 
      fg(g) 
      { 
      }
  
    operator T() const 
      { 
      return(get()); 
      }
  
    T get() const 
      { 
      if (fg == 0) 
        { 
        throw property_read_exception(); 
        } 
      return(fg()); 
      }
  
    private:
  
    boost::function<T ()> fg;
    
    prop_callable & operator = (const prop_callable & arg);
    
  };

  template <class T>
  class prop_callable<T,write_tag> : 
    public virtual i_property_write<T>
  {

    public:
  
    explicit prop_callable(boost::function<void (T)> s) : 
      fs(s) 
      { 
      }
      
    prop_callable(boost::function<void (T)> s,T arg) : 
      fs(s) 
      { 
      if (fs == 0) 
        { 
        throw property_write_exception(); 
        } 
      fs(arg); 
      }
      
    template<class U>
    prop_callable(boost::function<void (T)> s,U arg) : 
      fs(s)
      { 
      if (fs == 0) 
        { 
        throw property_write_exception(); 
        } 
      fs(static_cast<T>(arg)); 
      }
      
    prop_callable & operator = (T arg) 
      { 
      set(arg); 
      return(*this); 
      }
      
    template<class U>
    prop_callable & operator = (U arg) 
      { 
      set(static_cast<T>(arg)); 
      return(*this); 
      }
      
    void set(T arg) 
      { 
      if (fs == 0) 
        { 
        throw property_write_exception(); 
        } 
      fs(arg); 
      }
  
    private:
  
    boost::function<void (T)> fs;
    
    prop_callable & operator = (const prop_callable & arg);
    
  };
    
  template <class T>
  struct prop_callable<T,read_write_tag> : 
    prop_callable<T,read_tag>, 
    prop_callable<T,write_tag>,
    i_property_read_write<T>
  {
  
    prop_callable(const prop_callable & arg) :
      prop_callable<T,read_tag>(arg),
      prop_callable<T,write_tag>(static_cast<const prop_callable<T,write_tag> &>(arg)) 
      {
      }
    
    prop_callable(boost::function<T ()> g,boost::function<void (T)> s) : 
      prop_callable<T,read_tag>(g),
      prop_callable<T,write_tag>(s) 
      { 
      }
      
    prop_callable(boost::function<T ()> g,boost::function<void (T)> s,T arg) : 
      prop_callable<T,read_tag>(g),
      prop_callable<T,write_tag>(s,arg) 
      { 
      }
      
    template<class U>
    prop_callable(boost::function<T ()> g,boost::function<void (T)> s,U arg) : 
      prop_callable<T,read_tag>(g),
      prop_callable<T,write_tag>(s,arg) 
      { 
      }
      
    prop_callable & operator = (const prop_callable & arg)
      {
      return(*this = static_cast<T>(arg));
      }
      
    prop_callable & operator = (T arg) 
      { 
      (static_cast<prop_callable<T,write_tag> &>(*this)) = arg; 
      return(*this); 
      }
      
    template<class U>
    prop_callable & operator = (U arg) 
      { 
      (static_cast<prop_callable<T,write_tag> &>(*this)) = arg; 
      return(*this); 
      }
      
  };

template <class T> 
typename boost::enable_if
  <
  typename detail::omf_increment<T>::type,
  prop_callable<T,read_write_tag> &
  >::type
operator ++ (prop_callable<T,read_write_tag> & value)
  {
  
  T t(value.get());
  
  ++t;
  value.set(t);
  return(value);
  }
      
template <class T> 
typename boost::enable_if
  <
  typename detail::omf_decrement<T>::type,
  prop_callable<T,read_write_tag> &
  >::type
operator -- (prop_callable<T,read_write_tag> & value)
  {
  
  T t(value.get());
  
  --t;
  value.set(t);
  return(value);
  }
  
template <class T> 
typename boost::enable_if
  <
  typename detail::omf_assign_multiply_same<T>::type,
  prop_callable<T,read_write_tag> &
  >::type
operator *= (prop_callable<T,read_write_tag> & first,T second)
  {
  
  typename detail::binary_ro_multiply_same<T>::type t(first.get() * second);
  
  first.set(static_cast<T>(t));
  return(first);
  }

template <class T,class U>
typename boost::enable_if
  <
  typename detail::omf_assign_multiply<T,U>::type,
  prop_callable<T,read_write_tag> &
  >::type
operator *= (prop_callable<T,read_write_tag> & first,U second)
  {
  
  typename detail::binary_ro_multiply<T,U>::type t(first.get() * second);
  
  first.set(static_cast<T>(t));
  return(first);
  }

template <class T> 
typename boost::enable_if
  <
  typename detail::omf_assign_divide_same<T>::type,
  prop_callable<T,read_write_tag> &
  >::type
operator /= (prop_callable<T,read_write_tag> & first,T second)
  {
  
  typename detail::binary_ro_divide_same<T>::type t(first.get() / second);
  
  first.set(static_cast<T>(t));
  return(first);
  }
  
template <class T,class U>
typename boost::enable_if
  <
  typename detail::omf_assign_divide<T,U>::type,
  prop_callable<T,read_write_tag> &
  >::type
operator /= (prop_callable<T,read_write_tag> & first,U second)
  {
  
  typename detail::binary_ro_divide<T,U>::type t(first.get() / second);
  
  first.set(static_cast<T>(t));
  return(first);
  }
  
template <class T> 
typename boost::enable_if
  <
  typename detail::omf_assign_modulo_same<T>::type,
  prop_callable<T,read_write_tag> &
  >::type
operator %= (prop_callable<T,read_write_tag> & first,T second)
  {
  
  typename detail::binary_ro_modulo_same<T>::type t(first.get() % second);
  
  first.set(static_cast<T>(t));
  return(first);
  }
  
template <class T,class U> 
typename boost::enable_if
  <
  typename detail::omf_assign_modulo<T,U>::type,
  prop_callable<T,read_write_tag> &
  >::type
operator %= (prop_callable<T,read_write_tag> & first,U second)
  {
  
  typename detail::binary_ro_modulo<T,U>::type t(first.get() % second);
  
  first.set(static_cast<T>(t));
  return(first);
  }
  
template <class T> 
typename boost::enable_if
  <
  typename detail::omf_assign_add_same<T>::type,
  prop_callable<T,read_write_tag> &
  >::type
operator += (prop_callable<T,read_write_tag> & first,T second)
  {
  
  typename detail::binary_ro_add_same<T>::type t(first.get() + second);
  
  first.set(static_cast<T>(t));
  return(first);
  }

template <class T,class U> 
typename boost::enable_if
  <
  typename detail::omf_assign_add<T,U>::type,
  prop_callable<T,read_write_tag> &
  >::type
operator += (prop_callable<T,read_write_tag> & first,U second)
  {
  
  typename detail::binary_ro_add<T,U>::type t(first.get() + second);
  
  first.set(static_cast<T>(t));
  return(first);
  }

template <class T> 
typename boost::enable_if
  <
  typename detail::omf_assign_subtract_same<T>::type,
  prop_callable<T,read_write_tag> &
  >::type
operator -= (prop_callable<T,read_write_tag> & first,T second)
  {
  
  typename detail::binary_ro_subtract_same<T>::type t(first.get() - second);
  
  first.set(static_cast<T>(t));
  return(first);
  }

template <class T,class U> 
typename boost::enable_if
  <
  typename detail::omf_assign_subtract<T,U>::type,
  prop_callable<T,read_write_tag> &
  >::type
operator -= (prop_callable<T,read_write_tag> & first,U second)
  {
  
  typename detail::binary_ro_subtract<T,U>::type t(first.get() - second);
  
  first.set(static_cast<T>(t));
  return(first);
  }

template <class T> 
typename boost::enable_if
  <
  typename detail::omf_assign_left_shift_same<T>::type,
  prop_callable<T,read_write_tag> &
  >::type
operator <<= (prop_callable<T,read_write_tag> & first,T second)
  {
  
  typename detail::binary_ro_left_shift_same<T>::type t(first.get() << second);
  
  first.set(static_cast<T>(t));
  return(first);
  }
  
template <class T,class U> 
typename boost::enable_if
  <
  typename detail::omf_assign_left_shift<T,U>::type,
  prop_callable<T,read_write_tag> &
  >::type
operator <<= (prop_callable<T,read_write_tag> & first,U second)
  {
  
  typename detail::binary_ro_left_shift<T,U>::type t(first.get() << second);
  
  first.set(static_cast<T>(t));
  return(first);
  }
  
template <class T> 
typename boost::enable_if
  <
  typename detail::omf_assign_right_shift_same<T>::type,
  prop_callable<T,read_write_tag> &
  >::type
operator >>= (prop_callable<T,read_write_tag> & first,T second)
  {
  
  typename detail::binary_ro_right_shift_same<T>::type t(first.get() >> second);
  
  first.set(static_cast<T>(t));
  return(first);
  }
  
template <class T,class U>
typename boost::enable_if
  <
  typename detail::omf_assign_right_shift<T,U>::type,
  prop_callable<T,read_write_tag> &
  >::type
operator >>= (prop_callable<T,read_write_tag> & first,U second)
  {
  
  typename detail::binary_ro_right_shift<T,U>::type t(first.get() >> second);
  
  first.set(static_cast<T>(t));
  return(first);
  }
  
template <class T> 
typename boost::enable_if
  <
  typename detail::omf_assign_and_same<T>::type,
  prop_callable<T,read_write_tag> &
  >::type
operator &= (prop_callable<T,read_write_tag> & first,T second)
  {
  
  typename detail::binary_ro_and_same<T>::type t(first.get() & second);
  
  first.set(static_cast<T>(t));
  return(first);
  }
  
template <class T,class U>
typename boost::enable_if
  <
  typename detail::omf_assign_and<T,U>::type,
  prop_callable<T,read_write_tag> &
  >::type
operator &= (prop_callable<T,read_write_tag> & first,U second)
  {
  
  typename detail::binary_ro_and<T,U>::type t(first.get() & second);
  
  first.set(static_cast<T>(t));
  return(first);
  }
  
template <class T> 
typename boost::enable_if
  <
  typename detail::omf_assign_xor_same<T>::type,
  prop_callable<T,read_write_tag> &
  >::type
operator ^= (prop_callable<T,read_write_tag> & first,T second)
  {
  
  typename detail::binary_ro_xor_same<T>::type t(first.get() ^ second);
  
  first.set(static_cast<T>(t));
  return(first);
  }
  
template <class T,class U> 
typename boost::enable_if
  <
  typename detail::omf_assign_xor<T,U>::type,
  prop_callable<T,read_write_tag> &
  >::type
operator ^= (prop_callable<T,read_write_tag> & first,U second)
  {
  
  typename detail::binary_ro_xor<T,U>::type t(first.get() ^ second);
  
  first.set(static_cast<T>(t));
  return(first);
  }
  
template <class T> 
typename boost::enable_if
  <
  typename detail::omf_assign_or_same<T>::type,
  prop_callable<T,read_write_tag> &
  >::type
operator |= (prop_callable<T,read_write_tag> & first,T second)
  {
  
  typename detail::binary_ro_or_same<T>::type t(first.get() | second);
  
  first.set(static_cast<T>(t));
  return(first);
  }
  
template <class T,class U> 
typename boost::enable_if
  <
  typename detail::omf_assign_or<T,U>::type,
  prop_callable<T,read_write_tag> &
  >::type
operator |= (prop_callable<T,read_write_tag> & first,U second)
  {
  
  typename detail::binary_ro_or<T,U>::type t(first.get() | second);
  
  first.set(static_cast<T>(t));
  return(first);
  }
}

#if defined(BOOST_MSVC)

#pragma warning (pop)

#endif

#endif

#if !defined(PROPERTY_FUNCTION_VALUE_HPP)
#define PROPERTY_FUNCTION_VALUE_HPP

#include "property_interface.hpp"
#include "property_tag.hpp"
#include "property_exception.hpp"
#include "property_operators.hpp"

#if defined(BOOST_MSVC)

#pragma warning (push)
#pragma warning (disable: 4250)

#endif

namespace properties
{

  template <class T, T (*g)()>
  struct prop_read_function : 
    virtual i_property_read<T>
  {
  
    operator T() const 
      { 
      return(get()); 
      }
    
    T get() const 
      { 
      if (g == 0) 
        { 
        throw property_read_exception(); 
        } 
      return((*g)()); 
      }
      
    private:
    
    prop_read_function & operator = (const prop_read_function &);
    
  };
  
  template <class T, 
            void (*s)(T) 
           >
  struct prop_write_function : 
    virtual i_property_write<T>
  {
  
    prop_write_function() 
      { 
      }
      
    explicit prop_write_function(T arg) 
      { 
      if (s == 0) 
        { 
        throw property_write_exception(); 
        } 
      (*s)(arg); 
      }
    
    template<class U>
    explicit prop_write_function(U arg) 
      { 
      if (s == 0) 
        { 
        throw property_write_exception(); 
        } 
      (*s)(static_cast<T>(arg)); 
      }
    
    prop_write_function & operator = (T arg) 
      { 
      set(arg); 
      return(*this); 
      }
    
    template<class U>
    prop_write_function & operator = (U arg) 
      { 
      set(static_cast<T>(arg)); 
      return(*this); 
      }
    
    void set(T arg) 
      { 
      if (s == 0) 
        { 
        throw property_write_exception(); 
        } 
      (*s)(arg); 
      }
      
    private:
    
    prop_write_function & operator = (const prop_write_function &);
    
  };


  template <class T, T (*g)(), void (*s)(T)>
  struct prop_function : 
    prop_read_function<T,g>, 
    prop_write_function<T,s>
  {
  
    prop_function() 
      { 
      }
      
    explicit prop_function(T arg) : 
      prop_write_function<T,s>(arg) 
      { 
      }
    
    prop_function(const prop_function & arg) :
      prop_read_function<T,g>(arg),
      prop_write_function<T,s>(static_cast<const prop_write_function<T,s> &>(arg))
      { 
      }
      
    template<class U>
    explicit prop_function(U arg) : 
      prop_write_function<T,s>(arg) 
      { 
      }
    
    prop_function & operator = (const prop_function & arg)
      {
      return(*this = static_cast<T>(arg));
      }
    
    prop_function & operator = (T arg) 
      { 
      (static_cast<prop_write_function<T,s> &>(*this)) = arg; 
      return(*this); 
      }
    
    template<class U>
    prop_function & operator = (U arg) 
      { 
      (static_cast<prop_write_function<T,s> &>(*this)) = arg; 
      return(*this); 
      }
    
  };

template <class T, 
          T (*g)(),
          void (*s)(T) 
          >
typename boost::enable_if
  <
  typename detail::omf_increment<T>::type,
  prop_function<T,g,s> &
  >::type
operator ++ (prop_function<T,g,s> & value)
  {
  
  T t(value.get());
  
  ++t;
  value.set(t);
  return(value);
  }
      
template <class T, 
          T (*g)(),
          void (*s)(T)
          >
typename boost::enable_if
  <
  typename detail::omf_decrement<T>::type,
  prop_function<T,g,s> &
  >::type
operator -- (prop_function<T,g,s> & value)
  {
  
  T t(value.get());
  
  --t;
  value.set(t);
  return(value);
  }
  
template <class T, 
          T (*g)(),
          void (*s)(T) 
          >
typename boost::enable_if
  <
  typename detail::omf_assign_multiply_same<T>::type,
  prop_function<T,g,s> &
  >::type
operator *= (prop_function<T,g,s> & first,T second)
  {
  
  typename detail::binary_ro_multiply_same<T>::type t(first.get() * second);
  
  first.set(static_cast<T>(t));
  return(first);
  }

template <class T, 
          T (*g)(),
          void (*s)(T),
          class U>
typename boost::enable_if
  <
  typename detail::omf_assign_multiply<T,U>::type,
  prop_function<T,g,s> &
  >::type
operator *= (prop_function<T,g,s> & first,U second)
  {
  
  typename detail::binary_ro_multiply<T,U>::type t(first.get() * second);
  
  first.set(static_cast<T>(t));
  return(first);
  }

template <class T, 
          T (*g)(),
          void (*s)(T) 
          >
typename boost::enable_if
  <
  typename detail::omf_assign_divide_same<T>::type,
  prop_function<T,g,s> &
  >::type
operator /= (prop_function<T,g,s> & first,T second)
  {
  
  typename detail::binary_ro_divide_same<T>::type t(first.get() / second);
  
  first.set(static_cast<T>(t));
  return(first);
  }
  
template <class T, 
          T (*g)(),
          void (*s)(T),
          class U>
typename boost::enable_if
  <
  typename detail::omf_assign_divide<T,U>::type,
  prop_function<T,g,s> &
  >::type
operator /= (prop_function<T,g,s> & first,U second)
  {
  
  typename detail::binary_ro_divide<T,U>::type t(first.get() / second);
  
  first.set(static_cast<T>(t));
  return(first);
  }
  
template <class T, 
          T (*g)(),
          void (*s)(T) 
          >
typename boost::enable_if
  <
  typename detail::omf_assign_modulo_same<T>::type,
  prop_function<T,g,s> &
  >::type
operator %= (prop_function<T,g,s> & first,T second)
  {
  
  typename detail::binary_ro_modulo_same<T>::type t(first.get() % second);
  
  first.set(static_cast<T>(t));
  return(first);
  }
  
template <class T, 
          T (*g)(),
          void (*s)(T),
          class U> 
typename boost::enable_if
  <
  typename detail::omf_assign_modulo<T,U>::type,
  prop_function<T,g,s> &
  >::type
operator %= (prop_function<T,g,s> & first,U second)
  {
  
  typename detail::binary_ro_modulo<T,U>::type t(first.get() % second);
  
  first.set(static_cast<T>(t));
  return(first);
  }
  
template <class T, 
          T (*g)(),
          void (*s)(T) 
          >
typename boost::enable_if
  <
  typename detail::omf_assign_add_same<T>::type,
  prop_function<T,g,s> &
  >::type
operator += (prop_function<T,g,s> & first,T second)
  {
  
  typename detail::binary_ro_add_same<T>::type t(first.get() + second);
  
  first.set(static_cast<T>(t));
  return(first);
  }

template <class T, 
          T (*g)(),
          void (*s)(T),
          class U> 
typename boost::enable_if
  <
  typename detail::omf_assign_add<T,U>::type,
  prop_function<T,g,s> &
  >::type
operator += (prop_function<T,g,s> & first,U second)
  {
  
  typename detail::binary_ro_add<T,U>::type t(first.get() + second);
  
  first.set(static_cast<T>(t));
  return(first);
  }

template <class T, 
          T (*g)(),
          void (*s)(T) 
          >
typename boost::enable_if
  <
  typename detail::omf_assign_subtract_same<T>::type,
  prop_function<T,g,s> &
  >::type
operator -= (prop_function<T,g,s> & first,T second)
  {
  
  typename detail::binary_ro_subtract_same<T>::type t(first.get() - second);
  
  first.set(static_cast<T>(t));
  return(first);
  }

template <class T, 
          T (*g)(),
          void (*s)(T),
          class U> 
typename boost::enable_if
  <
  typename detail::omf_assign_subtract<T,U>::type,
  prop_function<T,g,s> &
  >::type
operator -= (prop_function<T,g,s> & first,U second)
  {
  
  typename detail::binary_ro_subtract<T,U>::type t(first.get() - second);
  
  first.set(static_cast<T>(t));
  return(first);
  }

template <class T, 
          T (*g)(),
          void (*s)(T) 
          >
typename boost::enable_if
  <
  typename detail::omf_assign_left_shift_same<T>::type,
  prop_function<T,g,s> &
  >::type
operator <<= (prop_function<T,g,s> & first,T second)
  {
  
  typename detail::binary_ro_left_shift_same<T>::type t(first.get() << second);
  
  first.set(static_cast<T>(t));
  return(first);
  }
  
template <class T, 
          T (*g)(),
          void (*s)(T),
          class U> 
typename boost::enable_if
  <
  typename detail::omf_assign_left_shift<T,U>::type,
  prop_function<T,g,s> &
  >::type
operator <<= (prop_function<T,g,s> & first,U second)
  {
  
  typename detail::binary_ro_left_shift<T,U>::type t(first.get() << second);
  
  first.set(static_cast<T>(t));
  return(first);
  }
  
template <class T, 
          T (*g)(),
          void (*s)(T) 
          >
typename boost::enable_if
  <
  typename detail::omf_assign_right_shift_same<T>::type,
  prop_function<T,g,s> &
  >::type
operator >>= (prop_function<T,g,s> & first,T second)
  {
  
  typename detail::binary_ro_right_shift_same<T>::type t(first.get() >> second);
  
  first.set(static_cast<T>(t));
  return(first);
  }
  
template <class T, 
          T (*g)(),
          void (*s)(T),
          class U>
typename boost::enable_if
  <
  typename detail::omf_assign_right_shift<T,U>::type,
  prop_function<T,g,s> &
  >::type
operator >>= (prop_function<T,g,s> & first,U second)
  {
  
  typename detail::binary_ro_right_shift<T,U>::type t(first.get() >> second);
  
  first.set(static_cast<T>(t));
  return(first);
  }
  
template <class T, 
          T (*g)(),
          void (*s)(T) 
          >
typename boost::enable_if
  <
  typename detail::omf_assign_and_same<T>::type,
  prop_function<T,g,s> &
  >::type
operator &= (prop_function<T,g,s> & first,T second)
  {
  
  typename detail::binary_ro_and_same<T>::type t(first.get() & second);
  
  first.set(static_cast<T>(t));
  return(first);
  }
  
template <class T, 
          T (*g)(),
          void (*s)(T),
          class U>
typename boost::enable_if
  <
  typename detail::omf_assign_and<T,U>::type,
  prop_function<T,g,s> &
  >::type
operator &= (prop_function<T,g,s> & first,U second)
  {
  
  typename detail::binary_ro_and<T,U>::type t(first.get() & second);
  
  first.set(static_cast<T>(t));
  return(first);
  }
  
template <class T, 
          T (*g)(),
          void (*s)(T) 
          >
typename boost::enable_if
  <
  typename detail::omf_assign_xor_same<T>::type,
  prop_function<T,g,s> &
  >::type
operator ^= (prop_function<T,g,s> & first,T second)
  {
  
  typename detail::binary_ro_xor_same<T>::type t(first.get() ^ second);
  
  first.set(static_cast<T>(t));
  return(first);
  }
  
template <class T, 
          T (*g)(),
          void (*s)(T),
          class U> 
typename boost::enable_if
  <
  typename detail::omf_assign_xor<T,U>::type,
  prop_function<T,g,s> &
  >::type
operator ^= (prop_function<T,g,s> & first,U second)
  {
  
  typename detail::binary_ro_xor<T,U>::type t(first.get() ^ second);
  
  first.set(static_cast<T>(t));
  return(first);
  }
  
template <class T, 
          T (*g)(),
          void (*s)(T) 
          >
typename boost::enable_if
  <
  typename detail::omf_assign_or_same<T>::type,
  prop_function<T,g,s> &
  >::type
operator |= (prop_function<T,g,s> & first,T second)
  {
  
  typename detail::binary_ro_or_same<T>::type t(first.get() | second);
  
  first.set(static_cast<T>(t));
  return(first);
  }
  
template <class T, 
          T (*g)(),
          void (*s)(T),
          class U> 
typename boost::enable_if
  <
  typename detail::omf_assign_or<T,U>::type,
  prop_function<T,g,s> &
  >::type
operator |= (prop_function<T,g,s> & first,U second)
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

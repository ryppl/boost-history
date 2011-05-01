#if !defined(PROPERTY_MEMBER_FUNCTION_VALUE_HPP)
#define PROPERTY_MEMBER_FUNCTION_VALUE_HPP

#include "PropertyInterface.h"
#include "PropertyTag.h"
#include "PropertyException.h"
#include "PropertyOperators.h"

#if defined(BOOST_MSVC)

#pragma warning (push)
#pragma warning (disable: 4250)

#endif

namespace properties
{

  template <class T, class C, T (C::*g)()>
  class prop_read_member_function : 
    public virtual i_property_read<T>
  {
    public:
    
    explicit prop_read_member_function(C & c) : 
      cf(c) 
      { 
      }
    
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
      return((cf.*g)()); 
      }
    
    private:
    
    C & cf;
    
    prop_read_member_function & operator = (const prop_read_member_function &);
    
  };

  template <class T, class C, void (C::*s)(T)>
  class prop_write_member_function : 
    public virtual i_property_write<T>
  {
    public:
    
    explicit prop_write_member_function(C & c) : 
      cf(c) 
      { 
      }
      
    prop_write_member_function(C & c,T arg) : 
      cf(c) 
      { 
      if (s == 0) 
        { 
        throw property_write_exception(); 
        } 
      (cf.*s)(arg); 
      }
    
    template<class U>
    prop_write_member_function(C & c,U arg) : 
      cf(c) 
      { 
      if (s == 0) 
        { 
        throw property_write_exception(); 
        } 
      (cf.*s)(static_cast<T>(arg)); 
      }
    
    prop_write_member_function & operator = (T arg) 
      { 
      set(arg); 
      return(*this); 
      }
    
    template<class U>
    prop_write_member_function & operator = (U arg) 
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
      (cf.*s)(arg); 
      }
    
    private:
    
    C & cf;
    
    prop_write_member_function & operator = (const prop_write_member_function &);
    
  };

  template <class T, class C, T (C::*g)(), void (C::*s)(T)>
  struct prop_member_function : 
    prop_read_member_function<T,C,g> , 
    prop_write_member_function<T,C,s>
  {
  
    explicit prop_member_function(C & c) : 
      prop_read_member_function<T,C,g>(c) , 
      prop_write_member_function<T,C,s>(c) 
      { 
      }
      
    prop_member_function(const prop_member_function & arg) : 
      prop_read_member_function<T,C,g>(arg) , 
      prop_write_member_function<T,C,s>(static_cast<const prop_write_member_function<T,C,s> &>(arg)) 
      { 
      }
    
    prop_member_function(C & c,T arg) : 
      prop_read_member_function<T,C,g>(c) , 
      prop_write_member_function<T,C,s>(c,arg) 
      { 
      }
    
    template<class U>
    prop_member_function(C & c,U arg) : 
      prop_read_member_function<T,C,g>(c) , 
      prop_write_member_function<T,C,s>(c,arg) 
      { 
      }
    
    prop_member_function & operator = (const prop_member_function & arg)
      {
      return(*this = static_cast<T>(arg));
      }
    
    prop_member_function & operator = (T arg) 
      { 
      (static_cast<prop_write_member_function<T,C,s> &>(*this)) = arg; 
      return(*this); 
      }
    
    template<class U>
    prop_member_function & operator = (U arg) 
      { 
      (static_cast<prop_write_member_function<T,C,s> &>(*this)) = arg; 
      return(*this); 
      }
    
  };

template <class T, 
          class C, 
          T (C::*g)(), 
          void (C::*s)(T)
         > 
typename boost::enable_if
  <
  typename detail::omf_increment<T>::type,
  prop_member_function<T,C,g,s> &
  >::type
operator ++ (prop_member_function<T,C,g,s> & value)
  {
  
  T t(value.get());
  
  ++t;
  value.set(t);
  return(value);
  }
      
template <class T, 
          class C, 
          T (C::*g)(), 
          void (C::*s)(T)
         > 
typename boost::enable_if
  <
  typename detail::omf_decrement<T>::type,
  prop_member_function<T,C,g,s> &
  >::type
operator -- (prop_member_function<T,C,g,s> & value)
  {
  
  T t(value.get());
  
  --t;
  value.set(t);
  return(value);
  }
  
template <class T, 
          class C, 
          T (C::*g)(), 
          void (C::*s)(T)
         > 
typename boost::enable_if
  <
  typename detail::omf_assign_multiply_same<T>::type,
  prop_member_function<T,C,g,s> &
  >::type
operator *= (prop_member_function<T,C,g,s> & first,T second)
  {
  
  typename detail::binary_ro_multiply_same<T>::type t(first.get() * second);
  
  first.set(static_cast<T>(t));
  return(first);
  }

template <class T, 
          class C, 
          T (C::*g)(), 
          void (C::*s)(T),
          class U
         > 
typename boost::enable_if
  <
  typename detail::omf_assign_multiply<T,U>::type,
  prop_member_function<T,C,g,s> &
  >::type
operator *= (prop_member_function<T,C,g,s> & first,U second)
  {
  
  typename detail::binary_ro_multiply<T,U>::type t(first.get() * second);
  
  first.set(static_cast<T>(t));
  return(first);
  }

template <class T, 
          class C, 
          T (C::*g)(), 
          void (C::*s)(T)
         > 
typename boost::enable_if
  <
  typename detail::omf_assign_divide_same<T>::type,
  prop_member_function<T,C,g,s> &
  >::type
operator /= (prop_member_function<T,C,g,s> & first,T second)
  {
  
  typename detail::binary_ro_divide_same<T>::type t(first.get() / second);
  
  first.set(static_cast<T>(t));
  return(first);
  }
  
template <class T, 
          class C, 
          T (C::*g)(), 
          void (C::*s)(T),
          class U
         > 
typename boost::enable_if
  <
  typename detail::omf_assign_divide<T,U>::type,
  prop_member_function<T,C,g,s> &
  >::type
operator /= (prop_member_function<T,C,g,s> & first,U second)
  {
  
  typename detail::binary_ro_divide<T,U>::type t(first.get() / second);
  
  first.set(static_cast<T>(t));
  return(first);
  }
  
template <class T, 
          class C, 
          T (C::*g)(), 
          void (C::*s)(T)
         > 
typename boost::enable_if
  <
  typename detail::omf_assign_modulo_same<T>::type,
  prop_member_function<T,C,g,s> &
  >::type
operator %= (prop_member_function<T,C,g,s> & first,T second)
  {
  
  typename detail::binary_ro_modulo_same<T>::type t(first.get() % second);
  
  first.set(static_cast<T>(t));
  return(first);
  }
  
template <class T, 
          class C, 
          T (C::*g)(), 
          void (C::*s)(T),
          class U
         > 
typename boost::enable_if
  <
  typename detail::omf_assign_modulo<T,U>::type,
  prop_member_function<T,C,g,s> &
  >::type
operator %= (prop_member_function<T,C,g,s> & first,U second)
  {
  
  typename detail::binary_ro_modulo<T,U>::type t(first.get() % second);
  
  first.set(static_cast<T>(t));
  return(first);
  }
  
template <class T, 
          class C, 
          T (C::*g)(), 
          void (C::*s)(T)
         > 
typename boost::enable_if
  <
  typename detail::omf_assign_add_same<T>::type,
  prop_member_function<T,C,g,s> &
  >::type
operator += (prop_member_function<T,C,g,s> & first,T second)
  {
  
  typename detail::binary_ro_add_same<T>::type t(first.get() + second);
  
  first.set(static_cast<T>(t));
  return(first);
  }

template <class T, 
          class C, 
          T (C::*g)(), 
          void (C::*s)(T),
          class U
         > 
typename boost::enable_if
  <
  typename detail::omf_assign_add<T,U>::type,
  prop_member_function<T,C,g,s> &
  >::type
operator += (prop_member_function<T,C,g,s> & first,U second)
  {
  
  typename detail::binary_ro_add<T,U>::type t(first.get() + second);
  
  first.set(static_cast<T>(t));
  return(first);
  }

template <class T, 
          class C, 
          T (C::*g)(), 
          void (C::*s)(T)
         > 
typename boost::enable_if
  <
  typename detail::omf_assign_subtract_same<T>::type,
  prop_member_function<T,C,g,s> &
  >::type
operator -= (prop_member_function<T,C,g,s> & first,T second)
  {
  
  typename detail::binary_ro_subtract_same<T>::type t(first.get() - second);
  
  first.set(static_cast<T>(t));
  return(first);
  }

template <class T, 
          class C, 
          T (C::*g)(), 
          void (C::*s)(T),
          class U
         > 
typename boost::enable_if
  <
  typename detail::omf_assign_subtract<T,U>::type,
  prop_member_function<T,C,g,s> &
  >::type
operator -= (prop_member_function<T,C,g,s> & first,U second)
  {
  
  typename detail::binary_ro_subtract<T,U>::type t(first.get() - second);
  
  first.set(static_cast<T>(t));
  return(first);
  }

template <class T, 
          class C, 
          T (C::*g)(), 
          void (C::*s)(T)
         > 
typename boost::enable_if
  <
  typename detail::omf_assign_left_shift_same<T>::type,
  prop_member_function<T,C,g,s> &
  >::type
operator <<= (prop_member_function<T,C,g,s> & first,T second)
  {
  
  typename detail::binary_ro_left_shift_same<T>::type t(first.get() << second);
  
  first.set(static_cast<T>(t));
  return(first);
  }
  
template <class T, 
          class C, 
          T (C::*g)(), 
          void (C::*s)(T),
          class U
         > 
typename boost::enable_if
  <
  typename detail::omf_assign_left_shift<T,U>::type,
  prop_member_function<T,C,g,s> &
  >::type
operator <<= (prop_member_function<T,C,g,s> & first,U second)
  {
  
  typename detail::binary_ro_left_shift<T,U>::type t(first.get() << second);
  
  first.set(static_cast<T>(t));
  return(first);
  }
  
template <class T, 
          class C, 
          T (C::*g)(), 
          void (C::*s)(T)
         > 
typename boost::enable_if
  <
  typename detail::omf_assign_right_shift_same<T>::type,
  prop_member_function<T,C,g,s> &
  >::type
operator >>= (prop_member_function<T,C,g,s> & first,T second)
  {
  
  typename detail::binary_ro_right_shift_same<T>::type t(first.get() >> second);
  
  first.set(static_cast<T>(t));
  return(first);
  }
  
template <class T, 
          class C, 
          T (C::*g)(), 
          void (C::*s)(T),
          class U
         > 
typename boost::enable_if
  <
  typename detail::omf_assign_right_shift<T,U>::type,
  prop_member_function<T,C,g,s> &
  >::type
operator >>= (prop_member_function<T,C,g,s> & first,U second)
  {
  
  typename detail::binary_ro_right_shift<T,U>::type t(first.get() >> second);
  
  first.set(static_cast<T>(t));
  return(first);
  }
  
template <class T, 
          class C, 
          T (C::*g)(), 
          void (C::*s)(T)
         > 
typename boost::enable_if
  <
  typename detail::omf_assign_and_same<T>::type,
  prop_member_function<T,C,g,s> &
  >::type
operator &= (prop_member_function<T,C,g,s> & first,T second)
  {
  
  typename detail::binary_ro_and_same<T>::type t(first.get() & second);
  
  first.set(static_cast<T>(t));
  return(first);
  }
  
template <class T, 
          class C, 
          T (C::*g)(), 
          void (C::*s)(T),
          class U
         > 
typename boost::enable_if
  <
  typename detail::omf_assign_and<T,U>::type,
  prop_member_function<T,C,g,s> &
  >::type
operator &= (prop_member_function<T,C,g,s> & first,U second)
  {
  
  typename detail::binary_ro_and<T,U>::type t(first.get() & second);
  
  first.set(static_cast<T>(t));
  return(first);
  }
  
template <class T, 
          class C, 
          T (C::*g)(), 
          void (C::*s)(T)
         > 
typename boost::enable_if
  <
  typename detail::omf_assign_xor_same<T>::type,
  prop_member_function<T,C,g,s> &
  >::type
operator ^= (prop_member_function<T,C,g,s> & first,T second)
  {
  
  typename detail::binary_ro_xor_same<T>::type t(first.get() ^ second);
  
  first.set(static_cast<T>(t));
  return(first);
  }
  
template <class T, 
          class C, 
          T (C::*g)(), 
          void (C::*s)(T),
          class U
         > 
typename boost::enable_if
  <
  typename detail::omf_assign_xor<T,U>::type,
  prop_member_function<T,C,g,s> &
  >::type
operator ^= (prop_member_function<T,C,g,s> & first,U second)
  {
  
  typename detail::binary_ro_xor<T,U>::type t(first.get() ^ second);
  
  first.set(static_cast<T>(t));
  return(first);
  }
  
template <class T, 
          class C, 
          T (C::*g)(), 
          void (C::*s)(T)
         > 
typename boost::enable_if
  <
  typename detail::omf_assign_or_same<T>::type,
  prop_member_function<T,C,g,s> &
  >::type
operator |= (prop_member_function<T,C,g,s> & first,T second)
  {
  
  typename detail::binary_ro_or_same<T>::type t(first.get() | second);
  
  first.set(static_cast<T>(t));
  return(first);
  }
  
template <class T, 
          class C, 
          T (C::*g)(), 
          void (C::*s)(T),
          class U
         > 
typename boost::enable_if
  <
  typename detail::omf_assign_or<T,U>::type,
  prop_member_function<T,C,g,s> &
  >::type
operator |= (prop_member_function<T,C,g,s> & first,U second)
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

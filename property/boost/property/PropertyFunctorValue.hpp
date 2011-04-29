#if !defined(PROPERTY_FUNCTOR_VALUE_HPP)
#define PROPERTY_FUNCTOR_VALUE_HPP

#include "PropertyInterface.h"
#include "PropertyTag.h"
#include "PropertyOperators.h"
#include <boost/type_traits/is_const.hpp>
#include <boost/mpl/if.hpp>

#if defined(BOOST_MSVC)

#pragma warning (push)
#pragma warning (disable: 4250)

#endif

namespace properties
{

  template <class T, 
            class C, 
            class Access = typename boost::mpl::if_
                        <
                        boost::is_const<T>,
                        read_tag,
                        read_write_tag
                        >::type
           >
  class prop_functor;
  
  template <class T, class C>
  class prop_functor<T,C,read_tag> : 
    public virtual i_property_read<T>
  {
  
    public:
    
    explicit prop_functor(C & c) : cf(c) 
      { 
      }
    
    operator T() const 
      { 
      return(get()); 
      }
    
    T get() const 
      { 
      return(cf()); 
      }
    
    private:
    
    C & cf;
    
    prop_functor & operator = (const prop_functor &);
    
  };

  template <class T, class C>
  class prop_functor<T,C,write_tag> : 
    public virtual i_property_write<T>
  {
  
    public:
    
    explicit prop_functor(C & c) : 
      cf(c) 
      { 
      }
      
    prop_functor(C & c,T arg) : 
      cf(c) 
      { 
      cf(arg); 
      }
    
    template<class U>
    prop_functor(C & c,U arg) : 
      cf(c) 
      { 
      cf(static_cast<T>(arg)); 
      }
    
    prop_functor & operator = (T arg) 
      { 
      set(arg); 
      return(*this); 
      }
    
    template<class U>
    prop_functor & operator = (U arg) 
      { 
      set(static_cast<T>(arg)); 
      return(*this); 
      }
    
    void set(T arg) 
      { 
      cf(arg); 
      }
    
    private:
    
    C & cf;
    
    prop_functor & operator = (const prop_functor &);
    
  };

  template <class T, class C>
  struct prop_functor<T,C,read_write_tag> : 
    prop_functor<T,C,read_tag>, 
    prop_functor<T,C,write_tag>,
    i_property_read_write<T>
  {
  
    explicit prop_functor(C & c) : 
      prop_functor<T,C,read_tag>(c), 
      prop_functor<T,C,write_tag>(c) 
      { 
      }
      
    prop_functor(const prop_functor & arg) : 
      prop_functor<T,C,read_tag>(arg),
      prop_functor<T,C,write_tag>(static_cast<const prop_functor<T,C,write_tag> &>(arg))
      {
      }
    
    prop_functor(C & c,T arg) : 
      prop_functor<T,C,read_tag>(c) , 
      prop_functor<T,C,write_tag>(c,arg) 
      { 
      }
    
    template<class U>
    prop_functor(C & c,U arg) : 
      prop_functor<T,C,read_tag>(c), 
      prop_functor<T,C,write_tag>(c,arg) 
      { 
      }
    
    prop_functor & operator = (const prop_functor & arg)
      {
      return(*this = static_cast<T>(arg));
      }
      
    prop_functor & operator = (T arg) 
      { 
      (static_cast<prop_functor<T,C,write_tag> & >(*this)) = arg; 
      return(*this); 
      }
    
    template<class U>
    prop_functor & operator = (U arg) 
      { 
      (static_cast<prop_functor<T,C,write_tag> & >(*this)) = arg; 
      return(*this); 
      }
    
  };

template <class T, class C>
typename boost::enable_if
  <
  typename detail::omf_increment<T>::type,
  prop_functor<T,C,read_write_tag> &
  >::type
operator ++ (prop_functor<T,C,read_write_tag> & value)
  {
  
  T t(value.get());
  
  ++t;
  value.set(t);
  return(value);
  }
      
template <class T, class C>
typename boost::enable_if
  <
  typename detail::omf_decrement<T>::type,
  prop_functor<T,C,read_write_tag> &
  >::type
operator -- (prop_functor<T,C,read_write_tag> & value)
  {
  
  T t(value.get());
  
  --t;
  value.set(t);
  return(value);
  }
  
template <class T, class C>
typename boost::enable_if
  <
  typename detail::omf_assign_multiply_same<T>::type,
  prop_functor<T,C,read_write_tag> &
  >::type
operator *= (prop_functor<T,C,read_write_tag> & first,T second)
  {
  
  typename detail::binary_ro_multiply_same<T>::type t(first.get() * second);
  
  first.set(static_cast<T>(t));
  return(first);
  }

template <class T, class C, class U>
typename boost::enable_if
  <
  typename detail::omf_assign_multiply<T,U>::type,
  prop_functor<T,C,read_write_tag> &
  >::type
operator *= (prop_functor<T,C,read_write_tag> & first,U second)
  {
  
  typename detail::binary_ro_multiply<T,U>::type t(first.get() * second);
  
  first.set(static_cast<T>(t));
  return(first);
  }

template <class T, class C>
typename boost::enable_if
  <
  typename detail::omf_assign_divide_same<T>::type,
  prop_functor<T,C,read_write_tag> &
  >::type
operator /= (prop_functor<T,C,read_write_tag> & first,T second)
  {
  
  typename detail::binary_ro_divide_same<T>::type t(first.get() / second);
  
  first.set(static_cast<T>(t));
  return(first);
  }
  
template <class T, class C, class U>
typename boost::enable_if
  <
  typename detail::omf_assign_divide<T,U>::type,
  prop_functor<T,C,read_write_tag> &
  >::type
operator /= (prop_functor<T,C,read_write_tag> & first,U second)
  {
  
  typename detail::binary_ro_divide<T,U>::type t(first.get() / second);
  
  first.set(static_cast<T>(t));
  return(first);
  }
  
template <class T, class C>
typename boost::enable_if
  <
  typename detail::omf_assign_modulo_same<T>::type,
  prop_functor<T,C,read_write_tag> &
  >::type
operator %= (prop_functor<T,C,read_write_tag> & first,T second)
  {
  
  typename detail::binary_ro_modulo_same<T>::type t(first.get() % second);
  
  first.set(static_cast<T>(t));
  return(first);
  }
  
template <class T, class C, class U>
typename boost::enable_if
  <
  typename detail::omf_assign_modulo<T,U>::type,
  prop_functor<T,C,read_write_tag> &
  >::type
operator %= (prop_functor<T,C,read_write_tag> & first,U second)
  {
  
  typename detail::binary_ro_modulo<T,U>::type t(first.get() % second);
  
  first.set(static_cast<T>(t));
  return(first);
  }
  
template <class T, class C>
typename boost::enable_if
  <
  typename detail::omf_assign_add_same<T>::type,
  prop_functor<T,C,read_write_tag> &
  >::type
operator += (prop_functor<T,C,read_write_tag> & first,T second)
  {
  
  typename detail::binary_ro_add_same<T>::type t(first.get() + second);
  
  first.set(static_cast<T>(t));
  return(first);
  }

template <class T, class C, class U>
typename boost::enable_if
  <
  typename detail::omf_assign_add<T,U>::type,
  prop_functor<T,C,read_write_tag> &
  >::type
operator += (prop_functor<T,C,read_write_tag> & first,U second)
  {
  
  typename detail::binary_ro_add<T,U>::type t(first.get() + second);
  
  first.set(static_cast<T>(t));
  return(first);
  }

template <class T, class C>
typename boost::enable_if
  <
  typename detail::omf_assign_subtract_same<T>::type,
  prop_functor<T,C,read_write_tag> &
  >::type
operator -= (prop_functor<T,C,read_write_tag> & first,T second)
  {
  
  typename detail::binary_ro_subtract_same<T>::type t(first.get() - second);
  
  first.set(static_cast<T>(t));
  return(first);
  }

template <class T, class C, class U>
typename boost::enable_if
  <
  typename detail::omf_assign_subtract<T,U>::type,
  prop_functor<T,C,read_write_tag> &
  >::type
operator -= (prop_functor<T,C,read_write_tag> & first,U second)
  {
  
  typename detail::binary_ro_subtract<T,U>::type t(first.get() - second);
  
  first.set(static_cast<T>(t));
  return(first);
  }

template <class T, class C>
typename boost::enable_if
  <
  typename detail::omf_assign_left_shift_same<T>::type,
  prop_functor<T,C,read_write_tag> &
  >::type
operator <<= (prop_functor<T,C,read_write_tag> & first,T second)
  {
  
  typename detail::binary_ro_left_shift_same<T>::type t(first.get() << second);
  
  first.set(static_cast<T>(t));
  return(first);
  }
  
template <class T, class C, class U>
typename boost::enable_if
  <
  typename detail::omf_assign_left_shift<T,U>::type,
  prop_functor<T,C,read_write_tag> &
  >::type
operator <<= (prop_functor<T,C,read_write_tag> & first,U second)
  {
  
  typename detail::binary_ro_left_shift<T,U>::type t(first.get() << second);
  
  first.set(static_cast<T>(t));
  return(first);
  }
  
template <class T, class C>
typename boost::enable_if
  <
  typename detail::omf_assign_right_shift_same<T>::type,
  prop_functor<T,C,read_write_tag> &
  >::type
operator >>= (prop_functor<T,C,read_write_tag> & first,T second)
  {
  
  typename detail::binary_ro_right_shift_same<T>::type t(first.get() >> second);
  
  first.set(static_cast<T>(t));
  return(first);
  }
  
template <class T, class C, class U>
typename boost::enable_if
  <
  typename detail::omf_assign_right_shift<T,U>::type,
  prop_functor<T,C,read_write_tag> &
  >::type
operator >>= (prop_functor<T,C,read_write_tag> & first,U second)
  {
  
  typename detail::binary_ro_right_shift<T,U>::type t(first.get() >> second);
  
  first.set(static_cast<T>(t));
  return(first);
  }
  
template <class T, class C>
typename boost::enable_if
  <
  typename detail::omf_assign_and_same<T>::type,
  prop_functor<T,C,read_write_tag> &
  >::type
operator &= (prop_functor<T,C,read_write_tag> & first,T second)
  {
  
  typename detail::binary_ro_and_same<T>::type t(first.get() & second);
  
  first.set(static_cast<T>(t));
  return(first);
  }
  
template <class T, class C, class U>
typename boost::enable_if
  <
  typename detail::omf_assign_and<T,U>::type,
  prop_functor<T,C,read_write_tag> &
  >::type
operator &= (prop_functor<T,C,read_write_tag> & first,U second)
  {
  
  typename detail::binary_ro_and<T,U>::type t(first.get() & second);
  
  first.set(static_cast<T>(t));
  return(first);
  }
  
template <class T, class C>
typename boost::enable_if
  <
  typename detail::omf_assign_xor_same<T>::type,
  prop_functor<T,C,read_write_tag> &
  >::type
operator ^= (prop_functor<T,C,read_write_tag> & first,T second)
  {
  
  typename detail::binary_ro_xor_same<T>::type t(first.get() ^ second);
  
  first.set(static_cast<T>(t));
  return(first);
  }
  
template <class T, class C, class U>
typename boost::enable_if
  <
  typename detail::omf_assign_xor<T,U>::type,
  prop_functor<T,C,read_write_tag> &
  >::type
operator ^= (prop_functor<T,C,read_write_tag> & first,U second)
  {
  
  typename detail::binary_ro_xor<T,U>::type t(first.get() ^ second);
  
  first.set(static_cast<T>(t));
  return(first);
  }
  
template <class T, class C>
typename boost::enable_if
  <
  typename detail::omf_assign_or_same<T>::type,
  prop_functor<T,C,read_write_tag> &
  >::type
operator |= (prop_functor<T,C,read_write_tag> & first,T second)
  {
  
  typename detail::binary_ro_or_same<T>::type t(first.get() | second);
  
  first.set(static_cast<T>(t));
  return(first);
  }
  
template <class T, class C, class U>
typename boost::enable_if
  <
  typename detail::omf_assign_or<T,U>::type,
  prop_functor<T,C,read_write_tag> &
  >::type
operator |= (prop_functor<T,C,read_write_tag> & first,U second)
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

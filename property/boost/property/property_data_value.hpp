#if !defined(PROPERTY_DATA_VALUE_HPP)
#define PROPERTY_DATA_VALUE_HPP

#include "property_interface.hpp"
#include "property_tag.hpp"
#include "property_exception.hpp"
#include "property_operators.hpp"
#include <boost/type_traits/is_const.hpp>
#include <boost/type_traits/add_const.hpp>
#include <boost/mpl/if.hpp>
#include <boost/optional.hpp>

#if defined(BOOST_MSVC)

#pragma warning (push)
#pragma warning (disable: 4250)

#endif

namespace properties
{

  template <class T, 
            T *d, 
            template <class> class NotificationPolicy = default_policy_tag,
            class Access = typename boost::mpl::if_
                        <
                        boost::is_const<T>,
                        read_tag,
                        read_write_tag
                        >::type
           >
  class prop_data;
  
  template <class T, T *d>
  struct prop_data<T,d,default_policy_tag,read_tag> : 
    virtual i_property_read<T>
  {
  
    operator T() const 
      { 
      return(get()); 
      }
    
    T get() const 
      { 
      return(*d); 
      }
      
    private:  
    
    prop_data & operator = (const prop_data &);
    
  };

  template <class T, T *d>
  struct prop_data<T,d,default_policy_tag,write_tag> : 
    virtual i_property_write<T>
  {
  
    prop_data() 
      { 
      }
      
    explicit prop_data(T arg) 
      { 
      *d = arg; 
      }
      
    template<class U>
    explicit prop_data(U arg) 
      { 
      *d = static_cast<T>(arg); 
      }
      
    prop_data & operator = (T arg) 
      { 
      set(arg); 
      return(*this); 
      }
      
    template<class U>
    prop_data & operator = (U arg) 
      { 
      set(static_cast<T>(arg)); 
      return(*this); 
      }
      
    void set(T arg) 
      { 
      *d = arg; 
      }
    
    private:  
    
    prop_data & operator = (const prop_data &);
    
  };

  template <class T, T *d,template <class> class NotificationPolicy>
  struct prop_data<T,d,NotificationPolicy,write_tag> : 
    virtual i_property_write<T>, 
    NotificationPolicy<T>
  {
  
    protected:
    
    boost::optional<T> oldT;
    
    public:
  
    prop_data() 
      { 
      }
      
    explicit prop_data(T arg) 
      { 
      *d = arg; 
      }
      
    template<class U>
    explicit prop_data(U arg) 
      { 
      *d = static_cast<T>(arg); 
      }
      
    prop_data & operator = (T arg) 
      { 
      set(arg); 
      return(*this); 
      }
      
    template<class U>
    prop_data & operator = (U arg) 
      { 
      set(static_cast<T>(arg)); 
      return(*this); 
      }
      
    void set(T arg) 
      { 
      *d = arg; 
      property_changed(*this,oldT,arg);
      }
      
    private:  
    
    prop_data & operator = (const prop_data &);
    
  };

  template <class T, T *d>
  struct prop_data<T,d,default_policy_tag,read_write_tag> : 
    prop_data<T,d,default_policy_tag,read_tag> , 
    prop_data<T,d,default_policy_tag,write_tag>,
    i_property_read_write<T>
  {
  
    prop_data() 
      { 
      }
      
    prop_data(const prop_data & arg) :
      prop_data<T,d,default_policy_tag,read_tag>(arg),
      prop_data<T,d,default_policy_tag,write_tag>(static_cast<const prop_data<T,d,default_policy_tag,write_tag> &>(arg))
      { 
      }
      
    explicit prop_data(T arg) : 
      prop_data<T,d,default_policy_tag,write_tag>(arg) 
      { 
      }
      
    template<class U>
    explicit prop_data(U arg) : 
      prop_data<T,d,default_policy_tag,write_tag>(arg) 
      { 
      }
      
    prop_data & operator = (const prop_data & arg)
      {
      return(*this = static_cast<T>(arg));
      }
      
    prop_data & operator = (T arg) 
      { 
      (static_cast<prop_data<T,d,default_policy_tag,write_tag> & >(*this)) = arg; 
      return(*this); 
      }
    
    template<class U>
    prop_data & operator = (U arg) 
      { 
      (static_cast<prop_data<T,d,default_policy_tag,write_tag> & >(*this)) = arg; 
      return(*this); 
      }
    
  };

  template <class T, T *d,template <class> class NotificationPolicy>
  struct prop_data<T,d,NotificationPolicy,read_write_tag> : 
    prop_data<T,d,default_policy_tag,read_tag> , 
    prop_data<T,d,NotificationPolicy,write_tag>,
    i_property_read_write<T>
  {
  
    prop_data() 
      { 
      }
      
    prop_data(const prop_data & arg) :
      prop_data<T,d,default_policy_tag,read_tag>(arg),
      prop_data<T,d,NotificationPolicy,write_tag>(static_cast<const prop_data<T,d,NotificationPolicy,write_tag> &>(arg))
      { 
      }
      
    explicit prop_data(T arg) : 
      prop_data<T,d,NotificationPolicy,write_tag>(arg) 
      { 
      }
      
    template<class U>
    explicit prop_data(U arg) : 
      prop_data<T,d,NotificationPolicy,write_tag>(arg) 
      { 
      }
      
    prop_data & operator = (const prop_data & arg)
      {
      return(*this = static_cast<T>(arg));
      }
      
    prop_data & operator = (T arg) 
      { 
      prop_data<T,d,NotificationPolicy,write_tag>::oldT = *this;
      (static_cast<prop_data<T,d,NotificationPolicy,write_tag> & >(*this)) = arg; 
      return(*this); 
      }
      
    template<class U>
    prop_data & operator = (U arg) 
      { 
      prop_data<T,d,NotificationPolicy,write_tag>::oldT = *this;
      (static_cast<prop_data<T,d,NotificationPolicy,write_tag> & >(*this)) = arg; 
      return(*this); 
      }
      
  };

template <class T, T *d,template <class> class NotificationPolicy> 
typename boost::enable_if
  <
  typename detail::omf_increment<T>::type,
  prop_data<T,d,NotificationPolicy,read_write_tag> &
  >::type
operator ++ (prop_data<T,d,NotificationPolicy,read_write_tag> & value)
  {
  
  T t(value.get());
  
  ++t;
  value.set(t);
  return(value);
  }
      
template <class T, T *d,template <class> class NotificationPolicy> 
typename boost::enable_if
  <
  typename detail::omf_decrement<T>::type,
  prop_data<T,d,NotificationPolicy,read_write_tag> &
  >::type
operator -- (prop_data<T,d,NotificationPolicy,read_write_tag> & value)
  {
  
  T t(value.get());
  
  --t;
  value.set(t);
  return(value);
  }
  
template <class T, T *d,template <class> class NotificationPolicy> 
typename boost::enable_if
  <
  typename detail::omf_assign_multiply_same<T>::type,
  prop_data<T,d,NotificationPolicy,read_write_tag> &
  >::type
operator *= (prop_data<T,d,NotificationPolicy,read_write_tag> & first,T second)
  {
  
  typename detail::binary_ro_multiply_same<T>::type t(first.get() * second);
  
  first.set(static_cast<T>(t));
  return(first);
  }

template <class T, T *d,template <class> class NotificationPolicy,class U>
typename boost::enable_if
  <
  typename detail::omf_assign_multiply<T,U>::type,
  prop_data<T,d,NotificationPolicy,read_write_tag> &
  >::type
operator *= (prop_data<T,d,NotificationPolicy,read_write_tag> & first,U second)
  {
  
  typename detail::binary_ro_multiply<T,U>::type t(first.get() * second);
  
  first.set(static_cast<T>(t));
  return(first);
  }

template <class T, T *d,template <class> class NotificationPolicy> 
typename boost::enable_if
  <
  typename detail::omf_assign_divide_same<T>::type,
  prop_data<T,d,NotificationPolicy,read_write_tag> &
  >::type
operator /= (prop_data<T,d,NotificationPolicy,read_write_tag> & first,T second)
  {
  
  typename detail::binary_ro_divide_same<T>::type t(first.get() / second);
  
  first.set(static_cast<T>(t));
  return(first);
  }
  
template <class T, T *d,template <class> class NotificationPolicy,class U>
typename boost::enable_if
  <
  typename detail::omf_assign_divide<T,U>::type,
  prop_data<T,d,NotificationPolicy,read_write_tag> &
  >::type
operator /= (prop_data<T,d,NotificationPolicy,read_write_tag> & first,U second)
  {
  
  typename detail::binary_ro_divide<T,U>::type t(first.get() / second);
  
  first.set(static_cast<T>(t));
  return(first);
  }
  
template <class T, T *d,template <class> class NotificationPolicy> 
typename boost::enable_if
  <
  typename detail::omf_assign_modulo_same<T>::type,
  prop_data<T,d,NotificationPolicy,read_write_tag> &
  >::type
operator %= (prop_data<T,d,NotificationPolicy,read_write_tag> & first,T second)
  {
  
  typename detail::binary_ro_modulo_same<T>::type t(first.get() % second);
  
  first.set(static_cast<T>(t));
  return(first);
  }
  
template <class T, T *d,template <class> class NotificationPolicy,class U> 
typename boost::enable_if
  <
  typename detail::omf_assign_modulo<T,U>::type,
  prop_data<T,d,NotificationPolicy,read_write_tag> &
  >::type
operator %= (prop_data<T,d,NotificationPolicy,read_write_tag> & first,U second)
  {
  
  typename detail::binary_ro_modulo<T,U>::type t(first.get() % second);
  
  first.set(static_cast<T>(t));
  return(first);
  }
  
template <class T, T *d,template <class> class NotificationPolicy> 
typename boost::enable_if
  <
  typename detail::omf_assign_add_same<T>::type,
  prop_data<T,d,NotificationPolicy,read_write_tag> &
  >::type
operator += (prop_data<T,d,NotificationPolicy,read_write_tag> & first,T second)
  {
  
  typename detail::binary_ro_add_same<T>::type t(first.get() + second);
  
  first.set(static_cast<T>(t));
  return(first);
  }

template <class T, T *d,template <class> class NotificationPolicy,class U> 
typename boost::enable_if
  <
  typename detail::omf_assign_add<T,U>::type,
  prop_data<T,d,NotificationPolicy,read_write_tag> &
  >::type
operator += (prop_data<T,d,NotificationPolicy,read_write_tag> & first,U second)
  {
  
  typename detail::binary_ro_add<T,U>::type t(first.get() + second);
  
  first.set(static_cast<T>(t));
  return(first);
  }

template <class T, T *d,template <class> class NotificationPolicy> 
typename boost::enable_if
  <
  typename detail::omf_assign_subtract_same<T>::type,
  prop_data<T,d,NotificationPolicy,read_write_tag> &
  >::type
operator -= (prop_data<T,d,NotificationPolicy,read_write_tag> & first,T second)
  {
  
  typename detail::binary_ro_subtract_same<T>::type t(first.get() - second);
  
  first.set(static_cast<T>(t));
  return(first);
  }

template <class T, T *d,template <class> class NotificationPolicy,class U> 
typename boost::enable_if
  <
  typename detail::omf_assign_subtract<T,U>::type,
  prop_data<T,d,NotificationPolicy,read_write_tag> &
  >::type
operator -= (prop_data<T,d,NotificationPolicy,read_write_tag> & first,U second)
  {
  
  typename detail::binary_ro_subtract<T,U>::type t(first.get() - second);
  
  first.set(static_cast<T>(t));
  return(first);
  }

template <class T, T *d,template <class> class NotificationPolicy> 
typename boost::enable_if
  <
  typename detail::omf_assign_left_shift_same<T>::type,
  prop_data<T,d,NotificationPolicy,read_write_tag> &
  >::type
operator <<= (prop_data<T,d,NotificationPolicy,read_write_tag> & first,T second)
  {
  
  typename detail::binary_ro_left_shift_same<T>::type t(first.get() << second);
  
  first.set(static_cast<T>(t));
  return(first);
  }
  
template <class T, T *d,template <class> class NotificationPolicy,class U> 
typename boost::enable_if
  <
  typename detail::omf_assign_left_shift<T,U>::type,
  prop_data<T,d,NotificationPolicy,read_write_tag> &
  >::type
operator <<= (prop_data<T,d,NotificationPolicy,read_write_tag> & first,U second)
  {
  
  typename detail::binary_ro_left_shift<T,U>::type t(first.get() << second);
  
  first.set(static_cast<T>(t));
  return(first);
  }
  
template <class T, T *d,template <class> class NotificationPolicy> 
typename boost::enable_if
  <
  typename detail::omf_assign_right_shift_same<T>::type,
  prop_data<T,d,NotificationPolicy,read_write_tag> &
  >::type
operator >>= (prop_data<T,d,NotificationPolicy,read_write_tag> & first,T second)
  {
  
  typename detail::binary_ro_right_shift_same<T>::type t(first.get() >> second);
  
  first.set(static_cast<T>(t));
  return(first);
  }
  
template <class T, T *d,template <class> class NotificationPolicy,class U>
typename boost::enable_if
  <
  typename detail::omf_assign_right_shift<T,U>::type,
  prop_data<T,d,NotificationPolicy,read_write_tag> &
  >::type
operator >>= (prop_data<T,d,NotificationPolicy,read_write_tag> & first,U second)
  {
  
  typename detail::binary_ro_right_shift<T,U>::type t(first.get() >> second);
  
  first.set(static_cast<T>(t));
  return(first);
  }
  
template <class T, T *d,template <class> class NotificationPolicy> 
typename boost::enable_if
  <
  typename detail::omf_assign_and_same<T>::type,
  prop_data<T,d,NotificationPolicy,read_write_tag> &
  >::type
operator &= (prop_data<T,d,NotificationPolicy,read_write_tag> & first,T second)
  {
  
  typename detail::binary_ro_and_same<T>::type t(first.get() & second);
  
  first.set(static_cast<T>(t));
  return(first);
  }
  
template <class T, T *d,template <class> class NotificationPolicy,class U>
typename boost::enable_if
  <
  typename detail::omf_assign_and<T,U>::type,
  prop_data<T,d,NotificationPolicy,read_write_tag> &
  >::type
operator &= (prop_data<T,d,NotificationPolicy,read_write_tag> & first,U second)
  {
  
  typename detail::binary_ro_and<T,U>::type t(first.get() & second);
  
  first.set(static_cast<T>(t));
  return(first);
  }
  
template <class T, T *d,template <class> class NotificationPolicy> 
typename boost::enable_if
  <
  typename detail::omf_assign_xor_same<T>::type,
  prop_data<T,d,NotificationPolicy,read_write_tag> &
  >::type
operator ^= (prop_data<T,d,NotificationPolicy,read_write_tag> & first,T second)
  {
  
  typename detail::binary_ro_xor_same<T>::type t(first.get() ^ second);
  
  first.set(static_cast<T>(t));
  return(first);
  }
  
template <class T, T *d,template <class> class NotificationPolicy,class U> 
typename boost::enable_if
  <
  typename detail::omf_assign_xor<T,U>::type,
  prop_data<T,d,NotificationPolicy,read_write_tag> &
  >::type
operator ^= (prop_data<T,d,NotificationPolicy,read_write_tag> & first,U second)
  {
  
  typename detail::binary_ro_xor<T,U>::type t(first.get() ^ second);
  
  first.set(static_cast<T>(t));
  return(first);
  }
  
template <class T, T *d,template <class> class NotificationPolicy> 
typename boost::enable_if
  <
  typename detail::omf_assign_or_same<T>::type,
  prop_data<T,d,NotificationPolicy,read_write_tag> &
  >::type
operator |= (prop_data<T,d,NotificationPolicy,read_write_tag> & first,T second)
  {
  
  typename detail::binary_ro_or_same<T>::type t(first.get() | second);
  
  first.set(static_cast<T>(t));
  return(first);
  }
  
template <class T, T *d,template <class> class NotificationPolicy,class U> 
typename boost::enable_if
  <
  typename detail::omf_assign_or<T,U>::type,
  prop_data<T,d,NotificationPolicy,read_write_tag> &
  >::type
operator |= (prop_data<T,d,NotificationPolicy,read_write_tag> & first,U second)
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

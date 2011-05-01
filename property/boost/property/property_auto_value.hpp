#if !defined(PROPERTY_AUTO_VALUE_HPP)
#define PROPERTY_AUTO_VALUE_HPP

#include "PropertyInterface.h"
#include "PropertyTag.h"
#include "PropertyOperators.h"
#include <boost/utility/value_init.hpp>
#include <boost/type_traits/is_const.hpp>
#include <boost/mpl/if.hpp>
#include <boost/optional.hpp>

#if defined(BOOST_MSVC)

#pragma warning (push)
#pragma warning (disable: 4250)

#endif

namespace properties
{

  template <class T, 
            template <class> class NotificationPolicy = default_policy_tag,
            class Access = typename boost::mpl::if_
              <
              boost::is_const<T>,
              read_tag,
              read_write_tag
              >::type
           >
  class prop_auto;
  
  template <class T>
  class prop_auto<T,default_policy_tag,read_tag> : 
    public virtual i_property_read<T>
  {
  
    public:

    prop_auto() 
      { 
      }
      
    explicit prop_auto(T arg) : 
      data(arg) 
      { 
      }
      
    template<class U>
    explicit prop_auto(U arg) :
      data(static_cast<T>(arg))
      {
      }
      
    operator T() const 
      { 
      return(get()); 
      }

    T get() const 
      { 
      return(boost::get(data)); 
      }
    
    protected:

    boost::initialized<T> data;
    
    private:
    
    prop_auto & operator = (const prop_auto & arg);
      
  };
  
  template <class T>
  struct prop_auto<T,default_policy_tag,read_write_tag> : 
    prop_auto<T,default_policy_tag,read_tag>, 
    i_property_read_write<T>
  {

    prop_auto() 
      { 
      }
      
    prop_auto(const prop_auto & arg) : 
      prop_auto<T,default_policy_tag,read_tag>(static_cast<const prop_auto<T,default_policy_tag,read_tag> &>(arg))
      { 
      }
      
    explicit prop_auto(T arg) : 
      prop_auto<T,default_policy_tag,read_tag>(arg)
      { 
      }
      
    template<class U>
    explicit prop_auto(U arg) : 
      prop_auto<T,default_policy_tag,read_tag>(arg)
      {
      }
      
    prop_auto & operator = (const prop_auto & arg) 
      {
      return(*this = static_cast<T>(arg));
      }
    
    prop_auto & operator = (T arg) 
      { 
      set(arg); 
      return(*this); 
      }
      
    template<class U>
    prop_auto & operator = (U arg) 
      { 
      set(static_cast<T>(arg)); 
      return(*this); 
      }
      
    void set(T arg) 
      { 
      boost::get(data) = arg; 
      }
    
  };
  
  template <class T,template <class> class NotificationPolicy>
  struct prop_auto<T,NotificationPolicy,read_write_tag> : 
    prop_auto<T,default_policy_tag,read_tag>, 
    i_property_read_write<T>, 
    NotificationPolicy<T>
  {

    prop_auto() 
      { 
      }
      
    prop_auto(const prop_auto & arg) : 
      prop_auto<T,default_policy_tag,read_tag>(static_cast<const prop_auto<T,default_policy_tag,read_tag> &>(arg))
      { 
      }
      
    explicit prop_auto(T arg) : 
      prop_auto<T,default_policy_tag,read_tag>(arg)
      { 
      }
      
    template<class U>
    explicit prop_auto(U arg) : 
      prop_auto<T,default_policy_tag,read_tag>(arg)
      {
      }
      
    prop_auto & operator = (const prop_auto & arg) 
      {
      return(*this = static_cast<T>(arg));
      }
    
    prop_auto & operator = (T arg) 
      { 
      set(arg); 
      return(*this); 
      }
      
    template<class U>
    prop_auto & operator = (U arg) 
      { 
      set(static_cast<T>(arg)); 
      return(*this); 
      }
    
    void set(T arg) 
      { 
      
      T t(*this);
      
      boost::get(data) = arg;
      property_changed(*this,boost::optional<T>(t),arg);
      }
    
  };
  
template <class T,template <class> class NotificationPolicy> 
typename boost::enable_if
  <
  typename detail::omf_increment<T>::type,
  prop_auto<T,NotificationPolicy,read_write_tag> &
  >::type
operator ++ (prop_auto<T,NotificationPolicy,read_write_tag> & value)
  {
  
  T t(value.get());
  
  ++t;
  value.set(t);
  return(value);
  }
      
template <class T,template <class> class NotificationPolicy> 
typename boost::enable_if
  <
  typename detail::omf_decrement<T>::type,
  prop_auto<T,NotificationPolicy,read_write_tag> &
  >::type
operator -- (prop_auto<T,NotificationPolicy,read_write_tag> & value)
  {
  
  T t(value.get());
  
  --t;
  value.set(t);
  return(value);
  }
  
template <class T,template <class> class NotificationPolicy> 
typename boost::enable_if
  <
  typename detail::omf_assign_multiply_same<T>::type,
  prop_auto<T,NotificationPolicy,read_write_tag> &
  >::type
operator *= (prop_auto<T,NotificationPolicy,read_write_tag> & first,T second)
  {
  
  typename detail::binary_ro_multiply_same<T>::type t(first.get() * second);
  
  first.set(static_cast<T>(t));
  return(first);
  }

template <class T,template <class> class NotificationPolicy,class U>
typename boost::enable_if
  <
  typename detail::omf_assign_multiply<T,U>::type,
  prop_auto<T,NotificationPolicy,read_write_tag> &
  >::type
operator *= (prop_auto<T,NotificationPolicy,read_write_tag> & first,U second)
  {
  
  typename detail::binary_ro_multiply<T,U>::type t(first.get() * second);
  
  first.set(static_cast<T>(t));
  return(first);
  }

template <class T,template <class> class NotificationPolicy> 
typename boost::enable_if
  <
  typename detail::omf_assign_divide_same<T>::type,
  prop_auto<T,NotificationPolicy,read_write_tag> &
  >::type
operator /= (prop_auto<T,NotificationPolicy,read_write_tag> & first,T second)
  {
  
  typename detail::binary_ro_divide_same<T>::type t(first.get() / second);
  
  first.set(static_cast<T>(t));
  return(first);
  }
  
template <class T,template <class> class NotificationPolicy,class U>
typename boost::enable_if
  <
  typename detail::omf_assign_divide<T,U>::type,
  prop_auto<T,NotificationPolicy,read_write_tag> &
  >::type
operator /= (prop_auto<T,NotificationPolicy,read_write_tag> & first,U second)
  {
  
  typename detail::binary_ro_divide<T,U>::type t(first.get() / second);
  
  first.set(static_cast<T>(t));
  return(first);
  }
  
template <class T,template <class> class NotificationPolicy> 
typename boost::enable_if
  <
  typename detail::omf_assign_modulo_same<T>::type,
  prop_auto<T,NotificationPolicy,read_write_tag> &
  >::type
operator %= (prop_auto<T,NotificationPolicy,read_write_tag> & first,T second)
  {
  
  typename detail::binary_ro_modulo_same<T>::type t(first.get() % second);
  
  first.set(static_cast<T>(t));
  return(first);
  }
  
template <class T,template <class> class NotificationPolicy,class U> 
typename boost::enable_if
  <
  typename detail::omf_assign_modulo<T,U>::type,
  prop_auto<T,NotificationPolicy,read_write_tag> &
  >::type
operator %= (prop_auto<T,NotificationPolicy,read_write_tag> & first,U second)
  {
  
  typename detail::binary_ro_modulo<T,U>::type t(first.get() % second);
  
  first.set(static_cast<T>(t));
  return(first);
  }
  
template <class T,template <class> class NotificationPolicy> 
typename boost::enable_if
  <
  typename detail::omf_assign_add_same<T>::type,
  prop_auto<T,NotificationPolicy,read_write_tag> &
  >::type
operator += (prop_auto<T,NotificationPolicy,read_write_tag> & first,T second)
  {
  
  typename detail::binary_ro_add_same<T>::type t(first.get() + second);
  
  first.set(static_cast<T>(t));
  return(first);
  }

template <class T,template <class> class NotificationPolicy,class U> 
typename boost::enable_if
  <
  typename detail::omf_assign_add<T,U>::type,
  prop_auto<T,NotificationPolicy,read_write_tag> &
  >::type
operator += (prop_auto<T,NotificationPolicy,read_write_tag> & first,U second)
  {
  
  typename detail::binary_ro_add<T,U>::type t(first.get() + second);
  
  first.set(static_cast<T>(t));
  return(first);
  }

template <class T,template <class> class NotificationPolicy> 
typename boost::enable_if
  <
  typename detail::omf_assign_subtract_same<T>::type,
  prop_auto<T,NotificationPolicy,read_write_tag> &
  >::type
operator -= (prop_auto<T,NotificationPolicy,read_write_tag> & first,T second)
  {
  
  typename detail::binary_ro_subtract_same<T>::type t(first.get() - second);
  
  first.set(static_cast<T>(t));
  return(first);
  }

template <class T,template <class> class NotificationPolicy,class U> 
typename boost::enable_if
  <
  typename detail::omf_assign_subtract<T,U>::type,
  prop_auto<T,NotificationPolicy,read_write_tag> &
  >::type
operator -= (prop_auto<T,NotificationPolicy,read_write_tag> & first,U second)
  {
  
  typename detail::binary_ro_subtract<T,U>::type t(first.get() - second);
  
  first.set(static_cast<T>(t));
  return(first);
  }

template <class T,template <class> class NotificationPolicy> 
typename boost::enable_if
  <
  typename detail::omf_assign_left_shift_same<T>::type,
  prop_auto<T,NotificationPolicy,read_write_tag> &
  >::type
operator <<= (prop_auto<T,NotificationPolicy,read_write_tag> & first,T second)
  {
  
  typename detail::binary_ro_left_shift_same<T>::type t(first.get() << second);
  
  first.set(static_cast<T>(t));
  return(first);
  }
  
template <class T,template <class> class NotificationPolicy,class U> 
typename boost::enable_if
  <
  typename detail::omf_assign_left_shift<T,U>::type,
  prop_auto<T,NotificationPolicy,read_write_tag> &
  >::type
operator <<= (prop_auto<T,NotificationPolicy,read_write_tag> & first,U second)
  {
  
  typename detail::binary_ro_left_shift<T,U>::type t(first.get() << second);
  
  first.set(static_cast<T>(t));
  return(first);
  }
  
template <class T,template <class> class NotificationPolicy> 
typename boost::enable_if
  <
  typename detail::omf_assign_right_shift_same<T>::type,
  prop_auto<T,NotificationPolicy,read_write_tag> &
  >::type
operator >>= (prop_auto<T,NotificationPolicy,read_write_tag> & first,T second)
  {
  
  typename detail::binary_ro_right_shift_same<T>::type t(first.get() >> second);
  
  first.set(static_cast<T>(t));
  return(first);
  }
  
template <class T,template <class> class NotificationPolicy,class U>
typename boost::enable_if
  <
  typename detail::omf_assign_right_shift<T,U>::type,
  prop_auto<T,NotificationPolicy,read_write_tag> &
  >::type
operator >>= (prop_auto<T,NotificationPolicy,read_write_tag> & first,U second)
  {
  
  typename detail::binary_ro_right_shift<T,U>::type t(first.get() >> second);
  
  first.set(static_cast<T>(t));
  return(first);
  }
  
template <class T,template <class> class NotificationPolicy> 
typename boost::enable_if
  <
  typename detail::omf_assign_and_same<T>::type,
  prop_auto<T,NotificationPolicy,read_write_tag> &
  >::type
operator &= (prop_auto<T,NotificationPolicy,read_write_tag> & first,T second)
  {
  
  typename detail::binary_ro_and_same<T>::type t(first.get() & second);
  
  first.set(static_cast<T>(t));
  return(first);
  }
  
template <class T,template <class> class NotificationPolicy,class U>
typename boost::enable_if
  <
  typename detail::omf_assign_and<T,U>::type,
  prop_auto<T,NotificationPolicy,read_write_tag> &
  >::type
operator &= (prop_auto<T,NotificationPolicy,read_write_tag> & first,U second)
  {
  
  typename detail::binary_ro_and<T,U>::type t(first.get() & second);
  
  first.set(static_cast<T>(t));
  return(first);
  }
  
template <class T,template <class> class NotificationPolicy> 
typename boost::enable_if
  <
  typename detail::omf_assign_xor_same<T>::type,
  prop_auto<T,NotificationPolicy,read_write_tag> &
  >::type
operator ^= (prop_auto<T,NotificationPolicy,read_write_tag> & first,T second)
  {
  
  typename detail::binary_ro_xor_same<T>::type t(first.get() ^ second);
  
  first.set(static_cast<T>(t));
  return(first);
  }
  
template <class T,template <class> class NotificationPolicy,class U> 
typename boost::enable_if
  <
  typename detail::omf_assign_xor<T,U>::type,
  prop_auto<T,NotificationPolicy,read_write_tag> &
  >::type
operator ^= (prop_auto<T,NotificationPolicy,read_write_tag> & first,U second)
  {
  
  typename detail::binary_ro_xor<T,U>::type t(first.get() ^ second);
  
  first.set(static_cast<T>(t));
  return(first);
  }
  
template <class T,template <class> class NotificationPolicy> 
typename boost::enable_if
  <
  typename detail::omf_assign_or_same<T>::type,
  prop_auto<T,NotificationPolicy,read_write_tag> &
  >::type
operator |= (prop_auto<T,NotificationPolicy,read_write_tag> & first,T second)
  {
  
  typename detail::binary_ro_or_same<T>::type t(first.get() | second);
  
  first.set(static_cast<T>(t));
  return(first);
  }
  
template <class T,template <class> class NotificationPolicy,class U> 
typename boost::enable_if
  <
  typename detail::omf_assign_or<T,U>::type,
  prop_auto<T,NotificationPolicy,read_write_tag> &
  >::type
operator |= (prop_auto<T,NotificationPolicy,read_write_tag> & first,U second)
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

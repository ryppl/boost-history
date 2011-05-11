#if !defined(PROPERTY_AUTO_PREDICATE_HPP)
#define PROPERTY_AUTO_PREDICATE_HPP

#include "property_tag.hpp"
#include "property_auto_value.hpp"
#include "property_operators.hpp"
#include <boost/function.hpp>
#include <boost/mpl/assert.hpp>
#include <boost/type_traits/is_const.hpp>

#if defined(BOOST_MSVC)

#pragma warning (push)
#pragma warning (disable: 4250)

#endif

namespace properties
{
  
  template <class T, 
            template <class> class NotificationPolicy = default_policy_tag,
            template <class> class PredicateFailurePolicy = default_policy_tag
           >
  struct prop_auto_predicate: 
    prop_auto<T,NotificationPolicy>,
    PredicateFailurePolicy<T>
  {

    BOOST_MPL_ASSERT_NOT((boost::is_const<T>));
    
    explicit prop_auto_predicate(boost::function<bool (T)> f) : 
      fs(f) 
      { 
      }
      
    prop_auto_predicate(const prop_auto_predicate & arg) : 
      prop_auto<T,NotificationPolicy>(static_cast<const prop_auto<T,NotificationPolicy> &>(arg)),
      fs(arg.fs)
      { 
      }
      
    prop_auto_predicate(boost::function<bool (T)> f,T arg) : 
      prop_auto<T,NotificationPolicy>(init(f,arg)),
      fs(f)
      { 
      }
      
    template<class U>
    prop_auto_predicate(boost::function<bool (T)> f,U arg) : 
      prop_auto<T,NotificationPolicy>(init(f,arg)),
      fs(f)
      { 
      }
      
    prop_auto_predicate & operator = (const prop_auto_predicate & arg) 
      {
      static_cast<prop_auto<T,NotificationPolicy> &>(*this) = static_cast<const prop_auto<T,NotificationPolicy> &>(arg); 
      return(*this); 
      }
      
    prop_auto_predicate & operator = (T arg) 
      { 
      static_cast<prop_auto<T,NotificationPolicy> &>(*this) = arg; 
      return(*this); 
      }
      
    template<class U>
    prop_auto_predicate & operator = (U arg) 
      { 
      static_cast<prop_auto<T,NotificationPolicy> &>(*this) = arg; 
      return(*this); 
      }
      
    void set(T arg) 
      { 
      if (!fs || fs(arg))
        {
        prop_auto<T,NotificationPolicy>::set(arg);
        }
      else
        {
      
        T t(*this);
      
        PredicateFailure(*this,fs,boost::optional<T>(t),arg);
        }
      }
    
    private:
    
    boost::function<bool (T)> fs;
    
    bool valid(boost::function<bool (T)> f,T arg) const
      {
      return(!f || f(arg));
      }
    
    template<class U>
    bool valid(boost::function<bool (T)> f,U arg) const
      {
      return(!f || f(static_cast<T>(arg)));
      }
      
    T init(boost::function<bool (T)> f,T arg) const
      {
      if (valid(f,arg))
        {
        return arg;
        }
      else  
        {
        
        boost::initialized<T> d;
    
        return(boost::get(d)); 
        }
      }
    
    template<class U>
    U init(boost::function<bool (T)> f,U arg) const
      {
      if (valid(f,arg))
        {
        return arg;
        }
      else  
        {
        
        boost::initialized<U> d;
    
        return(boost::get(d));
        }
      }
      
  };
  
  template <class T, 
            template <class> class NotificationPolicy
           >
  struct prop_auto_predicate<T,NotificationPolicy,default_policy_tag> : 
    prop_auto<T,NotificationPolicy>
  {

    BOOST_MPL_ASSERT_NOT((boost::is_const<T>));
    
    explicit prop_auto_predicate(boost::function<bool (T)> f) : 
      fs(f) 
      { 
      }
      
    prop_auto_predicate(const prop_auto_predicate & arg) : 
      prop_auto<T,NotificationPolicy>(static_cast<const prop_auto<T,NotificationPolicy> &>(arg)),
      fs(arg.fs)
      { 
      }
      
    prop_auto_predicate(boost::function<bool (T)> f,T arg) : 
      prop_auto<T,NotificationPolicy>(init(f,arg)),
      fs(f)
      { 
      }
      
    template<class U>
    prop_auto_predicate(boost::function<bool (T)> f,U arg) : 
      prop_auto<T,NotificationPolicy>(init(f,arg)),
      fs(f)
      { 
      }
      
    prop_auto_predicate & operator = (const prop_auto_predicate & arg) 
      {
      static_cast<prop_auto<T,NotificationPolicy> &>(*this) = static_cast<const prop_auto<T,NotificationPolicy> &>(arg); 
      return(*this); 
      }
      
    prop_auto_predicate & operator = (T arg) 
      { 
      static_cast<prop_auto<T,NotificationPolicy> &>(*this) = arg; 
      return(*this); 
      }
      
    template<class U>
    prop_auto_predicate & operator = (U arg) 
      { 
      static_cast<prop_auto<T,NotificationPolicy> &>(*this) = arg; 
      return(*this); 
      }
      
    void set(T arg) 
      { 
      if (!fs || fs(arg))
        {
        prop_auto<T,NotificationPolicy>::set(arg);
        }
      }
    
    private:
    
    boost::function<bool (T)> fs;
      
    bool valid(boost::function<bool (T)> f,T arg) const
      {
      return(!f || f(arg));
      }
    
    template<class U>
    bool valid(boost::function<bool (T)> f,U arg) const
      {
      return(!f || f(static_cast<T>(arg)));
      }
      
    T init(boost::function<bool (T)> f,T arg) const
      {
      if (valid(f,arg))
        {
        return arg;
        }
      else  
        {
        
        boost::initialized<T> d;
    
        return(boost::get(d)); 
        }
      }
    
    template<class U>
    U init(boost::function<bool (T)> f,U arg) const
      {
      if (valid(f,arg))
        {
        return arg;
        }
      else  
        {
        
        boost::initialized<U> d;
    
        return(boost::get(d));
        }
      }
      
  };
  
template <class T,template <class> class NotificationPolicy,template <class> class PredicateFailurePolicy> 
typename boost::enable_if
  <
  typename detail::omf_increment<T>::type,
  prop_auto_predicate<T,NotificationPolicy,PredicateFailurePolicy> &
  >::type
operator ++ (prop_auto_predicate<T,NotificationPolicy,PredicateFailurePolicy> & value)
  {
  
  T t(value.get());
  
  ++t;
  value.set(t);
  return(value);
  }
      
template <class T,template <class> class NotificationPolicy,template <class> class PredicateFailurePolicy> 
typename boost::enable_if
  <
  typename detail::omf_decrement<T>::type,
  prop_auto_predicate<T,NotificationPolicy,PredicateFailurePolicy> &
  >::type
operator -- (prop_auto_predicate<T,NotificationPolicy,PredicateFailurePolicy> & value)
  {
  
  T t(value.get());
  
  --t;
  value.set(t);
  return(value);
  }
  
template <class T,template <class> class NotificationPolicy,template <class> class PredicateFailurePolicy> 
typename boost::enable_if
  <
  typename detail::omf_assign_multiply_same<T>::type,
  prop_auto_predicate<T,NotificationPolicy,PredicateFailurePolicy> &
  >::type
operator *= (prop_auto_predicate<T,NotificationPolicy,PredicateFailurePolicy> & first,T second)
  {
  
  typename detail::binary_ro_multiply_same<T>::type t(first.get() * second);
  
  first.set(static_cast<T>(t));
  return(first);
  }

template <class T,template <class> class NotificationPolicy,template <class> class PredicateFailurePolicy,class U>
typename boost::enable_if
  <
  typename detail::omf_assign_multiply<T,U>::type,
  prop_auto_predicate<T,NotificationPolicy,PredicateFailurePolicy> &
  >::type
operator *= (prop_auto_predicate<T,NotificationPolicy,PredicateFailurePolicy> & first,U second)
  {
  
  typename detail::binary_ro_multiply<T,U>::type t(first.get() * second);
  
  first.set(static_cast<T>(t));
  return(first);
  }

template <class T,template <class> class NotificationPolicy,template <class> class PredicateFailurePolicy> 
typename boost::enable_if
  <
  typename detail::omf_assign_divide_same<T>::type,
  prop_auto_predicate<T,NotificationPolicy,PredicateFailurePolicy> &
  >::type
operator /= (prop_auto_predicate<T,NotificationPolicy,PredicateFailurePolicy> & first,T second)
  {
  
  typename detail::binary_ro_divide_same<T>::type t(first.get() / second);
  
  first.set(static_cast<T>(t));
  return(first);
  }
  
template <class T,template <class> class NotificationPolicy,template <class> class PredicateFailurePolicy,class U>
typename boost::enable_if
  <
  typename detail::omf_assign_divide<T,U>::type,
  prop_auto_predicate<T,NotificationPolicy,PredicateFailurePolicy> &
  >::type
operator /= (prop_auto_predicate<T,NotificationPolicy,PredicateFailurePolicy> & first,U second)
  {
  
  typename detail::binary_ro_divide<T,U>::type t(first.get() / second);
  
  first.set(static_cast<T>(t));
  return(first);
  }
  
template <class T,template <class> class NotificationPolicy,template <class> class PredicateFailurePolicy> 
typename boost::enable_if
  <
  typename detail::omf_assign_modulo_same<T>::type,
  prop_auto_predicate<T,NotificationPolicy,PredicateFailurePolicy> &
  >::type
operator %= (prop_auto_predicate<T,NotificationPolicy,PredicateFailurePolicy> & first,T second)
  {
  
  typename detail::binary_ro_modulo_same<T>::type t(first.get() % second);
  
  first.set(static_cast<T>(t));
  return(first);
  }
  
template <class T,template <class> class NotificationPolicy,template <class> class PredicateFailurePolicy,class U> 
typename boost::enable_if
  <
  typename detail::omf_assign_modulo<T,U>::type,
  prop_auto_predicate<T,NotificationPolicy,PredicateFailurePolicy> &
  >::type
operator %= (prop_auto_predicate<T,NotificationPolicy,PredicateFailurePolicy> & first,U second)
  {
  
  typename detail::binary_ro_modulo<T,U>::type t(first.get() % second);
  
  first.set(static_cast<T>(t));
  return(first);
  }
  
template <class T,template <class> class NotificationPolicy,template <class> class PredicateFailurePolicy> 
typename boost::enable_if
  <
  typename detail::omf_assign_add_same<T>::type,
  prop_auto_predicate<T,NotificationPolicy,PredicateFailurePolicy> &
  >::type
operator += (prop_auto_predicate<T,NotificationPolicy,PredicateFailurePolicy> & first,T second)
  {
  
  typename detail::binary_ro_add_same<T>::type t(first.get() + second);
  
  first.set(static_cast<T>(t));
  return(first);
  }

template <class T,template <class> class NotificationPolicy,template <class> class PredicateFailurePolicy,class U> 
typename boost::enable_if
  <
  typename detail::omf_assign_add<T,U>::type,
  prop_auto_predicate<T,NotificationPolicy,PredicateFailurePolicy> &
  >::type
operator += (prop_auto_predicate<T,NotificationPolicy,PredicateFailurePolicy> & first,U second)
  {
  
  typename detail::binary_ro_add<T,U>::type t(first.get() + second);
  
  first.set(static_cast<T>(t));
  return(first);
  }

template <class T,template <class> class NotificationPolicy,template <class> class PredicateFailurePolicy> 
typename boost::enable_if
  <
  typename detail::omf_assign_subtract_same<T>::type,
  prop_auto_predicate<T,NotificationPolicy,PredicateFailurePolicy> &
  >::type
operator -= (prop_auto_predicate<T,NotificationPolicy,PredicateFailurePolicy> & first,T second)
  {
  
  typename detail::binary_ro_subtract_same<T>::type t(first.get() - second);
  
  first.set(static_cast<T>(t));
  return(first);
  }

template <class T,template <class> class NotificationPolicy,template <class> class PredicateFailurePolicy,class U> 
typename boost::enable_if
  <
  typename detail::omf_assign_subtract<T,U>::type,
  prop_auto_predicate<T,NotificationPolicy,PredicateFailurePolicy> &
  >::type
operator -= (prop_auto_predicate<T,NotificationPolicy,PredicateFailurePolicy> & first,U second)
  {
  
  typename detail::binary_ro_subtract<T,U>::type t(first.get() - second);
  
  first.set(static_cast<T>(t));
  return(first);
  }

template <class T,template <class> class NotificationPolicy,template <class> class PredicateFailurePolicy> 
typename boost::enable_if
  <
  typename detail::omf_assign_left_shift_same<T>::type,
  prop_auto_predicate<T,NotificationPolicy,PredicateFailurePolicy> &
  >::type
operator <<= (prop_auto_predicate<T,NotificationPolicy,PredicateFailurePolicy> & first,T second)
  {
  
  typename detail::binary_ro_left_shift_same<T>::type t(first.get() << second);
  
  first.set(static_cast<T>(t));
  return(first);
  }
  
template <class T,template <class> class NotificationPolicy,template <class> class PredicateFailurePolicy,class U> 
typename boost::enable_if
  <
  typename detail::omf_assign_left_shift<T,U>::type,
  prop_auto_predicate<T,NotificationPolicy,PredicateFailurePolicy> &
  >::type
operator <<= (prop_auto_predicate<T,NotificationPolicy,PredicateFailurePolicy> & first,U second)
  {
  
  typename detail::binary_ro_left_shift<T,U>::type t(first.get() << second);
  
  first.set(static_cast<T>(t));
  return(first);
  }
  
template <class T,template <class> class NotificationPolicy,template <class> class PredicateFailurePolicy> 
typename boost::enable_if
  <
  typename detail::omf_assign_right_shift_same<T>::type,
  prop_auto_predicate<T,NotificationPolicy,PredicateFailurePolicy> &
  >::type
operator >>= (prop_auto_predicate<T,NotificationPolicy,PredicateFailurePolicy> & first,T second)
  {
  
  typename detail::binary_ro_right_shift_same<T>::type t(first.get() >> second);
  
  first.set(static_cast<T>(t));
  return(first);
  }
  
template <class T,template <class> class NotificationPolicy,template <class> class PredicateFailurePolicy,class U>
typename boost::enable_if
  <
  typename detail::omf_assign_right_shift<T,U>::type,
  prop_auto_predicate<T,NotificationPolicy,PredicateFailurePolicy> &
  >::type
operator >>= (prop_auto_predicate<T,NotificationPolicy,PredicateFailurePolicy> & first,U second)
  {
  
  typename detail::binary_ro_right_shift<T,U>::type t(first.get() >> second);
  
  first.set(static_cast<T>(t));
  return(first);
  }
  
template <class T,template <class> class NotificationPolicy,template <class> class PredicateFailurePolicy> 
typename boost::enable_if
  <
  typename detail::omf_assign_and_same<T>::type,
  prop_auto_predicate<T,NotificationPolicy,PredicateFailurePolicy> &
  >::type
operator &= (prop_auto_predicate<T,NotificationPolicy,PredicateFailurePolicy> & first,T second)
  {
  
  typename detail::binary_ro_and_same<T>::type t(first.get() & second);
  
  first.set(static_cast<T>(t));
  return(first);
  }
  
template <class T,template <class> class NotificationPolicy,template <class> class PredicateFailurePolicy,class U>
typename boost::enable_if
  <
  typename detail::omf_assign_and<T,U>::type,
  prop_auto_predicate<T,NotificationPolicy,PredicateFailurePolicy> &
  >::type
operator &= (prop_auto_predicate<T,NotificationPolicy,PredicateFailurePolicy> & first,U second)
  {
  
  typename detail::binary_ro_and<T,U>::type t(first.get() & second);
  
  first.set(static_cast<T>(t));
  return(first);
  }
  
template <class T,template <class> class NotificationPolicy,template <class> class PredicateFailurePolicy> 
typename boost::enable_if
  <
  typename detail::omf_assign_xor_same<T>::type,
  prop_auto_predicate<T,NotificationPolicy,PredicateFailurePolicy> &
  >::type
operator ^= (prop_auto_predicate<T,NotificationPolicy,PredicateFailurePolicy> & first,T second)
  {
  
  typename detail::binary_ro_xor_same<T>::type t(first.get() ^ second);
  
  first.set(static_cast<T>(t));
  return(first);
  }
  
template <class T,template <class> class NotificationPolicy,template <class> class PredicateFailurePolicy,class U> 
typename boost::enable_if
  <
  typename detail::omf_assign_xor<T,U>::type,
  prop_auto_predicate<T,NotificationPolicy,PredicateFailurePolicy> &
  >::type
operator ^= (prop_auto_predicate<T,NotificationPolicy,PredicateFailurePolicy> & first,U second)
  {
  
  typename detail::binary_ro_xor<T,U>::type t(first.get() ^ second);
  
  first.set(static_cast<T>(t));
  return(first);
  }
  
template <class T,template <class> class NotificationPolicy,template <class> class PredicateFailurePolicy> 
typename boost::enable_if
  <
  typename detail::omf_assign_or_same<T>::type,
  prop_auto_predicate<T,NotificationPolicy,PredicateFailurePolicy> &
  >::type
operator |= (prop_auto_predicate<T,NotificationPolicy,PredicateFailurePolicy> & first,T second)
  {
  
  typename detail::binary_ro_or_same<T>::type t(first.get() | second);
  
  first.set(static_cast<T>(t));
  return(first);
  }
  
template <class T,template <class> class NotificationPolicy,template <class> class PredicateFailurePolicy,class U> 
typename boost::enable_if
  <
  typename detail::omf_assign_or<T,U>::type,
  prop_auto_predicate<T,NotificationPolicy,PredicateFailurePolicy> &
  >::type
operator |= (prop_auto_predicate<T,NotificationPolicy,PredicateFailurePolicy> & first,U second)
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

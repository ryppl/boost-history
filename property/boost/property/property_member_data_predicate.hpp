#if !defined(PROPERTY_MEMBER_DATA_PREDICATE_HPP)
#define PROPERTY_MEMBER_DATA_PREDICATE_HPP

#include "property_tag.hpp"
#include "property_member_data_value.hpp"
#include "property_operators.hpp"
#include <boost/function.hpp>

#if defined(BOOST_MSVC)

#pragma warning (push)
#pragma warning (disable: 4250)

#endif

namespace properties
{

  template <class T, 
            class C, 
            T C::*d, 
            template <class> class NotificationPolicy = default_policy_tag,
            template <class> class PredicateFailurePolicy = default_policy_tag,
            class Access = read_write_tag
           >
  class prop_member_data_predicate;
  
  template <class T, class C, T C::*d,template <class> class NotificationPolicy,template <class> class PredicateFailurePolicy>
  struct prop_member_data_predicate<T,C,d,NotificationPolicy,PredicateFailurePolicy,write_tag> : 
    prop_member_data<T,C,d,NotificationPolicy,write_tag>,
    PredicateFailurePolicy<T>
  {
    
    prop_member_data_predicate(const prop_member_data_predicate & arg) : 
      prop_member_data<T,C,d,NotificationPolicy,write_tag>(static_cast<const prop_member_data<T,C,d,NotificationPolicy,write_tag> &>(arg)),
      fs(arg.fs)
      { 
      }
      
    prop_member_data_predicate(boost::function<bool (T)> f,C & c) : 
      prop_member_data<T,C,d,NotificationPolicy,write_tag>(c),
      fs(f)
      { 
      }
      
    prop_member_data_predicate(boost::function<bool (T)> f,C & c,T arg) : 
      prop_member_data<T,C,d,NotificationPolicy,write_tag>(c),
      fs(f)
      { 
      if (!fs || fs(arg))
        {
        c.*d = arg;
        }
      }
    
    template<class U>
    prop_member_data_predicate(boost::function<bool (T)> f,C & c,U arg) : 
      prop_member_data<T,C,d,NotificationPolicy,write_tag>(c),
      fs(f)
      { 
      if (!fs || fs(arg))
        {
        c.*d = static_cast<T>(arg);
        }
      }
    
    prop_member_data_predicate & operator = (T arg) 
      { 
      static_cast<prop_member_data<T,C,d,NotificationPolicy,write_tag> &>(*this) = arg;
      return(*this); 
      }
    
    template<class U>
    prop_member_data_predicate & operator = (U arg) 
      { 
      static_cast<prop_member_data<T,C,d,NotificationPolicy,write_tag> &>(*this) = arg;
      return(*this); 
      }
    
    void set(T arg) 
      { 
      if (!fs || fs(arg))
        {
        prop_member_data<T,C,d,NotificationPolicy,write_tag>::set(arg);
        }
      else
        {
        PredicateFailure(*this,fs,boost::optional<T>(),arg);
        }
      }
    
    private:
    
    boost::function<bool (T)> fs;
    
    prop_member_data_predicate & operator = (const prop_member_data_predicate & arg);
    
  };

  template <class T, class C, T C::*d,template <class> class NotificationPolicy>
  struct prop_member_data_predicate<T,C,d,NotificationPolicy,default_policy_tag,write_tag> : 
    prop_member_data<T,C,d,NotificationPolicy,write_tag>
  {
    
    prop_member_data_predicate(const prop_member_data_predicate & arg) : 
      prop_member_data<T,C,d,NotificationPolicy,write_tag>(static_cast<const prop_member_data<T,C,d,NotificationPolicy,write_tag> &>(arg)),
      fs(arg.fs)
      { 
      }
      
    prop_member_data_predicate(boost::function<bool (T)> f,C & c) : 
      prop_member_data<T,C,d,NotificationPolicy,write_tag>(c),
      fs(f)
      { 
      }
      
    prop_member_data_predicate(boost::function<bool (T)> f,C & c,T arg) : 
      prop_member_data<T,C,d,NotificationPolicy,write_tag>(c),
      fs(f)
      { 
      if (!fs || fs(arg))
        {
        c.*d = arg;
        }
      }
    
    template<class U>
    prop_member_data_predicate(boost::function<bool (T)> f,C & c,U arg) : 
      prop_member_data<T,C,d,NotificationPolicy,write_tag>(c),
      fs(f)
      { 
      if (!fs || fs(arg))
        {
        c.*d = static_cast<T>(arg);
        }
      }
    
    prop_member_data_predicate & operator = (T arg) 
      { 
      static_cast<prop_member_data<T,C,d,NotificationPolicy,write_tag> &>(*this) = arg;
      return(*this); 
      }
    
    template<class U>
    prop_member_data_predicate & operator = (U arg) 
      { 
      static_cast<prop_member_data<T,C,d,NotificationPolicy,write_tag> &>(*this) = arg;
      return(*this); 
      }
    
    void set(T arg) 
      { 
      if (!fs || fs(arg))
        {
        prop_member_data<T,C,d,NotificationPolicy,write_tag>::set(arg);
        }
      }
    
    private:
    
    boost::function<bool (T)> fs;
    
    prop_member_data_predicate & operator = (const prop_member_data_predicate & arg);
    
  };

  template <class T, class C, T C::*d,template <class> class NotificationPolicy,template <class> class PredicateFailurePolicy>
  struct prop_member_data_predicate<T,C,d,NotificationPolicy,PredicateFailurePolicy,read_write_tag> : 
    prop_member_data<T,C,d,NotificationPolicy,read_write_tag>,
    PredicateFailurePolicy<T>
  {
    
    prop_member_data_predicate(const prop_member_data_predicate & arg) : 
      prop_member_data<T,C,d,NotificationPolicy,read_write_tag>(static_cast<const prop_member_data<T,C,d,NotificationPolicy,read_write_tag> &>(arg)),
      fs(arg.fs)
      { 
      }
      
    prop_member_data_predicate(boost::function<bool (T)> f,C & c) : 
      prop_member_data<T,C,d,NotificationPolicy,read_write_tag>(c),
      fs(f)
      { 
      }
      
    prop_member_data_predicate(boost::function<bool (T)> f,C & c,T arg) : 
      prop_member_data<T,C,d,NotificationPolicy,read_write_tag>(c),
      fs(f)
      { 
      if (!fs || fs(arg))
        {
        c.*d = arg;
        }
      }
    
    template<class U>
    prop_member_data_predicate(boost::function<bool (T)> f,C & c,U arg) : 
      prop_member_data<T,C,d,NotificationPolicy,read_write_tag>(c),
      fs(f)
      { 
      if (!fs || fs(arg))
        {
        c.*d = static_cast<T>(arg);
        }
      }
    
    prop_member_data_predicate & operator = (const prop_member_data_predicate & arg)
      {
      static_cast<prop_member_data<T,C,d,NotificationPolicy,read_write_tag> &>(*this) = static_cast<const prop_member_data<T,C,d,NotificationPolicy,read_write_tag> &>(arg);
      return(*this); 
      }
    
    prop_member_data_predicate & operator = (T arg) 
      { 
      static_cast<prop_member_data<T,C,d,NotificationPolicy,read_write_tag> &>(*this) = arg;
      return(*this); 
      }
    
    template<class U>
    prop_member_data_predicate & operator = (U arg) 
      { 
      static_cast<prop_member_data<T,C,d,NotificationPolicy,read_write_tag> &>(*this) = arg;
      return(*this); 
      }
    
    void set(T arg) 
      { 
      if (!fs || fs(arg))
        {
        prop_member_data<T,C,d,NotificationPolicy,read_write_tag>::set(arg);
        }
      else
        {
      
        T t(*this);
      
        PredicateFailure(*this,fs,boost::optional<T>(t),arg);
        }
      }
    
    private:
    
    boost::function<bool (T)> fs;
    
  };

  template <class T, class C, T C::*d,template <class> class NotificationPolicy>
  struct prop_member_data_predicate<T,C,d,NotificationPolicy,default_policy_tag,read_write_tag> : 
    prop_member_data<T,C,d,NotificationPolicy,read_write_tag>
  {
    
    prop_member_data_predicate(const prop_member_data_predicate & arg) : 
      prop_member_data<T,C,d,NotificationPolicy,read_write_tag>(static_cast<const prop_member_data<T,C,d,NotificationPolicy,read_write_tag> &>(arg)),
      fs(arg.fs)
      { 
      }
      
    prop_member_data_predicate(boost::function<bool (T)> f,C & c) : 
      prop_member_data<T,C,d,NotificationPolicy,read_write_tag>(c),
      fs(f)
      { 
      }
      
    prop_member_data_predicate(boost::function<bool (T)> f,C & c,T arg) : 
      prop_member_data<T,C,d,NotificationPolicy,read_write_tag>(c),
      fs(f)
      { 
      if (!fs || fs(arg))
        {
        c.*d = arg;
        }
      }
    
    template<class U>
    prop_member_data_predicate(boost::function<bool (T)> f,C & c,U arg) : 
      prop_member_data<T,C,d,NotificationPolicy,read_write_tag>(c),
      fs(f)
      { 
      if (!fs || fs(arg))
        {
        c.*d = static_cast<T>(arg);
        }
      }
    
    prop_member_data_predicate & operator = (const prop_member_data_predicate & arg)
      {
      static_cast<prop_member_data<T,C,d,NotificationPolicy,read_write_tag> &>(*this) = static_cast<const prop_member_data<T,C,d,NotificationPolicy,read_write_tag> &>(arg);
      return(*this); 
      }
    
    prop_member_data_predicate & operator = (T arg) 
      { 
      static_cast<prop_member_data<T,C,d,NotificationPolicy,read_write_tag> &>(*this) = arg;
      return(*this); 
      }
    
    template<class U>
    prop_member_data_predicate & operator = (U arg) 
      { 
      static_cast<prop_member_data<T,C,d,NotificationPolicy,read_write_tag> &>(*this) = arg;
      return(*this); 
      }
    
    void set(T arg) 
      { 
      if (!fs || fs(arg))
        {
        prop_member_data<T,C,d,NotificationPolicy,read_write_tag>::set(arg);
        }
      }
    
    private:
    
    boost::function<bool (T)> fs;
    
  };

template <class T, class C, T C::*d,template <class> class NotificationPolicy,template <class> class PredicateFailurePolicy> 
typename boost::enable_if
  <
  typename detail::omf_increment<T>::type,
  prop_member_data_predicate<T,C,d,NotificationPolicy,PredicateFailurePolicy,read_write_tag> &
  >::type
operator ++ (prop_member_data_predicate<T,C,d,NotificationPolicy,PredicateFailurePolicy,read_write_tag> & value)
  {
  
  T t(value.get());
  
  ++t;
  value.set(t);
  return(value);
  }
      
template <class T, class C, T C::*d,template <class> class NotificationPolicy,template <class> class PredicateFailurePolicy> 
typename boost::enable_if
  <
  typename detail::omf_decrement<T>::type,
  prop_member_data_predicate<T,C,d,NotificationPolicy,PredicateFailurePolicy,read_write_tag> &
  >::type
operator -- (prop_member_data_predicate<T,C,d,NotificationPolicy,PredicateFailurePolicy,read_write_tag> & value)
  {
  
  T t(value.get());
  
  --t;
  value.set(t);
  return(value);
  }
  
template <class T, class C, T C::*d,template <class> class NotificationPolicy,template <class> class PredicateFailurePolicy> 
typename boost::enable_if
  <
  typename detail::omf_assign_multiply_same<T>::type,
  prop_member_data_predicate<T,C,d,NotificationPolicy,PredicateFailurePolicy,read_write_tag> &
  >::type
operator *= (prop_member_data_predicate<T,C,d,NotificationPolicy,PredicateFailurePolicy,read_write_tag> & first,T second)
  {
  
  typename detail::binary_ro_multiply_same<T>::type t(first.get() * second);
  
  first.set(static_cast<T>(t));
  return(first);
  }

template <class T, class C, T C::*d,template <class> class NotificationPolicy,template <class> class PredicateFailurePolicy, class U>
typename boost::enable_if
  <
  typename detail::omf_assign_multiply<T,U>::type,
  prop_member_data_predicate<T,C,d,NotificationPolicy,PredicateFailurePolicy,read_write_tag> &
  >::type
operator *= (prop_member_data_predicate<T,C,d,NotificationPolicy,PredicateFailurePolicy,read_write_tag> & first,U second)
  {
  
  typename detail::binary_ro_multiply<T,U>::type t(first.get() * second);
  
  first.set(static_cast<T>(t));
  return(first);
  }

template <class T, class C, T C::*d,template <class> class NotificationPolicy,template <class> class PredicateFailurePolicy> 
typename boost::enable_if
  <
  typename detail::omf_assign_divide_same<T>::type,
  prop_member_data_predicate<T,C,d,NotificationPolicy,PredicateFailurePolicy,read_write_tag> &
  >::type
operator /= (prop_member_data_predicate<T,C,d,NotificationPolicy,PredicateFailurePolicy,read_write_tag> & first,T second)
  {
  
  typename detail::binary_ro_divide_same<T>::type t(first.get() / second);
  
  first.set(static_cast<T>(t));
  return(first);
  }
  
template <class T, class C, T C::*d,template <class> class NotificationPolicy,template <class> class PredicateFailurePolicy, class U>
typename boost::enable_if
  <
  typename detail::omf_assign_divide<T,U>::type,
  prop_member_data_predicate<T,C,d,NotificationPolicy,PredicateFailurePolicy,read_write_tag> &
  >::type
operator /= (prop_member_data_predicate<T,C,d,NotificationPolicy,PredicateFailurePolicy,read_write_tag> & first,U second)
  {
  
  typename detail::binary_ro_divide<T,U>::type t(first.get() / second);
  
  first.set(static_cast<T>(t));
  return(first);
  }
  
template <class T, class C, T C::*d,template <class> class NotificationPolicy,template <class> class PredicateFailurePolicy> 
typename boost::enable_if
  <
  typename detail::omf_assign_modulo_same<T>::type,
  prop_member_data_predicate<T,C,d,NotificationPolicy,PredicateFailurePolicy,read_write_tag> &
  >::type
operator %= (prop_member_data_predicate<T,C,d,NotificationPolicy,PredicateFailurePolicy,read_write_tag> & first,T second)
  {
  
  typename detail::binary_ro_modulo_same<T>::type t(first.get() % second);
  
  first.set(static_cast<T>(t));
  return(first);
  }
  
template <class T, class C, T C::*d,template <class> class NotificationPolicy,template <class> class PredicateFailurePolicy, class U> 
typename boost::enable_if
  <
  typename detail::omf_assign_modulo<T,U>::type,
  prop_member_data_predicate<T,C,d,NotificationPolicy,PredicateFailurePolicy,read_write_tag> &
  >::type
operator %= (prop_member_data_predicate<T,C,d,NotificationPolicy,PredicateFailurePolicy,read_write_tag> & first,U second)
  {
  
  typename detail::binary_ro_modulo<T,U>::type t(first.get() % second);
  
  first.set(static_cast<T>(t));
  return(first);
  }
  
template <class T, class C, T C::*d,template <class> class NotificationPolicy,template <class> class PredicateFailurePolicy> 
typename boost::enable_if
  <
  typename detail::omf_assign_add_same<T>::type,
  prop_member_data_predicate<T,C,d,NotificationPolicy,PredicateFailurePolicy,read_write_tag> &
  >::type
operator += (prop_member_data_predicate<T,C,d,NotificationPolicy,PredicateFailurePolicy,read_write_tag> & first,T second)
  {
  
  typename detail::binary_ro_add_same<T>::type t(first.get() + second);
  
  first.set(static_cast<T>(t));
  return(first);
  }

template <class T, class C, T C::*d,template <class> class NotificationPolicy,template <class> class PredicateFailurePolicy, class U> 
typename boost::enable_if
  <
  typename detail::omf_assign_add<T,U>::type,
  prop_member_data_predicate<T,C,d,NotificationPolicy,PredicateFailurePolicy,read_write_tag> &
  >::type
operator += (prop_member_data_predicate<T,C,d,NotificationPolicy,PredicateFailurePolicy,read_write_tag> & first,U second)
  {
  
  typename detail::binary_ro_add<T,U>::type t(first.get() + second);
  
  first.set(static_cast<T>(t));
  return(first);
  }

template <class T, class C, T C::*d,template <class> class NotificationPolicy,template <class> class PredicateFailurePolicy> 
typename boost::enable_if
  <
  typename detail::omf_assign_subtract_same<T>::type,
  prop_member_data_predicate<T,C,d,NotificationPolicy,PredicateFailurePolicy,read_write_tag> &
  >::type
operator -= (prop_member_data_predicate<T,C,d,NotificationPolicy,PredicateFailurePolicy,read_write_tag> & first,T second)
  {
  
  typename detail::binary_ro_subtract_same<T>::type t(first.get() - second);
  
  first.set(static_cast<T>(t));
  return(first);
  }

template <class T, class C, T C::*d,template <class> class NotificationPolicy,template <class> class PredicateFailurePolicy, class U> 
typename boost::enable_if
  <
  typename detail::omf_assign_subtract<T,U>::type,
  prop_member_data_predicate<T,C,d,NotificationPolicy,PredicateFailurePolicy,read_write_tag> &
  >::type
operator -= (prop_member_data_predicate<T,C,d,NotificationPolicy,PredicateFailurePolicy,read_write_tag> & first,U second)
  {
  
  typename detail::binary_ro_subtract<T,U>::type t(first.get() - second);
  
  first.set(static_cast<T>(t));
  return(first);
  }

template <class T, class C, T C::*d,template <class> class NotificationPolicy,template <class> class PredicateFailurePolicy> 
typename boost::enable_if
  <
  typename detail::omf_assign_left_shift_same<T>::type,
  prop_member_data_predicate<T,C,d,NotificationPolicy,PredicateFailurePolicy,read_write_tag> &
  >::type
operator <<= (prop_member_data_predicate<T,C,d,NotificationPolicy,PredicateFailurePolicy,read_write_tag> & first,T second)
  {
  
  typename detail::binary_ro_left_shift_same<T>::type t(first.get() << second);
  
  first.set(static_cast<T>(t));
  return(first);
  }
  
template <class T, class C, T C::*d,template <class> class NotificationPolicy,template <class> class PredicateFailurePolicy, class U> 
typename boost::enable_if
  <
  typename detail::omf_assign_left_shift<T,U>::type,
  prop_member_data_predicate<T,C,d,NotificationPolicy,PredicateFailurePolicy,read_write_tag> &
  >::type
operator <<= (prop_member_data_predicate<T,C,d,NotificationPolicy,PredicateFailurePolicy,read_write_tag> & first,U second)
  {
  
  typename detail::binary_ro_left_shift<T,U>::type t(first.get() << second);
  
  first.set(static_cast<T>(t));
  return(first);
  }
  
template <class T, class C, T C::*d,template <class> class NotificationPolicy,template <class> class PredicateFailurePolicy> 
typename boost::enable_if
  <
  typename detail::omf_assign_right_shift_same<T>::type,
  prop_member_data_predicate<T,C,d,NotificationPolicy,PredicateFailurePolicy,read_write_tag> &
  >::type
operator >>= (prop_member_data_predicate<T,C,d,NotificationPolicy,PredicateFailurePolicy,read_write_tag> & first,T second)
  {
  
  typename detail::binary_ro_right_shift_same<T>::type t(first.get() >> second);
  
  first.set(static_cast<T>(t));
  return(first);
  }
  
template <class T, class C, T C::*d,template <class> class NotificationPolicy,template <class> class PredicateFailurePolicy, class U>
typename boost::enable_if
  <
  typename detail::omf_assign_right_shift<T,U>::type,
  prop_member_data_predicate<T,C,d,NotificationPolicy,PredicateFailurePolicy,read_write_tag> &
  >::type
operator >>= (prop_member_data_predicate<T,C,d,NotificationPolicy,PredicateFailurePolicy,read_write_tag> & first,U second)
  {
  
  typename detail::binary_ro_right_shift<T,U>::type t(first.get() >> second);
  
  first.set(static_cast<T>(t));
  return(first);
  }
  
template <class T, class C, T C::*d,template <class> class NotificationPolicy,template <class> class PredicateFailurePolicy> 
typename boost::enable_if
  <
  typename detail::omf_assign_and_same<T>::type,
  prop_member_data_predicate<T,C,d,NotificationPolicy,PredicateFailurePolicy,read_write_tag> &
  >::type
operator &= (prop_member_data_predicate<T,C,d,NotificationPolicy,PredicateFailurePolicy,read_write_tag> & first,T second)
  {
  
  typename detail::binary_ro_and_same<T>::type t(first.get() & second);
  
  first.set(static_cast<T>(t));
  return(first);
  }
  
template <class T, class C, T C::*d,template <class> class NotificationPolicy,template <class> class PredicateFailurePolicy, class U>
typename boost::enable_if
  <
  typename detail::omf_assign_and<T,U>::type,
  prop_member_data_predicate<T,C,d,NotificationPolicy,PredicateFailurePolicy,read_write_tag> &
  >::type
operator &= (prop_member_data_predicate<T,C,d,NotificationPolicy,PredicateFailurePolicy,read_write_tag> & first,U second)
  {
  
  typename detail::binary_ro_and<T,U>::type t(first.get() & second);
  
  first.set(static_cast<T>(t));
  return(first);
  }
  
template <class T, class C, T C::*d,template <class> class NotificationPolicy,template <class> class PredicateFailurePolicy> 
typename boost::enable_if
  <
  typename detail::omf_assign_xor_same<T>::type,
  prop_member_data_predicate<T,C,d,NotificationPolicy,PredicateFailurePolicy,read_write_tag> &
  >::type
operator ^= (prop_member_data_predicate<T,C,d,NotificationPolicy,PredicateFailurePolicy,read_write_tag> & first,T second)
  {
  
  typename detail::binary_ro_xor_same<T>::type t(first.get() ^ second);
  
  first.set(static_cast<T>(t));
  return(first);
  }
  
template <class T, class C, T C::*d,template <class> class NotificationPolicy,template <class> class PredicateFailurePolicy, class U> 
typename boost::enable_if
  <
  typename detail::omf_assign_xor<T,U>::type,
  prop_member_data_predicate<T,C,d,NotificationPolicy,PredicateFailurePolicy,read_write_tag> &
  >::type
operator ^= (prop_member_data_predicate<T,C,d,NotificationPolicy,PredicateFailurePolicy,read_write_tag> & first,U second)
  {
  
  typename detail::binary_ro_xor<T,U>::type t(first.get() ^ second);
  
  first.set(static_cast<T>(t));
  return(first);
  }
  
template <class T, class C, T C::*d,template <class> class NotificationPolicy,template <class> class PredicateFailurePolicy> 
typename boost::enable_if
  <
  typename detail::omf_assign_or_same<T>::type,
  prop_member_data_predicate<T,C,d,NotificationPolicy,PredicateFailurePolicy,read_write_tag> &
  >::type
operator |= (prop_member_data_predicate<T,C,d,NotificationPolicy,PredicateFailurePolicy,read_write_tag> & first,T second)
  {
  
  typename detail::binary_ro_or_same<T>::type t(first.get() | second);
  
  first.set(static_cast<T>(t));
  return(first);
  }
  
template <class T, class C, T C::*d,template <class> class NotificationPolicy,template <class> class PredicateFailurePolicy, class U> 
typename boost::enable_if
  <
  typename detail::omf_assign_or<T,U>::type,
  prop_member_data_predicate<T,C,d,NotificationPolicy,PredicateFailurePolicy,read_write_tag> &
  >::type
operator |= (prop_member_data_predicate<T,C,d,NotificationPolicy,PredicateFailurePolicy,read_write_tag> & first,U second)
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

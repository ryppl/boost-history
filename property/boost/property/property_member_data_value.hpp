#if !defined(PROPERTY_MEMBER_DATA_VALUE_HPP)
#define PROPERTY_MEMBER_DATA_VALUE_HPP

#include "property_interface.hpp"
#include "property_tag.hpp"
#include "property_operators.hpp"
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
            class C, 
            T C::*d, 
            template <class> class NotificationPolicy = default_policy_tag,
            class Access = typename boost::mpl::if_
                        <
                        boost::is_const<T>,
                        read_tag,
                        read_write_tag
                        >::type
           >
  class prop_member_data;
  
  template <class T, class C, T C::*d>
  class prop_member_data<T,C,d,default_policy_tag,read_tag> : 
    public virtual i_property_read<T>
  {
    public:
    
    explicit prop_member_data(C & c) : 
      cf(c) 
      { 
      }
    
    operator T() const 
      { 
      return(get()); 
      }
    
    T get() const 
      { 
      return(cf.*d); 
      }
                 
    private:
    
    C & cf;
    
    prop_member_data & operator = (const prop_member_data &);
  
  };

  template <class T, class C, T C::*d>
  class prop_member_data<T,C,d,default_policy_tag,write_tag> : 
    public virtual i_property_write<T>
  {
    public:
    
    explicit prop_member_data(C & c) : 
      cf(c) 
      { 
      }
      
    prop_member_data(C & c,T arg) : 
      cf(c)
      { 
      cf.*d = arg;
      }
    
    template<class U>
    prop_member_data(C & c,U arg) : 
      cf(c) 
      { 
      cf.*d = static_cast<T>(arg);
      }
    
    prop_member_data & operator = (T arg) 
      { 
      set(arg); 
      return(*this); 
      }
    
    template<class U>
    prop_member_data & operator = (U arg) 
      { 
      set(static_cast<T>(arg)); 
      return(*this); 
      }
    
    void set(T arg) 
      { 
      cf.*d = arg; 
      }
    
    private:
    
    C & cf;
  
    prop_member_data & operator = (const prop_member_data &);
  
  };

  template <class T, class C, T C::*d,template <class> class NotificationPolicy>
  class prop_member_data<T,C,d,NotificationPolicy,write_tag> : 
    public virtual i_property_write<T>, 
    public NotificationPolicy<T>
  {
    public:
    
    explicit prop_member_data(C & c) : 
      cf(c) 
      { 
      }
      
    prop_member_data(C & c,T arg) : 
      cf(c) 
      { 
      cf.*d = arg; 
      }
    
    template<class U>
    prop_member_data(C & c,U arg) : 
      cf(c) 
      { 
      cf.*d = static_cast<T>(arg); 
      }
    
    prop_member_data & operator = (T arg) 
      { 
      set(arg); 
      return(*this); 
      }
    
    template<class U>
    prop_member_data & operator = (U arg) 
      { 
      set(static_cast<T>(arg)); 
      return(*this); 
      }
    
    void set(T arg) 
      { 
      cf.*d = arg; 
      property_changed(*this,oldT,arg);
      }
    
    private:
    
    C & cf;
    
    protected:
    
    boost::optional<T> oldT;
  
    prop_member_data & operator = (const prop_member_data &);
  
  };

  template <class T, class C, T C::*d>
  struct prop_member_data<T,C,d,default_policy_tag,read_write_tag> : 
    prop_member_data<T,C,d,default_policy_tag,read_tag>, 
    prop_member_data<T,C,d,default_policy_tag,write_tag>,
    i_property_read_write<T>
  {
    public:
    
    explicit prop_member_data(C & c) : 
      prop_member_data<T,C,d,default_policy_tag,read_tag>(c) , 
      prop_member_data<T,C,d,default_policy_tag,write_tag>(c) 
      { 
      }
      
    prop_member_data(const prop_member_data & arg) : 
      prop_member_data<T,C,d,default_policy_tag,read_tag>(arg) , 
      prop_member_data<T,C,d,default_policy_tag,write_tag>(static_cast<const prop_member_data<T,C,d,default_policy_tag,write_tag> &>(arg)) 
      { 
      }
    
    prop_member_data(C & c,T arg) : 
      prop_member_data<T,C,d,default_policy_tag,read_tag>(c) , 
      prop_member_data<T,C,d,default_policy_tag,write_tag>(c,arg) 
      { 
      }
    
    template<class U>
    prop_member_data(C & c,U arg) : 
      prop_member_data<T,C,d,default_policy_tag,read_tag>(c), 
      prop_member_data<T,C,d,default_policy_tag,write_tag>(c,arg) 
      { 
      }
    
    prop_member_data & operator = (const prop_member_data & arg) 
      {
      return(*this = static_cast<T>(arg));
      }
      
    prop_member_data & operator = (T arg) 
      { 
      (static_cast<prop_member_data<T,C,d,default_policy_tag,write_tag> &>(*this)) = arg; 
      return(*this); 
      }
    
    template<class U>
    prop_member_data & operator = (U arg) 
      { 
      (static_cast<prop_member_data<T,C,d,default_policy_tag,write_tag> &>(*this)) = arg; 
      return(*this); 
      }
    
  };

  template <class T, class C, T C::*d,template <class> class NotificationPolicy>
  struct prop_member_data<T,C,d,NotificationPolicy,read_write_tag> : 
    prop_member_data<T,C,d,default_policy_tag,read_tag>, 
    prop_member_data<T,C,d,NotificationPolicy,write_tag>,
    i_property_read_write<T>
  {
    public:
    
    explicit prop_member_data(C & c) : 
      prop_member_data<T,C,d,default_policy_tag,read_tag>(c), 
      prop_member_data<T,C,d,NotificationPolicy,write_tag>(c) 
      { 
      }
      
    prop_member_data(const prop_member_data & arg) : 
      prop_member_data<T,C,d,default_policy_tag,read_tag>(arg), 
      prop_member_data<T,C,d,NotificationPolicy,write_tag>(static_cast<const prop_member_data<T,C,d,NotificationPolicy,write_tag> &>(arg)) 
      { 
      }
    
    prop_member_data(C & c,T arg) : 
      prop_member_data<T,C,d,default_policy_tag,read_tag>(c), 
      prop_member_data<T,C,d,NotificationPolicy,write_tag>(c,arg)
      { 
      }
    
    template<class U>
    prop_member_data(C & c,U arg) : 
      prop_member_data<T,C,d,default_policy_tag,read_tag>(c) , 
      prop_member_data<T,C,d,NotificationPolicy,write_tag>(c,arg) 
      { 
      }
    
    prop_member_data & operator = (const prop_member_data & arg) 
      {
      return(*this = static_cast<T>(arg));
      }
      
    prop_member_data & operator = (T arg) 
      { 
      (static_cast<prop_member_data<T,C,d,NotificationPolicy,write_tag> &>(*this)).oldT = *this;
      (static_cast<prop_member_data<T,C,d,NotificationPolicy,write_tag> &>(*this)) = arg; 
      return(*this); 
      }
    
    template<class U>
    prop_member_data & operator = (U arg) 
      { 
      (static_cast<prop_member_data<T,C,d,NotificationPolicy,write_tag> &>(*this)).oldT = *this;
      (static_cast<prop_member_data<T,C,d,NotificationPolicy,write_tag> &>(*this)) = arg; 
      return(*this); 
      }
    
  };

template <class T, class C, T C::*d,template <class> class NotificationPolicy> 
typename boost::enable_if
  <
  typename detail::omf_increment<T>::type,
  prop_member_data<T,C,d,NotificationPolicy,read_write_tag> &
  >::type
operator ++ (prop_member_data<T,C,d,NotificationPolicy,read_write_tag> & value)
  {
  
  T t(value.get());
  
  ++t;
  value.set(t);
  return(value);
  }
      
template <class T, class C, T C::*d,template <class> class NotificationPolicy> 
typename boost::enable_if
  <
  typename detail::omf_decrement<T>::type,
  prop_member_data<T,C,d,NotificationPolicy,read_write_tag> &
  >::type
operator -- (prop_member_data<T,C,d,NotificationPolicy,read_write_tag> & value)
  {
  
  T t(value.get());
  
  --t;
  value.set(t);
  return(value);
  }
  
template <class T, class C, T C::*d,template <class> class NotificationPolicy> 
typename boost::enable_if
  <
  typename detail::omf_assign_multiply_same<T>::type,
  prop_member_data<T,C,d,NotificationPolicy,read_write_tag> &
  >::type
operator *= (prop_member_data<T,C,d,NotificationPolicy,read_write_tag> & first,T second)
  {
  
  typename detail::binary_ro_multiply_same<T>::type t(first.get() * second);
  
  first.set(static_cast<T>(t));
  return(first);
  }

template <class T, class C, T C::*d,template <class> class NotificationPolicy, class U>
typename boost::enable_if
  <
  typename detail::omf_assign_multiply<T,U>::type,
  prop_member_data<T,C,d,NotificationPolicy,read_write_tag> &
  >::type
operator *= (prop_member_data<T,C,d,NotificationPolicy,read_write_tag> & first,U second)
  {
  
  typename detail::binary_ro_multiply<T,U>::type t(first.get() * second);
  
  first.set(static_cast<T>(t));
  return(first);
  }

template <class T, class C, T C::*d,template <class> class NotificationPolicy> 
typename boost::enable_if
  <
  typename detail::omf_assign_divide_same<T>::type,
  prop_member_data<T,C,d,NotificationPolicy,read_write_tag> &
  >::type
operator /= (prop_member_data<T,C,d,NotificationPolicy,read_write_tag> & first,T second)
  {
  
  typename detail::binary_ro_divide_same<T>::type t(first.get() / second);
  
  first.set(static_cast<T>(t));
  return(first);
  }
  
template <class T, class C, T C::*d,template <class> class NotificationPolicy, class U>
typename boost::enable_if
  <
  typename detail::omf_assign_divide<T,U>::type,
  prop_member_data<T,C,d,NotificationPolicy,read_write_tag> &
  >::type
operator /= (prop_member_data<T,C,d,NotificationPolicy,read_write_tag> & first,U second)
  {
  
  typename detail::binary_ro_divide<T,U>::type t(first.get() / second);
  
  first.set(static_cast<T>(t));
  return(first);
  }
  
template <class T, class C, T C::*d,template <class> class NotificationPolicy> 
typename boost::enable_if
  <
  typename detail::omf_assign_modulo_same<T>::type,
  prop_member_data<T,C,d,NotificationPolicy,read_write_tag> &
  >::type
operator %= (prop_member_data<T,C,d,NotificationPolicy,read_write_tag> & first,T second)
  {
  
  typename detail::binary_ro_modulo_same<T>::type t(first.get() % second);
  
  first.set(static_cast<T>(t));
  return(first);
  }
  
template <class T, class C, T C::*d,template <class> class NotificationPolicy, class U> 
typename boost::enable_if
  <
  typename detail::omf_assign_modulo<T,U>::type,
  prop_member_data<T,C,d,NotificationPolicy,read_write_tag> &
  >::type
operator %= (prop_member_data<T,C,d,NotificationPolicy,read_write_tag> & first,U second)
  {
  
  typename detail::binary_ro_modulo<T,U>::type t(first.get() % second);
  
  first.set(static_cast<T>(t));
  return(first);
  }
  
template <class T, class C, T C::*d,template <class> class NotificationPolicy> 
typename boost::enable_if
  <
  typename detail::omf_assign_add_same<T>::type,
  prop_member_data<T,C,d,NotificationPolicy,read_write_tag> &
  >::type
operator += (prop_member_data<T,C,d,NotificationPolicy,read_write_tag> & first,T second)
  {
  
  typename detail::binary_ro_add_same<T>::type t(first.get() + second);
  
  first.set(static_cast<T>(t));
  return(first);
  }

template <class T, class C, T C::*d,template <class> class NotificationPolicy, class U> 
typename boost::enable_if
  <
  typename detail::omf_assign_add<T,U>::type,
  prop_member_data<T,C,d,NotificationPolicy,read_write_tag> &
  >::type
operator += (prop_member_data<T,C,d,NotificationPolicy,read_write_tag> & first,U second)
  {
  
  typename detail::binary_ro_add<T,U>::type t(first.get() + second);
  
  first.set(static_cast<T>(t));
  return(first);
  }

template <class T, class C, T C::*d,template <class> class NotificationPolicy> 
typename boost::enable_if
  <
  typename detail::omf_assign_subtract_same<T>::type,
  prop_member_data<T,C,d,NotificationPolicy,read_write_tag> &
  >::type
operator -= (prop_member_data<T,C,d,NotificationPolicy,read_write_tag> & first,T second)
  {
  
  typename detail::binary_ro_subtract_same<T>::type t(first.get() - second);
  
  first.set(static_cast<T>(t));
  return(first);
  }

template <class T, class C, T C::*d,template <class> class NotificationPolicy, class U> 
typename boost::enable_if
  <
  typename detail::omf_assign_subtract<T,U>::type,
  prop_member_data<T,C,d,NotificationPolicy,read_write_tag> &
  >::type
operator -= (prop_member_data<T,C,d,NotificationPolicy,read_write_tag> & first,U second)
  {
  
  typename detail::binary_ro_subtract<T,U>::type t(first.get() - second);
  
  first.set(static_cast<T>(t));
  return(first);
  }

template <class T, class C, T C::*d,template <class> class NotificationPolicy> 
typename boost::enable_if
  <
  typename detail::omf_assign_left_shift_same<T>::type,
  prop_member_data<T,C,d,NotificationPolicy,read_write_tag> &
  >::type
operator <<= (prop_member_data<T,C,d,NotificationPolicy,read_write_tag> & first,T second)
  {
  
  typename detail::binary_ro_left_shift_same<T>::type t(first.get() << second);
  
  first.set(static_cast<T>(t));
  return(first);
  }
  
template <class T, class C, T C::*d,template <class> class NotificationPolicy, class U> 
typename boost::enable_if
  <
  typename detail::omf_assign_left_shift<T,U>::type,
  prop_member_data<T,C,d,NotificationPolicy,read_write_tag> &
  >::type
operator <<= (prop_member_data<T,C,d,NotificationPolicy,read_write_tag> & first,U second)
  {
  
  typename detail::binary_ro_left_shift<T,U>::type t(first.get() << second);
  
  first.set(static_cast<T>(t));
  return(first);
  }
  
template <class T, class C, T C::*d,template <class> class NotificationPolicy> 
typename boost::enable_if
  <
  typename detail::omf_assign_right_shift_same<T>::type,
  prop_member_data<T,C,d,NotificationPolicy,read_write_tag> &
  >::type
operator >>= (prop_member_data<T,C,d,NotificationPolicy,read_write_tag> & first,T second)
  {
  
  typename detail::binary_ro_right_shift_same<T>::type t(first.get() >> second);
  
  first.set(static_cast<T>(t));
  return(first);
  }
  
template <class T, class C, T C::*d,template <class> class NotificationPolicy, class U>
typename boost::enable_if
  <
  typename detail::omf_assign_right_shift<T,U>::type,
  prop_member_data<T,C,d,NotificationPolicy,read_write_tag> &
  >::type
operator >>= (prop_member_data<T,C,d,NotificationPolicy,read_write_tag> & first,U second)
  {
  
  typename detail::binary_ro_right_shift<T,U>::type t(first.get() >> second);
  
  first.set(static_cast<T>(t));
  return(first);
  }
  
template <class T, class C, T C::*d,template <class> class NotificationPolicy> 
typename boost::enable_if
  <
  typename detail::omf_assign_and_same<T>::type,
  prop_member_data<T,C,d,NotificationPolicy,read_write_tag> &
  >::type
operator &= (prop_member_data<T,C,d,NotificationPolicy,read_write_tag> & first,T second)
  {
  
  typename detail::binary_ro_and_same<T>::type t(first.get() & second);
  
  first.set(static_cast<T>(t));
  return(first);
  }
  
template <class T, class C, T C::*d,template <class> class NotificationPolicy, class U>
typename boost::enable_if
  <
  typename detail::omf_assign_and<T,U>::type,
  prop_member_data<T,C,d,NotificationPolicy,read_write_tag> &
  >::type
operator &= (prop_member_data<T,C,d,NotificationPolicy,read_write_tag> & first,U second)
  {
  
  typename detail::binary_ro_and<T,U>::type t(first.get() & second);
  
  first.set(static_cast<T>(t));
  return(first);
  }
  
template <class T, class C, T C::*d,template <class> class NotificationPolicy> 
typename boost::enable_if
  <
  typename detail::omf_assign_xor_same<T>::type,
  prop_member_data<T,C,d,NotificationPolicy,read_write_tag> &
  >::type
operator ^= (prop_member_data<T,C,d,NotificationPolicy,read_write_tag> & first,T second)
  {
  
  typename detail::binary_ro_xor_same<T>::type t(first.get() ^ second);
  
  first.set(static_cast<T>(t));
  return(first);
  }
  
template <class T, class C, T C::*d,template <class> class NotificationPolicy, class U> 
typename boost::enable_if
  <
  typename detail::omf_assign_xor<T,U>::type,
  prop_member_data<T,C,d,NotificationPolicy,read_write_tag> &
  >::type
operator ^= (prop_member_data<T,C,d,NotificationPolicy,read_write_tag> & first,U second)
  {
  
  typename detail::binary_ro_xor<T,U>::type t(first.get() ^ second);
  
  first.set(static_cast<T>(t));
  return(first);
  }
  
template <class T, class C, T C::*d,template <class> class NotificationPolicy> 
typename boost::enable_if
  <
  typename detail::omf_assign_or_same<T>::type,
  prop_member_data<T,C,d,NotificationPolicy,read_write_tag> &
  >::type
operator |= (prop_member_data<T,C,d,NotificationPolicy,read_write_tag> & first,T second)
  {
  
  typename detail::binary_ro_or_same<T>::type t(first.get() | second);
  
  first.set(static_cast<T>(t));
  return(first);
  }
  
template <class T, class C, T C::*d,template <class> class NotificationPolicy, class U> 
typename boost::enable_if
  <
  typename detail::omf_assign_or<T,U>::type,
  prop_member_data<T,C,d,NotificationPolicy,read_write_tag> &
  >::type
operator |= (prop_member_data<T,C,d,NotificationPolicy,read_write_tag> & first,U second)
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

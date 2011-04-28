#pragma once

#include <boost/property_map/property_map.hpp>
#include <boost/type_traits/is_const.hpp>
#include <boost/type_traits/is_reference.hpp>
#include <boost/type_traits/remove_reference.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/and.hpp>
#include <boost/mpl/not.hpp>
#include <boost/mpl/logical.hpp>
#include <boost/mpl/bool.hpp>
#include <PropertyIndexInterface.h>

namespace properties
{
  namespace detail
  {

    template <class PMAP>
    struct PropertyIndexMapBaseTypedefs
      {
      typedef typename boost::property_traits<PMAP>::value_type value_type;
      typedef typename boost::property_traits<PMAP>::key_type key_type;
      typedef typename boost::property_traits<PMAP>::reference reference_type;
      };
    
    template <class PMAP,class bref,class category>
    struct PropertyIndexMapBase;

    template <class PMAP,class bref>
    struct PropertyIndexMapBase<PMAP,bref,boost::readable_property_map_tag> : 
      IPropertyIndexRead<typename boost::property_traits<PMAP>::value_type,typename boost::property_traits<PMAP>::key_type>,
      PropertyIndexMapBaseTypedefs<PMAP>
      {
      
      PropertyIndexMapBase(PMAP & pm) : pmap(pm)
        { 
        }
      
      value_type get(key_type key) const
        {
        return(get(pmap,key));
        }
        
      private:
      
      PMAP & pmap;
        
      };

    template <class PMAP,class bref>
    struct PropertyIndexMapBase<PMAP,bref,boost::writable_property_map_tag> : 
      IPropertyIndexWrite<typename boost::property_traits<PMAP>::value_type,typename boost::property_traits<PMAP>::key_type>,
      PropertyIndexMapBaseTypedefs<PMAP>
      {
      
      PropertyIndexMapBase(PMAP & pm) : pmap(pm)
        { 
        }
      
      void set(key_type key,value_type value)
        {
        put(pmap,key,value);
        }
      
      private:
      
      PMAP & pmap;
        
      };

    template <class PMAP,class bref>
    struct PropertyIndexMapBase<PMAP,bref,boost::read_write_property_map_tag> : 
      IPropertyIndexRead<typename boost::property_traits<PMAP>::value_type,typename boost::property_traits<PMAP>::key_type>,
      IPropertyIndexWrite<typename boost::property_traits<PMAP>::value_type,typename boost::property_traits<PMAP>::key_type>,
      PropertyIndexMapBaseTypedefs<PMAP>
      {
      
      PropertyIndexMapBase(PMAP & pm) : pmap(pm)
        { 
        }
      
      value_type get(key_type key) const
        {
        return(get(pmap,key));
        }
      
      void set(key_type key,value_type value)
        {
        put(pmap,key,value);
        }
        
      private:
      
      PMAP & pmap;
        
      };

    template <class PMAP>
    struct PropertyIndexMapBase<PMAP,boost::mpl::false_,boost::lvalue_property_map_tag> : 
      IPropertyIndexReadWriteLValue<typename boost::property_traits<PMAP>::value_type,typename boost::property_traits<PMAP>::key_type,typename boost::property_traits<PMAP>::reference>,
      PropertyIndexMapBaseTypedefs<PMAP>
      {
      
      PropertyIndexMapBase(PMAP & pm) : pmap(pm)
        { 
        }
      
      value_type get(key_type key) const
        {
        return(get(pmap,key));
        }
      
      void set(key_type key,value_type value)
        {
        put(pmap,key,value);
        }
        
      reference_type operator [] (key_type key)
        {
        return(pmap[key]);
        }
        
      private:
      
      PMAP & pmap;
        
      };

    template <class PMAP>
    struct PropertyIndexMapBase<PMAP,boost::mpl::true_,boost::lvalue_property_map_tag> : 
      IPropertyIndexReadImmutable<typename boost::property_traits<PMAP>::value_type,typename boost::property_traits<PMAP>::key_type,typename boost::property_traits<PMAP>::reference>,
      PropertyIndexMapBaseTypedefs<PMAP>
      {
      
      PropertyIndexMapBase(PMAP & pm) : pmap(pm)
        { 
        }
      
      value_type get(key_type key) const
        {
        return(get(pmap,key));
        }
      
      reference_type operator [] (key_type key) const
        {
        return(pmap[key]);
        }
        
      private:
      
      PMAP & pmap;
        
      };

    template <class PMAP>
    struct PropertyIndexMapMakeBase
      {
      
      typedef PropertyIndexMapBase
              <
              PMAP,
              typename boost::mpl::if_
                <
                boost::mpl::and_
                  <
                  boost::is_reference
                    <
                    typename boost::property_traits<PMAP>::reference
                    >,
                  boost::mpl::not_
                    <
                    boost::is_const
                      <
                      typename boost::remove_reference<typename boost::property_traits<PMAP>::reference>::type
                      >
                    >
                  >,
                boost::mpl::false_,
                boost::mpl::true_
                >::type,
              typename boost::property_traits<PMAP>::category
              > 
              type;
      };
  }
}
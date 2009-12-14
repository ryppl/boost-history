
#ifndef BOOST_CGI_DATA_MAP_PROXY_HPP_INCLUDED_20091206_
#define BOOST_CGI_DATA_MAP_PROXY_HPP_INCLUDED_20091206_

#include <boost/assert.hpp>
///////////////////////////////////////////////////////////
#include "boost/cgi/config.hpp"

BOOST_CGI_NAMESPACE_BEGIN

 namespace common {

  /// A proxy class to provide access to the data maps as member variables.
  /**
   * This wraps the underlying data map and exposes a std::map-like 
   * interface for the different data maps.
   *
   * It also includes an as<> member function which casts the found data
   * into any type the user specifies.
   */
  template<typename MapType>
  struct data_map_proxy
  {
    typedef MapType                                   map_type;
    typedef typename map_type::key_type               key_type;
    typedef typename map_type::value_type             value_type;
    typedef typename map_type::mapped_type            mapped_type;
    typedef typename map_type::size_type              size_type;
    typedef typename map_type::iterator               iterator;
    typedef typename map_type::const_iterator         const_iterator;
    typedef typename map_type::reverse_iterator       reverse_iterator;
    typedef typename map_type::const_reverse_iterator const_reverse_iterator;
    typedef typename map_type::allocator_type         allocator_type;
    
    void set(map_type& data) { impl = &data; }
    
    iterator begin() {
       BOOST_ASSERT(impl); return impl->begin(); }
    iterator end() {
       BOOST_ASSERT(impl); return impl->end(); }
    const_iterator begin() const {
       BOOST_ASSERT(impl); return impl->begin(); }
    const_iterator end() const {
       BOOST_ASSERT(impl); return impl->end(); }
    reverse_iterator rbegin() {
       BOOST_ASSERT(impl); return impl->rbegin(); }
    reverse_iterator rend() {
       BOOST_ASSERT(impl); return impl->rend(); }
    const_reverse_iterator rbegin() const {
       BOOST_ASSERT(impl); return impl->rbegin(); }
    const_reverse_iterator rend() const {
       BOOST_ASSERT(impl); return impl->rend(); }

    bool empty() { BOOST_ASSERT(impl); return impl->empty(); }
    
    void clear() { BOOST_ASSERT(impl); return impl->clear(); }
    
    size_type size() const { BOOST_ASSERT(impl); return impl->size(); }
    
    size_type count(const key_type& key) {
       BOOST_ASSERT(impl);
       return impl->count(key);
    }
    
    /// Get a value for the key, with fallback.
    mapped_type const&
      pick(key_type const& key, mapped_type const& default_value) const
    {
      BOOST_ASSERT(impl);
      const_iterator iter = impl->find(key);
      return iter == impl->end() ? default_value : iter->second;
    }

    /// Get a value for the key as a specified type, with fallback.
    /**
     * @param key   The name of CGI parameter to look for.
     * @param default_value
     *              The default return value. If no data exists in the map
     *              for the specified key, or the data cannot be converted
     *              into the type of the default_ value, then this value is
     *              returned.
     *
     * If the key cannot be found, returns a default-constructed object
     * of type T.
     *
     * If the key is found, attempts to convert the value into the type
     * T. This throws a boost::bad_lexical_cast when it fails.
     */
    template<typename T>
    T as(key_type const& key, T const& default_value = T()) const
    {
      BOOST_ASSERT(impl);
      const_iterator iter = impl->find(key);

      T val (default_value);

      if (iter != impl->end()) {
        try {
          val = boost::lexical_cast<T>(iter->second);
        } catch(...) {
          // pass
        }
      }
      return val;
    }
    
    mapped_type& operator[](const char* varname) {
      BOOST_ASSERT(impl); 
      return (*impl)[varname];
    }
    
    mapped_type& operator[](const char* varname) const {
      BOOST_ASSERT(impl); 
      return (*impl)[varname];
    }
    
    mapped_type& operator[](key_type const& varname) {
      BOOST_ASSERT(impl); 
      return (*impl)[varname.c_str()];
    }
    
    mapped_type const& operator[](key_type const& varname) const {
      BOOST_ASSERT(impl); 
      return (*impl)[varname.c_str()];
    }
    
    operator map_type&() { BOOST_ASSERT(impl); return *impl; }
    bool operator!() const { return !impl; }

  private:      
    map_type* impl;
  };

 } // namespace common
BOOST_CGI_NAMESPACE_END

#endif // BOOST_CGI_DATA_MAP_PROXY_HPP_INCLUDED_20091206_



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

    mapped_type const&
      get(key_type const& key, mapped_type const& default_) const
    {
      BOOST_ASSERT(impl);
      const_iterator iter = impl->find(key);
      return iter == impl->end() ? default_ : iter->second;
    }
    
    template<typename T>
    T get_as(key_type const& key, T const& default_) const
    {
      BOOST_ASSERT(impl);
      const_iterator iter = impl->find(key);

      T val (default_);

      if (iter != impl->end()) {
        try {
          val = boost::lexical_cast<T>(iter->second);
        } catch(...) {
          // pass
        }
      }
      return val;
    }
    
    template<typename T>
    T as(key_type const& key) {
      BOOST_ASSERT(impl);
      const_iterator iter = impl->find(key);
      return iter == impl->end()
                   ? T()
                   : boost::lexical_cast<T>(val);
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


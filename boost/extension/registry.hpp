/* (C) Copyright Jeremy Pack 2007
* Distributed under the Boost Software License, Version 1.0. (See
* accompanying file LICENSE_1_0.txt or copy at
* http://www.boost.org/LICENSE_1_0.txt)
*/
#ifndef BOOST_EXTENSION_REGISTRY_HPP
#define BOOST_EXTENSION_REGISTRY_HPP
#include <map>
#include <string>
#include <boost/extension/counted_factory_map.hpp>
#include <boost/extension/shared_library.hpp>
#include  <boost/extension/impl/typeinfo.hpp>
namespace boost{namespace extensions{
template <class TypeInfo>
class basic_registry : public basic_counted_factory_map<TypeInfo>
{
protected:
  std::map<std::string, std::pair<shared_library, int> > libraries_;
  typedef std::map<std::string, std::pair<shared_library, int> >::iterator 
    library_iterator;
public:
  ~basic_registry() {
    for (library_iterator it = libraries_.begin(); it != libraries_.end();)
    {
      close((it++)->first.c_str());
    }
  }
  bool clear() {
    bool ret_val = false;
    for (library_iterator it = libraries_.begin(); it != libraries_.end();)
    {
      if (!it->second.second) {
        ret_val = true;
        close((it++)->first.c_str());
      }    
      else
      {
        ++it;
      }
    }
    return ret_val;
  }
  bool open(const char * library_location, const char * function_name = 
            "boost_extension_registry_function") {
    library_iterator it = libraries_.find(library_location);
    if (it == libraries_.end()) {
      it = libraries_.insert(std::make_pair(std::string(library_location), 
                             std::make_pair(shared_library(library_location, false), int(0)))).first;
      this->current_library_ = library_location;
      this->current_counter_ = &it->second.second;
      it->second.first.open();
      if (it->second.first.is_open() == false) {
        libraries_.erase(it);
        return false;
      }
      functor<void, counted_factory_map &> load_func = 
        it->second.first.get_functor<void, counted_factory_map &>(function_name);
      if (load_func.is_valid())
      {
        load_func(*this);
        return true;
      }
      return false;
    }
    return false;
  }
  bool close(const char * library_location) {
    // This removes all of the factories from this library
    library_iterator it = libraries_.find(library_location);
    if (it == libraries_.end()) 
      return false;
    for (typename basic_counted_factory_map<TypeInfo>::FactoryMap::iterator it = this->factories_.begin();
         it != this->factories_.end();) {
      if(it->second->remove_library(library_location)) 
      {
        delete it->second;
        this->factories_.erase(it++);
      }
      else
      {
        ++it; 
      }
    }
    it->second.first.close();
    libraries_.erase(it);
    return true;
  }
  size_t num_libraries() {
    return libraries_.size(); 
  }
};
typedef basic_registry<default_type_info> registry;
}}
#endif

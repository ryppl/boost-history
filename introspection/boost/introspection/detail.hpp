#include <iostream>
#include <vector>
#include <map>
#include <memory> 
#include <boost/type_traits.hpp> 
#include <boost/shared_ptr.hpp> 
#include <boost/intrusive_ptr.hpp> 

namespace boost { namespace introspection {

namespace detail {

template<typename T>
class auto_vector_ptr : public std::vector<T*>
{
public:
  ~auto_vector_ptr()
  {
    typename std::vector<T*>::reverse_iterator iter(this->rbegin()), last(this->rend());
    while (iter != last)
      delete *iter++;
  }
};

template<typename K, typename V>
class auto_map_ptr : public std::map<K, V*>
{
public:
  ~auto_map_ptr()
  {
    typename std::map<K, V*>::reverse_iterator iter(this->rbegin()), last(this->rend());
    while (iter != last)
      delete iter++->second;
  }
};

} } }
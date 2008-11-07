#ifndef BOOST_CONSTANT_TIME_SIZE_LIST_H_
#define BOOST_CONSTANT_TIME_SIZE_LIST_H_
#include "boost/constant_time_size/list_constructors.hpp"
#include "boost/constant_time_size/audit.hpp"
#include "boost/constant_time_size/guard_container.hpp"
#include "boost/constant_time_size/performance_policy.hpp"
//#include "constant_time_size/wrapper_container.hpp"
//#include "constant_time_size/wrapper_forward_container.hpp"
//#include "constant_time_size/wrapper_reversible_container.hpp"
//#include "constant_time_size/wrapper_sequence.hpp"
//#include "constant_time_size/wrapper_front_insertion.hpp"
//#include "constant_time_size/wrapper_back_insertion.hpp"
#include "boost/constant_time_size/wrapper_list_stl.hpp"
#ifndef BOOST_HAS_STL_LIST_SIZE_CONSTANT_TIME
#include "boost/constant_time_size/wrapper_list.hpp"
#endif
#include <list>
namespace boost { 
namespace constant_time_size {
template < 
    class T,
    class Allocator = std::allocator<T>,
    class SizeTime =  size_constant_time
> struct make_list;

#ifdef BOOST_HAS_STL_LIST_SIZE_CONSTANT_TIME
template < 
    class T,
    class Allocator,
    class SizeTime
> struct make_list  {
    typedef extension_list<std::list<T, Allocator> > type;
};

#else
template < 
    class T,
    class Allocator
> struct make_list<T, Allocator, size_constant_time > {
    typedef wrapper_list<std::list<T, Allocator>, coherent >  type;
};

template < 
    class T,
    class Allocator
> struct make_list<T, Allocator, size_quasy_constant_time > {
    typedef wrapper_list<std::list<T, Allocator>, lazy >  type;
};

template < 
    class T,
    class Allocator
> struct make_list<T, Allocator, size_linear_time > {
    typedef extension_list<std::list<T, Allocator> > type;
};

template < 
    class T,
    class Allocator,
    template <typename> class Policy
> struct make_list<T, Allocator, size_performance_policy<Policy> > {
    typedef wrapper_list<std::list<T, Allocator>, Policy >  type;
};


#endif

template < 
    class T,
    class Allocator = std::allocator<T>,
    typename SizeTime = size_constant_time
#ifdef BOOST_HAS_STL_LIST_SIZE_CONSTANT_TIME
> class list : public extension_list<std::list<T, Allocator> >
#else
> class list : public make_list<T, Allocator, SizeTime>::type
#endif
{
    typedef typename make_list<T, Allocator, SizeTime>::type super_type;
public:
    typedef typename super_type::container                container;
    typedef typename  container::value_type               value_type;
    typedef typename  container::pointer                  pointer;
    typedef typename  container::const_pointer            const_pointer;
    typedef typename  container::reference                reference;
    typedef typename  container::const_reference          const_reference;
    typedef typename  container::iterator                 iterator;
    typedef typename  container::const_iterator           const_iterator;
    typedef typename  container::const_reverse_iterator   const_reverse_iterator;
    typedef typename  container::reverse_iterator         reverse_iterator;
    typedef typename  container::size_type                size_type;
    typedef typename  container::difference_type          difference_type;
    typedef typename  container::allocator_type           allocator_type;

    BOOST_CONSTANT_TIME_SIZE_CONSTRUCTORS(list, super_type, container)


};

}
}
  
#endif // BOOST_CONSTANT_TIME_SIZE_LIST_H_

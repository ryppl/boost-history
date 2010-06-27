//          Copyright Stefan Strasser 2010.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_INTRO_DETAIL_AUTO_BUCKETS_HPP
#define BOOST_INTRO_DETAIL_AUTO_BUCKETS_HPP

#include <boost/scoped_array.hpp>

namespace boost{
namespace intro{
namespace detail{


template<class Container,std::size_t Initial,std::size_t GrowthFactor=2>
class auto_buckets{
    typedef typename Container::bucket_traits bucket_traits;
    typedef typename Container::bucket_type bucket_type;
public:
    bucket_traits initial_traits(){
        return bucket_traits(this->initial,Initial);
    }
    void rehash(Container &container){
        //keep a maximum load factor of 1.0
        std::size_t const bucketscount=container.bucket_count();
        if(container.size() > bucketscount){
            std::size_t const newbucketscount=bucketscount * GrowthFactor;
            bucket_type *newbuckets=
                new bucket_type[newbucketscount];
            container.rehash(
                bucket_traits(newbuckets,newbucketscount)
            ); //nothrow
            allocated.reset(newbuckets);
        }
    }
private:
    bucket_type initial[Initial];
    scoped_array<bucket_type> allocated;
};

}
}
}

#endif

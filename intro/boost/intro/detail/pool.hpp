//          Copyright Stefan Strasser 2010.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_INTRO_DETAIL_POOL_HPP
#define BOOST_INTRO_DETAIL_POOL_HPP

#include <boost/intro/detail/auto_buffer.hpp>

namespace boost{
namespace intro{
namespace detail{

template<class T,std::size_t Embed,std::size_t EmbedPools,std::size_t MaxExponent=16>
class expo_pool{
public:
    ~expo_pool(){
        for(std::size_t c=0;c<pools.size();++c){
            std::size_t fullsize=capacity(c);
            vector_view<T> view(
                pools[c],
                c == pools.size() - 1 ? back_size : fullsize,
                capacity(c)
            );
            view.clear();
            ::operator delete(pools[c]);
        }
    }
    T *new_(){
        return new_(boost::in_place());
    }
    template<class InPlaceFactory>
    T *new_(InPlaceFactory const &factory){
        if(emb_pool.size() < emb_pool.capacity()){
            emb_pool.push_back(factory);
            return &emb_pool.back();
        }
        if(pools.empty()
            || last_pool().size() == last_pool().capacity()){
            pools.push_back(reinterpret_cast<T *>(
                ::operator new(sizeof(T)*capacity(pools.size()))
            ));
            back_size=0;
        }

        last_pool().push_back(factory);
        return &last_pool().back();
    }
private:
    static std::size_t capacity(std::size_t n){
        if(n > MaxExponent) return Embed << MaxExponent;
        else return Embed << n;
    }
    vector_view<T> last_pool(){
        return vector_view<T>(pools.back(),back_size,capacity(pools.size() - 1));
    }
    static_buffer<T,Embed> emb_pool;
    typedef auto_buffer<T *,EmbedPools> pools_type;
    pools_type pools;
    std::size_t back_size;
};

}
}
}


#endif

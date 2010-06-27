//          Copyright Stefan Strasser 2010.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_INTRO_DETAIL_MAP_INSERT_VIEW_HPP
#define BOOST_INTRO_DETAIL_MAP_INSERT_VIEW_HPP

#include <boost/intro/detail/immutable_iterator.hpp>
#include <boost/intro/detail/ebco_container.hpp>
#include <boost/swap.hpp>
#include <functional>
#include <utility>

namespace boost{
namespace intro{
namespace detail{

template<class Map,class Equal=std::equal_to<typename Map::key_type> >
class map_insert_view : public ebco_container<Equal>{
    typedef ebco_container<Equal> eq;
public:
    typedef Equal key_equal;
    typedef typename Map::key_type key_type;
    typedef typename Map::mapped_type mapped_type;
    typedef typename Map::value_type value_type;
    typedef typename Map::reference reference;
    typedef typename Map::const_reference const_reference;
    typedef typename Map::pointer pointer;
    typedef typename Map::const_pointer const_pointer;
    typedef immutable_iterator<pointer> iterator;
    typedef immutable_iterator<const_pointer> const_iterator;
    typedef typename Map::size_type size_type;
    typedef typename Map::difference_type difference_type;

    map_insert_view(Map &map,const_reference element,key_equal const &eq=key_equal())
        : eq(eq),map(map),element(element){}

    bool empty() const{ return false; }
    size_type size() const{ return map.size() + 1; }
    mapped_type &operator[](key_type const &x){
        if(key_eq()(x,element.first)) return element.second;
        else return map[x];
    }
    std::pair<iterator,bool> insert(const_reference x){
        if(key_eq()(x.first,element.first)) return std::make_pair(iterator(&element),false);
        else{
            std::pair<typename Map::iterator,bool> ret=map.insert(x);
            return std::make_pair(&*ret.first,ret.second);
        }
    }
    iterator insert(iterator pos,const_reference x){ return insert(x).first; }
    template<class InputIterator>
    void insert(InputIterator b,InputIterator e){
        for(;b != e;++b) insert(*b);
    }
    size_type erase(key_type const &x){
        BOOST_ASSERT(!key_eq()(x,element.first));
        return map.erase(x);
    }
    void swap(map_insert_view &rhs){
        map.swap(rhs.map);
        boost::swap(element,rhs.element);
        eq::swap(*this);
    }
    iterator find(key_type const &x){
        if(key_eq()(x,element.first)) return iterator(&element);
        else return &*map.find(x);
    }
    const_iterator find(key_type const &x) const{
        if(key_eq()(x,element.first)) return const_iterator(&element);
        else return &*map.find(x);
    }
    size_type count(key_type const &x) const{
        if(key_eq()(x,element.first)) return 1;
        else return map.count(x);
    }
    std::pair<iterator,iterator> equal_range(key_type const &x){
        if(key_eq()(x,element.first)) return std::make_pair(iterator(&element),iterator(&element));
        else{
            std::pair<typename Map::iterator,typename Map::iterator> ret=map.equal_range(x);
            return std::make_pair(&*ret.first,&*ret.second);
        }
    }
    std::pair<const_iterator,const_iterator> equal_range(key_type const &x) const{
        if(key_eq()(x,element.first)) return std::make_pair(iterator(&element),iterator(&element));
        else{
            std::pair<typename Map::const_iterator,typename Map::const_iterator> ret=map.equal_range(x);
            return std::make_pair(&*ret.first,&*ret.second);
        }
    }

    key_equal key_eq() const{ return eq::get(); }
private:
    Map &map;
    value_type element;
};

}
}
}


#endif

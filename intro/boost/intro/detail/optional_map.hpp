//          Copyright Stefan Strasser 2010.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_INTRO_DETAIL_OPTIONAL_MAP_HPP
#define BOOST_INTRO_DETAIL_OPTIONAL_MAP_HPP

#include <boost/optional/optional.hpp>
#include <boost/utility/in_place_factory.hpp>
#include <boost/ref.hpp>
#include <boost/assert.hpp>
#include <utility>


namespace boost{
namespace intro{
namespace detail{

template<class Map>
class optional_map{
public:
    typedef typename Map::key_type key_type;
    typedef typename Map::mapped_type mapped_type;
    typedef typename Map::value_type value_type;
    typedef typename Map::reference reference;
    typedef typename Map::const_reference const_reference;
    typedef typename Map::iterator iterator;
    typedef typename Map::const_iterator const_iterator;
    typedef typename Map::size_type size_type;
    typedef typename Map::difference_type difference_type;
    typedef typename Map::pointer pointer;
    typedef typename Map::const_pointer const_pointer;

    optional_map &operator=(Map const &rhs){
        this->map=in_place(cref(rhs));
        return *this;
    }
    Map &get(){
        if(!map) map=in_place();
        return *map;
    }
    iterator begin(){ return map ? map->begin() : empty_map.begin(); }
    const_iterator begin() const{ return map ? map->begin() : empty_map.begin(); }
    iterator end(){ return map ? map->end() : empty_map.end(); }
    const_iterator end() const{ return map ? map->end() : empty_map.end(); }
    bool empty() const{ return !map || map->empty(); }
    size_type size() const{ return map ? map->size() : 0; }
    size_type max_size() const{ return empty_map.max_size(); }
    mapped_type &operator[](key_type const &x){ return get()[x]; }
    std::pair<iterator,bool> insert(const_reference x){ return get().insert(x); }
    iterator insert(iterator pos,const_reference x){ return get().insert(pos,x); }
    template<class InputIterator>
    void insert(InputIterator b,InputIterator e){ get().insert(b.e); }
    void erase(iterator it){
        BOOST_ASSERT(map);
        map->erase(it);
    }
    size_type erase(key_type const &x){
        if(map) return map->erase(x); else return 0;
    }
    void erase(iterator b,iterator e){
        if(map) return map->erase(b,e);
        else BOOST_ASSERT(b == e);
    }
    void swap(optional_map &rhs){
        this->map.swap(rhs.map);
    }
    void clear(){ if(map) map->clear(); }
    iterator find(key_type const &x){ return map ? map->find(x) : empty_map.end(); }
    const_iterator find(key_type const &x) const{ return map ? map->find(x) : empty_map.end(); }
    size_type count(key_type const &x) const{ return map ? map->count(x) : 0; }
    std::pair<iterator,iterator> equal_range(key_type const &x){
        return map ? map->equal_range(x) : std::make_pair(empty_map.end(),empty_map.end());
    }
    std::pair<const_iterator,const_iterator> equal_range(key_type const &x) const{
        return map ? map->equal_range(x) : std::make_pair(empty_map.end(),empty_map.end());
    }
private:
    optional<Map> map;
    static Map empty_map;
};

template<class Map>
Map optional_map<Map>::empty_map;

}
}
}

#endif

//          Copyright Stefan Strasser 2010.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_INTRO_DETAIL_POOLED_MAP_HPP
#define BOOST_INTRO_DETAIL_POOLED_MAP_HPP

#include <boost/intro/detail/pooled_set.hpp>
#include <utility>

namespace boost{
namespace intro{
namespace detail{


template<class Key,class Mapped,std::size_t InitialCapacity,class Compare=std::less<Key> >
class pooled_map{
public:
    typedef Key key_type;
    typedef Mapped mapped_type;
    typedef std::pair<key_type,mapped_type> value_type;
    typedef value_type &reference;
    typedef value_type const &const_reference;
private:
    struct comparer{
        bool operator()(const_reference l,const_reference r) const{
            return Compare()(l.first,r.first);
        }
        bool operator()(const_reference l,key_type const &r) const{
            return Compare()(l.first,r);
        }
        bool operator()(key_type const &l,const_reference r) const{
            return Compare()(l,r.first);
        }
    };
    typedef pooled_set<
        value_type,
        InitialCapacity,
        comparer
    > set_type;
public:
    typedef typename set_type::size_type size_type;
    typedef typename set_type::difference_type difference_type;
    typedef value_type *pointer;
    typedef value_type const *const_pointer;
    typedef immutable_iterator<pointer> iterator;
    typedef immutable_iterator<const_pointer> const_iterator;

    iterator begin(){ return set.begin(); }
    const_iterator begin() const{ return set.begin(); }
    iterator end(){ return set.end(); }
    const_iterator end() const{ return set.end(); }
    std::pair<iterator,bool> insert(const_reference x){
        std::pair<typename set_type::iterator,bool> ret=this->set.insert(x);
        return std::make_pair(iterator(&*ret.first),ret.second);
    }
    size_type size() const{ return set.size(); }
    iterator find(key_type const &x){
        return set.find(x,comparer());
    }
    const_iterator find(key_type const &x) const{
        return set.find(x,comparer());
    }
private:
    set_type set;
};

}
}
}


#endif

//          Copyright Stefan Strasser 2010.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_INTRO_DETAIL_SET_INSERT_VIEW_HPP
#define BOOST_INTRO_DETAIL_SET_INSERT_VIEW_HPP

#include <boost/intro/detail/immutable_iterator.hpp>
#include <boost/intro/detail/ebco_container.hpp>
#include <boost/swap.hpp>
#include <functional>
#include <utility>

namespace boost{
namespace intro{
namespace detail{

template<class Set,class Equal=std::equal_to<typename Set::key_type> >
class set_insert_view : public ebco_container<Equal>{
    typedef ebco_container<Equal> eq;
public:
    typedef Equal key_equal;
    typedef typename Set::key_type key_type;
    typedef typename Set::value_type value_type;
    typedef typename Set::reference reference;
    typedef typename Set::const_reference const_reference;
    typedef typename Set::pointer pointer;
    typedef typename Set::const_pointer const_pointer;
    typedef immutable_iterator<pointer> iterator;
    typedef immutable_iterator<const_pointer> const_iterator;
    typedef typename Set::size_type size_type;
    typedef typename Set::difference_type difference_type;

    set_insert_view(Set &set,const_reference element,key_equal const &eq_=key_equal())
        : eq(eq_),set(set),element(element){}

    bool empty() const{ return false; }
    size_type size() const{ return set.size() + 1; }
    std::pair<iterator,bool> insert(const_reference x){
        if(key_eq()(x,element)) return std::make_pair(iterator(&element),false);
        else{
            std::pair<typename Set::iterator,bool> ret=set.insert(x);
            return std::make_pair(&*ret.first,ret.second);
        }
    }
    iterator insert(iterator pos,const_reference x){ return insert(x).first; }
    template<class InputIterator>
    void insert(InputIterator b,InputIterator e){
        for(;b != e;++b) insert(*b);
    }
    size_type erase(const_reference x){
        BOOST_ASSERT(!key_eq()(x,element));
        return set.erase(x);
    }
    void swap(set_insert_view &rhs){
        set.swap(rhs.set);
        boost::swap(element,rhs.element);
        eq::swap(*this);
    }
    iterator find(const_reference x){
        if(key_eq()(x,element)) return iterator(&element);
        else return &*set.find(x);
    }
    const_iterator find(const_reference x) const{
        if(key_eq()(x,element)) return const_iterator(&element);
        else return &*set.find(x);
    }
    size_type count(const_reference x) const{
        if(key_eq()(x,element)) return 1;
        else return set.count(x);
    }
    std::pair<iterator,iterator> equal_range(const_reference x){
        if(key_eq()(x,element)) return std::make_pair(iterator(&element),iterator(&element));
        else{
            std::pair<typename Set::iterator,typename Set::iterator> ret=set.equal_range(x);
            return std::make_pair(&*ret.first,&*ret.second);
        }
    }
    std::pair<const_iterator,const_iterator> equal_range(const_reference x) const{
        if(key_eq()(x,element)) return std::make_pair(iterator(&element),iterator(&element));
        else{
            std::pair<typename Set::const_iterator,typename Set::const_iterator> ret=set.equal_range(x);
            return std::make_pair(&*ret.first,&*ret.second);
        }
    }

    key_equal key_eq() const{ return eq::get(); }
private:
    Set &set;
    value_type element;
};

}
}
}


#endif

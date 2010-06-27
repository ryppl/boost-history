//          Copyright Stefan Strasser 2010.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_INTRO_DETAIL_OPTIONAL_SET_HPP
#define BOOST_INTRO_DETAIL_OPTIONAL_SET_HPP

#include <boost/optional/optional.hpp>
#include <boost/utility/in_place_factory.hpp>
#include <boost/ref.hpp>
#include <boost/assert.hpp>
#include <utility>


namespace boost{
namespace intro{
namespace detail{

template<class Set>
class optional_set{
public:
    typedef typename Set::key_type key_type;
    typedef typename Set::value_type value_type;
    typedef typename Set::reference reference;
    typedef typename Set::const_reference const_reference;
    typedef typename Set::iterator iterator;
    typedef typename Set::const_iterator const_iterator;
    typedef typename Set::size_type size_type;
    typedef typename Set::difference_type difference_type;
    typedef typename Set::pointer pointer;
    typedef typename Set::const_pointer const_pointer;

    optional_set &operator=(Set const &rhs){
        this->set=in_place(cref(rhs));
        return *this;
    }
    Set &get(){
        if(!set) set=in_place();
        return *set;
    }
    iterator begin(){ return set ? set->begin() : empty_set.begin(); }
    const_iterator begin() const{ if(set) return set->begin(); else return empty_set.begin(); }
    iterator end(){ return set ? set->end() : empty_set.end(); }
    const_iterator end() const{ if(set) return set->end(); else return empty_set.end(); }
    bool empty() const{ return !set || set->empty(); }
    size_type size() const{ return set ? set->size() : 0; }
    size_type max_size() const{ return empty_set.max_size(); }
    std::pair<iterator,bool> insert(const_reference x){ return get().insert(x); }
    iterator insert(iterator pos,const_reference x){ return get().insert(pos,x); }
    template<class InputIterator>
    void insert(InputIterator b,InputIterator e){ get().insert(b.e); }
    void erase(iterator it){
        BOOST_ASSERT(set);
        set->erase(it);
    }
    size_type erase(const_reference x){
        if(set) return set->erase(x); else return 0;
    }
    void erase(iterator b,iterator e){
        if(set) return set->erase(b,e);
        else BOOST_ASSERT(b == e);
    }
    void swap(optional_set &rhs){
        this->set.swap(rhs.set);
    }
    void clear(){ if(set) set->clear(); }
    iterator find(const_reference x){ return set ? set->find(x) : empty_set.end(); }
    const_iterator find(const_reference x) const{ if(set) return set->find(x); else return empty_set.end(); }
    size_type count(const_reference x) const{ return set ? set->count(x) : 0; }
    std::pair<iterator,iterator> equal_range(const_reference x){
        return set ? set->equal_range(x) : std::make_pair(empty_set.end(),empty_set.end());
    }
    std::pair<const_iterator,const_iterator> equal_range(const_reference x) const{
        if(set) return set->equal_range(x); else return std::make_pair(empty_set.end(),empty_set.end());
    }
private:
    optional<Set> set;
    static Set empty_set;
};

template<class Set>
Set optional_set<Set>::empty_set;

}
}
}

#endif

//          Copyright Stefan Strasser 2010.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_INTRO_DETAIL_POOLED_SET_HPP
#define BOOST_INTRO_DETAIL_POOLED_SET_HPP

#include <utility>
#include <boost/intrusive/set.hpp>
#include <boost/utility/in_place_factory.hpp>
#include <boost/intro/detail/pool.hpp>
#include <boost/intro/detail/immutable_iterator.hpp>
#include <functional>

namespace boost{
namespace intro{
namespace detail{


template<class T,std::size_t InitialCapacity,class Compare=std::less<T> >
class pooled_set{
    struct node;
    typedef intrusive::set_base_hook<
        intrusive::link_mode<intrusive::normal_link>
    > hook;
    typedef intrusive::set<
        node,
        intrusive::base_hook<hook>
    > set_type;

public:
    typedef T key_type;
    typedef key_type value_type;
    typedef value_type &reference;
    typedef value_type const &const_reference;
    typedef typename set_type::size_type size_type;
    typedef typename set_type::difference_type difference_type;
    typedef value_type *pointer;
    typedef value_type const *const_pointer;
    typedef immutable_iterator<pointer> iterator;
    typedef immutable_iterator<const_pointer> const_iterator;

    iterator begin(){
        if(empty()) return end();
        else return iterator(&*set.begin());
    }
    const_iterator begin() const{
        if(empty()) return end();
        else return const_iterator(&*set.begin());
    }
    iterator end(){ return iterator(0); }
    const_iterator end() const{ return const_iterator(0); }
    bool empty() const{ return set.empty(); }
    std::pair<iterator,bool> insert(const_reference x){
        typename set_type::insert_commit_data commit_data;
        std::pair<typename set_type::iterator,bool> ret=
            this->set.insert_check(x,
            node::less<Compare>(Compare()),commit_data);
        if(ret.second){
            ret.first=this->set.insert_commit(*this->node_pool.new_(in_place(x)),commit_data);
            return std::make_pair(&ret.first->key,true);
        }else return std::make_pair(&ret.first->key,false);
    }
    size_type size() const{ return set.size(); }
    template<class Key,class KeyValueCompare>
    iterator find(Key const &key,KeyValueCompare compare){
        typename set_type::iterator it=this->set.find(key,node::less<KeyValueCompare>(compare));
        if(it == this->set.end()) return iterator(0); //TODO use real iterator
        else return iterator(&it->key);
    }
    template<class Key,class KeyValueCompare>
    const_iterator find(Key const &key,KeyValueCompare compare) const{
        typename set_type::const_iterator it=this->set.find(key,hash,node::less<KeyValueCompare>(compare));
        if(it == this->set.end()) return const_iterator(0);
        else return const_iterator(&it->key);
    }
private:
    struct node : hook{
        explicit node(const_reference x) : key(x){}
        bool operator<(node const &o) const{
            return Compare()(this->key,o.key);
        }
        template<class Base>
        struct less{
            explicit less(Base const &base) : base(base){}
            template<class T>
            bool operator()(T const &o1,node const &o2) const{
                return base(o1,o2.key);
            }
            template<class T>
            bool operator()(node const &o1,T const &o2) const{
                return base(o1.key,o2);
            }
        private:
            Base const base;
        };
        value_type key;
    };
    expo_pool<node,InitialCapacity,2> node_pool;
    set_type set;
};

}
}
}


#endif

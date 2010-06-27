//          Copyright Stefan Strasser 2010.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_INTRO_DETAIL_POOLED_UNORDERED_SET_HPP
#define BOOST_INTRO_DETAIL_POOLED_UNORDERED_SET_HPP

#include <utility>
#include <boost/intrusive/unordered_set.hpp>
#include <boost/functional/hash.hpp>
#include <boost/utility/in_place_factory.hpp>
#include <boost/intro/detail/pool.hpp>
#include <boost/intro/detail/auto_buckets.hpp>
#include <boost/intro/detail/immutable_iterator.hpp>

namespace boost{
namespace intro{
namespace detail{


template<class T,std::size_t InitialCapacity,class Hash=boost::hash<T>,class Equal=std::equal_to<T> >
class pooled_unordered_set{
    struct node;
    typedef intrusive::unordered_set_base_hook<
        intrusive::link_mode<intrusive::normal_link>
    > hook;
        
    typedef intrusive::unordered_set<
        node,
        intrusive::base_hook<hook>,
        intrusive::power_2_buckets<true>
    > set_type;

public:
    pooled_unordered_set()
        : set(buckets.initial_traits()){}

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
            this->set.insert_check(x,Hash(),
            typename node::template equal_to<Equal>(Equal()),commit_data);
        if(ret.second){
            ret.first=this->set.insert_commit(*this->node_pool.new_(in_place(x)),commit_data);
            this->buckets.rehash(this->set);
            return std::make_pair(&ret.first->key,true);
        }else return std::make_pair(&ret.first->key,false);
    }
    //erased nodes are NOT reused! erase() is only used on
    //exception at the moment, shortly before the entire map
    //is destroyed
    size_type erase(const_reference x){
        return this->set.erase(x,Hash(),typename node::template equal_to<Equal>(Equal()));
    }
    size_type size() const{ return set.size(); }
    template<class Key,class KeyHasher,class KeyValueEqual>
    iterator find(Key const &key,KeyHasher hash,KeyValueEqual equal){
        typename set_type::iterator it=this->set.find(key,hash,typename node::template equal_to<KeyValueEqual>(equal));
        if(it == this->set.end()) return iterator(0); //TODO use real iterator
        else return iterator(&it->key);
    }
    template<class Key,class KeyHasher,class KeyValueEqual>
    const_iterator find(Key const &key,KeyHasher hash,KeyValueEqual equal) const{
        typename set_type::const_iterator it=this->set.find(key,hash,typename node::template equal_to<KeyValueEqual>(equal));
        if(it == this->set.end()) return const_iterator(0);
        else return const_iterator(&it->key);
    }
private:
    struct node : hook{
        explicit node(const_reference x) : key(x){}
        friend std::size_t hash_value(node const &n){
            return Hash()(n.key);
        }
        bool operator==(node const &o) const{
            return Equal()(this->key,o.key);
        }
        template<class Base>
        struct equal_to{
            explicit equal_to(Base const &base) : base(base){}
            template<class K>
            bool operator()(K const &o1,node const &o2) const{
                return base(o1,o2.key);
            }
            template<class K>
            bool operator()(node const &o1,K const &o2) const{
                return base(o1.key,o2);
            }
        private:
            Base const base;
        };
        value_type key;
    };
    expo_pool<node,InitialCapacity,2> node_pool;
    auto_buckets<set_type,InitialCapacity> buckets;
    set_type set;
};

}
}
}


#endif

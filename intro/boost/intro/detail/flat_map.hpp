//          Copyright Stefan Strasser 2010.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_INTRO_DETAIL_FLAT_MAP_HPP
#define BOOST_INTRO_DETAIL_FLAT_MAP_HPP

#include <vector>
#include <utility>
#include <algorithm>

namespace boost{
namespace intro{
namespace detail{

template<class Iterator,class T,class Compare>
Iterator find(Iterator it,Iterator end,T const &value,Compare const &comp){
        it=std::lower_bound(it,end,value,comp);
        if(it == end || comp(value,*it)) return end;
        else return it;
}

template<class Key,class Mapped,class Compare=std::less<Key> >
class flat_map{
public:
    typedef Key key_type;
    typedef Mapped mapped_type;
    typedef std::pair<key_type,mapped_type> value_type;
    typedef std::vector<value_type> vector_type;
    typedef typename vector_type::iterator iterator;
    typedef typename vector_type::const_iterator const_iterator;
    typedef value_type const &const_reference;

    iterator begin(){ return vector.begin(); }
    const_iterator begin() const{ return vector.begin(); }
    iterator end(){ return vector.end(); }
    const_iterator end() const{ return vector.end(); }
    std::pair<iterator,bool>
    insert(const_reference x){
        value_comparer comp;
        iterator it=std::lower_bound(
            vector.begin(),
            vector.end(),
            x,
            comp);
        if(it == vector.end()){
            vector.push_back(x);
            return std::make_pair(vector.end()-1,true);
        }else if(!comp(x,*it)) return std::make_pair(it,false);
        else{
            std::size_t pos=it-vector.begin();
            vector.push_back(vector.back());
            it=vector.begin()+pos;
            std::copy_backward(
                it,
                vector.end()-2, //size() is at least 2
                vector.end()-1);
            *it=x;
            return std::make_pair(it,true);
        }
    }
    iterator find(key_type const &key){
        return detail::find(
            vector.begin(),
            vector.end(),
            std::make_pair(key,mapped_type()),
            value_comparer()
        );
    }
    const_iterator find(key_type const &key) const{
        return detail::find(
            vector.begin(),
            vector.end(),
            std::make_pair(key,mapped_type()),
            value_comparer()
        );
    }
private:
    struct value_comparer{
        bool operator()(const_reference l,const_reference r) const{
            return Compare()(l.first,r.first);
        }
    };

    vector_type vector;
};

}
}
}


#endif

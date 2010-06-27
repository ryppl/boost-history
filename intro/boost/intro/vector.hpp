//          Copyright Stefan Strasser 2010.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_INTRO_VECTOR_HPP
#define BOOST_INTRO_VECTOR_HPP

#include <vector>
#include <boost/intro/container.hpp>

namespace boost{
namespace intro{

template<class F,class Data,class T,class Allocator,class Semantics>
void apply_recursive(F const &f,Data &data,std::vector<T,Allocator> &v,Semantics){
    intro::container_apply_recursive(f,data,v,Semantics());
}
template<class F,class Data,class T,class Allocator,class Semantics>
void apply_recursive(F const &f,Data &data,std::vector<T,Allocator> const &v,Semantics){
    intro::container_apply_recursive(f,data,v,Semantics());
}

template<class Archive,class Data,class T,class Allocator,class Semantics>
void serialize(Archive &ar,Data &data,std::vector<T,Allocator> const &v,Semantics){
    intro::container_serialize(ar,data,v,Semantics());
}

template<class Map,class T,class Allocator,class Semantics>
void move(Map &map,std::vector<T,Allocator> &src,std::vector<T,Allocator> &dest,Semantics){
    intro::container_move(map,src,dest,Semantics());
}

template<class T,class Allocator>
void copy_shallow(std::vector<T,Allocator> const &src,std::vector<T,Allocator> &dest){
    intro::container_copy_shallow(src,dest);
}

template<class T,class Allocator,class Transformation>
void print(std::vector<T,Allocator> const &t,std::ostream &out,Transformation const &transform){
    intro::container_print(t,out,transform);
}

}
}


#endif

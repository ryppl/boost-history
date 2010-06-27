#ifndef BOOST_INTRO_CONTAINER_HPP
#define BOOST_INTRO_CONTAINER_HPP


#include <boost/mpl/if.hpp>
#include <boost/type_traits/is_const.hpp>
#include <boost/intro/detail/indent_streambuf.hpp>
#include <boost/intro/serialize.hpp>
#include <boost/intro/apply_recursive.hpp>
#include <ostream>

namespace boost{
namespace intro{


template<class F,class Data,class Container,class Semantics>
void container_apply_recursive(F const &f,Data &data,Container &c,Semantics){
    typedef typename mpl::if_<
        is_const<Container>,
        typename Container::const_iterator,
        typename Container::iterator>::type iterator;
    for(iterator it=c.begin();it != c.end();++it){
        intro::apply_member_recursive(f,data,*it,typename Semantics::indir_semantics());
    }
}

template<class Archive,class Data,class Container,class Semantics>
void container_serialize(Archive &ar,Data &data,Container const &c,Semantics){
    ar << c.size();
    for(typename Container::const_iterator it=c.begin();it != c.end();++it){
        intro::serialize_member(ar,data,*it,typename Semantics::indir_semantics());
    }
}

template<class Map,class Container,class Semantics>
void container_move(Map &,Container &src,Container &dest,Semantics){
    new (&dest) Container();
    src.swap(dest);
    src.~Container();
}

template<class Container>
void container_copy_shallow(Container const &src,Container &dest){
    new (&dest) Container(src);
}

template<class Container,class Transformation>
void container_print(Container const &c,std::ostream &out,Transformation const &transform){
    out << '{';
    detail::indent_streambuf indent(*out.rdbuf(),false);
    std::ostream indent_out(&indent);
    for(typename Container::const_iterator it=c.begin();it != c.end();){
        print(*it,indent_out,transform);
        if(++it != c.end()) indent_out << ", ";
    }
    out << '}';
}


}
}


#endif

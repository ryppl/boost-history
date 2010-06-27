//          Copyright Stefan Strasser 2010.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_INTRO_RESET_SHARED_HPP
#define BOOST_INTRO_RESET_SHARED_HPP

#include <boost/intro/apply_recursive.hpp>
#include <boost/intro/dispatch_polymorphic.hpp>
#include <boost/bind.hpp>

namespace boost{
namespace intro{

template<class Map>
class reset_shared_data{
public:
    explicit reset_shared_data(Map const &map) : map(map){}
    template<class T>
    T *translate(T *t) const{
        typename Map::const_iterator it=map.find(t);
        if(it == map.end()) return t;
        else return static_cast<T *>(it->second);
    }
private:
    Map const &map;
};

namespace detail{

template<class Data>
struct shared_resetter{
    explicit shared_resetter(Data const &data) : data(data){}
    template<class T,class Semantics>
    void operator()(T &t,Semantics) const{}
    template<class T,class Semantics>
    void operator()(T *&t,Semantics) const{
        if(t) t=&this->get(*t,Semantics());
    }
    template<class T,class Semantics>
    void operator()(T * const &t,Semantics) const{
        if(t) const_cast<T *&>(t)=&this->get(*t,Semantics());
    }
    template<class T,class Semantics>
    void operator()(reference<T> &t,Semantics) const{
        T &tmp=this->get(t.get(),Semantics());
        t.~reference<T>();
        new (&t) reference<T>(tmp);
    }
    template<class T,class Semantics>
    void operator()(reference<T> const &t,Semantics) const{
        T &tmp=this->get(t.get(),Semantics());
        t.~reference<T>();
        new (&t) reference<T>(tmp);
    }
    //TODO operator~
private:
    template<class T,class Semantics>
    T &get(T &t,Semantics) const{
        return this->get(t,typename mpl::has_key<typename Semantics::indir_semantics::set,polymorphic>::type(),
            typename mpl::has_key<typename Semantics::indir_semantics::set,shared>::type());
    }
    template<class T>
    T &get(T &t,mpl::false_ poly,mpl::true_ shared) const{
        return *data.translate(&t);
    }
    template<class T>
    struct getter{
        typedef T &result_type;
        template<class Derived>
        T &operator()(shared_resetter const &that,Derived &dt) const{
            return that.get(dt,mpl::false_(),mpl::true_());
        }
    };
    template<class T>
    T &get(T &t,mpl::true_ poly,mpl::true_ shared) const{
        //TODO optimization: the same object is dispatched shortly
        //thereafter by apply_recursive
        return dispatch_polymorphic(boost::bind(getter<T>(),cref(*this),_1),t);
    }
    template<class T,bool Poly>
    T &get(T &t,mpl::bool_<Poly>,mpl::false_ shared) const{
        return t;
    }
    Data const &data;
};

}

template<class Data,class T>
void reset_shared(Data const &data,T &t){
    apply_recursive(detail::shared_resetter<Data>(data),t);
}

}
}


#endif

//          Copyright Stefan Strasser 2010.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_INTRO_MOVE_HPP
#define BOOST_INTRO_MOVE_HPP

#include <boost/intro/apply_members_binary.hpp>
#include <boost/intro/reset_shared.hpp>
#include <boost/intro/detail/map_insert_view.hpp>
#include <boost/intro/detail/pooled_unordered_map.hpp>
#include <boost/intro/detail/optional_map.hpp>
#include <boost/intro/detail/ebco_container.hpp>
#include <boost/mpl/has_key.hpp>
#include <boost/static_assert.hpp>
#include <boost/type_traits/is_class.hpp>
#include <boost/assert.hpp>

namespace boost{
namespace intro{

template<class Map>
class move_data{
public:
    explicit move_data(Map &map) : map(map){}
    template<class T>
    void register_object(T *src,T *dest){
        BOOST_VERIFY( map.insert(std::make_pair(src,dest)).second );
    }
private:
    Map &map;
};

namespace detail{

class null_move_data{
public:
    template<class T>
    void register_object(T *,T *){}
};

class undo_mover;

template<class Data>
class mover{
public:
    explicit mover(Data &data) : data(data){}
    template<class T,class Semantics>
    void operator()(T &src,T &dest,Semantics) const{
        BOOST_ASSERT((!mpl::has_key<typename Semantics::set,polymorphic>::type::value));
        this->insert_shared(src,dest,typename mpl::has_key<typename Semantics::set,shared>::type());
        this->operator()(src,dest,Semantics(),typename is_class<T>::type());
    }
    template<class T,class Semantics>
    void operator()(reference<T> &src,reference<T> &dest,Semantics) const{
        this->insert_shared(src,dest,typename mpl::has_key<typename Semantics::set,shared>::type());
        new (&dest) reference<T>(src.get());
        src.~reference<T>();
    }
    undo_mover operator ~() const;
private:
    template<class T>
    void insert_shared(T &src,T &dest,mpl::true_ shared) const{
        data.register_object(&src,&dest);
    }
    template<class T>
    void insert_shared(T &src,T &dest,mpl::false_ shared) const{}
    template<class T,class Semantics>
    void operator()(T &src,T &dest,Semantics,mpl::true_ iscl) const{
        move(data,src,dest,Semantics());
    }
    template<class T,class Semantics>
    void operator()(T &src,T &dest,Semantics,mpl::false_ iscl) const{
        dest=src;
        src=T();
    }
    Data &data;
};

class undo_mover{
public:
    undo_mover() : mover_(data){}
    template<class T,class Semantics>
    void operator()(T &src,T &dest,Semantics) const{
        mover_(dest,src,Semantics());
    }
private:
    null_move_data data;
    mover<null_move_data> mover_;
};

template<class Data>
undo_mover mover<Data>::operator~() const{ return undo_mover(); }

}

template<class Data,class T,class Semantics>
void move(Data &data,T &src,T &dest,Semantics){
    apply_members_binary(detail::mover<Data>(data),src,dest);
}

template<class Data,class T>
void move(Data &data,T &src,T &dest){
    move(data,src,dest,typename default_semantics<T>::type());
}

namespace detail{

template<class T,class Semantics>
void move(T &src,T &dest,Semantics,mpl::true_ shared){
    typedef detail::optional_map<detail::pooled_unordered_map<void *,void *,4> > omap_type;
    omap_type omap;
    move_data<omap_type> data(omap);
    using intro::move;
    move(data,src,dest);
    try{
        typedef detail::map_insert_view<omap_type> map_type;
        map_type map(omap,std::make_pair(&src,&dest));
        reset_shared_data<map_type> resdata(map);
        reset_shared(resdata,dest);
    }catch(...){
        null_move_data ndata;
        move(ndata,dest,src);
        throw;
    }
}

template<class T,class Semantics>
void move(T &src,T &dest,Semantics,mpl::false_ shared){
    typedef detail::optional_map<detail::pooled_unordered_map<void *,void *,4> > map_type;
    map_type map;
    move_data<map_type> data(map);
    using intro::move;
    move(data,src,dest,Semantics());
    try{
        reset_shared_data<map_type> resdata(map);
        reset_shared(resdata,dest);
    }catch(...){
        null_move_data ndata;
        move(ndata,dest,src);
        throw;
    }
}

}


template<class T,class Semantics>
void move(T &src,T &dest,Semantics){
    detail::move(src,dest,Semantics(),typename mpl::has_key<typename Semantics::set,shared>::type());    
}


template<class T>
void move(T &src,T &dest){
    move(src,dest,typename default_semantics<T>::type());
}


}
}

#endif

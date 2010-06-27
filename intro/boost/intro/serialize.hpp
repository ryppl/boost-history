#ifndef BOOST_INTRO_SERIALIZE_HPP
#define BOOST_INTRO_SERIALIZE_HPP

#include <boost/intro/apply.hpp>
#include <boost/intro/dispatch_polymorphic.hpp>
#include <boost/intro/detail/pooled_unordered_map.hpp>
#include <boost/intro/detail/optional_map.hpp>
#include <boost/intro/detail/map_insert_view.hpp>
#include <boost/intro/detail/pooled_map.hpp>
#include <cstring>

namespace boost{
namespace intro{

template<class ObjectMap,class TypeMap>
class serialize_data{
public:
    serialize_data(ObjectMap &omap,TypeMap &tmap) : omap(omap),tmap(tmap){}
    template<class T>
    std::pair<unsigned int,bool> register_object(T const &t){
        std::pair<typename ObjectMap::iterator,bool> ret=
            omap.insert(std::make_pair(&t,omap.size()));
        return std::make_pair(ret.first->second,ret.second);
    }
    template<class T>
    std::pair<unsigned int,bool> register_type(){
        std::pair<typename TypeMap::iterator,bool> ret=
            tmap.insert(std::make_pair(&typeid(T),tmap.size()));
        return std::make_pair(ret.first->second,ret.second);
    }
private:
    ObjectMap &omap;
    TypeMap &tmap;
};


template<class Archive,class Data,class T,class Semantics>
void serialize_member(Archive &ar,Data &data,T const &t,Semantics){
    detail::serialize_member(ar,data,t,Semantics(),
        typename mpl::has_key<typename Semantics::set,polymorphic>::type(),
        typename mpl::has_key<typename Semantics::set,shared>::type());
}

namespace detail{

template<class Archive,class Data,class T,class Semantics,bool Shared>
void serialize_member(Archive &ar,Data &data,T const &t,Semantics,mpl::true_ poly,mpl::bool_<Shared>){
    dispatch_polymorphic(
        bind(
            detail::member_serializer<Archive,Data>(ar,data),
            _1,
            Semantics(),
            mpl::false_(),
            mpl::bool_<Shared>()
        ),t);
}
template<class Archive,class Data,class T,class Semantics>
void serialize_member(Archive &ar,Data &data,T const &t,Semantics,mpl::false_ poly,mpl::true_ shared_){
    std::pair<unsigned int,bool> ret=data.register_object(t);
    ar << ret.first;
    if(ret.second){
        detail::serialize_member(ar,data,t,Semantics(),poly,mpl::false_());
    }
}
template<class Archive,class Data,class T,class Semantics>
void serialize_member(Archive &ar,Data &data,T const &t,Semantics,mpl::false_ poly,mpl::false_ shared_){
    serialize(ar,data,t,Semantics());
}


template<class Archive,class Data>
struct member_serializer{
    typedef void result_type;
    member_serializer(Archive &ar,Data &data)
        : ar(ar),data(data){}
    template<class T,class Semantics>
    void operator()(T const &t,Semantics) const{
        intro::serialize_member(ar,data,t,Semantics());
    }
    template<class T,class Semantics,bool Shared>
    void operator()(T const &t,Semantics,mpl::false_ poly,mpl::bool_<Shared> sha) const{
        //called by polymorphic dispatch
        std::pair<unsigned int,bool> ret=data.register_type<T>();
        ar << ret.first;
        if(ret.second){
            char const *name=typeid(T).name();
            std::size_t const length=std::strlen(name);
            ar << length;
            ar.save_binary(name,length);
        }
        detail::serialize_member(ar,data,t,Semantics(),poly,sha);
    }
private:
    Archive &ar;
    Data &data;
};

template<class Archive,class Data,class T>
void serialize_class(Archive &ar,Data &data,T const &t,mpl::true_ is_class_){
    using intro::apply;
    apply(detail::member_serializer<Archive,Data>(ar,data),t);
}

template<class Archive,class Data,class T>
void serialize_class(Archive &ar,Data &data,T const &t,mpl::false_ is_class_){
    ar << t;
}


template<class Archive,class Data,class T,class Semantics>
void serialize(Archive &ar,Data &data,T * const &t,Semantics){
    //TODO optimization: if the pointer is shared, whether it is set or not
    //could be encoded in the object id
    if(t){
        ar << true;
        intro::serialize_member(ar,data,*t,typename Semantics::indir_semantics());
    }else ar << false;
}

template<class Archive,class Data,class T,class Semantics>
void serialize(Archive &ar,Data &data,reference<T> const &t,Semantics){
    intro::serialize_member(ar,data,t.get(),typename Semantics::indir_semantics());
}

struct type_info_less{
    bool operator()(std::type_info const *t1,std::type_info const *t2) const{
#ifdef BOOST_MSVC
#pragma warning(push)
#pragma warning(disable:4800)
#endif
        return t1->before(*t2);
#ifdef BOOST_MSVC
#pragma warning(pop)
#endif
    }
};

template<class Archive,class T,class Semantics>
void serialize(Archive &ar,T const &t,Semantics,mpl::false_ shared){
    typedef detail::optional_map<detail::pooled_unordered_map<void const *,unsigned int,4> > omap_type;
    typedef detail::optional_map<detail::pooled_map<std::type_info const *,unsigned int,1,type_info_less> > tmap_type;
    omap_type omap; tmap_type tmap;
    serialize_data<omap_type,tmap_type> data(omap,tmap);
    using intro::serialize;
    serialize(ar,data,t,Semantics());
}

template<class Archive,class T,class Semantics>
void serialize(Archive &ar,T const &t,Semantics,mpl::true_ shared){
    typedef detail::optional_map<detail::pooled_unordered_map<void const *,unsigned int,4> > omap_type;
    typedef detail::optional_map<detail::pooled_map<std::type_info const *,unsigned int,1,type_info_less> > tmap_type;
    omap_type omap; tmap_type tmap;
    typedef detail::map_insert_view<omap_type> omap_view_type;
    omap_view_type omap_view(omap,std::make_pair(&t,0));
    serialize_data<omap_view_type,tmap_type> data(omap_view,tmap);

    using intro::serialize;
    serialize(ar,data,t,Semantics());
}

}

template<class Archive,class Data,class T,class Semantics>
void serialize(Archive &ar,Data &data,T const &t,Semantics){
    detail::serialize_class(ar,data,t,typename is_class<T>::type());
}


template<class Archive,class T,class Semantics>
void serialize(Archive &ar,T const &t,Semantics){
    detail::serialize(ar,t,Semantics(),typename mpl::has_key<typename Semantics::set,shared>::type());
}

template<class Archive,class T>
void serialize(Archive &ar,T const &t){
    serialize(ar,t,typename default_semantics<T>::type());
}

}
}


#endif

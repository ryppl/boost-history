//          Copyright Stefan Strasser 2010.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_INTRO_APPLY_RECURSIVE_HPP
#define BOOST_INTRO_APPLY_RECURSIVE_HPP

#include <boost/intro/apply_members.hpp>
#include <boost/intro/dispatch_polymorphic.hpp>
#include <boost/intro/detail/pooled_unordered_set.hpp>
#include <boost/intro/detail/optional_set.hpp>
#include <boost/intro/detail/set_insert_view.hpp>

namespace boost{
namespace intro{

template<class Set>
class apply_recursive_data{
public:
    explicit apply_recursive_data(Set &set) : applied(set){}
    template<class T>
    bool register_applied_object(T const &t){
        return applied.insert(&t).second;
    }
    template<class T>
    bool deregister_applied_object(T const &t){
        return applied.erase(&t) > 0;
    }
private:
    Set &applied;
};

namespace detail{

template<class Base>
class apply_recursive_data_invert_view{
public:
    explicit apply_recursive_data_invert_view(Base &base) : base(base){}
    template<class T>
    bool register_applied_object(T const &t){
        return base.unregister_applied_object(t);
    }
    template<class T>
    bool deregister_applied_object(T const &t){
        return base.register_applied_object(t);
    }
private:
    Base &base;
};

}

namespace detail{

template<class,class>
struct recursive_member_applier;

template<class F,class Data,class T,class Semantics>
void apply_member_recursive(F const &f,Data &data,T &t,Semantics,mpl::false_ poly,mpl::false_ shared_){
    f(t,Semantics()); //TODO sollte das bei undo-en nach apply_recursive passieren?
    apply_recursive(f,data,t,Semantics());
}

template<class F,class Data,class T,class Semantics>
void apply_member_recursive(F const &f,Data &data,T &t,Semantics,mpl::false_ poly,mpl::true_ shared_){
    if(data.register_applied_object(t))
        detail::apply_member_recursive(f,data,t,Semantics(),poly,mpl::false_());
}
template<class F,class Data,class T,class Semantics,bool Shared>
void apply_member_recursive(F const &f,Data &data,T &t,Semantics,mpl::true_ poly,mpl::bool_<Shared>){
    dispatch_polymorphic(
        boost::bind(
            detail::recursive_member_applier<F,Data>(f,data),
            _1,
            Semantics(),
            mpl::false_(),
            mpl::bool_<Shared>()
        ),t);
}


}

template<class F,class Data,class T,class Semantics>
void apply_member_recursive(F const &f,Data &data,T &t,Semantics){
    detail::apply_member_recursive(f,data,t,Semantics(),
        typename mpl::has_key<typename Semantics::set,polymorphic>::type(),
        typename mpl::has_key<typename Semantics::set,shared>::type());
}

namespace detail{

template<class F,class Data>
struct recursive_member_applier{
    typedef void result_type;
    recursive_member_applier(F const &f,Data &data) : f(f),data(data){}
    template<class T,class Semantics>
    void operator()(T &t,Semantics) const{
        intro::apply_member_recursive(f,data,t,Semantics());
    }
    template<class T,class Semantics,bool Poly,bool Shared>
    void operator()(T &t,Semantics,mpl::bool_<Poly> poly,mpl::bool_<Shared> sha) const{
        detail::apply_member_recursive(f,data,t,Semantics(),poly,sha);
    }
    recursive_member_applier<complement<F>,apply_recursive_data_invert_view<Data> >
    operator~() const{
        return recursive_member_applier<complement<F>,apply_recursive_data_invert_view<Data> >
            (complement<F>(f),apply_recursive_data_invert_view<Data>(data));
    }
private:
    F const f;
    Data &data;
};

template<class F,class Data>
struct has_complement_operator<recursive_member_applier<F,Data> >
    : has_complement_operator<F>{};

template<class F,class Data,class T>
void apply_recursive_class(F const &f,Data &data,T &t,mpl::true_ is_class_){
    using intro::apply_members;
    apply_members(detail::recursive_member_applier<F,Data>(f,data),t);
}

template<class F,class Data,class T>
void apply_recursive_class(F const &f,Data &data,T &t,mpl::false_ is_class_){}


template<class F,class Data,class T,class Semantics>
void apply_recursive(F const &f,Data &data,T *&t,Semantics){
    if(t) intro::apply_member_recursive(f,data,*t,typename Semantics::indir_semantics());
}

template<class F,class Data,class T,class Semantics>
void apply_recursive(F const &f,Data &data,T * const &t,Semantics){
    if(t) intro::apply_member_recursive(f,data,*t,typename Semantics::indir_semantics());
}

template<class F,class Data,class T,class Semantics>
void apply_recursive(F const &f,Data &data,reference<T> &t,Semantics){
    intro::apply_member_recursive(f,data,t.get(),typename Semantics::indir_semantics());
}
template<class F,class Data,class T,class Semantics>
void apply_recursive(F const &f,Data &data,reference<T> const &t,Semantics){
    intro::apply_member_recursive(f,data,t.get(),typename Semantics::indir_semantics());
}

template<class F,class T,class Semantics>
void apply_recursive(F const &f,T &t,Semantics,mpl::false_ shared){
    typedef detail::optional_set<detail::pooled_unordered_set<void const *,4> > set_type;
    set_type set;
    apply_recursive_data<set_type> data(set);
    apply_recursive(f,data,t,Semantics());
}

template<class F,class T,class Semantics>
void apply_recursive(F const &f,T &t,Semantics,mpl::true_ shared){
    typedef detail::optional_set<detail::pooled_unordered_set<void const *,4> > oset_type;
    oset_type oset;
    typedef detail::set_insert_view<oset_type> set_type;
    set_type set(oset,&t);
    apply_recursive_data<set_type> data(set);
    apply_recursive(f,data,t,Semantics());
}


}

template<class F,class Data,class T,class Semantics>
void apply_recursive(F const &f,Data &data,T &t,Semantics){
    detail::apply_recursive_class(f,data,t,typename is_class<T>::type());
}


template<class F,class T,class Semantics>
void apply_recursive(F const &f,T &t,Semantics){
    detail::apply_recursive(f,t,Semantics(),typename mpl::has_key<typename Semantics::set,shared>::type());
}


template<class F,class T>
void apply_recursive(F const &f,T &t){
    apply_recursive(f,t,typename default_semantics<T>::type());
}

}
}


#endif

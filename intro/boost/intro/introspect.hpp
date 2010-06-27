//          Copyright Stefan Strasser 2010.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_INTRO_INTROSPECT_HPP
#define BOOST_INTRO_INTROSPECT_HPP

#include <boost/intro/reference.hpp>
#include <boost/intro/semantics.hpp>
#include <boost/intro/detail/has_complement_operator.hpp>
#include <boost/intro/detail/ebco_container.hpp>
#include <boost/intro/detail/complement.hpp>
#include <boost/type.hpp>
#include <boost/type_traits/remove_const.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/apply_wrap.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/push_back.hpp>
#include <boost/fusion/include/pair.hpp>
#include <boost/fusion/include/mpl.hpp>
#include <boost/noncopyable.hpp>
#include <boost/utility/enable_if.hpp>
#include <typeinfo>


namespace boost{
namespace intro{

template<class T,std::size_t N>
class unnamed{};


namespace detail{


struct null_mapping{
    template<class Local>
    null_mapping const &operator()(Local) const{
        return *this;
    }
    template<class Local,class Mapped>
    null_mapping const &operator()(Local,Mapped const &) const{
        return *this;
    }
    void operator()() const{}
};

template<class F,class Class,std::size_t UnnamedEnd=0>
class apply_mapping{
public:
    explicit apply_mapping(F const &f) : f(f){}
    template<class Local>
    apply_mapping<F,Class,UnnamedEnd+1> const &operator()(Local local) const{
        this->operator()(local,unnamed<Class,UnnamedEnd>());
        return reinterpret_cast<apply_mapping<F,Class,UnnamedEnd+1> const &>(*this);
    }
    template<class Local,class Mapped>
    apply_mapping const &operator()(Local local,Mapped const &mapped) const{
        this->f(local,mapped);
        return *this;
    }
    void operator()() const{}
private:
    F const &f;
};

template<class F,class Class,std::size_t UnnamedEnd=0>
class apply_break_mapping{
public:
    explicit apply_break_mapping(F const &f,std::type_info const *&br) : f(f),br(br){}
    template<class Local>
    apply_break_mapping<F,Class,UnnamedEnd+1> const &operator()(Local local) const{
        this->operator()(local,unnamed<Class,UnnamedEnd>());
        return reinterpret_cast<apply_break_mapping<F,Class,UnnamedEnd+1> const &>(*this);
    }
    template<class Local,class Mapped>
    apply_break_mapping const &operator()(Local local,Mapped const &mapped) const{
        if(br){
            if(*br == typeid(Local)) br=0;
            else this->f(local,mapped);
        }
        return *this;
    }
    void operator()() const{}
private:
    F const &f;
    std::type_info const *&br;
};

template<class F,class Class,class Sequence=mpl::vector0<>,std::size_t UnnamedEnd=0>
class static_mapping{
public:
    explicit static_mapping(F const &f) : f(f){}
    template<class Local,class Mapped> //TODO stateful mapped
    static_mapping<
        F,Class,typename mpl::push_back<Sequence,
            fusion::pair<Local,Mapped>
        >::type,UnnamedEnd
    > const &operator()(Local,Mapped const &) const{
        return reinterpret_cast<static_mapping<
            F,Class,typename mpl::push_back<Sequence,
                fusion::pair<Local,Mapped>
            >::type,UnnamedEnd> const &>(*this);
    }
    template<class Local>
    static_mapping<
        F,Class,typename mpl::push_back<Sequence,
            fusion::pair<Local,unnamed<Class,UnnamedEnd> >
        >::type,UnnamedEnd+1
    > const &operator()(Local local) const{
        this->operator()(local,unnamed<Class,UnnamedEnd>());
        return reinterpret_cast<static_mapping<
            F,Class,typename mpl::push_back<Sequence,
                fusion::pair<Local,unnamed<Class,UnnamedEnd> >
            >::type,UnnamedEnd+1
        > const &>(*this);
    }
    void operator()() const{
        this->f(Sequence());
    }
private:
    F const &f;
};

template<class F,class Sequence=mpl::vector0<> >
class local_static_mapping{
public:
    explicit local_static_mapping(F const &f) : f(f){}
    template<class Local,class Mapped> //TODO stateful mapped
    local_static_mapping<F,typename mpl::push_back<Sequence,Local>::type> const &
    operator()(Local local,Mapped const &) const{
        return this->operator()(local);
    }
    template<class Local>
    local_static_mapping<F,typename mpl::push_back<Sequence,Local>::type> const &
    operator()(Local) const{
        return reinterpret_cast<
            local_static_mapping<F,typename mpl::push_back<Sequence,Local>::type> const &
        >(*this);
    }
    void operator()() const{
        this->f(Sequence());
    }
private:
    F const &f;
};

}

template<class T,class F>
void apply_sequence(F const &f){
    introspect(
        detail::static_mapping<F,T>(f),
        type<typename remove_const<T>::type>()
    );
}

namespace detail{

template<class T,class F>
void for_each(F const &f,mpl::false_ complement){
    introspect( //ADL
        detail::apply_mapping<F,T>(f),
        type<typename remove_const<T>::type>()
    ); 
}

template<class T,class F>
apply_break_mapping<F,T> get_undo_mapping(F const &f,std::type_info const *&last_local){
    return apply_break_mapping<F,T>(f,last_local);
}

template<class F,class T>
struct apply_catcher{
    explicit apply_catcher(F const &f,std::type_info const *last_local)
        : f(f),last_local(last_local){}
    template<class Local,class Mapped>
    void operator()(Local local,Mapped const &mapped) const{
        //the type of Local is saved for the case f(p) throws an exception.
        //this creates much better code than using a try-block, and
        //it creates a lot less introspect() instantiations than
        //undo-ing with a static Local in a catch-block.
        //cost: type_info::operator== on exception for each member.
        last_local=&typeid(Local);
        f(local,mapped);
    }
private:
    F const &f;
    std::type_info const *&last_local;
};

template<class T,class F>
void for_each(F const &f,mpl::true_ complement){
    std::type_info const *last_local=0;
    try{
        introspect( //ADL
            apply_mapping<apply_catcher<F,T>,T>(apply_catcher<F,T>(f,last_local)),
            type<typename remove_const<T>::type>()
        ); 
    }catch(...){
        BOOST_ASSERT(last_local);
        introspect(
                get_undo_mapping<T>(~f,last_local),
                type<typename remove_const<T>::type>());
        throw;
    }
}

template<class F>
struct mapped_discarder{
    explicit mapped_discarder(F const &f) : f(f){}
    template<class Local,class Mapped>
    void operator()(Local local,Mapped const &) const{
        f(local);
    }
    mapped_discarder<complement<F> > operator~() const{
        return mapped_discarder<complement<F> >(complement<F>(f));
    }
private:
    F const f;
};

template<class F>
struct has_complement_operator<mapped_discarder<F> >
    : has_complement_operator<F>{};

}

template<class T,class F>
void for_each(F const &f){
    detail::for_each<T>(f,typename detail::has_complement_operator<F>::type());
}

template<class T,class F>
void for_each_local(F const &f){
    for_each<T>(detail::mapped_discarder<F>(f));
}


template<class T,class F>
void apply_local_sequence(F const &f){
    introspect(
        detail::local_static_mapping<F>(f),
        type<typename remove_const<T>::type>()
    );
}


template<class Class,class T,T Class::*Ptr,class Semantics>
class member_t{};

namespace detail{

template<class Semantics,class Add>
struct add_semantics{
    typedef semantics<
        typename mpl::fold<
            typename Add::set,
            typename Semantics::set,
            mpl::apply_wrap1<mpl::_2,mpl::_1>
        >::type,
        typename add_semantics<
            typename Semantics::indir_semantics,
            typename Add::indir_semantics
        >::type
    > type;
};

template<class Semantics>
struct add_semantics<Semantics,semantics<> >{
    typedef Semantics type;
};

template<class Arg>
struct get_semantics{
    typedef typename mpl::if_<
        is_semantics<Arg>,
        Arg,
        semantics<Arg>
    >::type type;
};

}

template<class Class,class T,T Class::*Ptr>
member_t<
    Class,
    T,
    Ptr,
    typename default_semantics<T>::type
> member(){
    return member_t<
        Class,
        T,
        Ptr,
        typename default_semantics<T>::type
    >();
}

template<class Class,class T,T Class::*Ptr,class Semantics>
member_t<
    Class,
    T,
    Ptr,
    typename detail::add_semantics<
        typename default_semantics<T>::type,
        typename detail::get_semantics<Semantics>::type
    >::type
> member(){
    return member_t<
        Class,
        T,
        Ptr,
        typename detail::add_semantics<
            typename default_semantics<T>::type,
            typename detail::get_semantics<Semantics>::type
        >::type
    >();
}
template<class Base>
class base_class{};

template<unsigned int Version>
class version{};


}
}


#endif

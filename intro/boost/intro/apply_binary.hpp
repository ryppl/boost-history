//          Copyright Stefan Strasser 2010.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_INTRO_APPLY_BINARY_HPP
#define BOOST_INTRO_APPLY_BINARY_HPP

#include <boost/intro/introspect.hpp>
#include <boost/type_traits/remove_const.hpp>
#include <boost/bind.hpp>
#include <boost/intro/detail/complement.hpp>
#include <boost/intro/detail/has_complement_operator.hpp>


namespace boost{
namespace intro{
namespace detail{
template<class,class,class>
struct binary_applier;
}

template<class F,class T,class T2>
void apply_binary(F const &f,T &t,T2 &t2){
    T const &t2const=t2; (void)t2const;//T and T2 must only differ in const-ness
    T2 const &t1const=t; (void)t1const;
    intro::for_each_local<T>(detail::binary_applier<F,T,T2>(f,t,t2));
}


namespace detail{

template<class F,class Class,class Class2,class T,T Class::*Ptr,class Semantics>
void apply_binary(F const &f,Class &c,Class2 &c2,
                     member_t<typename remove_const<Class>::type,T,Ptr,Semantics>){
    f(c.*Ptr,c2.*Ptr,Semantics());
}

template<class F,class Class,class Class2,class Base>
void apply_binary(F const &f,Class &c,Class2 &c2,base_class<Base>){
    typename mpl::if_<is_const<Class>,Base const,Base>::type &base=c;
    typename mpl::if_<is_const<Class2>,Base const,Base>::type &base2=c2;
    using intro::apply_binary;
    apply_binary(f,base,base2); //recurse into base classes
}

template<class F,class T,class T2>
struct binary_applier{
    binary_applier(F const &f,T &t,T2 &t2) : f(f),t(t),t2(t2){}
    typedef void result_type;
    template<class Local>
    void operator()(Local local) const{
        apply_binary(f,t,t2,local);
    }
    binary_applier<complement<F>,T,T2> operator~() const{
        return binary_applier<complement<F>,T,T2>(complement<F>(f),t,t2);
    }
private:
    F const f;
    T &t;
    T2 &t2;
};

template<class F,class T,class T2>
struct has_complement_operator<binary_applier<F,T,T2> >
    : has_complement_operator<F>{};


}
}
}

#endif

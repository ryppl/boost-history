//          Copyright Stefan Strasser 2010.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_INTRO_APPLY_HPP
#define BOOST_INTRO_APPLY_HPP

#include <boost/intro/introspect.hpp>
#include <boost/intro/detail/has_complement_operator.hpp>
#include <boost/intro/detail/complement.hpp>
#include <boost/type_traits/remove_const.hpp>
#include <boost/utility/enable_if.hpp>


namespace boost{
namespace intro{
namespace detail{
template<class,class>
struct applier;
}

template<class F,class T>
void apply(F const &f,T &t){
    intro::for_each_local<T>(detail::applier<F,T>(f,t));
}


namespace detail{

template<class F,class Class,class T,T Class::*Ptr,class Semantics>
void apply(F const &f,Class &c,
           member_t<typename remove_const<Class>::type,T,Ptr,Semantics>){
    f(c.*Ptr,Semantics());
}

template<class F,class Class,class Base>
void apply(F const &f,Class &c,base_class<Base>){
    typename mpl::if_<is_const<Class>,Base const,Base>::type &base=c;
    using intro::apply;
    apply(f,base); //recurse into base classes
}

template<class F,class Class>
struct applier{
    applier(F const &f,Class &c) : f(f),c(c){}
    typedef void result_type;
    template<class Local>
    void operator()(Local local) const{
        apply(f,c,local);
    }
    applier<complement<F>,Class> operator~() const{
        return applier<complement<F>,Class>(complement<F>(f),c);
    }
private:
    F const f;
    Class &c;
};

template<class F,class Class>
struct has_complement_operator<applier<F,Class> >
    : has_complement_operator<F>{};

}
}
}

#endif

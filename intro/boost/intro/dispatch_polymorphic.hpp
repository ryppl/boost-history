//          Copyright Stefan Strasser 2010.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_INTRO_DISPATCH_POLYMORPHIC_HPP
#define BOOST_INTRO_DISPATCH_POLYMORPHIC_HPP

#include <typeinfo>
#include <exception>
#include <boost/serialization/singleton.hpp>
#include <boost/serialization/smart_cast.hpp>
#include <boost/intro/detail/flat_map.hpp>
#include <boost/intro/introspect.hpp>
#include <boost/type_traits/is_polymorphic.hpp>
#include <boost/assert.hpp>


namespace boost{
namespace intro{

struct unregistered_class : std::exception{};

namespace detail{

struct adl_tag{};
template<class Algo,class Base>
void register_classes(Algo const *,Base *,int,adl_tag){}

template<class Algo,class Base>
void register_classes(){
    using namespace detail;
    register_classes((Algo *)0,(Base *)0,0,detail::adl_tag());
}

}

template<class F,class T>
class polymorphic_dispatcher{
    typedef typename F::result_type result_type;
public:
    static result_type dispatch(F const &f,T &t){
        return polymorphic_dispatcher::dispatch(f,t,typename is_polymorphic<T>::type());
    }
    template<class Derived>
    static void register_derived(){
        map_type &map=serialization::singleton<map_type>::get_mutable_instance();
        dispatcher<Derived> *dispatch=new dispatcher<Derived>();
        if(!map.insert(std::make_pair(&typeid(Derived),dispatch)).second){
            delete dispatch;
        }
    }
private:
    static result_type dispatch(F const &f,T &t,mpl::true_ poly){
        detail::register_classes<F,T>();
        map_type const &map=serialization::singleton<map_type>::get_const_instance();

        std::type_info const &type=typeid(t);
        if(type == typeid(T)) return f(t);
        else{
            typename map_type::const_iterator it=map.find(&type);
            if(it == map.end()) throw unregistered_class();
            return (*it->second)(f,t);
        }
    }
    static result_type dispatch(F const &f,T &t,mpl::false_ poly){
        BOOST_ASSERT(typeid(t) == typeid(T));
        return f(t);
    }

    struct dispatcher_base{
        virtual result_type operator()(F const &f,T &t) const = 0;
        virtual ~dispatcher_base(){}
    };
    template<class Derived>
    struct dispatcher : dispatcher_base{
        virtual result_type operator()(F const &f,T &t) const{
            Derived &d=serialization::smart_cast<Derived &,T &>(t);
            return f(d);
        }
    };
    struct compare{
        bool operator()(std::type_info const *l,std::type_info const *r) const{
#pragma warning(push)
#pragma warning(disable:4800)
            return l->before(*r);
#pragma warning(pop)
        }
    };
    struct map_type
        : intro::detail::flat_map<
            std::type_info const *,
            dispatcher_base *,
            compare
        >{
        ~map_type(){
            for(typename map_type::iterator it=this->begin();it != this->end();++it){
                delete it->second;
            }
        }
    };
};



template<class F,class T>
typename F::result_type dispatch_polymorphic(F const &f,T &t){
    return polymorphic_dispatcher<F,T>::dispatch(f,t);
}

namespace detail{

template<void (*)()> struct instantiate_function{};

template<class Algo,class Base,class Derived>
struct register_class{
#ifdef BOOST_MSVC 
    virtual void instantiate(){ reg.instantiate(); }
#else
    static void instantiate(){ reg.instantiate(); }
    typedef instantiate_function<&register_class::instantiate> _;
#endif
    struct reg_type{
        reg_type(){
            //std::cerr << typeid(Base).name() << " is base of " << typeid(Derived).name() << " for " << typeid(Algo).name() << std::endl << std::endl;
            this->reg(typename is_const<Base>::type());
        }
        void reg(mpl::true_){
            polymorphic_dispatcher<Algo,Base>::template register_derived<Derived const>();
        }
        void reg(mpl::false_){
            polymorphic_dispatcher<Algo,Base>::template register_derived<Derived>();
        }
        void instantiate(){}
    };
    static reg_type reg;
    typedef void type;
};


template<class Algo,class Base,class Derived>
typename register_class<Algo,Base,Derived>::reg_type
register_class<Algo,Base,Derived>::reg;

}
}
}

#endif

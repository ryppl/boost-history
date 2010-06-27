//          Copyright Stefan Strasser 2010.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_INTRO_PRINT_HPP
#define BOOST_INTRO_PRINT_HPP

#include <boost/intro/introspect.hpp>
#include <boost/intro/detail/indent_streambuf.hpp>
#include <boost/type_traits/is_pointer.hpp>
#include <boost/type_traits/is_arithmetic.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/lexical_cast.hpp>
#include <ostream>
#include <streambuf>
#include <string>
#include <cstring>

namespace boost{
namespace intro{
namespace detail{

template<std::size_t N>
class unnamed_name{
public:
    static char const *get(){
        return string.c_str();
    }
private:
    static std::string string;
};

template<std::size_t N>
std::string unnamed_name<N>::string(std::string("unnamed")+lexical_cast<std::string>(N));

template<class T,class Transformation>
void print(T const &t,std::ostream &out,Transformation const &,mpl::true_ ptrorint){
    out << t;
}

template<class,class>
class printer;


template<class T,class Transformation>
void print(T const &t,std::ostream &out,Transformation const &transform,mpl::false_ ptrorint){
    out << '{' << std::endl;
    indent_streambuf indent(*out.rdbuf(),true);
    std::ostream indent_out(&indent);
    intro::for_each<T>(detail::printer<T,Transformation>(t,indent_out,transform));
    out << '}';
}

}

template<class T,class Transformation>
void print(T const &t,std::ostream &out,Transformation const &transform){
    detail::print(t,out,transform,mpl::bool_<is_pointer<T>::type::value || is_arithmetic<T>::type::value>());
}

template<class T,class Transformation>
void print(reference<T> const &t,std::ostream &out,Transformation const &transform){
    print(&t.get(),out,transform);
}

namespace detail{


template<class T>
char const *typeid_name(){
    char const *name=typeid(T).name();
#ifdef BOOST_MSVC
    while(char const *scope=std::strstr(name,"::")){
        char const *templ=std::strchr(name,'<');
        if(templ && templ < scope) break;
        name=scope+2;
    }
    if(strncmp(name,"class ",6) == 0) name+=6;
    else if(strncmp(name,"struct ",7) == 0) name+=7;
#endif
    return name;
}

template<class Class,class Transformation>
class printer{
public:
    explicit printer(Class const &c,std::ostream &out,Transformation const &transform)
        : c(c),out(out),transform(transform){}
    template<class T,T Class::*Ptr,class Semantics,class Mapped>
    void operator()(member_t<Class,T,Ptr,Semantics>,Mapped const &mapped) const{
        out << typeid_name<T>() << ' ' << transform(mapped) << " = ";
        print(c.*Ptr,out,transform);
        out << ';' << std::endl;
    }
    template<class T,reference<T> Class::*Ptr,class Semantics,class Mapped>
    void operator()(member_t<Class,reference<T>,Ptr,Semantics>,Mapped const &mapped) const{
        out << typeid_name<T>() << " & " << transform(mapped) << " = ";
        print(c.*Ptr,out,transform);
        out << ';' << std::endl;
    }
    template<class Base,class Mapped>
    void operator()(base_class<Base>,Mapped const &) const{
        out << typeid_name<Base>() << " = ";
        Base const &base=c;
        print(base,out,transform);
        out << ';' << std::endl;
    }
private:
    Class const &c;
    std::ostream &out;
    Transformation transform;
};

}


class print_transformation{
public:
    char const *operator()(char *m) const{ return m; }
    char const *operator()(char const *m) const{ return m; }
    char const *operator()(std::string const &s) const{ return s.c_str(); }
    template<class Class,std::size_t N>
    std::string operator()(unnamed<Class,N>) const{
        return detail::unnamed_name<N>::get();
    }
    template<class Mapped>
    char const *operator()(Mapped const &) const{
        return detail::typeid_name<Mapped>();
    }
};


template<class T>
void print(T const &t,std::ostream &out){
    print(t,out,print_transformation());
}

}
}


#endif

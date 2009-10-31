/*-----------------------------------------------------------------------------+    
Copyright (c) 2008-2009: Joachim Faulhaber
+------------------------------------------------------------------------------+
   Distributed under the Boost Software License, Version 1.0.
      (See accompanying file LICENCE.txt or copy at
           http://www.boost.org/LICENSE_1_0.txt)
+-----------------------------------------------------------------------------*/
#include <iostream>
#include <set>
#include <vector>
#include <boost/itl/functors.hpp>
#include <boost/mpl/apply.hpp>
#include <boost/mpl/placeholders.hpp>

#include <boost/itl/interval_map.hpp>
#include <boost/itl/split_interval_map.hpp>
#include <boost/validate/type/nat.hpp>

using namespace std;
using namespace boost;
using namespace boost::mpl::placeholders;
using namespace boost::itl;

template<class T>struct unary{
  void speak(){ cout<<"unary"<<endl; }
};

template<class T, template<class>class U>
struct unator1{
  void speak(){U<T> obj; obj.speak();}
};

template<class T, class U>
struct unator2{
  void speak(){
    mpl::apply<U,T>::type obj; 
    obj.speak();
  }
};

//----------------------------------------
template<class T1, class T2>struct binary{
  void speak(){ cout<<"binary"<<endl; }
};

template<class T1, class T2,
         template<class,class>class U>
struct binator1{
  void speak(){U<T1,T2> obj; obj.speak();}
};

template<class T1, class T2, class U>
struct binator2{
  void speak(){
    mpl::apply2<U,T1,T2>::type obj; 
    obj.speak();
  }
};

template<class T1, class Compare = std::less<T1> >
class PairSet1
{
public:
    typedef std::set
            <
                std::pair<T1,T1>, 
                typename mpl::apply<Compare, std::pair<T1,T1> >::type
            > 
            ImplSetT;

    typedef std::set
            <
                T1, 
                typename mpl::apply<Compare, T1>::type
            > 
            ElemSetT;
    // 
    PairSet1(): _set() 
    {cout << "PairSet1 constructed" << endl;}

private:
    ElemSetT _set;
};

typedef PairSet1<int, std::less<_> > PairSet1_int;

template<class T, 
         template<class,class>class Vec, 
         class A = std::allocator<T> >
class my_class
{
public:
    typedef Vec<T,A> vec_type;
    my_class():_vec(1,static_cast<T>(42))
    { std::cout<<"Aswer: "<<_vec[0]<<endl; }
private:
    vec_type _vec;
};

void template_default_problem()
{
    my_class<double, std::vector> myInst;
}

/*
template<class T, 
         class C = std::less<T>, 
         class A = std::allocator<T> >
class interval_set
{
public:
  typedef 
    set<interval<T>, 
        exclusive_less<interval<T> >
        A::allocator_template<interval<T> >//error 
       > impl_type; // ^^^^ No language support

  typedef set<T,C,A> atomized_type;

  // Return the interval set as a set of elements 
  void atomize(atomized_type& atomized);
}

template<
  class T, 
  class C = std::less<T>, 
  template<class>class A = std::allocator 
>
class my_interval_set
{
public:
  typedef 
    set<interval<T>, 
        exclusive_less<interval<T> >
        A<interval<T> > //ok now 
       > impl_type;

  typedef 
    set<T,C,
        A<T> //Same allocator,
             //different instances
       > atomized_type;

  // Return the interval set as a set of elements 
  void atomize(atomized_type& atomized);
}
*/

class NoPlus
{
    //NoPlus(const NoPlus& val):_value(val._value){}
public:
    NoPlus():_value(0){}
    NoPlus(int value):_value(value){}

    bool operator < (const NoPlus& rhs)const
    { return _value < rhs._value; }

    bool operator == (const NoPlus& rhs)const
    { return _value == rhs._value; }

    int value()const { return _value; }

    std::string as_string()const
    { return itl::to_string<int>::apply(_value); }

private:
    int _value;
};

template<class CharType, class CharTraits>
std::basic_ostream<CharType, CharTraits>& operator <<
  (std::basic_ostream<CharType, CharTraits>& stream, 
   const NoPlus& object)
{
    return stream << object.value();
}

void codomain_test()
{
    typedef split_interval_map<int,NoPlus> NoplumT; 
    NoplumT noplum;
    //noplum.insert(mapping_pair<int,NoPlus>(47,NoPlus(11)));
    //noplum.insert(mapping_pair<int,NoPlus>(42,NoPlus(14)));
    //noplum.erase(mapping_pair<int,NoPlus>(42,NoPlus(14)));
    
    noplum.insert(make_pair(interval<int>::rightopen(2,6),NoPlus(1)));
    noplum.insert(make_pair(interval<int>::rightopen(4,8),NoPlus(2)));

    cout << noplum << endl;
}


void string_codomain_test()
{
    typedef interval_map<int,std::string> ConcaMapT; 
    ConcaMapT cat_map;
    cat_map += make_pair(interval<int>::rightopen(1,5),std::string("Hello"));
    cat_map += make_pair(interval<int>::rightopen(3,7),std::string(" World"));
    
    cout << "cat_map: " << cat_map << endl;
}

void quantifier_intersect_test()
{
    typedef interval_map<int,int> QuantifierT;
    QuantifierT sec_map;
    sec_map += make_pair(interval<int>::rightopen(1,5), 1);
    sec_map &= make_pair(interval<int>::rightopen(3,7), 1);
    //sec_map &= QuantifierT(make_pair(interval<int>::rightopen(3,7), 1));
    
    cout << "sec_map: " << sec_map << endl;
}

void quantifier_subtract_test()
{
    typedef interval_map<int,cnat> QuantifierT;
    QuantifierT sec_map;
    sec_map += QuantifierT::value_type(interval<int>::rightopen(1,5), 1);
    sec_map -= make_pair(interval<int>::rightopen(3,7), static_cast<cnat>(2));
    sec_map += make_pair(interval<int>::rightopen(3,7), static_cast<cnat>(3));
    //sec_map *= QuantifierT(make_pair(interval<int>::rightopen(3,7), 1));
    
    QuantifierT sec_map2;

    if(sec_map != sec_map2)
        cout << "geht doch\n";


    cout << "sec_map: " << sec_map << endl;
}

void misc_test()
{
    typedef interval_map<int,int,partial_enricher> MapT;
    MapT m;
    m += MapT::value_type(interval<int>::rightopen(1,3), 1);
    cout << m << endl;
    m -= MapT::value_type(interval<int>::rightopen(1,3), 1);
    cout << m << endl;
}

int main()
{
    cout << ">> Interval Template Library: Test meta_functors     <<\n";
    cout << "-------------------------------------------------------\n";

    /*
    unator1<int, unary>      untor1; untor1.speak();
    unator2<int, unary<_> >  untor2; untor2.speak();
    binator1<int, int, binary>     bintor1; bintor1.speak();
    binator2<int, int, binary<_1,_2> > bintor2; bintor2.speak();

    PairSet1_int ps1;

    template_default_problem();
    */
    //codomain_test();
    //string_codomain_test();
    //quantifier_subtract_test();
    misc_test();
    return 0;
}


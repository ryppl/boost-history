/*----------------------------------------------------------------------------+
Copyright (c) 2008-2008: Joachim Faulhaber
+-----------------------------------------------------------------------------+
Boost Software License - Version 1.0 - August 17th, 2003

Permission is hereby granted, free of charge, to any person or organization
obtaining a copy of the software and accompanying documentation covered by
this license (the "Software") to use, reproduce, display, distribute,
execute, and transmit the Software, and to prepare derivative works of the
Software, and to permit third-parties to whom the Software is furnished to
do so, all subject to the following:

The copyright notices in the Software and this entire statement, including
the above license grant, this restriction and the following disclaimer,
must be included in all copies of the Software, in whole or in part, and
all derivative works of the Software, unless such copies or derivative
works are solely in the form of machine-executable object code generated by
a source language processor.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
DEALINGS IN THE SOFTWARE.
+----------------------------------------------------------------------------*/

#include <iostream>
#include <set>
#include <vector>
#include <boost/itl/functors.hpp>
#include <boost/mpl/apply.hpp>
#include <boost/mpl/placeholders.hpp>

#include <boost/itl/interval_map.hpp>
#include <boost/itl/split_interval_map.hpp>

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
	{ return to_string<int>::apply(_value); }

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
	codomain_test();
	string_codomain_test();
    return 0;
}


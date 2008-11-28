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
#include <boost/mpl/apply.hpp>
#include <boost/mpl/placeholders.hpp>

using namespace std;
using namespace boost;
using namespace boost::mpl::placeholders;

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

int main()
{
    cout << ">> Interval Template Library: Test meta_functors     <<\n";
    cout << "-------------------------------------------------------\n";

	unator1<int, unary>      untor1; untor1.speak();
	unator2<int, unary<_> >  untor2; untor2.speak();
	binator1<int, int, binary>     bintor1; bintor1.speak();
	binator2<int, int, binary<_1,_2> > bintor2; bintor2.speak();

	PairSet1_int ps1;

    return 0;
}


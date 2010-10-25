//Purpose:
//  see if variadic reduce is possible using variadic templates.
//
//Result:
//  Yes.
//

struct add
{
      template<typename T>
      T
    operator()(T a,T b)
    { 
        return a+b;
    }

};

template<typename Op, typename T, typename... U>
T fold(Op op, T z, U... a)
;

template<typename Op, typename T>
T fold(Op op, T z)
{ return z
;}

template<typename Op, typename T, typename... U>
T fold(Op op, T z, T a, U... b)
{ return op(z,fold(op,a,b...))
;}

#include <iostream>

int main(void)
{
    add op;
    std::cout<<fold(op,1)<<"\n";
    std::cout<<fold(op,1,2,3)<<"\n";
    std::cout<<fold(op,10,20,30)<<"\n";
    return 0;
}    

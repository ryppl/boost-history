//Purpose:
//  see if variadic reduce is possible using variadic templates.
//
//Result:
//  No, apparently b... is not an arg pack.
//
int add(int a,int b){return a+b;}

template<typename T>
T fold(T (*op)(T,T), T z, T a...)
;
template<typename T>
T fold(T (*op)(T,T), T z, T a)
{ return op(z,a)
;}
template<typename T>
T fold(T (*op)(T,T), T z, T a, T b...)
{ return op(z,fold(op,a,b...))
;}


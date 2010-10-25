//Purpose:
//  see if variadic reduce is possible using variadic templates.
//
//Result:
// //  No, apparently b... is not an arg pack.
// //
// int add(int a,int b){return a+b;}

template<typename T, typename... U>
T fold(T (*op)(T,T), T z, U... a)
;

template<typename T>
T fold(T (*op)(T,T), T z, T a)
{ return op(z,a)
;}

template<typename T, typename... U>
T fold(T (*op)(T,T), T z, T a, U... b)
{ return op(z,fold(op,a,b...))
;}


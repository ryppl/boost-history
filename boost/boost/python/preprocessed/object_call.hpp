// Copyright David Abrahams 2002. Permission to copy, use,
// modify, sell and distribute this software is granted provided this
// copyright notice appears in all copies. This software is provided
// "as is" without express or implied warranty, and with no claim as
// to its suitability for any purpose.
#ifndef OBJECT_CALL_DWA2002612_HPP
# define OBJECT_CALL_DWA2002612_HPP

template<class A0>
typename dependent<object,A0>::type
operator()(A0 const&a0)const 
{
    typedef typename dependent<object,A0>::type obj;
    U const&self=*static_cast<U const*>(this);
    return call<obj>(converter::get_managed_object(self),a0);
}
template<class A0,class A1>
typename dependent<object,A0>::type
operator()(A0 const&a0,A1 const&a1)const 
{
    typedef typename dependent<object,A0>::type obj;
    U const&self=*static_cast<U const*>(this);
    return call<obj>(converter::get_managed_object(self),a0,a1);
}
template<class A0,class A1,class A2>
typename dependent<object,A0>::type
operator()(A0 const&a0,A1 const&a1,A2 const&a2)const 
{
    typedef typename dependent<object,A0>::type obj;
    U const&self=*static_cast<U const*>(this);
    return call<obj>(converter::get_managed_object(self),a0,a1,a2);
}
template<class A0,class A1,class A2,class A3>
typename dependent<object,A0>::type
operator()(A0 const&a0,A1 const&a1,A2 const&a2,A3 const&a3)const 
{
    typedef typename dependent<object,A0>::type obj;
    U const&self=*static_cast<U const*>(this);
    return call<obj>(converter::get_managed_object(self),a0,a1,a2,a3);
}
template<class A0,class A1,class A2,class A3,class A4>
typename dependent<object,A0>::type
operator()(A0 const&a0,A1 const&a1,A2 const&a2,A3 const&a3,A4 const&a4)const 
{
    typedef typename dependent<object,A0>::type obj;
    U const&self=*static_cast<U const*>(this);
    return call<obj>(converter::get_managed_object(self),a0,a1,a2,a3,a4);
}
template<class A0,class A1,class A2,class A3,class A4,class A5>
typename dependent<object,A0>::type
operator()(A0 const&a0,A1 const&a1,A2 const&a2,A3 const&a3,A4 const&a4,A5 const&a5)const 
{
    typedef typename dependent<object,A0>::type obj;
    U const&self=*static_cast<U const*>(this);
    return call<obj>(converter::get_managed_object(self),a0,a1,a2,a3,a4,a5);
}
template<class A0,class A1,class A2,class A3,class A4,class A5,class A6>
typename dependent<object,A0>::type
operator()(A0 const&a0,A1 const&a1,A2 const&a2,A3 const&a3,A4 const&a4,A5 const&a5,A6 const&a6)const 
{
    typedef typename dependent<object,A0>::type obj;
    U const&self=*static_cast<U const*>(this);
    return call<obj>(converter::get_managed_object(self),a0,a1,a2,a3,a4,a5,a6);
}
template<class A0,class A1,class A2,class A3,class A4,class A5,class A6,class A7>
typename dependent<object,A0>::type
operator()(A0 const&a0,A1 const&a1,A2 const&a2,A3 const&a3,A4 const&a4,A5 const&a5,A6 const&a6,A7 const&a7)const 
{
    typedef typename dependent<object,A0>::type obj;
    U const&self=*static_cast<U const*>(this);
    return call<obj>(converter::get_managed_object(self),a0,a1,a2,a3,a4,a5,a6,a7);
}
template<class A0,class A1,class A2,class A3,class A4,class A5,class A6,class A7,class A8>
typename dependent<object,A0>::type
operator()(A0 const&a0,A1 const&a1,A2 const&a2,A3 const&a3,A4 const&a4,A5 const&a5,A6 const&a6,A7 const&a7,A8 const&a8)const 
{
    typedef typename dependent<object,A0>::type obj;
    U const&self=*static_cast<U const*>(this);
    return call<obj>(converter::get_managed_object(self),a0,a1,a2,a3,a4,a5,a6,a7,a8);
}
template<class A0,class A1,class A2,class A3,class A4,class A5,class A6,class A7,class A8,class A9>
typename dependent<object,A0>::type
operator()(A0 const&a0,A1 const&a1,A2 const&a2,A3 const&a3,A4 const&a4,A5 const&a5,A6 const&a6,A7 const&a7,A8 const&a8,A9 const&a9)const 
{
    typedef typename dependent<object,A0>::type obj;
    U const&self=*static_cast<U const*>(this);
    return call<obj>(converter::get_managed_object(self),a0,a1,a2,a3,a4,a5,a6,a7,a8,a9);
}
template<class A0,class A1,class A2,class A3,class A4,class A5,class A6,class A7,class A8,class A9,class A10>
typename dependent<object,A0>::type
operator()(A0 const&a0,A1 const&a1,A2 const&a2,A3 const&a3,A4 const&a4,A5 const&a5,A6 const&a6,A7 const&a7,A8 const&a8,A9 const&a9,A10 const&a10)const 
{
    typedef typename dependent<object,A0>::type obj;
    U const&self=*static_cast<U const*>(this);
    return call<obj>(converter::get_managed_object(self),a0,a1,a2,a3,a4,a5,a6,a7,a8,a9,a10);
}
template<class A0,class A1,class A2,class A3,class A4,class A5,class A6,class A7,class A8,class A9,class A10,class A11>
typename dependent<object,A0>::type
operator()(A0 const&a0,A1 const&a1,A2 const&a2,A3 const&a3,A4 const&a4,A5 const&a5,A6 const&a6,A7 const&a7,A8 const&a8,A9 const&a9,A10 const&a10,A11 const&a11)const 
{
    typedef typename dependent<object,A0>::type obj;
    U const&self=*static_cast<U const*>(this);
    return call<obj>(converter::get_managed_object(self),a0,a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11);
}
template<class A0,class A1,class A2,class A3,class A4,class A5,class A6,class A7,class A8,class A9,class A10,class A11,class A12>
typename dependent<object,A0>::type
operator()(A0 const&a0,A1 const&a1,A2 const&a2,A3 const&a3,A4 const&a4,A5 const&a5,A6 const&a6,A7 const&a7,A8 const&a8,A9 const&a9,A10 const&a10,A11 const&a11,A12 const&a12)const 
{
    typedef typename dependent<object,A0>::type obj;
    U const&self=*static_cast<U const*>(this);
    return call<obj>(converter::get_managed_object(self),a0,a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11,a12);
}
template<class A0,class A1,class A2,class A3,class A4,class A5,class A6,class A7,class A8,class A9,class A10,class A11,class A12,class A13>
typename dependent<object,A0>::type
operator()(A0 const&a0,A1 const&a1,A2 const&a2,A3 const&a3,A4 const&a4,A5 const&a5,A6 const&a6,A7 const&a7,A8 const&a8,A9 const&a9,A10 const&a10,A11 const&a11,A12 const&a12,A13 const&a13)const 
{
    typedef typename dependent<object,A0>::type obj;
    U const&self=*static_cast<U const*>(this);
    return call<obj>(converter::get_managed_object(self),a0,a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11,a12,a13);
}
template<class A0,class A1,class A2,class A3,class A4,class A5,class A6,class A7,class A8,class A9,class A10,class A11,class A12,class A13,class A14>
typename dependent<object,A0>::type
operator()(A0 const&a0,A1 const&a1,A2 const&a2,A3 const&a3,A4 const&a4,A5 const&a5,A6 const&a6,A7 const&a7,A8 const&a8,A9 const&a9,A10 const&a10,A11 const&a11,A12 const&a12,A13 const&a13,A14 const&a14)const 
{
    typedef typename dependent<object,A0>::type obj;
    U const&self=*static_cast<U const*>(this);
    return call<obj>(converter::get_managed_object(self),a0,a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11,a12,a13,a14);
}

#endif // OBJECT_CALL_DWA2002612_HPP

//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#include <libs/assign/v2/example/include.h>
#include <libs/assign/v2/example/foo.h>

namespace example_assign_v2{

foo::foo() : ptr_a( 0 ), b( 0 ){}
foo::foo( int& a) : ptr_a( &a ), b( 0 ){}
foo::foo( int& a, int const& b_) : ptr_a( &a ), b( b_ ){}

bool foo::operator==(foo const& other)const
{
    return (this->ptr_a == other.ptr_a) && (this->b == other.b);
}

foo& foo::operator+=(int const& delta){ this->b += delta; return (*this); }

printer::printer(std::ostream& os_):os( os_ ){}
printer::result_type printer::operator()( foo const& t)const
{
    this->os << t;
}

make_foo::result_type make_foo::operator()(int& a, int const& b)const
{
    return result_type( a, b );
}

std::ostream& operator<<( std::ostream& os, foo const& t)
{
    os << '(';
    if(t.ptr_a){
        os << *t.ptr_a;
    }else{
        os << "NULL";
    }
    return os << ',' << t.b << ')';
}

}// example_assign_v2


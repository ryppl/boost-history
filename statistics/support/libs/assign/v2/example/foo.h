//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_EXAMPLE_FOO_ER_2010_H
#define BOOST_ASSIGN_V2_EXAMPLE_FOO_ER_2010_H
#include <ostream>

namespace example_assign_v2{

//[foo
class Foo
{
	public:
    Foo(); //:ptr_a( 0 ), b( 0 ){}
    Foo( int& a); // : ptr_a( &a ), b( 0 ){}
    Foo( int& a, int const& b_); //: ptr_a( &a ), b( b_ ){}

    bool operator==(Foo const& other)const;

    Foo& operator+=(int const& delta);

    private:
    friend std::ostream & operator<<(std::ostream &os, const Foo& p);
    int* ptr_a;
    int b;
};
//]


struct make_foo // TODO remove
{
    typedef Foo result_type;
    result_type operator()(int& a, int const& b)const;

};


}// example_assign_v2

#endif

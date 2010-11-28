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
#include <libs/assign/v2/example/include.h>

namespace example_assign_v2{

//[foo
struct foo
{
    foo(); //:ptr_a( 0 ), b( 0 ){}
    foo( int& a); // : ptr_a( &a ), b( 0 ){}
    foo( int& a, int const& b_); //: ptr_a( &a ), b( b_ ){}

    bool operator==(foo const& other)const;

    foo& operator+=(int const& delta);

    private:
    friend std::ostream & operator<<(std::ostream &os, const foo& p);	    
    int* ptr_a;
    int b;
};

struct make_foo
{
    typedef foo result_type;	
    result_type operator()(int& a, int const& b)const;

};

struct printer
{
    typedef void result_type;
    
    printer(std::ostream& os_);

    result_type operator()( foo const& t)const;    

    private:
    mutable std::ostream& os;
};

//]

//[foo_types
typedef const char* S; // key
typedef foo T; // data
typedef std::pair<S,T> U; //key-data pair
//]

}// example_assign_v2

#endif

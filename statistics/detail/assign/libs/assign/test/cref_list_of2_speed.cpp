//////////////////////////////////////////////////////////////////////////////
// test::cref_list_of2_speed.cpp                               				//
//                                                                          //
//  (C) Copyright 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#include <boost/timer.hpp>
#include <ostream>
#include <vector>
#include <boost/format.hpp>
#include <boost/typeof/typeof.hpp>
#include <boost/assign/list_of.hpp>
#include <boost/assign/cref_list_of2.hpp>
#include <libs/assign/test/cref_list_of2_speed.h>

void test_cref_list_of2_speed(std::ostream& os)
{
	os << "-> test_cref_listof2_speed : " << std::endl;

	typedef boost::timer timer_;
	using namespace boost::assign;

    typedef std::vector<int> vec_ints_;
    typedef boost::format fmt_;
    int a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p,q,r,s,t,u,v,w,x,y,z;
    fmt_ fmt("n = %1%");
    fmt_ fmt2("cref_list_of : t = %1%\n");
    fmt_ fmt3("cref_list_of2 : t = %1%\n");
    os << fmt_(fmt)%4 << std::endl;
    {
        timer_ timer;
        vec_ints_ vec_ints = cref_list_of<4>(a)(b)(c)(d);
        double t = timer.elapsed();
        os << (fmt_(fmt2)%t).str();
    }
    {
        timer_ timer;
        vec_ints_ vec_ints = cref_list_of2(a)(b)(c)(d);
        double t = timer.elapsed();
        os << (fmt_(fmt3)%t).str();
    }
    os << fmt_(fmt)%10 << std::endl;
    {
        timer_ timer;
        vec_ints_ vec_ints = ref_list_of<10>(a)(b)(c)(d)(e)(f)(g)(h)(i)(j);
        double t = timer.elapsed();
        os << (fmt_(fmt2)%t).str();
    }
    {
        timer_ timer;
        vec_ints_ vec_ints = cref_list_of2(a)(b)(c)(d)(e)(f)(g)(h)(i)(j);
        double t = timer.elapsed();
        os << (fmt_(fmt3)%t).str();
    }
    os << fmt_(fmt)%28 << std::endl;
    {
        timer_ timer;
        vec_ints_ vec_ints = cref_list_of<28>(a)(b)(c)(d)(e)(f)(g)(h)(i)(j)(k)(l)(m)(n)(o)(p)(q)(r)(s)(t)(u)(v)(w)(x)(y)(z);
        double t = timer.elapsed();
        os << (fmt_(fmt2)%t).str();
    }
    {
        timer_ timer;
        vec_ints_ vec_ints = cref_list_of2(a)(b)(c)(d)(e)(f)(g)(h)(i)(j)(k)(l)(m)(n)(o)(p)(q)(r)(s)(t)(u)(v)(w)(x)(y)(z);
        double t = timer.elapsed();
        os << (fmt_(fmt3)%t).str();
    }
    os << fmt_(fmt)%112 << std::endl;
    {
        timer_ timer;
        vec_ints_ vec_ints = cref_list_of<112>(a)(b)(c)(d)(e)(f)(g)(h)(i)(j)(k)(l)(m)(n)(o)(p)(q)(r)(s)(t)(u)(v)(w)(x)(y)(z)
        (a)(b)(c)(d)(e)(f)(g)(h)(i)(j)(k)(l)(m)(n)(o)(p)(q)(r)(s)(t)(u)(v)(w)(x)(y)(z)
        (a)(b)(c)(d)(e)(f)(g)(h)(i)(j)(k)(l)(m)(n)(o)(p)(q)(r)(s)(t)(u)(v)(w)(x)(y)(z)
        (a)(b)(c)(d)(e)(f)(g)(h)(i)(j)(k)(l)(m)(n)(o)(p)(q)(r)(s)(t)(u)(v)(w)(x)(y)(z);
        double t = timer.elapsed();
        os << (fmt_(fmt2)%t).str();
    }
    {
        timer_ timer;
        vec_ints_ vec_ints = cref_list_of2(a)(b)(c)(d)(e)(f)(g)(h)(i)(j)(k)(l)(m)(n)(o)(p)(q)(r)(s)(t)(u)(v)(w)(x)(y)(z)
        (a)(b)(c)(d)(e)(f)(g)(h)(i)(j)(k)(l)(m)(n)(o)(p)(q)(r)(s)(t)(u)(v)(w)(x)(y)(z)
        (a)(b)(c)(d)(e)(f)(g)(h)(i)(j)(k)(l)(m)(n)(o)(p)(q)(r)(s)(t)(u)(v)(w)(x)(y)(z)
        (a)(b)(c)(d)(e)(f)(g)(h)(i)(j)(k)(l)(m)(n)(o)(p)(q)(r)(s)(t)(u)(v)(w)(x)(y)(z);
        double t = timer.elapsed();
        os << (fmt_(fmt3)%t).str();
    }
    
    os << std::endl;
} 
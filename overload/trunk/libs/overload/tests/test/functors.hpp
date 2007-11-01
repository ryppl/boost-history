
#ifndef _FUNCTORS_HPP_
#define _FUNCTORS_HPP_


#include <string>
#include <iostream>
#include <boost/function.hpp>

///////////////////////////////////////////////////////////////////////////////
void foo0(std::string s1, std::string s2, std::string s3)
{
    std::cout << s1 << s2 << s3  << std::endl;
}

typedef void sig0_t (std::string , std::string , std::string );
typedef sig0_t* f0_t;

///////////////////////////////////////////////////////////////////////////////
struct bar1
{
    int foo1(char ) 
    {
        return 123;
    }
};

typedef int sig1_t (bar1*, char );
typedef int (bar1::* f1_t) (char );

///////////////////////////////////////////////////////////////////////////////
struct bar2
{
    double foo2(int, char ) const
    {
        return 123.456;
    }
};

typedef double sig2_t (const bar2*, int, char );
typedef double (bar2::* f2_t) (int, char ) const;

///////////////////////////////////////////////////////////////////////////////
char foo3(std::string )
{
    return 'x';
}

typedef char sig3_t (std::string );
typedef boost::function<sig3_t> f3_t;


///////////////////////////////////////////////////////////////////////////////
struct foo4
{
    int operator() (char )
    {
        return 123;
    }
};

typedef int sig4_t (char );
typedef foo4 f4_t;

///////////////////////////////////////////////////////////////////////////////
struct foo5
{
    double operator() (int, char ) const
    {
        return 123.456;
    }
};

typedef double sig5_t (int, char );
typedef foo5 f5_t;

///////////////////////////////////////////////////////////////////////////////
struct foo6
{
    double operator() (float x)
    {
        return x + 1;
    }

    double operator() (float x, float y)
    {
        return x + y;
    }
};

typedef double sig6_t (float );
typedef double sig7_t (float, float );
typedef foo6 f6_t;

///////////////////////////////////////////////////////////////////////////////
struct foo7
{
    template<typename T> 
    T operator()(T x)
    {
        return x + 1;
    }
};

typedef int sig8_t (int );
typedef double sig9_t (double );
typedef foo7 f7_t;

///////////////////////////////////////////////////////////////////////////////
int foo8( double & )
{
    return 8;
}

typedef int sig10_t (double& );
typedef sig10_t* f8_t;

///////////////////////////////////////////////////////////////////////////////
struct bar9
{
    int foo9( double** )
    {
        return 9;
    }

    int foo10( double* const& ) const
    {
        return 10;
    }
};
typedef int sig11_t (bar9*, double** );
typedef int (bar9::* f9_t) (double** );
typedef int sig12_t (bar9*, double* const& );
typedef int (bar9::* f10_t) (const double* & ) const;

///////////////////////////////////////////////////////////////////////////////
struct bar11
{
    int foo11( double** ) const
    {
        return 11;
    }
};
typedef int sig13_t (bar11*, double** );
typedef int (bar11::* f11_t) ( double** ) const;

///////////////////////////////////////////////////////////////////////////////
int foo12(int, int )
{
    return 12;
}

template<typename T1, typename T2>
T1 foo12(T1, T2 )
{
    return 122;
}

///////////////////////////////////////////////////////////////////////////////
struct bar13
{
    int foo13(int, int )
    {
        return 13;
    }
    
    template<typename T1, typename T2>
    T1 foo13(T1, T2 )
    {
        return 133;
    }
};

///////////////////////////////////////////////////////////////////////////////
struct foo14
{
    template< typename T1, typename T2 >
    int operator() ( T1, T2 )
    {
        return sizeof(T2);
    }
};
typedef foo14 f14_t;

///////////////////////////////////////////////////////////////////////////////
int inc(int x)
{
    return x + 1;
}

float inc(float x )
{
    return x + 1;
}
typedef int inc_int_sig_t (int );
typedef float inc_float_sig_t (float );

///////////////////////////////////////////////////////////////////////////////
struct multi
{
    int foo(int )
    {
        return 1;
    }

    int foo(int ) const
    {
        return 2;
    }

    float foo(std::string )
    {
        return 3.0f;
    }
};
typedef int foo_int_sig_t (multi*, int );
typedef int foo_int_const_sig_t (const multi*, int );
typedef float foo_float_sig_t (multi*, std::string );



#endif // _FUNCTORS_HPP_


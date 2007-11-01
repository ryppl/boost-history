/*=============================================================================
    Boost Overload    
    Example 2 - Using different types of callable entities
==============================================================================*/


#include <string>

#include <boost/detail/lightweight_test.hpp>

#include "../../../boost/overload/overload.hpp"


using boost::overload;


/* free function */
void foo1(std::string s1, std::string s2, std::string s3)
{
    std::cout << s1 << s2 << s3  << std::endl;
}
typedef void signature1_t (std::string , std::string , std::string );

/* member function */
struct bar2
{
    int foo2(char )
    {
        return 123;
    }
};
typedef int signature2_t (bar2*, char ); // explicit non-const signature

/* free function that will be wrapped by a boost::function */
char bar3(std::string )
{
    return 'x';
}
typedef char signature3_t (std::string );

/* class type function */
struct bar4
{
    double operator() (int, char ) const
    {
        return 123.456;
    }
};
typedef double signature4_t (int, char );

/* another class type function */
struct bar5
{
    int operator() (char )
    {
        return 123;
    }
};
typedef int signature5_t (char );


int main()
{
    overload<signature1_t, signature2_t,
             signature3_t, signature4_t, signature5_t> f;

    /* instance that will be used for invoking the member function */
    bar2 b2;
    /* boost::function object that uses bar3 as callable target */
    boost::function<signature3_t> foo3(&bar3);
    /* function object */
    bar4 foo4;
    /* function object that will be wrapped with boost::ref */
    bar5 foo5;


    f.set(&foo1);
    f.set(&bar2::foo2);
    // sets the callable target of foo3 as callable target of f 
    // tied to the signature 3
    f.set(foo3);                   
    f.set(foo4);
    f.set(boost::ref(foo5));


    f("Hello", " ", "world !");     // calls foo1 and print "Hello world !"
    int     r2 = f(&b2, 'x');       // calls b2.foo2
    char    r3 = f("hi");           // calls bar3
    double  r4 = f(1, 'x');         // calls foo4
    int     r5 = f('x');            // calls foo5

    BOOST_ASSERT(r2 == 123);
    BOOST_ASSERT(r3 == 'x' );
    BOOST_ASSERT(r4 > 123.455 && r4 < 123.457);
    BOOST_ASSERT(r5 == 123);

   return boost::report_errors();
}


/*=============================================================================
    Boost Overload    
    Example 6 - Using helper methods
==============================================================================*/


#include <string>

#include <boost/detail/lightweight_test.hpp>

#include <boost/overload.hpp>


using boost::overload;


void foo1(std::string s1, std::string s2, std::string s3)
{
    std::cout << s1 << s2 << s3  << std::endl;
}
typedef void signature1_t (std::string , std::string , std::string );


int foo2(int x)
{
    return x + 1;
}
typedef int signature2_t (int );


int main()
{
    overload<signature1_t, signature2_t> f(&foo1, &foo2);

    f("Hello", " ", "world !");                // print "Hello world !"

    f.clear<signature1_t>();                     
    BOOST_ASSERT( f.empty<signature1_t>() );   // f has no callable target
                                               // associated with signature 1  
                                        
    boost::function<signature1_t> g(&foo1);    // g has foo1 as callable target
    f.swap_function(g);                        // after swapping f has foo1
                                               // as callable target
    f("I'm ", "back ", "again !");             // associated with signature 1
    BOOST_ASSERT( g.empty() );                 // and g has no callable target

    g = f.get<signature1_t>();                 // g is set to the embedded object
    g("That's ", "all ", "folks !");           // of f with type
                                               // boost::function<signature1_t>

    f.clear_all();                             // now f has no callable target
    BOOST_ASSERT( f.empty_all() );             // associated to any signature

    return boost::report_errors();
}


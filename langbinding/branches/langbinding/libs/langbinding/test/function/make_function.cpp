#include <boost/langbinding/function/make_function.hpp>
#include <cassert>

using namespace boost::langbinding::function;
using namespace boost::langbinding::converter;

int x = 6;
float y = 3.14f;

void f(int x, float y)
{
    assert(x == ::x && y == ::y);
}

void g(int& x, float y)
{
    assert(x == ::x && y == ::y);
}

void h(int* x, float y)
{
    assert(*x == ::x && y == ::y);
}

int main()
{
    arg_conversion args[2] = { 
        0, &x, 0
      , 0, &y, 0
    };

    struct rc_ : result_converter<void>
    {
        void* operator()() { return 0; }
    } rc;
    
    make_function(&f)->invoke(args, rc);
    make_function(&g)->invoke(args, rc);
    make_function(&h)->invoke(args, rc);
}


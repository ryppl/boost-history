#include <boost/langbinding/function/make_function.hpp>
#include <cassert>

using namespace boost::langbinding::function;

int x = 6;
float y = 3.14f;

void f(int x, float y)
{
    assert(x == ::x && y == ::y);
}

#include <iostream>

void* construct(void* src, void* storage)
{
    return new (storage) int(6);
}

int main()
{
    boost::langbinding::converter::arg_conversion args[2] = { 
        { 0, 0, &construct }
      , { 0, &y, 0 }
    };

    struct rc_ : result_converter<void>
    {
        void* operator()() { return 0; }
    } rc;
    
    make_function(&f)->invoke(args, rc);
}


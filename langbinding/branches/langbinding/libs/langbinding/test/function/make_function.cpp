#include <iostream>

#include <boost/langbinding/function/make_function.hpp>

using namespace boost::langbinding::function;

void f(int x, float y)
{
    std::cout << x << ", " << y << "\n";
}

void g(int& x, float y)
{
    std::cout << x << ", " << y << "\n";
}

void h(int* x, float y)
{
    std::cout << *x << ", " << y << "\n";
}

int main()
{
    int x = 6;
    float y = 3.14f;
    void* args[2] = { &x, &y };

    make_function(&f)->invoke(args, 0);
    make_function(&g)->invoke(args, 0);
    make_function(&h)->invoke(args, 0);
}


#include <boost/langbinding/function/make_function.hpp>
#include <cassert>

using namespace boost::langbinding::function;
using namespace boost::langbinding::converter;
using namespace boost::langbinding;

struct dummy_plugin : backend::plugin
{
    dummy_plugin() {}

 private:
    // plugin implementations
    void* call(
        void* function
      , backend::call_xxx_data& signature_constants
      , void* result_storage
      , void* const* arg_storage) const
    {
        return result_storage;
    }

    backend::override
    find_override(char const* function_name, backend::class_instance const& instance) const
    {
        backend::override result = {0, 0};
        return result;
    } 
} dummy;

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

struct X { virtual ~X() {} };

std::auto_ptr<X> h(int* x, float y)
{
    assert(*x == ::x && y == ::y);
    return std::auto_ptr<X>(new X);
}

void* to_xxx(void* src, holder_installer const& installer)
{
    return 0;
}

int main()
{
    from_xxx_data args[2] = { 
        0, &x, 0
      , 0, &y, 0
    };

    registered<
        std::auto_ptr<X>
    >::instance.get(dummy.id()).to_xxx = &to_xxx;

    make_function(&f)->invoke(dummy, args);
    make_function(&g)->invoke(dummy, args);
    make_function(&h)->invoke(dummy, args);
}


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

    make_function(&f)->invoke(dummy, args);
}


// Copyright Daniel Wallin 2004. Use, modification and distribution is
// subject to the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <boost/langbinding/function/result_converter.hpp>

using namespace boost::langbinding::backend;
using namespace boost::langbinding::function;
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

struct X {};

void* convert(void* src, holder_installer const& installer)
{
    char storage[128];
    installer.install(src, class_instance(0, 0), storage);

    classes::instance_link* instance = (classes::instance_link*)storage;
    assert(instance->find(util::type_id<X>()));
    
    return 0;
}

int main()
{
    result_converter<X> cv(&convert, dummy);
    cv(X());
}

// Copyright Daniel Wallin 2004. Use, modification and distribution is
// subject to the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <boost/langbinding/function/make_function.hpp>
#include <boost/langbinding/converter/registry.hpp>

#include <boost/lexical_cast.hpp>
#include <cassert>
#include <vector>
#include <list>

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

struct dispatcher
{
    struct overload
    {
        invoker* invoke;
        std::vector<registry::backend_registration*> converters;
    };

    std::list<overload> overloads;

    void add(std::auto_ptr<invoker> p)
    {
        overload o;
        o.converters.resize(p->arity());

        argument_type const* args = p->arguments();
        for (int i = 0; i < p->arity(); ++i)
        {
            o.converters[i] = &registry::acquire(args[i].type).get(dummy.id());
        }

        o.invoke = p.release();
        overloads.push_back(o);
    }

    void execute(int arity, std::string* args)
    {
        void* arg_ptrs[10];
        arg_conversion converted_args[10];

        for (void** arg = arg_ptrs; arg != arg_ptrs + arity; ++arg)
        {
            *arg = args++;
        }
       
        for (std::list<overload>::const_iterator i(overloads.begin()),
            end(overloads.end()); i != end; ++i)
        {
            bool found = true;

            for (int j = 0; j < i->converters.size(); ++j)
            {
                converted_args[j] = registry::convert(
                    arg_ptrs[j]
                  , i->converters[j]->rvalue_converters
                );

                if (!converted_args[j].convertible)
                {
                    found = false;
                    break;
                }
            }

            if (found)
            {
                i->invoke->invoke(dummy, converted_args);
                return;
            }
        }
    }
};

void f(int x, int y)
{
}

void g(int x, float y)
{
}

template<class T>
struct from_str
{
    from_str()
    {
        registry::insert(
            dummy.id(), true, util::type_id<T>(), &from_str<T>::convertible);
    }
    
    static void* construct(void* src_, void* storage)
    {
        std::string const& src = *static_cast<std::string const*>(src_);
        return new (storage) T(boost::lexical_cast<T>(src));
    }

    static arg_conversion convertible(void* src_)
    {
        std::string const& src = *static_cast<std::string const*>(src_);
        arg_conversion result;
        result.source = src_;
        result.convertible = src_;
        result.construct = &from_str<T>::construct;

        try
        {
            boost::lexical_cast<T>(src);
        }
        catch (boost::bad_lexical_cast&)
        {
            result.convertible = 0;
        }

        return result;
    }
};

int main()
{
    from_str<int>();
    from_str<float>();
    dispatcher d;

    d.add(make_function(&f));
    d.add(make_function(&g));

    std::string args[2] = { "6", "42" };
    d.execute(2, args);
    args[1] = "3.14";
    d.execute(2, args);
}


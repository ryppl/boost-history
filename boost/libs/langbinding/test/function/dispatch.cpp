// Copyright Daniel Wallin 2004. Use, modification and distribution is
// subject to the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <boost/langbinding/function/make_function.hpp>
#include <boost/langbinding/converter/converter.hpp>
#include <boost/langbinding/converter/registry.hpp>

#include <boost/lexical_cast.hpp>
#include <cassert>
#include <vector>
#include <list>

using namespace boost::langbinding::function;
using namespace boost::langbinding::converter;

struct dispatcher
{
    struct overload
    {
        invoker* invoke;
        std::vector<registry::registration*> converters;
    };

    std::list<overload> overloads;

    void add(std::auto_ptr<invoker> p)
    {
        overload o;
        o.converters.resize(p->arity());

        argument_type const* args = p->arguments();
        for (int i = 0; i < p->arity(); ++i)
        {
            o.converters[i] = registry::get(args[i].type);
        }

        o.invoke = p.release();
        overloads.push_back(o);
    }

    struct rc_ : result_converter<void>
    {
        void* operator()() { return 0; }
    };    
    
    void execute(int arity, std::string* args)
    {
        void* arg_ptrs[10];
        arg_conversion converted_args[10];

        for (void** arg = arg_ptrs; arg != arg_ptrs + arity; ++arg)
        {
            *arg = args++;
        }
       
        rc_ rc;
        
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
                i->invoke->invoke(converted_args, rc);
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
struct from_str : converter<from_str<T>, std::string, T>
{
    static void construct(std::string const& src, void* storage)
    {
        new (storage) T(boost::lexical_cast<T>(src));
    }

    static void* convertible(std::string const& src)
    {
        try
        {
            boost::lexical_cast<T>(src);
        }
        catch (boost::bad_lexical_cast&)
        {
            return 0;
        }

        return yes;
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


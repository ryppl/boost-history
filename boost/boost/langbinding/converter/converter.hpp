// Copyright Daniel Wallin 2004. Use, modification and distribution is
// subject to the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef CONVERTER_040922_HPP
#define CONVERTER_040922_HPP

#include <boost/langbinding/converter/registry.hpp>
#include <boost/langbinding/converter/converter_function.hpp>
#include <boost/type_traits/is_pointer.hpp>
#include <boost/mpl/if.hpp>

namespace boost { namespace langbinding { namespace converter {

template<class Derived, class Src, class T>
struct converter
{
public:
    converter()
    {
        registry::insert(true, util::type_id<T>(), &convert);
    }

private:
    typedef typename mpl::if_<
        is_pointer<Src>
      , Src
      , Src*
    >::type src_ptr;

    template<class U>
    static U* dereference(U* p, mpl::true_)
    {
        return p;
    }

    template<class U>
    static U& dereference(U* p, mpl::false_)
    {
        return *p;
    }

    static arg_conversion convert(void* src)
    {
        void* convertible = Derived::convertible(
            dereference(static_cast<src_ptr>(src), is_pointer<Src>())
        );

        arg_conversion result;
        result.convertible = convertible;
        result.source = src;

        if (convertible == yes)
        {
            result.construct = &construct;
        }
        else
        {
            result.construct = 0;
        }

        return result;
    }

    static void* construct(void* src, void* storage)
    {
        Derived::construct(
            dereference(static_cast<src_ptr>(src), is_pointer<Src>())
          , storage
        );

        return storage;
    }

protected:
    static void* const yes;
};

template<class D, class S, class T>
void* const converter<D, S, T>::yes = (void*)-1;

}}} // namespace boost::langbinding::function

#endif // CONVERTER_040922_HPP


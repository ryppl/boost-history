#ifndef ARG_EXTRACTOR_040916_HPP
#define ARG_EXTRACTOR_040916_HPP

#include <boost/detail/indirect_traits.hpp>
#include <boost/type_traits/add_reference.hpp>
#include <boost/type_traits/is_pointer.hpp>
#include <boost/mpl/identity.hpp>
#include <boost/mpl/eval_if.hpp>

namespace boost { namespace langbinding { namespace function { namespace aux {

struct extractor_base
{
    extractor_base(void* p) : data(p) {}

    void* data;
};

template<class T>
struct ptr_extractor : extractor_base
{
    ptr_extractor(void* p) : extractor_base(p) {}

    T operator()() const
    {
        return static_cast<T>(data);
    }
};

template<class T>
struct ref_extractor : extractor_base
{
    ref_extractor(void* p) : extractor_base(p) {}

    T operator()() const
    {
        return extract(data, (T(*)())0);
    }

    template<class U>
    static T extract(void* ptr, U&(*)())
    {
        return *static_cast<U*>(ptr);
    }
};

template<class T>
struct rvalue_extractor : extractor_base
{
    typedef typename add_reference<T>::type result_type;

    rvalue_extractor(void* p) : extractor_base(p) 
    {
    }

    result_type operator()() const
    {
        return extract(data, (result_type(*)())0);
    }

    template<class U>
    static result_type extract(void* ptr, U&(*)())
    {
        return *static_cast<U*>(ptr);
    }
};

template<class T>
struct arg_extractor
{
    typedef typename mpl::eval_if<
        is_pointer<T>
      , mpl::identity<ptr_extractor<T> >
      , mpl::eval_if<
            boost::detail::is_reference_to_non_const<T>
          , mpl::identity<ref_extractor<T> >
          , mpl::identity<rvalue_extractor<T> >
        >
    >::type type;
    };

}}}} // namespace boost::langbinding::function::aux

#endif // ARG_EXTRACTOR_040916_HPP


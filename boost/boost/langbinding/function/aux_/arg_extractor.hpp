#ifndef ARG_EXTRACTOR_040916_HPP
#define ARG_EXTRACTOR_040916_HPP

#include <boost/detail/indirect_traits.hpp>
#include <boost/type_traits/add_reference.hpp>
#include <boost/type_traits/is_pointer.hpp>
#include <boost/mpl/identity.hpp>
#include <boost/mpl/eval_if.hpp>

#include <boost/langbinding/function/aux_/referent_storage.hpp>

namespace boost { namespace langbinding { namespace function { namespace aux {

// The extractors a responsible for the second stage of converting an
// argument. This involves casting from_xxx_data::convertible to the
// right type, or possibly using from_xxx_data::construct to construct
// a temporary.    
struct extractor_base
{
    extractor_base(void* p) : data(p) {}

    void* data;
};

template<class T>
struct ptr_extractor : extractor_base
{
    ptr_extractor(converter::from_xxx_data const& x) 
        : extractor_base(x.convertible) {}

    T operator()() const
    {
        return static_cast<T>(data);
    }
};

template<class T>
struct ref_extractor : extractor_base
{
    ref_extractor(converter::from_xxx_data const& x) 
        : extractor_base(x.convertible) {}

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
struct rvalue_extractor
{
    typedef typename add_reference<T>::type result_type;

    rvalue_extractor(converter::from_xxx_data const& x)
    {
        if (x.construct)
        {
            result = x.construct(x.source, storage.bytes);
        }
        else
        {
            result = x.convertible;
        }
    }

    ~rvalue_extractor()
    {
        if (result == storage.bytes)
        {
            destroy(result, (result_type(*)())0);
        }
    }

    result_type operator()() const
    {
        return extract(result, (result_type(*)())0);
    }

    template<class U>
    static result_type extract(void* ptr, U&(*)())
    {
        return *static_cast<U*>(ptr);
    }

    template<class U>
    static void destroy(void* p, U&(*)())
    {
        static_cast<U*>(p)->~U();
    }

    void* result;
    typename referent_storage<result_type>::type storage;
};

template<class T>
struct arg_extractor
    : mpl::eval_if<
          is_pointer<T>
        , mpl::identity<ptr_extractor<T> >
        , mpl::if_<
              boost::detail::is_reference_to_non_const<T>
            , ref_extractor<T>
            , rvalue_extractor<T>
          >
      >
{
};

}}}} // namespace boost::langbinding::function::aux

#endif // ARG_EXTRACTOR_040916_HPP


// Copyright David Abrahams 2003. Permission to copy, use,
// modify, sell and distribute this software is granted provided this
// copyright notice appears in all copies. This software is provided
// "as is" without express or implied warranty, and with no claim as
// to its suitability for any purpose.
#include <boost/iterator/iterator_adaptor.hpp>
#include <boost/static_assert.hpp>
#include <boost/type.hpp>

struct X { int a; };

#ifdef BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION
namespace boost { namespace detail {
template<> struct iterator_traits<X*>
  : ptr_iter_traits<X> {};
}}
#endif 

struct Xiter : boost::iterator_adaptor<Xiter,X*>
{
    Xiter();
    Xiter(X* p) : boost::iterator_adaptor<Xiter, X*>(p) {}
};

void take_xptr(X*) {}
void operator_arrow_test()
{
    // check that the operator-> result is a pointer for lvalue iterators
    X x;
    take_xptr(Xiter(&x).operator->());
}

template <class T, class U>
struct static_assert_same
#ifdef BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION
{
    template <class V>
    static int check(V, V);
    enum { value = sizeof(check(boost::type<T>(), boost::type<U>())) };
}
#endif 
;

#ifndef BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION
template <class T>
struct static_assert_same<T,T>
{
    enum { value };
};
#endif

template <class T, class U, class Min>
struct static_assert_min_cat
  : static_assert_same<
       typename boost::detail::minimum_category<T,U>::type, Min
    >
{};

void category_test()
{
    using namespace boost;
    using namespace boost::detail;
    
    BOOST_STATIC_ASSERT((
        !is_tag<
            input_output_iterator_tag
          , std::input_iterator_tag>::value));
    
    BOOST_STATIC_ASSERT((
        !is_tag<
            input_output_iterator_tag
          , std::output_iterator_tag>::value));
    
    BOOST_STATIC_ASSERT((
        is_tag<
            std::input_iterator_tag
          , input_output_iterator_tag>::value));
    
    BOOST_STATIC_ASSERT((
        is_tag<
            std::output_iterator_tag
          , input_output_iterator_tag>::value));

    BOOST_STATIC_ASSERT((
        is_tag<
            input_output_iterator_tag
          , std::forward_iterator_tag>::value));

    int test = static_assert_min_cat<
        std::input_iterator_tag,input_output_iterator_tag, std::input_iterator_tag
    >::value;

    test = static_assert_min_cat<
        input_output_iterator_tag,std::input_iterator_tag, std::input_iterator_tag
    >::value;

    test = static_assert_min_cat<
        input_output_iterator_tag,std::forward_iterator_tag, input_output_iterator_tag
    >::value;

    test = static_assert_min_cat<
        std::input_iterator_tag,std::forward_iterator_tag, std::input_iterator_tag
    >::value;

    test = static_assert_min_cat<
        std::input_iterator_tag,std::random_access_iterator_tag, std::input_iterator_tag
    >::value;

    test = static_assert_min_cat<
        std::output_iterator_tag,std::random_access_iterator_tag, std::output_iterator_tag
    >::value;
}

int main()
{
    category_test();
    operator_arrow_test();
    return 0;
}


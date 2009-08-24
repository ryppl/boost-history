#ifndef BOOST_ITERATOR_JOIN_ITERATOR_HPP
#define BOOST_ITERATOR_JOIN_ITERATOR_HPP

#include <boost/iterator/iterator_facade.hpp>
#include <boost/range.hpp>

#include <boost/mpl/front.hpp>
#include <boost/mpl/fold.hpp>
#include <boost/mpl/size.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/push_back.hpp>
#include <boost/mpl/transform_view.hpp>

#include <boost/fusion/include/mpl.hpp> 
#include <boost/fusion/tuple.hpp>
#include <boost/fusion/adapted.hpp>
#include <boost/fusion/algorithm/transformation/transform.hpp>
#include <boost/fusion/include/as_vector.hpp>

#include <boost/variant.hpp>

#include <boost/utility/enable_if.hpp>
#include <boost/utility/common_type.hpp>

#include <boost/preprocessor/repetition/enum_params.hpp>
#include <boost/preprocessor/repetition/enum_binary_params.hpp>
#include <boost/preprocessor/repetition/repeat.hpp>
#include <boost/preprocessor/repetition/repeat_from_to.hpp>


namespace boost
{
    
namespace detail
{
    template<typename T, int N>
    struct wrapper : iterator_facade<
        wrapper<T, N>,
        typename std::iterator_traits<T>::value_type,
        typename std::iterator_traits<T>::iterator_category,
        typename std::iterator_traits<T>::reference,
        typename std::iterator_traits<T>::difference_type
    >
    {
        static const int index = N;
        
        wrapper() {} // singular
        
        wrapper(const T& t_) : t(t_)
        {
        }
        
    private:
        friend class boost::iterator_core_access;
        
        typename std::iterator_traits<T>::reference
        dereference() const { return *t; }
        
        void increment() { ++t; }
        void decrement() { --t; }
        void advance(typename std::iterator_traits<T>::difference_type n) { t += n; }
        
        typename std::iterator_traits<T>::difference_type
        distance_to(const wrapper& other) const { other.t - t; }
        
        template<typename U, int M>
        typename std::iterator_traits<T>::difference_type
        distance_to(const wrapper<U, M>& other) const { other.t - t; }
        
        template<typename U>
        typename std::iterator_traits<T>::difference_type
        distance_to(const U& other) const { other - t; }
        
        bool equal(const wrapper& other) const { return t == other.t; }
        
        template<typename U, int M>
        bool equal(const wrapper<U, M>& other) const { return t == other.t; }
        
        template<typename U>
        bool equal(const U& other) const { return t == other; }
        
        T t;
    };
    
    template<typename F, typename N>
    struct meta_wrapper
    {
        typedef wrapper<F, N::value> type;
    };
    
    template<int N, typename T>
    wrapper<T, N> make_wrapper(const T& t)
    {
        return wrapper<T, N>(t);
    }
    
    template<typename RangeSequence>
    struct deduce_value_type : common_type_seq<
        mpl::transform_view<
            RangeSequence,
            range_value<mpl::_1>
        >
    >
    {
    };
    
    template<typename Iterator>
    struct reference_type
    {
        typedef typename std::iterator_traits<Iterator>::reference type;
    };
    
    template<typename RangeSequence>
    struct deduce_reference : common_type_seq<
        mpl::transform_view<
            RangeSequence,
            reference_type<
                range_iterator<mpl::_1>
            >
        >
    >
    {
    };
    
    template<typename Iterator>
    struct difference_type
    {
        typedef typename std::iterator_traits<Iterator>::difference_type type;
    };
    
    template<typename RangeSequence>
    struct deduce_difference_type : common_type_seq<
        mpl::transform_view<
            RangeSequence,
            difference_type<
                range_iterator<mpl::_1>
            >
        >
    >
    {
    };
    
    template<typename R>
    struct dereference_visitor : static_visitor<R>
    {
        template<typename T>
        R operator()(const T& t) const
        {
            return *t;
        }
    };
    
    template<typename Tuple, typename Variant>
    struct increment_visitor : static_visitor<Variant>
    {
        increment_visitor(Tuple& tuple_) : tuple(tuple_) {}
        
        template<typename T>
        typename enable_if_c<
            T::index+1 == mpl::size<Tuple>::type::value,
            Variant
        >::type operator()(T& t) const
        {
            return ++t;
        }
        
        template<typename T>
        typename disable_if_c<
            T::index+1 == mpl::size<Tuple>::type::value,
            Variant
        >::type operator()(T& t) const
        {
            if(++t == make_wrapper<T::index>(end(fusion::get<T::index>(tuple))))
                return make_wrapper<T::index+1>(
                    begin(
                        fusion::get<T::index+1>(tuple)
                    )
                );
            return t;
        }
        
        Tuple& tuple;
    };
    
    template<typename Tuple, typename Variant>
    struct decrement_visitor : static_visitor<Variant>
    {
        decrement_visitor(Tuple& tuple_) : tuple(tuple_) {}

        template<typename T>
        typename enable_if_c<
            T::index == 0,
            Variant
        >::type
        operator()(T& t) const
        {
            return --t;
        }
        
        template<typename T>
        typename disable_if_c<
            T::index == 0,
            Variant
        >::type
        operator()(T& t) const
        {
            if(t == make_wrapper<T::index>(begin(fusion::get<T::index>(tuple))))
                return make_wrapper<T::index-1>(
                    prior(
                        end(
                            fusion::get<T::index-1>(tuple)
                        )
                    )
                );
            return --t;
        }
        
        Tuple& tuple;
    };
    
    struct equal_visitor : static_visitor<bool>
    {
        template<typename T1, typename T2>
        bool operator()(const T1&, const T2&) const
        {
            return false;
        }
        
        template<typename T>
        bool operator()(const T& lft, const T& rgt) const
        {
            return lft == rgt;
        }
    };
    
} // namespace detail

template<typename Tuple>
struct join_iterator;

template<typename Tuple>
join_iterator<Tuple> make_join_end_iterator(const Tuple& tuple);

/** Iterator that wraps a tuple of ranges and makes it appear as
 * a single concatenated range. */
template<typename Tuple>
struct join_iterator
	: iterator_facade<
		join_iterator<Tuple>,
        typename detail::deduce_value_type<typename fusion::result_of::as_vector<Tuple>::type>::type,
		std::bidirectional_iterator_tag,
        typename detail::deduce_reference<typename fusion::result_of::as_vector<Tuple>::type>::type,
        typename detail::deduce_difference_type<typename fusion::result_of::as_vector<Tuple>::type>::type
	>
{
    join_iterator() {} // singular
    
    join_iterator(const Tuple& t_) : t(t_), v(detail::make_wrapper<0>(begin(fusion::get<0>(t))))
    {
    }
    
private:
    join_iterator(const Tuple& t_, bool) : t(t_), v(detail::make_wrapper<mpl::size<FusionTuple>::value-1>(end(fusion::get<mpl::size<FusionTuple>::value-1>(t))))
    {
    }
    
    friend join_iterator<Tuple> make_join_end_iterator<Tuple>(const Tuple& tuple);
	friend class boost::iterator_core_access;
    typedef typename fusion::result_of::as_vector<Tuple>::type FusionTuple;
    typedef typename detail::deduce_reference<FusionTuple>::type Reference;

	Reference dereference() const
	{
		return apply_visitor(
            detail::dereference_visitor<Reference>(),
            v
        );
	}
	
	void increment()
	{
        v = apply_visitor(detail::increment_visitor<FusionTuple, Variant>(t), v);
	}
	
	void decrement()
	{
        v = apply_visitor(detail::decrement_visitor<FusionTuple, Variant>(t), v);
	}
	
	bool equal(const join_iterator& other) const
	{
		return apply_visitor(detail::equal_visitor(), v, other.v);
	}
    
    FusionTuple t;
    typedef typename make_variant_over<
        typename mpl::fold<
            FusionTuple,
            mpl::vector<>,
            mpl::push_back<
                mpl::_1,
                mpl::quote2<detail::meta_wrapper>::apply<
                    range_iterator<mpl::_2>,
                    mpl::size<mpl::_1>
                >
            >
        >::type
    >::type Variant;
    Variant v;
};

template<typename Tuple>
join_iterator<Tuple> make_join_iterator(const Tuple& tuple)
{
    return join_iterator<Tuple>(tuple);
}

template<typename Tuple>
join_iterator<Tuple> make_join_end_iterator(const Tuple& tuple)
{
    return join_iterator<Tuple>(tuple, true);
}

template<typename Tuple>
iterator_range<
    join_iterator<Tuple>
> joined(const Tuple& tuple)
{
    return make_iterator_range(
        make_join_iterator(tuple),
        make_join_end_iterator(tuple)
    );
}

#ifdef BOOST_UNICODE_DOXYGEN_INVOKED
template<typename... T>
iterator_range<
    join_iterator< tuple<T...> >
> joined_n(const T&... n);
#else
#define BOOST_ITERATOR_JOIN_DEF(z, n, data) \
template<BOOST_PP_ENUM_PARAMS(n, typename T)> \
iterator_range< \
    join_iterator< tuple<BOOST_PP_REPEAT(n, BOOST_ITERATOR_JOIN_DEF_A, ~)> > \
> joined_n(BOOST_PP_ENUM_BINARY_PARAMS(n, const T, & t)) \
{ \
    return joined(make_tuple( \
        BOOST_PP_REPEAT(n, BOOST_ITERATOR_JOIN_DEF_B, ~) \
    )); \
} \
    /**/
#define BOOST_ITERATOR_JOIN_DEF_A(z, n, data) \
        BOOST_PP_COMMA_IF(n) iterator_range<typename range_iterator<const BOOST_PP_CAT(T, n)>::type> \
    /**/
#define BOOST_ITERATOR_JOIN_DEF_B(z, n, data) \
        BOOST_PP_COMMA_IF(n) make_iterator_range(BOOST_PP_CAT(t, n)) \
    /**/
BOOST_PP_REPEAT_FROM_TO(1, 5, BOOST_ITERATOR_JOIN_DEF, ~)
#endif

} // namespace boost

#endif

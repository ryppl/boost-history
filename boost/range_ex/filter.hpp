///////////////////////////////////////////////////////////////////////////////
/// \file filter.hpp
///   Defines the filter range adaptor, as well as the make_filter_range() helper
//
///////////////////////////////////////////////////////////////////////////////
#ifndef RANGE_FILTER_EN_12_09_2004_HPP
#define RANGE_FILTER_EN_12_09_2004_HPP

#include <boost/iterator/filter_iterator.hpp>
#include <boost/range/result_iterator.hpp>
#include "./iterator_cast.hpp"
#include "./detail/adaptor_base.hpp"
#include "./detail/adl_begin_end.hpp"

namespace boost
{

/// \brief filters a range using the specified predicate
///
/// filters a range using the specified predicate
///
template<typename FwdRng,typename Pred>
boost::iterator_range<
    ::boost::filter_iterator<
        Pred
      , BOOST_DEDUCED_TYPENAME ::boost::range_iterator<FwdRng>::type
    >
>
make_filter_range(FwdRng& rng,Pred pred)
{
    return ::boost::make_iterator_range(
        ::boost::make_filter_iterator(pred,detail::adl_begin(rng),detail::adl_end(rng))
      , ::boost::make_filter_iterator(pred,detail::adl_end(rng),detail::adl_end(rng))
    );
}

/// \overload
template<typename FwdRng,typename Pred>
boost::iterator_range<
    ::boost::filter_iterator<
        Pred
      , BOOST_DEDUCED_TYPENAME ::boost::range_const_iterator<FwdRng>::type
    >
>
make_filter_range(FwdRng const& rng,Pred pred)
{
    return ::boost::make_iterator_range(
        ::boost::make_filter_iterator(pred,detail::adl_begin(rng),detail::adl_end(rng))
      , ::boost::make_filter_iterator(pred,detail::adl_end(rng),detail::adl_end(rng))
    );
}

/// filter_range_adaptor
///   INTERNAL ONLY
struct filter_range_adaptor
{
    template<typename Rng,typename Args>
    struct apply
    {
        BOOST_STATIC_ASSERT((::boost::tuples::length<Args>::value==1));
        typedef BOOST_DEDUCED_TYPENAME ::boost::range_result_iterator<Rng>::type iterator;
        typedef ::boost::filter_iterator<
            BOOST_DEDUCED_TYPENAME ::boost::tuples::element<0,Args>::type
          , iterator
        >
        type;
    };

    template<typename Rng,typename Args>
    static BOOST_DEDUCED_TYPENAME apply<Rng,Args>::type
    begin(Rng & rng,Args const & args)
    {
        return ::boost::make_filter_iterator(
            ::boost::tuples::get<0>(args)
          , detail::adl_begin(rng)
          , detail::adl_end(rng)
        );
    }

    template<typename Rng,typename Args>
    static BOOST_DEDUCED_TYPENAME apply<Rng,Args>::type
    end(Rng & rng,Args const & args)
    {
        return ::boost::make_filter_iterator(
            ::boost::tuples::get<0>(args)
          , detail::adl_end(rng)
          , detail::adl_end(rng)
        );
    }
};

namespace adaptor
{
    namespace
    {
        /// \brief the filter range adaptor
        ///
        /// the filter range adaptor
        ///
        range_adaptor<filter_range_adaptor> const filter;
    }
}

} // namespace boost

#endif

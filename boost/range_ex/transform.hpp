///////////////////////////////////////////////////////////////////////////////
//
// File: transform.hpp
//
///////////////////////////////////////////////////////////////////////////////
#ifndef RANGE_TRANSFORM_EN_12_09_2004_HPP
#define RANGE_TRANSFORM_EN_12_09_2004_HPP

#include <boost/iterator/transform_iterator.hpp>
#include <boost/range/result_iterator.hpp>
#include "./iterator_cast.hpp"
#include "./detail/adaptor_base.hpp"
#include "./detail/adl_begin_end.hpp"

namespace boost
{

///////////////////////////////////////////////////////////////////////////////
// make_transform_range
//
template<typename FwdRng,typename UnaryFunc>
boost::iterator_range<
    ::boost::transform_iterator<
        UnaryFunc
      , BOOST_DEDUCED_TYPENAME ::boost::range_iterator<FwdRng>::type
    >
>
make_transform_range(FwdRng& rng, UnaryFunc fun)
{
    return ::boost::make_iterator_range(
        ::boost::make_transform_iterator(detail::adl_begin(rng), fun)
      , ::boost::make_transform_iterator(detail::adl_end(rng), fun)
    );
}

template<typename FwdRng,typename UnaryFunc>
boost::iterator_range<
    ::boost::transform_iterator<
        UnaryFunc
      , BOOST_DEDUCED_TYPENAME ::boost::range_const_iterator<FwdRng>::type
    >
>
make_transform_range(FwdRng const& rng, UnaryFunc fun)
{
    return ::boost::make_iterator_range(
        ::boost::make_transform_iterator(detail::adl_begin(rng), fun)
      , ::boost::make_transform_iterator(detail::adl_end(rng), fun)
    );
}

///////////////////////////////////////////////////////////////////////////////
// transform_range_adaptor
//
struct transform_range_adaptor
{
    template<typename Rng,typename Args>
    struct apply
    {
        BOOST_STATIC_ASSERT((::boost::tuples::length<Args>::value==1));
        typedef BOOST_DEDUCED_TYPENAME ::boost::range_result_iterator<Rng>::type iterator;
        typedef ::boost::transform_iterator<
            BOOST_DEDUCED_TYPENAME ::boost::tuples::element<0,Args>::type
          , iterator
        >
        type;
    };

    template<typename Rng,typename Args>
    static BOOST_DEDUCED_TYPENAME apply<Rng,Args>::type
    begin(Rng & rng, Args const & args)
    {
        return ::boost::make_transform_iterator(
            detail::adl_begin(rng)
          , ::boost::tuples::get<0>(args)
        );
    }

    template<typename Rng,typename Args>
    static BOOST_DEDUCED_TYPENAME apply<Rng,Args>::type
    end(Rng & rng, Args const & args)
    {
        return ::boost::make_transform_iterator(
            detail::adl_end(rng)
          , ::boost::tuples::get<0>(args)
        );
    }
};

///////////////////////////////////////////////////////////////////////////////
// transform
//
namespace adaptor
{
    namespace
    {
        range_adaptor<transform_range_adaptor> const transform;
    }
}

} // namespace boost

#endif

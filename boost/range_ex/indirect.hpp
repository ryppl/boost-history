///////////////////////////////////////////////////////////////////////////////
//
// File: indirect.hpp
//
///////////////////////////////////////////////////////////////////////////////
#ifndef RANGE_INDIRECT_EN_12_09_2004_HPP
#define RANGE_INDIRECT_EN_12_09_2004_HPP

#include <boost/iterator/indirect_iterator.hpp>
#include <boost/range/result_iterator.hpp>
#include "./iterator_cast.hpp"
#include "./detail/adaptor_base.hpp"
#include "./detail/adl_begin_end.hpp"

namespace boost
{

///////////////////////////////////////////////////////////////////////////////
// make_indirect_range
//
template<typename FwdRng>
boost::iterator_range<
    ::boost::indirect_iterator<
        BOOST_DEDUCED_TYPENAME ::boost::range_iterator<FwdRng>::type
    >
>
make_indirect_range(FwdRng& rng)
{
    return ::boost::make_iterator_range(
        ::boost::make_indirect_iterator(detail::adl_begin(rng))
      , ::boost::make_indirect_iterator(detail::adl_end(rng))
    );
}

template<typename FwdRng>
boost::iterator_range<
    ::boost::indirect_iterator<
        BOOST_DEDUCED_TYPENAME ::boost::range_const_iterator<FwdRng>::type
    >
>
make_indirect_range(FwdRng const& rng)
{
    return ::boost::make_iterator_range(
        ::boost::make_indirect_iterator(detail::adl_begin(rng))
      , ::boost::make_indirect_iterator(detail::adl_end(rng))
    );
}

///////////////////////////////////////////////////////////////////////////////
// indirect_range_adaptor
//
struct indirect_range_adaptor
{
    template<typename Rng,typename Args>
    struct apply
    {
        BOOST_STATIC_ASSERT((::boost::tuples::length<Args>::value==0));
        typedef BOOST_DEDUCED_TYPENAME ::boost::range_result_iterator<Rng>::type iterator;
        typedef ::boost::indirect_iterator<iterator> type;
    };

    template<typename Rng,typename Args>
    static BOOST_DEDUCED_TYPENAME apply<Rng,Args>::type
    begin(Rng & rng, Args)
    {
        return ::boost::make_indirect_iterator(detail::adl_begin(rng));
    }

    template<typename Rng,typename Args>
    static BOOST_DEDUCED_TYPENAME apply<Rng,Args>::type
    end(Rng & rng, Args)
    {
        return ::boost::make_indirect_iterator(detail::adl_end(rng));
    }
};

///////////////////////////////////////////////////////////////////////////////
// indirect
//
namespace adaptor
{
    namespace
    {
        range_adaptor<indirect_range_adaptor> const indirect;
    }
}

} // namespace boost

#endif

///////////////////////////////////////////////////////////////////////////////
/// \file indirect.hpp
///   Defines the indirect range adaptor, as well as the make_indirect_range() helper
//
///////////////////////////////////////////////////////////////////////////////
#ifndef RANGE_INDIRECT_EN_12_09_2004_HPP
#define RANGE_INDIRECT_EN_12_09_2004_HPP

#include <boost/static_assert.hpp>
#include <boost/iterator/indirect_iterator.hpp>
#include <boost/range/result_iterator.hpp>
#include "./iterator_cast.hpp"
#include "./detail/adaptor_base.hpp"
#include "./detail/adl_begin_end.hpp"

namespace boost
{

/// \brief produced an indirect range
///
/// produced an indirect range, where each element in the new
/// range is the result of a dereference of the corresponding
/// element in the original range.
///
template<typename FwdRng>
boost::iterator_range<
    boost::indirect_iterator<
        BOOST_DEDUCED_TYPENAME boost::range_iterator<FwdRng>::type
    >
>
make_indirect_range(FwdRng& rng)
{
    return boost::make_iterator_range(
        boost::make_indirect_iterator(range_ex_detail::adl_begin(rng))
      , boost::make_indirect_iterator(range_ex_detail::adl_end(rng))
    );
}

/// \overload
template<typename FwdRng>
boost::iterator_range<
    boost::indirect_iterator<
        BOOST_DEDUCED_TYPENAME boost::range_const_iterator<FwdRng>::type
    >
>
make_indirect_range(FwdRng const& rng)
{
    return boost::make_iterator_range(
        boost::make_indirect_iterator(range_ex_detail::adl_begin(rng))
      , boost::make_indirect_iterator(range_ex_detail::adl_end(rng))
    );
}

/// indirect_range_adaptor
///   INTERNAL ONLY
struct indirect_range_adaptor
{
    template<typename Rng,typename Args>
    struct apply
    {
        BOOST_STATIC_ASSERT((boost::tuples::length<Args>::value==0));
        typedef BOOST_DEDUCED_TYPENAME boost::range_result_iterator<Rng>::type iterator;
        typedef boost::indirect_iterator<iterator> type;
    };

    template<typename Rng,typename Args>
    static BOOST_DEDUCED_TYPENAME apply<Rng,Args>::type
    begin(Rng & rng, Args)
    {
        return boost::make_indirect_iterator(range_ex_detail::adl_begin(rng));
    }

    template<typename Rng,typename Args>
    static BOOST_DEDUCED_TYPENAME apply<Rng,Args>::type
    end(Rng & rng, Args)
    {
        return boost::make_indirect_iterator(range_ex_detail::adl_end(rng));
    }
};

namespace adaptor
{
    namespace
    {
        /// \brief the indirect range adaptor
        ///
        /// the indirect range adaptor
        ///
        range_adaptor<indirect_range_adaptor> const indirect;
    }
}

} // namespace boost

#endif

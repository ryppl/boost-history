///////////////////////////////////////////////////////////////////////////////
/// \file reverse.hpp
///   Defines the reverse range adaptor, as well as the make_reverse_range() helper
//
///////////////////////////////////////////////////////////////////////////////
#ifndef RANGE_REVERSE_EN_12_09_2004_HPP
#define RANGE_REVERSE_EN_12_09_2004_HPP

#include <boost/static_assert.hpp>
#include <boost/iterator/reverse_iterator.hpp>
#include <boost/range/result_iterator.hpp>
#include "./iterator_cast.hpp"
#include "./detail/adaptor_base.hpp"
#include "./detail/adl_begin_end.hpp"

namespace boost
{

/// \brief produces a range that is the reverse of the original
///
/// produces a range that is the reverse of the original
///
template<typename FwdRng>
boost::iterator_range<
    boost::reverse_iterator<
        BOOST_DEDUCED_TYPENAME boost::range_iterator<FwdRng>::type
    >
>
make_reverse_range(FwdRng& rng)
{
    return boost::make_iterator_range(
        boost::make_reverse_iterator(range_ex_detail::adl_end(rng))
      , boost::make_reverse_iterator(range_ex_detail::adl_begin(rng))
    );
}

/// \overload
template<typename FwdRng>
boost::iterator_range<
    boost::reverse_iterator<
        BOOST_DEDUCED_TYPENAME boost::range_const_iterator<FwdRng>::type
    >
>
make_reverse_range(FwdRng const& rng)
{
    return boost::make_iterator_range(
        boost::make_reverse_iterator(range_ex_detail::adl_end(rng))
      , boost::make_reverse_iterator(range_ex_detail::adl_begin(rng))
    );
}

/// reverse_range_adaptor
///   INTERNAL ONLY
struct reverse_range_adaptor
{
    template<typename Rng,typename Args>
    struct apply
    {
        BOOST_STATIC_ASSERT((boost::tuples::length<Args>::value==0));
        typedef BOOST_DEDUCED_TYPENAME boost::range_result_iterator<Rng>::type iterator;
        typedef boost::reverse_iterator<iterator> type;
    };


    template<typename Rng,typename Args>
    static BOOST_DEDUCED_TYPENAME apply<Rng,Args>::type
    begin(Rng & rng, Args)
    {
        return boost::make_reverse_iterator(range_ex_detail::adl_end(rng));
    }

    template<typename Rng,typename Args>
    static BOOST_DEDUCED_TYPENAME apply<Rng,Args>::type
    end(Rng & rng, Args)
    {
        return boost::make_reverse_iterator(range_ex_detail::adl_begin(rng));
    }
};

namespace adaptor
{
    namespace
    {
        /// \brief the reverse range adaptor
        ///
        /// the reverse range adaptor
        ///
        range_adaptor<reverse_range_adaptor> const reverse;
    }
}

} // namespace boost

#endif

///////////////////////////////////////////////////////////////////////////////
// acv0.hpp
//
//  Copyright 2008 Erwann Rogard. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_ACCUMULATORS_STATISTICS_ACV0_HPP_ER_04_2008
#define BOOST_ACCUMULATORS_STATISTICS_ACV0_HPP_ER_04_2008
#include <cmath>

#include <boost/mpl/size_t.hpp>
#include <boost/mpl/assert.hpp>
#include <boost/mpl/placeholders.hpp>

#include <boost/call_traits.hpp>
#include <boost/assert.hpp>
#include <boost/array.hpp>
#include <boost/range/iterator_range.hpp>
#include <boost/type_traits/add_const.hpp>

#include <boost/accumulators/framework/extractor.hpp>
#include <boost/accumulators/framework/accumulator_base.hpp>
#include <boost/accumulators/framework/extractor.hpp>
#include <boost/accumulators/numeric/functional.hpp>
#include <boost/accumulators/framework/parameters/sample.hpp>
#include <boost/accumulators/framework/depends_on.hpp>
#include <boost/accumulators/statistics_fwd.hpp>

#include <boost/accumulators/statistics/acvf.hpp>

namespace boost { namespace accumulators
{


namespace impl
{
    ////////////////////////////////////////////////////////////////////////////
    // acv0_impl
    template<typename Sample,typename Discriminator>
    class acv0_impl
      : public accumulator_base
    {
    public:
        typedef Sample result_type;
        acv0_impl(dont_care)
        {}

        template<typename Args>
        void operator()(Args const &args)
        {
            val = (*begin(acvf<Discriminator>(args[accumulator])));
        }


        result_type result(dont_care) const
        {
            return val;
        }
    private:
        Sample val;
    };

} // namespace impl
///////////////////////////////////////////////////////////////////////////////
// tag::acv0
//

namespace tag
{
    template <typename Discriminator = default_delay_discriminator>
    struct acv0
      : depends_on<acvf<Discriminator> >
    {
        /// INTERNAL ONLY
      typedef
        accumulators::impl::acv0_impl<
            mpl::_1,Discriminator> impl;

    };
}



///////////////////////////////////////////////////////////////////////////////
// extract::acv0
//

namespace extract
{

//  extractor<tag::acv0<> > const acv0 = {};
  // see acvf about default_delay_discriminator
  template<typename Discriminator,typename AccumulatorSet>
  typename mpl::apply<
    AccumulatorSet,tag::acv0<Discriminator>
    >::type::result_type
  acv0(AccumulatorSet const& acc){
    typedef tag::acv0<Discriminator> the_tag;
    return extract_result<the_tag>(acc);
  }

//  TODO
//  overload (default) see acvf

}

using extract::acv0;



}} // namespace boost::accumulators

#endif

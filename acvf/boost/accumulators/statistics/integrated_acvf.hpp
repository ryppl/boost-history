///////////////////////////////////////////////////////////////////////////////
// integrated_acvf.hpp
//
//  Copyright 2008 Erwann Rogard. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_ACCUMULATORS_STATISTICS_INTEGRATED_ACVF_HPP_ER_04_2008
#define BOOST_ACCUMULATORS_STATISTICS_INTEGRATED_ACVF_HPP_ER_04_2008

//TODO check if any header not needed
#include <cmath>
#include <vector>
#include <algorithm>
#include <numeric>
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
    // integrated_acvf_impl
    template<typename Sample,typename Discriminator>
    class integrated_acvf_impl
      : public accumulator_base
    {
       //TODO or via feature?
        typedef acvf_impl<Sample,Discriminator>                      acvf_type;
        typedef typename acvf_type::result_type                      range_type;
    public:
        typedef Sample result_type;
        integrated_acvf_impl(dont_care):val(0.0){}

        template<typename Args>
        void operator ()(Args const &args)
        {
          range_type range = acvf<Discriminator>(args[accumulator]);
          typedef typename range_type::const_iterator  iter_type;
          Sample acv0_val = (*begin(range));
          val = std::accumulate(begin(range),end(range),0.0);
          val *=2.0;
          val -=acv0_val;
		}

        result_type result(dont_care) const{return val;}

    private:
	   Sample  val;

    };

} // namespace impl

///////////////////////////////////////////////////////////////////////////////
// tag::integrated_acvf
//

namespace tag
{
    template <typename Discriminator = default_delay_discriminator>
    struct integrated_acvf
      : depends_on<acvf<Discriminator> >
    {
        /// INTERNAL ONLY
      typedef accumulators::impl::integrated_acvf_impl<mpl::_1,Discriminator> impl;

    };
}



///////////////////////////////////////////////////////////////////////////////
// extract::integrated_acvf
//

namespace extract
{

//  extractor<tag::integrated_acvf<> > const integrated_acvf = {};

  // see acvf about default_delay_discriminator
  template<typename Discriminator,typename AccumulatorSet>
  typename mpl::apply<
    AccumulatorSet,tag::integrated_acvf<Discriminator>
    >::type::result_type
  integrated_acvf(AccumulatorSet const& acc){
    typedef tag::integrated_acvf<Discriminator> the_tag;
    return extract_result<the_tag>(acc);
  }

//  TODO
//  overload (default) see acvf

}

using extract::integrated_acvf;


}} // namespace boost::accumulators

#endif

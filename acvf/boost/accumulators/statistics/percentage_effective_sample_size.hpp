///////////////////////////////////////////////////////////////////////////////
// percentage_effective_sample_size.hpp
//
//  Copyright 2008 Erwann Rogard. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_ACCUMULATORS_STATISTICS_PERCENTAGE_EFFECTIVE_SAMPLE_SIZE_HPP_ER_04_2008
#define BOOST_ACCUMULATORS_STATISTICS_PERCENTAGE_EFFECTIVE_SAMPLE_SIZE_HPP_ER_04_2008

//TODO check if any header not needed

#include <cmath>

#include <boost/mpl/size_t.hpp>
#include <boost/mpl/assert.hpp>
#include <boost/mpl/placeholders.hpp>

#include <boost/call_traits.hpp>
#include <boost/assert.hpp>
#include <boost/array.hpp>
#include <boost/range/iterator_range.hpp>
#include <boost/type_traits/add_const.hpp>
#include <boost/numeric/conversion/converter.hpp>

#include <boost/accumulators/framework/extractor.hpp>
#include <boost/accumulators/framework/accumulator_base.hpp>
#include <boost/accumulators/framework/extractor.hpp>
#include <boost/accumulators/numeric/functional.hpp>
#include <boost/accumulators/framework/parameters/sample.hpp>
#include <boost/accumulators/framework/depends_on.hpp>
#include <boost/accumulators/statistics_fwd.hpp>
#include <boost/accumulators/statistics/integrated_acvf.hpp>
#include <boost/accumulators/statistics/acv0.hpp>

namespace boost { namespace accumulators
{


namespace impl
{
    ////////////////////////////////////////////////////////////////////////////
    // percentage_effective_sample_size
    template<typename Sample,typename Discriminator>
    class percentage_effective_sample_size_impl
      : public accumulator_base
    {
    public:
        typedef std::size_t result_type;

        percentage_effective_sample_size_impl(dont_care):val(0){}

        template<typename Args>
        void operator()(const Args& args)
        {
            Sample iacvf = integrated_acvf<Discriminator>(args[accumulator]);
            if(iacvf>0.0){

                typedef boost::numeric::converter<result_type,Sample> Sample2res;
                Sample acv0_val = acv0<Discriminator>(args[accumulator]);
                Sample tmp = 100.0*acv0_val/iacvf;
                val = Sample2res::convert(tmp);
            }else{
                val = 0;
            }
        }

        result_type result(dont_care) const
        {
            return val;
        }
    private:
        result_type val;
    };

} // namespace impl
///////////////////////////////////////////////////////////////////////////////
// tag::integrated_acvf
//

namespace tag
{
    template <typename Discriminator = default_delay_discriminator>
    struct percentage_effective_sample_size
      : depends_on<acv0<Discriminator>, integrated_acvf<Discriminator> >
    {
        /// INTERNAL ONLY
      typedef
        accumulators::impl::percentage_effective_sample_size_impl<
            mpl::_1,Discriminator> impl;

    };
}



///////////////////////////////////////////////////////////////////////////////
// extract::percentage_effective_sample_size
//

namespace extract
{

//  extractor<tag::percentage_effective_sample_size<> >
//    const percentage_effective_sample_size = {};

  // see acvf about default_delay_discriminator
  template<typename Discriminator,typename AccumulatorSet>
  typename mpl::apply<
    AccumulatorSet,tag::percentage_effective_sample_size<Discriminator>
    >::type::result_type
  percentage_effective_sample_size(AccumulatorSet const& acc){
    typedef tag::percentage_effective_sample_size<Discriminator> the_tag;
    return extract_result<the_tag>(acc);
  }

//  TODO
//  overload (default) see acvf

}

using extract::percentage_effective_sample_size;
}}
#endif

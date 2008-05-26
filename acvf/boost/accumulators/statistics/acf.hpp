///////////////////////////////////////////////////////////////////////////////
// acf.hpp
//
//  Copyright 2008 Erwann Rogard. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_ACCUMULATORS_STATISTICS_ACF_HPP_ER_04_2008
#define BOOST_ACCUMULATORS_STATISTICS_ACF_HPP_ER_04_2008

#include <cmath>
#include <vector>
#include <algorithm>
#include <functional>
#include <boost/mpl/int.hpp>
#include <boost/mpl/size_t.hpp>
#include <boost/mpl/assert.hpp>
#include <boost/mpl/placeholders.hpp>

#include <boost/assert.hpp>
#include <boost/range/iterator_range.hpp>
#include <boost/type_traits/add_const.hpp>
#include <boost/bind.hpp>

#include <boost/accumulators/framework/extractor.hpp>
#include <boost/accumulators/framework/accumulator_base.hpp>
#include <boost/accumulators/framework/extractor.hpp>
#include <boost/accumulators/numeric/functional.hpp>
#include <boost/accumulators/framework/parameters/sample.hpp>
#include <boost/accumulators/framework/depends_on.hpp>
#include <boost/accumulators/statistics_fwd.hpp>
#include <boost/accumulators/statistics/acvf.hpp>
#include <boost/accumulators/statistics/delay.hpp>


namespace boost { namespace accumulators
{


namespace impl
{
    ////////////////////////////////////////////////////////////////////////////
    // acf_impl
    template<typename Sample,typename Discriminator>
    class acf_impl
      : public accumulator_base
    {
       //TODO via the feature for delay (?)
	   typedef acvf_impl<Sample,Discriminator>      acvf_type;
	   typedef typename acvf_type::result_type      acvfs_result_type;
       typedef std::vector<Sample>                  acfs_type;
    public:
       typedef boost::iterator_range<
            typename acfs_type::const_iterator>     result_type;

        acf_impl(dont_care):acfs(0,(Sample)(0.0)){}

        template<typename Args>
        void operator ()(Args const &args){
            const acvfs_result_type& range
                = acvf<Discriminator>(args[accumulator]);
            std::size_t sz = range.size();
            acfs.resize(sz);
            Sample acv0 = (*begin(range));
            if(acv0>0.0){
                Sample div=(1.0/acv0);
                transform(
                    begin(range),
                    end(range),
                    acfs.begin(),
                    boost::bind(
                        std::multiplies<Sample>(),
                        _1,
                        div
                    )
                );
            }else{
                std::fill_n(acfs.begin(),sz,0.0);
            }
        }
        result_type result(dont_care) const
        {
          return boost::make_iterator_range(acfs.begin(),acfs.end());
        }
    private:
        mutable acfs_type acfs;
    };

} // namespace impl

///////////////////////////////////////////////////////////////////////////////
// tag::acf
//

namespace tag
{
    template <typename Discriminator = default_delay_discriminator>
    struct acf
      : depends_on<acvf<Discriminator> >
    {
        /// INTERNAL ONLY
      typedef accumulators::impl::acf_impl<mpl::_1,Discriminator> impl;

    };
}



///////////////////////////////////////////////////////////////////////////////
// extract::acf
//

namespace extract
{

//  extractor<tag::acf<> > const acf = {};

  // see acvf about default_delay_discriminator
  template<typename Discriminator,typename AccumulatorSet>
  typename
    mpl::apply<AccumulatorSet,tag::acf<Discriminator> >::type::result_type
  acf(AccumulatorSet const& acc){
    typedef tag::acf<Discriminator> the_tag;
    return extract_result<the_tag>(acc);
  }//typical call://acf<default_delay_discriminator>(acc)

//  TODO
//  overload (default) see acvf


}

using extract::acf;


}} // namespace boost::accumulators

#endif

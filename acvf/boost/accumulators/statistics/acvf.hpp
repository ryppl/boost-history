///////////////////////////////////////////////////////////////////////////////
// acvf.hpp
//
//  Copyright 2008 Erwann Rogard. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_ACCUMULATORS_STATISTICS_ACVF_HPP_ER_04_2008
#define BOOST_ACCUMULATORS_STATISTICS_ACVF_HPP_ER_04_2008

//TODO check if any header not needed

#include <cmath>
#include <vector>
#include <algorithm>
#include <boost/mpl/int.hpp>
#include <boost/mpl/size_t.hpp>
#include <boost/mpl/assert.hpp>
#include <boost/mpl/placeholders.hpp>

#include <boost/call_traits.hpp>
//#include <boost/assert.hpp>
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

#include <boost/accumulators/statistics/count.hpp>
#include <boost/accumulators/statistics/mean.hpp>
#include <boost/accumulators/statistics/delay.hpp>

namespace boost { namespace accumulators
{


namespace impl
{
    ////////////////////////////////////////////////////////////////////////////
    // acvf_impl (Autocovariance function)
    template<typename Sample,typename Discriminator>
    class acvf_impl
      : public accumulator_base
    {
	   typedef std::vector<Sample>	               acvs_type;
       typedef delay_impl<Sample,Discriminator>    delay_type;
	   typedef typename delay_type::result_type    input_type;
    public:
       typedef boost::iterator_range<
            typename acvs_type::const_iterator
            > result_type;


        template<typename Args>
        acvf_impl(Args const &args)
        :acvs(
            args[tag::delay<Discriminator>::cache_size|delay(args).size()],
            (Sample)(0.0)
        )
        {
        }

        acvf_impl(const acvf_impl& that)
        :acvs(that.acvs){}

        acvf_impl& operator=(const acvf_impl& that){
            if(&that!=this){acvs = that.acvs;} return *this;
        }


        template<typename Args>
        void operator ()(Args const &args)
        {
          input_type in = delay(args); //0,1,2,...,K
          typedef typename input_type::const_iterator  in_iter_type;
          typedef typename acvs_type::iterator         out_iter_type;
          std::size_t in_sz = in.size();

          BOOST_ASSERT((in_sz<acvs.size())||(in_sz==acvs.size()));
          in_iter_type i = begin(in);
          in_iter_type e = end(in);

          Sample x0 = (*i);
          std::size_t n = count(args);
          std::size_t k = 0;
          //TODO replace by
          //for_each(make_zip_iterator(
          //  make_tuple(begin(in),acvs.begin())),...,local_class(...))
		  while((k<in_sz) && (n>1+k)){
		      BOOST_ASSERT(i<e);
              Sample xk = (*i);
              Sample div = (Sample) ((n-1)-k);
              Sample sum_prod = acvs[k] * div;
              Sample mean_val = mean(args);
              sum_prod += (xk - mean_val) * (x0 - mean_val);
              div = (Sample)(n-k);
              acvs[k] = sum_prod / div;
              ++i;
              ++k;
          }
		}

        result_type result(dont_care) const
        {
          return boost::make_iterator_range(acvs.begin(),acvs.end());
        }

    private:
	   acvs_type   acvs;

    };

} // namespace impl

///////////////////////////////////////////////////////////////////////////////
// tag::acvf
//

namespace tag
{
    template <typename Discriminator = default_delay_discriminator>
    struct acvf
      : depends_on<count,mean,delay<Discriminator> >
    {
        /// INTERNAL ONLY
      typedef accumulators::impl::acvf_impl<mpl::_1,Discriminator> impl;

    };
}



///////////////////////////////////////////////////////////////////////////////
// extract::acvf
//

namespace extract
{
//  extractor<tag::acvf<> > const acvf = {};
//  //a non-default discriminator requires
//  //struct my_other_delay {};
//  //extractor<tag::delay<my_other_delay> > other_delay={};

  template<typename Discriminator,typename AccumulatorSet>
  typename
    mpl::apply<AccumulatorSet,tag::acvf<Discriminator> >::type::result_type
  acvf(AccumulatorSet const& acc){
    typedef tag::acvf<Discriminator> the_tag;
    return extract_result<the_tag>(acc);
  }//typical call://acvf<default_delay_discriminator>(acc)

//  TODO
//  //overload
//  template<typename AccumulatorSet>
//  typename mpl::apply<AccumulatorSet,tag::acvf<> >::type::result_type
//  acvf(AccumulatorSet const& acc){
//    return acvf<default_delay_discriminator,AccumulatorSet>(acc);
//  }
// /../boost_1_35_0/boost/mpl/aux_/preprocessed/gcc/apply_wrap.hpp|39|error: no
// class template named ‘apply’
// in ‘struct boost::accumulators::default_delay_discriminator’|

}

using extract::acvf;


}} // namespace boost::accumulators

#endif

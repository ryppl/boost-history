///////////////////////////////////////////////////////////////////////////////
// standard_error_iid.hpp
//
//  Copyright 2008 Erwann Rogard. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_ACCUMULATORS_STATISTICS_STANDARD_ERROR_IID_HPP_ER_04_2008
#define BOOST_ACCUMULATORS_STATISTICS_STANDARD_ERROR_IID_HPP_ER_04_2008

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

#include <boost/accumulators/framework/extractor.hpp>
#include <boost/accumulators/framework/accumulator_base.hpp>
#include <boost/accumulators/framework/extractor.hpp>
#include <boost/accumulators/numeric/functional.hpp>
#include <boost/accumulators/framework/parameters/sample.hpp>
#include <boost/accumulators/framework/depends_on.hpp>
#include <boost/accumulators/statistics_fwd.hpp>

#include <boost/accumulators/statistics/acv0.hpp>
#include <boost/accumulators/statistics/count.hpp>

namespace boost { namespace accumulators
{


namespace impl
{
    ////////////////////////////////////////////////////////////////////////////
    // standard_error_iid
    template<typename Sample,typename Discriminator>
    class standard_error_iid_impl
      : public accumulator_base
    {
    public:
        typedef Sample result_type;

        standard_error_iid_impl(dont_care)
        {}

        template<typename Args>
        void operator()(Args const &args)
        {
            Sample acv0_val = acv0<Discriminator>(args[accumulator]);
            Sample n = (Sample)(count(args));
            val = 0.0;
            if((acv0_val>0.0) && (n>0.0)){val = sqrt(acv0_val/n);}
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
// tag::standard_error_iid
//

namespace tag
{
    template <typename Discriminator = default_delay_discriminator>
    struct standard_error_iid
      : depends_on<count,acv0<Discriminator> >
    {
        /// INTERNAL ONLY
      typedef
        accumulators::impl::standard_error_iid_impl<
            mpl::_1,Discriminator> impl;

    };
}



///////////////////////////////////////////////////////////////////////////////
// extract::standard_error_iid
//

namespace extract
{

//  extractor<tag::standard_error_iid<> >
//    const standard_error_iid = {};

  // see acvf about default_delay_discriminator
  template<typename Discriminator,typename AccumulatorSet>
  typename mpl::apply<
    AccumulatorSet,tag::standard_error_iid<Discriminator>
    >::type::result_type
  standard_error_iid(AccumulatorSet const& acc){
    typedef tag::standard_error_iid<Discriminator> the_tag;
    return extract_result<the_tag>(acc);
  }

//  TODO
//  overload (default) see acvf

}

using extract::standard_error_iid;

}}

#endif

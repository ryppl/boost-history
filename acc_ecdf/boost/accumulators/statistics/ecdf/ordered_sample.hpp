///////////////////////////////////////////////////////////////////////////////
//  acc_ecdf                                                                 //
//                                                                           //
//  Copyright (C) 2005 Eric Niebler                                          //
//  Copyright (C) 2011 Erwann Rogard                                         //
//  Software License, Version 1.0. (See accompanying file                    //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)         //
///////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ACCUMULATORS_STATISTICS_ECDF_ORDERED_SAMPLE_HPP_ER_2011
#define BOOST_ACCUMULATORS_STATISTICS_ECDF_ORDERED_SAMPLE_HPP_ER_2011
#include <map>
#include <functional>
#include <boost/mpl/placeholders.hpp>
#include <boost/mpl/apply.hpp>
#include <boost/accumulators/framework/extractor.hpp>
#include <boost/accumulators/framework/accumulator_base.hpp>
#include <boost/accumulators/framework/parameters/sample.hpp>
#include <boost/accumulators/framework/parameters/accumulator.hpp>
#include <boost/accumulators/framework/depends_on.hpp>
#include <boost/accumulators/statistics/ecdf/aux_/ignore.hpp>
#include <boost/accumulators/statistics_fwd.hpp>

namespace boost{ 
//[syntax_ecdf_ordered_sample
namespace accumulators{
namespace ecdf{
//<-
namespace impl{

    template<typename T>
    class ordered_sample 
        : public accumulator_base
    {
        typedef std::less<T> comp_;
        typedef std::size_t size_;
        typedef dont_care dont_care_;
        typedef std::map<T,size_,comp_> map_;

        public:
        
        // See accumulator_set for convention naming sample_type
        typedef T         sample_type; 
        typedef size_     size_type;     

        // non-const because map::operator[](key) returns a non-const
        typedef map_& result_type;

        ordered_sample(dont_care_){}

        template<typename Args>
        void operator()(const Args& args){
            ++(
                this->freq[
                    static_cast<T>(
                        args[sample]
                    )
                ]
            );
        }
        
        // Returns the entire distribution, represented by a map
        result_type result(dont_care_)const
        {
            return (this->freq); 
        }

        private:
        mutable map_ freq;
    };
    
}// impl
//->
namespace tag
{
   
    struct ordered_sample
        : depends_on<>
    {/*<-*/
      typedef ecdf::impl::ordered_sample<boost::mpl::_1> impl;
    /*->*/};
    
}// tag
namespace result_of{

    template<typename AccSet>
    struct ordered_sample/*<-*/
        : detail::extractor_result<
            AccSet,
            ecdf::tag::ordered_sample
        >
    {}/*->*/;

}// result_of
namespace extract
{

    template<typename AccSet>
    typename ecdf::result_of::template ordered_sample<AccSet>::type
    ordered_sample(AccSet const& acc)/*<-*/
    {
        typedef ecdf::tag::ordered_sample the_tag;
        return extract_result<the_tag>(acc);
    }BOOST_ACCUMULATORS_ECDF_IGNORE(/*->*/;/*<-*/)/*->*/

}// extract

using extract::ordered_sample;

}// ecdf
}// accumulators
//]
}// boost

#endif // BOOST_ACCUMULATORS_STATISTICS_ECDF_ORDERED_SAMPLE_HPP_ER_2011

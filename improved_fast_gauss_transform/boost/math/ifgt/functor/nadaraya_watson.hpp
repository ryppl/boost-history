//////////////////////////////////////////////////////////////////////////////
//  nadaraya_watson.hpp
//  (C) Copyright 2009 Erwann Rogard
//  Use, modification and distribution are subject to the
//  Boost Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#ifndef BOOST_MATH_IFGT_FUNCTOR_NADARAYA_WATSON_HPP_ER_2009
#define BOOST_MATH_IFGT_FUNCTOR_NADARAYA_WATSON_HPP_ER_2009
#include <vector>
#include <algorithm>
#include <boost/mpl/size_t.hpp>
#include <boost/mpl/prior.hpp>
#include <boost/mpl/void.hpp>
#include <boost/mpl/apply.hpp>
#include <boost/mpl/identity.hpp>
#include <boost/assert.hpp>
#include <boost/bind.hpp>
#include <boost/iterator/range_result.hpp>
#include <boost/iterator/range_result_size.hpp>
#include <boost/iterator/range_right.hpp>
#include <boost/iterator/range_same.hpp>
#include <boost/math/ifgt/functor/gauss_transform.hpp>
#include <boost/math/ifgt/crtp/evaluator.hpp>
#include <boost/utility/result_of.hpp>
namespace boost{
namespace math{
namespace ifgt{
namespace impl{

   // P models SubsetRangePolicy (for now not used)
   // TODO Make use of P
   template<typename D,typename P = mpl::void_>
   class nadaraya_watson{
       typedef typename D::value_type                   value_type;
       static const std::size_t weight_size = D::weight_size;

       public:
       typedef                                          void result_type;
       static const std::size_t stride0 = D::source_size;
       static const std::size_t stride1 = weight_size-1;

       nadaraya_watson(const D& e)
       :eval_(e){}

//       nadaraya_watson(const D& e,const P& pol)
//       :eval_(e){}

       nadaraya_watson(const nadaraya_watson& that) : eval_(that.eval_){}

       // Requires weight[0]==1
       // Initializes range_out to zero and writes NW to it
        template<typename R0,typename R1>
        void operator()(const R0& target,R1& range_out)const{
            typedef std::vector<value_type>             range_type;
            typedef range_right<weight_size-1>          policy_type;
            typedef typename boost::result_of<
                policy_type(const range_type&)>::type
                                                        rest_type;
            static range_type range(weight_size);
            static policy_type policy;
            BOOST_ASSERT(size(range_out)-(weight_size-1)==0);
            std::fill(
                begin(range),
                end(range),
                (value_type)(0)
            );
            typedef typename
                mpl::apply1<meta::gauss_transform<range_same>,D>::type gt_t;
            gt_t gt(eval_);
            gt(target,range);
            value_type denom = range[0];
            rest_type rest = policy(range);
            BOOST_ASSERT(size(rest)-(weight_size-1)==0);
            std::transform(
                begin(rest),
                end(rest),
                begin(range_out),
                bind(std::divides<value_type>(), _1, denom)
            );
        }


       private:
       nadaraya_watson& operator=(const nadaraya_watson& that);
       const D& eval_;
   };
}
namespace meta{
    template<typename P = mpl::void_>
    struct nadaraya_watson{
        template<typename D>
        struct apply : mpl::identity<impl::nadaraya_watson<D,P> >{};
    };
}

template<typename D>
typename mpl::apply1<meta::nadaraya_watson<>,D>::type
make_nadaraya_watson(const D& e){
    typedef typename
        mpl::apply1<meta::nadaraya_watson<>,D>::type result_type;
    return result_type(e);
}

}//ifgt
}//math
}//boost

#endif // NADARAYA_WATSON_ESTIMATE_HPP_INCLUDED

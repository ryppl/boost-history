//////////////////////////////////////////////////////////////////////////////
//  gauss_transform.hpp
//  (C) Copyright 2009 Erwann Rogard
//  Use, modification and distribution are subject to the
//  Boost Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#ifndef BOOST_MATH_IFGT_FUNCTOR_GAUSS_TRANSFORM_HPP_ER_2009
#define BOOST_MATH_IFGT_FUNCTOR_GAUSS_TRANSFORM_HPP_ER_2009
#include <vector>
#include <boost/assert.hpp>
#include <boost/iterator/range_same.hpp>
#include <boost/mpl/apply.hpp>
#include <boost/mpl/size_t.hpp>
#include <boost/iterator/range_result_size.hpp>
#include <boost/math/ifgt/crtp/evaluator.hpp>
namespace boost{
namespace math{
namespace ifgt{

namespace impl{
   // P : SubsetRangePolicy
   template<typename D,typename P>
   class gauss_transform{
       public:
       typedef void                                         result_type;
       static const std::size_t weight_size = D::weight_size;
       static const std::size_t stride0 = D::source_size;
       static const std::size_t stride1
        = range_result_size<P,weight_size>::type::value;

       gauss_transform(const D& e)
       :eval_(e),policy_(P()){}

//       gauss_transform(const D& e,const P& pol)
//       :eval_(e),policy_(pol){}

       gauss_transform(const gauss_transform& that)
       : eval_(that.eval_),policy_(that.policy_){}

       // Warning : adds to range_out
       template<typename R0,typename R1>
       void operator()(const R0& target,R1& range_out)const{
            eval_.call_impl(target,range_out,policy_);
       }

       private:
       gauss_transform& operator=(const gauss_transform& that);
       const D& eval_;
       P policy_;
   };
}
namespace meta{
    template<typename P = range_same>
    struct gauss_transform{
        template<typename D>
        struct apply{
            typedef impl::gauss_transform<D,P> type;
        };
    };
}

template<typename D>
typename mpl::apply1<meta::gauss_transform<>,D>::type
make_gauss_transform(const D& e){
    typedef typename mpl::apply1<meta::gauss_transform<>,D>::type
        result_type;
    return result_type(e);
}

}//ifgt
}//math
}//boost

#endif // GAUSS_TRANSFORM_HPP_INCLUDED

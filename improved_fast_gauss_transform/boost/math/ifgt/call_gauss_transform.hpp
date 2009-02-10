//////////////////////////////////////////////////////////////////////////////
// ifgt/call_gauss_transform.hpp
//  (C) Copyright 2009 Erwann Rogard
//  Use, modification and distribution are subject to the
//  Boost Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#ifndef BOOST_MATH_IFGT_CALL_GAUSS_TRANSFORM_HPP_ER_2009
#define BOOST_MATH_IFGT_CALL_GAUSS_TRANSFORM_HPP_ER_2009
#include <stdexcept>
#include <boost/math/ifgt/evaluator_base.hpp>
namespace boost{namespace math{namespace ifgt{

   template<typename Evaluator>
   class gauss_transform{
       public:
       typedef Evaluator                                    evaluator_type;
       struct stride_traits{
        typedef mpl::size_t<evaluator_type::source_size>     stride0_type;
        typedef mpl::size_t<evaluator_type::weight_size>     stride1_type;
       };
       typedef typename evaluator_type::value_type          value_type;
       template<typename Common>
       gauss_transform(const evaluator_base<Common,Evaluator>& e)
       :eval_(static_cast<const Evaluator&>(e)){}
       gauss_transform(const gauss_transform& that) : eval_(that.eval){}

       template<typename R0,typename R1>
       void operator()(const R0& targets,R1& ranges_out)const{
            eval_.gauss_transform(targets,ranges_out);
       };

       private:
       gauss_transform& operator=(const gauss_transform& that);
       const Evaluator& eval_;
   };

}}}
#endif

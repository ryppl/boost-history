/////////////////////////////////////////////////////////////////////////////
// adapter.hpp
//
//  Copyright 2009 Erwann Rogard. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_CONDITIONALLY_SPECIFIED_DISTRIBUTION_FUNCTION_DETAIL_ADAPTER_HPP_ER_2009
#define BOOST_CONDITIONALLY_SPECIFIED_DISTRIBUTION_FUNCTION_DETAIL_ADAPTER_HPP_ER_2009
#include <utility>//pair
#include <boost/conditionally_specified_distribution/result_of/include.hpp>
#include <boost/conditionally_specified_distribution/support/include.hpp>
#include <boost/conditionally_specified_distribution/function/include.hpp>
namespace boost{namespace conditionally_specified_distribution{namespace function{

namespace detail{

    //This adapter is needed for adaptive rejection sampling

    template<typename Par,typename Args>
    class adapter{
        typedef typename function_support<Par>::type    support_t;
        public:
        typedef typename function_argument<Par>::type   argument_type;
        typedef typename function_value<Par>::type      result_type;
        //TODO see adpative_rejection_sampler::value_type
        typedef result_type                             value_type;

        adapter(const Par& par,const Args& args)
        : par_(par),args_(args){}

        adapter(
            const adapter& that
        ): par_(that.par_),args_(that.args_){}

        result_type
        unnormalized_log_pdf(argument_type x)const{
            return function::unnormalized_log_pdf(
                this->par_,this->args_,x
            );
        }
        result_type
        derivative_unnormalized_log_pdf(argument_type x)const{
            return function::derivative_unnormalized_log_pdf(
                this->par_,this->args_,x);
        }

        result_type
        log_pdf(argument_type x)const{
            return log_pdf(this->par_,this->args_,x);
        }
        result_type
        score(argument_type x)const{
            return score(this->par_,this->args_,x);
        }

        argument_type
        min()const{
            return (this->support()).first;
        }

        argument_type
        max()const{
            return (this->support()).second;
        }

        typename function_support<Par>::type
        support()const{
            return function::support(
                (this->par_),
                (this->args_)
            );
        }

        private:
        adapter();
        adapter& operator()(const adapter&);
        const Par& par_;
        const Args& args_;

    };

template<typename Par,typename Args>
adapter<Par,Args>
make_adapter(const Par& p,const Args& a){
    return adapter<Par,Args>(p,a);
}

}//detail
}//function
}//distribution
}//boost
#endif // BOOST_DISTRIBUTION_FUNCTION_BIND_ARGUMENT_PACK_HPP_ER_2009

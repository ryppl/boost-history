///////////////////////////////////////////////////////////////////////////////
// count_matching.hpp                                                        //
//                                                                           //
//  Copyright 2010 Erwann Rogard. Distributed under the Boost                //
//  Software License, Version 1.0. (See accompanying file                    //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)         //
///////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_STATISTICS_DETAIL_NON_PARAMETRIC_CONTINGENCY_TABLE_CELLS_COUNT_MATCHING_HPP_ER_2010
#define BOOST_STATISTICS_DETAIL_NON_PARAMETRIC_CONTINGENCY_TABLE_CELLS_COUNT_MATCHING_HPP_ER_2010
#include <boost/mpl/assert.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/statistics/detail/non_parametric/contingency_table/cells/cells.hpp>
#include <boost/statistics/detail/non_parametric/contingency_table/cells/traits.hpp>
#include <boost/mpl/detail/wrapper.hpp>

namespace boost{
namespace statistics{
namespace detail{
namespace contingency_table{
namespace result_of{

    template<typename Keys,typename AccSet>
    struct count_matching 
        : contingency_table::cells_traits::mapped_of<Keys, AccSet>{};

}// result_of


  	template<typename Keys,typename AccSet,typename T>
    typename contingency_table::result_of::count_matching<Keys,AccSet>::type
    count_matching(AccSet const& acc,const T& sample){
        namespace ns = contingency_table;
        return ns::extract::cells<Keys>( acc )[ sample ];
    }

    // A: accumulator set
    // T: sample
    // F: unary functor
    template<typename A,typename T,typename F>
    struct fun_count_matcher{
            
        typedef void result_type;
    
        fun_count_matcher(const A& a,const T& s, const F& f)
            :acc( a ), sample( s ), fun( f ){}
        
        template<typename Keys>
        void operator()(const boost::mpl::detail::wrapper<Keys>& u)const{
            namespace ns = contingency_table;
            this->fun(
                ns::count_matching<Keys>(
                    this->acc,
                    this->sample
                )
            );
        }
        
        private:
        const A& acc;
        const T& sample;
        F fun;
    };

    template<typename A,typename T,typename F>
    fun_count_matcher<A,T,F> 
    make_fun_count_matcher(const A& a,const T& s, const F& f){
        namespace ns = contingency_table;
        typedef ns::fun_count_matcher<A,T,F> result_;
        return result_(a, s, f);
    }

}// contingency_table
}// detail
}// statistics
}// boost

#endif

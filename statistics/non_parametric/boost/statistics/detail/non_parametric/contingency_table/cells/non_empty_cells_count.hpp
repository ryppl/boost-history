///////////////////////////////////////////////////////////////////////////////
// non_empty_cells_count.hpp                                                 //
//                                                                           //
//  Copyright 2010 Erwann Rogard. Distributed under the Boost                //
//  Software License, Version 1.0. (See accompanying file                    //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)         //
///////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_STATISTICS_DETAIL_NON_PARAMETRIC_CONTINGENCY_TABLE_CELLS_NON_EMPTY_CELLS_COUNT_HPP_ER_2010
#define BOOST_STATISTICS_DETAIL_NON_PARAMETRIC_CONTINGENCY_TABLE_CELLS_NON_EMPTY_CELLS_COUNT_HPP_ER_2010
#include <boost/range.hpp>
#include <boost/statistics/detail/non_parametric/contingency_table/cells/cells.hpp>
#include <boost/statistics/detail/non_parametric/contingency_table/cells/traits.hpp>

namespace boost{
namespace statistics{
namespace detail{
namespace contingency_table{
namespace result_of{

    template<typename Keys,typename AccSet>
    struct non_empty_cells_count 
        : contingency_table::cells_traits::size_of<Keys,AccSet>{};

}// result_of

  	template<typename Keys,typename AccSet>
    typename contingency_table::result_of::non_empty_cells_count<
        Keys,AccSet>::type 
    non_empty_cells_count(AccSet const& acc){
        namespace ns = contingency_table;
        return ns::extract::cells<Keys>( acc ).size() ;    
    }

}// contingency_table
}// detail
}// statistics
}// boost

#endif

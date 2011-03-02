//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#include <deque>
#include <list>
#include <map>
#include <queue>
#include <set>
#include <stack>
#include <vector>
#include <string>
#include <boost/array.hpp>
#include <boost/type_traits/is_same.hpp>

#include <boost/ptr_container/ptr_array.hpp>
#include <boost/ptr_container/ptr_deque.hpp>
#include <boost/ptr_container/ptr_list.hpp>
#include <boost/ptr_container/ptr_vector.hpp>

#include <boost/assign/v2/put/deduce.hpp>
#include <libs/assign/v2/test/put/deduce.h>

namespace test_assign_v2{
namespace xxx_put{
namespace xxx_deduce{

    void test()
    {
        namespace as2 = boost::assign::v2;
        {
            typedef as2::modifier_tag::push_back answer_;
            as2::put_aux::check_deduce<std::deque<int>, answer_>();
            as2::put_aux::check_deduce<std::list<int>, answer_>();
            as2::put_aux::check_deduce<std::vector<int>, answer_>();

            as2::put_aux::check_deduce<boost::ptr_deque<int>, answer_>();
            as2::put_aux::check_deduce<boost::ptr_list<int>, answer_>();
            as2::put_aux::check_deduce<boost::ptr_vector<int>, answer_>();
        }
        {
            typedef as2::modifier_tag::iterate answer_;
            as2::put_aux::check_deduce<boost::array<int, 1>, answer_>();
            as2::put_aux::check_deduce<boost::ptr_array<int, 1>, answer_>();
        }
        {
            typedef as2::modifier_tag::push answer_;
            as2::put_aux::check_deduce<std::queue<int>, answer_>();
            as2::put_aux::check_deduce<std::stack<int>, answer_>();
        }
        {
            typedef as2::modifier_tag::insert answer_;
            as2::put_aux::check_deduce<std::set<int>, answer_>();
            typedef std::string str_;
            as2::put_aux::check_deduce<std::map<str_, int>, answer_>();
        }
        {
            //[put_deduce_map
            typedef std::map<std::string, int> C;
            typedef as2::put_aux::deduce_modifier_tag<C>::type found_;
            typedef as2::modifier_tag::insert answer_;
            BOOST_MPL_ASSERT(( boost::is_same<found_, answer_> ));
            //]
        }
    }


}// xxx_deduce
}// xxx_put
}// test_assign_v2

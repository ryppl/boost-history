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
            typedef as2::modifier_tag::push_back wanted_;
            as2::put_aux::check_deduce<std::deque<int>, wanted_>();
            as2::put_aux::check_deduce<std::list<int>, wanted_>();
            as2::put_aux::check_deduce<std::vector<int>, wanted_>();

            as2::put_aux::check_deduce<boost::ptr_deque<int>, wanted_>();
            as2::put_aux::check_deduce<boost::ptr_list<int>, wanted_>();
            as2::put_aux::check_deduce<boost::ptr_vector<int>, wanted_>();
        }
        {
            typedef as2::modifier_tag::iterate wanted_;
            as2::put_aux::check_deduce<boost::array<int, 1>, wanted_>();
            as2::put_aux::check_deduce<boost::ptr_array<int, 1>, wanted_>();
        }
        {
            typedef as2::modifier_tag::push wanted_;
            as2::put_aux::check_deduce<std::queue<int>, wanted_>();
            as2::put_aux::check_deduce<std::stack<int>, wanted_>();
        }
        {
            typedef as2::modifier_tag::insert wanted_;
            as2::put_aux::check_deduce<std::set<int>, wanted_>();
            typedef std::string str_;
            as2::put_aux::check_deduce<std::map<str_, int>, wanted_>();
        }
        {
            //[deduce_map
            typedef std::map<std::string, int> cont_;
            typedef as2::put_traits< cont_ > traits_;
            BOOST_MPL_ASSERT((boost::is_same<
                traits_::modifier_tag, 
                as2::modifier_tag::insert
            >));
            //]
        }
    }


}// xxx_deduce
}// xxx_put
}// test_assign_v2

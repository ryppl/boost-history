//  Copyright 2010 Brian Bartman.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at 
//  http://www.boost.org/LICENSE_1_0.txt)


#include <boost/integer/bitfield_tuple.hpp>
#include <boost/assert.hpp>
#include <boost/mpl/assert.hpp>
#include <boost/integer/details/fusion_ext_includes.hpp>

using namespace boost;

struct red;
struct green;
struct pink;
struct blue;
struct salmon;

typedef bitfield_tuple<
    member<char,red,4>,
    member<unsigned char, green,5>,
    storage<std::size_t>,
    member<int, salmon, 16>
>                                       test_tuple;


int main() {
    // iterator value_of
    //      this is strange to test, because of the construction of the
    // members vector.
    {
        typedef bitfield_tuple_iterator<test_tuple,0>   iter;
        typedef fusion::result_of::value_of<iter>::type value_of_result;
        BOOST_MPL_ASSERT(( is_same< value_of_result::return_type, char> ));
    }

    // Iterator deref
    {
        typedef bitfield_tuple_iterator<test_tuple,0>   Iter;
        test_tuple temp;
        temp.get<red>() = 3;
        Iter it(temp);
        BOOST_ASSERT(( fusion::deref(it) == 3 ));
    }

    // iterator next
    {
        typedef bitfield_tuple_iterator<test_tuple,0> Iter;
        test_tuple temp;
        temp.get<green>() = 3;
        Iter it(temp);
        BOOST_ASSERT(( fusion::deref(fusion::next(it)) == 3 ));
    }

    // prior
    {
        typedef bitfield_tuple_iterator<test_tuple,0> Iter;
        test_tuple temp;
        temp.get<red>() = 2;
        temp.get<green>() = 3;
        Iter it(temp);
        BOOST_ASSERT(( fusion::deref(fusion::prior(fusion::next(it))) == 2 ));
    }
    
    // distance tesing
    {
        typedef bitfield_tuple_iterator<test_tuple,0> Iter1;
        typedef bitfield_tuple_iterator<test_tuple,1> Iter2;
        test_tuple temp;
        temp.get<red>() = 2;
        temp.get<green>() = 3;
        Iter1 it1(temp);
        Iter2 it2(temp);
        BOOST_ASSERT(( fusion::distance(it2,it1) == 1 ));   
    }
    
    // key_of testing
    {
        typedef bitfield_tuple_iterator<test_tuple,1> Iter;
        test_tuple temp;
        BOOST_MPL_ASSERT((
            is_same<
                fusion::result_of::key_of<
                    Iter
                >::type,
                green
            >
        ));
    }

    // value_of_data testing
    {

        typedef bitfield_tuple_iterator<test_tuple,1> Iter;
        typedef fusion::result_of::value_of_data<
            Iter
        >::type                             value_of_data_t;

        test_tuple temp;
        BOOST_MPL_ASSERT((is_same<value_of_data_t::return_type,unsigned char>));
    }
    
    // deref_data testing
    {
        typedef bitfield_tuple_iterator<test_tuple,1>   Iter;
        typedef fusion::result_of::deref_data<
            Iter
        >::type                             deref_data_t;

        // test_tuple temp;
        // BOOST_MPL_ASSERT((is_same<deref_data_t::return_type,unsigned char>));
    }

}



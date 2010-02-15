//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Justin E. Gottchlich 2009.
// (C) Copyright Vicente J. Botet Escriba 2009.
// Distributed under the Boost
// Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or
// copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/stm for documentation.
//
//////////////////////////////////////////////////////////////////////////////


/*
Should we enable embedded TO?

* First, with coarse grained TO, we don't have any mean to force the user to don't embed TO
* Second, it is natural to embed objects, so why not TO.
* Third, with current implementation, embedded TO can fail

* Should we provide only fine grained TO? This seems a rather big restriction, as there are objets for which fine grained are not adapted, e.g. string.

* Is there an implementation that can make coarse grained TO be embedded?
There is an implementation that coudl make works this.

We will need to map intervals of shared address instead of address.

When an embedding TO is writen, all the current embedded TO must be replaced.
When a embedded TO is query, we need to see if there is already an embedding TO in the writer list.

In addition we need to take care of smart pointers as the cached specific address can change.

And of course we will need to change the conflic algorithm.

interval_map : address -> {address, {smart_pointers, offset}*}

write : address, size
    pair<interval_map::iterator> r = get [address, address+size);
    if (p.first!=p.second) // found

        if p.first.first==[address, address+size) //
            return p.first.second.first
        if p.first.first includes [address, address+size) //
            return p.first.second.first + offset
        create new cache
        for each interval
            update the cache with already written members
            update smart pointers with new offset
            concatenate smart pointers list

        replace all the intervals by the new interval
    else // not found
        insert
        return


This introduce an intrusive pointer class.
All this sems quite complex and will decreased the performance of
reads/writes and conflict detection.

*/

#include <boost/stm.hpp>

using namespace std;
using namespace boost;
using namespace boost::stm;


struct Inner : transaction_object<Inner>
{
    Inner(): i(0) {}
    int i;
};

struct Outer : transaction_object<Outer>
{
    Outer(): e(), j(0) {}
    Inner e;
    int j;
};



bool check_in_nested_fails() {
    Outer obj;
    BOOST_STM_OUTER_TRANSACTION(_) {
        _.write(obj.e).i=1;
        BOOST_STM_INNER_TRANSACTION(_) {
            std::cout <<__LINE__<<" "<< "obj.e.i 1!=" << _.read(obj).e.i << std::endl;
            if (_.read(obj).e.i>0) {
                std::cout <<__LINE__<<" "<< "obj.e.i 1!=" << _.read(obj).e.i << std::endl;
                _.write(obj).j=2;
            }
        }
    } BOOST_STM_RETRY
    BOOST_STM_OUTER_TRANSACTION(_) {
        std::cout <<__LINE__<<" "<< "obj.e.i 1=" << _.read(obj.e).i << std::endl;
        std::cout <<__LINE__<<" "<< "obj.j 2!=" << _.read(obj).j << std::endl;
        BOOST_STM_TX_RETURN(_, _.read(obj.e).i==1 && _.read(obj).j==2) ;
    } BOOST_STM_RETRY
    return false;
}

bool check_after_nested_fails() {
    Outer obj;
    BOOST_STM_OUTER_TRANSACTION(_) {
        _.write(obj).j=0;
        BOOST_STM_INNER_TRANSACTION(_) {
            _.write(obj.e).i=1;
        }
        std::cout <<__LINE__<<" "<< "obj.e.i 1!=" << _.read(obj).e.i << std::endl;
        if (_.read(obj).e.i>0)
            _.write(obj).j=2;
    } BOOST_STM_RETRY
    BOOST_STM_OUTER_TRANSACTION(_) {
        std::cout <<__LINE__<<" "<< "obj.e.i 1=" << _.read(obj.e).i << std::endl;
        std::cout <<__LINE__<<" "<< "obj.j 2!=" << _.read(obj).j << std::endl;
        BOOST_STM_TX_RETURN(_, _.read(obj.e).i==1 && _.read(obj).j==2) ;
    } BOOST_STM_RETRY
    return false;
}

bool mod_outer_same_in_nested_fails() {
    Outer obj;
    BOOST_STM_OUTER_TRANSACTION(_) {
        _.write(obj.e).i=1;
        BOOST_STM_INNER_TRANSACTION(_) {
            _.write(obj).e.i=2;
        }
        std::cout << "obj.e.i= 2!=" << _.read(obj.e).i << std::endl;
    } BOOST_STM_RETRY
    BOOST_STM_OUTER_TRANSACTION(_) {
        std::cout << "obj.e.i= 2!=" << _.read(obj.e).i << std::endl;
        std::cout << "obj.j= 0=" << _.read(obj).j << std::endl;
        BOOST_STM_TX_RETURN(_, _.read(obj.e).i==2 && _.read(obj).j==0) ;
    } BOOST_STM_RETRY
    return false;
}

bool mod_inner_same_in_nested_and_check_after_fails() {
    Outer obj;
    BOOST_STM_OUTER_TRANSACTION(_) {
        _.write(obj).e.i=1;
        BOOST_STM_INNER_TRANSACTION(_) {
            _.write(obj.e).i=2;
        }
        std::cout << "obj.e.i= 2!=" << _.read(obj).e.i << std::endl;
        if (_.read(obj).e.i>1)
            _.write(obj).j=1;
        //BOOST_STM_TX_RETURN(_, _.read(obj).e.i==2) ;
    } BOOST_STM_RETRY
    BOOST_STM_OUTER_TRANSACTION(_) {
        std::cout << "obj.e.i= 2=" << _.read(obj).e.i << std::endl;
        std::cout << "obj.j= 1!=" << _.read(obj).j << std::endl;
        BOOST_STM_TX_RETURN(_, _.read(obj).e.i==2 && _.read(obj).j==1) ;
    } BOOST_STM_RETRY
    return false;
}


int main() {
    transaction::enable_dynamic_priority_assignment();
    transaction::do_deferred_updating();
    transaction::initialize();
    thread_initializer thi;

    int res=0;
    res+=!check_in_nested_fails();
    res+=!check_after_nested_fails();
    res+=!mod_outer_same_in_nested_fails();
    res+=!mod_inner_same_in_nested_and_check_after_fails();
    return res;

}

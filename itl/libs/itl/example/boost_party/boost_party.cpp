/*---------------------------------------------------------------------------+
Interval Template Library
Author: Joachim Faulhaber
Copyright (c) 2007-2008: Joachim Faulhaber
Copyright (c) 1999-2006: Cortex Software GmbH, Kantstrasse 57, Berlin
+-----------------------------------------------------------------------------+
   Distributed under the Boost Software License, Version 1.0.
      (See accompanying file LICENCE.txt or copy at
           http://www.boost.org/LICENSE_1_0.txt)
+----------------------------------------------------------------------------*/

/** Example boost_party.cpp \file boost_party.cpp

    boost_party.cpp demonstrates the possibilities of an interval map 
    (interval_map or split_interval_map). Boost::posix_time::ptime is used as time
    parameter. An interval_map maps intervals to a given content. In this case the 
    content is a set of party guests represented by their name strings.

    As time goes by, groups of people join the party and leave later in the evening.
    So we add a time interval and a name set to the interval_map for the attendance
    of each group of people, that come together and leave together.

    On every overlap of intervals, the corresponding name sets are accumulated. At
    the points of overlap the intervals are split. The accumulation of content on
    overlap of intervals is done via an operator += that has to be implemented
    for the content parameter of the interval_map.

    Finally the interval_map contains the history of attendance and all points in
    time, where the group of party guests changed.

    boost_party.cpp demonstrates a principle that we call 
    <b><em>aggregate on overlap (aggrovering;)</em></b>:
    On insertion a value associated to the interval is aggregated (added) to those
    values in the interval_map that overlap with the inserted value.

    There are two behavioral aspects to <b>aggrovering</b>: a <em>decompositional
    behavior</em> and a <em>accumulative behavior</em>.

    The <em>decompositional behavior</em> splits up intervals on the time dimension of the 
    interval_map so that the intervals change whenever associated values
    change.

    The <em>accumulative behavior</em> accumulates associated values on every overlap of
    an insertion for the associated values.

    \include boost_party/boost_party.cpp
*/
//[example_boost_party
#include <iostream>
// The next line includes <boost/date_time/posix_time/posix_time.hpp>
// and a few lines of adapter code.
#include <boost/itl/ptime.hpp> 

#include <boost/itl/type_traits/to_string.hpp>
#include <boost/itl/interval_map.hpp>

using namespace std;
using namespace boost::posix_time;
using namespace boost::itl;

// Type itl::set<string> collects the names of party guests. Therefore it needs to
// implement operator += that performs a set union on overlap of intervals.
typedef boost::itl::set<string> GuestSetT;

// boost::posix_time::ptime is the domain type the the interval_map. 
// It's key values are therefore time intervals: interval<ptime>. The content
// is the set of names: GuestSetT.
typedef interval_map<ptime, GuestSetT> BoostPartyAttendenceHistoryT;

// A party's height shall be defined as the maximum height of all guests ;-)
typedef interval_map<ptime, int> BoostPartyHeightHistoryT;

void boost_party()
{
    GuestSetT mary_harry; 
    mary_harry.insert("Mary");
    mary_harry.insert("Harry");

    GuestSetT diana_susan; 
    diana_susan.insert("Diana");
    diana_susan.insert("Susan");

    GuestSetT peter; 
    peter.insert("Peter");

    BoostPartyAttendenceHistoryT party;

    party.add( // add and element
      make_pair( 
        interval<ptime>::rightopen(
          time_from_string("2008-05-20 19:30"), 
          time_from_string("2008-05-20 23:00")), 
          mary_harry));

    party += // element addition can also be done via operator +=
      make_pair( 
        interval<ptime>::rightopen(
          time_from_string("2008-05-20 20:10"), 
          time_from_string("2008-05-21 00:00")), 
          diana_susan);
    party +=
      make_pair( 
        interval<ptime>::rightopen(
          time_from_string("2008-05-20 22:15"), 
          time_from_string("2008-05-21 00:30")), 
          peter);

    //-------------------------------------------------------------------------
    BoostPartyHeightHistoryT tallest_guest;

    // adding an element can be done wrt. simple aggregate functions
    // like e.g. min, max etc. in their 'inplace' or op= incarnation
    tallest_guest.add(
      make_pair( 
        interval<ptime>::rightopen(
          time_from_string("2008-05-20 19:30"), 
          time_from_string("2008-05-20 23:00")), 
          180),
	  inplace_max<int>()
	);

    tallest_guest.add(
      make_pair( 
        interval<ptime>::rightopen(
          time_from_string("2008-05-20 20:10"), 
          time_from_string("2008-05-21 00:00")), 
          170),
	  inplace_max<int>()
	);

    tallest_guest.add(
      make_pair( 
        interval<ptime>::rightopen(
          time_from_string("2008-05-20 22:15"), 
          time_from_string("2008-05-21 00:30")), 
          200),
	  inplace_max<int>()
	);


    BoostPartyAttendenceHistoryT::iterator it = party.begin();
    cout << "----- History of party guests -------------------------\n";
    while(it != party.end())
    {
        interval<ptime> when = (*it).first;
        // Who is at the party within the time interval 'when' ?
        GuestSetT who = (*it++).second;
        cout << "[" << when.first() << " - " << when.upper() << ")"
             << ": " << who.as_string() << endl;
    }

    BoostPartyHeightHistoryT::iterator height_ = tallest_guest.begin();
    cout << "----- History of maximum guest height -----------------\n";
    while(height_ != tallest_guest.end())
    {
        interval<ptime> when = height_->first;
        // Of what height are the tallest guests within the time interval 'when' ?
        int height = (*height_++).second;
        cout << "[" << when.first() << " - " << when.upper() << ")"
             << ": " << height <<" cm = " << height/30.48 << " ft" << endl;
    }
}


int main()
{
    cout << ">> Interval Template Library: Sample boost_party.cpp <<\n";
    cout << "-------------------------------------------------------\n";
    boost_party();
    return 0;
}

// Program output:
/*-----------------------------------------------------------------------------
>> Interval Template Library: Sample boost_party.cpp <<
-------------------------------------------------------
----- History of party guests -------------------------
[2008-May-20 19:30:00 - 2008-May-20 20:10:00): Harry Mary
[2008-May-20 20:10:00 - 2008-May-20 22:15:00): Diana Harry Mary Susan
[2008-May-20 22:15:00 - 2008-May-20 23:00:00): Diana Harry Mary Peter Susan
[2008-May-20 23:00:00 - 2008-May-21 00:00:00): Diana Peter Susan
[2008-May-21 00:00:00 - 2008-May-21 00:30:00): Peter
----- History of maximum guest height -----------------
[2008-May-20 19:30:00 - 2008-May-20 22:15:00): 180 cm = 5.90551 ft
[2008-May-20 22:15:00 - 2008-May-21 00:30:00): 200 cm = 6.56168 ft
-----------------------------------------------------------------------------*/
//]
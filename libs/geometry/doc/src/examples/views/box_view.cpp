// Boost.Geometry (aka GGL, Generic Geometry Library)
// QuickBook Example

// Copyright (c) 2011 Barend Gehrels, Amsterdam, the Netherlands.

// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

//[box_range
//` Shows usage of the Boost.Range compatible view on a box

#include <iostream>

#include <boost/geometry.hpp>


int main()
{
    typedef boost::geometry::model::box
        <
            boost::geometry::model::point<double, 2, boost::geometry::cs::cartesian>
        > box_type;
        
    // Define the Boost.Range compatible type:
    typedef boost::geometry::box_range<box_type> box_range;        
        
    box_type box;
    boost::geometry::assign_values(box, 0, 0, 4, 4);
    
    box_range range(box);
    
    // Iterating in clockwise direction over the points of this box
    for (boost::range_iterator<box_range const>::type it = boost::begin(range);
        it != boost::end(range); ++it)
    {
        std::cout << " " << boost::geometry::dsv(*it);
    }
    std::cout << std::endl;
    
    // Note that a box_range is tagged as a ring, so supports area etc.
    std::cout << "Area: " << boost::geometry::area(range) << std::endl;
    
    return 0;
}

//]


//[box_range_output
/*`
Output:
[pre
 (0, 0) (0, 4) (4, 4) (4, 0) (0, 0)
Area: 16
]
*/
//]

/**
 * Non-intrusive Assignment Library
 * (C) Thorsten Ottosen 2002-2003
 * 
 * Permission to copy, use, modify,
 * sell and distribute this software is granted provided this
 * copyright notice appears in all copies. This software is provided
 * "as is" without express or implied warranty, and with no claim as
 * to its suitability for any purpose.
 *
 * See http://www.boost.org/libs/assign for documentation.
 *
 */

#include <boost/multi_array.hpp>
#include <boost/assign/fixed_size_assigner.hpp>
#include <boost/assign/array/multi_array.hpp>
#include <iostream>
#include <algorithm>
#include <iterator>
#include <cassert>

int main()
{
    using namespace std;
    using namespace boost;
    using namespace boost::assignment;

    try
    {
    	///////////////////////////////////////////////////////////////////////
    	// 'boost::array'
    	///////////////////////////////////////////////////////////////////////
    	
    	typedef boost::array<float,6> Array;
    
    	// create and initialize an array
    	Array a; 
       	copy( a.begin(), a.end(), ostream_iterator< float >( cout, " " ) );
    	
    	a << 1,2,3,4,5,6;
        assign_all( a )(2)(3)(4)(5)(6)(7);
    
    	copy( a.begin(), a.end(), ostream_iterator< float >( cout, " " ) );
    
    	///////////////////////////////////////////////////////////////////////
    	// 'boost::multi_array'
    	///////////////////////////////////////////////////////////////////////
    
     	// Create a 3D array that is 3 x 4 x 2
    	typedef boost::multi_array<double, 3> array_type;
    	typedef array_type::index index;
    	array_type A(boost::extents[3][4][2]);
    	
    	// Assign values to the elements
        // use tuples (here a pair) in a 3x4 matrix
    	assign_all( A ) (1,1)(2,2)(3,3)
                        (4,4)(5,5)(6,6)
                        (7,7)(8,8)(9,9);
        
        // use plain comma-separated list
        A << 1.,1, 2,2, 3,3, 
             4,4, 5,5, 6,6,
             7,7, 8,8, 9,9; 
    	
    	// Verify values
    	int verify = 0;
    	for(index i = 0; i != 3; ++i) 
    	    for(index j = 0; j != 4; ++j)
                for(index k = 0; k != 2; ++k)
                    assert(A[i][j][k] == verify++);
                        
    }
    catch( std::exception& e )
    {
        cout << e.what() << endl;
    }
    
    return 0;
}

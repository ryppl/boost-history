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

#include <boost/assign/ublas.hpp>
#include <boost/numeric/ublas/io.hpp>
#include <boost/numeric/ublas/functional.hpp>
#include <iostream>

int main()
{

    using namespace std;
    namespace bnu = boost::numeric::ublas;
    using namespace boost::assignment;
	
    try
    {
        bnu::vector<double>  v( 3 );
        v << 1,2,3; 
        
        bnu::matrix<int>     m( 3, 3 );
        m << 1,2,3,
             4,5,6,
             7,8,9;
        
        assign_all( m ) (1,2,3)
                        (4,5,6)
                        (6,7,8);
        
        bnu::matrix<double, bnu::column_major> m2( 3, 3 );
        m2 << 1,2,3,
              4,5,6,
              7,8,9;
        
        assign_all( m2 ) (1,2,3)
                         (4,5,6)
                         (6,7,8);
        
        cout << v << endl <<  m << endl << m2 << endl;
        
        bnu::triangular_matrix<double, bnu::lower> ml (3, 3);
        ml << 1,
              2,3,
              4,5,6;
        	
        bnu::triangular_matrix<double, bnu::upper> mu (3, 3);
        mu << 1,2,3,
                4,5,
                  6;
        	
        cout << ml << endl << mu << endl;
        
        bnu::symmetric_matrix<double, bnu::lower> msl (3, 3);
        msl << 0, 
               2, 0,
               3, 4, 0; 
        
        bnu::symmetric_matrix<double, bnu::upper> msu (3, 3);
        msu << 0, 2, 3,
                  0, 4,
                     0; 
        
        cout << msl << endl << msu << endl;
        
    }
    catch( std::exception& e )
    {
        cout << e.what() << endl;
    }
    
    return 0;
}

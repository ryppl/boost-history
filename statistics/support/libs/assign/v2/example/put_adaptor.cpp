//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#include <vector>
#include <list>
#include <map>
#include <boost/array.hpp>
#include <boost/lambda/bind.hpp>
#include <boost/ptr_container/ptr_vector.hpp>
#include <libs/assign/v2/example/include.h>
#include <libs/assign/v2/example/foo.h>
#include <libs/assign/v2/example/constants.h>
#include <libs/assign/v2/example/put_adaptor.h>

namespace example_assign_v2{
namespace xxx_put_adaptor{

    void run(std::ostream& os)
    {
        using namespace boost::assign::v2;
        os << "* xxx_put_adaptor " << std::endl;
        {
            os << "vector" << ' ';
            //[vector
            typedef std::vector<T> cont_;
            cont_ cont;
            using namespace adaptor;
            boost::for_each( cont | _csv_put( x, y, z ), printer(os) );
            //outputs (1,0)(NULL,0)(1,5)
            //]
            os << std::endl;
        }
        {
            //[map
            typedef std::map<S,T> cont_;
            typedef cont_::value_type key_data_;
            cont_ cont;
            using namespace adaptor;
            BOOST_ASSERT( 
                (
                    cont | _csv_put( 
                        key_data_(key_z, z), 
                        key_data_(key_y, y), 
                        key_data_(key_x, x)
                    )
                )[key_x] == x
            );
            //]
            //[map_proper
            cont.clear();
            BOOST_ASSERT( 
                ( cont | _put (key_z, z)(key_y, y)(key_x, x) )[key_x] == x
            );
            //]            
        }
        {
            os << "static_array" << ' ';
            //[static_array
            typedef boost::array<T,3> cont_;
            cont_ cont;
            using namespace adaptor;
            boost::for_each( cont | _csv_put( x, y, z ), printer(os) );
            //]
            os << std::endl;
        }
        {
            os << "ptr_vector" << ' ';
            //[ptr_vector
            typedef boost::ptr_vector<T> cont_; cont_ cont;
            using namespace adaptor;
            boost::for_each( 
                cont | _csv_put( x, y, z ), 
                printer(os) 
            );
            //]
            os << std::endl;
        }
        {
            os << "list" << ' ';
            //[list
            typedef std::list<T> cont_;
            cont_ cont;
            using namespace adaptor;
            boost::for_each( 
                cont | (_csv_put % _push_front)( x, y, z ), 
                printer(os) 
            );
            //outputs (1,5)(NULL,0)(1,0)
            //]
            os << std::endl;
        }
        {
            os << "in_place" << ' ';
            //[in_place
            typedef std::vector<T> cont_;
            cont_ cont;
            using namespace adaptor;
            boost::for_each( 
                cont | _csv_put( foo( a ), foo(), foo( a, b ) ), 
                printer(os) 
            );
            //]
            os << std::endl;
        }
        {
            os << "forward_to_constructor" << ' ';
            //[forward_to_constructor
            typedef std::vector<T> cont_;
            cont_ cont;
            using namespace adaptor;
            boost::for_each( 
                cont | _put( a )()( a, b ), 
                printer(os) 
            );
            //]

            os << std::endl;
            os << "forward_to_make_foo" << ' ';

            //[forward_to_make_foo
            int a0 = -1, a1 = 0, a2 = 1;
            cont.clear();
            boost::for_each( 
                cont | (
                    _put % ( _fun = make_foo() )
                )( a0, b )( a1, b )( a2, b ),
                printer(os)
            );
            //outputs (-1,5)(0,5)(1,5)
            //]

            os << std::endl;
            os << "forward_to_bind" << ' ';

            //[forward_to_bind
            cont.clear();
            using namespace boost::lambda;
            boost::for_each( 
                cont | (
                    _csv_put % (_fun = bind<T>( make_foo(), _1, b)  )
                )(a0, a1, a2), 
                printer(os) 
            );
            //]
            os << std::endl;
            os << "repeat_simpl" << ' ';
            {
                //[repeat_simple
                typedef std::vector<T> cont_;
                cont_ cont;
                using namespace adaptor;
                boost::for_each( 
                    cont | ( _csv_put % ( _repeat = 2  ) )( x ),
                    printer(os) 
                );
                // outputs (1,0)(1,0)
                //]
            }
            {
                os << std::endl;
            	os << "incr_lookup" << ' ';
                //[incr_lookup
                typedef std::map<S,int> cont_;
                cont_ cont;
                using namespace adaptor;
                cont | (
                    _csv_put % ( _incr_lookup = 2 ) 
                )( key_z, key_x, key_y );
                os 
                    << '('
                    << cont[key_x] << ',' 
                    << cont[key_y] << ',' 
                    << cont[key_z] << ')';
                // outputs (2,2,2)
                //]
            }
            {
                os << std::endl;
            	os << "complex_ex1" << ' ';
            	//[complex_ex1
                typedef std::list<T> cont_;	
                cont_ cont;
                using namespace boost::lambda;
                boost::for_each( 
                    cont | ( _csv_put )( x ) /*(1,0)*/ | ( 
                        _put % _push_front % ( _repeat = 2 ) 
                    )()( b, a ) /*(NULL,0)(5,1)*/ | ( 
                        _csv_put % ( _fun = bind<T>( make_foo(), _1, b) )
                    )( a0, a1, a2 ), /*(-1,5)(0,5)(1,5)*/
                    printer(os) 
                );
                //]
                //[complex_ex1_answer
                // outputs (5,1)(5,1)(NULL,0)(NULL,0)(1,0)(-1,5)(0,5)(1,5)
                //]
			}
        }
        os << std::endl;
    }

}// xxx_put_adaptor
}// example_assign_v2

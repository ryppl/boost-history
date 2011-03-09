//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_PUT_DEQUE_CONVERSION_ER_2010_HPP
#define BOOST_ASSIGN_V2_PUT_DEQUE_CONVERSION_ER_2010_HPP
#include <boost/assign/v2/put/deque/fwd.hpp>
#include <boost/assign/v2/utility/conversion/convert.hpp>
#include <boost/assign/v2/utility/conversion/converter.hpp>

namespace boost{
namespace assign{
namespace v2{
namespace put_aux{

#define BOOST_ASSIGN_V2_SEQ (T)(F)(Tag)
#define BOOST_ASSIGN_V2_R deque_adapter<T, F, Tag>
BOOST_ASSIGN_V2_UTILITY_CONVERSION_CONVERT_NAME_LOOKUP(BOOST_ASSIGN_V2_SEQ, BOOST_ASSIGN_V2_R)
BOOST_ASSIGN_V2_UTILITY_CONVERSION_CONVERTER_NAME_LOOKUP(BOOST_ASSIGN_V2_SEQ, BOOST_ASSIGN_V2_R)
#undef BOOST_ASSIGN_V2_SEQ
#undef BOOST_ASSIGN_V2_R

}// put_aux
}// v2
}// assign
}// boost

#endif // BOOST_ASSIGN_V2_PUT_DEQUE_CONVERSION_ER_2010_HPP

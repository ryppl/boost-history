//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_REF_ARRAY_CSV_NTH_RESULT_OF_ER_2010_HPP
#define BOOST_ASSIGN_V2_REF_ARRAY_CSV_NTH_RESULT_OF_ER_2010_HPP
#include <boost/mpl/empty_base.hpp>
#include <boost/assign/v2/ref/array/alloc/instant_fwd.hpp>
#include <boost/assign/v2/ref/array/tag1.hpp>

#include <boost/assign/v2/ref/array/impl_traits.hpp>

namespace boost{
    struct use_default;
namespace assign{
namespace v2{
namespace ref{
namespace array_aux{
    typedef boost::use_default csv_tag2_;
}// array_aux
namespace nth_result_of{

    template<std::size_t N, typename T>
    struct csv_array
    {
        typedef ref::array_tag1_ tag1_;
//        typedef array_aux::instant_alloc<N, tag1_, T> type;
        typedef array_aux::instant_alloc<N, T> type;
    };

}// nth_result_of
}// ref
}// v2
}// assign
}// boost

#endif

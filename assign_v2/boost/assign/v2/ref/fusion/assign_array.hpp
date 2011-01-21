//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_REF_FUSION_ASSIGN_ARRAY_ER_2010_HPP
#define BOOST_ASSIGN_V2_REF_FUSION_ASSIGN_ARRAY_ER_2010_HPP
#include <boost/static_assert.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/mpl/int.hpp>
#include <boost/mpl/equal_to.hpp>
#include <boost/assign/v2/ref/fusion/fwd.hpp>
#include <boost/assign/v2/ref/fusion/size_type.hpp>

namespace boost{
	struct use_default;
namespace assign{
namespace v2{
namespace ref{
namespace fusion_aux{

    // ---- assign_array ---- //

    template<
    	fusion_aux::size_type K,typename A,
        fusion_aux::size_type N, typename L, typename Tag1, typename Tag2, typename T
    >
    void assign_array(
    	::boost::mpl::true_ /*exit*/,
    	A& a,
        const fusion_aux::container<N, L, Tag1, Tag2, T>& f
    )
    {
    	/*exit*/
    }

    template<
    	fusion_aux::size_type K,typename A,
        fusion_aux::size_type N, typename L, typename Tag1, typename Tag2,typename T
    >
    void assign_array(
    	::boost::mpl::false_ /*exit*/,
    	A& a,
        const fusion_aux::container<N, L, Tag1, Tag2, T>& f
    )
    {
        typedef ::boost::mpl::int_<K-1> index_;
        a[ K - 1 ].rebind( f.static_lookup( index_() ) ) ;
        typedef index_ next_size_;
        typedef ::boost::mpl::int_<0> zero_;
        typedef typename ::boost::mpl::equal_to<next_size_,zero_>::type exit_;
        assign_array<K-1>( exit_(), a, f );
    }

	// A must be a static array of reference wrappers
    template<typename A,fusion_aux::size_type N, typename L,
    	typename Tag1, typename Tag2, typename T>
    void assign_array(
    	A& a,
        fusion_aux::container<N, L, Tag1, Tag2, T> const & f
    )
    {
    	// don't replace by size_::value <= N (causes warning)
    	BOOST_STATIC_ASSERT( A::static_size <= N );
        typedef ::boost::mpl::int_<0> zero_;
        typedef ::boost::mpl::int_<A::static_size> size_;
        typedef typename ::boost::mpl::equal_to<size_, zero_>::type exit_;
        fusion_aux::assign_array<size_::value>( exit_(), a, f );
    }

}// fusion_aux

using fusion_aux::assign_array;

}// ref
}// v2
}// assign
}// boost

#endif

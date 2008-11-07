/* Copyright 2008 Vicente J. Botet Escriba
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 *
 * See http://www.boost.org/libs/mpl_ext for library home page.
 */

#ifndef BOOST_MPLEXT_OR_SEQ_HPP_
#define BOOST_MPLEXT_OR_SEQ_HPP_

#include <boost/mpl/not.hpp>
#include <boost/mpl/find_if.hpp>
#include <boost/mpl/end.hpp>
#include <boost/type_traits/detail/type_trait_def.hpp>

namespace boost { 
namespace mpl_ext {

/*
Calculate the mlp::or_ of a sequence of nullary logical metafunctions.
*/    
template <typename Seq>
struct or_seq : mpl::not_<boost::is_same<typename mpl::find_if<Seq, mpl::_>::type, 
										 typename mpl::end<Seq>::type> 
				> {};

} // mpl_ext
} // boost
#endif // BOOST_MPLEXT_OR_SEQ_HPP_

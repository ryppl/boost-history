//  Boost string_algo library sequence_traits.hpp header file  ---------------------------//

//  (C) Copyright Pavol Droba 2002. Permission to copy, use, modify, sell and
//  distribute this software is granted provided this copyright notice appears
//  in all copies. This software is provided "as is" without express or implied
//  warranty, and with no claim as to its suitability for any purpose.

//  See http://www.boost.org for updates, documentation, and revision history.

#ifndef BOOST_STRING_SEQUENCE_TRAITS_HPP
#define BOOST_STRING_SEQUENCE_TRAITS_HPP

#include <boost/config.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/mpl/bool_c.hpp>
#include <boost/mpl/find_if.hpp>
#include <boost/mpl/list.hpp>
#include <boost/string_algo/detail/yes_no_type.hpp>

// Forward definitions for std containers
namespace std {
    template<typename T, typename TraitsT, typename AllocT> class basic_string;
    template<typename T, typename AllocT> class list;
    template<typename T, typename AllocT > class rope;
    template<typename T, typename AllocT > class slist;
}

namespace boost {

    namespace string_algo {

//  sequence traits  -----------------------------------------------//

		// native replace tester
		template<typename T, typename TraitsT, typename AllocT>
		yes_type sequence_has_native_replace( const std::basic_string<T, TraitsT, AllocT>* );
		no_type sequence_has_native_replace(...);

		// stable iterators tester
		template<typename T, typename AllocT>
		yes_type sequence_has_stable_iterators( const std::list<T,AllocT>* );
		no_type sequence_has_stable_iterators(...);		        		

		// const time insert tester
		template<typename T, typename AllocT>
		yes_type sequence_has_const_time_insert( const std::list<T,AllocT>* );
		no_type sequence_has_const_time_insert(...);		        		

		// const time erase tester
		template<typename T, typename AllocT>
		yes_type sequence_has_const_time_erase( const std::list<T,AllocT>* );
		no_type sequence_has_const_time_erase(...);		        		

		template< typename SequenceT >
		struct sequence_traits
		{
		private:
			// traits resolving
            static SequenceT* t;
            BOOST_STATIC_CONSTANT(bool, native_replace_=( 
				sizeof(sequence_has_native_replace(t))==sizeof(yes_type) ) );
            BOOST_STATIC_CONSTANT(bool, stable_iterators_=( 
				sizeof(sequence_has_stable_iterators(t))==sizeof(yes_type) ) );
            BOOST_STATIC_CONSTANT(bool, const_time_insert_=( 
				sizeof(sequence_has_const_time_insert(t))==sizeof(yes_type) ) );
            BOOST_STATIC_CONSTANT(bool, const_time_erase_=( 
				sizeof(sequence_has_const_time_erase(t))==sizeof(yes_type) ) );

		public:
            // Container features
			typedef ::boost::mpl::bool_c<native_replace_> native_replace;
            typedef ::boost::mpl::bool_c<stable_iterators_> stable_iterators;
            typedef ::boost::mpl::bool_c<const_time_insert_> const_time_insert;
            typedef ::boost::mpl::bool_c<const_time_erase_> const_time_erase;                
		};

    } // namespace string_algo

} // namespace boost


#endif  // BOOST_STRING_SEQUENCE_TRAITS_HPP

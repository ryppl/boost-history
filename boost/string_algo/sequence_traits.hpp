//  Boost string_algo library sequence_traits.hpp header file  ---------------------------//

//  (C) Copyright Pavol Droba 2002-2003. Permission to copy, use, modify, sell and
//  distribute this software is granted provided this copyright notice appears
//  in all copies. This software is provided "as is" without express or implied
//  warranty, and with no claim as to its suitability for any purpose.

//  See http://www.boost.org for updates, documentation, and revision history.

#ifndef BOOST_STRING_SEQUENCE_TRAITS_HPP
#define BOOST_STRING_SEQUENCE_TRAITS_HPP

#include <boost/config.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/string_algo/yes_no_type.hpp>

/*! \file
    Traits defined in this header are used by various algorithms to achieve
    better performance for specific containers.
    Traits provide fail-safe defaults. If a container supports some this
	features, it is possible to specialize the specific trait for this container.
	For lacking compilers, it is possible of define override for a specific tester
	function.

	Due to language restriction, it is not currently possible to specialize
	stl containers without including the stl header. To decrease overhead,
	needed by this inclusion, user can selectively include specialization 
	header for a specific container. They are located in boost/string_algo/stl
	directory. Alternatively she can include boost/string_algo/std_container_traits.hpp
	header which container specialization for all stl containers.
*/

namespace boost {

    namespace string_algo {

//  sequence traits  -----------------------------------------------//

        //! Native replace tester
        /*!
			Declare an override of this tester function with return 
			type boost::string_algo::yes_type for a sequence with this property.

            \return yes_type if the container has basic_string like native replace
            method.
        */
        no_type sequence_has_native_replace_tester(...);

        //! Native replace trait
        /*!
			This trait specifies that the sequence has std::string like replace method
        */
		template< typename T >
		class sequence_has_native_replace
		{
        private:
            static T* t;
		public:
            BOOST_STATIC_CONSTANT(bool, value=( 
                sizeof(sequence_has_native_replace_tester(t))==sizeof(yes_type) ) );
			typedef mpl::bool_<value> type;		
		};

        //! Stable iterators tester
        /*!
			Declare an override of this tester function with return 
			type boost::string_algo::yes_type for a sequence with this property.

			\return yes_type if the seqeunce's insert/replace/erase methods do not invalidate
            existing iterators.
        */
        no_type sequence_has_stable_iterators_tester(...);                     

        //! Stable iterators trait
        /*!
			This trait specifies that the sequence has stable iterators. It means,
			that operations like insert/erase/replace do not invalidate iterators.
        */
		template< typename T >
		class sequence_has_stable_iterators
		{
        private:
            static T* t;
		public:

            BOOST_STATIC_CONSTANT(bool, value=( 
                sizeof(sequence_has_stable_iterators_tester(t))==sizeof(yes_type) ) );
			typedef mpl::bool_<value> type;
		};

        //! const time insert tester
        /*!
			Declare an override of this tester function with return 
			type boost::string_algo::yes_type for a sequence with this property.

			\return yes_type if the sequence's insert method is working in constant time
        */
        no_type sequence_has_const_time_insert_tester(...);                        

		//! Const time insert trait
		/*
			This trait specifies that the sequence's insert method is has 
			constant time complexity.
		*/
		template< typename T >
		class sequence_has_const_time_insert
		{
        private:
            static T* t;
		public:
            BOOST_STATIC_CONSTANT(bool, value=( 
                sizeof(sequence_has_const_time_insert_tester(t))==sizeof(yes_type) ) );
			typedef mpl::bool_<value> type;
		};

        //! const time erase tester
        /*!
			Declare an override of this tester function with return 
			type boost::string_algo::yes_type for a sequence with this property.

			\return yes_type if the sequence's erase method is working in constant time
        */
        no_type sequence_has_const_time_erase_tester(...);                        

		//! Const time erase trait
		/*
			This trait specifies that the sequence's erase method is has 
			constant time complexity.
		*/
		template< typename T >
		class sequence_has_const_time_erase
		{
        private:
            static T* t;
		public:
            BOOST_STATIC_CONSTANT(bool, value=( 
                sizeof(sequence_has_const_time_erase_tester(t))==sizeof(yes_type) ) );
			typedef mpl::bool_<value> type;
		};

    } // namespace string_algo

} // namespace boost


#endif  // BOOST_STRING_SEQUENCE_TRAITS_HPP

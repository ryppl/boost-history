//  Boost string_algo library string_util.hpp header file  ---------------------------//

//  (C) Copyright Pavol Droba 2002. Permission to copy, use, modify, sell and
//  distribute this software is granted provided this copyright notice appears
//  in all copies. This software is provided "as is" without express or implied
//  warranty, and with no claim as to its suitability for any purpose.

//  See http://www.boost.org for updates, documentation, and revision history.

#ifndef BOOST_STRING_UTIL_DETAIL_HPP
#define BOOST_STRING_UTIL_DETAIL_HPP

#include <functional>

namespace boost {

    namespace string_algo {

        namespace detail {

//  equal_to functor  -----------------------------------------------//

			// equal_to functor reimplementation
            /*
                standard STL implementation does handle only comparison between types
                of the same value. This is less restrictive version which wraps == operator.
            */
            template< typename Type1, typename Type2 >
	        struct equal_toF : public std::binary_function< Type1, Type2, bool > 
            {
                typedef typename std::binary_function< Type1, Type2, bool >::result_type result_type;
                typedef typename std::binary_function< Type1, Type2, bool >::first_argument_type first_argument_type;
                typedef typename std::binary_function< Type1, Type2, bool >::second_argument_type second_argument_type;

                result_type operator ()( const first_argument_type& Arg1, const second_argument_type& Arg2 ) const
                {
                    return Arg1==Arg2;
                }
			};


//  replace_range functor  -------------------------------------------//

			// Replace a range in the sequence with another range ( functor )
            /*
				Returns the iterator pointing just after inserted subrange.
				Note:
					Function first tries to replace elements in the input sequence,
					up to To position with the elements form Repl sequence.
					The rest of the Repl is then inserted thereafter.
					If the Repl sequence is shorter, overlaping elements are erased from the Input
			*/
            template< typename InputT, typename ReplaceT >
            struct replace_rangeF
			{
				typedef InputT input_type;
				typedef typename InputT::iterator input_iterator_type;
				typedef input_type& input_reference_type;
				typedef ReplaceT replace_type;
				typedef typename ReplaceT::const_iterator replace_iterator_type;
				typedef const replace_type& replace_reference_type;

				// Operation
				input_iterator_type operator()(
                    input_reference_type Input,
					input_iterator_type From,
					input_iterator_type To,
					replace_reference_type Repl )
				{       
					typename input_iterator_type InsertIt=From;
					bool bReplace=(InsertIt!=To);

					for(replace_iterator_type ReplIt=Repl.begin();
						ReplIt!=Repl.end();
						ReplIt++)
					{
						if ( bReplace )
						{
							// Replace mode
							*InsertIt=*ReplIt;

							InsertIt++;
							bReplace=(InsertIt!=To);
						}
						else
						{
							// Insert mode
							InsertIt=Input.insert( InsertIt, *ReplIt );
							// Advance to the next item
							InsertIt++;
						}
					}

					// Erase the overlapping elements
					if ( bReplace )
					{
						InsertIt=Input.erase( InsertIt, To );
					}

					// Return InputIt iterator
					return InsertIt;
				}
			};

//  insert_range functor  -------------------------------------------//
		
            template< typename InputT, typename InsertT >
            struct insert_rangeF
			{
				typedef InputT input_type;
				typedef typename input_type::iterator input_iterator_type;
				typedef input_type& input_reference_type;
				typedef InsertT insert_type;
				typedef const insert_type& insert_reference_type;
				typedef insert_type::const_iterator insert_iterator_type;

				// Sequence variant
				void operator()(
                    input_reference_type Input,
					input_iterator_type At,
					insert_reference_type Insert )
				{
					Input.insert( At, Insert.begin(), Insert.end() );
				}

				// Iterator variant
				void operator()(
                    input_reference_type Input,
					input_iterator_type At,
					insert_iterator_type Begin,
					insert_iterator_type End)
				{
					Input.insert( At, Begin, End );
				}
			};
			
//  erase_range functor  -------------------------------------------//

			// Erase a range in the sequence ( functor )
            /*
				Returns the iterator pointing just after the erase subrange
			*/
            template< typename InputT >
            struct erase_rangeF
			{
				typedef InputT input_type;
				typedef typename InputT::iterator input_iterator_type;
				typedef input_type& input_reference_type;

				input_iterator_type operator()(
					input_reference_type Input,
					input_iterator_type From,
					input_iterator_type To )
				{
					return Input.erase( From, To );
				}
			};

        } // namespace detail

//  iterator range  -----------------------------------------------//

		// iterator_range
		/*
			Allow to threat a pair of iterators as a sequence by 
			string algorithms.
		*/
		template< typename IteratorT > class iterator_range
		{
		public:
			typedef iterator_range<IteratorT> type;
			typedef typename IteratorT::value_type value_type;
			typedef IteratorT const_iterator;
			typedef	IteratorT iterator;

			// Constructors
			iterator_range() {}
			iterator_range( IteratorT Begin, IteratorT End ) : 
				m_Begin( Begin ), m_End( End ) {}
			template< typename OtherItT >
			iterator_range( const iterator_range<OtherItT>& Other ) :
				m_Begin( Other.begin() ), m_End( Other.end() ) {}

			// Assignment
			template< typename OtherItT >
			iterator_range& operator=( const iterator_range<OtherItT>& Other )
			{
				m_Begin=Other.begin(); m_End=Other.end();
				return *this;
			}

			// Comparison
			template< typename OtherItT > 
			bool operator==( const iterator_range<OtherItT>& Other )
			{
				return m_Begin==Other.begin() && m_End==Other.end();
			}
			template< typename OtherItT > 
			bool operator!=( const iterator_range<OtherItT>& Other )
			{
				return m_Begin!=Other.begin() || m_End!=Other.end();
			}

			// Operations
			IteratorT begin() const { return m_Begin; } 
			IteratorT end() const { return m_End; } 
			bool empty() const { return m_Begin==m_End; } 

		private:
			// begin and end iterators
			IteratorT m_Begin;
			IteratorT m_End;
		};

		// iterator_range contructor ( iterator version )
		template< typename IteratorT >
		inline iterator_range< IteratorT > make_range( IteratorT Begin, IteratorT End ) 
		{	
			return iterator_range< IteratorT >( Begin, End );
		}
		// iterator_range contructors ( sequence version )
		template< typename SeqT >
		inline iterator_range< typename SeqT::iterator > make_range( SeqT& Seq ) 
		{	
			return iterator_range< SeqT::iterator >( Seq.begin(), Seq.end() );
		}
		// iterator_range contructor ( const sequence version  )
		template< typename SeqT >
		inline iterator_range< typename SeqT::const_iterator > make_range_const( const SeqT& Seq ) 
		{	
			return iterator_range< SeqT::const_iterator >( Seq.begin(), Seq.end() );
		}

    } // namespace string_algo

} // namespace boost


#endif  // BOOST_STRING_UTIL_DETAIL_HPP

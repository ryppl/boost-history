//  Boost string_algo library container.hpp header file  ---------------------------//

//  (C) Copyright Pavol Droba 2002. Permission to copy, use, modify, sell and
//  distribute this software is granted provided this copyright notice appears
//  in all copies. This software is provided "as is" without express or implied
//  warranty, and with no claim as to its suitability for any purpose.

//  See http://www.boost.org for updates, documentation, and revision history.

#ifndef BOOST_STRING_DETAIL_CONTAINER_HPP
#define BOOST_STRING_DETAIL_CONTAINER_HPP

namespace boost {

    namespace string_algo {

        namespace detail {


//  insert functor  -------------------------------------------//
        
            template< typename InputT >
            struct insertF
            {
                typedef InputT input_type;
                typedef typename input_type::iterator input_iterator_type;
                typedef input_type& input_reference_type;

                // Sequence variant
				template< typename InsertT >
                void operator()(
                    input_reference_type Input,
                    input_iterator_type At,
                    const InsertT& Insert )
                {
                    Input.insert( At, Insert.begin(), Insert.end() );
                }

                // Iterator variant
				template< typename InsertIteratorT >
                void operator()(
                    input_reference_type Input,
                    input_iterator_type At,
					InsertIteratorT Begin,
                    InsertIteratorT End )
                {
                    Input.insert( At, Begin, End );
                }
            };
            
//  erase functor  -------------------------------------------//

            // Erase a range in the sequence ( functor )
            /*
                Returns the iterator pointing just after the erase subrange
            */
            template< typename InputT >
            struct eraseF
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

//  replace functor  -------------------------------------------//
        
            template< typename InputT >
            struct replaceF
            {
                typedef InputT input_type;
                typedef typename input_type::iterator input_iterator_type;
                typedef input_type& input_reference_type;

                // Sequence variant
				template< typename InsertT >
                void operator()(
                    input_reference_type Input,
                    input_iterator_type From,
                    input_iterator_type To,
					const InsertT& Insert )
                {
					input_iterator_type At=Input.erase( From, To );
					if ( !Insert.empty() )
					{
						Input.insert( At, Insert.begin(), Insert.end() );
					}
                }

                // Iterator variant
				template< typename InsertIteratorT >
                void operator()(
                    input_reference_type Input,
                    input_iterator_type From,
                    input_iterator_type To,
					InsertIteratorT Begin,
                    InsertIteratorT End )
                {
                    input_iterator_type At=Input.erase( From, To );
					if ( Begin!=End )
					{
						Input.insert( At, Begin, End );
					}
                }
            };

//  replace_iter functor  -------------------------------------------//

            // Replace a range in the sequence with another range ( functor )
            /*
                Returns the iterator pointing just after inserted subrange.
				( This is an important difference from ordinary replace )
                Note:
                    Function first tries to replace elements in the input sequence,
                    up to To position with the elements form Repl sequence.
                    The rest of the Repl is then inserted thereafter.
                    If the Repl sequence is shorter, overlaping elements are erased from the Input
            */
            template< typename InputT >
            struct replace_iterF
            {
                typedef InputT input_type;
                typedef typename InputT::iterator input_iterator_type;
                typedef input_type& input_reference_type;

                // Operation
				template< typename ReplaceT >
                input_iterator_type operator()(
                    input_reference_type Input,
                    input_iterator_type From,
                    input_iterator_type To,
                    const ReplaceT& Repl )
                {       
					typedef typename ReplaceT::const_iterator replace_iterator_type;

                    input_iterator_type InsertIt=From;
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

		} // namespace detail

    } // namespace string_algo

} // namespace boost


#endif  // BOOST_STRING_DETAIL_CONTAINER_HPP

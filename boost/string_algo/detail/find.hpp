//  Boost string_algo library find.hpp header file  ---------------------------//

//  (C) Copyright Pavol Droba 2002. Permission to copy, use, modify, sell and
//  distribute this software is granted provided this copyright notice appears
//  in all copies. This software is provided "as is" without express or implied
//  warranty, and with no claim as to its suitability for any purpose.

//  See http://www.boost.org for updates, documentation, and revision history.

#ifndef BOOST_STRING_FIND_DETAIL_HPP
#define BOOST_STRING_FIND_DETAIL_HPP

#include <boost/string_algo/config.hpp>
#include <boost/detail/iterator.hpp>
#include <boost/string_algo/container_traits.hpp>
#include <boost/string_algo/iterator_range.hpp>

// Define container-based variant of functor operator for a find functor
#define BOOST_STRING_DEFINE_FIND_OPERATOR() \
	template<typename InputT> \
    iterator_range< BOOST_STRING_TYPENAME \
		string_algo::container_traits<InputT>::result_iterator > \
	operator()( InputT& Input ) const \
        { \
			return operator()( begin(Input), end(Input) ); \
		}

namespace boost {
    namespace string_algo {
        namespace detail {


//  find first functor -----------------------------------------------//

            // find a subsequnce in the sequence ( functor )
            /*
                Returns a pair <begin,end> marking the subsequence in the sequence. 
                If the find fails, functor returns <End,End>
            */
            template<typename SearchIteratorT>
			struct first_finderF
			{
                typedef SearchIteratorT search_iterator_type;

                // Construction
				template< typename SearchT >
                first_finderF( const SearchT& Search ) : 
					m_Search( begin(Search), end(Search) ) {}
				first_finderF( 
					search_iterator_type SearchBegin,
					search_iterator_type SearchEnd ) :
					m_Search( SearchBegin, SearchEnd ) {}

                // Operation
				template< typename ForwardIteratorT >
                iterator_range<ForwardIteratorT>
				operator()( 
					ForwardIteratorT Begin, 
                    ForwardIteratorT End ) const
                {
                    typedef ForwardIteratorT input_iterator_type;

					// Outer loop
                    for(input_iterator_type OuterIt=Begin;
                        OuterIt!=End;
                        OuterIt++)
                    {
						// Sanity check 
						if( m_Search.empty() ) 	
							return make_range( End, End );

						input_iterator_type InnerIt=OuterIt;
                        search_iterator_type SubstrIt=m_Search.begin();
                        for(;
                            InnerIt!=End && SubstrIt!=m_Search.end();
                            InnerIt++,SubstrIt++)
                        {
                            if( !( (*InnerIt)==(*SubstrIt) ) ) 
                                break;
                        }

                        // Substring matching succeeded
                        if ( SubstrIt==m_Search.end() )
                            return make_range( OuterIt, InnerIt );
                    }

                    return make_range( End, End );
                }

				// Container based find operator
				BOOST_STRING_DEFINE_FIND_OPERATOR()

            private:
                iterator_range<search_iterator_type> m_Search;
            };

//  find last functor -----------------------------------------------//

            // find the last match a subsequnce in the sequence ( functor )
            /*
                Returns a pair <begin,end> marking the subsequence in the sequence. 
                If the find fails, returns <End,End>
            */
            template<typename SearchIteratorT>
			struct last_finderF
            {
                typedef SearchIteratorT search_iterator_type;
                typedef first_finderF<search_iterator_type> first_finder_type;

                // Construction
				template< typename SearchT >
                last_finderF( const SearchT& Search ) : 
					m_Search( begin(Search), end(Search) ) {}
				last_finderF( 
						search_iterator_type SearchBegin,
						search_iterator_type SearchEnd ) :
					m_Search( SearchBegin, SearchEnd ) {}

                // Operation
				template< typename ForwardIteratorT >
                iterator_range<ForwardIteratorT>
				operator()( 
					ForwardIteratorT Begin, 
                    ForwardIteratorT End ) const
				{
					if( m_Search.empty() )
						return make_range( End, End );

					typedef BOOST_STRING_TYPENAME boost::detail::
                        iterator_traits<ForwardIteratorT>::iterator_category category;

                    return findit( Begin, End, category() );
                }   

				// Container based find operator
				BOOST_STRING_DEFINE_FIND_OPERATOR()

            private:
                // forward iterator
				template< typename ForwardIteratorT >
                iterator_range<ForwardIteratorT>
                findit( 
                    ForwardIteratorT Begin, 
                    ForwardIteratorT End, 
                    std::forward_iterator_tag ) const
                {
                    typedef ForwardIteratorT input_iterator_type;
					typedef iterator_range<ForwardIteratorT> result_type;

					first_finder_type first_finder( m_Search.begin(), m_Search.end() );

                    result_type M=first_finder( Begin, End );
                    result_type Last=M;

                    while( !empty(M) )
                    {
                        Last=M;
                        M=first_finder( end(M), End );
                    }

                    return Last;
                }

                // bidirectional iterator
				template< typename ForwardIteratorT >
                iterator_range<ForwardIteratorT>
                findit( 
                    ForwardIteratorT Begin, 
                    ForwardIteratorT End, 
                    std::bidirectional_iterator_tag ) const
                {
                    typedef ForwardIteratorT input_iterator_type;

					// Outer loop
                    for(input_iterator_type OuterIt=End;
                        OuterIt!=Begin; )
                    {
                        input_iterator_type OuterIt2=--OuterIt;

                        input_iterator_type InnerIt=OuterIt2;
                        search_iterator_type SubstrIt=m_Search.begin();
                        for(;
                            InnerIt!=End && SubstrIt!=m_Search.end();
                            InnerIt++,SubstrIt++)
                        {
                            if( !( (*InnerIt)==(*SubstrIt) ) ) 
                                break;
                        }

                        // Substring matching succeeded
                        if ( SubstrIt==m_Search.end() )
                            return make_range( OuterIt2, InnerIt );
                    }

                    return make_range( End, End );
                }

            private:
                iterator_range<search_iterator_type> m_Search;
            };
	
//  find n-th functor -----------------------------------------------//

            // find the n-th match of a subsequnce in the sequence ( functor )
            /*
                Returns a pair <begin,end> marking the subsequence in the sequence. 
                If the find fails, returns <End,End>
            */
            template< typename SearchIteratorT >
			struct nth_finderF
            {
                typedef SearchIteratorT search_iterator_type;
                typedef first_finderF<search_iterator_type> first_finder_type;

                // Construction
				template< typename SearchT >
                nth_finderF( const SearchT& Search, unsigned int Nth ) : 
                    m_Search(begin(Search), end(Search)), m_Nth(Nth) {}
				nth_finderF( 
						search_iterator_type SearchBegin,
						search_iterator_type SearchEnd,
						unsigned int Nth) :
				m_Search( SearchBegin, SearchEnd ), m_Nth(Nth) {}

                // Operation
				template< typename ForwardIteratorT >
                iterator_range<ForwardIteratorT>
				operator()( 
					ForwardIteratorT Begin, 
                    ForwardIteratorT End ) const
				{
					// Sanity check 
					if( m_Search.empty() ) 	
						return make_range( End, End );

					typedef ForwardIteratorT input_iterator_type;
					typedef iterator_range<ForwardIteratorT> result_type;

					// Instantiate find funtor 
                    first_finder_type first_finder( m_Search.begin(), m_Search.end() );

                    result_type M( Begin, Begin );

                    for( unsigned int n=0; n<=m_Nth; n++ )
                    {
                        // find next match        
                        M=first_finder( end(M), End );

                        if ( empty(M) )
                        {
                            // Subsequence not found, return 
                            return M;
                        }
                    }

                    return M;
                }

				// Container based find operator
				BOOST_STRING_DEFINE_FIND_OPERATOR()

			private:
                iterator_range<search_iterator_type> m_Search;
                unsigned int m_Nth;
            };

//  find head functor -----------------------------------------------//

            // find a head in the sequence ( functor )
            /*
                This functor find a head of the specified range. For 
                a specified N, the head is a subsequence of N starting 
                elements of the range.
            */
			struct head_finderF
            {
                // Construction
                head_finderF( unsigned int N ) : m_N(N) {}

                // Operation
				template< typename ForwardIteratorT >
                iterator_range<ForwardIteratorT>
				operator()( 
					ForwardIteratorT Begin, 
                    ForwardIteratorT End ) const
				{
                    typedef BOOST_STRING_TYPENAME boost::detail::
                        iterator_traits<ForwardIteratorT>::iterator_category category;

                    return findit( Begin, End, category() );
                }

				// Container based find operator
				BOOST_STRING_DEFINE_FIND_OPERATOR()

            private:
                // Find operation implementation
				template< typename ForwardIteratorT >
					iterator_range<ForwardIteratorT>
                findit( 
                    ForwardIteratorT Begin,
                    ForwardIteratorT End,
                    std::forward_iterator_tag ) const
                {
                    typedef ForwardIteratorT input_iterator_type;
					typedef iterator_range<ForwardIteratorT> result_type;

					input_iterator_type It=Begin;
                    for(
                        unsigned int Index=0; 
                        Index<m_N && It!=End; Index++,It++ );
    
                    return result_type( Begin, It );
                }

				template< typename ForwardIteratorT >
					iterator_range<ForwardIteratorT>
                findit( 
                    ForwardIteratorT Begin,
                    ForwardIteratorT End,
                    std::random_access_iterator_tag ) const
                {
                    typedef ForwardIteratorT input_iterator_type;
					typedef iterator_range<ForwardIteratorT> result_type;

					input_iterator_type It=Begin+m_N;
                    if ( It >= End ) It=End;

                    return result_type( Begin, It );
                }

            private:
                unsigned int m_N;
            };

//  find tail functor -----------------------------------------------//

            // find a tail in the sequence ( functor )
            /*
                This functor find a tail of the specified range. For 
                a specified N, the head is a subsequence of N starting 
                elements of the range.
            */
			struct tail_finderF
			{
                // Construction
                tail_finderF( unsigned int N ) : m_N(N) {}

                // Operation
				template< typename ForwardIteratorT >
                iterator_range<ForwardIteratorT>
				operator()( 
					ForwardIteratorT Begin, 
                    ForwardIteratorT End ) const
				{
                    typedef BOOST_STRING_TYPENAME boost::detail::
                        iterator_traits<ForwardIteratorT>::iterator_category category;

                    return findit( Begin, End, category() );
                }

        		// Container based find operator
				BOOST_STRING_DEFINE_FIND_OPERATOR()

			private:
                // Find operation implementation
				template< typename ForwardIteratorT >
					iterator_range<ForwardIteratorT>
                findit( 
                    ForwardIteratorT Begin,
                    ForwardIteratorT End,
                    std::forward_iterator_tag ) const
                {
                    typedef ForwardIteratorT input_iterator_type;
					typedef iterator_range<ForwardIteratorT> result_type;

					unsigned int Index=0; 
                    input_iterator_type It=Begin;
                    input_iterator_type It2=Begin;
                    
                    // Advance It2 by N incremets
                    for( Index=0; Index<m_N && It2!=End; Index++,It2++ );

                    // Advance It, It2 to the end
                    for(; It2!=End; It++,It2++ );

                    return result_type( It, It2 );
                }

				template< typename ForwardIteratorT >
					iterator_range<ForwardIteratorT>
                findit( 
                    ForwardIteratorT Begin,
                    ForwardIteratorT End,
                    std::bidirectional_iterator_tag ) const
                {
                    typedef ForwardIteratorT input_iterator_type;
					typedef iterator_range<ForwardIteratorT> result_type;

                    input_iterator_type It=End;
                    for(
                        unsigned int Index=0; 
                        Index<m_N && It!=Begin; Index++,It-- );
    
                    return result_type( It, End );
                }

				template< typename ForwardIteratorT >
					iterator_range<ForwardIteratorT>
                findit( 
                    ForwardIteratorT Begin,
                    ForwardIteratorT End,
                    std::random_access_iterator_tag ) const
                {
                    typedef ForwardIteratorT input_iterator_type;
					typedef iterator_range<ForwardIteratorT> result_type;

					if ( (End<=Begin) || (End < (Begin+m_N) ) ) 
                        return result_type( Begin, End );

                    return result_type( End-m_N, End );
                }


            private:
                unsigned int m_N;
            };

//  find token functor -----------------------------------------------//

            // find a token in a sequence ( functor )
            /*
				This find functor finds a token specified be a predicate
				in a sequence. It is equivalent of std::find algorithm,
				with an exception that it return range instead of a single
				iterator.

				If bCompress is set to true, adjacent matching tokens are 
				concatenated into one match.
			*/
			template< typename PredicateT >
			struct token_finderF
			{
                // Construction
                token_finderF( 
					PredicateT Pred, bool bCompress=true ) : 
						m_Pred(Pred), m_bCompress(bCompress) {}

                // Operation
				template< typename ForwardIteratorT >
                iterator_range<ForwardIteratorT>
				operator()( 
					ForwardIteratorT Begin, 
                    ForwardIteratorT End ) const
				{
					ForwardIteratorT It=std::find_if( Begin, End, m_Pred );
				
					if ( It==End )
					{
						return make_range( End, End );
					}
					else
					{
						ForwardIteratorT It2=It;

						if ( m_bCompress )
						{
							// Find first non-matching character
							while( m_Pred(*It2) && It2!=End ) It2++;
						}
						else
						{
							// Advance by one possition                         
							It2++;
						}

						return make_range( It, It2 );
					}
				}

        		// Container based find operator
				BOOST_STRING_DEFINE_FIND_OPERATOR()

			private:
				PredicateT m_Pred;
				bool m_bCompress;
			};

//  find range functor -----------------------------------------------//

            // find a range in the sequence ( functor )
            /*
                This functor actually does not perform any find operation.
                It always returns given iterator range as a result.
            */
            template<typename ForwardIteratorT>
			struct range_finderF
            {
                typedef ForwardIteratorT input_iterator_type;
                typedef iterator_range<input_iterator_type> result_type;

                // Construction
                range_finderF( 
                    input_iterator_type Begin, 
                    input_iterator_type End ) : m_Range(Begin, End) {}

                range_finderF(const iterator_range<input_iterator_type>& Range) : 
                    m_Range(Range) {}

                // Operation
                result_type operator()( 
                    input_iterator_type, 
                    input_iterator_type ) const
                {
                    return m_Range;
                }

				template< typename InputT> 
				result_type operator() ( InputT& Input )
				{
					// Concept check
					{
						input_iterator_type It=begin(Input);
					}

					return m_Range;
				}

            private:
                iterator_range<input_iterator_type> m_Range;
            };


        } // namespace detail
    } // namespace string_algo
} // namespace boost

#undef BOOST_STRING_DEFINE_FIND_OPERATOR

#endif  // BOOST_STRING_FIND_DETAIL_HPP
